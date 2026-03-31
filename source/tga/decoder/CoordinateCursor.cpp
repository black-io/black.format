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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/CoordinateCursor";
}


namespace
{
	// Get the interlaced output index according to given input one and interlace settings.
	size_t GetInterlacedRowIndex( const size_t row_index, const size_t interlace_height, const size_t interlace_step )
	{
		return interlace_step * ( row_index % interlace_height ) + ( row_index / interlace_height );
	}
}


	CoordinateCursor::CoordinateCursor( const Internal::Header& header, const size_t output_width, const size_t output_height )
		: m_input_width{ header.image.width }
		, m_input_height{ header.image.height }
		, m_input_length{ m_input_width * m_input_height }
		, m_output_width{ output_width }
		, m_output_height{ output_height }
		, m_output_length{ m_output_width * m_output_height }
		, m_interlace_height{ m_input_height >> header.image.flags.interlace_mode }
		, m_interlace_step{ 1ULL << header.image.flags.interlace_mode }
		, m_output_column_step{ ( ( header.image.flags.origin_position & 0x01ULL ) == 0 )? +1 : -1 }
	{
		EXPECTS_DEBUG( m_interlace_step != 0 );
		EXPECTS_DEBUG( m_interlace_height != 0 );
		SetupFlags( header );
		FixOutputPosition();
	}

	void CoordinateCursor::Rewind()
	{
		RewindInputFeeder();

		m_input_index	= 0;
		m_input_column	= 0;
		m_input_row		= 0;

		FixOutputPosition();
	}

	void CoordinateCursor::StepForward()
	{
		ShiftInputFeeder();

		++m_input_column;
		++m_input_index;
		m_output_column += m_output_column_step;
		m_output_index += m_output_column_step;
		CRET( m_input_column < m_input_width );

		FixInputPosition();
		FixOutputPosition();
	}

	void CoordinateCursor::ResetInputFeeder()
	{
		m_input_feeder = nullptr;
	}

	void CoordinateCursor::SetInputFeeder( BasicInputFeeder& feeder )
	{
		m_input_feeder = &feeder;
	}

	void CoordinateCursor::SetupFlags( const Internal::Header& header )
	{
		m_has_horizontal_inversion	= ( header.image.flags.origin_position & 0x01U ) != 0;
		m_has_vertical_inversion	= ( header.image.flags.origin_position & 0x02U ) != 0;
		m_use_interlacing			= header.image.flags.interlace_mode != 0;
	}

	void CoordinateCursor::RewindInputFeeder()
	{
		CRET( m_input_feeder == nullptr );
		m_input_feeder->Rewind();
	}

	void CoordinateCursor::ShiftInputFeeder()
	{
		CRET( m_input_feeder == nullptr );
		m_input_feeder->StepForward();
	}

	void CoordinateCursor::FixInputPosition()
	{
		// Remember the number of input rows to shift.
		m_input_row += m_input_column / m_input_width;

		// Normalize the column index. It should be inside of input row.
		m_input_column = m_input_column % m_input_width;

		// Fix the index of input pixel.
		m_input_index = ( m_input_row * m_input_width ) + m_input_column;
	}

	void CoordinateCursor::FixOutputPosition()
	{
		const size_t last_column_index	= m_input_width - 1;
		const size_t last_row_index		= m_input_height - 1;

		// Fix output column according to settings of mirroring.
		m_output_column = ( m_has_horizontal_inversion )? ( last_column_index - m_input_column ) : m_input_column;

		// At first step interlacing applied to index of output row.
		const size_t output_row = ( m_use_interlacing )? GetInterlacedRowIndex( m_input_row, m_interlace_height, m_interlace_step ) : m_input_row;

		// At second step mirroring applied to output row index.
		m_output_row = ( m_has_vertical_inversion )? ( last_row_index - output_row ) : output_row;

		// Fix the index of output pixel according to output settings.
		m_output_index = ( m_output_row * m_output_width ) + m_output_column;
	}
}
}
}
}
