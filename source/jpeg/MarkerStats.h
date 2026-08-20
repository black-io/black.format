#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	/**
	*/
	class MarkerStats final
	{
	// Public interface.
	public:
		//
		void LogMarker( const Internal::Marker& marker, Black::NotNull<const std::byte*> memory );

	// Private constants.
	private:
		//
		static constexpr size_t BASE_OFFSET	= Black::GetEnumValue( Internal::MarkerCode::Sof0 );

		//
		static constexpr size_t CODES_COUNT	= size_t( Black::GetEnumValue( Internal::MarkerCode::Com ) ) - BASE_OFFSET + 1;

	// Private state.
	private:
		Black::PlainView<const std::byte>	m_file_memory;

		size_t	m_marker_positions[ CODES_COUNT ];
	};
}
}
}
