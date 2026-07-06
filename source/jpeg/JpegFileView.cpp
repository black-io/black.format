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
		return false;
	}
}
}
}
