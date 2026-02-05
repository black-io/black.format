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
	std::pair<Internal::ZipExtraData, Black::PlainView<std::byte>> ParseExtraDataRecord( Black::PlainView<std::byte>&& file_memory )
	{
		std::pair<Internal::ZipExtraData, Black::PlainView<std::byte>> result{};
		auto& [ extra_data, buffer ] = result;

		buffer = std::move( file_memory );

		std::byte* current_memory = buffer.GetMemory();
		extra_data.header = std::shared_ptr<Internal::ArchiveExtraDataRecord>{
			reinterpret_cast<Internal::ArchiveExtraDataRecord*>( current_memory ),
			[]( Internal::ArchiveExtraDataRecord* const record ) {}
		};

		current_memory += sizeof( Internal::ArchiveExtraDataRecord );
		extra_data.payload = {
			current_memory,
			size_t( extra_data.header->extra_field_length )
		};

		current_memory += extra_data.payload.GetLength();
		buffer = { current_memory, buffer.GetEnd() };

		return result;
	}

	//
	std::pair<Internal::ZipFileEntry, Black::PlainView<std::byte>> ParseCentralDirectoryEntry( Black::PlainView<std::byte>&& file_memory )
	{
		std::pair<Internal::ZipFileEntry, Black::PlainView<std::byte>> result{};
		auto& [ file_entry, buffer ] = result;

		buffer = std::move( file_memory );

		std::byte* current_memory = buffer.GetMemory();
		file_entry.central_directory_header = std::shared_ptr<Internal::CentralDirectoryFileHeader>{
			reinterpret_cast<Internal::CentralDirectoryFileHeader*>( current_memory ),
			[]( Internal::CentralDirectoryFileHeader* const header ) {}
		};

		current_memory += sizeof( Internal::CentralDirectoryFileHeader );
		file_entry.name = {
			reinterpret_cast<const char*>( current_memory ),
			size_t( file_entry.central_directory_header->name_length )
		};

		file_entry.name_hash = std::hash<std::string_view>{}( file_entry.name );

		current_memory += file_entry.name.length();
		file_entry.central_directory_extra_field = {
			current_memory,
			size_t( file_entry.central_directory_header->extra_field_length )
		};

		current_memory += file_entry.central_directory_extra_field.GetLength();
		file_entry.comment = {
			reinterpret_cast<const char*>( current_memory ),
			size_t( file_entry.central_directory_header->comment_length )
		};

		current_memory += file_entry.comment.length();
		buffer = { current_memory, buffer.GetEnd() };

		return result;
	}

	//
	std::pair<Internal::ZipDigitalSignature, Black::PlainView<std::byte>> ParseCentralDirectoryDigitalSignature( Black::PlainView<std::byte>&& file_memory )
	{
		std::pair<Internal::ZipDigitalSignature, Black::PlainView<std::byte>> result{};
		auto& [ signature, buffer ] = result;

		buffer = std::move( file_memory );

		std::byte* current_memory = buffer.GetMemory();
		signature.header = std::shared_ptr<Internal::CentralDirectoryDigitalSignature>{
			reinterpret_cast<Internal::CentralDirectoryDigitalSignature*>( current_memory ),
			[]( Internal::CentralDirectoryDigitalSignature* const block ) {}
		};

		current_memory += sizeof( Internal::CentralDirectoryDigitalSignature );
		signature.payload = {
			current_memory,
			size_t( signature.header->data_length )
		};

		current_memory += signature.payload.GetLength();
		buffer = { current_memory, buffer.GetEnd() };

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
		m_file_memory	= {};
		m_entries		= {};
		m_is_valid		= false;
		m_is_parsed		= false;
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
				m_is_valid = false;
				m_entries.clear();
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
					ENSURES_DEBUG( rest_memory.has_value() );

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
					std::tie( m_extra_data, file_memory ) = ParseExtraDataRecord( std::move( file_memory ) );
				}
				break;
			case Internal::CentralDirectoryFileHeader::SIGNATURE:
				{
					Internal::ZipFileEntry file_entry;
					std::tie( file_entry, file_memory ) = ParseCentralDirectoryEntry( std::move( file_memory ) );

					Black::FindItem(
						m_entries,
						[&file_entry]( const Internal::ZipFileEntry& candidate )
						{
							return ( candidate.name_hash == file_entry.name_hash ) && ( candidate.name == file_entry.name );
						}
					).AndThen(
						[&file_entry]( Internal::ZipFileEntry& stored_entry )
						{
							stored_entry.central_directory_header = std::move( file_entry.central_directory_header );
							stored_entry.central_directory_extra_field = std::move( file_entry.central_directory_extra_field );
							stored_entry.comment = std::move( file_entry.comment );
						}
					);
				}
				break;
			case Internal::CentralDirectoryDigitalSignature::SIGNATURE:
				{
					std::tie( m_digital_signature, file_memory ) = ParseCentralDirectoryDigitalSignature( std::move( file_memory ) );
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
			// T.B.D.
		}
		else if( !file_entry.header->compressed_length > 0 )
		{
			CRETE( buffer.GetLength() < size_t( file_entry.header->compressed_length ), {}, LOG_CHANNEL, "The rest memory is less than length file content." );
			file_entry.payload = {
				buffer.GetMemory(),
				size_t( file_entry.header->compressed_length )
			};
		}

		buffer = buffer.TruncatePrefix( file_entry.payload.GetLength() );

		m_entries.emplace_back( std::move( file_entry ) );
		return { buffer };
	}
}
}
}
