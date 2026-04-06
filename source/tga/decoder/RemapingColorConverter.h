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
	class RemappingColorConverter final : public BasicColorConverter
	{
	// Public life-time management.
	public:
		inline RemappingColorConverter() noexcept	= default;
		inline ~RemappingColorConverter() noexcept	= default;

	// `BasicColorConverter` interface implementations.
	private:
		//
		const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ColorFormat color_format ) const final;
	};
}
}
}
}
