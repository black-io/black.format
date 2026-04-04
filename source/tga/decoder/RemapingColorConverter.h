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
	class RemappingColorConverter final : private Black::HostRelatedComponent<DecodePipeline>, public BasicColorConverter
	{
	// Public life-time management.
	public:
		using Black::HostRelatedComponent<DecodePipeline>::HostRelatedComponent;
		~RemappingColorConverter() noexcept = default;

	// `BasicColorMapper` interface implementations.
	private:
		//
		const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const final;
	};
}
}
}
}
