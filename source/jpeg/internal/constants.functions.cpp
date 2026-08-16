#include <black/format/jpeg.h>

#include <black/core/algorithms.h>


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
namespace Internal
{
namespace
{
	// Logging channel.
	constexpr const char* LOG_CHANNEL = "Black/JPEG/Functions";
}


namespace
{
	const bool IsMarkerCodeInvalid( const MarkerCode candidate )
	{
		return ( candidate == MarkerCode( 0x00U ) ) || ( candidate == MarkerCode( 0xFFU ) );
	}

	const bool IsMarkerCodePrivate( const MarkerCode candidate )
	{
		return candidate == MarkerCode( 0x01U );
	}

	const bool IsMarkerCodeReserved( const MarkerCode candidate )
	{
		return ( candidate >= MarkerCode( 0x02U ) ) && ( candidate <= MarkerCode( 0xBFU ) );
	}
}


	const bool IsMarkerCodeValid( const MarkerCode candidate )
	{
		CRET( IsMarkerCodeInvalid( candidate ), false );
		CRET( IsMarkerCodePrivate( candidate ), false );
		CRET( IsMarkerCodeReserved( candidate ), false );

		return true;
	}

	const bool IsDesityUnitValid( const DensityUnit candidate )
	{
		static constexpr DensityUnit allowed_units[] { DensityUnit::None, DensityUnit::Inch, DensityUnit::Centimeter };

		return Black::HasItem( allowed_units, candidate );
	}

	const bool IsTiffEndiannessValid( const TiffEndianness candidate )
	{
		static constexpr TiffEndianness allowed_endianness[] { TiffEndianness::LittleEndian, TiffEndianness::BigEndian };

		return Black::HasItem( allowed_endianness, candidate );
	}

	const Black::PlatformEndianness TranslateTiffEndianness( const TiffEndianness endianness )
	{
	}
}
}
}
}
