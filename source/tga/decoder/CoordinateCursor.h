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

		CoordinateCursor( const Internal::Header& header, const size_t output_width, const size_t output_height, const Black::ImageFormat output_format );

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
		const bool IsFinished() const;

	// Private state.
	private:
		size_t	m_source_index	= 0;
		size_t	m_source_column	= 0;
		size_t	m_source_row	= 0;

		size_t	m_target_index	= 0;
		size_t	m_target_column	= 0;
		size_t	m_target_row	= 0;
	};
}
}
}
}
