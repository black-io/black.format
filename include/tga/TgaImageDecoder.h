#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
	/**
		@brief	TGA image decoder.

		In TGA file format the image can be stored using various storage settings. The image under format should be decoded in most of settings combinations.
		Only couple of settings combinations allows to display image directly from file.
		`TgaImageDecoder` may be used in any case the image should be converted before displaying.

		Right before the image decoding, `TgaFileView` may be used to test the file and get required data for decoding.
		After the decoder is initialized using valid TGA file header, it can be configured for decoding process.
		One can set desired format and resolution of decoded image. `Decode` method should be used to perform the decoding of image.
		After the decoding finished, result of `Decode` method may be used to check the decoding was succeeded.
		The decoded image may be accessed via `GetOutputImageBuffer` method. Memory of decoded image may be owned after the moving the decoder.

		Decoder may be swapped or initialized by default. The header may be assigned after the initialization.
		But the decoding process may not be started before the valid header is assigned to decoder.
	*/
	class TgaImageDecoder final
	{
	// Friendship interface.
	public:
		friend inline void swap( TgaImageDecoder& left, TgaImageDecoder& right )	{ left.Swap( right ); };

	// Public life-time management.
	public:
		inline TgaImageDecoder() noexcept								= default;
		inline TgaImageDecoder( const TgaImageDecoder& other ) noexcept	= default;
		inline TgaImageDecoder( TgaImageDecoder&& other ) noexcept		= default;

		explicit TgaImageDecoder( const TgaStructure::Header& input_header );

		inline ~TgaImageDecoder() noexcept = default;


		inline TgaImageDecoder& operator = ( const TgaImageDecoder& other ) noexcept	= default;
		inline TgaImageDecoder& operator = ( TgaImageDecoder&& other ) noexcept			{ return Black::CopyAndSwap( *this, std::move( other ) ); };

		inline TgaImageDecoder& operator = ( const TgaStructure::Header& input_header )	{ return Black::CopyAndSwap( *this, input_header ); };

	// Public interface.
	public:
		// Clear the output buffer of decoder.
		void ClearOutputBuffer();

		// Swap the state of this decoder with other one.
		void Swap( TgaImageDecoder& other );

		/**
			@brief	Perform the TGA image decoding.
			This function translate the given buffers into internally allocated memory.
			Given buffer should correlate with previously set TGA file header.

			Decoded image will be stored in memory, that represents the image with given output resolution.

			@param	image_buffer	Buffer with image data to be decoded.
			@param	palette_buffer	Buffer with optional palette to be used while decoding the image.
			@return					The value returned means was the decoding successful or not.
		*/
		Black::BooleanStatus Decode( const Black::PlainView<const std::byte>& image_buffer, const Black::PlainView<const std::byte>& palette_buffer );


		/**
			@brief	Set the resolution of decoded image.

			The output resolution should be not less than original resolution of original image.
			In other case original resolution will be used on decoding.
			Decoder does not process the part of image.

			@param	width	X-length of image in pixels.
			@param	height	Y-length of image in pixels.
			@return			The value returned is this decoder. It may be used for method chaining.
		*/
		TgaImageDecoder& SetOutputResolution( const size_t width, const size_t height );

		/**
			@brief	Set the format of decoded image.

			Output format may differ the one of original image. But the difference should not be dramatic.
			In other case the decoding process fails.

			@param	format	Desired format of output image.
			@return			The value returned is this decoder. It may be used for method chaining.
		*/
		TgaImageDecoder& SetOutputFormat( const Black::ColorFormat format );


		// Get the header the decoder was initialized with.
		inline const TgaStructure::Header& GetInputHeader() const				{ return m_input_header; };

		// Get the view of output buffer.
		inline Black::PlainView<const std::byte> GetOutputImageBuffer() const &	{ return { m_output_buffer.GetMemory(), m_output_buffer.GetLength() }; };

		// Get the moved-out output buffer. It can be owned in this way.
		inline Black::PlainVector<std::byte> GetOutputImageBuffer() &&			{ return std::move( m_output_buffer ); };

		// Get the width of output image.
		inline const size_t GetOutputWidth() const								{ return m_output_width; };

		// Get the height of output image.
		inline const size_t GetOutputHeight() const								{ return m_output_height; };

		// Get the size of output images row, in bytes.
		inline const size_t GetOutputRowSize() const							{ return m_output_row_size; };

		// Get the format of output image.
		inline const Black::ColorFormat GetOutputFormat() const					{ return m_output_format; };

	// Private state.
	private:
		Internal::Header				m_input_header	{};	// Header of input TGA image.
		Black::PlainVector<std::byte>	m_output_buffer;	// buffer to store decoded image.

		size_t	m_output_width		= 0;	// Width of decoded image. X-length.
		size_t	m_output_height		= 0;	// Height of decoded image. Y-length.
		size_t	m_output_row_size	= 0;	// In bytes. Size of single row in decoded image.

		Black::ColorFormat	m_output_format	= Black::ColorFormats::UNDEFINED;	// Format of decoded image.
	};
}
}
}
