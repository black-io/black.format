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
		LocalFileHeader						= '\x50\x4B\x03\x04',	// (4.3.7) Local file header signature.
		DataDescriptor						= '\x50\x4B\x07\x08',	// (4.3.9) File data descriptor signature.
		ArchiveExtraDataRecord				= '\x50\x4B\x06\x08',	// (4.3.11) Archive extra-data record signature.
		CentralDirectoryFileHeader			= '\x50\x4B\x01\x02',	// (4.3.12) File header in central directory.
		CentralDirectoryDigitalSignature	= '\x50\x4B\x05\x05',	// (4.3.13) Digital signature of central directory.
		Zip64EndOfCentralDirectory			= '\x50\x4B\x06\x06',	// (4.3.14) EOCD signature for Zip64.
		Zip64EndOfCentralDirectoryLocator	= '\x50\x4B\x06\x07',	// (4.3.15) EOCD locator signature for Zip64.
		EndOfCentralDirectory				= '\x50\x4B\x05\x06',	// (4.3.16) EOCD signature.
	};
}
}
}
}
