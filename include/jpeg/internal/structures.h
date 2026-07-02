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
}
}
}
}
