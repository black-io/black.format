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
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::ByteOrderedIntegral( TValue value ) noexcept
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::ByteOrderedIntegral( TValue value, const Black::StoreAsIs ) noexcept
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
	inline void ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::StoreAsIs( TValue value )
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline TValue ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::GetValue() const
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS>::operator TValue () const
	{
	}
}
}
}
