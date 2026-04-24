#include <black/format/png.h>

#include <black/core/algorithms.h>


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/PNG/Functions";
}


	const bool IsBitDepthValid( const BitDepth bit_depth )
	{
		constexpr BitDepth valid_values[] { BitDepth::Bit, BitDepth::QuarterByte, BitDepth::HalfByte, BitDepth::Byte, BitDepth::Word };
		return Black::HasItem( valid_values, bit_depth );
	}

	const bool IsColorTypeValid( const ColorType color_type )
	{
		constexpr ColorType valid_values[] { ColorType::Grayscale, ColorType::RGB, ColorType::Paletted, ColorType::GrayscaleAlpha, ColorType::ARGB };
		return Black::HasItem( valid_values, color_type );
	}

	const bool HasValidColorCombination( const BitDepth bit_depth, const ColorType color_type )
	{
		constexpr std::pair<ColorType, BitDepth> valid_values[] {
			{ ColorType::Grayscale, BitDepth::Bit },		{ ColorType::Grayscale, BitDepth::QuarterByte },	{ ColorType::Grayscale, BitDepth::HalfByte },
			{ ColorType::Grayscale, BitDepth::Byte },		{ ColorType::Grayscale, BitDepth::Word },

			{ ColorType::RGB, BitDepth::Byte },				{ ColorType::RGB, BitDepth::Word },

			{ ColorType::Paletted, BitDepth::Bit },			{ ColorType::Paletted, BitDepth::QuarterByte },		{ ColorType::Paletted, BitDepth::HalfByte },
			{ ColorType::Paletted, BitDepth::Byte },

			{ ColorType::GrayscaleAlpha, BitDepth::Byte },	{ ColorType::GrayscaleAlpha, BitDepth::Word },

			{ ColorType::ARGB, BitDepth::Byte },			{ ColorType::ARGB, BitDepth::Word },
		};

		return Black::HasItem( valid_values, { color_type, bit_depth } );
	}

	const bool IsCompressionMethodValid( const CompressionMethod compression_method )
	{
		constexpr CompressionMethod valid_values[] { CompressionMethod::Deflate };
		return Black::HasItem( valid_values, compression_method );
	}

	const bool IsFilterMethodValid( const FilterMethod filter_method )
	{
		constexpr FilterMethod valid_values[] { FilterMethod::Adaptive };
		return Black::HasItem( valid_values, filter_method );
	}

	const bool IsInterlaceMethodValid( const InterlaceMethod interlace_method )
	{
		constexpr InterlaceMethod valid_values[] { InterlaceMethod::Disabled, InterlaceMethod::Adam7 };
		return Black::HasItem( valid_values, interlace_method );
	}
}
}
}
}
