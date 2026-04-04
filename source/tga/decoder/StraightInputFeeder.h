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
	class StraightInputFeeder final : public BasicInputFeeder
	{
	// Public life-time management.
	public:
		inline StraightInputFeeder() noexcept	= default;
		inline ~StraightInputFeeder() noexcept	= default;

	// `BasicInputFeeder` interface implementations.
	private:
		//
		void OnRewind() final;

		//
		const Black::BooleanStatus OnStepForward() final;

		//
		const std::byte* PerformPeekElement() const final;
	};
}
}
}
}
