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


	ColorConverter ColorConverter::SetupRgbToMonochromeConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupMonochromeToRgbConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupDirectConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupRemappingConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupTransformConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter::ColorConverter( ColorConverter&& other ) noexcept
	{
	}

	ColorConverter& ColorConverter::operator = ( ColorConverter&& other ) noexcept
	{
	}

	void ColorConverter::Swap( ColorConverter& other )
	{
	}

	const uint32_t ColorConverter::ConvertColor( const uint32_t color ) const
	{
	}
}
}
}
}
