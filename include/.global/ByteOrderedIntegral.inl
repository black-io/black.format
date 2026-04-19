#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::ByteOrderedIntegral() noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::ByteOrderedIntegral(
		const ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>& other
	) noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::ByteOrderedIntegral(
		ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>&& other
	) noexcept
		: m_value{ std::exchange( other.m_value, 0 ) }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::ByteOrderedIntegral( TValue value ) noexcept
		: m_value{ Black::GetTransformedEndianness<VALUE_ENDIANNESS>( value ) }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::ByteOrderedIntegral(
		TValue value,
		const Black::StoreAsIs
	) noexcept
		: m_value{ value }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::~ByteOrderedIntegral() noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>&
	ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::operator=(
		const ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>& other
	) noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>&
	ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::operator=(
		ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>&& other
	) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( other ) );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>&
	ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::operator=( TValue value ) noexcept
	{
		return Black::CopyAndSwap( *this, value );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline void ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::Swap(
		ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>& other
	)
	{
		Black::Swap( m_value, other.m_value );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline void ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::SetValueAsIs( TValue value )
	{
		m_value = value;
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline TValue ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::GetValue() const
	{
		return Black::GetTransformedEndianness<Black::BUILD_ENDIANNESS, VALUE_ENDIANNESS>( m_value );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline TValue ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::GetValueAsIs() const
	{
		return m_value;
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>>::operator TValue () const
	{
		return GetValue();
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::ByteOrderedIntegral() noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::ByteOrderedIntegral(
		const ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>& other
	) noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::ByteOrderedIntegral(
		ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>&& other
	) noexcept
		: m_value{ std::exchange( other.m_value, 0 ) }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::ByteOrderedIntegral( TValue value ) noexcept
		: m_value{ Black::GetTransformedEndianness<VALUE_ENDIANNESS>( Black::GetEnumValue( value ) ) }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::ByteOrderedIntegral(
		TValue value,
		const Black::StoreAsIs
	) noexcept
		: m_value{ Black::GetEnumValue( value ) }
	{
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::~ByteOrderedIntegral() noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>&
	ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::operator=(
		const ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>& other
	) noexcept = default;

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>&
	ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::operator=(
		ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>&& other
	) noexcept
	{
		return Black::CopyAndSwap( *this, std::move( other ) );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>&
	ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::operator=( TValue value ) noexcept
	{
		return Black::CopyAndSwap( *this, value );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline void ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::Swap(
		ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>& other
	)
	{
		Black::Swap( m_value, other.m_value );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline void ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::SetValueAsIs( TValue value )
	{
		m_value = Black::GetEnumValue( value );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline TValue ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::GetValue() const
	{
		return TValue( Black::GetTransformedEndianness<Black::BUILD_ENDIANNESS, VALUE_ENDIANNESS>( m_value ) );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline TValue ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::GetValueAsIs() const
	{
		return TValue( m_value );
	}

	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	inline ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_enum_v<TValue>>>::operator TValue () const
	{
		return GetValue();
	}
}
}
}
