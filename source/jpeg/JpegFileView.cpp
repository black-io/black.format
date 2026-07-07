#include <black/format/jpeg.h>


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/JPEG/FileView";
}


	const bool JpegFileView::IsHeaderValid( const Black::PlainView<const std::byte>& file_memory )
	{
		BLACK_LOG_FATAL( LOG_CHANNEL, "Unimplemented method!" );
		return false;
	}

	const bool JpegFileView::IsFileValid( const Black::PlainView<const std::byte>& file_memory )
	{
		BLACK_LOG_FATAL( LOG_CHANNEL, "Unimplemented method!" );
		return false;
	}

	JpegFileView::JpegFileView( JpegFileView&& other ) noexcept
		: m_file_memory{ std::move( other.m_file_memory ) }
		, m_markers{ std::move( other.m_markers ) }
		, m_segments{ std::move( other.m_segments ) }
		, m_is_valid{ std::exchange( other.m_is_valid, false ) }
		, m_is_parsed{ std::exchange( other.m_is_parsed, false ) }
	{
	}

	JpegFileView::JpegFileView( Black::PlainView<const std::byte> file_memory ) noexcept
		: m_file_memory{ std::move( file_memory ) }
	{
		TestFileMemory();
	}

	JpegFileView::JpegFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace ) noexcept
		: JpegFileView{ std::move( file_memory ) }
	{
		EnsureFileMemoryParsed();
	}

	JpegFileView& JpegFileView::operator=( Black::PlainView<const std::byte> file_memory ) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( file_memory ) );
	}

	void JpegFileView::Reset()
	{
		m_file_memory = {};
		InvalidateCache();
	}

	void JpegFileView::Swap( JpegFileView& other )
	{
		Black::Swap( m_file_memory, other.m_file_memory );
		Black::Swap( m_markers, other.m_markers );
		Black::Swap( m_segments, other.m_segments );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_parsed, other.m_is_parsed );
	}

	const Black::PlainView<const Internal::Marker*> JpegFileView::GetMarkers() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return { m_markers.data(), m_markers.size() };
	}

	const Black::PlainView<const Internal::SegmentEntry> JpegFileView::GetSegments() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return { m_segments.data(), m_segments.size() };
	}
}
}
}
