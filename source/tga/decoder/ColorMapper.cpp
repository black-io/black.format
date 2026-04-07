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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/ColorMapper";
}


	ColorMapper ColorMapper::SetupDirectMapper( const Internal::Header& header )
	{
	}

	ColorMapper ColorMapper::SetupPaletteMapper( const Internal::Header& header, Black::PlainView<const std::byte> palette_buffer )
	{
	}

	ColorMapper::ColorMapper( ColorMapper&& other ) noexcept
	{
	}

	ColorMapper& ColorMapper::operator=( ColorMapper&& other ) noexcept
	{
	}

	void ColorMapper::Swap( ColorMapper& other )
	{
	}

	const uint32_t ColorMapper::MapColor( const std::byte* color_buffer ) const
	{
	}

	const uint32_t ColorMapper::PeekPaletteElement( const size_t element_index ) const
	{
	}
}
}
}
}
