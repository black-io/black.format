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
	class StraightInputFeeder final : private Black::HostRelatedComponent<DecodePipeline>, public BasicInputFeeder
	{
	// Friendship declarations.
	public:
		// Allow to use interface of composition.
		friend class DecodePipeline;

	// Public life-time management.
	public:
		StraightInputFeeder() noexcept	= default;
		~StraightInputFeeder() noexcept	= default;

	// `BasicInputFeeder` interface implementations.
	private:
		//
		void OnRewind() final;

		//
		const Black::BooleanStatus OnStepForward() final;
	};
}
}
}
}
