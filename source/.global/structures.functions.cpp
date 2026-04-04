#include <black/format.h>


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	const uint64_t GetAlphaMask( const ImageFormat format )
	{
		CRET( !format.has_alpha, 0 );

		const size_t red_padding	= ( format.red_channel_index < format.alpha_channel_index )? format.red_channel_bits : 0;
		const size_t green_padding	= ( format.green_channel_index < format.alpha_channel_index )? format.green_channel_bits : 0;
		const size_t blue_padding	= ( format.blue_channel_index < format.alpha_channel_index )? format.blue_channel_bits : 0;
		const size_t shift_length	= red_padding + green_padding + blue_padding;
		const uint64_t mask			= ~( ~uint64_t{} << format.alpha_channel_bits );

		return mask << shift_length;
	}
}
}
}
