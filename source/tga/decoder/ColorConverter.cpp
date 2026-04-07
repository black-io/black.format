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


	ColorConverter ColorConverter::SetupRgbToMonochromeConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupMonochromeToRgbConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupDirectConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupRemappingConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter ColorConverter::SetupTransformConverter( const Internal::Header& header, const Black::ColorFormat output_format )
	{
	}

	ColorConverter::ColorConverter( ColorConverter&& other ) noexcept
		: m_input_operator{ std::move( other.m_input_operator ) }
		, m_output_operator{ std::move( other.m_output_operator ) }
	{
	}

	ColorConverter& ColorConverter::operator=( ColorConverter&& other ) noexcept
	{
		ColorConverter temp{ std::move( other ) };
		Swap( temp );
		return *this;
	}

	void ColorConverter::Swap( ColorConverter& other )
	{
		Black::Swap( m_input_operator, other.m_input_operator );
		Black::Swap( m_output_operator, other.m_output_operator );
	}

	const uint32_t ColorConverter::ConvertColor( const uint32_t color ) const
	{
		uint64_t converted_color = (this->*m_convert_method)( color );
		CRET( !m_output_operator.CanProcessAlphaChannel(), uint32_t( converted_color ) );

		if( m_input_operator.CanProcessAlphaChannel() )
		{
			const uint64_t alpha = m_input_operator.ExtractAlphaChannel( color );
			converted_color = m_output_operator.InsertAlphaChannel( converted_color, alpha );
		}
		else
		{
			converted_color = m_output_operator.ReplaceAlphaChannel( converted_color, m_output_operator.GetAlphaChannelMask() );
		}

		return uint32_t( converted_color );
	}

	const uint64_t ColorConverter::BypassColor( const uint32_t color ) const
	{
		return color;
	}

	const uint64_t ColorConverter::ConvertFromMonochrome( const uint32_t color ) const
	{
		const uint32_t magnitude = uint32_t( m_input_operator.MaskWhiteChannel( color ) );

		uint64_t converted_color = 0;
		converted_color = m_output_operator.InsertRedChannel( converted_color, magnitude );
		converted_color = m_output_operator.InsertGreenChannel( converted_color, magnitude );
		converted_color = m_output_operator.InsertBlueChannel( converted_color, magnitude );

		return converted_color;
	}

	const uint64_t ColorConverter::ConvertToMonochrome( const uint32_t color ) const
	{
		const uint64_t red			= m_input_operator.ExtractRedChannel( color );
		const uint64_t green		= m_input_operator.ExtractGreenChannel( color );
		const uint64_t blue			= m_input_operator.ExtractBlueChannel( color );
		const uint64_t magnitude	= ( red + green + blue ) / 3;

		return m_output_operator.InsertWhiteChannel( 0, magnitude );
	}

	const uint64_t ColorConverter::RebindChannels( const uint32_t color ) const
	{
		return 0;
	}

	const uint64_t ColorConverter::TransformChannels( const uint32_t color ) const
	{
		return 0;
	}
}
}
}
}
