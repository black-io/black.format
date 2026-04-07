#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Decoder
{
	/**
	*/
	const Black::BooleanStatus SetupInputFeeder( InputFeeder& feeder, const Internal::Header& header, const Black::PlainView<const std::byte>& image_buffer );

	/**
	*/
	const Black::BooleanStatus SetupColorMapper( ColorMapper& mapper, const Internal::Header& header, const Black::PlainView<const std::byte>& palette_buffer );

	/**
	*/
	const Black::BooleanStatus SetupColorConverter( ColorConverter& converter, const Internal::Header& header, const Black::ColorFormat output_format );

	/**
	*/
	const Black::BooleanStatus SetupOutputBuilder(
		OutputBuilder& builder,
		const Black::PlainView<std::byte>& image_buffer,
		const Black::ColorFormat output_format
	);
}
}
}
}
