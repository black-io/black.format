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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/ColorConverter";
}


	const Black::BooleanStatus MonochromeColorConverter::ConvertFromMonochrome( const uint32_t color, const Black::ImageFormat color_format ) const
	{
	}

	const Black::BooleanStatus MonochromeColorConverter::ConvertToMonochrome( const uint32_t color, const Black::ImageFormat color_format ) const
	{
	}

	const Black::BooleanStatus MonochromeColorConverter::PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const
	{
		CRET( color_format.is_monochrome, ConvertFromMonochrome( color, color_format ) );
		return ConvertToMonochrome( color, color_format );
	}
}
}
}
}
