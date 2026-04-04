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


	void BasicColorConverter::RefuseOutputBuilder()
	{
		m_output_builder = nullptr;
	}

	void BasicColorConverter::UseOutputBuilder( BasicOutputBuilder& builder )
	{
		m_output_builder = &builder;
	}

	void BasicColorConverter::UseOutputFormat( const Black::ImageFormat output_format )
	{
		m_output_format = output_format;
	}

	const Black::BooleanStatus BasicColorConverter::ConvertColor( const uint32_t color, const Black::ImageFormat color_format ) const
	{
		EXPECTS_DEBUG( m_output_builder != nullptr );
		return PerformColorConversion( color, color_format );
	}
}
}
}
}
