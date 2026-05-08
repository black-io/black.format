#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
	/**
		@brief	TGA image encoder.

		Currently this type is only stub. No functionality implemented.
	*/
	class TgaImageEncoder final
	{
	// Friendship interface.
	public:
		friend inline void swap( TgaImageEncoder& left, TgaImageEncoder& right )	{ left.Swap( right ); };

	// Public life-time management.
	public:
		inline TgaImageEncoder() noexcept						= default;
		inline TgaImageEncoder( const TgaImageEncoder& other )	= default;
		TgaImageEncoder( TgaImageEncoder&& other ) noexcept;

		explicit TgaImageEncoder( const TgaStructure::Header& output_header );

		inline ~TgaImageEncoder() noexcept = default;


		inline TgaImageEncoder& operator = ( const TgaImageEncoder& other )					= default;
		inline TgaImageEncoder& operator = ( TgaImageEncoder&& other ) noexcept				{ return Black::CopyAndSwap( *this, std::move( other ) ); };

		inline TgaImageEncoder& operator = ( const TgaStructure::Header& output_header )	{ return Black::CopyAndSwap( *this, output_header ); };

	// Public interface.
	public:
		// Clear the output buffer of encoder.
		void ClearOutputBuffer();

		// Swap the state of this encoder with other one.
		void Swap( TgaImageEncoder& other );

		// Perform the encoding process.
		Black::BooleanStatus Encode( const Black::PlainView<const std::byte>& image_buffer, const Black::ColorFormat image_format );

	// Private state.
	private:
		Internal::Header				m_output_header	{};	// Header of output TGA image.
		Black::PlainVector<std::byte>	m_output_buffer;	// buffer to store encoded image.
	};
}
}
}
