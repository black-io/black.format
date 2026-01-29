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
}
}
}
}
