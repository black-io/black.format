#include <black/format/jpeg.h>

#include <black/core/algorithms.h>


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

		CRET( size_t( app_segment_header.length ) < sizeof( app_segment_header.length ), false );
		const size_t header_size = app_segment_header.length - sizeof( app_segment_header.length );

		buffer = buffer.TruncatePrefix( sizeof( app_segment_header.length ) );
		bool is_app_header_valid = false;
		switch( app_segment_header.marker.code )
		{
		case Internal::MarkerCode::App0:
			{
				CRET( header_size < sizeof( Internal::JfifHeader ), false );
				const Internal::JfifHeader& jfif_header = *reinterpret_cast<const Internal::JfifHeader*>( buffer.GetMemory() );

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
				const Internal::ExifHeader& exif_header = *reinterpret_cast<const Internal::ExifHeader*>( buffer.GetMemory() );
				CRET( exif_header.tiff_header.signature != Internal::TIFF_SIGNATURE, false );

				const size_t min_ifd_offset = sizeof( Internal::ExifHeader ) - Internal::ExifHeader::IDENTIFIER_LENGTH;
				CRET( exif_header.ifd_offset < min_ifd_offset, false );

				is_app_header_valid = true;
			}
			else if(
				( header_size >= sizeof( Internal::XmpHeader ) ) &&
				Black::IsMemoryEqual( buffer.GetMemory(), Internal::XMP_HEADER_IDENTIFIER, Internal::XmpHeader::IDENTIFIER_LENGTH )
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

		constexpr size_t first_marker_index	= Black::GetEnumValue( Internal::MIN_CODE );
		constexpr size_t markers_count		= Black::GetEnumValue( Internal::MarkerCode::Com ) - first_marker_index + 1;

		size_t marker_positions[ markers_count ];
		std::fill( std::begin( marker_positions ), std::end( marker_positions ), ~size_t{} );

		Black::PlainView<const std::byte> buffer{ file_memory };
		while( !buffer.IsEmpty() )
		{
			CBRK( buffer.GetLength() < sizeof( Internal::Marker ) );

			const Internal::Marker& marker = *reinterpret_cast<const Internal::Marker*>( buffer.GetMemory() );
			CBRK( marker.prefix != Internal::MARKER_PREFIX );
			CBRK( marker.code == Internal::INVALID_CODE_1 );
			CBRK( marker.code == Internal::INVALID_CODE_2 );
			CBRK( marker.code < Internal::MIN_CODE );

			const size_t marker_position = std::distance( file_memory.GetMemory(), buffer.GetMemory() );
			marker_positions[ Black::GetEnumValue( marker.code ) - first_marker_index ] = marker_position;

			switch( marker.code )
			{
			case Internal::MarkerCode::Eoi:
				CRET( marker_positions[ Black::GetEnumValue( Internal::MarkerCode::Soi ) - first_marker_index ] >= marker_position, false );
				break;
			case Internal::MarkerCode::Sos:
				{
					const bool has_required_markers = Black::AnyOf(
						{ Internal::MarkerCode::Dht, Internal::MarkerCode::Dqt, Internal::MarkerCode::Dri },
						[&marker_positions, marker_position]( const Internal::MarkerCode code )
						{
							return marker_positions[ Black::GetEnumValue( Internal::MarkerCode::Soi ) - first_marker_index ] < marker_position;
						}
					);

					CRET( !has_required_markers, false );
				}
				break;
			default:
				break;
			}

			buffer = buffer.TruncatePrefix( sizeof( Internal::Marker ) );
			CBRK( buffer.GetLength() < sizeof( Internal::Marker ) );

			{
				const Internal::Marker& marker_candidate = *reinterpret_cast<const Internal::Marker*>( buffer.GetMemory() );

				const bool has_valid_prefix	= marker_candidate.prefix == Internal::MARKER_PREFIX;
				const bool has_code			= ( marker_candidate.code != Internal::INVALID_CODE_1 ) && ( marker_candidate.code != Internal::INVALID_CODE_2 );
				const bool has_valid_code	= has_code && ( marker_candidate.code >= Internal::MIN_CODE );
				CCON( has_valid_prefix && has_valid_code );
			}

			const Internal::SegmentHeader& segment_header = reinterpret_cast<const Internal::SegmentHeader&>( marker );
			CBRK( segment_header.length > buffer.GetLength() );

			buffer = buffer.TruncatePrefix( segment_header.length );
		}

		const Internal::MarkerCode sof_markers[] {
			Internal::MarkerCode::Sof0,		Internal::MarkerCode::Sof1,		Internal::MarkerCode::Sof2,
			Internal::MarkerCode::Sof3,		Internal::MarkerCode::Sof5,		Internal::MarkerCode::Sof6,
			Internal::MarkerCode::Sof7,		Internal::MarkerCode::Sof9,		Internal::MarkerCode::Sof10,
			Internal::MarkerCode::Sof11,	Internal::MarkerCode::Sof13,	Internal::MarkerCode::Sof14,
			Internal::MarkerCode::Sof15,
		};

		const bool has_sof_marker = Black::AnyOf(
			sof_markers,
			[&marker_positions]( const Internal::MarkerCode code )
			{
				return marker_positions[ Black::GetEnumValue( Internal::MarkerCode::Soi ) - first_marker_index ] != ~size_t{};
			}
		);

		CRET( !has_sof_marker, false );

		return true;
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
			CBRK( marker.code == Internal::MarkerCode::Eoi );
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

			switch( marker.code )
			{
			case Internal::MarkerCode::Sof0:
				[[fallthrough]];
			case Internal::MarkerCode::Sof1:
				[[fallthrough]];
			case Internal::MarkerCode::Sof2:
				[[fallthrough]];
			case Internal::MarkerCode::Sof3:
				CBRK( m_frame_header != nullptr );
				m_frame_header = reinterpret_cast<const Internal::FrameHeader*>( segment.content.GetMemory() );
				break;
			case Internal::MarkerCode::Sos:
				{
					Internal::ImageBlockEntry& image_block = m_image_blocks.emplace_back();

					image_block.scan_header		= reinterpret_cast<const Internal::ScanHeader*>( segment.content.GetMemory() );
					image_block.scan_components	= {
						reinterpret_cast<const Internal::ScanComponent*>( segment.content.GetMemory() + sizeof( Internal::ScanHeader ) ),
						size_t( image_block.scan_header->components_count )
					};
					image_block.scan_footer		= reinterpret_cast<const Internal::ScanFooter*>( image_block.scan_components.GetEnd() );

					for( size_t index = 0; index < segments_buffer.GetLength(); ++index )
					{
						CBRK( ( index + 1 ) >= segments_buffer.GetLength() );

						const Internal::Marker& candidate = *reinterpret_cast<const Internal::Marker*>( &segments_buffer.GetValueAt( index ) );
						CCON( candidate.prefix != Internal::MARKER_PREFIX );
						CCON( candidate.code == Internal::INVALID_CODE_1 );
						CCON( candidate.code == Internal::INVALID_CODE_2 );
						CCON( candidate.code < Internal::MIN_CODE );

						image_block.image = segments_buffer.GetSubview( 0, index );
						break;
					}

					segments_buffer = segments_buffer.TruncatePrefix( image_block.image.GetLength() );
				}
				break;
			case Internal::MarkerCode::App0:
				CBRK( m_jfif_header != nullptr );
				m_jfif_header = reinterpret_cast<const Internal::JfifHeader*>( segment.content.GetMemory() );
				break;
			default:
				break;
			}
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
