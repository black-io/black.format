#include <black/format/tga.h>

#include "decoder/decoder.h"


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/ImageDecoder";
}


namespace
{
	const Black::BooleanStatus SetupInputFeeder(
		Decoder::InputFeeder& feeder,
		const Internal::Header& header,
		const Black::PlainView<const std::byte>& image_buffer
	)
	{
		switch( Internal::ClassifyContentCompression( header.content_type ) )
		{
		case Internal::ContentCompression::None:
			feeder = Decoder::InputFeeder::SetupStraightFeeder( image_buffer, header.image.bitrate );

			return Black::BooleanStatus::Success;
		case Internal::ContentCompression::Rle:
			feeder = Decoder::InputFeeder::SetupRleFeeder( image_buffer, header.image.bitrate );

			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine compression of image input buffer." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus SetupColorMapper(
		Decoder::ColorMapper& mapper,
		const Internal::Header& header,
		const Black::PlainView<const std::byte>& palette_buffer
	)
	{
		switch( Internal::GetContentTypeBehindCompression( header.content_type ) )
		{
		case Internal::ContentType::Paletted:
			mapper = Decoder::ColorMapper::SetupPaletteMapper( header, palette_buffer );

			return Black::BooleanStatus::Success;
		case Internal::ContentType::TrueColor:
			mapper = Decoder::ColorMapper::SetupDirectMapper( header );

			return Black::BooleanStatus::Success;
		case Internal::ContentType::Grayscale:
			mapper = Decoder::ColorMapper::SetupDirectMapper( header );

			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine real content type of image input buffer." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus SetupColorConverter( Decoder::ColorConverter& converter, const Internal::Header& header, const Black::ColorFormat output_format )
	{
		// Prepare for comparison.
		Black::ColorFormat out_format	= output_format;
		Black::ColorFormat in_format	= Internal::SelectColorFormat(
			header.content_type,
			header.palette.bitrate,
			header.image.bitrate,
			header.image.flags.alpha_length
		);

		// First of all, the monochrome-to-rgb color converter.
		if( in_format.is_monochrome && !out_format.is_monochrome )
		{
			converter = Decoder::ColorConverter::SetupMonochromeToRgbConverter( header, output_format );

			return Black::BooleanStatus::Success;
		}

		// Alpha settings are irrelevant for this comparison.
		out_format.size_bits			= 0;
		out_format.size_bytes			= 0;
		out_format.alpha_channel_bits	= 0;
		out_format.alpha_channel_index	= 0;
		out_format.has_alpha			= false;
		in_format.size_bits				= 0;
		in_format.size_bytes			= 0;
		in_format.alpha_channel_bits	= 0;
		in_format.alpha_channel_index	= 0;
		in_format.has_alpha				= false;

		if( in_format == out_format )
		{
			converter = Decoder::ColorConverter::SetupDirectConverter( header, output_format );

			return Black::BooleanStatus::Success;
		}

		// Now compare it without red-blue channel positions.
		out_format.red_channel_index	= 0;
		out_format.blue_channel_index	= 0;
		in_format.red_channel_index		= 0;
		in_format.blue_channel_index	= 0;

		if( in_format == out_format )
		{
			converter = Decoder::ColorConverter::SetupRemappingConverter( header, output_format );

			return Black::BooleanStatus::Success;
		}

		// So, setup the most slow and most comprehensive converter.
		converter = Decoder::ColorConverter::SetupTransformConverter( header, output_format );
		return BooleanStatus::Success;
	}

	const Black::BooleanStatus SetupOutputBuilder(
		Decoder::OutputBuilder& builder,
		const Black::PlainView<std::byte>& image_buffer,
		const Black::ColorFormat output_format
	)
	{
		builder = {};
		builder.UseOutputBuffer( image_buffer );
		builder.UseOutputFormat( output_format );

		return BooleanStatus::Success;
	}
}


	TgaImageDecoder::TgaImageDecoder( const TgaStructure::Header& input_header )
		: m_input_header{ input_header }
	{
	}

	void TgaImageDecoder::ClearOutputBuffer()
	{
		m_output_buffer.Invalidate();
	}

	void TgaImageDecoder::Swap( TgaImageDecoder& other )
	{
		Black::Swap( m_input_header, other.m_input_header );
		Black::Swap( m_output_buffer, other.m_output_buffer );
		Black::Swap( m_output_width, other.m_output_width );
		Black::Swap( m_output_height, other.m_output_height );
		Black::Swap( m_output_row_size, other.m_output_row_size );
		Black::Swap( m_output_format, other.m_output_format );
	}

	Black::BooleanStatus TgaImageDecoder::Decode(
		const Black::PlainView<const std::byte>& image_buffer,
		const Black::PlainView<const std::byte>& palette_buffer
	)
	{
		CRETE( !TgaFileView::IsHeaderValid( m_input_header ), Black::BooleanStatus::Failure, LOG_CHANNEL, "Given TGA header is invalid." );
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Content type form header: {}.", m_input_header.content_type );

		m_output_width	= std::max<size_t>( m_output_width, m_input_header.image.width );
		m_output_height	= std::max<size_t>( m_output_height, m_input_header.image.height );
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Output image properties: {}x{} {}Bpp.", m_output_width, m_output_height, uint16_t( m_output_format.size_bits ) );

		m_output_row_size = m_output_width * m_output_format.size_bytes;
		m_output_buffer.SetLength( m_output_row_size * m_output_height );
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Output image size: {}B.", m_output_buffer.GetLength() );

		// Discard the result in case of any error while decoding process.
		Black::ScopeLeaveHandler reset_contract{ Black::BindMethod<&TgaImageDecoder::ClearOutputBuffer>( *this ) };

		Decoder::InputFeeder	input_feeder;
		Decoder::ColorMapper	color_mapper;
		Decoder::ColorConverter	color_converter;
		Decoder::OutputBuilder	output_builder;
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Pipeline configuration started." );
		{
			Black::BooleanStatus status = SetupInputFeeder( input_feeder, m_input_header, image_buffer );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup image buffer for decoding." );

			status = SetupColorMapper( color_mapper, m_input_header, palette_buffer );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup image color mapper for decoding." );

			status = SetupColorConverter( color_converter, m_input_header, m_output_format );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup output color converter for decoding." );

			status = SetupOutputBuilder( output_builder, m_output_buffer, m_output_format );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup output color converter for decoding." );
		}
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Pipeline configuration finished." );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Decode started." );
		{
			Decoder::CoordinateCursor cursor{ m_input_header, m_output_width, m_output_height };

			cursor.UseInputFeeder( input_feeder );
			output_builder.UseCursor( cursor );

			for( cursor.Rewind(); !cursor.IsFinished(); cursor.StepForward() )
			{
				// Pull the color from input.
				const std::byte* const color_buffer = input_feeder.PeekColorBuffer();
				const uint32_t color = color_mapper.MapColor( color_buffer );

				// Push the color to output.
				const uint32_t converted_color		= color_converter.ConvertColor( color );
				const Black::BooleanStatus status	= output_builder.ProduceElement( converted_color );
				CRETE( Black::IsFailed( status ), Black::BooleanStatus::Failure, LOG_CHANNEL, "Pipeline failed processing at position {}.", cursor );
			}

			output_builder.RefuseCursor();
		}
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Decode finished." );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Decode finished successfully." );
		reset_contract.Cancel();
		return Black::BooleanStatus::Success;
	}

	TgaImageDecoder& TgaImageDecoder::SetOutputResolution( const size_t width, const size_t height )
	{
		m_output_width		= width;
		m_output_height		= height;
		m_output_row_size	= m_output_width * m_output_format.size_bytes;
		return *this;
	}

	TgaImageDecoder& TgaImageDecoder::SetOutputFormat( const Black::ColorFormat format )
	{
		m_output_format		= format;
		m_output_row_size	= m_output_width * m_output_format.size_bytes;
		return *this;
	}
}
}
}
