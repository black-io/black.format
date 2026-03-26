#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Internal
{
	/**
		@brief	Perform the valid formatting of TGA version.
		This function performs the formatting API for {fmt} library.

		@param	value	Given version to be formatted.
		@retrn			The value returned is name of given version.
	*/
	std::string_view format_as( const Version value );

	/**
		@brief	Perform the valid formatting of TGA content type.
		This function performs the formatting API for {fmt} library.

		@param	value	Given content type to be formatted.
		@retrn			The value returned is name of given content type.
	*/
	std::string_view format_as( const ContentType value );

	/**
		@brief	Perform the valid formatting of TGA bit-rate.
		This function performs the formatting API for {fmt} library.

		@param	value	Given bit-rate to be formatted.
		@retrn			The value returned is name of given bit-rate.
	*/
	std::string_view format_as( const Bitrate value );

	/**
		@brief	Perform the valid formatting of TGA interlacing mode.
		This function performs the formatting API for {fmt} library.

		@param	value	Given interlacing mode to be formatted.
		@retrn			The value returned is name of given mode.
	*/
	std::string_view format_as( const Interlacing value );

	/**
		@brief	Perform the valid formatting of TGA position of image origin.
		This function performs the formatting API for {fmt} library.

		@param	value	Given origin position to be formatted.
		@retrn			The value returned is name of given mode.
	*/
	std::string_view format_as( const OriginPosition value );
}
}
}
}
