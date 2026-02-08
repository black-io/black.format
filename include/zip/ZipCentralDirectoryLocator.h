#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
	/**
	*/
	class ZipCentralDirectoryLocator final : private Black::NonCopyable
	{
	// Public inner types.
	public:
		//
		using EndOfCentralDirectoryHeader = Internal::EndOfCentralDirectoryHeader;

		// Central directory footer entry as described in section 4.3.16 (End of central directory record).
		using FooterEntry = Internal::EndOfCentralDirectoryRecord;

	//	Public constants.
	public:
		// Size of EOCD header.
		static constexpr size_t HEADER_SIZE = sizeof( EndOfCentralDirectoryHeader );

		// Maximum length at the end of ZIP file where the EOCD header may be found.
		static constexpr size_t MAX_LOOKUP_DISTANCE = std::numeric_limits<uint16_t>::max() + HEADER_SIZE;

	// Friendship interface.
	public:
		friend inline void swap( ZipCentralDirectoryLocator& left, ZipCentralDirectoryLocator& right ) { left.Swap( right ); };

	// Public life-time management.
	public:
		ZipCentralDirectoryLocator() noexcept;
		ZipCentralDirectoryLocator( ZipCentralDirectoryLocator&& other ) noexcept;

		explicit ZipCentralDirectoryLocator( Black::PlainView<std::byte> file_memory );

		~ZipCentralDirectoryLocator() noexcept;


		ZipCentralDirectoryLocator& operator = ( ZipCentralDirectoryLocator&& other ) noexcept;
		ZipCentralDirectoryLocator& operator = ( Black::PlainView<std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		// Perform the full reset of file view. As result, the view become like just default-constructed.
		void Reset();

		// Swap the state of this view with other one.
		void Swap( ZipCentralDirectoryLocator& other );


		//
		EndOfCentralDirectoryHeader* const QueryDirectoryFooter() const;

		//
		const EndOfCentralDirectoryHeader& GetDirectoryFooter() const;

		//
		const size_t GetDirectoryBaseOffset() const;

		//
		const size_t GetDirectoryLength() const;


		//
		const bool HasCentralDirectory() const;

	// Private interface.
	private:
		//
		void EnsureMemoryProcessed() const;

		//
		void ProcessMemory() const;

		// Perform the quick tests for underlying ZIP file to ensure the file may be processed.
		void TestFileMemory() const;

	// Private state.
	private:
		Black::PlainView<std::byte>	m_file_memory;		// ZIP file memory.

	// Private non-state.
	private:
		mutable FooterEntry	m_footer;					// Central directory footer.

		mutable bool		m_is_valid		= false;	// Whether the `m_file_memory` can be processed.
		mutable bool		m_is_processed	= false;	// Whether the file memory is processed.
	};
}
}
}
