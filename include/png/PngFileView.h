#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Png
{
	/**
	*/
	class PngFileView final
	{
	// Friendship interface.
	public:
		friend inline void swap( PngFileView& left, PngFileView& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		//
		static const bool IsHeaderValid( const Black::PlainView<const std::byte>& file_memory );

		//
		static const bool IsFooterValid( const Black::PlainView<const std::byte>& file_memory );

		//
		static const bool IsFileValid( const Black::PlainView<const std::byte>& file_memory );

	// Public life-time management.
	public:
		inline PngFileView() noexcept						= default;;
		PngFileView( PngFileView&& other ) noexcept;

		explicit PngFileView( Black::PlainView<const std::byte> file_memory ) noexcept;
		PngFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace ) noexcept;

		inline ~PngFileView() noexcept = default;


		PngFileView& operator = ( PngFileView&& other ) noexcept;
		PngFileView& operator = ( Black::PlainView<const std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		//
		void Reset();

		//
		void Swap( PngFileView& other );


		//
		const PngStructure::Header* const QueryHeader() const;

		//
		const PngStructure::Header& GetHeader() const;

		//
		const Black::PlainView<const std::byte> GetPaletteBuffer() const;

		//
		const Black::PlainView<const std::byte> GetImageBuffer() const;


		//
		const bool HasValidHeader() const;

		//
		const bool HasValidFooter() const;

		//
		const bool IsValidFile() const;

		//
		const bool IsEmpty() const;

		//
		inline const bool IsValid() const	{ return m_is_valid; };

	// Private interface.
	private:
		//
		void InvalidateCache() const;

		//
		void EnsureFileMemoryParsed() const;

		//
		void ParseFileMemory() const;

		//
		void TestFileMemory() const;

	// Private state.
	private:
		Black::PlainView<const std::byte>	m_file_memory;

	// Private non-state.
	private:
		mutable std::vector<Internal::Chunk>		m_cunks;
		mutable const Internal::ImageHeader*		m_header	= nullptr;
		mutable Black::PlainView<const std::byte>	m_palette;
		mutable Black::PlainView<const std::byte>	m_image;

		mutable bool	m_is_valid	= false;
		mutable bool	m_is_parsed	= false;
	};
}
}
}
