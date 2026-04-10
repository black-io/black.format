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
	struct ChunkHeader final
	{
		size32_t	content_size;
		uint32_t	type_code;
	};

	//
	struct Chunk final
	{
		ChunkHeader					header;
		Black::PlainView<std::byte>	content;
		uint32_t					checksumm;
	};
}
}
}
}
