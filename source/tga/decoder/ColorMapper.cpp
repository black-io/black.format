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


	ColorMapper ColorMapper::SetupDirectMapper( const Internal::Header& header )
	{
	}

	ColorMapper ColorMapper::SetupPaletteMapper( const Internal::Header& header, Black::PlainView<const std::byte> palette_buffer )
	{
	}

	ColorMapper::ColorMapper( ColorMapper&& other ) noexcept
		: m_palette{ std::move( other.m_palette ) }
		, m_input_operator{ std::move( other.m_input_operator ) }
		, m_output_operator{ std::move( other.m_output_operator ) }
		, m_input_bitrate{ std::exchange( other.m_input_bitrate, Internal::Bitrate::Undefined ) }
		, m_palette_bitrate{ std::exchange( other.m_palette_bitrate, Internal::Bitrate::Undefined ) }
	{
	}

	ColorMapper& ColorMapper::operator=( ColorMapper&& other ) noexcept
	{
		ColorMapper temp{ std::move( other ) };
		Swap( temp );
		return *this;
	}

	void ColorMapper::Swap( ColorMapper& other )
	{
		Black::Swap( m_palette, other.m_palette );
		Black::Swap( m_input_operator, other.m_input_operator );
		Black::Swap( m_output_operator, other.m_output_operator );
		Black::Swap( m_input_bitrate, other.m_input_bitrate );
		Black::Swap( m_palette_bitrate, other.m_palette_bitrate );
	}

	const uint32_t ColorMapper::MapColor( const std::byte* color_buffer ) const
	{
	}

	const uint32_t ColorMapper::PeekPaletteElement( const size_t element_index ) const
	{
	}
}
}
}
}
