#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
	/**
		@brief	Whether the given candidate may be considered as valid marker code.

		Valid code of JIF marker is:
		- not `0x00`,
		- not `0xFF`,
		- not private (aka 0x01),
		- not reserved (aka 0x02..0x0BF).

		@param	candidate	Given code to be checked.
		@return				`true` for valid code of JIF marker and `false` in other way.
	*/
	const bool IsMarkerCodeValid( const MarkerCode candidate );
}
}
}
}
