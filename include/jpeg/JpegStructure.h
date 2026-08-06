#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	struct JpegStructure final
	{
		//
		using Marker = Internal::Marker;

		//
		using SegmentHeader = Internal::SegmentHeader;

		//
		using JfifHeader = Internal::JfifHeader;

		//
		using FrameHeader = Internal::FrameHeader;

		//
		using SegmentEntry = Internal::SegmentEntry;

		//
		using ImageBlockEntry = Internal::ImageBlockEntry;
	};
}
}
}
