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
		//
		void Swap( OutputBuilder& other );

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
