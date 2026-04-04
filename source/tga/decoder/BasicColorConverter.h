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
		const Black::BooleanStatus ConvertColor( const uint32_t color, const Black::ImageFormat color_format );

	// Heirs virtual interface.
	protected:
		//
		virtual const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format );

	// Private state.
	private:
		BasicOutputBuilder*	m_output_builder	= nullptr;

		Black::ImageFormat	m_output_format		= Black::ImageFormats::UNDEFINED;
	};
}
}
}
}
