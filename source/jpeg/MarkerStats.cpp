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
		const size_t marker_offset = std::distance( m_file_memory.GetMemory(), memory.Get() );
		CRETE( marker_offset >= m_file_memory.GetLength(), , LOG_CHANNEL, "Attempt to log the marker from out of file bounds." );

		m_marker_positions[ GetCodeIndex( marker.code ) ] = marker_offset;
	}
}
}
}
