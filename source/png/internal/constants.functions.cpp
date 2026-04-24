#include <black/format/png.h>

#include <black/core/algorithms.h>


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/PNG/Functions";
}


	const bool IsBitDepthValid( const BitDepth bit_depth )
	{
		constexpr BitDepth valid_values[] { BitDepth::Bit, BitDepth::QuarterByte, BitDepth::HalfByte, BitDepth::Byte, BitDepth::Word };
		return Black::HasItem( valid_values, bit_depth );
	}
}
}
}
}
