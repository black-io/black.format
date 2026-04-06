#include <black/format/tga.h>


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/ImageEncoder";
}


	TgaImageEncoder::TgaImageEncoder( const TgaStructure::Header& output_header )
		: m_output_header{ output_header }
	{
	}

	void TgaImageEncoder::ClearOutputBuffer()
	{
		m_output_buffer.Invalidate();
	}

	void TgaImageEncoder::Swap( TgaImageEncoder& other )
	{
		Black::Swap( m_output_header, other.m_output_header );
		Black::Swap( m_output_buffer, other.m_output_buffer );
	}

	Black::BooleanStatus TgaImageEncoder::Encode( const Black::PlainView<const std::byte>& image_buffer, const Black::ColorFormat image_format )
	{
		BLACK_LOG_FATAL( LOG_CHANNEL, "Unimplemented method used." );
		return Black::BooleanStatus::Failure;
	}
}
}
}
