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
		@brief	Local File Header description.

		This header described in section 4.3.6 of .ZIP file format specification.

		4.3.7  Local file header:
			local file header signature     4 bytes  (0x04034b50)
			version needed to extract       2 bytes
			general purpose bit flag        2 bytes
			compression method              2 bytes
			last mod file time              2 bytes
			last mod file date              2 bytes
			crc-32                          4 bytes
			compressed size                 4 bytes
			uncompressed size               4 bytes
			file name length                2 bytes
			extra field length              2 bytes

			file name (variable size)
			extra field (variable size)

		30 bytes total.
		This is only header. It does not include the strings of file name and extra field.

		The `SIGNATURE` static field stores the valid value of local file header signature. So it can be used during the parsing of format.
	*/
	#pragma pack( push, 2 )
	struct LocalFileHeader final
	{
		// Should be 0x04034b50. Valid signature of header.
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::LocalFileHeader;


		HeaderSignature			signature				= SIGNATURE;				// [4B] {0x04034b50} local file header signature.
		uint16_t				extractor_version		= 0;						// [2B] (4.4.3) version needed to extract.
		GeneralPurposeBitFlags	general_purpose_bits	{};							// [2B] (4.4.4) general purpose bit flag.
		CompressionMethod		compression_function	= CompressionMethod::Store;	// [2B] (4.4.5) compression method.
		uint16_t				last_modification_time	= 0;						// [2B] (4.4.6) last mod file time.
		uint16_t				last_modification_date	= 0;						// [2B] (4.4.6) last mod file date.
		uint32_t				checksumm				= 0;						// [4B] (4.4.7) crc-32.
		uint32_t				compressed_length		= 0;						// [4B] (4.4.8) compressed size.
		uint32_t				uncompressed_length		= 0;						// [4B] (4.4.9) uncompressed size.
		uint16_t				name_length				= 0;						// [2B] (4.4.10) file name length.
		uint16_t				extra_field_length		= 0;						// [2B] (4.4.11) extra field length.
	};
	#pragma pack( pop )

	static_assert( sizeof( LocalFileHeader ) == 30, "Local File Header does not aligned properly." );

	/**
		@brief	Data descriptor.

		This block described in section 4.3.9 of .ZIP file format specification.

		4.3.9  Data descriptor:
			crc-32                          4 bytes
			compressed size                 4 bytes
			uncompressed size               4 bytes

		4.3.9.3 Although not originally assigned a signature, the value
		0x08074b50 has commonly been adopted as a signature value
		for the data descriptor record.

		16 bytes total.
		This is only header. It does not include the strings of file name and extra field.

		The `SIGNATURE` static field stores the valid value of data descriptor signature. So it can be used during the parsing of format.
	*/
	#pragma pack( push, 4 )
	struct FileDataDescriptor final
	{
		// Should be 0x08074b50. Valid signature of block.
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::DataDescriptor;


		HeaderSignature	signature				= SIGNATURE;	// [4B] {0x08074b50} data descriptor signature.
		uint32_t		checksumm				= 0;			// [4B] (4.4.7) crc-32.
		uint32_t		compressed_length		= 0;			// [4B]	(4.4.8) compressed size.
		uint32_t		uncompressed_length		= 0;			// [4B]	(4.4.9) uncompressed size.
	};
	#pragma pack( pop )

	static_assert( sizeof( FileDataDescriptor ) == 16, "File Data Descriptor does not aligned properly." );

	/**
		@brief	Archive extra data record.

		This block described in section 4.3.9 of .ZIP file format specification.

		4.3.11  Archive extra data record:
			archive extra data signature    4 bytes  (0x08064b50)
			extra field length              4 bytes

			extra field data                (variable size)

		8 bytes total.
		This is only header. It does not include the string of extra field.

		The `SIGNATURE` static field stores the valid value of block signature. So it can be used during the parsing of format.
	*/
	#pragma pack( push, 4 )
	struct ArchiveExtraDataRecord final
	{
		// Should be 0x08064b50. Valid signature of block.
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::ArchiveExtraDataRecord;


		HeaderSignature	signature				= SIGNATURE;	// [4B] {0x08064b50} archive extra data signature.
		uint32_t		extra_field_length		= 0;			// [4B] (4.4.11) extra field length.
	};
	#pragma pack( pop )

	static_assert( sizeof( ArchiveExtraDataRecord ) == 8, "Archive Extra Data Record does not aligned properly." );

	/**
		@brief	Central directory file header.

		This header described in section 4.3.12 of .ZIP file format specification.

		File header:
			central file header signature   4 bytes  (0x02014b50)
			version made by                 2 bytes
			version needed to extract       2 bytes
			general purpose bit flag        2 bytes
			compression method              2 bytes
			last mod file time              2 bytes
			last mod file date              2 bytes
			crc-32                          4 bytes
			compressed size                 4 bytes
			uncompressed size               4 bytes
			file name length                2 bytes
			extra field length              2 bytes
			file comment length             2 bytes
			disk number start               2 bytes
			internal file attributes        2 bytes
			external file attributes        4 bytes
			relative offset of local header 4 bytes

			file name (variable size)
			extra field (variable size)
			file comment (variable size)

		46 bytes total.
		This is only header. It does not include the strings of file name, extra field or file comment.

		The `SIGNATURE` static field stores the valid value of header signature. So it can be used during the parsing of format.
	*/
	#pragma pack( push, 2 )
	struct CentralDirectoryFileHeader final
	{
		// Should be 0x08064b50. Valid signature of header.
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::CentralDirectoryFileHeader;


		HeaderSignature				signature				= SIGNATURE;				// [4B] {0x08064b50} central file header signature.
		uint16_t					compressor_version		= 0;						// [2B] (4.4.2) version made by.
		uint16_t					extractor_version		= 0;						// [2B] (4.4.3) version needed to extract.
		GeneralPurposeBitFlags		general_purpose_bits	{};							// [2B] (4.4.4) general purpose bit flag.
		CompressionMethod			compression_function	= CompressionMethod::Store;	// [2B] (4.4.5) compression method.
		uint16_t					last_modification_time	= 0;						// [2B] (4.4.6) last mod file time.
		uint16_t					last_modification_date	= 0;						// [2B] (4.4.6) last mod file date.
		uint32_t					checksumm				= 0;						// [4B] (4.4.7) crc-32.
		uint32_t					compressed_length		= 0;						// [4B] (4.4.8) compressed size.
		uint32_t					uncompressed_length		= 0;						// [4B] (4.4.9) uncompressed size.
		uint16_t					name_length				= 0;						// [2B] (4.4.10) file name length.
		uint16_t					extra_field_length		= 0;						// [2B] (4.4.11) extra field length.
		uint16_t					comment_length			= 0;						// [2B] (4.4.12) file comment length.
		uint16_t					disk_number_start		= 0;						// [2B] (4.4.13) disk number start.
		uint16_t					internal_attributes		= 0;						// [2B] (4.4.14) internal file attributes.
		uint32_t					external_attributes		= 0;						// [4B] (4.4.15) external file attributes.
		uint32_t					local_header_offset		= 0;						// [4B] (4.4.16) relative offset of local header.
	};
	#pragma pack( pop )

	static_assert( sizeof( CentralDirectoryFileHeader ) == 46, "Central Directory File Header does not aligned properly." );

	/**
		@brief	Central directory digital signature.

		This block described in section 4.3.13 of .ZIP file format specification.

		4.3.13 Digital signature:

			header signature                4 bytes  (0x05054b50)
			size of data                    2 bytes

			signature data (variable size)

		6 bytes total.
		This is only header. It does not include the signature data.

		The `SIGNATURE` static field stores the valid value of header signature. So it can be used during the parsing of format.
	*/
	#pragma pack( push, 2 )
	struct CentralDirectoryDigitalSignatureHeader final
	{
		// Should be 0x05054b50. Valid signature of block.
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::CentralDirectoryDigitalSignature;


		HeaderSignature	signature				= SIGNATURE;	// [4B] {0x05054b50} header signature.
		uint16_t		data_length				= 0;			// [2B] (?) size of data.
	};
	#pragma pack( pop )

	static_assert( sizeof( CentralDirectoryDigitalSignatureHeader ) == 6, "Central Directory Digital Signature does not aligned properly." );

	/**
		@brief	End of central directory record.

		This block described in section 4.3.16 of .ZIP file format specification.

		4.3.16  End of central directory record:
			end of central dir signature    4 bytes  (0x06054b50)
			number of this disk             2 bytes
			number of the disk with the
			start of the central directory  2 bytes
			total number of entries in the
			central directory on this disk  2 bytes
			total number of entries in
			the central directory           2 bytes
			size of the central directory   4 bytes
			offset of start of central
			directory with respect to
			the starting disk number        4 bytes
			.ZIP file comment length        2 bytes

			.ZIP file comment       (variable size)

		20 bytes total.
		This is only header. It does not include the string of .ZIP file comment.

		The `SIGNATURE` static field stores the valid value of block signature. So it can be used during the parsing of format.
	*/
	#pragma pack( push, 2 )
	struct EndOfCentralDirectoryHeader final
	{
		// Should be 0x06054b50. Valid signature of block.
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::EndOfCentralDirectory;


		HeaderSignature	signature					= SIGNATURE;	// [4B] {0x06054b50} end of central dir signature
		uint16_t		current_disk_index			= 0;			// [2B] (4.4.19) number of this disk
		uint16_t		first_disk_index			= 0;			// [2B] (4.4.20) number of the disk with the start of the central directory
		uint16_t		current_disk_entries		= 0;			// [2B] (4.4.21) total number of entries in the central directory on this disk
		uint16_t		entries_total_count			= 0;			// [2B] (4.4.22) total number of entries in the central directory
		uint32_t		central_directory_length	= 0;			// [4B] (4.4.23) size of the central directory
		uint32_t		central_directory_position	= 0;			// [4B] (4.4.24) offset of start of central directory with respect to the starting disk number
		uint16_t		comment_length				= 0;			// [2B] (4.4.25) .ZIP file comment length
	};
	#pragma pack( pop )

	static_assert( sizeof( EndOfCentralDirectoryHeader ) == 22, "Central Directory Digital Signature does not aligned properly." );

	/**
		@brief	Regular entry of local file in ZIP format.

		This type reflects the union of sections 4.3.7 (Local file header) and 4.3.8 (File data) of .ZIP file format specification.

		This type describes the placement of regular file (or empty folder) inside of ZIP format.
		The core header of file is `LocalFileHeader`. All other headers are optional and may be not found while parsing the format.

		Once some additional block or header is found in ZIP format for certain file entry, it will be linked with file entry.
	*/
	struct LocalFileEntry final
	{
		size_t										name_hash;						// Hash of name.
		std::string_view							name;							// Name of file.
		size_t										base_offset;					// File placement base offset. Points to local file header.
		size_t										base_length;					// File placement length. Relative to base offset.
		size_t										payload_offset;					// File payload base offset. Points to begin of file payload.
		size_t										payload_length;					// File payload length. Means the length of file payload.
		GeneralPurposeBitFlags						general_purpose_bits;			// Given bits of general purpose flags from headers of file.
		CompressionMethod							compression_function;			// Given compression function from headers of file.

		std::shared_ptr<LocalFileHeader>			header;							// Header of file.
		std::shared_ptr<FileDataDescriptor>			data_descriptor;				// Data descriptor of file.
		std::shared_ptr<CentralDirectoryFileHeader>	central_directory_header;		// Central directory header of file.

		Black::PlainView<std::byte>					payload;						// Payload of file.
		Black::PlainView<std::byte>					extra_field;					// Extra field, if used.
		Black::PlainView<std::byte>					central_directory_extra_field;	// Extra field from central directory, if used.
		std::string_view							comment;						// Comment for file in central directory.
	};

	/**
		@brief	Extra data of ZIP format.

		This type reflects the section 4.3.11 (Archive extra data record) of .ZIP file format specification.

		This type describes the regular extra data for ZIP file decryption.
	*/
	struct ArchiveDecryptionExtraData final
	{
		std::shared_ptr<ArchiveExtraDataRecord>		header;		// Header of extra data.
		Black::PlainView<std::byte>					payload;	// Payload of extra data.
	};

	/**
		@brief	Central directory digital signature.

		This type reflects the section 4.3.13 (Digital signature) of .ZIP file format specification.

		Object of this type will be filled once the ZIP file contains encrypted central directory.
	*/
	struct CentralDirectoryDigitalSignature final
	{
		std::shared_ptr<CentralDirectoryDigitalSignatureHeader>	header;		// Header of digital signature.
		Black::PlainView<std::byte>								payload;	// Signature data.
	};

	/**
		@brief	Central directory footer.

		This type reflects the section 4.3.16 (End of central directory record) of .ZIP file format specification.

		This type describes the ending of ZIP format. It consists of stored trailing headers and blocks.
	*/
	struct EndOfCentralDirectoryRecord final
	{
		std::shared_ptr<EndOfCentralDirectoryHeader>		description;		// EOCD record header.

		std::string_view									comment;			// Main comment of ZIP file.
	};
}
}
}
}
