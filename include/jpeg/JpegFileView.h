#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	class JpegFileView final
	{
	// Public life-time management.
	public:
		inline JpegFileView() noexcept						= default;
		inline JpegFileView( const JpegFileView& other )	= default;
		JpegFileView( JpegFileView&& other ) noexcept;

		explicit JpegFileView( Black::PlainView<const std::byte> file_memory ) noexcept;
		JpegFileView( Black::PlainView<const std::byte> file_memory, const Black::ConstructInplace ) noexcept;

		inline ~JpegFileView() noexcept = default;


		inline JpegFileView& operator = ( const JpegFileView& other )		= default;
		inline JpegFileView& operator = ( JpegFileView&& other ) noexcept	{ return Black::CopyAndSwap( *this, std::move( other ) ); };

		JpegFileView& operator = ( Black::PlainView<const std::byte> file_memory ) noexcept;
	};
}
}
}
