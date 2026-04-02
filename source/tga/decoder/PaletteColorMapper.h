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
	class PaletteColorMapper final : private Black::HostRelatedComponent<DecodePipeline>, public BasicColorMapper
	{
	// Public life-time management.
	public:
		using Black::HostRelatedComponent<DecodePipeline>::HostRelatedComponent;
		~PaletteColorMapper() noexcept = default;

	// Public interface.
	public:
		//
		void UsePalette( Black::PlainView<const std::byte> palette_buffer, const Internal::Bitrate bitrate );

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
		Black::PlainView<const std::byte>	m_palette;

		size_t				m_output_first_alpha_bit	= 0;
		size_t				m_output_color_mask			= 0;
		size_t				m_output_alpha_mask			= 0;
		size_t				m_palete_element_size		= 0;
		Black::ImageFormat	m_palete_format				= Black::ImageFormats::UNDEFINED;

		Internal::Bitrate	m_bitrate					= Internal::Bitrate::Undefined;
	};
}
}
}
}
