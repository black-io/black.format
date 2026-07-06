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
}
}
}
