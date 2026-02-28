#include <black/format/zip.h>

#include <black/core/algorithms.h>


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


	//
	const Internal::HeaderSignature PeekHeaderSignature( const Black::PlainView<std::byte>& buffer )
	{
		Internal::HeaderSignature result;

		Black::CopyMemory( &result, buffer.GetMemory(), sizeof( result ) );

		return result;
	}
}


	ZipCentralDirectoryView::ZipCentralDirectoryView() noexcept = default;

	ZipCentralDirectoryView::ZipCentralDirectoryView( ZipCentralDirectoryView&& other ) noexcept = default;

	ZipCentralDirectoryView::ZipCentralDirectoryView( Black::PlainView<std::byte> file_memory )
		: m_file_memory{ std::move( file_memory ) }
	{
		TestFileMemory();
	}

	ZipCentralDirectoryView::ZipCentralDirectoryView( Black::PlainView<std::byte> file_memory, const size_t central_directory_length )
		: m_file_memory{ std::move( file_memory ) }
		, m_central_directory_length{ central_directory_length }
	{
		TestFileMemory();
	}

	ZipCentralDirectoryView::~ZipCentralDirectoryView() noexcept = default;

	ZipCentralDirectoryView& ZipCentralDirectoryView::operator=( ZipCentralDirectoryView&& other ) noexcept = default;

	ZipCentralDirectoryView& ZipCentralDirectoryView::operator=( Black::PlainView<std::byte> file_memory ) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( file_memory ) );
	}

	void ZipCentralDirectoryView::Reset()
	{
		m_file_memory				= {};
		m_central_directory_length	= Black::UNDEFINED_INDEX;
		m_entries.clear();
		m_footer					= {};
		m_is_valid					= false;
		m_is_parsed					= false;
	}

	void ZipCentralDirectoryView::Swap( ZipCentralDirectoryView& other )
	{
		Black::Swap( m_file_memory, other.m_file_memory );
		Black::Swap( m_central_directory_length, other.m_central_directory_length );
		Black::Swap( m_entries, other.m_entries );
		Black::Swap( m_footer, other.m_footer );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_parsed, other.m_is_parsed );
	}

	Black::Hypothetical<const ZipCentralDirectoryView::LocalFileEntry&> ZipCentralDirectoryView::FindEntry( const std::string_view entry_name ) const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( m_entries.empty(), {} );

		const size_t name_hash = std::hash<std::string_view>{}( entry_name );
		return Black::FindItem(
			m_entries,
			[&entry_name, name_hash]( const LocalFileEntry& entry )
			{
				return ( entry.name_hash == name_hash ) && ( entry.name == entry_name );
			}
		).Transform(
			[]( LocalFileEntry& entry ) -> const LocalFileEntry&
			{
				return entry;
			}
		);
	}

	const size_t ZipCentralDirectoryView::GetEntriesCount() const
	{
		CRET( !m_is_valid, 0 );
		EnsureFileMemoryParsed();

		return m_entries.size();
	}

	const ZipCentralDirectoryView::LocalFileEntry* ZipCentralDirectoryView::QueryEntry( const size_t entry_index ) const
	{
		CRET( !m_is_valid, nullptr );
		EnsureFileMemoryParsed();
		CRET( m_entries.empty(), nullptr );

		return ( entry_index < m_entries.size() )? &m_entries[ entry_index ] : nullptr;
	}

	const ZipCentralDirectoryView::LocalFileEntry& ZipCentralDirectoryView::GetEntry( const size_t entry_index ) const
	{
		EXPECTS( m_is_valid );

		const LocalFileEntry* const found_entry = QueryEntry( entry_index );
		ENSURES( found_entry != nullptr );

		return *found_entry;
	}

	ZipCentralDirectoryView::ConstIterator ZipCentralDirectoryView::GetBegin() const
	{
		CRET( !m_is_valid, m_entries.end() );
		EnsureFileMemoryParsed();

		return m_entries.begin();
	}

	ZipCentralDirectoryView::ConstIterator ZipCentralDirectoryView::GetEnd() const
	{
		CRET( !m_is_valid, m_entries.end() );
		EnsureFileMemoryParsed();

		return m_entries.end();
	}

	const bool ZipCentralDirectoryView::IsEmpty() const
	{
		CRET( !m_is_valid, false );
		EnsureFileMemoryParsed();
		return m_entries.empty();
	}

	const bool ZipCentralDirectoryView::HasEntries() const
	{
		CRET( !m_is_valid, false );
		EnsureFileMemoryParsed();
		return !m_entries.empty();
	}

	void ZipCentralDirectoryView::EnsureFileMemoryParsed() const
	{
		CRET( !m_is_valid || m_is_parsed );
		ParseFileMemory();
	}

	void ZipCentralDirectoryView::ParseFileMemory() const
	{
		using ParseRoutine = decltype( &ZipCentralDirectoryView::ParseCentralDirectoryEntry );

		m_is_parsed = false;
		CRETW( !m_is_valid, , LOG_CHANNEL, "Unable to parse invalid memory." );
		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Perform central directory parsing." );

		auto reset_contract = Black::ScopeLeaveHandler{
			[this]()
			{
				m_is_valid			= false;
				m_entries.clear();
				m_footer			= {};
				BLACK_LOG_ERROR( LOG_CHANNEL, "Parse of central directory failed." );
			}
		};

		Black::PlainView<std::byte> file_memory = m_file_memory.GetSubview( 0, m_central_directory_length );
		while( !file_memory.IsEmpty() )
		{
			CRETE( file_memory.GetLength() < sizeof( Internal::HeaderSignature ), , LOG_CHANNEL, "Unexpected end of file." );

			const Internal::HeaderSignature signature = PeekHeaderSignature( file_memory );
			ParseRoutine routine = {};
			std::string_view routine_comment;

			switch( signature )
			{
			case Internal::CentralDirectoryFileHeader::SIGNATURE:
				routine = &ZipCentralDirectoryView::ParseCentralDirectoryEntry;
				routine_comment = "central directory file entry";
				break;
			case Internal::EndOfCentralDirectoryHeader::SIGNATURE:
				routine = &ZipCentralDirectoryView::ParseCentralDirectoryFooter;
				routine_comment = "central directory footer";
				break;
			default:
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unexpected signature of ZIP command: 0x{:X}.", Black::GetEnumValue( signature ) );
				return;
			}

			std::optional<Black::PlainView<std::byte>> rest_memory{ (this->*routine)( std::move( file_memory ) ) };
			CRETE( !rest_memory.has_value(), , LOG_CHANNEL, "Failed to parse {}.", routine_comment );
			file_memory = *std::move( rest_memory );
		}

		reset_contract.Cancel();
		m_is_parsed = true;
	}

	void ZipCentralDirectoryView::TestFileMemory() const
	{
		m_is_valid = false;

		CRET( m_file_memory.IsEmpty() );
		CRET( m_file_memory.size() < sizeof( FileHeader ) );

		const FileHeader& header = *reinterpret_cast<FileHeader*>( m_file_memory.GetMemory() );
		CRET( header.signature != FileHeader::SIGNATURE );
		CRET( ( sizeof( FileHeader ) + header.name_length + header.extra_field_length + header.comment_length ) > m_file_memory.GetLength() );

		m_is_valid = true;
	}

	std::optional<Black::PlainView<std::byte>> ZipCentralDirectoryView::ParseCentralDirectoryEntry( Black::PlainView<std::byte>&& memory ) const
	{
		constexpr size_t header_size = sizeof( FileHeader );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of extra data record." );

		std::shared_ptr<FileHeader> header{ reinterpret_cast<FileHeader*>( buffer.GetMemory() ), []( FileHeader* const header ) {} };
		CRETE( header->signature != FileHeader::SIGNATURE, {}, LOG_CHANNEL, "Central directory file header signature mismatch." );

		buffer = buffer.TruncatePrefix( header_size );
		CRETE( buffer.GetLength() < size_t( header->name_length ), {}, LOG_CHANNEL, "The rest memory is less than length of file path." );
		std::string_view file_name{
			reinterpret_cast<const char*>( buffer.GetMemory() ),
			size_t( header->name_length )
		};

		const std::size_t name_hash = std::hash<std::string_view>{}( file_name );

		buffer = buffer.TruncatePrefix( file_name.length() );
		CRETE( buffer.GetLength() < size_t( header->extra_field_length ), {}, LOG_CHANNEL, "The rest memory is less than length of extra data." );
		Black::PlainView<std::byte> extra_field{
			buffer.GetMemory(),
			size_t( header->extra_field_length )
		};

		buffer = buffer.TruncatePrefix( extra_field.GetLength() );
		CRETE( buffer.GetLength() < size_t( header->comment_length ), {}, LOG_CHANNEL, "The rest memory is less than length of file comment." );
		std::string_view file_comment{
			reinterpret_cast<const char*>( buffer.GetMemory() ),
			size_t( header->comment_length )
		};

		buffer = buffer.TruncatePrefix( file_comment.length() );

		LocalFileEntry& file_entry			= m_entries.emplace_back();

		file_entry.name_hash				= name_hash;
		file_entry.name						= std::move( file_name );
		file_entry.base_offset				= size_t( header->local_header_offset );
		file_entry.base_length				= sizeof( Internal::LocalFileHeader ) + header->name_length + header->extra_field_length + header->compressed_length;
		file_entry.payload_offset			= file_entry.base_offset + sizeof( Internal::LocalFileHeader ) + header->name_length + header->extra_field_length;
		file_entry.payload_length			= size_t( header->compressed_length );
		file_entry.general_purpose_bits		= header->general_purpose_bits;
		file_entry.compression_function		= header->compression_function;

		file_entry.central_directory_header	= std::move( header );
		file_entry.extra_field				= std::move( extra_field );
		file_entry.comment					= std::move( file_comment );

		return { buffer };
	}

	std::optional<Black::PlainView<std::byte>> ZipCentralDirectoryView::ParseCentralDirectoryFooter( Black::PlainView<std::byte>&& memory ) const
	{
		constexpr size_t description_size = sizeof( EndHeader );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < description_size, {}, LOG_CHANNEL, "The rest memory is less than size of central directory footer." );

		std::shared_ptr<EndHeader> description{ reinterpret_cast<EndHeader*>( buffer.GetMemory() ), []( EndHeader* const block ) {} };
		CRETE( description->signature != EndHeader::SIGNATURE, {}, LOG_CHANNEL, "Central directory footer signature mismatch." );

		buffer = buffer.TruncatePrefix( description_size );
		CRETE( buffer.GetLength() < size_t( description->comment_length ), {}, LOG_CHANNEL, "The rest memory is less than size of central directory comment." );
		std::string_view comment {
			reinterpret_cast<char*>( buffer.GetMemory() ),
			size_t( description->comment_length )
		};

		buffer = buffer.TruncatePrefix( comment.length() );

		m_footer.description	= std::move( description );
		m_footer.comment		= std::move( comment );

		return { buffer };
	}
}
}
}
