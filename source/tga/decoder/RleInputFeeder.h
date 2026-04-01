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
	class RleInputFeeder final : private Black::HostRelatedComponent<DecodePipeline>, public BasicInputFeeder
	{
	// Public life-time management.
	public:
		RleInputFeeder() noexcept	= default;
		~RleInputFeeder() noexcept	= default;

	// Private interface.
	private:
		//
		const Black::BooleanStatus ReadCurrentBlock() const;

	// `BasicInputFeeder` interface implementations.
	private:
		//
		void OnRewind() final;

		//
		const Black::BooleanStatus OnStepForward() final;

		//
		const std::byte* PerformPeekElement() const final;

	// Private non-state.
	private:
		mutable const std::byte*	m_block_payload			= nullptr;
		mutable size_t				m_block_position		= 0;
		mutable size_t				m_block_length			= 0;
		mutable size_t				m_block_size			= 0;
		mutable bool				m_is_block_compressed	= false;
	};
}
}
}
}
