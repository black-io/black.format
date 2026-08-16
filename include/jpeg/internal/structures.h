#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
	/**
		@brief	Regular JIF marker.

		This structure described in section B.1.1.2 (Markers) of ISO/IEC 10918-1 : 1993(E) document.

		All markers are assigned two-byte codes:
			- an X’FF’ byte,
			- a byte which is not equal to 0 or X’FF’.

		Markers are the first of major JIF entities.
	*/
	#pragma pack( push, 1 )
	struct Marker final
	{
		std::byte	prefix;	// First byte of marker means prefix. Should be always equal to `MARKER_PREFIX` constant.
		MarkerCode	code;	// Second byte of marker stores the marker code. Should be one of valid `MarkerCode` values.
	};
	#pragma pack( pop )

	/**
		@brief	Regular header of JIF segment.

		This structure does not documented in JIF or JFIF specifications. But it much usable for purposes of JIF analysis.
		The meaning of structure is to declare the statement of segment in memory right after the header with length stored in header.

		The segments of marker always start with length. But the memory of `length` field also counted as part of segment, meaning the length of segment is
		part of it. It is not so usable to know the length only after the begin of segment investigation. Its much better to know the length of segment first,
		and next use the known bounds of memory as desired.
	*/
	#pragma pack( push, 1 )
	struct SegmentHeader final
	{
		Marker								marker;	// Marker corresponding to segment.
		Black::BigEndianIntegral<uint16_t>	length;	// Length of segment.
	};
	#pragma pack( pop )

	/**
		@brief	Header of APP0 segment in JFIF file.

		This structure described in section (JPEG File Interchange Format Specification) of JPEG File Interchange Format, Version 1.02 document.
		The memory of very first APP0 segment has format of this structure. So the memory of segment may be interpreted as object of this structure.

		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct JfifHeader final
	{
		// Length of JFIF header identifier.
		static constexpr size_t					IDENTIFIER_LENGTH = std::size( JFIF_HEADER_IDENTIFIER );

		char									identifier[ IDENTIFIER_LENGTH ];	// Identifier of header. Should equal only to `JFIF_HEADER_IDENTIFIER`.
		uint8_t									version[2];							// Version of JFIF format. Format: maj.min.
		DensityUnit								units;								// Units of `density` values.

		struct
		{
			Black::BigEndianIntegral<uint16_t>	x;
			Black::BigEndianIntegral<uint16_t>	y;
		}										density;

		struct
		{
			uint8_t								x;
			uint8_t								y;
		}										thumbnail;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct TiffHeader final
	{
		TiffEndianness	endianness;
		uint16_t		signature;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct ExifHeader final
	{
		static constexpr size_t		IDENTIFIER_LENGTH = std::size( EXIF_HEADER_IDENTIFIER );

		char						identifier[ IDENTIFIER_LENGTH ];
		TiffHeader					tiff_header;
		uint32_t					ifd_offset;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct XmpHeader final
	{
		static constexpr size_t	IDENTIFIER_LENGTH = std::size( XMP_HEADER_IDENTIFIER );

		char	identifier[ IDENTIFIER_LENGTH ];
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct FrameHeader final
	{
		uint8_t								image_precision;
		Black::BigEndianIntegral<uint16_t>	image_height;
		Black::BigEndianIntegral<uint16_t>	image_width;
		uint8_t								components_count;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct ScanHeader final
	{
		uint8_t	components_count;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct ScanFooter final
	{
		uint8_t		section_start;
		uint8_t		section_end;

		struct
		{
			uint8_t	high_bits	: 4;
			uint8_t	low_bits	: 4;
		}			approximation;
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct ScanComponent final
	{
		uint8_t		id;

		struct
		{
			uint8_t	ac_index	: 4;
			uint8_t	dc_index	: 4;
		}			dht_info;
	};
	#pragma pack( pop )

	struct SegmentEntry final
	{
		const SegmentHeader*				header;
		Black::PlainView<const std::byte>	content;
	};

	struct ImageBlockEntry final
	{
		const ScanHeader*						scan_header;
		const ScanFooter*						scan_footer;
		Black::PlainView<const ScanComponent>	scan_components;
		Black::PlainView<const std::byte>		image;
	};
}
}
}
}
