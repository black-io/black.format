#include "decoder.h"


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Decoder
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/InputFeeder";
}


	void BasicInputFeeder::UseImageBuffer( Black::PlainView<const std::byte> image_buffer )
	{
		m_image_buffer		= std::move( image_buffer );
		m_current_element	= m_image_buffer.GetBegin();
	}

	void BasicInputFeeder::UseBitrate( const Bitrate bitrate )
	{
		m_bitrage		= bitrate;
		m_element_size	= Internal::GetElementSize( bitrate );
	}

	void BasicInputFeeder::Rewind()
	{
		m_current_element = m_image_buffer.GetBegin();
		OnRewind();
	}

	const Black::BooleanStatus BasicInputFeeder::StepForward()
	{
		CRET( !HasElements(), Black::BooleanStatus::Failure );
		CRET( Black::IsFailed( OnStepForward() ), Black::BooleanStatus::Failure );

		return Black::BooleanStatus::Success;
	}

	void BasicInputFeeder::ShiftCurrentElement( const size_t size )
	{
		m_current_element += size;
		CRET( m_image_buffer.IsInside( m_current_element ) );

		m_current_element = m_image_buffer.GetEnd();
	}
}
}
}
}
