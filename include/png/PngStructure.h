#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
	/**
		@brief	Representation of PNG file internal structure.

		Here are collected internal types of PNG file format. All of it aggregated in certain structures, as PNG 1.2 file format specification describes.
		In any case any of internal PNG-file types needed by consumer's code, it may be accessed via `PngStructure`.
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
