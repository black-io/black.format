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
	constexpr const char* LOG_CHANNEL = "Black/TGA/DecodePipeline";
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
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentCompression::Rle:
			{
				Decoder::RleInputFeeder& feeder = ConstructComponent<Decoder::RleInputFeeder>();
				feeder.UseImageBuffer( image_buffer );
				feeder.UseBitrate( header.image.bitrate );
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
				mapper.UsePalette( palette_buffer, header.palette.bitrate );
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentType::TrueColor:
			{
				Decoder::TrueColorMapper& mapper = ConstructComponent<Decoder::TrueColorMapper>();
			}
			return Black::BooleanStatus::Success;
		case Internal::ContentType::Grayscale:
			{
				Decoder::MonochromeColorMapper& mapper = ConstructComponent<Decoder::MonochromeColorMapper>();
			}
			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine real content type of image input buffer." );
		return BooleanStatus::Failure;
	}
}
}
}
}
