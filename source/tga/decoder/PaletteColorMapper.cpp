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


	void PaletteColorMapper::UsePalette( Black::PlainView<const std::byte> palette_buffer, const Internal::Bitrate bitrate )
	{
		m_palette				= std::move( palette_buffer );
		m_bitrate				= bitrate;
		m_palete_element_size	= Internal::GetElementSize( m_bitrate );
	}

	const uint32_t PaletteColorMapper::PeekPaletteElement( const size_t element_index ) const
	{
		uint32_t result = 0;

		const size_t element_offset = element_index * m_palete_element_size;
		EXPECTS_DEBUG( element_offset < m_palette.GetLength() );

		Black::CopyMemory( &result, &m_palette[ element_offset ], m_palete_element_size );

		return result;
	}

	const uint32_t PaletteColorMapper::PerformPeekElement() const
	{
		uint32_t result = 0;

		const Black::ColorFormat& input_format = GetInputFormat();
		Black::CopyMemory( &result, GetInputFeeder().PeekElement(), input_format.size_bytes );

		const size_t index		= ( input_format.has_alpha )? ( result & GetInputColorMask() ) : result;
		const uint32_t alpha	= ( ( input_format.has_alpha )? ( result >> GetInputFirstAlphaBit() ) : ~uint32_t{} ) & 0xFFUL;

		result = PeekPaletteElement( index );
		if( input_format.has_alpha )
		{
			result = result | ( ( alpha << m_output_first_alpha_bit ) & m_output_alpha_mask );
		}

		return result;
	}
}
}
}
}
