#include <black/format/tga.h>

#include "decoder/decoder.h"


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


	TgaImageDecoder::TgaImageDecoder( const TgaStructure::Header& input_header )
		: m_input_header{ input_header }
	{
	}

	TgaImageDecoder& TgaImageDecoder::operator=( const TgaStructure::Header& input_header )
	{
		return Black::CopyAndSwap( *this, input_header );
	}

	void TgaImageDecoder::ClearOutputBuffer()
	{
		m_output_buffer.Invalidate();
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
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Content type form header: {}.", m_input_header.content_type );

		m_output_width	= std::max<size_t>( m_output_width, m_input_header.image.width );
		m_output_height	= std::max<size_t>( m_output_height, m_input_header.image.height );
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Output image properties: {}x{} {}Bpp.", m_output_width, m_output_height, uint16_t( m_output_format.size_bits ) );

		m_output_row_size = m_output_width * m_output_format.size_bytes;
		m_output_buffer.SetLength( m_output_row_size * m_output_height );
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Output image size: {}B.", m_output_buffer.GetLength() );

		// Discard the result in case of any error while decoding process.
		Black::ScopeLeaveHandler reset_contract{ Black::BindMethod<&TgaImageDecoder::ClearOutputBuffer>( *this ) };

		Decoder::DecodePipeline pipeline;
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Pipeline configuration started." );
		{
			Black::BooleanStatus status = pipeline.SetupInputFeeder( m_input_header, image_buffer );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup image buffer for decoding." );

			status = pipeline.SetupColorMapper( m_input_header, palette_buffer );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup image color mapper for decoding." );

			status = pipeline.SetupColorConverter( m_input_header, m_output_format );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup output color converter for decoding." );

			status = pipeline.SetupOutputBuilder( m_output_buffer, m_output_format );
			CRETE( Black::IsFailed( status ), status, LOG_CHANNEL, "Failed to setup output color converter for decoding." );
		}
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Pipeline configuration finished." );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Decode started." );
		{
			Decoder::CoordinateCursor cursor{ m_input_header, m_output_width, m_output_height };

			CRETE( Black::IsFailed( pipeline.BeginProcessing( cursor ) ), Black::BooleanStatus::Failure, LOG_CHANNEL, "Pipeline failed to begin processing." );
			for( cursor.Rewind(); !cursor.IsFinished(); cursor.StepForward() )
			{
				CRETE( Black::IsFailed( pipeline() ), Black::BooleanStatus::Failure, LOG_CHANNEL, "Pipeline failed processing at position {}.", cursor );
			}
			CRETE( Black::IsFailed( pipeline.EndProcessing( cursor ) ), Black::BooleanStatus::Failure, LOG_CHANNEL, "Pipeline failed to finish processing." );
		}
		BLACK_LOG_DEBUG( LOG_CHANNEL, "Decode finished." );

		BLACK_LOG_DEBUG( LOG_CHANNEL, "Decode finished successfully." );
		reset_contract.Cancel();
		return Black::BooleanStatus::Success;
	}

	TgaImageDecoder& TgaImageDecoder::SetOutputResolution( const size_t width, const size_t height )
	{
		m_output_width		= width;
		m_output_height		= height;
		m_output_row_size	= m_output_width * m_output_format.size_bytes;
		return *this;
	}

	TgaImageDecoder& TgaImageDecoder::SetOutputFormat( const Black::ColorFormat format )
	{
		m_output_format		= format;
		m_output_row_size	= m_output_width * m_output_format.size_bytes;
		return *this;
	}
}
}
}
