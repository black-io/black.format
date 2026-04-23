#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
	/**
		@brief	View of regular PNG file.

		This type can be used to verify that some particular file satisfy the basic requirements of valid PNG file or even has valid internal structure of PNG file.
		The view doesn't own given memory. It doesn't copy the memory of file nor create some expensive objects.
		It can't decode stored image or change the content of chunks. This view just gives to user the easiest way to access the data of stored image.
	*/
	class PngFileView final
	{
	// Friendship interface.
	public:
		friend inline void swap( PngFileView& left, PngFileView& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		/**
			@brief	Whether the memory of file carries valid header of PNG format.

			This function perform the most basic checks at the beginning of given file.
			It succeeds only for valid PNG files.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case of valid PNG header found in valid place of given memory.
		*/
		static const bool IsHeaderValid( const Black::PlainView<const std::byte>& file_memory );

		/**
			@brief	Whether the given image header satisfy the requirements of PNG file format specification.

			All requirements of image header are described in section 4.1.1 (IHDR Image header) of PNG 1.2 file format specification.

			@param	header	Given header of be checked.
			@result			`true` in case of valid values stored in given header.
		*/
		static const bool IsHeaderValid( const Black::PngStructure::Image::Header& header );

		/**
			@brief	Whether the memory of file carries valid footer of PNG format.

			This function just looks up the IEND chunk at the end of file.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case of valid PNG footer found in valid place of given memory.
		*/
		static const bool IsFooterValid( const Black::PlainView<const std::byte>& file_memory );

		/**
			@brief	Whether the memory of file represents the valid PNG format.

			This function does not perform the whole file parsing, but only looks for certain chunks in valid places it should be found.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case of valid PNG format found in given memory.
		*/
		static const bool IsFileValid( const Black::PlainView<const std::byte>& file_memory );

	// Public life-time management.
	public:
		inline PngFileView() noexcept							= default;
		inline PngFileView( const PngFileView& other ) noexcept	= default;
		PngFileView( PngFileView&& other ) noexcept;

		explicit PngFileView( Black::PlainView<const std::byte> file_memory ) noexcept;
		PngFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace ) noexcept;

		inline ~PngFileView() noexcept = default;


		inline PngFileView& operator = ( const PngFileView& other ) noexcept	= default;
		inline PngFileView& operator = ( PngFileView&& other ) noexcept			{ return Black::CopyAndSwap( *this, std::move( other ) ); };

		PngFileView& operator = ( Black::PlainView<const std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		// Perform the full reset of file view. As result, the view become like just default-constructed.
		void Reset();

		// Swap the state of this view with other one.
		void Swap( PngFileView& other );


		// Query the header of viewed file. May return `nullptr` in case of invalid or empty file.
		const PngStructure::Image::Header* const QueryHeader() const;

		// Get the header of file. Can be used only for valid and not empty file.
		const PngStructure::Image::Header& GetHeader() const;

		//
		const Black::PlainView<const std::byte> GetPaletteBuffer() const;

		//
		const Black::PlainView<const std::byte> GetImageBuffer() const;

		//
		Black::PlainView<const PngStructure::ChunkEntry> GetChunks() const;


		//
		const bool HasValidHeader() const;

		//
		const bool HasValidFooter() const;

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
		mutable std::vector<Internal::ChunkEntry>	m_cunks;
		mutable const Internal::ImageHeader*		m_header	= nullptr;
		mutable Black::PlainView<const std::byte>	m_palette;
		mutable Black::PlainView<const std::byte>	m_image;

		mutable bool	m_is_valid	= false;
		mutable bool	m_is_parsed	= false;
	};
}
}
}
