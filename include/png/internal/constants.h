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
		None = 0,
		Sub,
		Up,
		Average,
		Paeth,
	};

	//
	inline constexpr uint8_t FILE_PREAMBULA[] { 137, 80, 78, 71, 13, 10, 26, 10 };

	//
	inline constexpr uint32_t TYPE_CODE_IHDR = 'RDHI';

	//
	inline constexpr uint32_t TYPE_CODE_PLTE = 'ETLP';

	//
	inline constexpr uint32_t TYPE_CODE_IDAT = 'TADI';

	//
	inline constexpr uint32_t TYPE_CODE_IEND = 'DNEI';
}
}
}
}
