#include <black/format/jpeg.h>


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/JPEG/Operations";
}


	std::string_view format_as( const MarkerCode value )
	{
		static constexpr std::string_view code_names[] {
			"SOF0",		"SOF1",		"SOF2",	"SOF3",
			"DHT",
			"SOF5",		"SOF6",		"SOF7",
			"JPG",
			"SOF9",		"SOF10",	"SOF11",
			"DAC",
			"SOF13",	"SOF14",	"SOF15",
			"RST0",		"RST1",		"RST2",		"RST3",		"RST4",		"RST5",		"RST6",		"RST7",
			"SOI",		"EOI",		"SOS",		"DQT",		"DNL",		"DRI",		"DHP",		"EXP",
			"APP0",		"APP1",		"APP2",		"APP3",		"APP4",		"APP5",		"APP6",		"APP7",		"APP8",		"APP9",		"APP10",
			"APP11",	"APP12",	"APP13",	"APP14",	"APP15",
			"JPG0",		"JPG1",		"JPG2",		"JPG3",		"JPG4",		"JPG5",		"JPG6",		"JPG7",		"JPG8",		"JPG9",		"JPG10",
			"JPG11",	"JPG12",	"JPG13",
			"COM",
		};

		CRET( !IsMarkerCodeValid( value ), "Unknown" );

		const size_t code_index = Black::GetEnumValue( value ) - Black::GetEnumValue( MarkerCode::Sof0 );
		return code_names[ code_index ];
	}

	std::string_view format_as( const DensityUnit value )
	{
		switch( value )
		{
		case DensityUnit::None:
			return "None";
		case DensityUnit::Inch:
			return "DPI";
		case DensityUnit::Centimeter:
			return "DPC";
		default:
			break;
		}

		return "Unknown";
	}

	std::string_view format_as( const TiffEndianness value )
	{
	}
}
}
}
}
