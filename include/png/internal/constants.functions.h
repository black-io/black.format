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

	/**
		@brief	Whether the combination of color type and bit depth is valid.

		This check complies to section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.
		Bit depth restrictions for each color type are imposed to simplify implementations and to prohibit combinations that do not compress well.
		Decoders must support all valid combinations of bit depth and color type. The allowed combinations are:
			Color	Allowed		Interpretation
			Type	Bit Depths

			0		1,2,4,8,16	Each pixel is a grayscale sample.

			2		8,16		Each pixel is an R,G,B triple.

			3		1,2,4,8		Each pixel is a palette index;
								a PLTE chunk must appear.

			4		8,16		Each pixel is a grayscale sample,
								followed by an alpha sample.

			6		8,16		Each pixel is an R,G,B triple,
								followed by an alpha sample.

		@param	bit_depth	Given depth to be checked.
		@param	color_type	Given color type to be checked.
		@return				`true` for valid combination. `false` in other cases.
	*/
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
