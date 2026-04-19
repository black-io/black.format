#include <black/format/png.h>


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
	constexpr const char* LOG_CHANNEL = "Black/PNG/Operations";
}


	std::string_view format_as( const ChunkTypeCode value )
	{
		switch( value )
		{
		case ChunkTypeCode::ImageHeader:
			return "IHDR";
		case ChunkTypeCode::Palette:
			return "PLTE";
		case ChunkTypeCode::ImageData:
			return "IDAT";
		case ChunkTypeCode::ImageEnd:
			return "IEND";
		case ChunkTypeCode::Transparency:
			return "tRNS";
		case ChunkTypeCode::ImageGamma:
			return "gAMA";
		case ChunkTypeCode::Chromaticities:
			return "cHRM";
		case ChunkTypeCode::SrgbColorSpace:
			return "sRGB";
		case ChunkTypeCode::EmbeddedIcc:
			return "iCCP";
		case ChunkTypeCode::TextualData:
			return "tEXt";
		case ChunkTypeCode::CompressedTextualData:
			return "zTXt";
		case ChunkTypeCode::InternationalTextualData:
			return "iTXt";
		case ChunkTypeCode::BackgroundColor:
			return "bKGD";
		case ChunkTypeCode::PhysicalDimensions:
			return "pHYs";
		case ChunkTypeCode::SignificantBits:
			return "sBIT";
		case ChunkTypeCode::SuggestedPalette:
			return "sPLT";
		case ChunkTypeCode::PaletteHistogram:
			return "hIST";
		case ChunkTypeCode::LastModificationTime:
			return "tIME";
		default:
			break;
		};

		return "Unknown";
	}

	std::string_view format_as( const ChunkTypeFlag value )
	{
		switch( value )
		{
		case ChunkTypeFlag::None:
			return "None";
		case ChunkTypeFlag::Ancillary:
			return "Ancillary";
		case ChunkTypeFlag::Private:
			return "Private";
		case ChunkTypeFlag::Reserved:
			return "Reserved";
		case ChunkTypeFlag::CopySafe:
			return "Safe to copy";
		default:
			break;
		}

		return "Unknown";
	}

	std::string_view format_as( const BitDepth value )
	{
		switch( value )
		{
		case BitDepth::Undefined:
			return "Undefined";
		case BitDepth::Bit:
			return "1Bpp";
		case BitDepth::QuarterByte:
			return "2Bpp";
		case BitDepth::HalfByte:
			return "4Bpp";
		case BitDepth::Byte:
			return "8Bpp";
		case BitDepth::Word:
			return "16Bpp";
		default:
			break;
		}

		return "Unknown";
	}

	std::string_view format_as( const ColorType value )
	{
		switch( value )
		{
		case ColorType::Grayscale:
			return "Grayscale";
		case ColorType::RGB:
			return "RGB";
		case ColorType::Paletted:
			return "Paletted";
		case ColorType::GrayscaleAlpha:
			return "Transparent grayscale";
		case ColorType::ARGB:
			return "Transparent RGB";
		default:
			break;
		}

		return "Unknown";
	}

	std::string_view format_as( const ColorTypeFlag value )
	{
		switch( value )
		{
		case ColorTypeFlag::None:
			return "None";
		case ColorTypeFlag::Palette:
			return "Palette";
		case ColorTypeFlag::TrueColor:
			return "TrueColor";
		case ColorTypeFlag::Alpha:
			return "Alpha";
		default:
			break;
		}

		return "Unknown";
	}
}
}
}
}
