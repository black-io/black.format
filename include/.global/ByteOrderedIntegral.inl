#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::ByteOrderedIntegral() noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::ByteOrderedIntegral( const ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>& other ) noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::ByteOrderedIntegral( ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>&& other ) noexcept
		: m_value{ std::exchange( other.m_value, 0 ) }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::ByteOrderedIntegral( TValue value ) noexcept
		: m_value{ Black::GetTransformedEndianness<VALUE_ENDIANNESS>( value ) }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::ByteOrderedIntegral( TValue value, const Black::StoreAsIs ) noexcept
		: m_value{ value }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::~ByteOrderedIntegral() noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>& ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::operator=(
		const ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>& other
	) noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>& ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::operator=(
		ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>&& other
	) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( other ) );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>& ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::operator=( TValue value ) noexcept
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline void ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::Swap( ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>& other )
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline void ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::SetValueAsIs( TValue value )
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline TValue ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::GetValue() const
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline TValue ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::GetValueAsIs() const
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::operator TValue () const
	{
	}
}
}
}
