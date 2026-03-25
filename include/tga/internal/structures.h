#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Internal
{
	/**
		@brief	Header of TGA file.

		This header described in section `TGA FILE HEADER` of TGA 2.0 file format specification.
		All fields are implemented as specified. But it was decided to give new names to some fields in order to improve the readability and meaning of code.
		Also some fields are implemented with different types than specifications describes. It was made also to improve the readability and meaning of code.

		Header always placed at first 18 bytes of valid TGA file. Any arbitrary file may be verified to satisfy the specification of TGA format via
		reading and testing the header. Due some limitations, values of header fields depends on some other values in header.
		So, checking this relations may give the answer whether the file is valid TGA or not.

		Some of fields become irrelevant through the days. Such fields are made anonymous in this implementation to prevent the ambiguity of meaning.

		The total size of palette (in bytes) may be only calculated using the values of `palette` field.
		Since the `bitrate` fields store only size in bits, the per-byte size should be calculated through the division by `8`.
		In this case the size of palette may be calculated as `palette.length * ( palette.bitrate >> 3 )`.
		Also the size of image may be calculated as `image.width * image.height * ( image.bitrate >> 3 )`.
	*/
	#pragma pack( push, 1 )
	struct Header final
	{
		uint8_t				id_length;				// ID Length - Field 1 (1 byte). A value of zero indicates that no Image ID field is included with the image.
		bool				has_palette;			// Color Map Type - Field 2 (1 byte). Due to specification, this field is made boolean.
		ContentType			content_type;			// Image Type - Field 3 (1 byte). Details may be observed in docs of `ContentType` type.

		struct
		{
			uint16_t		: 16;					// Field 4.1 (2 bytes) - First Entry Index. Unused more.
			uint16_t		length;					// Field 4.2 (2 bytes) - Color map Length. Measured in palette items.
			Bitrate			bitrate;				// Field 4.3 (1 byte) - Color map Entry Size. Bit-rate of single item in palette.
		}					palette;				// Color Map Specification - Field 4 (5 bytes). Valuable only if `has_palette` is `true`.

		struct
		{
			uint16_t		: 16;					// Field 5.1 (2 bytes) - X-origin of Image. Unused more.
			uint16_t		: 16;					// Field 5.2 (2 bytes) - Y-origin of Image. Unused more.
			uint16_t		width;					// Field 5.3 (2 bytes) - Image Width. In pixels/indices. Width of stored image.
			uint16_t		height;					// Field 5.4 (2 bytes) - Image Height. In pixels/indices. Height of stored image.
			Bitrate			bitrate;				// Field 5.5 (1 byte) - Pixel Depth. Bit-rate of single pixel/index of image.

			union
			{
				uint8_t		bits;					// Overall bits of `Image Descriptor` may be observed using this value.
				struct
				{
					uint8_t	alpha_length	: 4;	// Bits 3-0. Describe the length of alpha channel, in bits.
					uint8_t	origin_position	: 2;	// Bits 5 & 4. May be covered by `OriginPosition` type to understand the meaning of values.
					uint8_t	interlace_mode	: 2;	// Bits 7 & 6. May be covered by `Interlacing` type to understand the meaning of values.
				};
			}				flags;					// Field 5.6 (1 byte) - Image Descriptor. This field represented by bit-fields for best usability.
		}					image;					// Image Specification - Field 5 (10 bytes). Describe the image screen location, size and pixel depth.
	};
	#pragma pack( pop )

	/**
		@brief	Footer of TGA file.

		This footer described in section `TGA FILE FOOTER` of TGA 2.0 file format specification.

		Footer exist only in version 2.0 of TGA file format. It always occupy the last 26 bytes of valid TGA 2.0 file.
		Valid footer indicates the file as valid TGA 2.0 file in case the header of file also checked and valid.

		Both offsets are zero-based and measured from beginning of file.
		According to specification of TGA format, developer directory always should be placed after the image, but before the extension area.
		So the offset of developer directory should be always less than offset of extension area.
		Anyway, both offsets are optional and may be initialized as `0` in case the corresponding area does not placed in file.
	*/
	#pragma pack( push, 1 )
	struct Footer final
	{
		size32_t	extension_area_offset;		// Byte 0-3 - Extension Area Offset - Field 28. Optional offset of extension area in file.
		size32_t	developer_directory_offset;	// Byte 4-7 - Developer Directory Offset - Field 29. Optional offset of developer directory in file.
		char		signature[18];				// Byte 8-23 - Signature - Field 30, 31 and 32. All last fields combined in single string.
	};
	#pragma pack( pop )
}
}
}
}
