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
	constexpr const char* LOG_CHANNEL = "Black/TGA/ImageDecoder";
}



	TgaImageDecoder::TgaImageDecoder() noexcept = default;

	TgaImageDecoder::TgaImageDecoder( const TgaImageDecoder& other ) noexcept = default;

	TgaImageDecoder::TgaImageDecoder( TgaImageDecoder&& other ) noexcept = default;

	TgaImageDecoder::TgaImageDecoder( const TgaStructure::Header& input_header )
		: m_input_header{ input_header }
	{
	}

	TgaImageDecoder::~TgaImageDecoder() noexcept = default;

	TgaImageDecoder& TgaImageDecoder::operator=( const TgaImageDecoder& other ) noexcept = default;

	TgaImageDecoder& TgaImageDecoder::operator=( TgaImageDecoder&& other ) noexcept = default;

	TgaImageDecoder& TgaImageDecoder::operator=( const TgaStructure::Header& input_header )
	{
		return Black::CopyAndSwap( *this, input_header );
	}

	void TgaImageDecoder::Swap( TgaImageDecoder& other )
	{
		Black::Swap( m_input_header, other.m_input_header );
		Black::Swap( m_output_buffer, other.m_output_buffer );
		Black::Swap( m_output_width, other.m_output_width );
		Black::Swap( m_output_height, other.m_output_height );
		Black::Swap( m_output_row_size, other.m_output_row_size );
		Black::Swap( m_output_format, other.m_output_format );
	}

	Black::BooleanStatus TgaImageDecoder::Decode(
		const Black::PlainView<const std::byte>& image_buffer,
		const Black::PlainView<const std::byte>& palette_buffer
	)
	{
		CRETE( !TgaFileView::IsHeaderValid( m_input_header ), Black::BooleanStatus::Failure, LOG_CHANNEL, "Given TGA header is invalid." );

		m_output_width		= std::max<size_t>( m_output_width, m_input_header.image.width );
		m_output_height		= std::max<size_t>( m_output_height, m_input_header.image.height );
		m_output_row_size	= m_output_width * m_output_format.size_bytes;
		m_output_buffer.SetLength( m_output_row_size * m_output_height );
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Output image properties: {}x{} {}Bpp.", m_output_width, m_output_height, uint16_t( m_output_format.size_bits ) );
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Output image size: {}B.", m_output_buffer.GetLength() );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Content type form header: {}.", m_input_header.content_type );
		switch( m_input_header.content_type )
		{
		default:
			break;
		}

		BLACK_LOG_ERROR( LOG_CHANNEL, "Unable to decode image of given content type." );
		return Black::BooleanStatus::Failure;
	}

	TgaImageDecoder& TgaImageDecoder::SetOutputSize( const size_t width, const size_t height )
	{
		m_output_width		= width;
		m_output_height		= height;
		m_output_row_size	= m_output_width * m_output_format.size_bytes;
		return *this;
	}

	TgaImageDecoder& TgaImageDecoder::SetOutputFormat( const Black::ImageFormat format )
	{
		m_output_format		= format;
		m_output_row_size	= m_output_width * m_output_format.size_bytes;
		return *this;
	}
}
}
}
