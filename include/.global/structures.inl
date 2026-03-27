#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	inline ImageFormat::operator const uint64_t () const
	{
		static_assert( sizeof( ImageFormat ) == sizeof( uint64_t ), "Size of image format should be same as `uint64_t`." );

		uint64_t result;
		std::memcpy( &result, this, sizeof( uint64_t ) );
		return result;
	}
}
}
}
