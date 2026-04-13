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

		constexpr size_t file_preambula_size = std::size( Internal::FILE_PREAMBULA );
		CRET( buffer.GetLength() < file_preambula_size, false );
		CRET( !Black::IsMemoryEqual( buffer.GetMemory(), Internal::FILE_PREAMBULA, file_preambula_size ), false );
		buffer = buffer.TruncatePrefix( file_preambula_size );

		constexpr size_t chunk_header_size = sizeof( Internal::Chunk::content_size ) + sizeof( Internal::Chunk::type_code );
		CRET( buffer.GetLength() < chunk_header_size, false );
		Internal::Chunk header_chunk;
		Black::CopyMemory( &header_chunk, buffer.GetMemory(), chunk_header_size );
		buffer = buffer.TruncatePrefix( chunk_header_size );

		CRET( header_chunk.content_size != sizeof( Internal::ImageHeader ), false );
		CRET( header_chunk.type_code = Internal::TYPE_CODE_IHDR, false );
		CRET( buffer.GetLength() < header_chunk.content_size, false );

		constexpr size_t chunk_checksumm_size = sizeof( Internal::Chunk::checksumm );
		const Internal::ImageHeader* const header = reinterpret_cast<const Internal::ImageHeader*>( buffer.GetMemory() );
		buffer = buffer.TruncatePrefix( header_chunk.content_size );
		CRET( header->width == 0, false );
		CRET( header->height == 0, false );

		CRET( buffer.GetLength() < chunk_checksumm_size, false );
		Black::CopyMemory( &header_chunk.checksumm, buffer.GetMemory(), chunk_checksumm_size );

		return true;
	}

	const bool PngFileView::IsFooterValid( const Black::PlainView<const std::byte>& file_memory )
	{
		constexpr size_t chunk_header_size = sizeof( Internal::Chunk::content_size ) + sizeof( Internal::Chunk::type_code );
		constexpr size_t chunk_checksumm_size = sizeof( Internal::Chunk::checksumm );
		constexpr size_t chunk_size = chunk_header_size + chunk_checksumm_size;

		CRET( file_memory.GetLength() < chunk_size, false );
		const Black::PlainView<const std::byte> chunk_buffer{ file_memory.GetSubview( file_memory.GetLength() - chunk_size, chunk_size ) };

		Internal::Chunk footer_chunk;
		Black::CopyMemory( &footer_chunk, chunk_buffer.GetMemory(), chunk_header_size );
		Black::CopyMemory( &footer_chunk.checksumm, &chunk_buffer[ chunk_header_size ], chunk_checksumm_size );

		CRET( footer_chunk.content_size != 0, false );
		CRET( footer_chunk.type_code != Internal::TYPE_CODE_IEND, false );

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

	PngFileView& PngFileView::operator=( PngFileView&& other ) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( other ) );
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

	const PngStructure::Header* const PngFileView::QueryHeader() const
	{
		CRET( !m_is_valid, nullptr );

		EnsureFileMemoryParsed();
		CRET( !m_is_parsed, nullptr );

		return m_header;
	}

	const PngStructure::Header& PngFileView::GetHeader() const
	{
		EXPECTS( m_is_valid );

		const PngStructure::Header* const header = QueryHeader();
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
		return !chunks.empty() && chunks.front().type_code == Internal::TYPE_CODE_IHDR;
	}

	const bool PngFileView::HasValidFooter() const
	{
		return !m_cunks.empty() && m_cunks.back().type_code == Internal::TYPE_CODE_IEND;
	}

	const bool PngFileView::IsValidFile() const
	{
		return HasValidHeader() && !GetImageBuffer().IsEmpty();
	}

	const bool PngFileView::IsEmpty() const
	{
		return m_file_memory.IsEmpty() || ( QueryHeader() == nullptr ) || GetImageBuffer().IsEmpty();
	}
}
}
}
