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
		@brief	TGA file image type.

		This enumeration explained in section `Image Type - Field 3 (1 byte)` of TGA 2.0 file format specification.
		Additional explanation of image types may be seen at section `IMAGE TYPES` of TGA 2.0 file format specification.

		Image Data Type codes 0 to 127 are reserved for use by Truevision for general applications.
		Image Data Type codes 128 to 255 may be used for developer applications.
		Such additional type codes can't be predicted and so can't be defined directly in this enumeration.
		But any external user-defined constant of this enumerations may be defined in code of library user to specify such image type code.

		As may be seen through the values, bit `0x080` of value explains the RLE compression. After masking this bit, the rest value become equal
		to group of uncompressed image types. Such information may be useful to classify the content type under the compression.
		Also today is known that bit `0x020` of value explain more deep compression that use Huffman compression over RLE sequences.
	*/
	enum class ContentType : uint8_t
	{
		Empty				= 0,	// No Image Data Included.
		Paletted			= 1,	// Uncompressed, Color-mapped Image.
		Rgba				= 2,	// Uncompressed, True-color Image.
		Grayscale			= 3,	// Uncompressed, Black-and-white Image.
		RlePaletted			= 9,	// Run-length encoded, Color-mapped Image.
		RleRgba				= 10,	// Run-length encoded, True-color Image.
		RleGrayscale		= 11,	// Run-length encoded, Black-and-white Image.
	};
}
}
}
}
