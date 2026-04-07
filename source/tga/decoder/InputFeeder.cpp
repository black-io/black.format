#include "decoder.h"


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Decoder
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/InputFeeder";
}


	InputFeeder InputFeeder::SetupStraightFeeder( Black::PlainView<const std::byte> input_buffer, const Internal::Bitrate input_bitrate )
	{
		InputFeeder feeder;

		feeder.m_input_buffer			= std::move( input_buffer );
		feeder.m_input_element_size		= Internal::GetElementSize( input_bitrate );
		feeder.m_input_bitrage			= input_bitrate;
		feeder.m_has_compressed_input	= false;
		feeder.Rewind();

		return feeder;
	}

	InputFeeder InputFeeder::SetupRleFeeder( Black::PlainView<const std::byte> input_buffer, const Internal::Bitrate input_bitrate )
	{
		InputFeeder feeder;

		feeder.m_input_buffer			= std::move( input_buffer );
		feeder.m_input_element_size		= Internal::GetElementSize( input_bitrate );
		feeder.m_input_bitrage			= input_bitrate;
		feeder.m_has_compressed_input	= true;
		feeder.Rewind();

		return feeder;
	}

	InputFeeder::InputFeeder( InputFeeder&& other ) noexcept
		: m_input_buffer{ std::move( other.m_input_buffer ) }
		, m_block_payload{ std::exchange( other.m_block_payload, nullptr ) }
		, m_block_rest_length{ std::exchange( other.m_block_rest_length, 0 ) }
		, m_input_element_size{ std::exchange( other.m_input_element_size, 0 ) }
		, m_input_bitrage{ std::exchange( other.m_input_bitrage, Internal::Bitrate::Undefined ) }
		, m_flags_buffer{ std::exchange( other.m_flags_buffer, 0 ) }
	{
	}

	InputFeeder& InputFeeder::operator=( InputFeeder&& other ) noexcept
	{
		InputFeeder temp{ std::move( other ) };
		Swap( temp );
		return *this;
	}

	void InputFeeder::Swap( InputFeeder& other )
	{
		Black::Swap( m_input_buffer, other.m_input_buffer );
		Black::Swap( m_block_payload, other.m_block_payload );
		Black::Swap( m_block_rest_length, other.m_block_rest_length );
		Black::Swap( m_input_element_size, other.m_input_element_size );
		Black::Swap( m_input_bitrage, other.m_input_bitrage );
		Black::Swap( m_flags_buffer, other.m_flags_buffer );
	}

	void InputFeeder::Rewind()
	{
		m_block_payload			= m_input_buffer.GetBegin();
		m_block_rest_length		= m_input_buffer.GetLength();
		m_is_block_compressed	= false;

		CRET( !m_has_compressed_input );
		ParseBlockHeader();
	}

	const Black::BooleanStatus InputFeeder::StepForward()
	{
		if( m_block_rest_length == 0 )
		{
			EXPECTS_DEBUG( m_has_compressed_input );

			// Payload was not shifted last time, so shift it here before accessing next block.
			m_block_payload += m_input_element_size;

			ParseBlockHeader();
			return Black::BooleanStatus::Success;
		}

		EXPECTS_DEBUG( m_block_rest_length >= m_input_element_size );
		m_block_rest_length -= m_input_element_size;

		CRET( m_is_block_compressed, Black::BooleanStatus::Success );
		m_block_payload += m_input_element_size;

		ENSURES_DEBUG( m_input_buffer.IsInside( m_block_payload ) );
		return Black::BooleanStatus::Success;
	}

	const std::byte* const InputFeeder::PeekColorBuffer() const
	{
		EXPECTS_DEBUG( m_input_buffer.IsInside( m_block_payload ) );
		return m_block_payload;
	}

	void InputFeeder::ParseBlockHeader()
	{
		constexpr uint8_t	compression_mask	= 0x80U;
		constexpr uint8_t	length_mask			= 0x7FU;

		EXPECTS_DEBUG( m_input_buffer.IsInside( m_block_payload ) );
		const uint8_t block_header = Black::GetEnumValue( *m_block_payload );

		m_is_block_compressed	= ( block_header & compression_mask ) != 0;
		m_block_rest_length		= size_t( block_header & length_mask ) * m_input_element_size;

		++m_block_payload;
		ENSURES_DEBUG( m_input_buffer.IsInside( m_block_payload ) );
	}
}
}
}
}
