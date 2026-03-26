#pragma once


// Fundamental external dependencies.


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
	/**
		@brief	View of regular TGA file.

		This type can be used to test whether the arbitrary file satisfy the TGA format requirements or even get the full internal structure of TGA file.
		This view does nothing with store image. This view just gives to user the easiest way to access the data of stored image.
		`TgaImageDecoder` type can be used to decode the stored image from buffers and headers this view provide.

		The view can determine version of TGA format the viewed file was saved.
	*/
	class TgaFileView final
	{
	// Public inner types.
	public:
		// TGA format version.
		using Version = Internal::Version;

	// Friendship interface.
	public:
		friend inline void swap( TgaFileView& left, TgaFileView& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		/**
			@brief	Whether the memory of file carries valid header of TGA format.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case the valid header placed at first bytes of given memory.
		*/
		static const bool IsHeaderValid( const Black::PlainView<const std::byte>& file_memory );

		/**
			@brief	Whether the memory of file carries valid footer of TGA 2.0 format.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case the valid footer placed at last bytes of given memory.
		*/
		static const bool IsFooterValid( const Black::PlainView<const std::byte>& file_memory );

		/**
			@brief	Whether the memory of file can be safely parsed as TGA file.

			This check does not guaranties the parsing process or image decode will be succeeded. It only says that file may be parsed safely.

			@param	file_memory	Memory of file to be checked.
			@result				`true` in case the given file may be parsed as TGA file.
		*/
		static const bool IsFileValid( const Black::PlainView<const std::byte>& file_memory );

	// Public life-time management.
	public:
		TgaFileView() noexcept;
		TgaFileView( TgaFileView&& other ) noexcept;

		explicit TgaFileView( Black::PlainView<const std::byte> file_memory );
		TgaFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace );

		~TgaFileView() noexcept;


		TgaFileView& operator = ( TgaFileView&& other ) noexcept;
		TgaFileView& operator = ( Black::PlainView<const std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		// Perform the full reset of file view. As result, the view become like just default-constructed.
		void Reset();

		// Swap the state of this view with other one.
		void Swap( TgaFileView& other );


		// Query the header of viewed file. May return `nullptr` in case of invalid or empty file.
		const TgaStructure::Header* const QueryHeader() const;

		// Query the footer of viewed file. May return `nullptr` in case of invalid file or file of older version.
		const TgaStructure::Footer* const QueryFooter() const;

		// Get the header of file. Can be used only for valid and not empty file.
		const TgaStructure::Header& GetHeader() const;

		// Get the buffer of image identification data. The result may be empty if no id stored in file.
		const Black::PlainView<const std::byte> GetIdBuffer() const;

		// Get the buffer of image palette. The result may be empty if not palette provided in file.
		const Black::PlainView<const std::byte> GetPaletteBuffer() const;

		// Get the buffer of stored image. The result may be empty in case the content type in header is set to `Empty`.
		const Black::PlainView<const std::byte> GetImageBuffer() const;

		// Get the buffer of developer directory. The result may be empty in case of file was saved in earlier version.
		const Black::PlainView<const std::byte> GetDeveloperDirectoryBuffer() const;

		// Get the buffer of extension area. The result may be empty in case of file was saved in earlier version.
		const Black::PlainView<const std::byte> GetExtensionAreaBuffer() const;

		// Get the footer of file. Can be used only for valid and not empty file.
		const TgaStructure::Footer& GetFooter() const;

		// Get the TGA format version of viewed file.
		const Version GetVersion() const;


		// Whether the viewed file carries valid TGA header.
		const bool HasValidHeader() const;

		// Whether the viewed file carries valid TGA 2.0 footer.
		const bool HasValidFooter() const;

		// Whether the viewed file satisfies the conditions of valid TGA file.
		const bool IsValidFile() const;

		// Whether the TGA file is invalid or consists of no image.
		const bool IsEmpty() const;

		// Whether the TGA file has basically valid format.
		inline const bool IsValid() const	{ return m_is_valid; };

	// Private interface.
	private:
		// reset only non-state fields.
		void InvalidateCache() const;

		// Make the guarantee the underlying TGA file is parsed once.
		void EnsureFileMemoryParsed() const;

		// Perform the TGA file parsing.
		void ParseFileMemory() const;

		// Perform the quick tests for underlying TGA file to ensure the file may be parsed.
		void TestFileMemory() const;

	// Private state.
	private:
		Black::PlainView<const std::byte>	m_file_memory;	// TGA file memory.

	// Private on-state.
	private:
		mutable const Internal::Header*				m_header				= nullptr;	// Header of file, if parsed.
		mutable Black::PlainView<const std::byte>	m_id;								// Image id data.
		mutable Black::PlainView<const std::byte>	m_palette;							// Palette data.
		mutable Black::PlainView<const std::byte>	m_image;							// Image data.
		mutable Black::PlainView<const std::byte>	m_developer_directory;				// Developer directory data.
		mutable Black::PlainView<const std::byte>	m_extension_area;					// Extension area data.
		mutable const Internal::Footer*				m_footer				= nullptr;	// Footer of file, if parsed.

		mutable bool	m_is_valid	= false;	// Whether the `m_file_memory` can be parsed.
		mutable bool	m_is_parsed	= false;	// Whether the TGA file already parsed.
	};
}
}
}
