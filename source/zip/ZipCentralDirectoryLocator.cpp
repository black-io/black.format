#include <black/format/zip.h>


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/Zip/CenatralDirectory";
}


	ZipCentralDirectoryLocator::ZipCentralDirectoryLocator() noexcept = default;

	ZipCentralDirectoryLocator::ZipCentralDirectoryLocator( ZipCentralDirectoryLocator&& other ) noexcept = default;

	ZipCentralDirectoryLocator::ZipCentralDirectoryLocator( Black::PlainView<std::byte> file_memory )
		: m_file_memory{ std::move( file_memory ) }
	{
		TestFileMemory();
	}

	ZipCentralDirectoryLocator::~ZipCentralDirectoryLocator() noexcept = default;

	ZipCentralDirectoryLocator& ZipCentralDirectoryLocator::operator=( ZipCentralDirectoryLocator&& other ) noexcept = default;

	ZipCentralDirectoryLocator& ZipCentralDirectoryLocator::operator=( Black::PlainView<std::byte> file_memory ) noexcept
	{
		ZipCentralDirectoryLocator new_locator{ std::move( file_memory ) };
		Black::Swap( *this, new_locator );

		return *this;
	}

	void ZipCentralDirectoryLocator::Reset()
	{
		m_file_memory	= {};
		m_footer		= {};
		m_is_processed	= false;
		m_is_valid		= false;
	}

	void ZipCentralDirectoryLocator::Swap( ZipCentralDirectoryLocator& other )
	{
		Black::Swap( m_file_memory, other.m_file_memory );
		Black::Swap( m_footer, other.m_footer );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_processed, other.m_is_processed );
	}

	ZipCentralDirectoryLocator::EndOfCentralDirectoryHeader* const ZipCentralDirectoryLocator::QueryDirectoryFooter() const
	{
		CRET( !m_is_valid, nullptr );
		EnsureMemoryProcessed();
		return m_footer.description.get();
	}

	const ZipCentralDirectoryLocator::EndOfCentralDirectoryHeader& ZipCentralDirectoryLocator::GetDirectoryFooter() const
	{
		EnsureMemoryProcessed();
		ENSURES( m_footer.description != nullptr );

		return *m_footer.description;
	}

	const size_t ZipCentralDirectoryLocator::GetDirectoryBaseOffset() const
	{
		CRET( !m_is_valid, Black::UNDEFINED_INDEX );
		EnsureMemoryProcessed();
		CRET( m_footer.description == nullptr, Black::UNDEFINED_INDEX );

		return size_t( m_footer.description->central_directory_position );
	}

	const size_t ZipCentralDirectoryLocator::GetDirectoryLength() const
	{
		CRET( !m_is_valid, Black::UNDEFINED_INDEX );
		EnsureMemoryProcessed();
		CRET( m_footer.description == nullptr, Black::UNDEFINED_INDEX );

		return size_t( m_footer.description->central_directory_length );
	}

	const bool ZipCentralDirectoryLocator::HasCentralDirectory() const
	{
		CRET( !m_is_valid, false );
		EnsureMemoryProcessed();

		return m_footer.description != nullptr;
	}

	void ZipCentralDirectoryLocator::EnsureMemoryProcessed() const
	{
		CRET( m_is_processed );
		EXPECTS( m_is_valid );
		ProcessMemory();
	}

	void ZipCentralDirectoryLocator::ProcessMemory() const
	{
		constexpr Internal::HeaderSignature valid_signature = Internal::HeaderSignature::EndOfCentralDirectory;
		constexpr std::byte vaild_first_byte = std::byte{ Black::GetEnumValue( valid_signature ) & 0xFF };

		m_footer		= {};
		m_is_processed	= false;
		CRETW( !m_is_valid, , LOG_CHANNEL, "Unable to process invalid memory." );
		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Look for the central directory in file." );

		Internal::HeaderSignature candidate_signature;
		for( std::byte* memory_head = m_file_memory.GetEnd() - HEADER_SIZE; memory_head >= m_file_memory.GetBegin(); --memory_head )
		{
			CCON( *memory_head != vaild_first_byte );

			Black::CopyMemory( &candidate_signature, memory_head, sizeof( candidate_signature ) );
			CCON( candidate_signature != valid_signature );

			std::shared_ptr<EndOfCentralDirectoryHeader> header{
				reinterpret_cast<EndOfCentralDirectoryHeader*>( memory_head ),
				[]( EndOfCentralDirectoryHeader* header ) {}
			};

			std::byte* comment_head = memory_head + HEADER_SIZE;
			CCON( header->comment_length != std::distance( comment_head, m_file_memory.GetEnd() ) );

			m_footer.description	= std::move( header );
			m_footer.comment		= {
				reinterpret_cast<const char*>( comment_head ),
				size_t( m_footer.description->comment_length )
			};

			break;
		}

		CRETW( m_footer.description == nullptr, , LOG_CHANNEL, "Unable to find central directory in file." );

		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Central directory found in file." );
		m_is_processed = true;
	}

	void ZipCentralDirectoryLocator::TestFileMemory() const
	{
		m_is_valid = false;

		CRET( m_file_memory.IsEmpty() );
		CRET( m_file_memory.size() < HEADER_SIZE );

		m_is_valid = true;
	}
}
}
}
