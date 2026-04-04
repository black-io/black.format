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
			Black::TypesUnion<PaletteColorMapper, TrueColorMapper, MonochromeColorMapper>,
			Black::TypesUnion<MonochromeColorConverter, DirectColorConverter, RemappingColorConverter, TransformColorConverter>
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
		const Black::BooleanStatus EndProcessing( CoordinateCursor& image_cursor );


		inline const Black::BooleanStatus operator () () { return Process(); };

	// Private state.
	private:
		BasicInputFeeder*		m_input_feeder		= nullptr;
		BasicColorMapper*		m_color_mapper		= nullptr;
		BasicOutputBuilder*		m_output_builder	= nullptr;
		BasicColorConverter*	m_color_converter	= nullptr;
	};
}
}
}
}
