#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
namespace Internal
{
	/**
		@brief	General purpose bit flags.

		Functionality of this type described in section 4.4.4 of .ZIP file format specification.

		This type implements the useful interface to operate with bit flags in ZIP headers.
		Instead of bit flag description, implemented in `GeneralPurposeBitFlag`, this type implements only handy usage interface.
	*/
	using GeneralPurposeBitFlags = Black::EnumFlags<GeneralPurposeBitFlag>;
}
}
}
}
