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
	const Internal::HeaderSignature PeekHeaderSignature( const Black::PlainView<std::byte> buffer )
	{
		Internal::HeaderSignature result;

		Black::CopyMemory( &result, buffer.GetMemory(), sizeof( result ) );

		return result;
	}

	//
	std::pair<Internal::ZipCentralDirectoryFooter, Black::PlainView<std::byte>> ParseZip64EndOfCentralDirectory( Black::PlainView<std::byte>&& file_memory )
	{
		// (4.3.14) ... SHOULD be the size of the remaining record and SHOULD NOT include the leading 12 bytes.
		const size_t header_rest_length = (
			sizeof( Internal::Zip64EndOfCentralDirectory ) - Black::GetFieldOffset( &Internal::Zip64EndOfCentralDirectory::compressor_version )
		);

		std::pair<Internal::ZipCentralDirectoryFooter, Black::PlainView<std::byte>> result{};
		auto& [ footer, buffer ] = result;

		buffer = std::move( file_memory );

		std::byte* current_memory = buffer.GetMemory();
		footer.zip64_description = std::shared_ptr<Internal::Zip64EndOfCentralDirectory>{
			reinterpret_cast<Internal::Zip64EndOfCentralDirectory*>( current_memory ),
			[]( Internal::Zip64EndOfCentralDirectory* const block ) {}
		};

		current_memory += sizeof( Internal::Zip64EndOfCentralDirectory );
		footer.zip64_extra_field = {
			current_memory,
			std::max( size_t( footer.zip64_description->length ), header_rest_length ) - header_rest_length
		};

		current_memory += footer.zip64_extra_field.GetLength();
		buffer = { current_memory, buffer.GetEnd() };

		return result;
	}

	//
	std::pair<Internal::ZipCentralDirectoryFooter, Black::PlainView<std::byte>> ParseZip64EndOfCentralDirectoryLocator( Black::PlainView<std::byte>&& file_memory )
	{
		std::pair<Internal::ZipCentralDirectoryFooter, Black::PlainView<std::byte>> result{};
		auto& [ footer, buffer ] = result;

		buffer = std::move( file_memory );

		std::byte* current_memory = buffer.GetMemory();
		footer.zip64_locator = std::shared_ptr<Internal::Zip64EndOfCentralDirectoryLocator>{
			reinterpret_cast<Internal::Zip64EndOfCentralDirectoryLocator*>( current_memory ),
			[]( Internal::Zip64EndOfCentralDirectoryLocator* const block ) {}
		};

		current_memory += sizeof( Internal::Zip64EndOfCentralDirectoryLocator );
		buffer = { current_memory, buffer.GetEnd() };

		return result;
	}

	//
	std::pair<Internal::ZipCentralDirectoryFooter, Black::PlainView<std::byte>> ParseEndOfCentralDirectory( Black::PlainView<std::byte>&& file_memory )
	{
		std::pair<Internal::ZipCentralDirectoryFooter, Black::PlainView<std::byte>> result{};
		auto& [ footer, buffer ] = result;

		buffer = std::move( file_memory );

		std::byte* current_memory = buffer.GetMemory();
		footer.description = std::shared_ptr<Internal::EndOfCentralDirectory>{
			reinterpret_cast<Internal::EndOfCentralDirectory*>( current_memory ),
			[]( Internal::EndOfCentralDirectory* const block ) {}
		};

		current_memory += sizeof( Internal::EndOfCentralDirectory );
		footer.comment = {
			reinterpret_cast<char*>( current_memory ),
			size_t( footer.description->comment_length )
		};

		current_memory += footer.comment.length();
		buffer = { current_memory, buffer.GetEnd() };

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
		ZipFileView new_view{ std::move( file_memory ) };
		Black::Swap( *this, new_view );

		return *this;
	}

	void ZipFileView::Reset()
	{
		m_file_memory		= {};
		m_entries			= {};
		m_extra_data		= {};
		m_digital_signature	= {};
		m_footer			= {};
		m_is_valid			= false;
		m_is_parsed			= false;
	}

	void ZipFileView::Swap( ZipFileView& other )
	{
		Black::Swap( m_file_memory, other.m_file_memory );
		Black::Swap( m_entries, other.m_entries );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_parsed, other.m_is_parsed );
	}

	Black::Hypothetical<const ZipFileView::Entry&> ZipFileView::FindEntry( const std::string_view entry_name ) const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( m_entries.empty(), {} );

		const size_t name_hash = std::hash<std::string_view>{}( entry_name );
		return Black::FindItem(
			m_entries,
			[&entry_name, name_hash]( const Entry& entry )
			{
				return ( entry.name_hash == name_hash ) && ( entry.name == entry_name );
			}
		).Transform(
			[]( Entry& entry ) -> const Entry&
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

	const ZipFileView::Entry* ZipFileView::QueryEntry( const size_t entry_index ) const
	{
		CRET( !m_is_valid, nullptr );
		EnsureFileMemoryParsed();
		CRET( m_entries.empty(), nullptr );

		return ( entry_index < m_entries.size() )? &m_entries[ entry_index ] : nullptr;
	}

	const ZipFileView::Entry& ZipFileView::GetEntry( const size_t entry_index ) const
	{
		EXPECTS( m_is_valid );

		const Entry* const found_entry = QueryEntry( entry_index );
		ENSURES( found_entry != nullptr );

		return *found_entry;
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
		m_is_parsed = false;
		CRETW( !m_is_valid, , LOG_CHANNEL, "Unable to parse invalid memory." );
		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Perform file parsing." );

		auto reset_contract = Black::ScopeLeaveHandler{
			[this]()
			{
				m_is_valid			= false;
				m_entries.clear();
				m_extra_data		= {};
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
			switch( signature )
			{
			case Internal::LocalFileHeader::SIGNATURE:
				{
					std::optional<Black::PlainView<std::byte>> rest_memory{ ParseFileEntry( std::move( file_memory ) ) };
					CRETE( !rest_memory.has_value(), , LOG_CHANNEL, "Failed to parse local file entry." );

					file_memory = *std::move( rest_memory );
				}
				break;
			case Internal::FileDataDescriptor::SIGNATURE:
				{
					BLACK_LOG_ERROR( LOG_CHANNEL, "Unexpected data descriptor command." );
				}
				return;
			case Internal::ArchiveExtraDataRecord::SIGNATURE:
				{
					std::optional<Black::PlainView<std::byte>> rest_memory{ ParseExtraDataEntry( std::move( file_memory ) ) };
					CRETE( !rest_memory.has_value(), , LOG_CHANNEL, "Failed to parse archive extra data." );

					file_memory = *std::move( rest_memory );
				}
				break;
			case Internal::CentralDirectoryFileHeader::SIGNATURE:
				{
					std::optional<Black::PlainView<std::byte>> rest_memory{ ParseCentralDirectoryEntry( std::move( file_memory ) ) };
					CRETE( !rest_memory.has_value(), , LOG_CHANNEL, "Failed to parse central directory file entry." );

					file_memory = *std::move( rest_memory );
				}
				break;
			case Internal::CentralDirectoryDigitalSignature::SIGNATURE:
				{
					std::optional<Black::PlainView<std::byte>> rest_memory{ ParseCentralDirectoryDigitalSignature( std::move( file_memory ) ) };
					CRETE( !rest_memory.has_value(), , LOG_CHANNEL, "Failed to parse central directory digital signature." );

					file_memory = *std::move( rest_memory );
				}
				break;
			case Internal::Zip64EndOfCentralDirectory::SIGNATURE:
				{
					Internal::ZipCentralDirectoryFooter footer;
					std::tie( footer, file_memory ) = ParseZip64EndOfCentralDirectory( std::move( file_memory ) );

					m_footer.zip64_description = std::move( footer.zip64_description );
					m_footer.zip64_extra_field = std::move( footer.zip64_extra_field );
				}
				break;
			case Internal::Zip64EndOfCentralDirectoryLocator::SIGNATURE:
				{
					Internal::ZipCentralDirectoryFooter footer;
					std::tie( footer, file_memory ) = ParseZip64EndOfCentralDirectoryLocator( std::move( file_memory ) );

					m_footer.zip64_locator = std::move( footer.zip64_locator );
				}
				break;
			case Internal::EndOfCentralDirectory::SIGNATURE:
				{
					Internal::ZipCentralDirectoryFooter footer;
					std::tie( footer, file_memory ) = ParseEndOfCentralDirectory( std::move( file_memory ) );

					m_footer.description = std::move( footer.description );
					m_footer.comment = std::move( footer.comment );
				}
				break;
			default:
				BLACK_LOG_ERROR( LOG_CHANNEL, "Unexpected signature of ZIP command: 0x{:X}.", Black::GetEnumValue( signature ) );
				return;
			}
		}
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
		constexpr size_t header_size = sizeof( Internal::LocalFileHeader );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of local file header." );

		Internal::ZipFileEntry file_entry;
		file_entry.header = std::shared_ptr<Internal::LocalFileHeader>{
			reinterpret_cast<Internal::LocalFileHeader*>( buffer.GetMemory() ),
			[]( Internal::LocalFileHeader* const header ) {}
		};
		CRETE( file_entry.header->signature != Internal::LocalFileHeader::SIGNATURE, {}, LOG_CHANNEL, "Local file signature mismatch." );

		buffer = buffer.TruncatePrefix( header_size );
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

		buffer = buffer.TruncatePrefix( file_entry.extra_field.GetLength() );
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

		m_entries.emplace_back( std::move( file_entry ) );
		return { buffer };
	}

	std::optional<Black::PlainView<std::byte>> ZipFileView::ParseExtraDataEntry( Black::PlainView<std::byte>&& memory ) const
	{
		constexpr size_t header_size = sizeof( Internal::ArchiveExtraDataRecord );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of extra data record." );

		Internal::ZipExtraData extra_data;
		extra_data.header = std::shared_ptr<Internal::ArchiveExtraDataRecord>{
			reinterpret_cast<Internal::ArchiveExtraDataRecord*>( buffer.GetMemory() ),
			[]( Internal::ArchiveExtraDataRecord* const record ) {}
		};
		CRETE( extra_data.header->signature != Internal::ArchiveExtraDataRecord::SIGNATURE, {}, LOG_CHANNEL, "Extra data record signature mismatch." );

		buffer = buffer.TruncatePrefix( header_size );
		CRETE( buffer.GetLength() < size_t( extra_data.header->extra_field_length ), {}, LOG_CHANNEL, "The rest memory is less than length extra data." );
		extra_data.payload = {
			buffer.GetMemory(),
			size_t( extra_data.header->extra_field_length )
		};

		buffer = buffer.TruncatePrefix( extra_data.payload.GetLength() );

		m_extra_data = std::move( extra_data );
		return { buffer };
	}

	std::optional<Black::PlainView<std::byte>> ZipFileView::ParseCentralDirectoryEntry( Black::PlainView<std::byte>&& memory ) const
	{
		constexpr size_t header_size = sizeof( Internal::CentralDirectoryFileHeader );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of extra data record." );

		std::shared_ptr<Internal::CentralDirectoryFileHeader> header{
			reinterpret_cast<Internal::CentralDirectoryFileHeader*>( buffer.GetMemory() ),
			[]( Internal::CentralDirectoryFileHeader* const header ) {}
		};
		CRETE( header->signature != Internal::CentralDirectoryFileHeader::SIGNATURE, {}, LOG_CHANNEL, "Central directory file header signature mismatch." );

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
			[name_hash, &file_path]( const Internal::ZipFileEntry& file_entry )
			{
				return ( file_entry.name_hash == name_hash ) && ( file_entry.name == file_path );
			}
		).AndThen(
			[&header, &extra_data, &file_comment]( Internal::ZipFileEntry& file_entry )
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
		constexpr size_t header_size = sizeof( Internal::CentralDirectoryDigitalSignature );

		Black::PlainView<std::byte> buffer{ std::move( memory ) };
		CRETE( buffer.GetLength() < header_size, {}, LOG_CHANNEL, "The rest memory is less than size of central directory digital signature." );

		std::shared_ptr<Internal::CentralDirectoryDigitalSignature> header{
			reinterpret_cast<Internal::CentralDirectoryDigitalSignature*>( buffer.GetMemory() ),
			[]( Internal::CentralDirectoryDigitalSignature* const block ) {}
		};
		CRETE( header->signature != Internal::CentralDirectoryDigitalSignature::SIGNATURE, {}, LOG_CHANNEL, "Central directory digital signature mismatch." );

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
}
}
}
