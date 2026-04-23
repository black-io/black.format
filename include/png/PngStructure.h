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
		// Support of PNG chunks.
		struct Chunk final
		{
			// Enumeration of chunk type codes.
			using TypeCode = Internal::ChunkTypeCode;

			// Enumeration of chunk property bits.
			using PropertyBit = Internal::ChunkPropertyBit;

			// Header of PNG file chunk.
			using Header = Internal::ChunkHeader;

			// Footer of PNG file chunk.
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
