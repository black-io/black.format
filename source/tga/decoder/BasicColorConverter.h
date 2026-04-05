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
		void UseOutputBuilder( OutputBuilder& builder );

		//
		void UseOutputFormat( const Black::ImageFormat output_format );


		//
		const Black::BooleanStatus ConvertColor( const uint32_t color, const Black::ImageFormat color_format ) const;


		//
		inline const Black::ImageFormat GetOutputFormat() const				{ return m_output_operator.GetFormat(); };

	// Heirs interface.
	protected:
		//
		inline OutputBuilder& GetOutputBuilder() const					{ return *m_output_builder; };

		//
		inline const Black::ColorFormatOperator& GetOutputOperator() const	{ return m_output_operator; };

	// Heirs virtual interface.
	protected:
		//
		virtual const Black::BooleanStatus PerformColorConversion( const uint32_t color, const Black::ImageFormat color_format ) const = 0;

	// Private state.
	private:
		OutputBuilder*	m_output_builder	= nullptr;

		Black::ColorFormatOperator	m_output_operator;
	};
}
}
}
}
