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
}
}
}
}
