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
	}

	const bool PngFileView::IsFooterValid( const Black::PlainView<const std::byte>& file_memory )
	{
	}

	const bool PngFileView::IsFileValid( const Black::PlainView<const std::byte>& file_memory )
	{
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
	}

	const bool PngFileView::HasValidHeader() const
	{
		return !m_cunks.empty() && m_cunks.front().type_code == Internal::TYPE_CODE_IHDR;
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
