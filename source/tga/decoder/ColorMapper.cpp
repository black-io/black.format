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
		ColorMapper mapper;

		mapper.m_input_operator		= Internal::SelectColorFormat( header.content_type, header.image.bitrate, header.image.flags.alpha_length );
		mapper.m_output_operator	= mapper.m_input_operator;
		mapper.m_input_bitrate		= header.image.bitrate;
		ENSURES_DEBUG( !mapper.m_input_operator.GetFormat().is_index );

		return mapper;
	}

	ColorMapper ColorMapper::SetupPaletteMapper( const Internal::Header& header, Black::PlainView<const std::byte> palette_buffer )
	{
		ColorMapper mapper;

		mapper.m_palette			= std::move( palette_buffer );
		mapper.m_input_operator		= Internal::SelectColorFormat( header.content_type, header.image.bitrate, header.image.flags.alpha_length );
		mapper.m_output_operator	= Internal::SelectColorFormat( Internal::ContentType::TrueColor, header.palette.bitrate, header.image.flags.alpha_length );
		mapper.m_input_bitrate		= header.image.bitrate;
		mapper.m_palette_bitrate	= header.palette.bitrate;
		ENSURES_DEBUG( mapper.m_input_operator.GetFormat().is_index );
		ENSURES_DEBUG( !mapper.m_output_operator.GetFormat().is_index );

		return mapper;
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

	const uint32_t ColorMapper::MapColor( const std::byte* const color_buffer ) const
	{
		uint64_t color = 0;
		Black::CopyMemory( &color, color_buffer, m_input_operator.GetFormat().size_bytes );
		CRET( !m_input_operator.GetFormat().is_index, uint32_t( color ) );

		const size_t index		= size_t( m_input_operator.MaskIndex( color ) );
		const uint64_t alpha	= m_input_operator.ExtractAlphaChannel( color );

		color = PeekPaletteElement( index );
		CRET( !m_output_operator.CanProcessAlphaChannel(), uint32_t( color ) );

		if( m_input_operator.CanProcessAlphaChannel() )
		{
			color = m_output_operator.InsertAlphaChannel( color, alpha );
		}
		else
		{
			color = m_output_operator.ReplaceAlphaChannel( color, m_output_operator.GetAlphaChannelMask() );
		}

		return uint32_t( color );
	}

	const uint32_t ColorMapper::PeekPaletteElement( const size_t element_index ) const
	{
		uint32_t color = 0;

		const size_t element_offset = element_index * m_output_operator.GetFormat().size_bytes;
		EXPECTS_DEBUG( element_offset < m_palette.GetLength() );

		Black::CopyMemory( &color, &m_palette[ element_offset ], m_output_operator.GetFormat().size_bytes );

		return color;
	}
}
}
}
}
