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
	class MonochromeColorMapper final : public BasicColorMapper
	{
	// Public life-time management.
	public:
		inline MonochromeColorMapper() noexcept		= default;
		inline ~MonochromeColorMapper() noexcept	= default;

	// Public interface.
	public:
		//
		void FixOutputFormat( const Internal::Header& header );

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
