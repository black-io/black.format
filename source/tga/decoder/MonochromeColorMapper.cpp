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


	void MonochromeColorMapper::FixOutputFormat( const Internal::Header& header )
	{
		const bool has_alpha = header.image.flags.alpha_length != 0;
		SetOutputFormat( ( has_alpha )? Black::ImageFormats::A8R8G8B8 : Black::ImageFormats::R8G8B8 );

		const size_t magnitude_bits	= ( has_alpha )? size_t( Black::GetEnumValue( header.image.bitrate ) ) : GetInputFirstAlphaBit();
		m_input_color_shrink_bits	= std::max<size_t>( magnitude_bits, 8 ) - 8;
	}

	const uint32_t MonochromeColorMapper::PerformPeekElement() const
	{
		uint32_t result = 0;

		const Black::ImageFormat& input_format = GetInputFormat();
		Black::CopyMemory( &result, GetInputFeeder().PeekElement(), input_format.size_bytes );

		const uint32_t magnitude	= ( result >> m_input_color_shrink_bits ) & GetInputColorMask();
		const uint32_t alpha		= ( ( input_format.has_alpha )? ( result & GetInputAlphaMask() ) : GetInputAlphaMask() ) >> GetInputFirstAlphaBit();

		const Black::ImageFormat& output_format = GetOutputFormat();

		uint8_t result_buffer[ sizeof( result ) ];
		result_buffer[ output_format.red_channel_index ]	= magnitude;
		result_buffer[ output_format.green_channel_index ]	= magnitude;
		result_buffer[ output_format.blue_channel_index ]	= magnitude;

		if( output_format.has_alpha )
		{
			result_buffer[ output_format.alpha_channel_index ] = alpha;
		}

		Black::CopyMemory( &result, result_buffer, output_format.size_bytes );

		return result;
	}
}
}
}
}
