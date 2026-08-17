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
			Black::BigEndianIntegral<uint16_t>	x;									// Density of image width.
			Black::BigEndianIntegral<uint16_t>	y;									// Density of image height.
		}										density;							// Image density. The meaning of values depends on `units` value.

		struct
		{
			uint8_t								x;									// Width of thumbnail.
			uint8_t								y;									// Height of thumbnail.
		}										thumbnail;							// Properties of embedded thumbnail.
	};
	#pragma pack( pop )

	/**
		@brief	TIFF image header.

		This structure described in section (Image File Header) of TIFF 6.0 Specification.
		This header used as part of Exif header in APP1 segment.
	*/
	#pragma pack( push, 1 )
	struct TiffHeader final
	{
		TiffEndianness	endianness;	// Endianness of following data.
		uint16_t		signature;	// TIFF signature value. Should equal only to `TIFF_SIGNATURE`.
	};
	#pragma pack( pop )

	/**
		@brief	EXIF data header.

		This structure described in section 4.7.2.B (APP1 Interoperability structure) of JEITA CP-3451 document.
		In EXIF file, the memory of very first APP1 segment should has format of this structure.
		So the memory of segment may be interpreted as object of this structure.

		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct ExifHeader final
	{
		// Length of EXIF header identifier.
		static constexpr size_t		IDENTIFIER_LENGTH = std::size( EXIF_HEADER_IDENTIFIER );

		char						identifier[ IDENTIFIER_LENGTH ];	// Identifier of header. Should equal only to `EXIF_HEADER_IDENTIFIER`.
		uint8_t						padding;							// Padding to align the TIFF header.
		TiffHeader					tiff_header;						// Embedded TIFF header.
		uint32_t					ifd_offset;							// Offset to first Image File Directory.
	};
	#pragma pack( pop )

	/**
		@brief	Adobe XAP file header.

		Adobe XAP is proprietary file format for Adobe XMP platform. So this way no citations placed here.

		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct XapHeader final
	{
		// Length of XAP header identifier.
		static constexpr size_t	IDENTIFIER_LENGTH = std::size( XAP_HEADER_IDENTIFIER );

		char	identifier[ IDENTIFIER_LENGTH ];	// Identifier of header. Should equal only to `XAP_HEADER_IDENTIFIER`.
	};
	#pragma pack( pop )

	/**
		@brief	Header of SOFn segment in regular JIF file.

		This structure describer in section B.2.2 (Frame header syntax) of ISO/IEC 10918-1 : 1993(E) document.

		This header specifies the source image characteristics, the components in the frame, and the sampling factors for each component,
		and specifies the destinations from which the quantized tables to be used with each component are retrieved.

		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct FrameHeader final
	{
		uint8_t								image_precision;	// (P) Sample precission, in bits.
		Black::BigEndianIntegral<uint16_t>	image_height;		// (Y) Number of lines in the source image.
		Black::BigEndianIntegral<uint16_t>	image_width;		// (X) Number of samples per line in the source image.
		uint8_t								components_count;	// (Nf) Number of source image components in the frame.
	};
	#pragma pack( pop )

	/**
		@brief	Header of SOS segment in regular JIF file.

		This structure describer in section B.2.3 (Scan header syntax) of ISO/IEC 10918-1 : 1993(E) document.

		This header specifies which component(s) are contained in the scan, specifies the destinations from which the entropy tables to be used with each
		component are retrieved, and (for the progressive DCT) which part of the DCT quantized coefficient data is contained in the scan.

		The structure is only part of described SOS segment format. Due to variable number of component descriptors right after the count,
		the memory of SOS segment is segmented in 3 structures: header, components and footer.
		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct ScanHeader final
	{
		uint8_t	components_count;	// (Ns) Number of source image components in the scan.
	};
	#pragma pack( pop )

	/**
		@brief	Footer of SOS segment in regular JIF file.

		This structure describer in section B.2.3 (Scan header syntax) of ISO/IEC 10918-1 : 1993(E) document.

		The structure is only part of described SOS segment format. Due to variable number of component descriptors right after the count,
		the memory of SOS segment is segmented in 3 structures: header, components and footer.
		This structure always used to map the memory of file and read the fields directly.
	*/
	#pragma pack( push, 1 )
	struct ScanFooter final
	{
		uint8_t		section_start;		// Start of spectral or predictor selection.
		uint8_t		section_end;		// End of spectral selection.

		struct
		{
			uint8_t	high_bits	: 4;	// Successive approximation bit position high.
			uint8_t	low_bits	: 4;	// Successive approximation bit position low or point transform.
		}			approximation;		// Settings of scan approximation process.
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
