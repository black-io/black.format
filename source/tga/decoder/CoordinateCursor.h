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
		@brief	Image-space coordinate cursor.

		Decoding pipeline operates over two images at same time. Single color at some position of input image should be placed into output image,
		but the coordinates of that color may differ from input to output.
		At the same time, the input and output positions of same color should be calculated sequentially.
		Moreover, the color position of output image strongly depends on position of that color in input image.

		Coordinate cursor takes all the stress about settings and dependencies between coordinates for input and output image.
		Cursor should be configured before being used. Such configuration should be performed via initialization.

		Cursor can accept the instance of input feeder. In this case the cursor will properly shift the input feeder in same time
		after the input position shifted.
	*/
	class CoordinateCursor final
	{
	// Friendship declarations.
	public:
		friend std::string format_as( const CoordinateCursor& cursor );

	// Public life-time management.
	public:
		inline CoordinateCursor() noexcept									= delete;
		inline CoordinateCursor( const CoordinateCursor& other ) noexcept	= default;
		inline CoordinateCursor( CoordinateCursor&& other ) noexcept		= default;

		CoordinateCursor( const Internal::Header& header, const size_t output_width, const size_t output_height );

		~CoordinateCursor() noexcept	= default;

		inline CoordinateCursor& operator = ( const CoordinateCursor& other ) noexcept	= default;
		inline CoordinateCursor& operator = ( CoordinateCursor&& other ) noexcept		= default;

	// Public interface.
	public:
		// Rewind the cursor.
		void Rewind();

		// Shift the cursor to next position.
		void StepForward();


		// Instruct the cursor to stop the using of previously given input feeder.
		void RefuseInputFeeder();

		// Instruct the cursor to use given input feeder.
		void UseInputFeeder( InputFeeder& feeder );


		// Get the ordinal index of color in space of input image.
		inline const size_t GetInputIndex() const	{ return m_input_index; };

		// Get the column (X-axis) coordinate of color in space of input image.
		inline const size_t GetInputColumn() const	{ return m_input_column; };

		// Get the row (Y-axis) coordinate of color in space of input image.
		inline const size_t GetInputRow() const		{ return m_input_row; };

		// Get the ordinal index of color in space of output image.
		inline const size_t GetOutputIndex() const	{ return m_output_index; };

		// Get the column (X-axis) coordinate of color in space of output image.
		inline const size_t GetOutputColumn() const	{ return m_output_column; };

		// Get the row (Y-axis) coordinate of color in space of output image.
		inline const size_t GetOutputRow() const	{ return m_output_row; };


		// Whether the cursor is finished. It considered finished at the moment all input colors was observed.
		inline const bool IsFinished() const		{ return m_input_index == m_input_length; };

	// Private interface.
	private:
		// Perform the configuration of boolean flags in state of cursor.
		void SetupFlags( const Internal::Header& header );

		// Safe rewind of input feeder. Does nothing in case the input feeder didn't set before.
		void RewindInputFeeder();

		// Safe shifting of input feeder. Does nothing in case the input feeder didn't set before.
		void ShiftInputFeeder();

		// Perform calculations to normalize the input position after it moved.
		void FixInputPosition();

		// Perform calculations to normalize the output position after it moved.
		void FixOutputPosition();

	// Private state.
	private:
		InputFeeder* m_input_feeder	= nullptr;		// Optional input feeder.

		size_t		m_input_width			= 0;	// Width of input image. In pixels.
		size_t		m_input_height			= 0;	// Height of input image. In pixels.
		size_t		m_input_length			= 0;	// Total number of pixels in input image.

		size_t		m_output_width			= 0;	// Width of output image. In pixels.
		size_t		m_output_height			= 0;	// Height of output image. In pixels.
		size_t		m_output_length			= 0;	// Total number of pixels in output image.

		size_t		m_interlace_height		= 0;	// Count of input image rows in single group.
		size_t		m_interlace_step		= 0;	// Count of output image rows to skip before reading next row from input group.
		ptrdiff_t	m_output_column_step	= 0;	// Count (with direction) of pixels in row of output image to be skipped on shifting.

		size_t		m_input_index			= 0;	// Current ordinal index of pixel in input image.
		size_t		m_input_column			= 0;	// Current X-position in spae of input image.
		size_t		m_input_row				= 0;	// Current Y-position in spae of input image.

		size_t		m_output_index			= 0;	// Current ordinal index of pixel in output image.
		size_t		m_output_column			= 0;	// Current X-position in spae of output image.
		size_t		m_output_row			= 0;	// Current Y-position in spae of output image.

		union
		{
			size_t		m_flag_bits					= 0;	// Buffer for boolean flags.

			struct
			{
				bool	m_has_horizontal_inversion	: 1;	// Whether the output space mirrored horizontally from input space.
				bool	m_has_vertical_inversion	: 1;	// Whether the output space mirrored vertically from input space.
				bool	m_use_interlacing			: 1;	// Whether the input space is split on groups of rows.
			};
		};
	};
}
}
}
}
