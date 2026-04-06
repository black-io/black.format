#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	inline ColorFormat::operator const uint64_t () const
	{
		static_assert( sizeof( ColorFormat ) == sizeof( uint64_t ), "Size of color format should be same as `uint64_t`." );

		uint64_t result;
		std::memcpy( &result, this, sizeof( uint64_t ) );
		return result;
	}
}
}
}
