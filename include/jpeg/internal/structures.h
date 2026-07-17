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
		TiffEndianness						endianness;
		Black::BigEndianIntegral<uint16_t>	signature;
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

		char	identifier[29];
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
