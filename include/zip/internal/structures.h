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
	*/
	#pragma pack( push, 2 )
	struct LocalFileHeader final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::LocalFileHeader;


		HeaderSignature	signature				= SIGNATURE;
		uint16_t		extractor_version		= 0;
		uint16_t		general_purpose_bits	= 0;
		uint16_t		compression_function	= 0;
		uint16_t		last_modification_time	= 0;
		uint16_t		last_modification_date	= 0;
		uint32_t		checksumm				= 0;
		uint32_t		compressed_length		= 0;
		uint32_t		uncompressed_length		= 0;
		uint16_t		name_length				= 0;
		uint16_t		extra_field_length		= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( LocalFileHeader ) == 30, "Local File Header does not aligned properly." );

	/**
	*/
	#pragma pack( push, 4 )
	struct FileDataDescriptor final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::DataDescriptor;


		HeaderSignature	signature				= SIGNATURE;
		uint32_t		checksumm				= 0;
		uint32_t		compressed_length		= 0;
		uint32_t		uncompressed_length		= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( FileDataDescriptor ) == 16, "File Data Descriptor does not aligned properly." );

	/**
	*/
	#pragma pack( push, 4 )
	struct ArchiveExtraDataRecord final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::ArchiveExtraDataRecord;


		HeaderSignature	signature				= SIGNATURE;
		uint32_t		extra_field_length		= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( ArchiveExtraDataRecord ) == 8, "Archive Extra Data Record does not aligned properly." );

	/**
	*/
	#pragma pack( push, 2 )
	struct CentralDirectoryFileHeader final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::CentralDirectoryFileHeader;


		HeaderSignature	signature				= SIGNATURE;
		uint16_t		compressor_version		= 0;
		uint16_t		extractor_version		= 0;
		uint16_t		general_purpose_bits	= 0;
		uint16_t		compression_function	= 0;
		uint16_t		last_modification_time	= 0;
		uint16_t		last_modification_date	= 0;
		uint32_t		checksumm				= 0;
		uint32_t		compressed_length		= 0;
		uint32_t		uncompressed_length		= 0;
		uint16_t		name_length				= 0;
		uint16_t		extra_field_length		= 0;
		uint16_t		comment_length			= 0;
		uint16_t		disk_number_start		= 0;
		uint16_t		internal_attributes		= 0;
		uint32_t		external_attributes		= 0;
		uint32_t		local_header_offset		= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( CentralDirectoryFileHeader ) == 46, "Central Directory File Header does not aligned properly." );

	/**
	*/
	#pragma pack( push, 2 )
	struct CentralDirectoryDigitalSignature final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::CentralDirectoryDigitalSignature;


		HeaderSignature	signature				= SIGNATURE;
		uint16_t		data_length				= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( CentralDirectoryDigitalSignature ) == 6, "Central Directory Digital Signature does not aligned properly." );

	/**
	*/
	#pragma pack( push, 2 )
	struct Zip64EndOfCentralDirectory final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::Zip64EndOfCentralDirectory;


		HeaderSignature	signature					= SIGNATURE;
		uint64_t		length						= 0;
		uint16_t		compressor_version			= 0;
		uint16_t		extractor_version			= 0;
		uint32_t		current_disk_index			= 0;
		uint32_t		first_disk_index			= 0;
		uint64_t		current_disk_entries		= 0;
		uint64_t		entries_total_count			= 0;
		uint64_t		central_directory_length	= 0;
		uint64_t		central_directory_position	= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( Zip64EndOfCentralDirectory ) == 56, "End of Zip64 Central Directory does not aligned properly." );

	/**
	*/
	#pragma pack( push, 4 )
	struct Zip64EndOfCentralDirectoryLocator final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::Zip64EndOfCentralDirectoryLocator;


		HeaderSignature	signature				= SIGNATURE;
		uint32_t		first_disk_index		= 0;
		uint64_t		position_offset			= 0;
		uint32_t		disks_count				= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( Zip64EndOfCentralDirectoryLocator ) == 20, "Central Directory Digital Signature does not aligned properly." );

	/**
	*/
	#pragma pack( push, 2 )
	struct EndOfCentralDirectory final
	{
		//
		static constexpr HeaderSignature SIGNATURE = HeaderSignature::EndOfCentralDirectory;


		HeaderSignature	signature					= SIGNATURE;
		uint16_t		current_disk_index			= 0;
		uint16_t		first_disk_index			= 0;
		uint16_t		current_disk_entries		= 0;
		uint16_t		entries_total_count			= 0;
		uint32_t		central_directory_length	= 0;
		uint32_t		central_directory_position	= 0;
		uint16_t		comment_length				= 0;
	};
	#pragma pack( pop )

	static_assert( sizeof( EndOfCentralDirectory ) == 22, "Central Directory Digital Signature does not aligned properly." );
}
}
}
}
