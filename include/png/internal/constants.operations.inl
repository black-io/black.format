#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
	inline const ChunkTypeFlag operator & ( const ChunkTypeCode type_code, const ChunkTypeFlag type_flag )
	{
		return ChunkTypeFlag{ Black::GetEnumValue( type_code ) & Black::GetEnumValue( type_flag ) };
	}
}
}
}
}
