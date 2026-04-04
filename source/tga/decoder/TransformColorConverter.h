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
	class TransformColorConverter final : public BasicColorConverter
	{
	// Public life-time management.
	public:
		inline TransformColorConverter() noexcept	= default;
		inline ~TransformColorConverter() noexcept	= default;

	// `BasicColorConverter` interface implementations.
	private:
		//
		const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const final;
	};
}
}
}
}
