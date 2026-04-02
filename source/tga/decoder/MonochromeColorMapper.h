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
	class MonochromeColorMapper final : private Black::HostRelatedComponent<DecodePipeline>, public BasicColorMapper
	{
	// Public life-time management.
	public:
		using Black::HostRelatedComponent<DecodePipeline>::HostRelatedComponent;
		~MonochromeColorMapper() noexcept = default;

	// `BasicColorMapper` interface implementations.
	private:
		//
		const uint32_t PerformPeekElement() const final;

	// Private state.
	private:
		size_t m_input_color_shrink_bits = 0; // Dithering.
	};
}
}
}
}
