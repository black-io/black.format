#include <black/format/tga.h>

#include <black/core/algorithms.h>


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/TGA/FileView";
}


namespace
{
	// Whether the given content type is valid.
	const bool IsContentTypeValid( const Internal::ContentType content_type )
	{
		static constexpr Internal::ContentType allowed_types[] {
			Internal::ContentType::Empty,
			Internal::ContentType::Paletted,
			Internal::ContentType::TrueColor,
			Internal::ContentType::Grayscale,
			Internal::ContentType::RlePaletted,
			Internal::ContentType::RleTrueColor,
			Internal::ContentType::RleGrayscale,
		};

		return Black::HasItem( allowed_types, content_type );
	}

	// Whether the given content type assumes compression.
	const bool IsContentCompressed( const Internal::ContentType content_type )
	{
		constexpr uint8_t rle_mask = 0x08;

		const uint8_t value = Black::GetEnumValue( content_type );
		return ( value & rle_mask ) != 0;
	}

	// Whether the given bit-rate is valid.
	const bool IsBitrateValid( const Internal::Bitrate bitrate )
	{
		static constexpr Internal::Bitrate allowed_bitrates[] {
			Internal::Bitrate::Monochrome,
			Internal::Bitrate::ARGB16,
			Internal::Bitrate::RGB24,
			Internal::Bitrate::ARGB32,
		};

		return Black::HasItem( allowed_bitrates, bitrate );
	}

	// Translate the given bit-rate to size (in bytes) of pixel.
	const size_t GetPixelSize( const Internal::Bitrate bitrate )
	{
		switch( bitrate )
		{
		case Internal::Bitrate::Monochrome:
			return 1;
		case Internal::Bitrate::ARGB16:
			return 2;
		case Internal::Bitrate::RGB24:
			return 3;
		case Internal::Bitrate::ARGB32:
			return 4;
		default:
			break;
		}

		return 0;
	}

	// Get the TGA image id offset like it described in TGA file format specification.
	const size_t GetIdOffset( const Internal::Header& header )
	{
		// TGA image id should be located right after the header.
		return sizeof( header );
	}

	// Get the TGA palette offset like it described in TGA file format specification.
	const size_t GetPaletteOffset( const Internal::Header& header )
	{
		// Palette should be located just after the image id.
		return GetIdOffset( header ) + header.id_length;
	}

	// Get the size (in bytes) of palette like it described in TGA file format specification.
	const size_t GetPaletteSize( const Internal::Header& header )
	{
		// It depends on the state of palette flag.
		return ( header.has_palette )? ( size_t( header.palette.length ) * GetPixelSize( header.palette.bitrate ) ) : 0ULL;
	}

	// Get the TGA image offset like it described in TGA file format specification.
	const size_t GetImageOffset( const Internal::Header& header )
	{
		// It is always valid, even if palette of id does not located in file.
		return GetPaletteOffset( header ) + GetPaletteSize( header );
	}

	// Get the TGA footer offset like it described in TGA file format specification.
	const size_t GetFooterOffset( const Black::PlainView<const std::byte>& file_memeory )
	{
		constexpr size_t footer_size = sizeof( Internal::Footer );

		// TGA footer should be located right at last bytes of file.
		return ( file_memeory.GetLength() > footer_size )? ( file_memeory.GetLength() - footer_size ) : Black::UNDEFINED_INDEX;
	}

	// Get the size (in bytes) of image.
	const size_t GetImageSize( const Black::PlainView<const std::byte>& file_memeory, const Internal::Header& header, const Internal::Footer* const footer )
	{
		const size_t pixel_size = GetPixelSize( header.image.bitrate );

		if( IsContentCompressed( header.content_type ) )
		{
			// The rest of file is image if no footer located in file.
			const size_t image_offset = GetImageOffset( header );
			CRET( footer == nullptr, file_memeory.GetLength() - image_offset );

			// Developer directory always located before the others in TGA 2.0.
			const bool has_developer_directory = footer->developer_directory_offset != 0;
			CRET( has_developer_directory, footer->developer_directory_offset - image_offset );

			// Extension area marks the end of image in case the offset is specified in footer.
			const bool has_extension_area = footer->extension_area_offset != 0;
			CRET( has_extension_area, footer->extension_area_offset - image_offset );

			// Truncate the footer from rest memory to form the size of compressed image.
			return file_memeory.GetLength() - ( image_offset + sizeof( Internal::Footer ) );
		}

		return size_t( header.image.width ) * header.image.height * pixel_size;
	}

