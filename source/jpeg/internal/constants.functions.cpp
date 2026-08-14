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

	const MarkerCode INVALID_CODE_1	= MarkerCode( 0x00U );
	const MarkerCode INVALID_CODE_2	= MarkerCode( 0xFFU );
}


namespace
{
	const bool IsMarkerCodePrivate( const MarkerCode candidate )
	{
		return candidate == MarkerCode( 0x01U );
	}

	const bool IsMarkerCodeReserved( const MarkerCode candidate )
	{
		return ( candidate >= MarkerCode( 0x02U ) ) && ( candidate <= MarkerCode( 0xBFU ) );
	}
}


	const bool IsMarkerCodeValid( const MarkerCode candidate )
	{
		CRET( candidate == INVALID_CODE_1, false );
		CRET( candidate == INVALID_CODE_2, false );

		return true;
	}
}
}
}
}
