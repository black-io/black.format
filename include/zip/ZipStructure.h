#pragma once

namespace Black
{
inline namespace Format
{
inline namespace Zip
{
	/**
		@brief	Representation of ZIP-file internal structure.
		ZIP-file internal types, collected in single structure as ZIP-file documentation describes.
		In any case any of internal ZIP-file types needed by consumer's code, it may be accessed via `ZipStructure`.
	*/
	struct ZipStructure final
	{
		// Enumeration of all ZIP-file header signatures.
		using Signatures = Internal::HeaderSignature;

		// Enumeration of general purpose bit flags as described in section 4.4.4 (general purpose bit flag).
		using GeneralPurposeBit = Internal::GeneralPurposeBitFlag;

		// Aggregation of general purpose bit flags to be used in file headers.
		using GeneralPurposeBits = Internal::GeneralPurposeBitFlags;

		// Enumeration of documented compression methods as described in section 4.4.5 (compression method).
		using CompressionMethod = Internal::CompressionMethod;

		// ZIP-file internal header as described in section 4.3.7 (Local file header).
		using FileHeader = Internal::LocalFileHeader;

		// ZIP-file internal header as described in section 4.3.9 (Data descriptor).
		using DataDescriptor = Internal::FileDataDescriptor;

		// ZIP-file internal header as described in section 4.3.11 (Archive extra data record).
		using ExtraDataRecord = Internal::ArchiveExtraDataRecord;

		// Collection of central directory records as described in section 4.3.12 (Central directory structure).
		struct CentralDirectory final
		{
			// ZIP-file internal header as described in section 4.3.12 (File header).
			using FileHeader = Internal::CentralDirectoryFileHeader;

			// ZIP-file internal header as described in section 4.3.13 (Digital signature).
			using DigitalSignature = Internal::CentralDirectoryDigitalSignatureHeader;

			// ZIP-file internal header as described in section 4.3.16 (End of central directory record).
			using EndHeader = Internal::EndOfCentralDirectoryHeader;
		};
	};
}
}
}
