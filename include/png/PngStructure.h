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
		struct Chunk final
		{
			//
			using TypeCode = Internal::ChunkTypeCode;

			//
			using PropertyBit = Internal::ChunkPropertyBit;

			//
			using Header = Internal::ChunkHeader;

			//
			using Footer = Internal::ChunkFooter;
		};

		//
		struct Image final
		{
			//
			using BitDepth = Internal::BitDepth;

			//
			using ColorType = Internal::ColorType;

			//
			using ColorTypeFlag = Internal::ColorTypeFlag;

			//
			using CompressionMethod = Internal::CompressionMethod;

			//
			using FilterMethod = Internal::FilterMethod;

			//
			using InterlaceMthod = Internal::InterlaceMethod;

			//
			using Header = Internal::ImageHeader;
		};

		//
		using ChunkEntry = Internal::ChunkEntry;
	};
}
}
}
