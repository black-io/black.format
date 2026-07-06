#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	class JpegFileView final
	{
	// Public static interface.
	public:
		//
		static const bool IsHeaderValid( const Black::PlainView<const std::byte>& file_memory );

		//
		static const bool IsFileValid( const Black::PlainView<const std::byte>& file_memory );

	// Public life-time management.
	public:
		inline JpegFileView() noexcept						= default;
		inline JpegFileView( const JpegFileView& other )	= default;
		JpegFileView( JpegFileView&& other ) noexcept;

		explicit JpegFileView( Black::PlainView<const std::byte> file_memory ) noexcept;
		JpegFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace ) noexcept;

		inline ~JpegFileView() noexcept = default;


		inline JpegFileView& operator = ( const JpegFileView& other )		= default;
		inline JpegFileView& operator = ( JpegFileView&& other ) noexcept	{ return Black::CopyAndSwap( *this, std::move( other ) ); };

		JpegFileView& operator = ( Black::PlainView<const std::byte> file_memory ) noexcept;

	// Private state.
	private:
		Black::PlainView<const std::byte>	m_file_memory;

	// Private non-state.
	private:
		mutable std::vector<const Internal::Marker*>	m_markers;
		mutable std::vector<Internal::SegmentEntry>		m_segments;

		mutable bool	m_is_valid	= false;
		mutable bool	m_is_parsed	= false;
	};
}
}
}
