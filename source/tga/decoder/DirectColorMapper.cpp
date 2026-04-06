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


	const uint32_t DirectColorMapper::PerformPeekElement() const
	{
		uint32_t result = 0;

		Black::CopyMemory( &result, GetInputFeeder().PeekElement(), GetInputFormat().size_bytes );

		return result;
	}
}
}
}
}
