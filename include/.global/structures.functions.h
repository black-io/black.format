#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Global
{
	/**
		@brief	Get the mask of alpha-channel for given image format.

		This function may return `0` in case the given format does not declare alpha-channel.

		@param	format	Given image format.
		@return			The value returned is valid mask for alpha-channel of color described by given image format.
	*/
	const uint64_t GetAlphaMask( const ImageFormat format );
}
}
}
