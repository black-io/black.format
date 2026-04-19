#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
		@brief	Byte-ordered integral value template.

		This template represents the type of value object, which store the underlying value in potentially different endianness than target platform use.
		In particular case, the endianness of stored value may be the same as one of target platform.
		But the major thing here is to fix the endianness out of target platform bounding.

		The role of byte-ordered integral value is crucial in networking of file formats.
		Underlying type should be trivial, integral and may not be pointer.

		@tparam	TValue				Type of stored value.
		@tparam	VALUE_ENDIANNESS	Fixed endianness of stored value.
	*/
	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS, typename = void >
	class ByteOrderedIntegral;

	/**
	*/
	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	class ByteOrderedIntegral<TValue, VALUE_ENDIANNESS, std::enable_if_t<std::is_integral_v<TValue>>> final
	{
		static_assert( !std::is_const_v<TValue>, "Type of stored value may not be constant." );
		static_assert( sizeof( TValue ) > 1, "Type of stored value should be at last of word size." );

	// Friendship interface.
	public:
		friend inline void swap( ByteOrderedIntegral& left, ByteOrderedIntegral& right )	{ left.Swap( right ); };

	// Public life-time management.
	public:
		inline ByteOrderedIntegral() noexcept;
		inline ByteOrderedIntegral( const ByteOrderedIntegral& other ) noexcept;
		inline ByteOrderedIntegral( ByteOrderedIntegral&& other ) noexcept;

		inline ByteOrderedIntegral( TValue value ) noexcept;
		inline ByteOrderedIntegral( TValue value, const Black::StoreAsIs ) noexcept;

		inline ~ByteOrderedIntegral() noexcept;


		inline ByteOrderedIntegral& operator = ( const ByteOrderedIntegral& other ) noexcept;
		inline ByteOrderedIntegral& operator = ( ByteOrderedIntegral&& other ) noexcept;

		inline ByteOrderedIntegral& operator = ( TValue value ) noexcept;

	// Public interface.
	public:
		// Swap the state of this value with other.
		inline void Swap( ByteOrderedIntegral& other );


		// Store the given value as is. This method ignores any difference in endianness.
		inline void SetValueAsIs( TValue value );


		// Get the value converted in endianness of target platform.
		inline TValue GetValue() const;

		// Get the value as is. This method ignores any difference in endianness.
		inline TValue GetValueAsIs() const;


		inline operator TValue () const;

	// Private state.
	private:
		TValue m_value; // Stored value.
	};
}
}
}
