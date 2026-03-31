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
	class CoordinateCursor final
	{
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
		//
		void Rewind();

		//
		void StepForward();


		//
		void ResetInputFeeder();

		//
		void SetInputFeeder( BasicInputFeeder& feeder );


		//
		inline const size_t GetInputIndex() const	{ return m_input_index; };

		//
		inline const size_t GetInputColumn() const	{ return m_input_column; };

		//
		inline const size_t GetInputRow() const		{ return m_input_row; };

		//
		inline const size_t GetOutputIndex() const	{ return m_output_index; };

		//
		inline const size_t GetOutputColumn() const	{ return m_output_column; };

		//
		inline const size_t GetOutputRow() const	{ return m_output_row; };


		//
		inline const bool IsFinished() const		{ return m_input_index == m_input_length; };

	// Private interface.
	private:
		//
		void SetupFlags( const Internal::Header& header );

		//
		void RewindInputFeeder();

		//
		void ShiftInputFeeder();

		//
		void FixInputPosition();

		//
		void FixOutputPosition();

	// Private state.
	private:
		BasicInputFeeder* m_input_feeder	= nullptr;

		size_t		m_input_width			= 0;
		size_t		m_input_height			= 0;
		size_t		m_input_length			= 0;

		size_t		m_output_width			= 0;
		size_t		m_output_height			= 0;
		size_t		m_output_length			= 0;

		size_t		m_interlace_height		= 0;
		size_t		m_interlace_step		= 0;
		ptrdiff_t	m_output_column_step	= 0;

		size_t		m_input_index			= 0;
		size_t		m_input_column			= 0;
		size_t		m_input_row				= 0;

		size_t		m_output_index			= 0;
		size_t		m_output_column			= 0;
		size_t		m_output_row			= 0;

		union
		{
			size_t		m_flag_bits					= 0;

			struct
			{
				bool	m_has_horizontal_inversion	: 1;
				bool	m_has_vertical_inversion	: 1;
				bool	m_use_interlacing			: 1;
			};
		};
	};
}
}
}
}
