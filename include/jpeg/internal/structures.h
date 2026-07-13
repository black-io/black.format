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
		char			identifier[5];
		uint8_t			version[2];
		DensityUnit		units;

		struct
		{
			uint16_t	x;
			uint16_t	y;
		}				density;

		struct
		{
			uint8_t		x;
			uint8_t		y;
		}				thumbnail;
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
