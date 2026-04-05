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
	class DirectColorMapper final : public BasicColorMapper
	{
	// Public life-time management.
	public:
		inline DirectColorMapper() noexcept	= default;
		inline ~DirectColorMapper() noexcept	= default;

	// `BasicColorMapper` interface implementations.
	private:
		//
		const uint32_t PerformPeekElement() const final;
	};
}
}
}
}
