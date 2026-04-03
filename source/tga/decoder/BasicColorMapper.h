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
		void UseInputFeeder( BasicInputFeeder& feeder );

		//
		void UseImageSettings( const Internal::Header& header );


		//
		const uint32_t PeekElement() const;


		//
		inline const size_t GetElementSize() const					{ return ( m_input_feeder == nullptr )? 0 : m_input_feeder->GetElementSize(); };

		//
		inline const Bitrate GetBitrate() const						{ return ( m_input_feeder == nullptr )? Bitrate::Undefined : m_input_feeder->GetBitrate(); };

		//
		inline const Black::ImageFormat& GetInputFormat() const		{ return m_input_format; };

		//
		inline const Black::ImageFormat& GetOutputFormat() const	{ return m_output_format; };


		//
		inline const bool HasElements() const						{ return ( m_input_feeder != nullptr ) && m_input_feeder->HasElements(); };

	// Heirs life-time management.
	protected:
		inline BasicColorMapper() noexcept	= default;
		inline ~BasicColorMapper() noexcept	= default;

	// Heirs interface.
	protected:
		//
		void SetOutputFormat( const Black::ImageFormat output_format );

		//
		inline BasicInputFeeder& GetInputFeeder() const				{ return *m_input_feeder; };

		//
		inline const size_t GetInputSize() const					{ return m_input_size; };

		//
		inline const size_t GetInputFirstAlphaBit() const			{ return m_input_first_alpha_bit; };

		//
		inline const size_t GetInputColorMask() const				{ return m_input_color_mask; };

		//
		inline const size_t GetInputAlphaMask() const				{ return m_input_alpha_mask; };

		//
		inline const Internal::Bitrate GetInputBitrate() const		{ return m_input_bitrate; };

	// Heirs virtual interface.
	protected:
		//
		virtual const uint32_t PerformPeekElement() const = 0;

	// Private state.
	private:
		BasicInputFeeder*	m_input_feeder			= nullptr;

		size_t				m_input_size			= 0;
		size_t				m_input_first_alpha_bit	= 0;
		uint32_t			m_input_color_mask		= 0;
		uint32_t			m_input_alpha_mask		= 0;
		Black::ImageFormat	m_input_format			= Black::ImageFormats::UNDEFINED;
		Black::ImageFormat	m_output_format			= Black::ImageFormats::UNDEFINED;

		Internal::Bitrate	m_input_bitrate			= Internal::Bitrate::Undefined;
	};
}
}
}
}
