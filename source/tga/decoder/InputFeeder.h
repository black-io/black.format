#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
namespace Decoder
{
	/**
		@brief	Input feeder for image decoding pipeline.

		Input feeder is the first step in TGA image decoding pipeline. It converts the memory of TGA image into memory of discrete image elements.

		This feeder works on memory of TGA file, where the image located.
		The feeder can operate with plain memory of image elements, or it can unpack the RLE blocks.
		The thing of this feeder hides in structure of RLE blocks in TGA file format. To read the uncompressed memory, the feeder is instructed to work
		like it reads block of uncompressed image elements with the length of all image.

		Input feeder can be prepared using factory functions.
	*/
	class InputFeeder final
	{
	// Friendship interface.
	public:
		friend inline void swap( InputFeeder& left, InputFeeder& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		/**
			@brief	Factory function to initialize the feeder for straight unpacked memory.

			@param	input_buffer	view of TGA image memory to be used by feeder as input for decoding pipeline.
			@param	input_bitrate	Image bit-rate from TGA header.
			@result					The value returned is ready to work input feeder.
		*/
		static InputFeeder SetupStraightFeeder( Black::PlainView<const std::byte> input_buffer, const Internal::Bitrate input_bitrate );

		/**
			@brief	Factory function to initialize the feeder for memory packed using RLE algorithm.

			@param	input_buffer	view of TGA image memory to be used by feeder as input for decoding pipeline.
			@param	input_bitrate	Image bit-rate from TGA header.
			@result					The value returned is ready to work input feeder.
		*/
		static InputFeeder SetupRleFeeder( Black::PlainView<const std::byte> input_buffer, const Internal::Bitrate input_bitrate );

	// Public life-time management.
	public:
		inline InputFeeder() noexcept							= default;
		inline InputFeeder( const InputFeeder& other ) noexcept	= default;
		InputFeeder( InputFeeder&& other ) noexcept;

		inline ~InputFeeder() noexcept = default;

		inline InputFeeder& operator = ( const InputFeeder& other ) noexcept = default;
		InputFeeder& operator = ( InputFeeder&& other ) noexcept;

	// Public interface.
	public:
		// Swap the state of this feeder with other.
		void Swap( InputFeeder& other );

		// Rewind the feeder. As result, internal position of feeder will be reset to beginning of image memory.
		void Rewind();


		// Instruct this feeder to move at next image element.
		const Black::BooleanStatus StepForward();

		// Peek currently observed image element. May return `nullptr` in case of no element can be observed in input buffer.
		const std::byte* const PeekColorBuffer() const;


		// Get the size (in bytes) of single element of image.
		inline const size_t GetElementSize() const			{ return m_input_element_size; };

		// Get the bit-rate of image.
		inline const Internal::Bitrate GetBitrate() const	{ return m_input_bitrage; };


		// Whether more of image element still can be observed.
		inline const bool HasElements() const				{ return m_input_buffer.IsInside( m_block_payload ); };

	// Private interface.
	private:
		// Parse next block of RLE-compressed image.
		void ParseBlockHeader();

	// Private state.
	private:
		Black::PlainView<const std::byte> m_input_buffer; // Buffer of image input. Points to TGA file, where the image data stored.

		const std::byte*	m_block_payload			= nullptr;						// Current head of input content.
		size_t				m_block_rest_size		= 0;							// Rest size of currently observed RLE-block.
		size_t				m_input_element_size	= 0;							// Size of single element in input memory.
		Internal::Bitrate	m_input_bitrage			= Internal::Bitrate::Undefined;	// Bit-rate of elements in input memory.

		union
		{
			uint8_t			m_flags_buffer			= 0;	// Buffer for boolean flags.

			struct
			{
				bool		m_has_compressed_input	: 1;	// Whether the input buffer is compressed with RLE algorithm.
				bool		m_is_block_compressed	: 1;	// Whether currently observed RLE-block is compressed.
			};
		};
	};
}
}
}
}
