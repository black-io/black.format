#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Tga
{
	/**
		@brief	TGA file builder.

		Currently this type is only stub. No functionality implemented.
	*/
	class TgaFileBilder final
	{
	// Friendship interface.
	public:
		friend inline void swap( TgaFileBilder& left, TgaFileBilder& right )	{ left.Swap( right ); };

	// Public life-time management.
	public:
		inline TgaFileBilder() noexcept								= default;
		inline TgaFileBilder( const TgaFileBilder& other ) noexcept	= default;
		inline TgaFileBilder( TgaFileBilder&& other ) noexcept		= default;

		inline ~TgaFileBilder() noexcept = default;


		inline TgaFileBilder& operator = ( const TgaFileBilder& other ) noexcept		= default;
		inline TgaFileBilder& operator = ( TgaFileBilder&& other ) noexcept				= default;

	// Public interface.
	public:
		// Swap the state of this encoder with other one.
		void Swap( TgaFileBilder& other );

	// Private state.
	private:
	};
}
}
}
