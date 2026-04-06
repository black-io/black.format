#include <black/format.h>


namespace Black
{
inline namespace Format
{
inline namespace Global
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/ColorFormat/Operator";
}


namespace
{
	//
	const uint64_t ReplaceChannel( const uint64_t target, const uint64_t source, const uint64_t mask )
	{
		return ( target & ~mask ) | ( source & mask );
	}

	//
	const uint64_t InsertChannel( const uint64_t target, const uint64_t channel, const uint64_t mask, const size_t offset )
	{
		return ReplaceChannel( target, channel << offset, mask );
	}
}


	ColorFormatOperator::ColorFormatOperator( ColorFormat format ) noexcept
		: m_format{ format }
	{
		ConfigureFlags();
		CalculateOffsets();
		CalculateMasks();
	}

	void ColorFormatOperator::Swap( ColorFormatOperator& other ) noexcept
	{
		Black::Swap( m_red_channel_offset, other.m_red_channel_offset );
		Black::Swap( m_green_channel_offset, other.m_green_channel_offset );
		Black::Swap( m_blue_channel_offset, other.m_blue_channel_offset );
		Black::Swap( m_alpha_channel_offset, other.m_alpha_channel_offset );
		Black::Swap( m_white_channel_offset, other.m_white_channel_offset );
		Black::Swap( m_index_offset, other.m_index_offset );
		Black::Swap( m_red_channel_mask, other.m_red_channel_mask );
		Black::Swap( m_green_channel_mask, other.m_green_channel_mask );
		Black::Swap( m_blue_channel_mask, other.m_blue_channel_mask );
		Black::Swap( m_alpha_channel_mask, other.m_alpha_channel_mask );
		Black::Swap( m_white_channel_mask, other.m_white_channel_mask );
		Black::Swap( m_index_mask, other.m_index_mask );
		Black::Swap( m_format, other.m_format );
		Black::Swap( m_flags_buffer, other.m_flags_buffer );
	}

	const uint64_t ColorFormatOperator::InsertRedChannel( const uint64_t target_color, const uint64_t channel_value ) const
	{
		return InsertChannel( target_color, channel_value, m_red_channel_mask, m_red_channel_offset );
	}

	const uint64_t ColorFormatOperator::InsertGreenChannel( const uint64_t target_color, const uint64_t channel_value ) const
	{
		return InsertChannel( target_color, channel_value, m_green_channel_mask, m_green_channel_offset );
	}

	const uint64_t ColorFormatOperator::InsertBlueChannel( const uint64_t target_color, const uint64_t channel_value ) const
	{
		return InsertChannel( target_color, channel_value, m_blue_channel_mask, m_blue_channel_offset );
	}

	const uint64_t ColorFormatOperator::InsertAlphaChannel( const uint64_t target_color, const uint64_t channel_value ) const
	{
		return InsertChannel( target_color, channel_value, m_alpha_channel_mask, m_alpha_channel_offset );
	}

	const uint64_t ColorFormatOperator::InsertWhiteChannel( const uint64_t target_color, const uint64_t channel_value ) const
	{
		return InsertChannel( target_color, channel_value, m_white_channel_mask, m_white_channel_offset );
	}

	const uint64_t ColorFormatOperator::InsertIndexChannel( const uint64_t target_color, const uint64_t channel_value ) const
	{
		return InsertChannel( target_color, channel_value, m_index_mask, m_index_offset );
	}

	const uint64_t ColorFormatOperator::ReplaceRedChannel( const uint64_t target_color, const uint64_t source_color ) const
	{
		return ReplaceChannel( target_color, source_color, m_red_channel_mask );
	}

	const uint64_t ColorFormatOperator::ReplaceGreenChannel( const uint64_t target_color, const uint64_t source_color ) const
	{
		return ReplaceChannel( target_color, source_color, m_green_channel_mask );
	}

	const uint64_t ColorFormatOperator::ReplaceBlueChannel( const uint64_t target_color, const uint64_t source_color ) const
	{
		return ReplaceChannel( target_color, source_color, m_blue_channel_mask );
	}

	const uint64_t ColorFormatOperator::ReplaceAlphaChannel( const uint64_t target_color, const uint64_t source_color ) const
	{
		return ReplaceChannel( target_color, source_color, m_alpha_channel_mask );
	}

	const uint64_t ColorFormatOperator::ReplaceWhiteChannel( const uint64_t target_color, const uint64_t source_color ) const
	{
		return ReplaceChannel( target_color, source_color, m_white_channel_mask );
	}

	const uint64_t ColorFormatOperator::ReplaceIndexChannel( const uint64_t target_color, const uint64_t source_color ) const
	{
		return ReplaceChannel( target_color, source_color, m_index_mask );
	}

	const uint64_t ColorFormatOperator::ExtractRedChannel( const uint64_t color ) const
	{
		return MaskRedChannel( color ) >> m_red_channel_offset;
	}

