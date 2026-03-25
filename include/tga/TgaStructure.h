#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
	/**
		@brief	Representation of TGA-file internal structure.
		TGA-file internal types, collected in single structure as TGA 2.0 file format specification describes.
		In any case any of internal TGA-file types needed by consumer's code, it may be accessed via `TgaStructure`.
	*/
	struct TgaStructure final
	{
		// Enumeration of supported content types.
		using ContentType = Internal::ContentType;

		// Enumeration of supported bit-rates.
		using Bitrate = Internal::Bitrate;

		// Enumeration of supported interlace modes.
		using Interlacing = Internal::Interlacing;

		// Enumeration of supported image orientations.
		using OriginPosition = Internal::OriginPosition;

		// Header of TGA file. Compatible with TGA 1.0 specification.
		using Header = Internal::Header;

		// Variable id placed here after the header, if `header.id_length` is specified.

		// Palette data placed here after the id of file, but only if `header.has_palete` is set.

		// Actual image data placed. It has different size, especially in case of RLE compression.

		// In case of TGA 2.0, the developer directory may be found here in case the `footer.developer_directory_offset` is specified.

		// In case of TGA 2.0, the extension area may be found here in case the `footer.extension_area_offset` is specified.

		// Footer of TGA 2.0 file.
		using Footer = Internal::Footer;
	};
}
}
}
