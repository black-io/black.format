#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
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
		//
		void Swap( ColorFormatOperator& other ) noexcept;


		//
		const uint64_t InsertRedChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		//
		const uint64_t InsertGreenChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		//
		const uint64_t InsertBlueChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		//
		const uint64_t InsertAlphaChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		//
		const uint64_t InsertWhiteChannel( const uint64_t target_color, const uint64_t channel_value ) const;

		//
		const uint64_t InsertIndexChannel( const uint64_t target_color, const uint64_t channel_value ) const;


		//
		const uint64_t ReplaceRedChannel( const uint64_t target_color, const uint64_t source_color ) const;

		//
		const uint64_t ReplaceGreenChannel( const uint64_t target_color, const uint64_t source_color ) const;

		//
		const uint64_t ReplaceBlueChannel( const uint64_t target_color, const uint64_t source_color ) const;

		//
		const uint64_t ReplaceAlphaChannel( const uint64_t target_color, const uint64_t source_color ) const;

		//
		const uint64_t ReplaceWhiteChannel( const uint64_t target_color, const uint64_t source_color ) const;

		//
		const uint64_t ReplaceIndexChannel( const uint64_t target_color, const uint64_t source_color ) const;


		//
		const uint64_t ExtractRedChannel( const uint64_t color ) const;

		//
		const uint64_t ExtractGreenChannel( const uint64_t color ) const;

		//
		const uint64_t ExtractBlueChannel( const uint64_t color ) const;

		//
		const uint64_t ExtractAlphaChannel( const uint64_t color ) const;


		//
		const uint64_t MaskRedChannel( const uint64_t color ) const;

		//
		const uint64_t MaskGreenChannel( const uint64_t color ) const;

		//
		const uint64_t MaskBlueChannel( const uint64_t color ) const;

		//
		const uint64_t MaskAlphaChannel( const uint64_t color ) const;

		//
		const uint64_t MaskWhiteChannel( const uint64_t color ) const;

		//
		const uint64_t MaskIndexChannel( const uint64_t color ) const;


		//
		inline const uint64_t GetRedChannelMask() const		{ return m_red_channel_mask; };

		//
		inline const uint64_t GetGreenChannelMask() const	{ return m_green_channel_mask; };

		//
		inline const uint64_t GetBlueChannelMask() const	{ return m_blue_channel_mask; };

		//
		inline const uint64_t GetAlphaChannelMask() const	{ return m_alpha_channel_mask; };

		//
		inline const uint64_t GetWhiteChannelMask() const	{ return m_white_channel_mask; };

		//
		inline const uint64_t GetIndexMask() const			{ return m_index_mask; };

		//
		inline const ColorFormat GetFormat() const			{ return m_format; };


		//
		inline const bool IsMonochromeFormat() const		{ return m_format.is_monochrome; };

		//
		inline const bool IsColorFormat() const				{ return m_format.is_color; };

		//
		inline const bool IsIndexFormat() const				{ return m_format.is_index; };

		//
		inline const bool CanProcessRedChannel() const		{ return m_has_red_channel; };

		//
		inline const bool CanProcessGreenChannel() const	{ return m_has_green_channel; };

		//
		inline const bool CanProcessBlueChannel() const		{ return m_has_blue_channel; };

		//
		inline const bool CanProcessAlphaChannel() const	{ return m_has_alpha_channel; };

		//
		inline const bool CanProcessWhiteChannel() const	{ return m_has_white_channel; };

		//
		inline const bool CanProcessIndexChannel() const	{ return m_has_index_channel; };

	// Private interface.
	private:
		//
		void ConfigureFlags();

		//
		void CalculateOffsets();

		//
		void CalculateMasks();

	// Private state.
	private:
		size_t		m_red_channel_offset	= 0;
		size_t		m_green_channel_offset	= 0;
		size_t		m_blue_channel_offset	= 0;
		size_t		m_alpha_channel_offset	= 0;
		size_t		m_white_channel_offset	= 0;
		size_t		m_index_offset			= 0;

		uint64_t	m_red_channel_mask		= 0;
		uint64_t	m_green_channel_mask	= 0;
		uint64_t	m_blue_channel_mask		= 0;
		uint64_t	m_alpha_channel_mask	= 0;
		uint64_t	m_white_channel_mask	= 0;
		uint64_t	m_index_mask			= 0;

		ColorFormat	m_format				= ColorFormats::UNDEFINED;

		union
		{
			uint64_t	m_flags_buffer		= 0;

			struct
			{
				bool	m_has_red_channel	: 1;
				bool	m_has_green_channel	: 1;
				bool	m_has_blue_channel	: 1;
				bool	m_has_alpha_channel	: 1;
				bool	m_has_white_channel	: 1;
				bool	m_has_index_channel	: 1;
			};
		};
	};
}
}
}
