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
		const Black::BooleanStatus ConvertFromMonochrome( const uint32_t color, const Black::ImageFormat color_format ) const;

		//
		const Black::BooleanStatus ConvertToMonochrome( const uint32_t color, const Black::ImageFormat color_format ) const;

	// `BasicColorConverter` interface implementations.
	private:
		//
		const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const final;
	};
}
}
}
}
