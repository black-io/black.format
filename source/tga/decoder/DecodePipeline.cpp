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
				Decoder::StraightInputFeeder& feeder = ConstructComponent<Decoder::StraightInputFeeder>();
				feeder.UseImageBuffer( image_buffer );
				feeder.UseBitrate( header.image.bitrate );
				m_input_feeder = &feeder;
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentCompression::Rle:
			{
				Decoder::RleInputFeeder& feeder = ConstructComponent<Decoder::RleInputFeeder>();
				feeder.UseImageBuffer( image_buffer );
				feeder.UseBitrate( header.image.bitrate );
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
				Decoder::PaletteColorMapper& mapper = ConstructComponent<Decoder::PaletteColorMapper>();
				mapper.UseImageSettings( header );
				mapper.UsePalette( palette_buffer, header.palette.bitrate );
				m_color_mapper = &mapper;
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentType::TrueColor:
			{
				Decoder::TrueColorMapper& mapper = ConstructComponent<Decoder::TrueColorMapper>();
				mapper.UseImageSettings( header );
				m_color_mapper = &mapper;
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentType::Grayscale:
			{
				Decoder::MonochromeColorMapper& mapper = ConstructComponent<Decoder::MonochromeColorMapper>();
				mapper.UseImageSettings( header );
				mapper.FixOutputFormat( header );
				m_color_mapper = &mapper;
			}
			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine real content type of image input buffer." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus DecodePipeline::SetupColorConverter( const Internal::Header& header, const Black::ImageFormat output_format )
	{
		BLACK_LOG_WARNING( LOG_CHANNEL, "Not implemented." );
		return BooleanStatus::Success;
	}

	const Black::BooleanStatus DecodePipeline::SetupOutputBuilder( const Black::PlainView<std::byte>& image_buffer, const Black::ImageFormat output_format )
	{
		BLACK_LOG_WARNING( LOG_CHANNEL, "Not implemented." );
		return BooleanStatus::Success;
	}

	const Black::BooleanStatus DecodePipeline::BeginProcessing( CoordinateCursor& image_cursor )
	{
		CRETE( m_input_feeder == nullptr, Black::BooleanStatus::Failure, LOG_CHANNEL, "Input feeder does not configured." );
		CRETE( m_color_mapper == nullptr, Black::BooleanStatus::Failure, LOG_CHANNEL, "Input color mapper does not configured." );

		image_cursor.SetInputFeeder( *m_input_feeder );
		m_color_mapper->UseInputFeeder( *m_input_feeder );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Image processing began." );
		return BooleanStatus::Success;
	}

	const Black::BooleanStatus DecodePipeline::Process()
	{
		EXPECTS_DEBUG( m_color_mapper != nullptr );
		const uint32_t color = m_color_mapper->PeekElement();

		//EXPECTS_DEBUG( m_color_converter != nullptr );
		//m_color_converter->Consumer( color );

		return BooleanStatus::Success;
	}

	const Black::BooleanStatus DecodePipeline::EndProcessing( CoordinateCursor& image_cursor )
	{
		image_cursor.ResetInputFeeder();

		if( m_color_mapper != nullptr )
		{
			m_color_mapper->RefuseInputFeeder();
		}

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Image processing ended." );
		return BooleanStatus::Success;
	}
}
}
}
}
