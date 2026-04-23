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
		@brief	PNG file chunk type code.

		This enumeration described in section 4 (Chunk Specifications) of PNG 1.2 file format specification.
		Values of enumeration reflects all discussed in specification codes, but not the all possible codes of PNG file format.

		All chunks of PNG format are ancillary or critical. Critical chunks can be always found in any PNG file. Ancillary chunks are optional
		and may by missed in PNG file.
	*/
	enum class ChunkTypeCode : uint32_t
	{
		// Critical chunks.
		ImageHeader					= 'IHDR',	// (4.1.1) Image header.
		Palette						= 'PLTE',	// (4.1.2) Image palette.
		ImageData					= 'IDAT',	// (4.1.3) Image data.
		ImageEnd					= 'IEND',	// (4.1.4) End of image.
		// Ancillary chunks.
		Transparency				= 'tRNS',	// (4.2.1) Transparency.
		ImageGamma					= 'gAMA',	// (4.2.2.1) Image gamma.
		Chromaticities				= 'cHRM',	// (4.2.2.2) Primary chromaticities.
		SrgbColorSpace				= 'sRGB',	// (4.2.2.3) Standard RGB color space.
		EmbeddedIcc					= 'iCCP',	// (4.2.2.4) Embedded ICC profile.
		TextualData					= 'tEXt',	// (4.2.3.1) Textual data.
		CompressedTextualData		= 'zTXt',	// (4.2.3.2) Compressed textual data.
		InternationalTextualData	= 'iTXt',	// (4.2.3.3) International textual data.
		BackgroundColor				= 'bKGD',	// (4.2.4.1) Background color.
		PhysicalDimensions			= 'pHYs',	// (4.2.4.2) Physical pixel dimensions.
		SignificantBits				= 'sBIT',	// (4.2.4.3) Significant bits.
		SuggestedPalette			= 'sPLT',	// (4.2.4.4) Suggested palette.
		PaletteHistogram			= 'hIST',	// (4.2.4.5) Palette histogram.
		LastModificationTime		= 'tIME',	// (4.2.4.6) Image last-modification time.
	};

	/**
		@brief	PNG file chunk options.

		This enumeration described in section 3.3 (Chunk naming conventions) of PNG 1.2 file format specification.
		Each flag of this enumeration can be used as mask for value of `ChunkTypeCode` to check required option in type code.

		There are bitwise `&` operations defined for operands `ChunkTypeCode` and `ChunkTypeFlag` that will return wither `0` or value of `ChunkTypeFlag` operand.
	*/
	enum class ChunkTypeFlag : uint32_t
	{
		None		= 0,			// None property.
		Ancillary	= 0x00000020U,	// Ancillary bit: bit 5 of first byte.
		Private		= 0x00002000U,	// Private bit: bit 5 of second byte.
		Reserved	= 0x00200000U,	// Reserved bit: bit 5 of third byte.
		CopySafe	= 0x20000000U,	// Safe-to-copy bit: bit 5 of fourth byte.
	};

	//
	enum class BitDepth : uint8_t
	{
		Undefined	= 0,
		Bit			= 1,
		QuarterByte	= 2,
		HalfByte	= 4,
		Byte		= 8,
		Word		= 16,
	};

	//
	enum class ColorType : uint8_t
	{
		Grayscale		= 0,
		RGB				= 2,
		Paletted		= 3,
		GrayscaleAlpha	= 4,
		ARGB			= 6,
	};

	//
	enum class ColorTypeFlag : uint8_t
	{
		None		= 0,
		Palette		= 0x01U,
		TrueColor	= 0x02U,
		Alpha		= 0x04U,
	};

	//
	enum class CompressionMethod : uint8_t
	{
		Deflate = 0,
	};

	//
	enum class FilterMethod : uint8_t
	{
		None	= 0,
		Sub,
		Up,
		Average,
		Paeth,
	};

	//
	enum class InterlaceMethod : uint8_t
	{
		Disabled	= 0,
		Adam7,
	};

	//
	inline constexpr uint8_t FILE_PREAMBULA[] { 137, 80, 78, 71, 13, 10, 26, 10 };
}
}
}
}
