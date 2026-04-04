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
	class BasicColorConverter : private Black::NonTransferable
	{
	// Public interface.
	public:
		//
		void RefuseOutputBuilder();

		//
		void UseOutputBuilder( BasicOutputBuilder& builder );

		//
		void UseOutputFormat( const Black::ImageFormat output_format );


		//
		const Black::BooleanStatus ConvertColor( const uint32_t color, const Black::ImageFormat color_format ) const;


		//
		inline const Black::ImageFormat GetOutputFormat() const	{ return m_output_format; };

	// Heirs interface.
	protected:
		//
		inline BasicOutputBuilder& GetOutputBuilder() const		{ return *m_output_builder; };

	// Heirs virtual interface.
	protected:
		//
		virtual const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const = 0;

	// Private state.
	private:
		BasicOutputBuilder*	m_output_builder	= nullptr;

		uint32_t			m_output_alpha_mask	= 0;
		Black::ImageFormat	m_output_format		= Black::ImageFormats::UNDEFINED;
	};
}
}
}
}
