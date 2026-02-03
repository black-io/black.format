#pragma once


namespace Black
{
inline namespace Format
{
inline namespace Zip
{
namespace Internal
{
	/**
		@brief	Enumeration of header signatures, described in ZIP File Format specification.

		All constants implemented using PKZIP specification of ZIP file format.
	*/
	enum class HeaderSignature : uint32_t
	{
		LocalFileHeader						= '\x04\x03\x4B\x50',	// (4.3.7) Local file header signature.
		DataDescriptor						= '\x08\x07\x4B\x50',	// (4.3.9) File data descriptor signature.
		ArchiveExtraDataRecord				= '\x08\x06\x4B\x50',	// (4.3.11) Archive extra-data record signature.
		CentralDirectoryFileHeader			= '\x02\x01\x4B\x50',	// (4.3.12) File header in central directory.
		CentralDirectoryDigitalSignature	= '\x05\x05\x4B\x50',	// (4.3.13) Digital signature of central directory.
		Zip64EndOfCentralDirectory			= '\x06\x06\x4B\x50',	// (4.3.14) EOCD signature for Zip64.
		Zip64EndOfCentralDirectoryLocator	= '\x07\x06\x4B\x50',	// (4.3.15) EOCD locator signature for Zip64.
		EndOfCentralDirectory				= '\x06\x05\x4B\x50',	// (4.3.16) EOCD signature.
	};

	/**
	*/
	enum class GeneralPurposeBitFlag : uint16_t
	{
		IsEncrypted				= 0x0001U,
		// For compression method 6:
		Use8kSlidingDictionary	= 0x0002U,
		Use3sfAlgorithm			= 0x0004U,
		// For compression method 8/9:
		UseMaxAlgorithmVersion	= 0x0002U,
		FavorFastAlgorithm		= 0x0004U,
		// For compression method 14:
		UseEosMarker			= 0x0002U,
		// Other bits:
		UseDataDescriptor		= 0x0008U,
		// 0x0010U is reserved.
		IsCompressedPatchedData	= 0x0020U,
		UseStrongEncryption		= 0x0040U,
		// 0x0080U is reserved.
		// 0x0100U is reserved.
		// 0x0200U is reserved.
		// 0x0400U is reserved.
		UseUtfStrings			= 0x0800U,
		// 0x1000U is reserved.
		IsCentralDirectoryEncrypted	= 0x2000U,
		// 0x4000U is reserved.
		// 0x8000U is reserved.
	};
}
}
}
}
