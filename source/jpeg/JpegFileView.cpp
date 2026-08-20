#include <black/format/jpeg.h>

#include <black/core/algorithms.h>

#include "MarkerStats.h"


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


namespace
{
	// Enumeration of JIF memory events.
	enum class FileEventId : uint32_t
	{
		Empty = 0,	// Empty event.
		Marker,		// New marker found.
		Segment,	// New segment found.
		Image,		// New image block found.
	};

	// Enumeration of event handler response.
	enum class EventHandlerResponse : bool
	{
		Abort		= false,	// Abort the file processing.
		Continue,				// Continue file processing.
	};


	// Whether the next bytes of buffer represent padding sequence. Such padding may be skipped.
	const bool IsPaddingSequence( const Black::PlainView<const std::byte>& buffer )
	{
		EXPECTS_DEBUG( buffer.GetLength() >= sizeof( Internal::Marker ) );
		return ( buffer[0] == Internal::MARKER_PREFIX ) && ( buffer[1] == Internal::MARKER_PREFIX );
	}

	// Perform the padding length counting.
	const size_t GatherPaddingLength( const Black::PlainView<const std::byte>& buffer )
	{
		EXPECTS_DEBUG( IsPaddingSequence( buffer ) );
		return std::count( buffer.begin(), buffer.end(), Internal::MARKER_PREFIX ) - 1;
	}

	// Treat the given memory as JIF marker object. Much unsafe. No checks done internally.
	const Internal::Marker& PromoteMarker( const Black::PlainView<const std::byte>& buffer )
	{
		EXPECTS_DEBUG( buffer.GetLength() >= sizeof( Internal::Marker ) );
		return *reinterpret_cast<const Internal::Marker*>( buffer.GetMemory() );
	}

	// Treat the given memory as object of JIF segment header. Much unsafe. No checks done internally.
	const Internal::SegmentHeader& PromoteSegmentHeader( const Black::PlainView<const std::byte>& buffer )
	{
		EXPECTS_DEBUG( buffer.GetLength() >= sizeof( Internal::SegmentHeader ) );
		return *reinterpret_cast<const Internal::SegmentHeader*>( buffer.GetMemory() );
	}

	// Treat the given memory as object of given segment type. Much unsafe. No checks done internally.
	template< typename TSegment >
	const TSegment& PromoteSegment( const Black::PlainView<const std::byte>& buffer, const Internal::SegmentHeader& header )
	{
		EXPECTS_DEBUG( header.length >= ( sizeof( TSegment ) + sizeof( header.length ) ) );
		EXPECTS_DEBUG( buffer.GetLength() >= sizeof( TSegment ) );
		return *reinterpret_cast<const TSegment*>( buffer.GetMemory() );
	}

	// Perform the byte-order transformation for value of EXIF format, using TIFF endianness as guide of storage endianness.
	template< typename TValue >
	const TValue GetExifValue( const TValue original_value, const Internal::TiffEndianness endianness )
	{
		switch( endianness )
		{
		case Internal::TiffEndianness::LittleEndian:
			return Black::GetTransformedEndianness<Black::BUILD_ENDIANNESS, Black::PlatformEndianness::LittleEndian>( original_value );
		case Internal::TiffEndianness::BigEndian:
			return Black::GetTransformedEndianness<Black::BUILD_ENDIANNESS, Black::PlatformEndianness::BigEndian>( original_value );
		default:
			break;
		}

		return original_value;
	}

