#include <black/format/jpeg.h>

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
	constexpr const char* LOG_CHANNEL = "Black/JPEG/MarkerStats";
}


	MarkerStats::MarkerStats( Black::PlainView<const std::byte> file_memory ) noexcept
		: m_file_memory{ std::move( file_memory ) }
	{
		std::fill( std::begin( m_marker_positions ), std::end( m_marker_positions ), Black::UNDEFINED_INDEX );
	}

	void MarkerStats::LogMarker( const Internal::Marker& marker, Black::NotNull<const std::byte*> memory )
	{
		CRETE( !Internal::IsMarkerCodeValid( marker.code ), , LOG_CHANNEL, "Attempt to log the marker with invalid code." );

		const size_t marker_offset = std::distance( m_file_memory.GetMemory(), memory.Get() );
		CRETE( marker_offset >= m_file_memory.GetLength(), , LOG_CHANNEL, "Attempt to log the marker from out of file bounds." );

		m_marker_positions[ GetCodeIndex( marker.code ) ] = marker_offset;
	}

	const size_t MarkerStats::GetPosition( const Internal::MarkerCode code ) const
	{
		CRETE( !Internal::IsMarkerCodeValid( code ), Black::UNDEFINED_INDEX, LOG_CHANNEL, "Attempt to request position for invalid marker code." );
		return m_marker_positions[ GetCodeIndex( code ) ];
	}

	const bool MarkerStats::IsPositionValid( const Internal::MarkerCode code ) const
	{
		CRETE( !Internal::IsMarkerCodeValid( code ), false, LOG_CHANNEL, "Attempt to check position for invalid marker code." );
		return m_marker_positions[ GetCodeIndex( code ) ] != Black::UNDEFINED_INDEX;
	}

	const size_t MarkerStats::GetCodeIndex( const Internal::MarkerCode code )
	{
		EXPECTS_DEBUG( Internal::IsMarkerCodeValid( code ) );
		return size_t( Black::GetEnumValue( code ) ) - BASE_OFFSET;
	}
}
}
}
