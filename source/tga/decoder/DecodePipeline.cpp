#include "decoder.h"


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Decoder
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/Pipeline";
}


	const Black::BooleanStatus DecodePipeline::SetupInputFeeder( const Internal::Header& header, const Black::PlainView<const std::byte>& image_buffer )
	{
		switch( Internal::ClassifyContentCompression( header.content_type ) )
		{
		case Internal::ContentCompression::None:
			{
				InputFeeder& feeder = ConstructComponent<InputFeeder>();
				feeder = InputFeeder::SetupStraightFeeder( image_buffer, header.image.bitrate );
				m_input_feeder = &feeder;
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentCompression::Rle:
			{
				InputFeeder& feeder = ConstructComponent<InputFeeder>();
				feeder = InputFeeder::SetupRleFeeder( image_buffer, header.image.bitrate );
				m_input_feeder = &feeder;
			}
			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine compression of image input buffer." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus DecodePipeline::SetupColorMapper( const Internal::Header& header, const Black::PlainView<const std::byte>& palette_buffer )
	{
		switch( Internal::GetContentTypeBehindCompression( header.content_type ) )
		{
		case Internal::ContentType::Paletted:
			{
				ColorMapper& mapper = ConstructComponent<ColorMapper>();
				mapper = ColorMapper::SetupPaletteMapper( header, palette_buffer );
				m_color_mapper = &mapper;
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentType::TrueColor:
			{
				ColorMapper& mapper = ConstructComponent<ColorMapper>();
				mapper = ColorMapper::SetupDirectMapper( header );
				m_color_mapper = &mapper;
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentType::Grayscale:
			{
				ColorMapper& mapper = ConstructComponent<ColorMapper>();
				mapper = ColorMapper::SetupDirectMapper( header );
				m_color_mapper = &mapper;
			}
			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine real content type of image input buffer." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus DecodePipeline::SetupColorConverter( const Internal::Header& header, const Black::ColorFormat output_format )
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
			MonochromeColorConverter& converter = ConstructComponent<MonochromeColorConverter>();
			converter.UseOutputFormat( output_format );
			m_color_converter = &converter;

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
			DirectColorConverter& converter = ConstructComponent<DirectColorConverter>();
			converter.UseOutputFormat( output_format );
			m_color_converter = &converter;

			return Black::BooleanStatus::Success;
		}

		// Now compare it without red-blue channel positions.
		out_format.red_channel_index	= 0;
		out_format.blue_channel_index	= 0;
		in_format.red_channel_index		= 0;
		in_format.blue_channel_index	= 0;

		if( in_format == out_format )
		{
			//RemappingColorConverter& converter = ConstructComponent<RemappingColorConverter>();
			//converter.UseOutputFormat( output_format );
			//m_color_converter = &converter;
			//
			//return Black::BooleanStatus::Success;
		}

		//TransformColorConverter& converter = ConstructComponent<TransformColorConverter>();
		//converter.UseOutputFormat( output_format );
		//m_color_converter = &converter;
		//
		//return Black::BooleanStatus::Success;

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine color converting conditions for image." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus DecodePipeline::SetupOutputBuilder( const Black::PlainView<std::byte>& image_buffer, const Black::ColorFormat output_format )
	{
		OutputBuilder& builder = ConstructComponent<OutputBuilder>();
		builder.UseOutputBuffer( image_buffer );
		builder.UseOutputFormat( output_format );
		m_output_builder = &builder;

		return BooleanStatus::Success;
	}

	const Black::BooleanStatus DecodePipeline::BeginProcessing( CoordinateCursor& image_cursor )
	{
		CRETE( m_input_feeder == nullptr, Black::BooleanStatus::Failure, LOG_CHANNEL, "Input feeder does not configured." );
		CRETE( m_color_mapper == nullptr, Black::BooleanStatus::Failure, LOG_CHANNEL, "Input color mapper does not configured." );
		CRETE( m_color_converter == nullptr, Black::BooleanStatus::Failure, LOG_CHANNEL, "Output color converter does not configured." );
		CRETE( m_output_builder == nullptr, Black::BooleanStatus::Failure, LOG_CHANNEL, "Output image builder does not configured." );

		image_cursor.UseInputFeeder( *m_input_feeder );
		m_color_converter->UseOutputBuilder( *m_output_builder );
		m_output_builder->UseCursor( image_cursor );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Image processing began." );
		return BooleanStatus::Success;
	}

	const Black::BooleanStatus DecodePipeline::Process()
	{
		// Pull the color from input.
		const std::byte* const color_buffer = m_input_feeder->PeekElement();
		const uint32_t color = m_color_mapper->MapColor( color_buffer );

		// Push the color to output.
		EXPECTS_DEBUG( m_color_converter != nullptr );
		return m_color_converter->ConvertColor( color, m_color_mapper->GetOutputFormat() );
	}

	const Black::BooleanStatus DecodePipeline::EndProcessing( CoordinateCursor& image_cursor )
	{
		image_cursor.RefuseInputFeeder();

		m_input_feeder		= nullptr;
		m_color_mapper		= nullptr;
		m_color_converter	= nullptr;
		m_output_builder	= nullptr;

		DestructAllComponents();

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Image processing ended." );
		return BooleanStatus::Success;
	}
}
}
}
}