	// Enumerate the JIF events in given memory and pass them to given handler.
	template< typename THandler >
	void EnumerateFileEvents( const Black::PlainView<const std::byte>& file_memory, THandler&& event_handler )
	{
		CRET( file_memory.GetLength() < sizeof( Internal::Marker ) );

		Black::PlainView<const std::byte> segments_buffer{ file_memory };
		while( !segments_buffer.IsEmpty() )
		{
			CBRK( segments_buffer.GetLength() < sizeof( Internal::Marker ) );

			// B.1.1.2: Any marker may optionally be preceded by any number of fill bytes, which are bytes assigned code X’FF’.
			if( IsPaddingSequence( segments_buffer ) )
			{
				segments_buffer = segments_buffer.TruncatePrefix( GatherPaddingLength( segments_buffer ) );
				continue;
			}

			const Internal::Marker& marker = PromoteMarker( segments_buffer );
			CBRK( !Internal::IsMarkerValid( marker ) );

			// Push the event.
			event_handler( FileEventId::Marker, segments_buffer.GetSubview( 0, sizeof( Internal::Marker ) ) );

			Black::PlainView<const std::byte> segment_candidate{ segments_buffer };

			segments_buffer = segments_buffer.TruncatePrefix( sizeof( Internal::Marker ) );
			CBRK( marker.code == Internal::MarkerCode::Eoi );
			CBRK( segments_buffer.GetLength() < sizeof( Internal::Marker ) );

			// Check that the segment is started here or the next marker.
			{
				const Internal::Marker& next_marker = PromoteMarker( segments_buffer );
				CCON( Internal::IsMarkerValid( next_marker ) );
			}

			const Internal::SegmentHeader& segment_header = PromoteSegmentHeader( segment_candidate );
			CBRK( segment_header.length > segments_buffer.GetLength() );

			// Push the segment.
			event_handler( FileEventId::Segment, segment_candidate.GetSubview( 0, size_t( segment_header.length ) + sizeof( Internal::Marker ) ) );

			segments_buffer = segments_buffer.TruncatePrefix( segment_header.length );
			CCON( marker.code != Internal::MarkerCode::Sos );

			// Investigate the image block.
			for( size_t index = 0; index < segments_buffer.GetLength(); ++index )
			{
				CBRK( ( index + 1 ) >= segments_buffer.GetLength() );

				// F.1.2.3:	Whenever, in the course of normal encoding, the byte value X’FF’ is created in the code string,
				//			a X’00’ byte is stuffed into the code string.
				// This rule prevent to observe the valid marker inside of image block. So the first valid marker can be found only after the block ends.
				const Internal::Marker& candidate = *reinterpret_cast<const Internal::Marker*>( &segments_buffer.GetValueAt( index ) );
				CCON( !Internal::IsMarkerValid( candidate ) );

				const size_t image_length = index;

				// Push the image block.
				event_handler( FileEventId::Image, segment_candidate.GetSubview( 0, size_t( segment_header.length ) + sizeof( Internal::Marker ) + image_length ) );
				segments_buffer = segments_buffer.TruncatePrefix( image_length );
				break;
			}
		}
	}
}


	const bool JpegFileView::IsHeaderValid( const Black::PlainView<const std::byte>& file_memory )
	{
		constexpr size_t marker_size = sizeof( Internal::Marker );

		Black::PlainView<const std::byte> buffer{ file_memory };
		CRET( buffer.GetLength() < marker_size, false );

		const Internal::Marker& soi_marker = PromoteMarker( buffer );
		CRET( soi_marker.prefix != Internal::MARKER_PREFIX, false );
		CRET( soi_marker.code != Internal::MarkerCode::Soi, false );

		buffer = buffer.TruncatePrefix( marker_size );
		CRET( buffer.GetLength() < sizeof( Internal::SegmentHeader ), false );

		const Internal::SegmentHeader& app_segment_header = PromoteSegmentHeader( buffer );
		CRET( app_segment_header.marker.prefix != Internal::MARKER_PREFIX, false );

		buffer = buffer.TruncatePrefix( marker_size );
		CRET( size_t( app_segment_header.length ) > buffer.GetLength(), false );

		CRET( size_t( app_segment_header.length ) < sizeof( app_segment_header.length ), false );
		const size_t header_size = app_segment_header.length - sizeof( app_segment_header.length );

		buffer = buffer.TruncatePrefix( sizeof( app_segment_header.length ) );
		bool is_app_header_valid = false;
		switch( app_segment_header.marker.code )
		{
		case Internal::MarkerCode::App0:
			{
				CRET( header_size < sizeof( Internal::JfifHeader ), false );
				const Internal::JfifHeader& jfif_header = PromoteSegment<Internal::JfifHeader>( buffer, app_segment_header );

				CRET( !Black::IsMemoryEqual( jfif_header.identifier, Internal::JFIF_HEADER_IDENTIFIER ), false );

				is_app_header_valid = true;
			}
			break;
		case Internal::MarkerCode::App1:
			if(
				( header_size >= sizeof( Internal::ExifHeader ) ) &&
				Black::IsMemoryEqual( buffer.GetMemory(), Internal::EXIF_HEADER_IDENTIFIER, Internal::ExifHeader::IDENTIFIER_LENGTH )
			)
			{
				const Internal::ExifHeader& exif_header = PromoteSegment<Internal::ExifHeader>( buffer, app_segment_header );
				CRET( GetExifValue( exif_header.tiff_header.signature, exif_header.tiff_header.endianness ) != Internal::TIFF_SIGNATURE, false );

				const size_t min_ifd_offset = sizeof( Internal::ExifHeader ) - Black::GetFieldOffset( &Internal::ExifHeader::tiff_header );
				CRET( GetExifValue( exif_header.ifd_offset, exif_header.tiff_header.endianness ) < min_ifd_offset, false );
				CRET( GetExifValue( exif_header.ifd_offset, exif_header.tiff_header.endianness ) > header_size, false );

				is_app_header_valid = true;
			}
			else if(
				( header_size >= sizeof( Internal::XapHeader ) ) &&
				Black::IsMemoryEqual( buffer.GetMemory(), Internal::XAP_HEADER_IDENTIFIER, Internal::XapHeader::IDENTIFIER_LENGTH )
			)
			{
				is_app_header_valid = true;
			}
			break;
		default:
			return false;
		}
		CRET( !is_app_header_valid, false );

		return true;
	}

	const bool JpegFileView::IsFileValid( const Black::PlainView<const std::byte>& file_memory )
	{
		CRET( !IsHeaderValid( file_memory ), false );

		bool has_valid_format = true;
		MarkerStats marker_stats{ file_memory };
		EnumerateFileEvents(
			file_memory,
			[&marker_stats, &has_valid_format]( const FileEventId event_id, const Black::PlainView<const std::byte>& buffer )
			{
				CRET( !has_valid_format );

				switch( event_id )
				{
				case FileEventId::Marker:
					marker_stats.LogMarker( PromoteMarker( buffer ), buffer.GetMemory() );
					break;
				case FileEventId::Segment:
					{
						const Internal::SegmentHeader& segment_header = PromoteSegmentHeader( buffer );
						switch( segment_header.marker.code )
						{
						case Internal::MarkerCode::Eoi:
							has_valid_format = marker_stats.GetPosition( Internal::MarkerCode::Soi ) < marker_stats.GetPosition( Internal::MarkerCode::Eoi );
							break;
						default:
							break;
						}
					}
					break;
				case FileEventId::Image:
					{
						const size_t marker_position = marker_stats.GetPosition( Internal::MarkerCode::Sos );

						has_valid_format = Black::AnyOf(
							{ Internal::MarkerCode::Dht, Internal::MarkerCode::Dqt, Internal::MarkerCode::Dri },
							[&marker_stats, marker_position]( const Internal::MarkerCode code )
							{
								return marker_stats.GetPosition( code ) < marker_position;
							}
						);
					}
					break;
				default:
					break;
				}
			}
		);

		CRET( !has_valid_format, false );

		const Internal::MarkerCode sof_markers[] {
			Internal::MarkerCode::Sof0,		Internal::MarkerCode::Sof1,		Internal::MarkerCode::Sof2,
			Internal::MarkerCode::Sof3,		Internal::MarkerCode::Sof5,		Internal::MarkerCode::Sof6,
			Internal::MarkerCode::Sof7,		Internal::MarkerCode::Sof9,		Internal::MarkerCode::Sof10,
			Internal::MarkerCode::Sof11,	Internal::MarkerCode::Sof13,	Internal::MarkerCode::Sof14,
			Internal::MarkerCode::Sof15,
		};

		return Black::AnyOf( sof_markers, Black::BindMethod<&MarkerStats::IsPositionValid>( marker_stats ) );
	}

	JpegFileView::JpegFileView( JpegFileView&& other ) noexcept
		: m_file_memory{ std::move( other.m_file_memory ) }
		, m_markers{ std::move( other.m_markers ) }
		, m_segments{ std::move( other.m_segments ) }
		, m_image_blocks{ std::move( other.m_image_blocks ) }
		, m_jfif_header{ std::exchange( other.m_jfif_header, nullptr ) }
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
		Black::Swap( m_image_blocks, other.m_image_blocks );
		Black::Swap( m_jfif_header, other.m_jfif_header );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_parsed, other.m_is_parsed );
	}

	const Black::PlainView<const JpegStructure::Jif::Marker*> JpegFileView::GetMarkers() const
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

	const Black::PlainView<const JpegStructure::ImageBlockEntry> JpegFileView::GetImageBlocks() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return { m_image_blocks.data(), m_image_blocks.size() };
	}

	const JpegStructure::Jfif::MainHeader* JpegFileView::QueryJfifMainHeader() const
	{
		CRET( !m_is_valid, nullptr );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, nullptr );

		return m_jfif_header;
	}

	const JpegStructure::Jif::FrameHeader* JpegFileView::QueryFrameHeader() const
	{
		CRET( !m_is_valid, nullptr );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, nullptr );

		return m_frame_header;
	}

	const JpegStructure::Jfif::MainHeader& JpegFileView::GetJfifMainHeader() const
	{
		EXPECTS( m_is_valid );

		const JpegStructure::Jfif::MainHeader* const header = QueryJfifMainHeader();
		ENSURES( header != nullptr );

		return *header;
	}

	const JpegStructure::Jif::FrameHeader& JpegFileView::GetFrameHeader() const
	{
		EXPECTS( m_is_valid );

		const JpegStructure::Jif::FrameHeader* const header = QueryFrameHeader();
		ENSURES( header != nullptr );

		return *header;
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
		m_image_blocks.clear();

		m_jfif_header	= nullptr;

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

		EnumerateFileEvents(
			m_file_memory,
			[this]( const FileEventId event_id, const Black::PlainView<const std::byte>& buffer )
			{
				switch( event_id )
				{
				case FileEventId::Marker:
					m_markers.push_back( &PromoteMarker( buffer ) );
					break;
				case FileEventId::Segment:
					{
						const Internal::SegmentHeader& segment_header = PromoteSegmentHeader( buffer );

						Internal::SegmentEntry& segment = m_segments.emplace_back();

						segment.header	= &segment_header;
						segment.content	= buffer.GetSubview( sizeof( Internal::Marker ), segment_header.length ).TruncatePrefix( sizeof( segment_header.length ) );

						ParseSegment( segment );
					}
					break;
				case FileEventId::Image:
					{
						const Internal::SegmentHeader& segment_header = PromoteSegmentHeader( buffer );
						Black::PlainView<const std::byte> segment_buffer{
							buffer.GetSubview( sizeof( Internal::Marker ), segment_header.length ).TruncatePrefix( sizeof( segment_header.length ) )
						};

						Internal::ImageBlockEntry& image_block = m_image_blocks.emplace_back();

						image_block.scan_header = &PromoteSegment<Internal::ScanHeader>( segment_buffer, segment_header );
						segment_buffer = segment_buffer.TruncatePrefix( sizeof( Internal::ScanHeader ) );

						image_block.scan_components = {
							&PromoteSegment<Internal::ScanComponent>( segment_buffer, segment_header ),
							size_t( image_block.scan_header->components_count )
						};
						segment_buffer = segment_buffer.TruncatePrefix( image_block.scan_components.GetUsedBytes() );

						image_block.scan_footer = &PromoteSegment<Internal::ScanFooter>( segment_buffer, segment_header );

						image_block.image = buffer.TruncatePrefix( std::distance( buffer.begin(), segment_buffer.end() ) );
					}
					break;
				default:
					break;
				}
			}
		);

		BLACK_LOG_VERBOSE( LOG_CHANNEL, "File successfully parsed." );
		reset_contract.Cancel();
		m_is_parsed = true;
	}

	void JpegFileView::ParseSegment( const Internal::SegmentEntry& segment ) const
	{
		EXPECTS_DEBUG( segment.header != nullptr );
		switch( segment.header->marker.code )
		{
		case Internal::MarkerCode::Sof0:
			[[fallthrough]];
		case Internal::MarkerCode::Sof1:
			[[fallthrough]];
		case Internal::MarkerCode::Sof2:
			[[fallthrough]];
		case Internal::MarkerCode::Sof3:
			CBRK( m_frame_header != nullptr );
			m_frame_header = &PromoteSegment<Internal::FrameHeader>( segment.content, *segment.header );
			break;
		case Internal::MarkerCode::App0:
			CBRK( m_jfif_header != nullptr );
			m_jfif_header = &PromoteSegment<Internal::JfifHeader>( segment.content, *segment.header );
			break;
		default:
			break;
		}
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
