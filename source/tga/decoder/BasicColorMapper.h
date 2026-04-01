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
	class BasicColorMapper : private Black::NonTransferable
	{
	// Public inner types.
	public:
		//
		using Bitrate = Black::TgaStructure::Bitrate;

	// Public interface.
	public:
		//
		void RefuseInputFeeder();

		//
		void UseInputFeeder( BasicInputFeeder& const feeder );


		//
		const uint32_t PeekElement() const;


		//
		inline const size_t GetElementSize() const	{ return ( m_input_feeder == nullptr )? 0 : m_input_feeder->GetElementSize(); };

		//
		inline const Bitrate GetBitrate() const		{ return ( m_input_feeder == nullptr )? Bitrate::Undefined : m_input_feeder->GetBitrate(); };


		//
		inline const bool HasElements() const		{ return ( m_input_feeder != nullptr ) && m_input_feeder->HasElements(); };

	// Heirs life-time management.
	protected:
		inline BasicColorMapper() noexcept	= default;
		inline ~BasicColorMapper() noexcept	= default;

	// Heirs interface.
	protected:
		//
		inline BasicInputFeeder& GetInputFeeder() const	{ return *m_input_feeder; };

	// Heirs virtual interface.
	protected:
		//
		virtual const uint32_t PerformPeekElement() const = 0;

	// Private state.
	private:
		BasicInputFeeder*	m_input_feeder = nullptr;
	};
}
}
}
}
