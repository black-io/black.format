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
	constexpr const char* LOG_CHANNEL = "Black/TGA/Decoder/InputFeeder";
}


	void StraightInputFeeder::OnRewind()
	{
	}

	const Black::BooleanStatus StraightInputFeeder::OnStepForward()
	{
		ShiftCurrentElement( GetElementSize() );
		return Black::BooleanStatus::Success;
	}
}
}
}
}
