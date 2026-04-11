#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
	/**
	*/
	class PngFileView final
	{
	// Private state.
	private:
		std::vector<Internal::Chunk>		m_cunks;
		const Internal::ImageHeader*		m_header	= nullptr;
		Black::PlainView<const std::byte>	m_palette;
		Black::PlainView<const std::byte>	m_image;
	};
}
}
}
