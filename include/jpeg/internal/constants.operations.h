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
		@brief	Perform the valid formatting of JIF marker code.
		This function performs the formatting API for {fmt} library.

		@param	value	Given code to be formatted.
		@retrn			The value returned is name of given code.
	*/
	std::string_view format_as( const MarkerCode value );

	/**
	*/
	std::string_view format_as( const DensityUnit value );

	/**
	*/
	std::string_view format_as( const TiffEndianness value );
}
}
}
}
