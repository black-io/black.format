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


	const Black::BooleanStatus RleInputFeeder::ReadCurrentBlock() const
	{
		constexpr size_t	header_size			= sizeof( std::byte );
		constexpr uint8_t	compression_mask	= 0x80U;
		constexpr uint8_t	length_mask			= 0x7FU;

		CRET( !HasElements(), Black::BooleanStatus::Failure );
		const std::byte block_header = *PeekElement();

		m_is_block_compressed	= ( Black::GetEnumValue( block_header ) & compression_mask ) != 0;
		m_block_payload			= PeekElement() + header_size;
		m_block_position		= 0;
		m_block_length			= size_t( Black::GetEnumValue( block_header ) & length_mask ) + 1;
		m_block_size			= header_size + ( ( m_is_block_compressed )? ( GetElementSize() ) : ( GetElementSize() * m_block_length ) );

		return Black::BooleanStatus::Success;
	}

	void RleInputFeeder::OnRewind()
	{
		m_is_block_compressed	= false;
		m_block_length			= 0;
		m_block_size			= 0;
		m_block_payload			= nullptr;
		m_block_position		= 0;
	}

	const Black::BooleanStatus RleInputFeeder::OnStepForward()
	{
		++m_block_position;
		CRET( m_block_position < m_block_length, Black::BooleanStatus::Success );

		ShiftCurrentElement( m_block_size );
		return ReadCurrentBlock();
	}

	const std::byte* RleInputFeeder::PerformPeekElement() const
	{
		if( m_block_position >= m_block_length )
		{
			EXPECTS_DEBUG( m_block_position == m_block_length );
			CRET( Black::IsFailed( ReadCurrentBlock() ), nullptr );
		}

		CRET( m_is_block_compressed, m_block_payload );
		return m_block_payload + ( m_block_position * GetElementSize() );
	}
}
}
}
}
