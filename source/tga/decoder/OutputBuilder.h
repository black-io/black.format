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
	*/
	class BasicOutputBuilder : private Black::NonTransferable
	{
	// Public interface.
	public:
		//
		const Black::BooleanStatus ProduceElement( const uint32_t color );

	// Heirs virtual interface.
	protected:
		//
		virtual const Black::BooleanStatus PerformElementProduction( const uint32_t color ) = 0;
	};
}
}
}
}
