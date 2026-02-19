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
	constexpr const char* LOG_CHANNEL = "Black/Zip/FileView";


	//
	const Internal::HeaderSignature PeekHeaderSignature( const Black::PlainView<std::byte>& buffer )
	{
		Internal::HeaderSignature result;

		Black::CopyMemory( &result, buffer.GetMemory(), sizeof( result ) );

		return result;
	}
}


	ZipFileView::ZipFileView() noexcept = default;

	ZipFileView::ZipFileView( ZipFileView&& other ) noexcept = default;

	ZipFileView::ZipFileView( Black::PlainView<std::byte> file_memory )
		: m_file_memory{ std::move( file_memory ) }
	{
		TestFileMemory();
	}

	ZipFileView::~ZipFileView() noexcept = default;

	ZipFileView& ZipFileView::operator=( ZipFileView&& other ) noexcept = default;

	ZipFileView& ZipFileView::operator=( Black::PlainView<std::byte> file_memory ) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( file_memory ) );
	}

	void ZipFileView::Reset()
	{
		m_file_memory		= {};
		m_entries			= {};
		m_decryption_entry	= {};
		m_digital_signature	= {};
		m_footer			= {};
		m_is_valid			= false;
		m_is_parsed			= false;
	}

	void ZipFileView::Swap( ZipFileView& other )
	{
		Black::Swap( m_file_memory, other.m_file_memory );
		Black::Swap( m_entries, other.m_entries );
		Black::Swap( m_decryption_entry, other.m_decryption_entry );
		Black::Swap( m_digital_signature, other.m_digital_signature );
		Black::Swap( m_footer, other.m_footer );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_parsed, other.m_is_parsed );
	}

	Black::Hypothetical<const ZipFileView::LocalFileEntry&> ZipFileView::FindEntry( const std::string_view entry_name ) const
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

	const size_t ZipFileView::GetEntriesCount() const
	{
		CRET( !m_is_valid, 0 );
		EnsureFileMemoryParsed();

		return m_entries.size();
	}

	const ZipFileView::LocalFileEntry* ZipFileView::QueryEntry( const size_t entry_index ) const
	{
		CRET( !m_is_valid, nullptr );
		EnsureFileMemoryParsed();
		CRET( m_entries.empty(), nullptr );

		return ( entry_index < m_entries.size() )? &m_entries[ entry_index ] : nullptr;
	}

	const ZipFileView::LocalFileEntry& ZipFileView::GetEntry( const size_t entry_index ) const
	{
		EXPECTS( m_is_valid );

		const LocalFileEntry* const found_entry = QueryEntry( entry_index );
		ENSURES( found_entry != nullptr );

		return *found_entry;
	}

	ZipFileView::ConstIterator ZipFileView::GetBegin() const
	{
		CRET( !m_is_valid, m_entries.end() );
		EnsureFileMemoryParsed();

		return m_entries.begin();
	}

	ZipFileView::ConstIterator ZipFileView::GetEnd() const
	{
		CRET( !m_is_valid, m_entries.end() );
		EnsureFileMemoryParsed();

		return m_entries.end();
	}

	const bool ZipFileView::IsEmpty() const
	{
		CRET( !m_is_valid, false );
		EnsureFileMemoryParsed();
		return m_entries.empty();
	}

	const bool ZipFileView::HasEntries() const
	{
		CRET( !m_is_valid, false );
		EnsureFileMemoryParsed();
		return !m_entries.empty();
	}

	void ZipFileView::EnsureFileMemoryParsed() const
	{
		CRET( !m_is_valid || m_is_parsed );
		ParseFileMemory();
	}

	void ZipFileView::ParseFileMemory() const
	{
		using ParseRoutine = decltype( &ZipFileView::ParseFileEntry );

		m_is_parsed = false;
		CRETW( !m_is_valid, , LOG_CHANNEL, "Unable to parse invalid memory." );
		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Perform file parsing." );

		auto reset_contract = Black::ScopeLeaveHandler{
			[this]()
			{
				m_is_valid			= false;
				m_entries.clear();
				m_decryption_entry	= {};
				m_digital_signature	= {};
				m_footer			= {};
				BLACK_LOG_ERROR( LOG_CHANNEL, "Parse of file failed." );
			}
		};

		Black::PlainView<std::byte> file_memory = m_file_memory;
		while( !file_memory.IsEmpty() )
		{
			CRETE( file_memory.GetLength() < sizeof( Internal::HeaderSignature ), , LOG_CHANNEL, "Unexpected end of file." );

			const Internal::HeaderSignature signature = PeekHeaderSignature( file_memory );
			ParseRoutine routine = {};
			std::string_view routine_comment;

			switch( signature )
			{
			case Internal::LocalFileHeader::SIGNATURE:
				routine = &ZipFileView::ParseFileEntry;
				routine_comment = "local file entry";
				break;
			case Internal::ArchiveExtraDataRecord::SIGNATURE:
				routine = &ZipFileView::ParseExtraDataEntry;
				routine_comment = "archive extra data";
				break;
			case Internal::CentralDirectoryFileHeader::SIGNATURE:
				routine = &ZipFileView::ParseCentralDirectoryEntry;
				routine_comment = "central directory file entry";
				break;
			case Internal::CentralDirectoryDigitalSignatureHeader::SIGNATURE:
				routine = &ZipFileView::ParseCentralDirectoryDigitalSignature;
				routine_comment = "central directory digital signature";
				break;
			case Internal::EndOfCentralDirectoryHeader::SIGNATURE:
				routine = &ZipFileView::ParseCentralDirectoryFooter;
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

	void ZipFileView::TestFileMemory() const
	{
		m_is_valid = false;

		CRET( m_file_memory.IsEmpty() );
		CRET( m_file_memory.size() < sizeof( Internal::LocalFileHeader ) );

		const Internal::LocalFileHeader& header = *reinterpret_cast<Internal::LocalFileHeader*>( m_file_memory.GetMemory() );
		CRET( header.signature != Internal::LocalFileHeader::SIGNATURE );
		CRET( ( sizeof( Internal::LocalFileHeader ) + header.name_length + header.extra_field_length + header.compressed_length ) > m_file_memory.GetLength() );

		m_is_valid = true;
	}

	std::shared_ptr<Internal::FileDataDescriptor> ZipFileView::LocateDataDescriptor( const Black::PlainView<std::byte>& memory ) const
	{
		BLACK_LOG_ERROR( LOG_CHANNEL, "Data descriptor lookup is not supported." );
		return {};
	}

	std::optional<Black::PlainView<std::byte>> ZipFileView::ParseFileEntry( Black::PlainView<std::byte>&& memory ) const
	{
		using Header = Internal::LocalFileHeader;
		constexpr size_t header_size = sizeof( Header );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of local file header." );

		LocalFileEntry file_entry;
		file_entry.header = std::shared_ptr<Header>{ reinterpret_cast<Header*>( buffer.GetMemory() ), []( Header* const header ) {} };
		CRETE( file_entry.header->signature != Header::SIGNATURE, {}, LOG_CHANNEL, "Local file signature mismatch." );

		file_entry.base_offset	= std::distance( m_file_memory.GetBegin(), buffer.GetBegin() );
		buffer					= buffer.TruncatePrefix( header_size );
		CRETE( buffer.GetLength() < size_t( file_entry.header->name_length ), {}, LOG_CHANNEL, "The rest memory is less than length of file path." );
		file_entry.name = {
			reinterpret_cast<const char*>( buffer.GetMemory() ),
			size_t( file_entry.header->name_length )
		};

		file_entry.name_hash = std::hash<std::string_view>{}( file_entry.name );

		buffer = buffer.TruncatePrefix( file_entry.name.length() );
		CRETE( buffer.GetLength() < size_t( file_entry.header->extra_field_length ), {}, LOG_CHANNEL, "The rest memory is less than length of extra field." );
		file_entry.extra_field = {
			buffer.GetMemory(),
			size_t( file_entry.header->extra_field_length )
		};

		buffer						= buffer.TruncatePrefix( file_entry.extra_field.GetLength() );
		file_entry.payload_offset	= std::distance( m_file_memory.GetBegin(), buffer.GetBegin() );
		if( file_entry.header->general_purpose_bits.HasFlag( Internal::GeneralPurposeBitFlag::UseDataDescriptor ) )
		{
			std::shared_ptr<Internal::FileDataDescriptor> data_descriptor{ LocateDataDescriptor( buffer ) };
			CRETE( data_descriptor == nullptr, {}, LOG_CHANNEL, "Failed to locate the data descriptor block for file." );
			CRETE( buffer.GetLength() < size_t( data_descriptor->compressed_length ), {}, LOG_CHANNEL, "The rest memory is less than length file content." );

			file_entry.data_descriptor = std::move( data_descriptor );
			file_entry.payload = {
				buffer.GetMemory(),
				size_t( file_entry.data_descriptor->compressed_length )
			};

			buffer = buffer.TruncatePrefix( file_entry.payload.GetLength() + sizeof( Internal::FileDataDescriptor ) );
		}
		else if( file_entry.header->compressed_length > 0 )
		{
			CRETE( buffer.GetLength() < size_t( file_entry.header->compressed_length ), {}, LOG_CHANNEL, "The rest memory is less than length file content." );
			file_entry.payload = {
				buffer.GetMemory(),
				size_t( file_entry.header->compressed_length )
			};

			buffer = buffer.TruncatePrefix( file_entry.payload.GetLength() );
		}

		file_entry.payload_length	= file_entry.payload.GetLength();
		file_entry.base_length		= std::distance( m_file_memory.GetBegin(), buffer.GetBegin() );

		m_entries.emplace_back( std::move( file_entry ) );
		return { buffer };
	}

	std::optional<Black::PlainView<std::byte>> ZipFileView::ParseExtraDataEntry( Black::PlainView<std::byte>&& memory ) const
	{
		using Header = Internal::ArchiveExtraDataRecord;
		constexpr size_t header_size = sizeof( Header );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of extra data record." );

		DecryptionDataEntry decryption_entry;
		decryption_entry.header = std::shared_ptr<Header>{ reinterpret_cast<Header*>( buffer.GetMemory() ), []( Header* const record ) {} };
		CRETE( decryption_entry.header->signature != Header::SIGNATURE, {}, LOG_CHANNEL, "Extra data record signature mismatch." );

		buffer = buffer.TruncatePrefix( header_size );
		CRETE( buffer.GetLength() < size_t( decryption_entry.header->extra_field_length ), {}, LOG_CHANNEL, "The rest memory is less than length extra data." );
		decryption_entry.payload = {
			buffer.GetMemory(),
			size_t( decryption_entry.header->extra_field_length )
		};

		buffer = buffer.TruncatePrefix( decryption_entry.payload.GetLength() );

		m_decryption_entry = std::move( decryption_entry );
		return { buffer };
	}

	std::optional<Black::PlainView<std::byte>> ZipFileView::ParseCentralDirectoryEntry( Black::PlainView<std::byte>&& memory ) const
	{
		using Header = Internal::CentralDirectoryFileHeader;
		constexpr size_t header_size = sizeof( Header );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of extra data record." );

		std::shared_ptr<Header> header{ reinterpret_cast<Header*>( buffer.GetMemory() ), []( Header* const header ) {} };
		CRETE( header->signature != Header::SIGNATURE, {}, LOG_CHANNEL, "Central directory file header signature mismatch." );

		buffer = buffer.TruncatePrefix( header_size );
		CRETE( buffer.GetLength() < size_t( header->name_length ), {}, LOG_CHANNEL, "The rest memory is less than length of file path." );
		std::string_view file_path{
			reinterpret_cast<const char*>( buffer.GetMemory() ),
			size_t( header->name_length )
		};

		const std::size_t name_hash = std::hash<std::string_view>{}( file_path );

		buffer = buffer.TruncatePrefix( file_path.length() );
		CRETE( buffer.GetLength() < size_t( header->extra_field_length ), {}, LOG_CHANNEL, "The rest memory is less than length of extra data." );
		Black::PlainView<std::byte> extra_data{
			buffer.GetMemory(),
			size_t( header->extra_field_length )
		};

		buffer = buffer.TruncatePrefix( extra_data.GetLength() );
		CRETE( buffer.GetLength() < size_t( header->comment_length ), {}, LOG_CHANNEL, "The rest memory is less than length of file comment." );
		std::string_view file_comment{
			reinterpret_cast<const char*>( buffer.GetMemory() ),
			size_t( header->comment_length )
		};

		buffer = buffer.TruncatePrefix( file_comment.length() );

		// Remember the taken data into corresponded file entry.
		Black::FindItem(
			m_entries,
			[name_hash, &file_path]( const LocalFileEntry& file_entry )
			{
				return ( file_entry.name_hash == name_hash ) && ( file_entry.name == file_path );
			}
		).AndThen(
			[&header, &extra_data, &file_comment]( LocalFileEntry& file_entry )
			{
				file_entry.central_directory_header			= std::move( header );
				file_entry.central_directory_extra_field	= std::move( extra_data );
				file_entry.comment							= std::move( file_comment );
			}
		);

		return { buffer };
	}

	std::optional<Black::PlainView<std::byte>> ZipFileView::ParseCentralDirectoryDigitalSignature( Black::PlainView<std::byte>&& memory ) const
	{
		using Header = Internal::CentralDirectoryDigitalSignatureHeader;
		constexpr size_t header_size = sizeof( Header );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of central directory digital signature." );

		std::shared_ptr<Header> header{ reinterpret_cast<Header*>( buffer.GetMemory() ), []( Header* const block ) {} };
		CRETE( header->signature != Header::SIGNATURE, {}, LOG_CHANNEL, "Central directory digital signature mismatch." );

		buffer = buffer.TruncatePrefix( header_size );
		CRETE( buffer.GetLength() < size_t( header->data_length ), {}, LOG_CHANNEL, "The rest memory is less than length of payload." );
		Black::PlainView<std::byte> payload{
			buffer.GetMemory(),
			size_t( header->data_length )
		};

		buffer = buffer.TruncatePrefix( payload.GetLength() );

		m_digital_signature.header	= std::move( header );
		m_digital_signature.payload	= std::move( payload );

		return { buffer };
	}

	std::optional<Black::PlainView<std::byte>> ZipFileView::ParseCentralDirectoryFooter( Black::PlainView<std::byte>&& memory ) const
	{
		using Header = Internal::EndOfCentralDirectoryHeader;
		constexpr size_t description_size = sizeof( Header );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < description_size, {}, LOG_CHANNEL, "The rest memory is less than size of central directory footer." );

		std::shared_ptr<Header> description{ reinterpret_cast<Header*>( buffer.GetMemory() ), []( Header* const block ) {} };
		CRETE( description->signature != Header::SIGNATURE, {}, LOG_CHANNEL, "Central directory footer signature mismatch." );

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
