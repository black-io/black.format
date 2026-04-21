#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
	//
	enum class ChunkTypeCode : uint32_t
	{
		ImageHeader					= 'IHDR',
		Palette						= 'PLTE',
		ImageData					= 'IDAT',
		ImageEnd					= 'IEND',
		Transparency				= 'tRNS',
		ImageGamma					= 'gAMA',
		Chromaticities				= 'cHRM',
		SrgbColorSpace				= 'sRGB',
		EmbeddedIcc					= 'iCCP',
		TextualData					= 'tEXt',
		CompressedTextualData		= 'zTXt',
		InternationalTextualData	= 'iTXt',
		BackgroundColor				= 'bKGD',
		PhysicalDimensions			= 'pHYs',
		SignificantBits				= 'sBIT',
		SuggestedPalette			= 'sPLT',
		PaletteHistogram			= 'hIST',
		LastModificationTime		= 'tIME',
	};

	//
	enum class ChunkTypeFlag : uint32_t
	{
		None		= 0,
		Ancillary	= 0x00000020U,
		Private		= 0x00002000U,
		Reserved	= 0x00200000U,
		CopySafe	= 0x20000000U,
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
