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
	}

	const uint64_t ColorConverter::ConvertFromMonochrome( const uint32_t color ) const
	{
	}

	const uint64_t ColorConverter::ConvertToMonochrome( const uint32_t color ) const
	{
	}

	const uint64_t ColorConverter::RebindChannels( const uint32_t color ) const
	{
	}

	const uint64_t ColorConverter::TransformChannels( const uint32_t color ) const
	{
	}
}
}
}
}
