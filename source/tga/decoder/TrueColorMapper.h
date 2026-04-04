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
	class TrueColorMapper final : public BasicColorMapper
	{
	// Public life-time management.
	public:
		inline TrueColorMapper() noexcept	= default;
		inline ~TrueColorMapper() noexcept	= default;

	// `BasicColorMapper` interface implementations.
	private:
		//
		const uint32_t PerformPeekElement() const final;
	};
}
}
}
}
