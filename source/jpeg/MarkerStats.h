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
	// Private constants.
	private:
		//
		static constexpr size_t BASE_OFFSET	= Black::GetEnumValue( Internal::MarkerCode::Sof0 );

		//
		static constexpr size_t CODES_COUNT	= size_t( Black::GetEnumValue( Internal::MarkerCode::Com ) ) - BASE_OFFSET + 1;

	// Private state.
	private:
		size_t	m_marker_positions[ CODES_COUNT ];
	};
}
}
}
