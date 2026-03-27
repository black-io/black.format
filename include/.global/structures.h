#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
		@brief	Pixel format description for image formats.

		All fields are compressed to reduce the size of overall pixel format collection.
	*/
	struct ImageFormat final
	{
		uint16_t			red_channel_bits	: 6;	// Size of red channel in bits.
		uint16_t			red_channel_index	: 2;	// Ordinal index of red channel in pixel.
		uint16_t			green_channel_bits	: 6;	// Size of green channel in bits.
		uint16_t			green_channel_index	: 2;	// Ordinal index of green channel in pixel.

		uint16_t			blue_channel_bits	: 6;	// Size of blue channel in bits.
		uint16_t			blue_channel_index	: 2;	// Ordinal index of blue channel in pixel.
		uint16_t			alpha_channel_bits	: 6;	// Size of alpha channel in bits.
		uint16_t			alpha_channel_index	: 2;	// Ordinal index of alpha channel in pixel.

		uint16_t			index_channel_bits	: 6;	// Size of index channel in bits.
		uint16_t			size_bits			: 6;	// Size of pixel block in bits.
		uint16_t			size_bytes			: 4;	// Size of pixel block in bytes.

		bool				is_index			: 1;	// Whether the format represents the palette index.
		bool				is_color			: 1;	// Whether the format represents the color.
		bool				is_monochrome		: 1;	// Whether the format describes monochrome color.
		bool				has_alpha			: 1;	// Whether the format contains alpha channel.


		inline operator const uint64_t () const;
	};

	/**
		@brief	Collection of supported image formats.

		Each format is named according to next rules.
		Component of image format named like `<Char><Value>`, where the `Char` means channels and `Value` means count of bits the channel occupy in format.
		All channels of format are packed tightly in bits. In the name of format all channels listed in <hi-lo> order.
		It means the first channel in name is placed in highest byte of format and the last channel in name is placed in lowest byte of format.

		Next chars used:
		- X: Irrelevant bits.
		- I: Palette index.
		- W: White channel. Black-White gradient.
		- R: Red channel.
		- G: Green channel.
		- B: Blue channel.
		- A: Alpha channel.
	*/
	struct ImageFormats final
	{
		static constexpr ImageFormat	UNDEFINED	{ 0, 0, 0, 0,		0, 0, 0, 0,		0,	0,	0,		false,	false,	false,	false };

		// 8Bpp formats.
		static constexpr ImageFormat	W8			{ 0, 0, 0, 0,		0, 0, 0, 0,		0,	8,	1,		false,	true,	true,	false };
		static constexpr ImageFormat	I8			{ 0, 0, 0, 0,		0, 0, 0, 0,		8,	8,	1,		true,	false,	false,	false };

		// 16Bpp formats.
		static constexpr ImageFormat	W16			{ 0, 0, 0, 0,		0, 0, 0, 0,		0,	16,	2,		false,	true,	true,	false };
		static constexpr ImageFormat	I16			{ 0, 0, 0, 0,		0, 0, 0, 0,		16,	16,	2,		true,	false,	false,	false };
		static constexpr ImageFormat	A8W8		{ 0, 0, 0, 0,		0, 0, 8, 1,		0,	16,	2,		false,	true,	true,	true };
		static constexpr ImageFormat	A8I8		{ 0, 0, 0, 0,		0, 0, 8, 1,		8,	16,	2,		true,	false,	false,	true };
		static constexpr ImageFormat	R5G6B5		{ 5, 2, 6, 1,		5, 0, 0, 0,		0,	16,	2,		false,	true,	false,	false };
		static constexpr ImageFormat	X1R5G5B5	{ 5, 2, 5, 1,		5, 0, 0, 0,		0,	16,	2,		false,	true,	false,	false };
		static constexpr ImageFormat	A1R5G5B5	{ 5, 2, 5, 1,		5, 0, 1, 3,		0,	16,	2,		false,	true,	false,	true };

		// 24Bpp formats.
		static constexpr ImageFormat	A8W16		{ 0, 0, 0, 0,		0, 0, 8, 1,		0,	24,	3,		false,	true,	true,	true };
		static constexpr ImageFormat	A8I16		{ 0, 0, 0, 0,		0, 0, 8, 1,		16,	24,	3,		true,	false,	false,	true };
		static constexpr ImageFormat	R8G8B8		{ 8, 2, 8, 1,		8, 0, 0, 0,		0,	24,	3,		false,	true,	false,	false };

		// 32Bpp formats.
		static constexpr ImageFormat	X8R8G8B8	{ 8, 2, 8, 1,		8, 0, 0, 0,		0,	32,	4,		false,	true,	false,	false };
		static constexpr ImageFormat	A8R8G8B8	{ 8, 2, 8, 1,		8, 0, 8, 3,		0,	32,	4,		false,	true,	false,	true };
	};
}
}
}
