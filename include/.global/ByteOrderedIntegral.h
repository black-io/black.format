#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
	*/
	template< typename TValue, const Black::PlatformEndianness VALUE_ENDIANNESS >
	class ByteOrderedIntegral final
	{
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
		//
		inline void Swap( ByteOrderedIntegral& other );


		//
		inline void StoreAsIs( TValue value );


		//
		inline TValue GetValue() const;


		inline operator TValue () const;

	// Private state.
	private:
		TValue	m_value;
	};
}
}
}
