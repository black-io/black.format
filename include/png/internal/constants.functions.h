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
		@brief	Whether the given bit depth is valid.

		This check complies to section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.
		Bit depth is a single-byte integer giving the number of bits per sample or per palette index (not per pixel).
		Valid values are 1, 2, 4, 8, and 16, although not all values are allowed for all color types.

		@param	bit_depth	Given depth to be checked.
		@return				`true` for valid values. `false` in other cases.
	*/
	const bool IsBitDepthValid( const BitDepth bit_depth );

	/**
		@brief	Whether the given color type is valid.

		This check complies to section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.
		Color type is a single-byte integer that describes the interpretation of the image data.
		Color type codes represent sums of the following values:
		- 1 (palette used),
		- 2 (color used),
		- 4 (alpha channel used).
		Valid values are 0, 2, 3, 4, and 6.

		@param	color_type	Given color type to be checked.
		@return				`true` for valid values. `false` in other cases.
	*/
	const bool IsColorTypeValid( const ColorType color_type );

	//
	const bool HasValidColorCombination( const BitDepth bit_depth, const ColorType color_type );

	//
	const bool IsCompressionMethodValid( const CompressionMethod compression_method );

	//
	const bool IsFilterMethodValid( const FilterMethod filter_method );

	//
	const bool IsInterlaceMethodValid( const InterlaceMethod interlace_method );
}
}
}
}
