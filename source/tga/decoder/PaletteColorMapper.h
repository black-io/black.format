#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Decoder
{
	/**
	*/
	class PaletteColorMapper final : public BasicColorMapper
	{
	// Public life-time management.
	public:
		inline PaletteColorMapper() noexcept	= default;
		inline ~PaletteColorMapper() noexcept	= default;

	// Public interface.
	public:
		//
		void UsePalette( Black::PlainView<const std::byte> palette_buffer, const Internal::Bitrate bitrate );

		//
		void FixOutputFormat( const Internal::Header& header );

	// Private interface.
	private:
		//
		const uint32_t PeekPaletteElement( const size_t element_index ) const;

	// `BasicColorMapper` interface implementations.
	private:
		//
		const uint32_t PerformPeekElement() const final;

	// Private state.
	private:
		Black::PlainView<const std::byte> m_palette;

		Internal::Bitrate m_palette_bitrate	= Internal::Bitrate::Undefined;
	};
}
}
}
}
