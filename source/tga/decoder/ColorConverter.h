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
		@brief	Color converter for image decoding pipeline.

		Color converter is the third step in TGA image decoding pipeline. It converts the color of image input into format of decoded image.

		Converter is the most complex part of decode pipeline. Conversion of color between different formats is complex due to great number
		of different color formats.
		The converter can be configured in one of 5 different modes.

		Direct converter just bypass the color from mapper to output. It is useful in case the input and output color formats are same.
		Monochrome converter will convert the grayscale image from input image.
		Remapping converter can change the order of color channels from input format to output format.
		Transforming converter is the most expensive and can totally rebuild the input color into output one.

		There are few factory functions to obtain the converter in each mode.
	*/
	class ColorConverter final
	{
	// Friendship interface.
	public:
		friend inline void swap( ColorConverter& left, ColorConverter& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		/**
			@brief	Factory function to initialize the converter in monochrome converting mode.

			In this mode, the color converter will produce monochrome color from input in RGB format.

			@param	header			TGA file header that describe input format for converter.
			@param	output_format	Format of converter output.
			@return					The value returned is ready to work color converter.
		*/
		static ColorConverter BuildRgbToMonochromeConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		/**
			@brief	Factory function to initialize the converter in monochrome converting mode.

			In this mode, the color converter will produce grayscale color in RGB format from monochrome input format.

			@param	header			TGA file header that describe input format for converter.
			@param	output_format	Format of converter output.
			@return					The value returned is ready to work color converter.
		*/
		static ColorConverter BuildMonochromeToRgbConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		/**
			@brief	Factory function to initialize the converter in direct mode.

			In this mode, the color converter bypass the input color as output color.

			@param	header			TGA file header that describe input format for converter.
			@param	output_format	Format of converter output.
			@return					The value returned is ready to work color converter.
		*/
		static ColorConverter SetupDirectConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		/**
			@brief	Factory function to initialize the converter in remapping mode.

			In this mode, the color converter split the input color into channels and bake it in order of output format.

			@param	header			TGA file header that describe input format for converter.
			@param	output_format	Format of converter output.
			@return					The value returned is ready to work color converter.
		*/
		static ColorConverter SetupRemappingConverter( const Internal::Header& header, const Black::ColorFormat output_format );

		/**
			@brief	Factory function to initialize the converter in transforming mode.

			In this mode, the color converter rebuild the output color totally. The input color used as instruction how to build output color.

			@param	header			TGA file header that describe input format for converter.
			@param	output_format	Format of converter output.
			@return					The value returned is ready to work color converter.
		*/
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
		// Swap the state of this converter with other.
		void Swap( ColorConverter& other );


		/**
			@brief	Perform the color conversion.

			Depending on mode of converter, the behavior and result of this method will be different.

			@param	color	Input color to be converted.
			@return			The value returned is valid color in format the converter output configured.
		*/
		const uint32_t ConvertColor( const uint32_t color ) const;

	// Private inner types.
	private:
		// Type of conversion method.
		using ConvertMethod = const uint64_t (ColorConverter::*)( const uint32_t color ) const;

	// Private interface.
	private:
		/**
			@brief	Perform the color bypassing.
			This method implements the direct mode of converter.

			@param	color	Input color of converter.
			@return			Same color as given input.
		*/
		const uint64_t BypassColor( const uint32_t color ) const;

		/**
			@brief	Perform the monochrome conversion.
			This method implements the monochrome mode of converter.
			Input color format is single-channel whiteness, that will be used to fill the channels of output RGB color.

			@param	color	Input monochrome color.
			@return			RGB grayscale color.
		*/
		const uint64_t ConvertFromMonochrome( const uint32_t color ) const;

		/**
			@brief	Perform the monochrome conversion.
			This method implements the monochrome mode of converter.
			Input color format is RGB, whose all channels will be combined in single-channel whiteness of output monochrome color.

			@param	color	Input RGB color.
			@return			Monochrome color.
		*/
		const uint64_t ConvertToMonochrome( const uint32_t color ) const;

		/**
			@brief	Perform the remapping of color channels.
			This method implements the remapping mode of converter.
			The input and output channels are identical, except the positions of channels in color. On the conversion, the channels of input color
			will be ordered as output format describes.

			@param	color	Input RGB color.
			@return			RGB color that satisfy the output format.
		*/
		const uint64_t RemapChannels( const uint32_t color ) const;

		/**
			@brief	Perform the transform of input color.
			This method implements the transforming mode of converter.

			@param	color	Input RGB color.
			@return			RGB color that satisfy the output format.
		*/
		const uint64_t TransformChannels( const uint32_t color ) const;

	// Private state.
	private:
		ConvertMethod				m_convert_method	= &ColorConverter::BypassColor;	// Selected mode of converter.
		Black::ColorFormatOperator	m_input_operator;									// Color operator for converter input.
		Black::ColorFormatOperator	m_output_operator;									// Color operator for converter output.
	};
}
}
}
}
