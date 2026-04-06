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

	void BasicColorMapper::UseInputFeeder( BasicInputFeeder& feeder )
	{
		m_input_feeder = &feeder;
	}

	void BasicColorMapper::UseImageSettings( const Internal::Header& header )
	{
		m_input_bitrate			= header.image.bitrate;
		const size_t bits_count	= Black::GetEnumValue( m_input_bitrate );

		m_input_size			= Internal::GetElementSize( m_input_bitrate );
		m_input_first_alpha_bit	= bits_count - header.image.flags.alpha_length;
		m_input_color_mask		= ~( ~uint32_t{} << m_input_first_alpha_bit );
		m_input_alpha_mask		= uint32_t( uint64_t( ~uint32_t{} << m_input_first_alpha_bit ) & ~uint64_t( ~uint32_t{} << bits_count ) );

		m_input_format			= Internal::SelectColorFormat( header.content_type, m_input_bitrate, header.image.flags.alpha_length );
		m_output_format			= m_input_format;
	}

	const uint32_t BasicColorMapper::PeekElement() const
	{
		EXPECTS_DEBUG( m_input_feeder != nullptr );
		return PerformPeekElement();
	}

	void BasicColorMapper::SetOutputFormat( const Black::ColorFormat output_format )
	{
		m_output_format = output_format;
	}
}
}
}
}
