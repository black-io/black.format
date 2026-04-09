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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/OutputBuilder";
}


	OutputBuilder::OutputBuilder( OutputBuilder&& other ) noexcept
		: m_output_buffer{ std::move( other.m_output_buffer ) }
		, m_output_format{ std::exchange( other.m_output_format, Black::ColorFormats::UNDEFINED ) }
	{
	}

	OutputBuilder& OutputBuilder::operator=( OutputBuilder&& other ) noexcept
	{
		OutputBuilder temp{ std::move( other ) };
		Swap( temp );
		return *this;
	}

	void OutputBuilder::Swap( OutputBuilder& other )
	{
		Black::Swap( m_output_buffer, other.m_output_buffer );
		Black::Swap( m_output_format, other.m_output_format );
	}

	void OutputBuilder::ProduceElement( const uint32_t color, const CoordinateCursor& cursor ) const
	{
		EXPECTS_DEBUG( m_output_format.size_bits > 0 );

		const size_t offset = m_output_format.size_bytes * cursor.GetOutputIndex();
		ENSURES_DEBUG( offset < m_output_buffer.GetLength() );

		Black::CopyMemory( &m_output_buffer[ offset ], &color, m_output_format.size_bytes );
	}
}
}
}
}
