#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
	/**
		@brief	View of ZIP file.
	*/
	class ZipFileView final : private Black::NonCopyable
	{
	// Public inner types and friendship declarations.
	public:
		// Entry point of file inside of ZIP format.
		using Entry = Internal::ZipFileEntry;

	// Friendship interface.
	public:
		friend inline void swap( ZipFileView& left, ZipFileView& right ) { left.Swap( right ); };

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
		//
		void Reset();

		//
		void Swap( ZipFileView& other );


		//
		Black::Hypothetical<const Entry&> FindEntry( const std::string_view entry_name ) const;


		//
		const size_t GetEntriesCount() const;

		//
		const Entry* QueryEntry( const size_t entry_index ) const;

		//
		const Entry& GetEntry( const size_t entry_index ) const;


		//
		const bool HasEntries() const;

		//
		inline const bool IsValid() const									{ return m_is_valid; };


		inline const Entry& operator [] ( const size_t entry_index ) const	{ return GetEntry( entry_index ); };

	// Private interface.
	private:
		//
		void EnsureFileMemoryParsed() const;

		//
		void ParseFileMemory() const;

		//
		void TestFileMemory() const;

	// .ZIP format pipeline.
	private:
		//
		std::shared_ptr<Internal::FileDataDescriptor> LocateDataDescriptor( const Black::PlainView<std::byte>& memory ) const;

		//
		std::optional<Black::PlainView<std::byte>> ParseFileEntry( Black::PlainView<std::byte>&& memory ) const;

		//
		std::optional<Black::PlainView<std::byte>> ParseExtraDataEntry( Black::PlainView<std::byte>&& memory ) const;

		//
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryEntry( Black::PlainView<std::byte>&& memory ) const;

		//
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryDigitalSignature( Black::PlainView<std::byte>&& memory ) const;

		//
		std::optional<Black::PlainView<std::byte>> ParseZip64CentralDirectoryFooter( Black::PlainView<std::byte>&& memory ) const;

		//
		std::optional<Black::PlainView<std::byte>> ParseZip64CentralDirectoryLocator( Black::PlainView<std::byte>&& memory ) const;

		//
		std::optional<Black::PlainView<std::byte>> ParseCentralDirectoryFooter( Black::PlainView<std::byte>&& memory ) const;

	// Private state.
	private:
		Black::PlainView<std::byte>	m_file_memory;

	// Private on-state.
	private:
		mutable std::vector<Entry>					m_entries;
		mutable	Internal::ZipExtraData				m_extra_data;
		mutable Internal::ZipDigitalSignature		m_digital_signature;
		mutable Internal::ZipCentralDirectoryFooter	m_footer;

		mutable bool	m_is_valid	= false;
		mutable bool	m_is_parsed	= false;
	};
}
}
}
