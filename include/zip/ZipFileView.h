#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
	/**
		@brief	View of regular ZIP file.

		This type support only basic ZIP file format. Not the Zip64.
		The meaning of this type is to directly scan all the file forwardly and build the FAT of ZIP file.
		Central directory is ignored due to nontrivial lookup in case the file comment supplied in ZIP file.

		As result, the view of ZIP file supplies collection of file entries to be operated somehow.
		No decryption algorithms supplied. No compression algorithms supplied. Compressed or encrypted files should be processed by client code.

		Object of file view should be used to analyze the memory of ZIP file. It may be used only with fully loaded file.
		The best way to use file views is to combine it with file mappings.
	*/
	class ZipFileView final : private Black::NonCopyable
	{
	// Public inner types.
	public:
		// Entry point of file as described in section 4.3.7 (Local file header).
		using LocalFileEntry = Internal::LocalFileEntry;

		// Zip file extra data as declared in section 4.3.11 (Archive extra data record).
		using DecryptionDataEntry = Internal::ArchiveDecryptionExtraData;

		// Central directory digital signature as described in section 4.3.13 (Digital signature).
		using DigitalSignatureEntry = Internal::CentralDirectoryDigitalSignature;

		// Central directory footer entry as described in section 4.3.16 (End of central directory record).
		using FooterEntry = Internal::EndOfCentralDirectoryRecord;

		//
		using ConstIterator = std::vector<LocalFileEntry>::const_iterator;

	// Friendship interface.
	public:
		friend inline void swap( ZipFileView& left, ZipFileView& right )	{ left.Swap( right ); };

		friend inline ConstIterator begin( const ZipFileView& instance )	{ return instance.GetBegin(); };
		friend inline ConstIterator end( const ZipFileView& instance )		{ return instance.GetEnd(); };

	// Public life-time management.
	public:
		ZipFileView() noexcept;
		ZipFileView( ZipFileView&& other ) noexcept;

		explicit ZipFileView( Black::PlainView<std::byte> file_memory );

		~ZipFileView() noexcept;


		ZipFileView& operator = ( ZipFileView&& other ) noexcept;
		ZipFileView& operator = ( Black::PlainView<std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		// Perform the full reset of file view. As result, the view become like just default-constructed.
		void Reset();

		// Swap the state of this view with other one.
		void Swap( ZipFileView& other );


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
		// Locate the data descriptor record of local file entry.
		std::shared_ptr<Internal::FileDataDescriptor> LocateDataDescriptor( const Black::PlainView<std::byte>& memory ) const;

		// Parse the local file entry in memory of ZIP file.
		std::optional<Black::PlainView<std::byte>> ParseFileEntry( Black::PlainView<std::byte>&& memory ) const;

		// Parse the archive extra data entry in memory of ZIP file.
		std::optional<Black::PlainView<std::byte>> ParseExtraDataEntry( Black::PlainView<std::byte>&& memory ) const;

		// Parse the central directory entry of local file in memory of ZIP file.
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryEntry( Black::PlainView<std::byte>&& memory ) const;

		// Parse the digital signature entry of central directory in memory of ZIP file.
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryDigitalSignature( Black::PlainView<std::byte>&& memory ) const;

		// Parse the end of central directory in memory of ZIP file.
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryFooter( Black::PlainView<std::byte>&& memory ) const;

	// Private state.
	private:
		Black::PlainView<std::byte>	m_file_memory;					// ZIP file memory.

	// Private on-state.
	private:
		mutable std::vector<LocalFileEntry>	m_entries;				// FAT of ZIP file. Consists of all found local file headers.
		mutable	DecryptionDataEntry			m_decryption_entry;		// ZIP file decryption header.
		mutable DigitalSignatureEntry		m_digital_signature;	// Central directory digital signature.
		mutable FooterEntry					m_footer;				// Central directory footer.

		mutable bool	m_is_valid	= false;						// Whether the `m_file_memory` can be parsed.
		mutable bool	m_is_parsed	= false;						// Whether the ZIP file already parsed.
	};
}
}
}
