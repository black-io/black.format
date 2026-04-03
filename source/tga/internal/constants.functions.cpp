#include <black/format/tga.h>

#include <black/core/algorithms.h>


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Internal
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/Functions";

	// `ContentType` bits mask, that indicates about RLE compression.
	constexpr uint8_t RLE_COMPRESSION_MASK = 0x08UL;

	// `ContentType` bits mask, that marks the bits of content type behind any compression.
	constexpr uint8_t PURE_CONTENT_MASK = 0x07UL;
}


	const bool IsContentTypeValid( const ContentType content_type )
	{
		static constexpr Internal::ContentType allowed_types[] {
			Internal::ContentType::Empty,
			Internal::ContentType::Paletted,
			Internal::ContentType::TrueColor,
			Internal::ContentType::Grayscale,
			Internal::ContentType::RlePaletted,
			Internal::ContentType::RleTrueColor,
			Internal::ContentType::RleGrayscale,
		};

		return Black::HasItem( allowed_types, content_type );
	}

	const bool IsBitrateValid( const Bitrate bitrate )
	{
		static constexpr Internal::Bitrate allowed_bitrates[] {
			Internal::Bitrate::Monochrome,
			Internal::Bitrate::ARGB16,
			Internal::Bitrate::RGB24,
			Internal::Bitrate::ARGB32,
		};

		return Black::HasItem( allowed_bitrates, bitrate );
	}

	const bool IsContentCompressed( const ContentType content_type )
	{
		const uint8_t value = Black::GetEnumValue( content_type );
		return ( value & RLE_COMPRESSION_MASK ) != 0;
	}

	const ContentCompression ClassifyContentCompression( const ContentType content_type )
	{
		const uint8_t value = Black::GetEnumValue( content_type );
		CRET( ( value & RLE_COMPRESSION_MASK ) != 0, ContentCompression::Rle );

		return ContentCompression::None;
	}

	const ContentType GetContentTypeBehindCompression( const ContentType content_type )
	{
		const uint8_t value = Black::GetEnumValue( content_type );
		return ContentType( value & PURE_CONTENT_MASK );
	}

	const size_t GetElementSize( const Bitrate bitrate )
	{
		switch( bitrate )
		{
		case Internal::Bitrate::Monochrome:
			return 1;
		case Internal::Bitrate::ARGB16:
			return 2;
		case Internal::Bitrate::RGB24:
			return 3;
		case Internal::Bitrate::ARGB32:
			return 4;
		default:
			break;
		}

		return 0;
	}

	const Black::ImageFormat SelectImageFormat( const ContentType content_type, const Bitrate bitrate, const size_t alpha_bits_count )
	{
		switch( GetContentTypeBehindCompression( content_type ) )
		{
		case ContentType::Paletted:
			switch( bitrate )
			{
			case Bitrate::Monochrome:
				return Black::ImageFormats::I8;
			case Bitrate::ARGB16:
				CRET( alpha_bits_count == 0, Black::ImageFormats::I16 );
				CRET( alpha_bits_count == 8, Black::ImageFormats::A8I8 );
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported value of alpha channel length - {}.", alpha_bits_count );
				break;
			default:
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported bit-rate of palette index - {}.", bitrate );
				break;
			}
			break;
		case ContentType::TrueColor:
			switch( bitrate )
			{
			case Bitrate::ARGB16:
				CRET( alpha_bits_count == 0, Black::ImageFormats::X1R5G5B5 );
				CRET( alpha_bits_count == 1, Black::ImageFormats::A1R5G5B5 );
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported value of alpha channel length - {}.", alpha_bits_count );
				break;
			case Bitrate::RGB24:
				return Black::ImageFormats::R8G8B8;
			case Bitrate::ARGB32:
				CRET( alpha_bits_count == 0, Black::ImageFormats::X8R8G8B8 );
				CRET( alpha_bits_count == 8, Black::ImageFormats::A8R8G8B8 );
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported value of alpha channel length - {}.", alpha_bits_count );
				break;
			default:
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported bit-rate of True Color image - {}.", bitrate );
				break;
			}
			break;
		case ContentType::Grayscale:
			switch( bitrate )
			{
			case Bitrate::Monochrome:
				return Black::ImageFormats::W8;
			case Bitrate::ARGB16:
				CRET( alpha_bits_count == 0, Black::ImageFormats::W16 );
				CRET( alpha_bits_count == 8, Black::ImageFormats::A8W8 );
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported value of alpha channel length - {}.", alpha_bits_count );
				break;
			default:
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported bit-rate of Monochrome image - {}.", bitrate );
				break;
			}
			break;
		default:
			BLACK_LOG_ERROR( LOG_CHANNEL, "Unsupported type of image content - {}.", content_type );
			break;
		}

		BLACK_LOG_ERROR(
			LOG_CHANNEL,
			"Unknown combination of content type ({}), bit-rate ({}) and alpha-channel ({}).",
			Black::GetEnumValue( content_type ),
			Black::GetEnumValue( bitrate ),
			alpha_bits_count
		);

		return Black::ImageFormats::UNDEFINED;
	}
}
}
}
}
