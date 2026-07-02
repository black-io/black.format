#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
	struct Marker final
	{
		std::byte	prefix;
		MarkerCode	code;
	};
}
}
}
}
