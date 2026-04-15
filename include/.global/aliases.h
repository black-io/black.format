#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
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