	const uint64_t ColorFormatOperator::ExtractGreenChannel( const uint64_t color ) const
	{
		return MaskGreenChannel( color ) >> m_green_channel_offset;
	}

	const uint64_t ColorFormatOperator::ExtractBlueChannel( const uint64_t color ) const
	{
		return MaskBlueChannel( color ) >> m_blue_channel_offset;
	}

	const uint64_t ColorFormatOperator::ExtractAlphaChannel( const uint64_t color ) const
	{
		return MaskAlphaChannel( color ) >> m_alpha_channel_offset;
	}

	const uint64_t ColorFormatOperator::MaskRedChannel( const uint64_t color ) const
	{
		return color & m_red_channel_mask;
	}

	const uint64_t ColorFormatOperator::MaskGreenChannel( const uint64_t color ) const
	{
		return color & m_green_channel_mask;
	}

	const uint64_t ColorFormatOperator::MaskBlueChannel( const uint64_t color ) const
	{
		return color & m_blue_channel_mask;
	}

	const uint64_t ColorFormatOperator::MaskAlphaChannel( const uint64_t color ) const
	{
		return color & m_alpha_channel_mask;
	}

	const uint64_t ColorFormatOperator::MaskWhiteChannel( const uint64_t color ) const
	{
		return color & m_white_channel_mask;
	}

	const uint64_t ColorFormatOperator::MaskIndexChannel( const uint64_t color ) const
	{
		return color & m_index_mask;
	}

	void ColorFormatOperator::ConfigureFlags()
	{
		m_has_red_channel	= m_format.red_channel_bits > 0;
		m_has_green_channel	= m_format.green_channel_bits > 0;
		m_has_blue_channel	= m_format.blue_channel_bits > 0;
		m_has_alpha_channel	= m_format.has_alpha;
		m_has_white_channel	= m_format.is_monochrome;
		m_has_index_channel	= m_format.is_index;
	}

	void ColorFormatOperator::CalculateOffsets()
	{
		if( m_has_index_channel && m_has_alpha_channel )
		{
			m_alpha_channel_offset = m_format.index_channel_bits;
			return;
		}

		if( m_has_white_channel && m_has_alpha_channel )
		{
			m_alpha_channel_offset = size_t( m_format.size_bits ) - m_format.alpha_channel_bits;
			return;
		}

		size_t sizes[4] {};
		if( m_has_red_channel )
		{
			sizes[ m_format.red_channel_index ] = m_format.red_channel_bits;
		}

		if( m_has_green_channel )
		{
			sizes[ m_format.green_channel_index ] = m_format.green_channel_bits;
		}

		if( m_has_blue_channel )
		{
			sizes[ m_format.blue_channel_index ] = m_format.blue_channel_bits;
		}

		if( m_has_alpha_channel )
		{
			sizes[ m_format.alpha_channel_index ] = m_format.alpha_channel_bits;
		}

		size_t offsets[4] {};
		size_t offset = 0;
		for( size_t index = 0; index < std::size( offsets ); ++index )
		{
			offsets[ index ] = offset;
			offset += sizes[ index ];
		}

		m_red_channel_offset	= ( m_has_red_channel )? offsets[ m_format.red_channel_index ] : 0;
		m_green_channel_offset	= ( m_has_green_channel )? offsets[ m_format.green_channel_index ] : 0;
		m_blue_channel_offset	= ( m_has_blue_channel )? offsets[ m_format.blue_channel_index ] : 0;
		m_alpha_channel_offset	= ( m_has_alpha_channel )? offsets[ m_format.alpha_channel_index ] : 0;
		m_white_channel_offset	= ( m_has_white_channel )? offsets[0] : 0;
		m_index_offset			= ( m_has_index_channel )? offsets[0] : 0;
	}

	void ColorFormatOperator::CalculateMasks()
	{
		constexpr uint64_t all_bits = ~uint64_t{};

		const size_t white_channel_bits = ( m_has_white_channel )? ( m_format.size_bits - m_format.alpha_channel_bits ) : 0;

		m_red_channel_mask		= ( all_bits << m_red_channel_offset ) & ~( all_bits << ( m_red_channel_offset + m_format.red_channel_bits ) );
		m_green_channel_mask	= ( all_bits << m_green_channel_offset ) & ~( all_bits << ( m_green_channel_offset + m_format.green_channel_bits ) );
		m_blue_channel_mask		= ( all_bits << m_blue_channel_offset ) & ~( all_bits << ( m_blue_channel_offset + m_format.blue_channel_bits ) );
		m_alpha_channel_mask	= ( all_bits << m_alpha_channel_offset ) & ~( all_bits << ( m_alpha_channel_offset + m_format.alpha_channel_bits ) );
		m_white_channel_mask	= ( all_bits << m_white_channel_offset ) & ~( all_bits << ( m_white_channel_offset + white_channel_bits ) );
		m_index_mask			= ( all_bits << m_index_offset ) & ~( all_bits << ( m_index_offset + m_format.index_channel_bits ) );
	}
}
}
}
