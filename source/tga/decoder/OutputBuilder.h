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
		@brief	Output image builder for image decoding pipeline.

		Output builder is the final, the fourth, step in TGA image decoding pipeline. It builds the output image
		using position of cursor and color converted from input.

		Main thing of output builder is the memory to store the output image.
		Coordinate cursor is used to place the input color at right output position.
	*/
	class OutputBuilder final
	{
	// Friendship interface.
	public:
		friend inline void swap( OutputBuilder& left, OutputBuilder& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		//
		static OutputBuilder SetupDirectFeeder( Black::PlainView<std::byte> output_buffer, const Black::ColorFormat output_format );

	// Public life-time management.
	public:
		inline OutputBuilder() noexcept								= default;
		inline OutputBuilder( const OutputBuilder& other ) noexcept	= default;
		OutputBuilder( OutputBuilder&& other ) noexcept;

		inline ~OutputBuilder() noexcept = default;

		inline OutputBuilder& operator = ( const OutputBuilder& other ) noexcept = default;
		OutputBuilder& operator = ( OutputBuilder&& other ) noexcept;

	// Public interface.
	public:
		// Swap the state of this builder with other.
		void Swap( OutputBuilder& other );


		// Produce the single pixel of output image.
		void ProduceElement( const uint32_t color, const CoordinateCursor& cursor );

	// Private state.
	private:
		Black::PlainView<std::byte>	m_output_buffer;	// Memory of output image.

		Black::ColorFormat			m_output_format		= Black::ColorFormats::UNDEFINED;	// Output image format.
	};
}
}
}
}
