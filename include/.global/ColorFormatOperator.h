#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
		@brief	Color operator, based on given color format.

		Operator can perform various operations on color values. Channel manipulations, masking or even combining of color values.
		All operations are made based on color format, that was used to initialize the operator.
		So each given color value is considered of that color format, which was used to initialize the operator.
	*/
	class ColorFormatOperator final
	{
	// Friendship interface.
	public:
		inline void swap( ColorFormatOperator& left, ColorFormatOperator& right )	{ left.Swap( right ); };

	// Public life-time management.
	public:
		inline ColorFormatOperator() noexcept									= default;
		inline ColorFormatOperator( const ColorFormatOperator& other ) noexcept	= default;
		ColorFormatOperator( ColorFormatOperator&& other ) noexcept;

		explicit ColorFormatOperator( ColorFormat format ) noexcept;

		inline ~ColorFormatOperator() noexcept	= default;

		inline ColorFormatOperator& operator = ( const ColorFormatOperator& other ) noexcept	= default;
		ColorFormatOperator& operator = ( ColorFormatOperator&& other ) noexcept;

		inline ColorFormatOperator& operator = ( ColorFormat format ) noexcept					{ return Black::CopyAndSwap( *this, format ); };

	// Public interface.
	public:
		// swap the state of this operator with other.
		void Swap( ColorFormatOperator& other ) noexcept;


		/**
			@brief	Insert the value of red channel into target color.

			The value of channel considered here as 0-based and out of position in packed color format.
			In case of no red channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	channel_value	Zero-based value of red channel.
			@return					The value returned is `target_color` after the `channel_value` is inserted into it.
		*/
		const uint64_t InsertRedChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		/**
			@brief	Insert the value of green channel into target color.

			The value of channel considered here as 0-based and out of position in packed color format.
			In case of no green channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	channel_value	Zero-based value of green channel.
			@return					The value returned is `target_color` after the `channel_value` is inserted into it.
		*/
		const uint64_t InsertGreenChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		/**
			@brief	Insert the value of blue channel into target color.

			The value of channel considered here as 0-based and out of position in packed color format.
			In case of no blue channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	channel_value	Zero-based value of blue channel.
			@return					The value returned is `target_color` after the `channel_value` is inserted into it.
		*/
		const uint64_t InsertBlueChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		/**
			@brief	Insert the value of alpha channel into target color.

			The value of channel considered here as 0-based and out of position in packed color format.
			In case of no alpha channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	channel_value	Zero-based value of alpha channel.
			@return					The value returned is `target_color` after the `channel_value` is inserted into it.
		*/
		const uint64_t InsertAlphaChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		/**
			@brief	Insert the value of white channel into target color.

			The value of channel considered here as 0-based and out of position in packed color format.
			In case of no white channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	channel_value	Zero-based value of white channel.
			@return					The value returned is `target_color` after the `channel_value` is inserted into it.
		*/
		const uint64_t InsertWhiteChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		/**
			@brief	Insert the value of palette index into target color.

			The value of channel considered here as 0-based and out of position in packed color format.
			In case of no palette index in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	channel_value	Zero-based value of palette index.
			@return					The value returned is `target_color` after the `channel_value` is inserted into it.
		*/
		const uint64_t InsertIndexChannel( const uint64_t target_color, const uint64_t channel_value ) const;


		/**
			@brief	Replace the red channel in target color with one given from source color.

			To set all the bits of channel, one can use result of `GetRedChannelMask()` as source color.
			Source and target colors are considered valid color of operators color format.
			In case of no red channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	source_color	Valid color with valuable red channel.
			@return					The value returned is `target_color` with only red channel replaced from `source_color`.
		*/
		const uint64_t ReplaceRedChannel( const uint64_t target_color, const uint64_t source_color ) const;

		/**
			@brief	Replace the green channel in target color with one given from source color.

			To set all the bits of channel, one can use result of `GetGreenChannelMask()` as source color.
			Source and target colors are considered valid color of operators color format.
			In case of no green channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	source_color	Valid color with valuable green channel.
			@return					The value returned is `target_color` with only green channel replaced from `source_color`.
		*/
		const uint64_t ReplaceGreenChannel( const uint64_t target_color, const uint64_t source_color ) const;

		/**
			@brief	Replace the blue channel in target color with one given from source color.

			To set all the bits of channel, one can use result of `GetBlueChannelMask()` as source color.
			Source and target colors are considered valid color of operators color format.
			In case of no blue channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	source_color	Valid color with valuable blue channel.
			@return					The value returned is `target_color` with only blue channel replaced from `source_color`.
		*/
		const uint64_t ReplaceBlueChannel( const uint64_t target_color, const uint64_t source_color ) const;

		/**
			@brief	Replace the alpha channel in target color with one given from source color.

			To set all the bits of channel, one can use result of `GetAlphaChannelMask()` as source color.
			Source and target colors are considered valid color of operators color format.
			In case of no alpha channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	source_color	Valid color with valuable alpha channel.
			@return					The value returned is `target_color` with only alpha channel replaced from `source_color`.
		*/
		const uint64_t ReplaceAlphaChannel( const uint64_t target_color, const uint64_t source_color ) const;

		/**
			@brief	Replace the white channel in target color with one given from source color.

			To set all the bits of channel, one can use result of `GetWhiteChannelMask()` as source color.
			Source and target colors are considered valid color of operators color format.
			In case of no white channel in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	source_color	Valid color with valuable white channel.
			@return					The value returned is `target_color` with only white channel replaced from `source_color`.
		*/
		const uint64_t ReplaceWhiteChannel( const uint64_t target_color, const uint64_t source_color ) const;

		/**
			@brief	Replace the palette index in target color with one given from source color.

			To set all the bits of channel, one can use result of `GetIndexMask()` as source color.
			Source and target colors are considered valid color of operators color format.
			In case of no palette index in color format, `target_color` will be bypassed into result.

			@param	target_color	Given color to be modified.
			@param	source_color	Valid color with valuable palette index.
			@return					The value returned is `target_color` with only palette index replaced from `source_color`.
		*/
		const uint64_t ReplaceIndexChannel( const uint64_t target_color, const uint64_t source_color ) const;


		/**
			@brief	Extract the value of red channel from given color.

			@param	color	Given color with valuable red channel.
			@return			The value returned is zero-based red channel of given `color`, stored out of channel packing.
			@retval	0		In case of no red channel declared in color format.
		*/
		const uint64_t ExtractRedChannel( const uint64_t color ) const;

		/**
			@brief	Extract the value of green channel from given color.

			@param	color	Given color with valuable green channel.
			@return			The value returned is zero-based green channel of given `color`, stored out of channel packing.
			@retval	0		In case of no green channel declared in color format.
		*/
		const uint64_t ExtractGreenChannel( const uint64_t color ) const;

		/**
			@brief	Extract the value of blue channel from given color.

			@param	color	Given color with valuable blue channel.
			@return			The value returned is zero-based blue channel of given `color`, stored out of channel packing.
			@retval	0		In case of no blue channel declared in color format.
		*/
		const uint64_t ExtractBlueChannel( const uint64_t color ) const;

		/**
			@brief	Extract the value of alpha channel from given color.

			@param	color	Given color with valuable alpha channel.
			@return			The value returned is zero-based alpha channel of given `color`, stored out of channel packing.
			@retval	0		In case of no alpha channel declared in color format.
		*/
		const uint64_t ExtractAlphaChannel( const uint64_t color ) const;


		/**
			@brief	Produce new color, where only red channel exists from given `color`.

			@param	color	Given color with valuable red channel.
			@return			The value returned is valid color, where only red channel saved from given one.
		*/
		const uint64_t MaskRedChannel( const uint64_t color ) const;

		/**
			@brief	Produce new color, where only green channel exists from given `color`.

			@param	color	Given color with valuable green channel.
			@return			The value returned is valid color, where only green channel saved from given one.
		*/
		const uint64_t MaskGreenChannel( const uint64_t color ) const;

		/**
			@brief	Produce new color, where only blue channel exists from given `color`.

			@param	color	Given color with valuable blue channel.
			@return			The value returned is valid color, where only blue channel saved from given one.
		*/
		const uint64_t MaskBlueChannel( const uint64_t color ) const;

		/**
			@brief	Produce new color, where only alpha channel exists from given `color`.

			@param	color	Given color with valuable alpha channel.
			@return			The value returned is valid color, where only alpha channel saved from given one.
		*/
		const uint64_t MaskAlphaChannel( const uint64_t color ) const;

		/**
			@brief	Produce new color, where only white channel exists from given `color`.

			@param	color	Given color with valuable white channel.
			@return			The value returned is valid color, where only white channel saved from given one.
		*/
		const uint64_t MaskWhiteChannel( const uint64_t color ) const;

		/**
			@brief	Produce new color, where only palette index exists from given `color`.

			@param	color	Given color with valuable palette index.
			@return			The value returned is valid color, where only palette index saved from given one.
		*/
		const uint64_t MaskIndexChannel( const uint64_t color ) const;


		// Get the mask of red channel.
		inline const uint64_t GetRedChannelMask() const		{ return m_red_channel_mask; };

		// Get the mask of green channel.
		inline const uint64_t GetGreenChannelMask() const	{ return m_green_channel_mask; };

		// Get the mask of blue channel.
		inline const uint64_t GetBlueChannelMask() const	{ return m_blue_channel_mask; };

		// Get the mask of alpha channel.
		inline const uint64_t GetAlphaChannelMask() const	{ return m_alpha_channel_mask; };

		// Get the mask of white channel.
		inline const uint64_t GetWhiteChannelMask() const	{ return m_white_channel_mask; };

		// Get the mask of palette index.
		inline const uint64_t GetIndexMask() const			{ return m_index_mask; };

		// Get the color format used to initialize this operator.
		inline const ColorFormat GetFormat() const			{ return m_format; };


		// Whether the color format describes monochrome color.
		inline const bool IsMonochromeFormat() const		{ return m_format.is_monochrome; };

		// Whether the color format describes color.
		inline const bool IsColorFormat() const				{ return m_format.is_color; };

		// Whether the color format describes palette index.
		inline const bool IsIndexFormat() const				{ return m_format.is_index; };

		// Whether the operator can process red channel.
		inline const bool CanProcessRedChannel() const		{ return m_has_red_channel; };

		// Whether the operator can process green channel.
		inline const bool CanProcessGreenChannel() const	{ return m_has_green_channel; };

		// Whether the operator can process blue channel.
		inline const bool CanProcessBlueChannel() const		{ return m_has_blue_channel; };

		// Whether the operator can process alpha channel.
		inline const bool CanProcessAlphaChannel() const	{ return m_has_alpha_channel; };

		// Whether the operator can process white channel.
		inline const bool CanProcessWhiteChannel() const	{ return m_has_white_channel; };

		// Whether the operator can process palette indices.
		inline const bool CanProcessIndexChannel() const	{ return m_has_index_channel; };

	// Private interface.
	private:
		// Perform the configuration of boolean flags in state of operator.
		void ConfigureFlags();

		// Perform the configuration of channel offsets.
		void CalculateOffsets();

		// Perform the configuration of channel masks.
		void CalculateMasks();

	// Private state.
	private:
		size_t		m_red_channel_offset	= 0;	// Bitwise offset of red channel in given color format.
		size_t		m_green_channel_offset	= 0;	// Bitwise offset of green channel in given color format.
		size_t		m_blue_channel_offset	= 0;	// Bitwise offset of blue channel in given color format.
		size_t		m_alpha_channel_offset	= 0;	// Bitwise offset of alpha channel in given color format.
		size_t		m_white_channel_offset	= 0;	// Bitwise offset of white channel in given color format.
		size_t		m_index_offset			= 0;	// Bitwise offset of palette index in given color format.

		uint64_t	m_red_channel_mask		= 0;	// Bitwise mask of red channel according to given color format.
		uint64_t	m_green_channel_mask	= 0;	// Bitwise mask of green channel according to given color format.
		uint64_t	m_blue_channel_mask		= 0;	// Bitwise mask of blue channel according to given color format.
		uint64_t	m_alpha_channel_mask	= 0;	// Bitwise mask of alpha channel according to given color format.
		uint64_t	m_white_channel_mask	= 0;	// Bitwise mask of white channel according to given color format.
		uint64_t	m_index_mask			= 0;	// Bitwise mask of palette index according to given color format.

		ColorFormat	m_format				= ColorFormats::UNDEFINED; // Color format, given in initialization of operator.

		union
		{
			uint64_t	m_flags_buffer		= 0;	// Buffer for boolean flags.

			struct
			{
				bool	m_has_red_channel	: 1;	// Whether the given color format declares red channel.
				bool	m_has_green_channel	: 1;	// Whether the given color format declares green channel.
				bool	m_has_blue_channel	: 1;	// Whether the given color format declares blue channel.
				bool	m_has_alpha_channel	: 1;	// Whether the given color format declares alpha channel.
				bool	m_has_white_channel	: 1;	// Whether the given color format declares white channel.
				bool	m_has_index_channel	: 1;	// Whether the given color format declares palette index.
			};
		};
	};
}
}
}
