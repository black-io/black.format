#include <black/format/tga.h>


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Internal
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/Operations";
}


	std::string_view format_as( const ContentType value )
	{
		switch( value )
		{
		case ContentType::Empty:
			return "Empty";
		case ContentType::Paletted:
			return "Paletted";
		case ContentType::TrueColor:
			return "TrueColor";
		case ContentType::Grayscale:
			return "Grayscale";
		case ContentType::RlePaletted:
			return "RLE Paletted";
		case ContentType::RleTrueColor:
			return "RLE TrueColor";
		case ContentType::RleGrayscale:
			return "RLE Grayscale";
		default:
			break;
		}

		return "Unknown";
	}

	std::string_view format_as( const Bitrate value )
	{
		switch( value )
		{
		case Bitrate::Undefined:
			return "Undefined";
		case Bitrate::Monochrome:
			return "Monochrome";
		case Bitrate::ARGB16:
			return "ARGB16";
		case Bitrate::RGB24:
			return "RGB24";
		case Bitrate::ARGB32:
			return "ARGB32";
		default:
			break;
		}

		return "Unknown";
	}

	std::string_view format_as( const Interlacing value )
	{
		switch( value )
		{
		case Interlacing::Disabled:
			return "Disabled";
		case Interlacing::TwoWay:
			return "2-way";
		case Interlacing::QuadWay:
			return "4-way";
		case Interlacing::OctaWay:
			return "8-way";
		default:
			break;
		}

		return "Unknown";
	}

	std::string_view format_as( const OriginPosition value )
	{
		switch( value )
		{
		case OriginPosition::BottomLeft:
			return "BottomLeft";
		case OriginPosition::BottomRight:
			return "BottomRight";
		case OriginPosition::TopLeft:
			return "TopLeft";
		case OriginPosition::TopRight:
			return "TopRight";
		default:
			break;
		}

		return "Unknown";
	}
}
}
}
}
