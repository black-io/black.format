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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/OutputBuilder";
}


	const Black::BooleanStatus BasicOutputBuilder::ProduceElement( const uint32_t color )
	{
		return PerformElementProduction( color );
	}
}
}
}
}
