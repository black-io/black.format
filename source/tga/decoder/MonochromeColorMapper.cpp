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


	const uint32_t MonochromeColorMapper::PerformPeekElement() const
	{
		uint32_t result = 0;

		const Black::ImageFormat& input_format = GetInputFormat();
		Black::CopyMemory( &result, GetInputFeeder().PeekElement(), input_format.size_bytes );

		const uint32_t magnitude	= ( ( input_format.has_alpha )? ( result & GetInputColorMask() ) : ( result >> m_input_color_shrink_bits ) ) & 0xFFUL;
		const uint32_t alpha		= ( ( input_format.has_alpha )? ( result >> GetInputFirstAlphaBit() ) : ~uint32_t{} ) & 0xFFUL;

		const Black::ImageFormat& output_format = GetOutputFormat();

		uint8_t result_buffer[ sizeof( result ) ];
		result_buffer[ output_format.red_channel_index ]	= magnitude;
		result_buffer[ output_format.green_channel_index ]	= magnitude;
		result_buffer[ output_format.blue_channel_index ]	= magnitude;
		result_buffer[ output_format.alpha_channel_index ]	= ( output_format.has_alpha )? alpha : 0;
		Black::CopyMemory( &result, result_buffer, output_format.size_bytes );

		return result;
	}
}
}
}
}
