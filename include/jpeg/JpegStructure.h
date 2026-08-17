#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	/**
		@brief	Representation of JPEG file internal structure.

		Here are collected internal types of JPEG file format. All of it aggregated in certain structures by its meaning.
		In any case any of internal JPEG-file types needed by consumer's code, it may be accessed via `JpegStructure`.

		JPEG file format is complex a lot, since it is not the single file format. Different file format are hidden under the hood of single 'jpg' extension.
		So more than some single document is required to understand internals of JPEG file.
	*/
	struct JpegStructure final
	{
		// Types of JIF (JPEG Interchange Format).
		struct Jif final
		{
			// Prefix of valid marker.
			static inline constexpr std::byte MARKER_PREFIX = Internal::MARKER_PREFIX;

			// Enumeration of marker codes.
			using MarkerCode = Internal::MarkerCode;

			// Format of regular marker.
			using Marker = Internal::Marker;

			// Format of regular segment header.
			using SegmentHeader = Internal::SegmentHeader;
		};

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
