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
	class DecodePipeline final
		: public Black::Composition<
			DecodePipeline,
			Black::TypesUnion<StraightInputFeeder, RleInputFeeder>,
			Black::TypesUnion<PaletteColorMapper, TrueColorMapper, MonochromeColorMapper>
		>
	{
	// Public interface.
	public:
		//
		const Black::BooleanStatus SetupInputFeeder( const Internal::Header& header, const Black::PlainView<const std::byte>& image_buffer );

		//
		const Black::BooleanStatus SetupColorMapper( const Internal::Header& header, const Black::PlainView<const std::byte>& palette_buffer );

		//
		const Black::BooleanStatus SetupColorConverter( const Internal::Header& header, const Black::ImageFormat output_format );

		//
		const Black::BooleanStatus SetupOutputBuilder( const Black::PlainView<std::byte>& image_buffer, const Black::ImageFormat output_format );


		//
		const Black::BooleanStatus BeginProcessing( CoordinateCursor& image_cursor );

		//
		const Black::BooleanStatus Process();

		//
		const Black::BooleanStatus EndProcessing();


		inline const Black::BooleanStatus operator () () { return Process(); };

	// Private state.
	private:
		BasicInputFeeder*	m_input_feeder = nullptr;
		BasicColorMapper*	m_color_mapper = nullptr;

		size_t m_pixels_cout = 0;
	};
}
}
}
}
