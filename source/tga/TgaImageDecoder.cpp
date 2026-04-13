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


	TgaImageDecoder::TgaImageDecoder( TgaImageDecoder&& other ) noexcept
		: m_input_header{ std::exchange( other.m_input_header, Internal::Header{} ) }
		, m_output_buffer{ std::move( other.m_output_buffer ) }
		, m_output_width{ std::exchange( other.m_output_width, 0 ) }
		, m_output_height{ std::exchange( other.m_output_height, 0 ) }
		, m_output_row_size{ std::exchange( other.m_output_row_size, 0 ) }
		, m_output_format{ std::exchange( other.m_output_format, Black::ColorFormats::UNDEFINED ) }
	{
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
			Black::BooleanStatus status = Decoder::SetupInputFeeder( input_feeder, m_input_header, image_buffer );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup image buffer for decoding." );

			status = Decoder::SetupColorMapper( color_mapper, m_input_header, palette_buffer );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup image color mapper for decoding." );

			status = Decoder::SetupColorConverter( color_converter, m_input_header, m_output_format );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup output color converter for decoding." );

			status = Decoder::SetupOutputBuilder( output_builder, m_output_buffer, m_output_format );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup output color converter for decoding." );
		}
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Pipeline configuration finished." );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Decode started." );
		{
			Decoder::CoordinateCursor cursor{ m_input_header, m_output_width, m_output_height };

			cursor.UseInputFeeder( input_feeder );
			for( cursor.Rewind(); !cursor.IsFinished(); cursor.StepForward() )
			{
				// Pull the color from input.
				const std::byte* const color_buffer	= input_feeder.PeekColorBuffer();
				const uint32_t color				= color_mapper.MapColor( color_buffer );

				// Push the color to output.
				const uint32_t converted_color = color_converter.ConvertColor( color );
				output_builder.ProduceElement( converted_color, cursor );
			}
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
