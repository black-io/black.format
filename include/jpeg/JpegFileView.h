#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	class JpegFileView final
	{
	// Friendship interface.
	public:
		friend inline void swap( JpegFileView& left, JpegFileView& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		//
		static const bool IsHeaderValid( const Black::PlainView<const std::byte>& file_memory );

		//
		static const bool IsFileValid( const Black::PlainView<const std::byte>& file_memory );

	// Public life-time management.
	public:
		inline JpegFileView() noexcept						= default;
		inline JpegFileView( const JpegFileView& other )	= default;
		JpegFileView( JpegFileView&& other ) noexcept;

		explicit JpegFileView( Black::PlainView<const std::byte> file_memory ) noexcept;
		JpegFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace ) noexcept;

		inline ~JpegFileView() noexcept = default;


		inline JpegFileView& operator = ( const JpegFileView& other )		= default;
		inline JpegFileView& operator = ( JpegFileView&& other ) noexcept	{ return Black::CopyAndSwap( *this, std::move( other ) ); };

		JpegFileView& operator = ( Black::PlainView<const std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		//
		void Reset();

		//
		void Swap( JpegFileView& other );


		//
		const Black::PlainView<const JpegStructure::Jif::Marker*> GetMarkers() const;

		//
		const Black::PlainView<const JpegStructure::SegmentEntry> GetSegments() const;

		//
		const Black::PlainView<const JpegStructure::ImageBlockEntry> GetImageBlocks() const;

		//
		const JpegStructure::Jfif::MainHeader* QueryJfifMainHeader() const;

		//
		const JpegStructure::Jif::FrameHeader* QueryFrameHeader() const;

		//
		const JpegStructure::JfifHeader& GetJfifHeader() const;

		//
		const JpegStructure::FrameHeader& GetFrameHeader() const;


		//
		const bool IsValidFile() const;

		//
		const bool IsEmpty() const;

		//
		inline const bool IsValid() const	{ return m_is_valid; };

	// Private interface.
	private:
		//
		void InvalidateCache() const;

		//
		void EnsureFileMemoryParsed() const;

		//
		void ParseFileMemory() const;

		//
		void TestFileMemory() const;

	// Private state.
	private:
		Black::PlainView<const std::byte>	m_file_memory;

	// Private non-state.
	private:
		mutable std::vector<const Internal::Marker*>	m_markers;
		mutable std::vector<Internal::SegmentEntry>		m_segments;
		mutable std::vector<Internal::ImageBlockEntry>	m_image_blocks;

		mutable const Internal::JfifHeader*		m_jfif_header	= nullptr;
		mutable const Internal::FrameHeader*	m_frame_header	= nullptr;

		mutable bool	m_is_valid	= false;
		mutable bool	m_is_parsed	= false;
	};
}
}
}
