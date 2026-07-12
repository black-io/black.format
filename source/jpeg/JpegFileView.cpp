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
		constexpr size_t marker_size = sizeof( Internal::Marker );

		Black::PlainView<const std::byte> buffer{ file_memory };
		CRET( buffer.GetLength() < marker_size, false );

		const Internal::Marker& soi_marker = *reinterpret_cast<const Internal::Marker*>( buffer.GetMemory() );
		CRET( soi_marker.prefix != Internal::MARKER_PREFIX, false );
		CRET( soi_marker.code != Internal::MarkerCode::Soi, false );

		buffer = buffer.TruncatePrefix( marker_size );
		CRET( buffer.GetLength() < sizeof( Internal::SegmentHeader ), false );

		const Internal::SegmentHeader& app_segment_header = *reinterpret_cast<const Internal::SegmentHeader*>( buffer.GetMemory() );
		CRET( app_segment_header.marker.prefix != Internal::MARKER_PREFIX, false );

		buffer = buffer.TruncatePrefix( marker_size );
		CRET( size_t( app_segment_header.length ) > buffer.GetLength(), false );

		switch( app_segment_header.marker.code )
		{
		case Internal::MarkerCode::App0:
			break;
		case Internal::MarkerCode::App1:
			break;
		default:
			return false;
		}

		return true;
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
		CRETW( m_file_memory.GetLength() < sizeof( Internal::Marker ), , LOG_CHANNEL, "Size of file is too low." );

		Black::PlainView<const std::byte> segments_buffer{ m_file_memory };
		while( !segments_buffer.IsEmpty() )
		{
			CBRK( segments_buffer.GetLength() < sizeof( Internal::Marker ) );

			const Internal::Marker& marker = *reinterpret_cast<const Internal::Marker*>( segments_buffer.GetMemory() );
			CBRK( marker.prefix != Internal::MARKER_PREFIX );
			CBRK( marker.code == Internal::INVALID_CODE_1 );
			CBRK( marker.code == Internal::INVALID_CODE_2 );
			CBRK( marker.code < Internal::MIN_CODE );

			m_markers.push_back( &marker );
			segments_buffer = segments_buffer.TruncatePrefix( sizeof( Internal::Marker ) );
			CBRK( segments_buffer.GetLength() < sizeof( Internal::Marker ) );

			{
				const Internal::Marker& next_marker = *reinterpret_cast<const Internal::Marker*>( segments_buffer.GetMemory() );

				const bool has_valid_prefix	= next_marker.prefix == Internal::MARKER_PREFIX;
				const bool has_code			= ( next_marker.code != Internal::INVALID_CODE_1 ) && ( next_marker.code != Internal::INVALID_CODE_2 );
				const bool has_valid_code	= has_code && ( next_marker.code >= Internal::MIN_CODE );
				CCON( has_valid_prefix && has_valid_code );
			}

			const Internal::SegmentHeader& segment_header = reinterpret_cast<const Internal::SegmentHeader&>( marker );
			CBRK( segment_header.length > segments_buffer.GetLength() );

			Internal::SegmentEntry& segment = m_segments.emplace_back();
			segment.header	= &segment_header;
			segment.content	= segments_buffer.GetSubview( 0, segment_header.length ).TruncatePrefix( sizeof( segment_header.length ) );
			segments_buffer = segments_buffer.TruncatePrefix( segment_header.length );
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
