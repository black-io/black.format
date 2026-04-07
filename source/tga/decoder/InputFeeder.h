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
	class InputFeeder final
	{
	// Friendship interface.
	public:
		friend inline void swap( InputFeeder& left, InputFeeder& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		//
		static InputFeeder SetupStraightFeeder( Black::PlainView<const std::byte> input_buffer, const Internal::Bitrate input_bitrate );

		//
		static InputFeeder SetupRleFeeder( Black::PlainView<const std::byte> input_buffer, const Internal::Bitrate input_bitrate );

	// Public life-time management.
	public:
		inline InputFeeder() noexcept							= default;
		inline InputFeeder( const InputFeeder& other ) noexcept	= default;
		InputFeeder( InputFeeder&& other ) noexcept;

		inline ~InputFeeder() noexcept = default;

		inline InputFeeder& operator = ( const InputFeeder& other ) noexcept = default;
		InputFeeder& operator = ( InputFeeder&& other ) noexcept;

	// Public interface.
	public:
		//
		void Rewind();

		//
		void Swap( InputFeeder& other );


		//
		const Black::BooleanStatus StepForward();

		//
		inline const std::byte* const PeekElement() const	{ return m_block_payload; };


		//
		inline const size_t GetElementSize() const			{ return m_input_element_size; };

		//
		inline const Internal::Bitrate GetBitrate() const	{ return m_input_bitrage; };


		//
		inline const bool HasElements() const				{ return m_input_buffer.IsInside( m_block_payload ); };

	// Private interface.
	private:
		//
		void ParseBlockHeader();

	// Private state.
	private:
		Black::PlainView<const std::byte> m_input_buffer;

		const std::byte*	m_block_payload			= nullptr;
		size_t				m_block_rest_length		= 0;
		size_t				m_input_element_size	= 0;
		Internal::Bitrate	m_input_bitrage			= Internal::Bitrate::Undefined;

		union
		{
			uint8_t			m_flags_buffer			= 0;

			struct
			{
				bool		m_has_compressed_input	: 1;
				bool		m_is_block_compressed	: 1;
			};
		};
	};
}
}
}
}
