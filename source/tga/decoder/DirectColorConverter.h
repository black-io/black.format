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
	class DirectColorConverter final : private Black::HostRelatedComponent<DecodePipeline>, public BasicColorConverter
	{
	// Public life-time management.
	public:
		using Black::HostRelatedComponent<DecodePipeline>::HostRelatedComponent;
		~DirectColorConverter() noexcept = default;

	// `BasicColorConverter` interface implementations.
	private:
		//
		const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const final;
	};
}
}
}
}
