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
	class ColorConverter final
	{
	// Friendship interface.
	public:
		friend inline void swap( ColorConverter& left, ColorConverter& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		//
		static ColorConverter SetupRgbToMonochromeConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		//
		static ColorConverter SetupMonochromeToRgbConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		//
		static ColorConverter SetupDirectConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		//
		static ColorConverter SetupRemappingConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		//
		static ColorConverter SetupTransformConverter( const Internal::Header& header, const Black::ColorFormat output_format );

	// Public life-time management.
	public:
		inline ColorConverter() noexcept = default;
		inline ColorConverter( const ColorConverter& other ) noexcept = default;
		ColorConverter( ColorConverter&& other ) noexcept;

		inline ~ColorConverter() noexcept = default;

		inline ColorConverter& operator = ( const ColorConverter& other ) noexcept = default;
		ColorConverter& operator = ( ColorConverter&& other ) noexcept;

	// Public interface.
	public:
		//
		void Swap( ColorConverter& other );


		//
		const uint32_t ConvertColor( const uint32_t color );

	// Private interface.
	private:

	// Private state.
	private:
		Black::ColorFormatOperator	m_input_operator;
		Black::ColorFormatOperator	m_output_operator;
	};
}
}
}
}
