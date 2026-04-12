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

	PngFileView::PngFileView( Black::PlainView<const std::byte> file_memory )
	{
	}

	PngFileView::PngFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace )
	{
	}

	PngFileView& PngFileView::operator=( Black::PlainView<const std::byte> file_memory ) noexcept
	{
	}

	void PngFileView::Reset()
	{
	}

	void PngFileView::Swap( PngFileView& other )
	{
	}

	const bool PngFileView::HasValidHeader() const
	{
	}

	const bool PngFileView::HasValidFooter() const
	{
	}

	const bool PngFileView::IsValidFile() const
	{
	}

	const bool PngFileView::IsEmpty() const
	{
	}
}
}
}
