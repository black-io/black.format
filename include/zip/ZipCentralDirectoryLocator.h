#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
	/**
		@brief	Central directory locator implementation for ZIP format.

		Locator type is usable for lookup the central directory only. It can only locate and read the EOCD header.
		Locator type works only over unowned memory in a tail of ZIP file.

		`MAX_LOOKUP_DISTANCE` inner constant may be used to feed the locator with sufficient block of file memory.
		But if the size of file less than `MAX_LOOKUP_DISTANCE`, it rather better to feed the locator with memory of entire file.

		After the locator is constructed, it does nothing but only the basic checks of given memory. Result of `IsValid()` may be used to know the result of checks.
		Locator is designed as lazy view of file memory. The lookup process starts once the locator is asked for some of it's properties.
	*/
	class ZipCentralDirectoryLocator final : private Black::NonCopyable
	{
	// Friendship interface.
	public:
		friend inline void swap( ZipCentralDirectoryLocator& left, ZipCentralDirectoryLocator& right ) { left.Swap( right ); };

	// Public inner types.
	public:
		// Type of EOCD Header in most tail of ZIP file.
		using EndOfCentralDirectoryHeader = Internal::EndOfCentralDirectoryHeader;

		// Central directory footer entry as described in section 4.3.16 (End of central directory record).
		using FooterEntry = Internal::EndOfCentralDirectoryRecord;

	//	Public constants.
	public:
		// Size of EOCD header.
		static constexpr size_t HEADER_SIZE = sizeof( EndOfCentralDirectoryHeader );

		// Maximum length at the end of ZIP file where the EOCD header may be found.
		static constexpr size_t MAX_LOOKUP_DISTANCE = std::numeric_limits<uint16_t>::max() + HEADER_SIZE;

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
		// Perform the full reset of locator. As result, locator becomes like just default-constructed.
		void Reset();

		// Swap the state of this locator with other one.
		void Swap( ZipCentralDirectoryLocator& other );


		// Query the EOCD header of file. May return `nullptr` in case the file is not ZIP or if central directory is missing.
		EndOfCentralDirectoryHeader* const QueryDirectoryFooter() const;

		// Get the EOCD header of file. Causes termination on call for invalid view or if central directory is missing.
		const EndOfCentralDirectoryHeader& GetDirectoryFooter() const;

		/**
			@brief	Get the base offset to central directory of ZIP file.

			Returned value is in bytes.
			In case the valid value returned, central directory can be found in range [base offset, length] of file.

			@return							The value returned is offset from file begin, where the central directory begins.
			@retval	Black::UNDEFINED_INDEX	Returned in case the given memory is invalid or the central directory is missing.
		*/
		const size_t GetDirectoryBaseOffset() const;

		/**
			@brief	Get the length of central directory.

			Returned value is in bytes.
			In case the valid value returned, central directory can be found in range [base offset, length] of file.

			@return							The value returned is length of central directory in file.
			@retval	Black::UNDEFINED_INDEX	Returned in case the given memory is invalid or the central directory is missing.
		*/
		const size_t GetDirectoryLength() const;

		/**
			@brief	Get the length of central directory footer.

			Returned value is in bytes.
			In case the valid value returned, range [base offset, length + footer length] describes the part of central directory in ZIP file.

			@return							The value returned is length of central directory footer in file.
			@retval	Black::UNDEFINED_INDEX	Returned in case the given memory is invalid or the central directory is missing.
		*/
		const size_t GetDirectoryFooterLength() const;


		// Whether the central directory found in given ZIP file.
		const bool HasCentralDirectory() const;

	// Private interface.
	private:
		// Make the guarantee the underlying ZIP file is processed once.
		void EnsureMemoryProcessed() const;

		// Perform the central directory lookup in ZIP file.
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
