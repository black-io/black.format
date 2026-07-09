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

	const Black::PlainView<const JpegStructure::Marker*> JpegFileView::GetMarkers() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return { m_markers.data(), m_markers.size() };
	}

	const Black::PlainView<const JpegStructure::SegmentEntry> JpegFileView::GetSegments() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return { m_segments.data(), m_segments.size() };
	}

	const bool JpegFileView::IsValidFile() const
	{
		BLACK_LOG_FATAL( LOG_CHANNEL, "Unimplemented method!" );
		return false;
	}

	const bool JpegFileView::IsEmpty() const
	{
		return !m_is_valid || m_file_memory.empty();
	}

	void JpegFileView::InvalidateCache() const
	{
		m_markers.clear();
		m_segments.clear();

		m_is_valid	= false;
		m_is_parsed	= false;
	}

	void JpegFileView::EnsureFileMemoryParsed() const
	{
		CRET( !m_is_valid || m_is_parsed );
		ParseFileMemory();
	}

	void JpegFileView::ParseFileMemory() const
	{
		m_is_parsed = false;
		CRETW( !m_is_valid, , LOG_CHANNEL, "Unable to parse invalid memory." );
		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Perform file parsing." );

		Black::ScopeLeaveHandler reset_contract{ Black::BindMethod<&JpegFileView::InvalidateCache>( *this ) };

		Black::PlainView<const std::byte> segments_buffer{ m_file_memory };
		while( !segments_buffer.IsEmpty() )
		{
			CBRK( segments_buffer.GetLength() < sizeof( Internal::Marker ) );
		}

		BLACK_LOG_VERBOSE( LOG_CHANNEL, "File successfully parsed." );
		reset_contract.Cancel();
		m_is_parsed = true;
	}

	void JpegFileView::TestFileMemory() const
	{
		m_is_valid = false;

		CRET( !IsHeaderValid( m_file_memory ) );

		m_is_valid = true;
	}
}
}
}
