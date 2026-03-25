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

	/**
		@brief	TGA image bit-rate specification.

		This enumeration explained in next sections of TGA 2.0 file format specification:
		- `Field 4.3 (1 byte) - Color map Entry Size`;
		- `Field 5.5 (1 byte) - Pixel Depth`.
		The description in this sections differ from each other, but the overall conclusion makes it real to combine both fields into same enumeration.

		The user should note that bit-rate of image describes total size of single element in bits.
		In case of paletted image, bit-rate of image describes the size of color index. And even in this case the image can store images with alpha-channel.

		Description of values uses some characters in next notation:
		- `<Char><Value>` is channel specification, where the `Value` means number of bits used and `Char` is one of next;
		- `R` - Red channel;
		- `G` - Green channel;
		- `B` - Blue channel;
		- `A` - Alpha channel;
		- `W` - Black to White magnitude;
		- `I` - index in color palette.
		- `X` - Irrelevant data;
		- All channel specifications are written in "hi->lo" order, representing the Little-endian byte order.
	*/
	enum class Bitrate : uint8_t
	{
		Undefined	= 0,	// Undefined bit-rate.
		Monochrome	= 8,	// 8Bpp W8 color or I8 color index in palette.
		ARGB16		= 16,	// 15+1Bpp X1R5G5B5 color or 16Bpp A1R5G5B5 color. Also may represent I16 or A8I8 color index in palette.
		RGB24		= 24,	// 24Bpp R8G8B8 color. Also may be represent A8I16 color index in palette.
		ARGB32		= 32,	// 24+8Bpp X8R8G8B8 color or 32Bpp A8R8G8B8 color.
	};
}
}
}
}
