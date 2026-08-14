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

	inline constexpr MarkerCode INVALID_CODE_1	= MarkerCode( 0x00U );
	inline constexpr MarkerCode INVALID_CODE_2	= MarkerCode( 0xFFU );
}


}
}
}
}
