#include <black/format/jpeg.h>

#include "MarkerStats.h"


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/JPEG/MarkerStats";
}


	MarkerStats::MarkerStats( Black::PlainView<const std::byte> file_memory ) noexcept
	{
	}
}
}
}
