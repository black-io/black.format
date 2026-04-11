#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
	//
	struct Chunk final
	{
		size32_t					content_size;
		uint32_t					type_code;
		Black::PlainView<std::byte>	content;
		uint32_t					checksumm;
	};
}
}
}
}
