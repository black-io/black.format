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
		static constexpr size_t					IDENTIFIER_LENGTH = std::size( JFIF_HEADER_IDENTIFIER );

		char									identifier[ IDENTIFIER_LENGTH ];
		uint8_t									version[2];
		DensityUnit								units;

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
		static constexpr size_t				IDENTIFIER_LENGTH = std::size( EXIF_HEADER_IDENTIFIER );

		char								identifier[ IDENTIFIER_LENGTH ];
		TiffHeader							tiff_header;
		Black::BigEndianIntegral<uint32_t>	ifd_offset;
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
