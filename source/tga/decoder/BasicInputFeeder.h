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
	class BasicInputFeeder : private Black::NonTransferable
	{
	// Public inner types.
	public:
		//
		using Bitrate = Black::TgaStructure::Bitrate;

	// Public interface.
	public:
		//
		void UseImageBuffer( Black::PlainView<const std::byte> image_buffer );

		//
		void UseBitrate( const Bitrate bitrate );


		//
		void Rewind();

		//
		const Black::BooleanStatus StepForward();


		//
		inline const std::byte* PeekElement() const	{ return m_current_element; };

		//
		inline const size_t GetElementSize() const	{ return m_element_size; };

		//
		inline const Bitrate GetBitrate() const		{ return m_bitrage; };


		//
		inline const bool HasElements() const		{ return m_current_element != m_image_buffer.GetEnd(); };

	// Heirs life-time management.
	protected:
		inline BasicInputFeeder() noexcept	= default;
		inline ~BasicInputFeeder() noexcept	= default;

	// Heirs interface.
	protected:
		//
		void ShiftCurrentElement( const size_t size );

	// Heirs virtual interface.
	protected:
		//
		virtual void OnRewind() = 0;

		//
		virtual const Black::BooleanStatus OnStepForward() = 0;

	// Private state.
	private:
		Black::PlainView<const std::byte>	m_image_buffer;
		const std::byte*					m_current_element	= nullptr;
		size_t								m_element_size		= 0;

		Bitrate m_bitrage = Bitrate::Undefined;
	};
}
}
}
}
