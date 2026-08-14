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


namespace
{
	const bool IsMarkerCodeInvalid( const MarkerCode candidate )
	{
		return ( candidate == MarkerCode( 0x00U ) ) || ( candidate == MarkerCode( 0xFFU ) );
	}

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
		CRET( candidate == MarkerCode( 0x00U ), false );
		CRET( candidate == MarkerCode( 0xFFU ), false );

		return true;
	}
}
}
}
}
