#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
	/**
	*/
	class TgaImageDecoder final
	{
	// Friendship interface.
	public:
		friend inline void swap( TgaImageDecoder& left, TgaImageDecoder& right )	{ left.Swap( right ); };

	// Public life-time management.
	public:
		TgaImageDecoder() noexcept;
		TgaImageDecoder( const TgaImageDecoder& other ) noexcept;
		TgaImageDecoder( TgaImageDecoder&& other ) noexcept;

		explicit TgaImageDecoder( const TgaStructure::Header& input_header );

		~TgaImageDecoder() noexcept;


		TgaImageDecoder& operator = ( const TgaImageDecoder& other ) noexcept;
		TgaImageDecoder& operator = ( TgaImageDecoder&& other ) noexcept;

		TgaImageDecoder& operator = ( const TgaStructure::Header& input_header );

	// Public interface.
	public:
		//
		void Reset();

		// Swap the state of this decoder with other one.
		void Swap( TgaImageDecoder& other );

		/**
			@brief	Perform the TGA file decoding.

			This function translate the given buffers into internally allocated memory.
		*/
		Black::BooleanStatus Decode( const Black::PlainView<const std::byte>& image_buffer, const Black::PlainView<const std::byte>& palette_buffer );


		//
		TgaImageDecoder& SetOutputSize( const size_t width, const size_t height );

		//
		TgaImageDecoder& SetOutputFormat( const Black::ColorFormat format );


		//
		inline const TgaStructure::Header& GetInputHeader() const				{ return m_input_header; };

		//
		inline Black::PlainView<const std::byte> GetOutputImageBuffer() const &	{ return { m_output_buffer.GetMemory(), m_output_buffer.GetLength() }; };

		//
		inline Black::PlainVector<std::byte>& GetOutputImageBuffer() &&			{ return m_output_buffer; };

		//
		inline const size_t GetOutputWidth() const								{ return m_output_width; };

		//
		inline const size_t GetOutputHeight() const								{ return m_output_height; };

		//
		inline const size_t GetOutputRowSize() const							{ return m_output_row_size; };

		//
		inline const Black::ColorFormat GetOutputFormat() const					{ return m_output_format; };

	// Private state.
	private:
		Internal::Header				m_input_header;
		Black::PlainVector<std::byte>	m_output_buffer;

		size_t	m_output_width		= 0;
		size_t	m_output_height		= 0;
		size_t	m_output_row_size	= 0;

		Black::ColorFormat	m_output_format	= Black::ColorFormats::UNDEFINED;
	};
}
}
}
