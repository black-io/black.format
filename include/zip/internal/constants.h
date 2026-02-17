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
		EndOfCentralDirectory				= '\x50\x4B\x05\x06',	// (4.3.16) EOCD signature.
	};

	/**
		@brief	Enumeration of general purpose bit flags.

		This constants described in section 4.4.4 of .ZIP file format specification.

		4.4.4 general purpose bit flag: (2 bytes)

			Bit 0: If set, indicates that the file is encrypted.

			(For Method 6 - Imploding)
			Bit 1: If the compression method used was type 6,
					Imploding, then this bit, if set, indicates
					an 8K sliding dictionary was used.	If clear,
					then a 4K sliding dictionary was used.

			Bit 2: If the compression method used was type 6,
					Imploding, then this bit, if set, indicates
					3 Shannon-Fano trees were used to encode the
					sliding dictionary output.	If clear, then 2
					Shannon-Fano trees were used.

			(For Methods 8 and 9 - Deflating)
			Bit 2	Bit 1
				0		0 Normal (-en) compression option was used.
				0		1 Maximum (-exx/-ex) compression option was used.
				1		0 Fast (-ef) compression option was used.
				1		1 Super Fast (-es) compression option was used.

			(For Method 14 - LZMA)
			Bit 1: If the compression method used was type 14,
					LZMA, then this bit, if set, indicates
					an end-of-stream (EOS) marker is used to
					mark the end of the compressed data stream.
					If clear, then an EOS marker is not present
					and the compressed data size must be known
					to extract.

			Note:	Bits 1 and 2 are undefined if the compression
					method is any other.

			Bit 3: If this bit is set, the fields crc-32, compressed
					size and uncompressed size are set to zero in the
					local header.	The correct values are put in the
					data descriptor immediately following the compressed
					data.	(Note: PKZIP version 2.04g for DOS only
					recognizes this bit for method 8 compression, newer
					versions of PKZIP recognize this bit for any
					compression method.)

			Bit 4: Reserved for use with method 8, for enhanced
					deflating.

			Bit 5: If this bit is set, this indicates that the file is
					compressed patched data.	(Note: Requires PKZIP
					version 2.70 or greater)

			Bit 6: Strong encryption.	If this bit is set, you MUST
					set the version needed to extract value to at least
					50 and you MUST also set bit 0.	If AES encryption
					is used, the version needed to extract value MUST
					be at least 51. See the section describing the Strong
					Encryption Specification for details.	Refer to the
					section in this document entitled "Incorporating PKWARE
					Proprietary Technology into Your Product" for more
					information.

			Bit 7: Currently unused.

			Bit 8: Currently unused.

			Bit 9: Currently unused.

			Bit 10: Currently unused.

			Bit 11: Language encoding flag (EFS).	If this bit is set,
					the filename and comment fields for this file
					MUST be encoded using UTF-8. (see APPENDIX D)

			Bit 12: Reserved by PKWARE for enhanced compression.

			Bit 13: Set when encrypting the Central Directory to indicate
					selected data values in the Local Header are masked to
					hide their actual values.	See the section describing
					the Strong Encryption Specification for details.	Refer
					to the section in this document entitled "Incorporating
					PKWARE Proprietary Technology into Your Product" for
					more information.

			Bit 14: Reserved by PKWARE for alternate streams.

			Bit 15: Reserved by PKWARE.
	*/
	enum class GeneralPurposeBitFlag : uint16_t
	{
		IsEncrypted					= 0x0001U,	// If set, indicates that the file is encrypted.
		// For compression method 6:
		Use8kSlidingDictionary		= 0x0002U,	// If set, indicates an 8K sliding dictionary was used.  If clear, then a 4K sliding dictionary was used.
		Use3sfAlgorithm				= 0x0004U,	// If set, indicates 3 Shannon-Fano trees were used. If clear, then 2 Shannon-Fano trees were used.
		// For compression method 8/9:
		UseMaxAlgorithmVersion		= 0x0002U,	// Indicates to use maximum compression, or most fast compression. Depending on next bit.
		FavorFastAlgorithm			= 0x0004U,	// If set, indicates the favor to use fast compression. If clear, then favor to best compression.
		// For compression method 14:
		UseEosMarker				= 0x0002U,	// If set, indicates an end-of-stream (EOS) marker is used to mark the end of the compressed data stream.
		// Other bits:
		UseDataDescriptor			= 0x0008U,	// If this bit is set, the fields crc-32, compressed size and uncompressed size are set to zero in the local header.
		// 0x0010U is reserved.
		IsCompressedPatchedData		= 0x0020U,	// If this bit is set, this indicates that the file is compressed patched data.
		UseStrongEncryption			= 0x0040U,	// If this bit is set, you MUST set the version needed to extract value to at least 50 and you MUST also set bit 0.
		// 0x0080U is reserved.
		// 0x0100U is reserved.
		// 0x0200U is reserved.
		// 0x0400U is reserved.
		UseUtfStrings				= 0x0800U,	// If this bit is set, the filename and comment fields for this file MUST be encoded using UTF-8.
		// 0x1000U is reserved.
		IsCentralDirectoryEncrypted	= 0x2000U,	// Set when encrypting the Central Directory to indicate selected data values in the Local Header are masked.
		// 0x4000U is reserved.
		// 0x8000U is reserved.
	};
}
}
}
}
