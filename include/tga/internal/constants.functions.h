#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Internal
{
	/**
		@brief	Whether the given content type is valid.

		@param	content_type	Given content type to be checked.
		@return					`true` in case the given content type is valid.
	*/
	const bool IsContentTypeValid( const ContentType content_type );

	/**
		@brief	Whether the given bit-rate is valid.

		@param	bitrate	Given bit-rate to be checked.
		@return			`true` in case the given bit-rate is valid.
	*/
	const bool IsBitrateValid( const Bitrate bitrate );

	/**
		@brief	Whether the given content type assumes compression.

		@param	content_type	Given content type to be checked.
		@return					`true` in case the given type represents compressed content.
	*/
	const bool IsContentCompressed( const ContentType content_type );

	/**
		@brief	Perform the compression method classification, declared by given content type.

		@param	content_type	Given content type to classify.
		@return					The value returned is compression method, declared by given content type.
	*/
	const ContentCompression ClassifyContentCompression( const ContentType content_type );

	/**
		@brief	Get the type of content under any compression.

		TGA specification declares only 3 such content types: Paletted image, True color or grayscale image.

		@param	content_type	Given content type to classify.
		@return					The value returned is content type, that will be given after decompression or before compression.
	*/
	const ContentType GetContentTypeBehindCompression( const ContentType content_type );

	/**
		@brief	Translate the given bit-rate to size (in bytes) of element.

		@param	bitrate	Given bit-rate to be translated.
		@return			The value returned is size of element declared with given bit-rate.
		@retval	0		Zero will be returned for any unknown bit-rate.
	*/
	const size_t GetElementSize( const Bitrate bitrate );

	/**
		@brief	Select proper image format according to given image settings.

		@param	content_type		Declared type of image content.
		@param	bitrate				Declared bit-rate of image elements.
		@param	alpha_bits_count	Declared length of alpha-channel in image.
		@return						The value returned is `ImageFormat` that can describe elements of given image.
	*/
	const Black::ImageFormat SelectImageFormat( const ContentType content_type, const Bitrate bitrate, const size_t alpha_bits_count );
}
}
}
}
