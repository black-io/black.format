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
		@brief	Header of PNG chunk.

		This structure described in section 3.2 (Chunk layout) of PNG 1.2 file format specification.

		Chunk header precedes the chunk data.
		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct ChunkHeader final
	{
		Black::BigEndianIntegral<uint32_t>		content_size;	// A 4-byte unsigned integer giving the number of bytes in the chunk's data field.
		Black::BigEndianIntegral<ChunkTypeCode>	type_code;		// A 4-byte chunk type code.
	};
	#pragma pack( pop )

	/**
		@brief	Footer of PNG chunk.

		This structure described in section 3.2 (Chunk layout) of PNG 1.2 file format specification.

		Chunk footer always placed at the tail of chunk, right after the chunk data.
		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct ChunkFooter final
	{
		Black::BigEndianIntegral<uint32_t>	checksum;	// A 4-byte CRC (Cyclic Redundancy Check) calculated on the preceding bytes in the chunk.
	};
	#pragma pack( pop )

	/**
		@brief	PNG image header.

		This structure described in section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.

		Image header always placed as field `Chunk data` of chunk with `IDAT` type code.
		This structure always used to map the memory of file and read the fields directly.
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

	/**
		@brief	Regular chunk entry of PNG file.

		This entry implements the chunk of PNG file.
	*/
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
