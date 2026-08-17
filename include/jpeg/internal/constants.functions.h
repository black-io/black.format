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

	/**
		@brief	Whether the given candidate is valid density unit of JFIF format.

		@param	candidate	Given candidate to be checked.
		@return				`true` in case the candidate is valid density unit value.
	*/
	const bool IsDesityUnitValid( const DensityUnit candidate );

	/**
		@brief	Whether the given candidate is valid endianness value of TIFF format.

		Valid value is:
		- 0x4949U ('II'),
		- 0x4D4DU ('MM').

		@param	candidate	Given candidate to be checked.
		@return				`true` in case the candidate is valid value of TIFF endianness.
	*/
	const bool IsTiffEndiannessValid( const TiffEndianness candidate );

	/**
		@brief	Translate the TIFF format endianness into regaular endianness for framework.

		The function will translate only valid value of TIFF endianness. Behavior of function is undefined for invalid values.

		@param	endianness	Given endianness to be translated.
		@return				For ank valid TIFF endianness the value returned will be corresponded framework endianness.
	*/
	const Black::PlatformEndianness TranslateTiffEndianness( const TiffEndianness endianness );
}
}
}
}
