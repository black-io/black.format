#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
	#pragma pack( push, 1 )
	struct Marker final
	{
		std::byte	prefix;
		MarkerCode	code;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct SegmentHeader final
	{
		Marker								marker;
		Black::BigEndianIntegral<uint16_t>	length;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct JfifHeader final
	{
		char		signature[5];
		uint8_t		version[2];
		uint8_t		units;
		uint16_t	width;
		uint16_t	height;
		uint8_t		thumbnail_width;
		uint8_t		thumbnail_height;
	};
	#pragma pack( pop )

	struct SegmentEntry final
	{
		const SegmentHeader*				header;
		Black::PlainView<const std::byte>	content;
	};
}
}
}
}
