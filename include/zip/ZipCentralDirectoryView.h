#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
	/**
	*/
	class ZipCentralDirectoryView final : private Black::NonCopyable
	{
	// Public inner types.
	public:
		//
		using EndHeader = Internal::EndOfCentralDirectoryHeader;

		//
		using FileHeader = Internal::CentralDirectoryFileHeader;

		// Central directory footer entry as described in section 4.3.16 (End of central directory record).
		using FooterEntry = Internal::EndOfCentralDirectoryRecord;

		//
		using LocalFileEntry = Internal::LocalFileEntry;

		//
		using ConstIterator = std::vector<LocalFileEntry>::const_iterator;

	// Friendship interface.
	public:
		friend inline void swap( ZipCentralDirectoryView& left, ZipCentralDirectoryView& right )	{ left.Swap( right ); };

		friend inline ConstIterator begin( const ZipCentralDirectoryView& instance )				{ return instance.GetBegin(); };
		friend inline ConstIterator end( const ZipCentralDirectoryView& instance )					{ return instance.GetEnd(); };

	// Public life-time management.
	public:
		ZipCentralDirectoryView() noexcept;
		ZipCentralDirectoryView( ZipCentralDirectoryView&& other ) noexcept;

		explicit ZipCentralDirectoryView( Black::PlainView<std::byte> file_memory );
		ZipCentralDirectoryView( Black::PlainView<std::byte> file_memory, const size_t central_directory_length );

		~ZipCentralDirectoryView() noexcept;


		ZipCentralDirectoryView& operator = ( ZipCentralDirectoryView&& other ) noexcept;
		ZipCentralDirectoryView& operator = ( Black::PlainView<std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		// Perform the full reset of file view. As result, the view become like just default-constructed.
		void Reset();

		// Swap the state of this view with other one.
		void Swap( ZipCentralDirectoryView& other );


		/**
			@brief	Perform the local file lookup in underlying ZIP file.

			@param	entry_name	File path as it stored in ZIP file.
			@return				The value returned is hypothetical reference to found file entry.
		*/
		Black::Hypothetical<const LocalFileEntry&> FindEntry( const std::string_view entry_name ) const;


		// Get the number of local file entries located in ZIP file.
		const size_t GetEntriesCount() const;

		// Query the file entry by ordinal index. `nullptr` will be returned in case the given index is invalid.
		const LocalFileEntry* QueryEntry( const size_t entry_index ) const;

		// Get the reference to local file entry by ordinal index. The given index should be valid.
		const LocalFileEntry& GetEntry( const size_t entry_index ) const;

		//
		ConstIterator GetBegin() const;

		//
		ConstIterator GetEnd() const;


		// Whether the ZIP file consists of file entries.
		const bool HasEntries() const;

		// Whether the ZIP file has valid format.
		inline const bool IsValid() const											{ return m_is_valid; };


		inline const LocalFileEntry& operator [] ( const size_t entry_index ) const	{ return GetEntry( entry_index ); };

	// Private interface.
	private:
		// Make the guarantee the underlying ZIP file is parsed once.
		void EnsureFileMemoryParsed() const;

		// Perform the ZIP file parsing.
		void ParseFileMemory() const;

		// Perform the quick tests for underlying ZIP file to ensure the file may be parsed.
		void TestFileMemory() const;

	// .ZIP format pipeline.
	private:
		// Parse the central directory entry of local file in memory of ZIP file.
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryEntry( Black::PlainView<std::byte>&& memory ) const;

		// Parse the end of central directory in memory of ZIP file.
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryFooter( Black::PlainView<std::byte>&& memory ) const;

	// Private state.
	private:
		Black::PlainView<std::byte>	m_file_memory;											// ZIP file memory.
		size_t						m_central_directory_length	= Black::UNDEFINED_INDEX;	// Length of central directory.

	// Private on-state.
	private:
		mutable std::vector<LocalFileEntry>	m_entries;	// FAT of ZIP file. Consists of all found local file headers.
		mutable FooterEntry					m_footer;	// Central directory footer.

		mutable bool	m_is_valid	= false;			// Whether the `m_file_memory` can be parsed.
		mutable bool	m_is_parsed	= false;			// Whether the ZIP file already parsed.
	};
}
}
}
