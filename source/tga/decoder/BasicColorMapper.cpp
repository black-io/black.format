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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/ColorMapper";
}


	void BasicColorMapper::RefuseInputFeeder()
	{
		m_input_feeder = nullptr;
	}

	void BasicColorMapper::UseInputFeeder( BasicInputFeeder& const feeder )
	{
		m_input_feeder = &feeder;

		m_input_size = feeder.GetElementSize();
		m_input_bitrate = feeder.GetBitrate();
	}

	const uint32_t BasicColorMapper::PeekElement() const
	{
		EXPECTS_DEBUG( m_input_feeder != nullptr );
		return PerformPeekElement();
	}

	void BasicColorMapper::SetOutputFormat( const Black::ImageFormat output_format )
	{
		m_output_format = output_format;
	}
}
}
}
}
