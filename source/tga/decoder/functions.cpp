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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder";
}


	const Black::BooleanStatus SetupInputFeeder( InputFeeder& feeder, const Internal::Header& header, const Black::PlainView<const std::byte>& image_buffer )
	{
		switch( Internal::ClassifyContentCompression( header.content_type ) )
		{
		case Internal::ContentCompression::None:
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the straight input feeder." );
			feeder = InputFeeder::BuildStraightFeeder( image_buffer, header.image.bitrate );

			return Black::BooleanStatus::Success;
		case Internal::ContentCompression::Rle:
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the RLE input feeder." );
			feeder = InputFeeder::BuildRleFeeder( image_buffer, header.image.bitrate );

			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine compression of image input buffer." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus SetupColorMapper( ColorMapper& mapper, const Internal::Header& header, const Black::PlainView<const std::byte>& palette_buffer )
	{
		switch( Internal::GetContentTypeBehindCompression( header.content_type ) )
		{
		case Internal::ContentType::Paletted:
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the palette color mapper." );
			mapper = ColorMapper::SetupPaletteMapper( header, palette_buffer );

			return Black::BooleanStatus::Success;
		case Internal::ContentType::TrueColor:
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the direct color mapper." );
			mapper = ColorMapper::SetupDirectMapper( header );

			return Black::BooleanStatus::Success;
		case Internal::ContentType::Grayscale:
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the direct color mapper." );
			mapper = ColorMapper::SetupDirectMapper( header );

			return Black::BooleanStatus::Success;
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Failed to determine real content type of image input buffer." );
		return BooleanStatus::Failure;
	}

	const Black::BooleanStatus SetupColorConverter( ColorConverter& converter, const Internal::Header& header, const Black::ColorFormat output_format )
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
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the W->RGB color converter." );
			converter = ColorConverter::SetupMonochromeToRgbConverter( header, output_format );

			return Black::BooleanStatus::Success;
		}
		else if( !in_format.is_monochrome && out_format.is_monochrome )
		{
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the RGB->W color converter." );
			converter = ColorConverter::SetupRgbToMonochromeConverter( header, output_format );

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
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the direct color converter." );
			converter = ColorConverter::SetupDirectConverter( header, output_format );

			return Black::BooleanStatus::Success;
		}

		// Now compare it without red-blue channel positions.
		out_format.red_channel_index	= 0;
		out_format.blue_channel_index	= 0;
		in_format.red_channel_index		= 0;
		in_format.blue_channel_index	= 0;

		if( in_format == out_format )
		{
			BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the channel-remap color converter." );
			converter = ColorConverter::SetupRemappingConverter( header, output_format );

			return Black::BooleanStatus::Success;
		}

		// So, setup the most slow and most comprehensive converter.
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the transforming color converter." );
		converter = ColorConverter::SetupTransformConverter( header, output_format );
		return BooleanStatus::Success;
	}

	const Black::BooleanStatus SetupOutputBuilder( OutputBuilder& builder, const Black::PlainView<std::byte>& image_buffer, const Black::ColorFormat output_format )
	{
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Configure the trivial output builder." );

		builder = OutputBuilder::SetupDirectFeeder( image_buffer, output_format );

		return BooleanStatus::Success;
	}
}
}
}
}