	// Map the first bytes of file memory into TGA file header. No checks performed through, except the size.
	const Internal::Header* const MapHeader( const Black::PlainView<const std::byte>& file_memeory )
	{
		CRET( file_memeory.GetLength() < sizeof( Internal::Header ), nullptr );
		return reinterpret_cast<const Internal::Header*>( file_memeory.GetMemory() );
	}

	// Map the last bytes of file memory into TGA file footer. No checks performed through, except the size.
	const Internal::Footer* const MapFooter( const Black::PlainView<const std::byte>& file_memeory )
	{
		CRET( file_memeory.GetLength() < sizeof( Internal::Footer ), nullptr );
		return reinterpret_cast<const Internal::Footer*>( &file_memeory[ GetFooterOffset( file_memeory ) ] );
	}
}



	const bool TgaFileView::IsHeaderValid( const Black::PlainView<const std::byte>& file_memory )
	{
		CRET( file_memory.GetLength() < sizeof( Internal::Header ), false );
		CRET( ( Black::GetEnumValue( file_memory[1] ) & 0xFEU ) != 0, false );

		const Internal::Header& header = *MapHeader( file_memory );
		CRET( !IsContentTypeValid( header.content_type ), false );

		if( Black::HasItem( { Internal::ContentType::Paletted, Internal::ContentType::RlePaletted }, header.content_type ) )
		{
			CRET( header.palette.length == 0, false );
			CRET( !IsBitrateValid( header.palette.bitrate ), false );
			CRET( header.image.flags.alpha_length != 0, false );

			const size_t max_index = ~( ~size_t{} << Black::GetEnumValue( header.image.bitrate ) ) + 1;
			CRET( header.palette.length > max_index, false );
		}

		CRET( header.image.width == 0, false );
		CRET( header.image.height == 0, false );
		CRET( !IsBitrateValid( header.image.bitrate ), false );

		return true;
	}

	const bool TgaFileView::IsFooterValid( const Black::PlainView<const std::byte>& file_memory )
	{
		CRET( file_memory.GetLength() < ( sizeof( Internal::Header ) + sizeof( Internal::Footer ) ), false );

		const size_t footer_offset		= GetFooterOffset( file_memory );
		const Internal::Footer& footer	= *MapFooter( file_memory );

		constexpr const char valid_signature[] = "TRUEVISION-XFILE.";
		CRET( !Black::IsMemoryEqual( footer.signature, valid_signature, std::size( valid_signature ) ), false );
		CRET( footer.developer_directory_offset >= footer_offset, false );
		CRET( footer.extension_area_offset >= footer_offset, false );

		if( ( footer.developer_directory_offset != 0 ) && ( footer.extension_area_offset != 0 ) )
		{
			CRET( footer.developer_directory_offset > footer.extension_area_offset, false );
		}

		return true;
	}

	const bool TgaFileView::IsFileValid( const Black::PlainView<const std::byte>& file_memory )
	{
		CRET( !IsHeaderValid( file_memory ), false );

		const Internal::Header& header = *MapHeader( file_memory );
		const size_t image_offset = GetImageOffset( header );
		CRET( image_offset > file_memory.GetLength(), false );

		const Internal::Footer* const footer = ( IsFooterValid( file_memory ) )? MapFooter( file_memory ) : nullptr;

		const size_t image_size = GetImageSize( file_memory, header, footer );
		CRET( image_size >= file_memory.GetLength(), false );
		CRET( ( image_offset + image_size ) > file_memory.GetLength(), false );

		return true;
	}

	TgaFileView::TgaFileView() noexcept = default;

	TgaFileView::TgaFileView( TgaFileView&& other ) noexcept = default;

	TgaFileView::TgaFileView( Black::PlainView<const std::byte> file_memory )
		: m_file_memory{ std::move( file_memory ) }
	{
		TestFileMemory();
	}

	TgaFileView::TgaFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace )
		: TgaFileView{ std::move( file_memory ) }
	{
		EnsureFileMemoryParsed();
	}

	TgaFileView::~TgaFileView() noexcept = default;

	TgaFileView& TgaFileView::operator=( TgaFileView&& other ) noexcept = default;

	TgaFileView& TgaFileView::operator=( Black::PlainView<const std::byte> file_memory ) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( file_memory ) );
	}

	void TgaFileView::Reset()
	{
		m_file_memory = {};
		InvalidateCache();
	}

	void TgaFileView::Swap( TgaFileView& other )
	{
		Black::Swap( m_file_memory, other.m_file_memory );
		Black::Swap( m_header, other.m_header );
		Black::Swap( m_id, other.m_id );
		Black::Swap( m_palette, other.m_palette );
		Black::Swap( m_image, other.m_image );
		Black::Swap( m_developer_directory, other.m_developer_directory );
		Black::Swap( m_extension_area, other.m_extension_area );
		Black::Swap( m_footer, other.m_footer );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_parsed, other.m_is_parsed );
	}

	const TgaStructure::Header* const TgaFileView::QueryHeader() const
	{
		CRET( !m_is_valid, nullptr );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, nullptr );

		return m_header;
	}

	const TgaStructure::Footer* const TgaFileView::QueryFooter() const
	{
		CRET( !m_is_valid, nullptr );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, nullptr );

		return m_footer;
	}

	const TgaStructure::Header& TgaFileView::GetHeader() const
	{
		EXPECTS( m_is_valid );

		const TgaStructure::Header* const header = QueryHeader();
		ENSURES( header != nullptr );

		return *header;
	}

	const Black::PlainView<const std::byte> TgaFileView::GetIdBuffer() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return m_id;
	}

	const Black::PlainView<const std::byte> TgaFileView::GetPaletteBuffer() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return m_palette;
	}

	const Black::PlainView<const std::byte> TgaFileView::GetImageBuffer() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return m_image;
	}

	const Black::PlainView<const std::byte> TgaFileView::GetDeveloperDirectoryBuffer() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return m_developer_directory;
	}

	const Black::PlainView<const std::byte> TgaFileView::GetExtensionAreaBuffer() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return m_extension_area;
	}

	const TgaStructure::Footer& TgaFileView::GetFooter() const
	{
		EXPECTS( m_is_valid );

		const TgaStructure::Footer* const footer = QueryFooter();
		ENSURES( footer != nullptr );

		return *footer;
	}

	const TgaFileView::Version TgaFileView::GetVersion() const
	{
		CRET( !m_is_valid, Version::Undefined );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, Version::Undefined );

		return ( QueryFooter() == nullptr )? Version::Version1 : Version::Version2;
	}

	const bool TgaFileView::HasValidHeader() const
	{
		return QueryHeader() != nullptr;
	}

	const bool TgaFileView::HasValidFooter() const
	{
		return QueryFooter() != nullptr;
	}

	const bool TgaFileView::IsValidFile() const
	{
		return HasValidHeader() && !GetImageBuffer().IsEmpty();
	}

	const bool TgaFileView::IsEmpty() const
	{
		return m_file_memory.IsEmpty() || ( QueryHeader() == nullptr ) || GetImageBuffer().IsEmpty();
	}

	void TgaFileView::InvalidateCache() const
	{
		m_header				= nullptr;
		m_id					= {};
		m_palette				= {};
		m_image					= {};
		m_developer_directory	= {};
		m_extension_area		= {};
		m_footer				= nullptr;
		m_is_valid				= false;
		m_is_parsed				= false;
	}

	void TgaFileView::EnsureFileMemoryParsed() const
	{
		CRET( !m_is_valid || m_is_parsed );
		ParseFileMemory();
	}

	void TgaFileView::ParseFileMemory() const
	{
		m_is_parsed = false;
		CRETW( !m_is_valid, , LOG_CHANNEL, "Unable to parse invalid memory." );
		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Perform file parsing." );

		Black::ScopeLeaveHandler reset_contract{ Black::BindMethod<&TgaFileView::InvalidateCache>( *this ) };

		CRETW( !IsHeaderValid( m_file_memory ), , LOG_CHANNEL, "Unable to determine TGA header." );
		m_header = MapHeader( m_file_memory );
		m_footer = ( IsFooterValid( m_file_memory ) )? MapFooter( m_file_memory ) : nullptr;

		{
			const size_t id_offset = GetIdOffset( *m_header );
			CRETW( id_offset >= m_file_memory.GetLength(), , LOG_CHANNEL, "Offset of image id is larger than size of file." );
			CRETW( ( id_offset + m_header->id_length ) > m_file_memory.GetLength(), , LOG_CHANNEL, "Length of image id is larger than size of file." );

			BLACK_LOG_DEBUG( LOG_CHANNEL, "Id buffer mapped to [{}..{}] (size: {}).", id_offset, id_offset + m_header->id_length, m_header->id_length );
			m_id = m_file_memory.GetSubview( id_offset, m_header->id_length );
		}

		{
			const size_t palette_offset = GetPaletteOffset( *m_header );
			CRETW( palette_offset >= m_file_memory.GetLength(), , LOG_CHANNEL, "Offset of palette is larger than size of file." );

			const size_t palette_size = GetPaletteSize( *m_header );
			CRETW( ( palette_offset + palette_size ) > m_file_memory.GetLength(), , LOG_CHANNEL, "Size of palette is larger than size of file." );

			BLACK_LOG_DEBUG( LOG_CHANNEL, "Palette buffer mapped to [{}..{}] (size: {}).", palette_offset, palette_offset + palette_size, palette_size );
			m_palette = m_file_memory.GetSubview( palette_offset, palette_size );
		}

		{
			const size_t image_offset = GetImageOffset( *m_header );
			CRETW( image_offset >= m_file_memory.GetLength(), , LOG_CHANNEL, "Offset of image is larger than size of file." );

			const size_t image_size = GetImageSize( m_file_memory, *m_header, m_footer );
			CRETW( ( image_offset + image_size ) > m_file_memory.GetLength(), , LOG_CHANNEL, "Size of image is larger than size of file." );

			BLACK_LOG_DEBUG( LOG_CHANNEL, "Image buffer mapped to [{}..{}] (size: {}).", image_offset, image_offset + image_size, image_size );
			m_image = m_file_memory.GetSubview( image_offset, image_size );
		}

		if( ( m_footer != nullptr ) && ( m_footer->developer_directory_offset != 0 ) )
		{
			// No checks needed here, since the `IsFooterValid` was used.
			const size_t buffer_offset	= m_footer->developer_directory_offset;
			const size_t buffer_end		= ( m_footer->extension_area_offset == 0 )? GetFooterOffset( m_file_memory ) : m_footer->extension_area_offset;
			const size_t buffer_size	= buffer_end - buffer_offset;

			BLACK_LOG_DEBUG( LOG_CHANNEL, "Developer directory mapped to [{}..{}] (size: {}).", buffer_offset, buffer_offset + buffer_size, buffer_size );
			m_developer_directory = m_file_memory.GetSubview( buffer_offset, buffer_size );
		}

		if( ( m_footer != nullptr ) && ( m_footer->extension_area_offset != 0 ) )
		{
			// No checks needed here, since the `IsFooterValid` was used.
			const size_t buffer_offset	= m_footer->extension_area_offset;
			const size_t buffer_end		= GetFooterOffset( m_file_memory );
			const size_t buffer_size	= buffer_end - buffer_offset;

			BLACK_LOG_DEBUG( LOG_CHANNEL, "Extension area mapped to [{}..{}] (size: {}).", buffer_offset, buffer_offset + buffer_size, buffer_size );
			m_extension_area = m_file_memory.GetSubview( buffer_offset, buffer_size );
		}

		BLACK_LOG_VERBOSE( LOG_CHANNEL, "File successfully parsed." );
		reset_contract.Cancel();
		m_is_parsed = true;
	}

	void TgaFileView::TestFileMemory() const
	{
		m_is_valid = false;

		CRET( m_file_memory.IsEmpty() );
		CRET( !IsHeaderValid( m_file_memory ) );

		m_is_valid = true;
	}
}
}
}
