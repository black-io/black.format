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

	/**
		@brief	Whether the given compression method is valid.

		This check complies to section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.
		Compression method is a single-byte integer that indicates the method used to compress the image data.
		At present, only compression method 0 (deflate/inflate compression with a sliding window of at most 32768 bytes) is defined.
		All standard PNG images must be compressed with this scheme.
		The compression method field is provided for possible future expansion or proprietary variants.
		Decoders must check this byte and report an error if it holds an unrecognized code.

		@param	compression_method	Given method type to be checked.
		@return						`true` for valid combination. `false` in other cases.
	*/
	const bool IsCompressionMethodValid( const CompressionMethod compression_method );

	/**
		@brief	Whether the given filter method is valid.

		This check complies to section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.
		Filter method is a single-byte integer that indicates the preprocessing method applied to the image data before compression.
		At present, only filter method 0 (adaptive filtering with five basic filter types) is defined.
		As with the compression method field, decoders must check this byte and report an error if it holds an unrecognized code.

		@param	filter_method	Given method type to be checked.
		@return					`true` for valid combination. `false` in other cases.
	*/
	const bool IsFilterMethodValid( const FilterMethod filter_method );

	/**
		@brief	Whether the given interlace methods is valid.

		This check complies to section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.
		Interlace method is a single-byte integer that indicates the transmission order of the image data.
		Two values are currently defined:
		- 0 (no interlace),
		- 1 (Adam7 interlace).

		@param	interlace_method	Given method type to be checked.
		@return						`true` for valid combination. `false` in other cases.
	*/
	const bool IsInterlaceMethodValid( const InterlaceMethod interlace_method );
}
}
}
}
