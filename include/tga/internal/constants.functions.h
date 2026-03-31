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
}
}
}
}
