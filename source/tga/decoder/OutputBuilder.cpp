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
		: m_cursor{ std::exchange( other.m_cursor, nullptr ) }
		, m_output_buffer{ std::move( other.m_output_buffer ) }
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
		Black::Swap( m_cursor, other.m_cursor );
		Black::Swap( m_output_buffer, other.m_output_buffer );
		Black::Swap( m_output_format, other.m_output_format );
	}

	void OutputBuilder::RefuseCursor()
	{
		m_cursor = nullptr;
	}

	void OutputBuilder::UseCursor( const CoordinateCursor& cursor )
	{
		m_cursor = &cursor;
	}

	void OutputBuilder::UseOutputBuffer( Black::PlainView<std::byte> output_buffer )
	{
		m_output_buffer = std::move( output_buffer );
	}

	void OutputBuilder::UseOutputFormat( const Black::ColorFormat output_format )
	{
		m_output_format = output_format;
	}

	const Black::BooleanStatus OutputBuilder::ProduceElement( const uint32_t color )
	{
		CRET( m_cursor == nullptr, Black::BooleanStatus::Failure );
		EXPECTS_DEBUG( m_output_format.size_bits > 0 );

		const size_t offset = m_output_format.size_bytes * m_cursor->GetOutputIndex();
		CRET( offset >= m_output_buffer.GetLength(), Black::BooleanStatus::Failure );

		Black::CopyMemory( &m_output_buffer[ offset ], &color, m_output_format.size_bytes );
		return Black::BooleanStatus::Success;
	}
}
}
}
}
