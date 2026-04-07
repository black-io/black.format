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
		void UseInputFeeder( InputFeeder& feeder );

		//
		void UseImageSettings( const Internal::Header& header );


		//
		const uint32_t PeekElement() const;


		//
		inline const size_t GetElementSize() const					{ return ( m_input_feeder == nullptr )? 0 : m_input_feeder->GetElementSize(); };

		//
		inline const Bitrate GetBitrate() const						{ return ( m_input_feeder == nullptr )? Bitrate::Undefined : m_input_feeder->GetBitrate(); };

		//
		inline const Black::ColorFormat GetInputFormat() const		{ return m_input_operator.GetFormat(); };

		//
		inline const Black::ColorFormat GetOutputFormat() const		{ return m_output_operator.GetFormat(); };


		//
		inline const bool HasElements() const						{ return ( m_input_feeder != nullptr ) && m_input_feeder->HasElements(); };

	// Heirs life-time management.
	protected:
		inline BasicColorMapper() noexcept	= default;
		inline ~BasicColorMapper() noexcept	= default;

	// Heirs interface.
	protected:
		//
		void SetOutputFormat( const Black::ColorFormat output_format );

		//
		inline InputFeeder& GetInputFeeder() const						{ return *m_input_feeder; };

		//
		inline const Black::ColorFormatOperator& GetInputOperator() const	{ return m_input_operator; };

		//
		inline const Black::ColorFormatOperator& GetOutputOperator() const	{ return m_output_operator; };

	// Heirs virtual interface.
	protected:
		//
		virtual const uint32_t PerformPeekElement() const = 0;

	// Private state.
	private:
		InputFeeder*				m_input_feeder		= nullptr;

		Black::ColorFormatOperator	m_input_operator;
		Black::ColorFormatOperator	m_output_operator;

		Internal::Bitrate			m_input_bitrate		= Internal::Bitrate::Undefined;
	};
}
}
}
}
