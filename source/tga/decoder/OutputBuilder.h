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
	class OutputBuilder final : private Black::NonTransferable
	{
	// Public interface.
	public:
		//
		void RefuseCursor();

		//
		void UseCursor( const CoordinateCursor& cursor );

		//
		void UseOutputBuffer( Black::PlainView<std::byte> output_buffer );

		//
		void UseOutputFormat( const Black::ColorFormat output_format );


		//
		const Black::BooleanStatus ProduceElement( const uint32_t color );

	// Private state.
	private:
		const CoordinateCursor*		m_cursor			= nullptr;
		Black::PlainView<std::byte>	m_output_buffer;

		Black::ColorFormat			m_output_format		= Black::ColorFormats::UNDEFINED;
	};
}
}
}
}
