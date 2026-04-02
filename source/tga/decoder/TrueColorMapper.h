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
	class TrueColorMapper final : private Black::HostRelatedComponent<DecodePipeline>, public BasicColorMapper
	{
	// Public life-time management.
	public:
		using Black::HostRelatedComponent<DecodePipeline>::HostRelatedComponent;
		~TrueColorMapper() noexcept = default;

	// `BasicColorMapper` interface implementations.
	private:
		//
		const uint32_t PerformPeekElement() const final;
	};
}
}
}
}
