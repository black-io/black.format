#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
	/**
	*/
	struct PngStructure final
	{
		//
		using ChunkHeader = Internal::ChunkHeader;

		//
		using ChunkFooter = Internal::ChunkFooter;

		//
		using Header = Internal::ImageHeader;

		//
		using ChunkEntry = Internal::ChunkEntry;
	};
}
}
}
