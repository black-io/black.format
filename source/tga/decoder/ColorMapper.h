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
		@brief	Color mapper for image decoding pipeline.

		Color mapper is placed at second step of TGA image decoding pipeline. It perform valid color production from elements of input image.

		Color mapper can operate in two distinct modes. In first mode it just bypass the element of input image into color with declared input bit-rate.
		The second mode is required to support the Color Mapped TGA images. It treats the element of input image as palette index and perform color mapping
		using given memory of palette.
		In case the element of input image carries alpha channel along with palette index, color mapper performs the migration of such alpha channel into result.

		To operate in each mode, the color mapper may be prepared by means of factory functions.
	*/
	class ColorMapper final
	{
	// Friendship interface.
	public:
		friend inline void swap( ColorMapper& left, ColorMapper& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		/**
			@brief	Factory function to initialize the mapper for bypassing the input data as color.

			@param	header	TGA file header, given from input image.
			@return			The value returned is ready to work color mapper.
		*/
		static ColorMapper BuildDirectMapper( const Internal::Header& header );

		/**
			@brief	Factory function to initialize the mapper for mapping the input data into palette colors.

			@param	header			TGA file header, given from input image.
			@param	palette_buffer	Memory of palette from TGA file.
			@return					The value returned is ready to work color mapper.
		*/
		static ColorMapper BuildPaletteMapper( const Internal::Header& header, Black::PlainView<const std::byte> palette_buffer );

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
		// Swap the state of this mapper with other.
		void Swap( ColorMapper& other );


		/**
			@brief	Map the given color buffer into color.

			Typically the input feeder should be used to obtain the color buffer.

			@param	color_buffer	Buffer with element of input image.
			@return					The value returned is color, that formed according to configuration of color mapper.
		*/
		const uint32_t MapColor( const std::byte* const color_buffer ) const;


		// Get the color format of input elements.
		inline const Black::ColorFormat GetInputFormat() const { return m_input_operator.GetFormat(); };

		// Get the format of output color.
		inline const Black::ColorFormat GetOutputFormat() const { return m_output_operator.GetFormat(); };

	// Private interface.
	private:
		// Peek the color from palette, using given index of color element.
		const uint32_t PeekPaletteElement( const size_t element_index ) const;

	// Private state.
	private:
		Black::PlainView<const std::byte> m_palette;	// Buffer for palette.

		Black::ColorFormatOperator	m_input_operator;	// Color operator for mapper input.
		Black::ColorFormatOperator	m_output_operator;	// Color operator of mapper output.

		Internal::Bitrate			m_input_bitrate		= Internal::Bitrate::Undefined;	// Bit-rate of mapper input.
		Internal::Bitrate			m_palette_bitrate	= Internal::Bitrate::Undefined;	// Bit-rate of mapper output.
	};
}
}
}
}
