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
	class ColorMapper final
	{
	// Friendship interface.
	public:
		friend inline void swap( ColorMapper& left, ColorMapper& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		//
		static ColorMapper SetupDirectMapper( const Internal::Header& header );

		//
		static ColorMapper SetupPaletteMapper( const Internal::Header& header, Black::PlainView<const std::byte> palette_buffer );

	// Public life-time management.
	public:
		inline ColorMapper() noexcept = default;
		inline ColorMapper( const ColorMapper& other ) noexcept = default;
		ColorMapper( ColorMapper&& other ) noexcept;

		inline ~ColorMapper() noexcept = default;

		inline ColorMapper& operator = ( const ColorMapper& other ) noexcept = default;
		ColorMapper& operator = ( ColorMapper&& other ) noexcept;

	// Public interface.
	public:
		//
		void Swap( ColorMapper& other );


		//
		const uint32_t MapColor( const std::byte* color_buffer ) const;

	// Private interface.
	private:
		//
		const uint32_t PeekPaletteElement( const size_t element_index ) const;

	// Private state.
	private:
		Black::PlainView<const std::byte> m_palette;

		Black::ColorFormatOperator	m_input_operator;
		Black::ColorFormatOperator	m_output_operator;

		Internal::Bitrate			m_input_bitrate		= Internal::Bitrate::Undefined;
		Internal::Bitrate			m_palette_bitrate	= Internal::Bitrate::Undefined;
	};
}
}
}
}
