#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	/**
		@brief	View of regualr JPEG file.

		This type can be used to verify that some particular file satisfy the basic requirements of valid JPEG file or even has
		valid internal structure of JPEG file.
		The view doesn't own given memory. It doesn't copy the memory of file nor create some expensive objects.
		It can't decode stored image or change the content of segments.
		This view just gives to user the easiest way to access the data of stored image.
	*/
	class JpegFileView final
	{
	// Friendship interface.
	public:
		friend inline void swap( JpegFileView& left, JpegFileView& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		/**
			@brief	Whether the memory of file carries valid header of JPEG format.

			This function perform the most basic checks at the beginning of given file.
			It succeeds only for valid JPEG files.

			Under the `JPEG header` is meant the few of first JIF markers and segments at the very begin of file.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case of valid JPEG header found in valid place of given memory.
		*/
		static const bool IsHeaderValid( const Black::PlainView<const std::byte>& file_memory );

		/**
			@brief	Whether the memory of file represents the valid JPEG format.

			This function does not perform the whole file parsing, but only looks for certain segments or markers in valid places it should be found.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case of valid JPEG format found in given memory.
		*/
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
		// Perform the full reset of file view. As result, the view become like just default-constructed.
		void Reset();

		// Swap the state of this view with other one.
		void Swap( JpegFileView& other );


		// Query the list of markers found in file. May return empty list in case of invalid or empty file.
		const Black::PlainView<const JpegStructure::Jif::Marker*> GetMarkers() const;

		// Query the list of segments found in file. May return empty list in case of invalid or empty file.
		const Black::PlainView<const JpegStructure::SegmentEntry> GetSegments() const;

		// Query the list of image blocks found in file. May return empty list in case of invalid or empty file.
		const Black::PlainView<const JpegStructure::ImageBlockEntry> GetImageBlocks() const;

		// Query the main header of JFIF file, that was found in first APP0 segment of file.
		const JpegStructure::Jfif::MainHeader* QueryJfifMainHeader() const;

		// Query the frame header, that was found in SOFn segment of file.
		const JpegStructure::Jif::FrameHeader* QueryFrameHeader() const;

		// Get the main header of JFIF file. Method can be used only for valid and not empty file.
		const JpegStructure::Jfif::MainHeader& GetJfifMainHeader() const;

		// Get the frame header of file. Method can be used only for valid and not empty file.
		const JpegStructure::Jif::FrameHeader& GetFrameHeader() const;


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
