#include <black/format/jpeg.h>


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/JPEG/Functions";
}


	const bool IsMarkerValid( const Marker& marker )
	{
		CRET( marker.prefix != MARKER_PREFIX, false );
		return IsMarkerCodeValid( marker.code );
	}
}
}
}
}
