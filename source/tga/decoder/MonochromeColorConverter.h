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
	class MonochromeColorConverter final : public BasicColorConverter
	{
	// Public life-time management.
	public:
		inline MonochromeColorConverter() noexcept	= default;
		inline ~MonochromeColorConverter() noexcept	= default;

	// Private interface.
	private:
		//
		const Black::BooleanStatus ConvertFromMonochrome( const uint32_t color, const Black::ColorFormat color_format ) const;

		//
		const Black::BooleanStatus ConvertToMonochrome( const uint32_t color, const Black::ColorFormat color_format ) const;

	// `BasicColorConverter` interface implementations.
	private:
		//
		const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ColorFormat color_format ) const final;

	// Private non-state.
	private:
		mutable Black::ColorFormatOperator	m_input_operator;
		mutable size_t						m_red_channel_shrink;
		mutable size_t						m_green_channel_shrink;
		mutable size_t						m_blue_channel_shrink;
	};
}
}
}
}
