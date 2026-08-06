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
		using SegmentEntry = Internal::SegmentEntry;

		//
		using ImageBlockEntry = Internal::ImageBlockEntry;
	};
}
}
}
