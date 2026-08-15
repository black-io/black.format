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
		@brief	Perform the valid formatting of JFIF density unit.
		This function performs the formatting API for {fmt} library.

		@param	value	Given unit to be formatted.
		@retrn			The value returned is name of given unit.
	*/
	std::string_view format_as( const DensityUnit value );

	/**
		@brief	Perform the valid formatting of TIFF format endianness.
		This function performs the formatting API for {fmt} library.

		@param	value	Given endianness to be formatted.
		@retrn			The value returned is name of given endianness.
	*/
	std::string_view format_as( const TiffEndianness value );
}
}
}
}
