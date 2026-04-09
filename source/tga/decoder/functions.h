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
		@brief	Setup the given input feeder according to settings from TGA image.

		This function detect the mode of input feeder and perform re-initialization of target feeder in required mode.
		As result, the target feeder will produce valid output for given input TGA file.

		@param	feeder			Target feeder for set up.
		@param	header			TGA file header of input image.
		@param	image_buffer	View of input image from TGA file.
		@return					`Succeed` status in case of complete setup. `Failed` in any other case.
	*/
	const Black::BooleanStatus SetupInputFeeder( InputFeeder& feeder, const Internal::Header& header, const Black::PlainView<const std::byte>& image_buffer );

	/**
		@brief	Setup the given color mapper according to settings from TGA image.

		This function select the mode of color mapper basing on given TGA header.

		@param	mapper			Target mapper for set up.
		@param	header			TGA file header of input image.
		@param	palette_buffer	Optional buffer of palette. May be empty, but should be valid for TGA file with `Paletted` content type.
		@return					`Succeed` status in case of complete setup. `Failed` in any other case.
	*/
	const Black::BooleanStatus SetupColorMapper( ColorMapper& mapper, const Internal::Header& header, const Black::PlainView<const std::byte>& palette_buffer );

	/**
		@brief	Setup the given color converter according to settings from TGA image and output settings.

		This function compares the formats of pipeline input and output to determine the mode for target converter.

		@param	converter		Target converter for set up.
		@param	header			TGA file header of input image.
		@param	output_format	Format of output image.
		@return					`Succeed` status in case of complete setup. `Failed` in any other case.
	*/
	const Black::BooleanStatus SetupColorConverter( ColorConverter& converter, const Internal::Header& header, const Black::ColorFormat output_format );

	/**
		@brief	Setup the given output builder according to output settings of decoding pipeline.

		This function just combine the given output settings into object of target builder.

		@param	builder			Target builder for set up.
		@param	image_buffer	View of memory, where the output image should be stored.
		@param	output_format	Format of output image.
		@return					`Succeed` status in case of complete setup. `Failed` in any other case.
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
