#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
	/**
		@brief	Perform the valid formatting of PNG chunk type code.
		This function performs the formatting API for {fmt} library.

		@param	value	Given code to be formatted.
		@retrn			The value returned is name of given code.
	*/
	std::string_view format_as( const ChunkTypeCode value );

	/**
		@brief	Perform the valid formatting of PNG chunk type flag.
		This function performs the formatting API for {fmt} library.

		@param	value	Given flag to be formatted.
		@retrn			The value returned is name of given flag.
	*/
	std::string_view format_as( const ChunkTypeFlag value );

	/**
		@brief	Perform the valid formatting of PNG bit depth.
		This function performs the formatting API for {fmt} library.

		@param	value	Given bit depth to be formatted.
		@retrn			The value returned is name of given bit depth.
	*/
	std::string_view format_as( const BitDepth value );

	/**
		@brief	Perform the valid formatting of PNG color type.
		This function performs the formatting API for {fmt} library.

		@param	value	Given color type to be formatted.
		@retrn			The value returned is name of given color type.
	*/
	std::string_view format_as( const ColorType value );

	/**
		@brief	Perform the valid formatting of PNG color type flag.
		This function performs the formatting API for {fmt} library.

		@param	value	Given flag to be formatted.
		@retrn			The value returned is name of given flag.
	*/
	std::string_view format_as( const ColorTypeFlag value );

	/**
		@brief	Perform the valid formatting of PNG compression method.
		This function performs the formatting API for {fmt} library.

		@param	value	Given compression to be formatted.
		@retrn			The value returned is name of given compression.
	*/
	std::string_view format_as( const CompressionMethod value );

	/**
		@brief	Perform the valid formatting of PNG filter method.
		This function performs the formatting API for {fmt} library.

		@param	value	Given filter to be formatted.
		@retrn			The value returned is name of given filter.
	*/
	std::string_view format_as( const FilterMethod value );


	inline const ChunkTypeFlag operator & ( const ChunkTypeCode type_code, const ChunkTypeFlag type_flag );
	inline const ColorTypeFlag operator & ( const ColorType color_type, const ColorTypeFlag type_flag );
}
}
}
}
