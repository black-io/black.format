#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
	/**
	*/
	#pragma pack( push, 1 )
	struct ChunkHeader final
	{
		Black::BigEndianIntegral<uint32_t>	content_size;
		Black::BigEndianIntegral<uint32_t>	type_code;
	};
	#pragma pack( pop )

	/**
	*/
	#pragma pack( push, 1 )
	struct ChunkFooter final
	{
		Black::BigEndianIntegral<uint32_t>	checksum;
	};
	#pragma pack( pop )

	/**
	*/
	#pragma pack( push, 1 )
	struct ImageHeader final
	{
		uint32_t	width;
		uint32_t	height;
		uint8_t		bit_depth;			// TODO: Express the enumeration.
		uint8_t		color_type;			// TODO: Express the enumeration.
		uint8_t		compression_method;	// TODO: Express the enumeration.
		uint8_t		filter_method;		// TODO: Express the enumeration.
		uint8_t		interlace_method;	// TODO: Express the enumeration.
	};
	#pragma pack( pop )

	//
	struct ChunkEntry final
	{
		const ChunkHeader*					header;
		Black::PlainView<const std::byte>	content;
		const ChunkFooter*					footer;
	};
}
}
}
}
