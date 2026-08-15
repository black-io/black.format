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
