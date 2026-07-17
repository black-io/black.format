#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
	inline constexpr std::byte MARKER_PREFIX = ~std::byte{};

	enum class MarkerCode : uint8_t
	{
		Sof0		= 0xC0,	// [SOF0] Start of Frame 0 - Baseline DCT.
		Sof1		= 0xC1,	// [SOF1] Start of Frame 1 - Extended Sequential DCT.
		Sof2		= 0xC2,	// [SOF2] Start of Frame 2 - Progressive DCT.
		Sof3		= 0xC3,	// [SOF3] Start of Frame 3 - Lossless (sequential).
		Dht			= 0xC4,	// [DHT] Define Huffman Table.
		Sof5		= 0xC5,	// [SOF5] Start of Frame 5 - Differential sequential DCT.
		Sof6		= 0xC6,	// [SOF6] Start of Frame 6 - Differential progressive DCT.
		Sof7		= 0xC7,	// [SOF7] Start of Frame 7 - Differential lossless (sequential).
		Jpg			= 0xC8,	// [JPG] JPEG Extensions.
		Sof9		= 0xC9,	// [SOF9] Start of Frame 9 - Extended sequential DCT, Arithmetic coding.
		Sof10		= 0xCA,	// [SOF10] Start of Frame 10 - Progressive DCT, Arithmetic coding.
		Sof11		= 0xCB,	// [SOF11] Start of Frame 11 - Lossless (sequential), Arithmetic coding.
		Dac			= 0xCC,	// [DAC] Define Arithmetic Coding.
		Sof13		= 0xCD,	// [SOF13] Start of Frame 13 - Differential sequential DCT, Arithmetic coding.
		Sof14		= 0xCE,	// [SOF14] Start of Frame 14 - Differential progressive DCT, Arithmetic coding.
		Sof15		= 0xCF,	// [SOF15] Start of Frame 15 - Differential lossless (sequential), Arithmetic coding.
		Rst0		= 0xD0,	// [RST0] Restart Marker 0.
		Rst1		= 0xD1,	// [RST1] Restart Marker 1.
		Rst2		= 0xD2,	// [RST2] Restart Marker 2.
		Rst3		= 0xD3,	// [RST3] Restart Marker 3.
		Rst4		= 0xD4,	// [RST4] Restart Marker 4.
		Rst5		= 0xD5,	// [RST5] Restart Marker 5.
		Rst6		= 0xD6,	// [RST6] Restart Marker 6.
		Rst7		= 0xD7,	// [RST7] Restart Marker 7.
		Soi			= 0xD8,	// [SOI] Start of Image.
		Eoi			= 0xD9,	// [EOI] End of Image.
		Sos			= 0xDA,	// [SOS] Start of Scan.
		Dqt			= 0xDB,	// [DQT] Define Quantization Table.
		Dnl			= 0xDC,	// [DNL] Define Number of Lines - (Not common).
		Dri			= 0xDD,	// [DRI] Define Restart Interval.
		Dhp			= 0xDE,	// [DHP] Define Hierarchical Progression - (Not common).
		Exp			= 0xDF,	// [EXP] Expand Reference Component - (Not common).
		App0		= 0xE0,	// [APP0] Application Segment 0 - JFIF JPEG image.
		App1		= 0xE1,	// [APP1] Application Segment 1 - EXIF Metadata, TIFF IFD format, JPEG Thumbnail (160x120), Adobe XMP.
		App2		= 0xE2,	// [APP2] Application Segment 2 - ICC color profile, FlashPix.
		App3		= 0xE3,	// [APP3] Application Segment 3 - (Not common) JPS Tag for Stereoscopic JPEG images.
		App4		= 0xE4,	// [APP4] Application Segment 4 - (Not common).
		App5		= 0xE5,	// [APP5] Application Segment 5 - (Not common).
		App6		= 0xE6,	// [APP6] Application Segment 6 - (Not common) NITF Lossless profile.
		App7		= 0xE7,	// [APP7] Application Segment 7 - (Not common).
		App8		= 0xE8,	// [APP8] Application Segment 8 - (Not common).
		App9		= 0xE9,	// [APP9] Application Segment 9 - (Not common).
		App10		= 0xEA,	// [APP10] Application Segment 10 - (Not common) ActiveObject (multimedia messages / captions).
		App11		= 0xEB,	// [APP11] Application Segment 11 - (Not common) HELIOS JPEG Resources (OPI Postscript).
		App12		= 0xEC,	// [APP12] Application Segment 12 - Picture Info (older digicams), Photoshop Save for Web: Ducky.
		App13		= 0xED,	// [APP13] Application Segment 13 - Photoshop Save As: IRB, 8BIM, IPTC.
		App14		= 0xEE,	// [APP14] Application Segment 14 - (Not common).
		App15		= 0xEF,	// [APP15] Application Segment 15 - (Not common).
		Jpg0		= 0xF0,	// [JPG0] JPEG Extension 0 - (Not common).
		Jpg1		= 0xF1,	// [JPG1] JPEG Extension 1 - (Not common).
		Jpg2		= 0xF2,	// [JPG2] JPEG Extension 2 - (Not common).
		Jpg3		= 0xF3,	// [JPG3] JPEG Extension 3 - (Not common).
		Jpg4		= 0xF4,	// [JPG4] JPEG Extension 4 - (Not common).
		Jpg5		= 0xF5,	// [JPG5] JPEG Extension 5 - (Not common).
		Jpg6		= 0xF6,	// [JPG6] JPEG Extension 6 - (Not common).
		Jpg7		= 0xF7,	// [JPG7] JPEG Extension 7 - Lossless JPEG.
		Jpg8		= 0xF8,	// [JPG8] JPEG Extension 8 - Lossless JPEG Extension Parameters.
		Jpg9		= 0xF9,	// [JPG9] JPEG Extension 9 - (Not common).
		Jpg10		= 0xFA,	// [JPG10] JPEG Extension 10 - (Not common).
		Jpg11		= 0xFB,	// [JPG11] JPEG Extension 11 - (Not common).
		Jpg12		= 0xFC,	// [JPG12] JPEG Extension 12 - (Not common).
		Jpg13		= 0xFD,	// [JPG13] JPEG Extension 13 - (Not common).
		Com			= 0xFE,	// [COM] Comment - Comment.
	};

	inline constexpr MarkerCode INVALID_CODE_1	= MarkerCode( 0x00U );
	inline constexpr MarkerCode INVALID_CODE_2	= MarkerCode( 0xFFU );
	inline constexpr MarkerCode MIN_CODE		= MarkerCode::Sof0;

	enum class DensityUnit : uint8_t
	{
		None,		// Density measured in pixels as is.
		Inch,		// Density per inch.
		Centimeter,	// Density per centimeter.
	};

	constexpr const char* JFIF_HEADER_IDENTIFIER = "JFIF";

	enum class TiffEndianness : uint16_t
	{
		LittleEndian	= 0x4949U,	// Value 'II' that represent little-endian endianness.
		BigEndian		= 0x4D4DU,	// Value 'MM' that represent big-endian endianness.
	};

	constexpr const char* EXIF_HEADER_IDENTIFIER = "Exif";
}
}
}
}
