#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
	inline const ChunkPropertyBit operator & ( const ChunkTypeCode type_code, const ChunkPropertyBit type_flag )
	{
		return ChunkPropertyBit( Black::GetEnumValue( type_code ) & Black::GetEnumValue( type_flag ) );
	}

	inline const ColorTypeFlag operator & ( const ColorType color_type, const ColorTypeFlag type_flag )
	{
		return ColorTypeFlag( Black::GetEnumValue( color_type ) & Black::GetEnumValue( type_flag ) );
	}
}
}
}
}
