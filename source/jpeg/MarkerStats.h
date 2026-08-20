#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Jpeg
{
	/**
	*/
	class MarkerStats final : private Black::NonTransferable
	{
	// Public life-time management.
	public:
		MarkerStats() noexcept	= delete;
		~MarkerStats() noexcept	= default;

		explicit MarkerStats( Black::PlainView<const std::byte> file_memory ) noexcept;

	// Public interface.
	public:
		//
		void LogMarker( const Internal::Marker& marker, Black::NotNull<const std::byte*> memory );


		//
		const size_t GetPosition( const Internal::MarkerCode code ) const;


		//
		const bool IsPositionValid( const Internal::MarkerCode code ) const;

	// Private constants.
	private:
		//
		static constexpr size_t BASE_OFFSET	= Black::GetEnumValue( Internal::MarkerCode::Sof0 );

		//
		static constexpr size_t CODES_COUNT	= size_t( Black::GetEnumValue( Internal::MarkerCode::Com ) ) - BASE_OFFSET + 1;

	// Private interface.
	private:
		//
		static const size_t GetCodeIndex( const Internal::MarkerCode code );

	// Private state.
	private:
		Black::PlainView<const std::byte>	m_file_memory;

		size_t	m_marker_positions[ CODES_COUNT ];
	};
}
}
}
