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


	const Black::BooleanStatus DirectColorConverter::PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const
	{
		uint32_t converted_color = color;

		if( !color_format.has_alpha && GetOutputFormat().has_alpha )
		{
			converted_color = converted_color | GetOutputAlphaMask();
		}

		return GetOutputBuilder().ProduceElement( converted_color );
	}
}
}
}
}
