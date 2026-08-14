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
	inline constexpr MarkerCode MIN_CODE		= MarkerCode::Sof0;
}


	const bool IsMarkerCodeValid( const MarkerCode candidate )
	{
		CRET( candidate == INVALID_CODE_1, false );
		CRET( candidate == INVALID_CODE_2, false );
		CRET( candidate < MIN_CODE, false );

		return true;
	}
}
}
}
}
