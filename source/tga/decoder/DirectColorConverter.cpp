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


	const Black::BooleanStatus DirectColorConverter::PerformColorConversion( const uint32_t color, const Black::ColorFormat color_format ) const
	{
		uint64_t converted_color = color;

		const Black::ColorFormatOperator& output_operator = GetOutputOperator();
		if( !color_format.has_alpha && output_operator.CanProcessAlphaChannel() )
		{
			converted_color = output_operator.ReplaceAlphaChannel( converted_color, output_operator.GetAlphaChannelMask() );
		}

		return GetOutputBuilder().ProduceElement( uint32_t( converted_color ) );
	}
}
}
}
}
