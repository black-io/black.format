#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
		@brief	Platform-independent storage for Little-Endian integral value or enumeration.

		This value behaves like regular. It consumes values in platform-specific endianness. It can be implicitly converted to underlying value
		of platform-specific endianness. But internally it stored always in Little-Endian byte order.

		Such behavior is crucial for file- or network formats, where the values should be stored in particular endianness regardless
		to platform-specific endianness.

		This type safely consumes enumerations as well. It converts the order of bytes in enumerations, so for user of this type there will be no problem
		to use enumeration.

		@tparam	TValue	Type of underlying value. May be integral or enumeration.
	*/
	template< typename TValue >
	using LittleEndianIntegral = ByteOrderedIntegral<TValue, Black::PlatformEndianness::LittleEndian>;

	/**
	*/
	template< typename TValue >
	using BigEndianIntegral = ByteOrderedIntegral<TValue, Black::PlatformEndianness::BigEndian>;
}
}
}
