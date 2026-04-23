#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
namespace Internal
{
	//
	const bool IsBitDepthValid( const BitDepth bit_depth );

	//
	const bool IsColorTypeValid( const ColorType color_type );

	//
	const bool HasValidColorCombination( const BitDepth bit_depth, const ColorType color_type );

	//
	const bool IsCompressionMethodValid( const CompressionMethod compression_method );

	//
	const bool IsFilterMethodValid( const FilterMethod filter_method );
}
}
}
}
