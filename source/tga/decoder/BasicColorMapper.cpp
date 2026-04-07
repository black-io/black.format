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

	void BasicColorMapper::UseInputFeeder( InputFeeder& feeder )
	{
		m_input_feeder = &feeder;
	}

	void BasicColorMapper::UseImageSettings( const Internal::Header& header )
	{
		m_input_bitrate			= header.image.bitrate;
		m_input_operator		= Internal::SelectColorFormat( header.content_type, m_input_bitrate, header.image.flags.alpha_length );
		m_output_operator		= m_input_operator;
	}

	const uint32_t BasicColorMapper::PeekElement() const
	{
		EXPECTS_DEBUG( m_input_feeder != nullptr );
		return PerformPeekElement();
	}

	void BasicColorMapper::SetOutputFormat( const Black::ColorFormat output_format )
	{
		m_output_operator = output_format;
	}
}
}
}
}
