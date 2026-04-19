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
		Black::BigEndianIntegral<uint32_t>		content_size;
		Black::BigEndianIntegral<ChunkTypeCode>	type_code;
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
		Black::BigEndianIntegral<uint32_t>	width;
		Black::BigEndianIntegral<uint32_t>	height;
		BitDepth							bit_depth;
		ColorType							color_type;
		CompressionMethod					compression_method;
		FilterMethod						filter_method;
		InterlaceMethod						interlace_method;
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
