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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/ColorConverter";
}


	const Black::BooleanStatus MonochromeColorConverter::ConvertFromMonochrome( const uint32_t color, const Black::ColorFormat color_format ) const
	{
		const Black::ColorFormatOperator& output_operator = GetOutputOperator();
		if( color_format != m_input_operator.GetFormat() )
		{
			const ColorFormat output_format	= output_operator.GetFormat();
			const uint16_t magnitude_bits	= color_format.size_bits - color_format.alpha_channel_bits;

			m_input_operator		= color_format;
			m_red_channel_shrink	= std::max( magnitude_bits, output_format.red_channel_bits ) - output_format.red_channel_bits;
			m_green_channel_shrink	= std::max( magnitude_bits, output_format.green_channel_bits ) - output_format.green_channel_bits;
			m_blue_channel_shrink	= std::max( magnitude_bits, output_format.blue_channel_bits ) - output_format.blue_channel_bits;
		}

		const uint32_t magnitude	= uint32_t( m_input_operator.MaskWhiteChannel( color ) );
		const uint32_t alpha		= uint32_t( m_input_operator.ExtractAlphaChannel( color ) );

		uint64_t converted_color = 0;
		converted_color = output_operator.InsertRedChannel( converted_color, magnitude >> m_red_channel_shrink );
		converted_color = output_operator.InsertGreenChannel( converted_color, magnitude >> m_green_channel_shrink );
		converted_color = output_operator.InsertBlueChannel( converted_color, magnitude >> m_blue_channel_shrink );
		CRET( !output_operator.CanProcessAlphaChannel(), GetOutputBuilder().ProduceElement( uint32_t( converted_color ) ) );

		if( m_input_operator.CanProcessAlphaChannel() )
		{
			converted_color = output_operator.InsertAlphaChannel( converted_color, alpha );
		}
		else
		{
			converted_color = output_operator.ReplaceAlphaChannel( converted_color, output_operator.GetAlphaChannelMask() );
		}

		return GetOutputBuilder().ProduceElement( uint32_t( converted_color ) );
	}

	const Black::BooleanStatus MonochromeColorConverter::ConvertToMonochrome( const uint32_t color, const Black::ColorFormat color_format ) const
	{
		const Black::ColorFormatOperator& output_operator = GetOutputOperator();
		if( color_format != m_input_operator.GetFormat() )
		{
			m_input_operator = color_format;
		}

		const uint64_t red			= m_input_operator.ExtractRedChannel( color );
		const uint64_t green		= m_input_operator.ExtractGreenChannel( color );
		const uint64_t blue			= m_input_operator.ExtractBlueChannel( color );
		const uint32_t alpha		= uint32_t( m_input_operator.ExtractAlphaChannel( color ) );
		const uint32_t magnitude	= uint32_t( ( red + green + blue ) / 3 );

		uint64_t converted_color = 0;
		converted_color = output_operator.InsertWhiteChannel( converted_color, magnitude );
		CRET( !output_operator.CanProcessAlphaChannel(), GetOutputBuilder().ProduceElement( uint32_t( converted_color ) ) );

		if( m_input_operator.CanProcessAlphaChannel() )
		{
			converted_color = output_operator.InsertAlphaChannel( converted_color, alpha );
		}
		else
		{
			converted_color = output_operator.ReplaceAlphaChannel( converted_color, output_operator.GetAlphaChannelMask() );
		}

		return GetOutputBuilder().ProduceElement( uint32_t( converted_color ) );
	}

	const Black::BooleanStatus MonochromeColorConverter::PerformColorConversion( const uint32_t color, const Black::ColorFormat color_format ) const
	{
		CRET( color_format.is_monochrome, ConvertFromMonochrome( color, color_format ) );
		return ConvertToMonochrome( color, color_format );
	}
}
}
}
}
