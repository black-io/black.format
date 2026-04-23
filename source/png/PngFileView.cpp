#include <black/format/png.h>


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/PNG/FileView";
}


	const bool PngFileView::IsHeaderValid( const Black::PlainView<const std::byte>& file_memory )
	{
		Black::PlainView<const std::byte> buffer{ file_memory };

		constexpr size_t file_preambula_size = std::size( Internal::FILE_PREAMBLE );
		CRET( buffer.GetLength() < file_preambula_size, false );
		CRET( !Black::IsMemoryEqual( buffer.GetMemory(), Internal::FILE_PREAMBLE, file_preambula_size ), false );
		buffer = buffer.TruncatePrefix( file_preambula_size );

		constexpr size_t chunk_header_size = sizeof( Internal::ChunkHeader );
		CRET( buffer.GetLength() < chunk_header_size, false );
		Internal::ChunkEntry chunk;
		chunk.header = reinterpret_cast<const Internal::ChunkHeader*>( buffer.GetMemory() );
		buffer = buffer.TruncatePrefix( chunk_header_size );

		CRET( chunk.header->content_size != sizeof( Internal::ImageHeader ), false );
		CRET( chunk.header->type_code != Internal::ChunkTypeCode::ImageHeader, false );
		CRET( buffer.GetLength() < chunk.header->content_size, false );

		const Internal::ImageHeader* const header = reinterpret_cast<const Internal::ImageHeader*>( buffer.GetMemory() );
		buffer = buffer.TruncatePrefix( chunk.header->content_size );
		CRET( !IsHeaderValid( *header ), false );

		constexpr size_t chunk_footer_size = sizeof( Internal::ChunkFooter );
		CRET( buffer.GetLength() < chunk_footer_size, false );
		chunk.footer = reinterpret_cast<const Internal::ChunkFooter*>( buffer.GetMemory() );

		return true;
	}

	const bool PngFileView::IsHeaderValid( const Black::PngStructure::Image::Header& header )
	{
		CRET( header.width == 0, false );
		CRET( header.width > ( 1 << 31 ), false );
		CRET( header.height == 0, false );
		CRET( header.height > ( 1 << 31 ), false );

		CRET( !Internal::IsBitDepthValid( header.bit_depth ), false );
		CRET( !Internal::IsColorTypeValid( header.color_type ), false );
		CRET( !Internal::HasValidColorCombination( header.bit_depth, header.color_type ), false );
		CRET( !Internal::IsCompressionMethodValid( header.compression_method ), false );

		return true;
	}

	const bool PngFileView::IsFooterValid( const Black::PlainView<const std::byte>& file_memory )
	{
		constexpr size_t chunk_size = sizeof( Internal::ChunkHeader ) + sizeof( Internal::ChunkFooter );

		CRET( file_memory.GetLength() < chunk_size, false );
		const Black::PlainView<const std::byte> buffer{ file_memory.GetSubview( file_memory.GetLength() - chunk_size, chunk_size ) };

		Internal::ChunkEntry chunk;
		chunk.header = reinterpret_cast<const Internal::ChunkHeader*>( buffer.GetMemory() );
		chunk.footer = reinterpret_cast<const Internal::ChunkFooter*>( &buffer[ sizeof( Internal::ChunkHeader ) ] );

		CRET( chunk.header->content_size != 0, false );
		CRET( chunk.header->type_code != Internal::ChunkTypeCode::ImageEnd, false );

		return true;
	}

	const bool PngFileView::IsFileValid( const Black::PlainView<const std::byte>& file_memory )
	{
		CRET( !IsHeaderValid( file_memory ), false );
		CRET( !IsFooterValid( file_memory ), false );

		return true;
	}

	PngFileView::PngFileView( PngFileView&& other ) noexcept
		: m_file_memory{ std::move( other.m_file_memory ) }
		, m_cunks{ std::move( other.m_cunks ) }
		, m_header{ std::exchange( other.m_header, nullptr ) }
		, m_palette{ std::move( other.m_palette ) }
		, m_image{ std::move( other.m_image ) }
		, m_is_valid{ std::exchange( other.m_is_valid, false ) }
		, m_is_parsed{ std::exchange( other.m_is_parsed, false ) }
	{
	}

	PngFileView::PngFileView( Black::PlainView<const std::byte> file_memory ) noexcept
		: m_file_memory{ std::move( file_memory ) }
	{
		TestFileMemory();
	}

	PngFileView::PngFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace ) noexcept
		: PngFileView{ std::move( file_memory ) }
	{
		EnsureFileMemoryParsed();
	}

	PngFileView& PngFileView::operator=( Black::PlainView<const std::byte> file_memory ) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( file_memory ) );
	}

	void PngFileView::Reset()
	{
		m_file_memory = {};
		InvalidateCache();
	}

	void PngFileView::Swap( PngFileView& other )
	{
		Black::Swap( m_file_memory, other.m_file_memory );
		Black::Swap( m_cunks, other.m_cunks );
		Black::Swap( m_header, other.m_header );
		Black::Swap( m_palette, other.m_palette );
		Black::Swap( m_image, other.m_image );
		Black::Swap( m_is_valid, other.m_is_valid );
		Black::Swap( m_is_parsed, other.m_is_parsed );
	}

	const PngStructure::Image::Header* const PngFileView::QueryHeader() const
	{
		CRET( !m_is_valid, nullptr );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, nullptr );

		return m_header;
	}

	const PngStructure::Image::Header& PngFileView::GetHeader() const
	{
		EXPECTS( m_is_valid );

		const PngStructure::Image::Header* const header = QueryHeader();
		ENSURES( header != nullptr );

		return *header;
	}

	const Black::PlainView<const std::byte> PngFileView::GetPaletteBuffer() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return m_palette;
	}

	const Black::PlainView<const std::byte> PngFileView::GetImageBuffer() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return m_image;
	}

	Black::PlainView<const PngFileView::ChunkEntry> PngFileView::GetChunks() const
	{
		CRET( !m_is_valid, {} );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, {} );

		return { m_cunks.data(), m_cunks.size() };
	}

	const bool PngFileView::HasValidHeader() const
	{
		const Black::PlainView<const PngFileView::ChunkEntry> chunks{ GetChunks() };
		return !chunks.empty() && chunks.front().header->type_code == Internal::ChunkTypeCode::ImageHeader;
	}

	const bool PngFileView::HasValidFooter() const
	{
		const Black::PlainView<const PngFileView::ChunkEntry> chunks{ GetChunks() };
		return !chunks.empty() && chunks.back().header->type_code == Internal::ChunkTypeCode::ImageEnd;
	}

	const bool PngFileView::IsValidFile() const
	{
		return HasValidHeader() && !GetImageBuffer().IsEmpty();
	}

	const bool PngFileView::IsEmpty() const
	{
		return m_file_memory.IsEmpty() || ( QueryHeader() == nullptr ) || GetImageBuffer().IsEmpty();
	}

	void PngFileView::InvalidateCache() const
	{
		m_cunks.clear();

		m_header	= nullptr;
		m_palette	= {};
		m_image		= {};
		m_is_valid	= false;
		m_is_parsed	= false;
	}

	void PngFileView::EnsureFileMemoryParsed() const
	{
		CRET( !m_is_valid || m_is_parsed );
		ParseFileMemory();
	}

	void PngFileView::ParseFileMemory() const
	{
		m_is_parsed = false;
		CRETW( !m_is_valid, , LOG_CHANNEL, "Unable to parse invalid memory." );
		BLACK_LOG_VERBOSE( LOG_CHANNEL, "Perform file parsing." );

		Black::ScopeLeaveHandler reset_contract{ Black::BindMethod<&PngFileView::InvalidateCache>( *this ) };

		CRETW( !IsHeaderValid( m_file_memory ), , LOG_CHANNEL, "Unable to determine TGA header." );

		Black::PlainView<const std::byte> chunks_buffer{ m_file_memory.TruncatePrefix( std::size( Internal::FILE_PREAMBLE ) ) };
		while( !chunks_buffer.IsEmpty() )
		{
			Internal::ChunkEntry& chunk = m_cunks.emplace_back();

			CRETW( chunks_buffer.GetLength() < sizeof( Internal::ChunkHeader ), , LOG_CHANNEL, "The rest length of file is less than header of chunk." );
			chunk.header = reinterpret_cast<const Internal::ChunkHeader*>( chunks_buffer.GetMemory() );
			chunks_buffer = chunks_buffer.TruncatePrefix( sizeof( Internal::ChunkHeader ) );

			CRETW( chunks_buffer.GetLength() < chunk.header->content_size, , LOG_CHANNEL, "Size of chunk content exceeds the size of file." );
			chunk.content = chunks_buffer.GetSubview( 0, chunk.header->content_size );
			chunks_buffer = chunks_buffer.TruncatePrefix( chunk.header->content_size );

			CRETW( chunks_buffer.GetLength() < sizeof( Internal::ChunkFooter ), , LOG_CHANNEL, "The rest length of file is less than footer of chunk." );
			chunk.footer = reinterpret_cast<const Internal::ChunkFooter*>( chunks_buffer.GetMemory() );
			chunks_buffer = chunks_buffer.TruncatePrefix( sizeof( Internal::ChunkFooter ) );

			switch( chunk.header->type_code )
			{
			case Internal::ChunkTypeCode::ImageHeader:
				BLACK_LOG_DEBUG( LOG_CHANNEL, "Image header found at chunk #{}.", m_cunks.size() );
				m_header = reinterpret_cast<const Internal::ImageHeader*>( chunk.content.GetMemory() );
				break;
			case Internal::ChunkTypeCode::ImageData:
				BLACK_LOG_DEBUG( LOG_CHANNEL, "Image data found at chunk #{}.", m_cunks.size() );
				m_image = chunk.content;
				break;
			case Internal::ChunkTypeCode::Palette:
				BLACK_LOG_DEBUG( LOG_CHANNEL, "Palette found at chunk #{}.", m_cunks.size() );
				m_palette = chunk.content;
				break;
			default:
				break;
			}
		}

		BLACK_LOG_VERBOSE( LOG_CHANNEL, "File successfully parsed." );
		reset_contract.Cancel();
		m_is_parsed = true;
	}

	void PngFileView::TestFileMemory() const
	{
		m_is_valid = false;

		CRET( m_file_memory.IsEmpty() );
		CRET( !IsHeaderValid( m_file_memory ) );
		CRET( !IsFooterValid( m_file_memory ) );

		m_is_valid = true;
	}
}
}
}
