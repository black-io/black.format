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
