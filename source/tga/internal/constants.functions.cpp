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
		return ContentType{ value & PURE_CONTENT_MASK };
	}
}
}
}
}
