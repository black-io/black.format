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
		m_palette_bitrate		= bitrate;
	}

	void PaletteColorMapper::FixOutputFormat( const Internal::Header& header )
	{
		SetOutputFormat( Internal::SelectColorFormat( Internal::ContentType::TrueColor, header.palette.bitrate, header.image.flags.alpha_length ) );
	}

	const uint32_t PaletteColorMapper::PeekPaletteElement( const size_t element_index ) const
	{
		uint32_t result = 0;
		const Black::ColorFormatOperator& output_operator = GetOutputOperator();

		const size_t element_offset = element_index * output_operator.GetFormat().size_bytes;
		EXPECTS_DEBUG( element_offset < m_palette.GetLength() );

		Black::CopyMemory( &result, &m_palette[ element_offset ], output_operator.GetFormat().size_bytes );

		return result;
	}

	const uint32_t PaletteColorMapper::PerformPeekElement() const
	{
		uint64_t result = 0;

		const Black::ColorFormatOperator& input_operator = GetInputOperator();
		Black::CopyMemory( &result, GetInputFeeder().PeekElement(), input_operator.GetFormat().size_bytes );

		const uint64_t index	= size_t( input_operator.MaskIndexChannel( result ) );
		const uint64_t alpha	= input_operator.ExtractAlphaChannel( result );

		result = PeekPaletteElement( index );

		const Black::ColorFormatOperator& output_operator = GetOutputOperator();
		CRET( !output_operator.CanProcessAlphaChannel(), uint32_t( result ) );

		if( input_operator.CanProcessAlphaChannel() )
		{
			result = output_operator.InsertAlphaChannel( result, alpha );
		}
		else
		{
			result = output_operator.ReplaceAlphaChannel( result, output_operator.GetAlphaChannelMask() );
		}

		return uint32_t( result );
	}
}
}
}
}
