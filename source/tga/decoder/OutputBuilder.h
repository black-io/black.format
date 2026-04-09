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

		// Instruct the builder to stop the using of previously given coordinate cursor.
		void RefuseCursor();

		// Instruct the builder to use given coordinate cursor.
		void UseCursor( const CoordinateCursor& cursor );

		// Instruct the builder to use given buffer as output image memory.
		void UseOutputBuffer( Black::PlainView<std::byte> output_buffer );

		// Instruct he builder to use given format of output image.
		void UseOutputFormat( const Black::ColorFormat output_format );


		// Produce the single pixel of output image, using position of coordinate cursor along with given color.
		const Black::BooleanStatus ProduceElement( const uint32_t color );

	// Private state.
	private:
		const CoordinateCursor*		m_cursor			= nullptr;							// Configured coordinate cursor.
		Black::PlainView<std::byte>	m_output_buffer;										// Memory of output image.

		Black::ColorFormat			m_output_format		= Black::ColorFormats::UNDEFINED;	// Output image format.
	};
}
}
}
}
