/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _AUTHIMGGEN_
#define _AUTHIMGGEN_

#include <string>
#include <vector>
#include "mbedtls/sha256.h"
#define MBEDTLS_FS_IO
#include "mbedtls/x509_crt.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/gcm.h"
#include "elftosb.h"

using namespace std;

/*comming from skboot_image.h*/
/*! @brief Offset to Image Length and Image Link Address info */
#define RT6XX "rt6xx"
#define RT5XX "rt5xx"
#define LPC55XX "lpc55xx"
#define NIOBE4MINI "lpc55s3x"
#define LPC54X0XX "lpc54x0xx"
#define SKBOOT_IMG_HDR_OFFSET(offset) ((offset & 0x0000FFFFu) << 16)

/* bits to be combined with image type */
#define SKBOOT_IMG_HDR_KEY_STORE_INCLUDED_MASK (0x00008000u)
#define SKBOOT_IMG_HDR_TZM_IMAGE_TYPE_MASK (0x00004000u) /* TZM image type */
#define SKBOOT_IMG_HDR_TZM_PRESET_MASK (0x00002000u)     /* TZM preset */
#define SKBOOT_IMG_HDR_HW_USER_KEY_EN (0x00001000u)

	typedef enum _skboot_image_type
	{
		kSKBOOT_ImageTypeInvalid = 0x0,

		/* Image types with image header compatible with ARM-v7m and ARM-v8m vector table */
		kSKBOOT_ImageTypePlainSigned = SKBOOT_IMG_HDR_OFFSET(0) | 0x1u,
		kSKBOOT_ImageTypePlainCrc = SKBOOT_IMG_HDR_OFFSET(0) | 0x2u,
		kSKBOOT_ImageTypeEncryptedSigned = SKBOOT_IMG_HDR_OFFSET(0) | 0x3u,
		kSKBOOT_ImageTypeXipPlainSigned = SKBOOT_IMG_HDR_OFFSET(0) | 0x4u,
		kSKBOOT_ImageTypeXipPlainCrc = SKBOOT_IMG_HDR_OFFSET(0) | 0x5u,
		kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded = kSKBOOT_ImageTypeEncryptedSigned | SKBOOT_IMG_HDR_KEY_STORE_INCLUDED_MASK,
		kSKBOOT_ImageTypePlainSignedKeyStoreIncluded = kSKBOOT_ImageTypePlainSigned | SKBOOT_IMG_HDR_KEY_STORE_INCLUDED_MASK,

		/* Note this concept allows for future image types like
		kSKBOOT_ImageTypePlainSigned_0x400 = SKBOOT_IMG_HDR_OFFSET(0x400) | 0x1u,
		in case if we need to move Image Length and Image Link Address fields to some fixed offset, because some
		future version of ARM-vX might make use of those reserved fields in ARM-v7m and ARM-v8m vector table
		*/

	} skboot_image_type_t;

	/*comming from authentication.h*/

//! @brief Build a 32-bit code from four character values.
//!
//! The resulting value is built with a byte order such that the string
//! being readable in expected order when viewed in a hex editor, if the value
//! is treated as a 32-bit little endian value.
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))


//! @brief Define the length of SHA256 hash.
#define SHA256_HASH_LENGTH_IN_BYTES (32)

//! @brief Tag constants for the certificate block header.
#define SIGNATURE_TAG FOUR_CHAR_CODE('c', 'e', 'r', 't')

//! @brief Define the format related constants.
	enum _bootloader_signed_image_format_constants
	{
		kCertBlockHeaderOffsetAddress = 0x28,   //!< The address of the certificate block header offset.
		kCertBlockHeaderOffsetMinValue = 0x40,  //!< The min value of the certificate block header offset.
		kCertMaxSupportedSizeInBytes = 4096,    //!< The maximum size of certificate, supported by bootloader.
		kRootPublicKeyHashTableMaxEntries = 4,  //!< The maxinum count of RK table entries.
		kImageSessionsAlignmentSizeInBytes = 4, //!< The alignment bytes request by signed image format.
	};

	/*! @brief root public key hash entry definition. */
	//size 32 bytes
	typedef struct _rkh_entry
	{
		uint8_t rkh[SHA256_HASH_LENGTH_IN_BYTES];
	} rkh_entry_t;

	/*! @brief root public key hash table definition. */
	//size 128bytes
	typedef struct _rkh_table
	{
		rkh_entry_t entries[kRootPublicKeyHashTableMaxEntries];
	} rkh_table_t;

	/*! @brief signature block header definition. */
	typedef struct _certificate_block_header
	{
		uint32_t signature;           //!< Always set to 'cert'.
		uint16_t headerMajorVersion;  //!< Major Version.
		uint16_t headerMinorVersion;  //!< Minor Version.
		uint32_t headerLengthInBytes; //!< Number of bytes long the header is, starting from the signature. Does not include
									  //!< the certificate table.
		uint32_t flags;               //!< Reserved for future use.
		uint32_t buildNumber; //!< User specified build number for the signed image. Allows user to prevent reverting to old
							  //! versions.
		uint32_t totalImageLengthInBytes; //!< Length in bytes of the signed data. Does not include RSASSA-PKCS1-v1_5 Signature
		uint32_t certificateCount;   //!< Certificate number in the certificate table. Must be greater than 0.
		uint32_t certificateTableLengthInBytes; //!< Certificate table length.
	} certificate_block_header_t;

	/*comming from key_store.h*/
	//56 bytes
	typedef struct _key_item
	{
		uint32_t type; //4
		union _key_code {
			uint32_t w[52 / sizeof(uint32_t)]; //52
			uint8_t b[52];
		} keyCode;
	} skboot_key_code_item;

	typedef struct _key_item_lpc54X0xx
	{
		union _key_code
		{
			uint32_t w[52 / sizeof(uint32_t)];
			uint8_t b[52];
		} keyCode;
	} skboot_key_code_item_lpc54X0xx;

	//1536 bytes
	typedef struct _key_store_lpc55xx
	{
		uint32_t header;//4
		uint32_t pufDischargeTime_ms;//4
		union _activation_code {
			uint32_t w[1192 / sizeof(uint32_t)]; //1192
			uint8_t b[1192];
		} activationCode;

		skboot_key_code_item sbkek; //336
		skboot_key_code_item userkek;
		skboot_key_code_item uds;
		skboot_key_code_item encryptedRegion0;
		skboot_key_code_item encryptedRegion1;
		skboot_key_code_item encryptedRegion2;
	} skboot_key_store_lpc55xx_t;
	//1424 bytes
	typedef struct _key_store_lpc68xx
	{
		uint32_t header;
		uint32_t pufDischargeTime_ms;
		union _activation_code {
			uint32_t w[1192 / sizeof(uint32_t)];
			uint8_t b[1192];
		} activationCode;

		skboot_key_code_item sbkek;
		skboot_key_code_item userkek;
		skboot_key_code_item uds;
		skboot_key_code_item otfadkek;
	} skboot_key_store_lpc68xx_t;

	typedef struct _key_store_lpc54x0xx
	{
		union _activation_code
		{
			uint32_t w[1192 / sizeof(uint32_t)];
			uint8_t b[1192];
		} activationCode;
		skboot_key_code_item_lpc54X0xx image_key;
		skboot_key_code_item_lpc54X0xx uds;
		skboot_key_code_item_lpc54X0xx fw_update_key;
	} skboot_key_store_lpc54x0xx_t;

	union keyStore_t {
		_key_store_lpc68xx lpc68xx;
		_key_store_lpc55xx lpc55xx;
		_key_store_lpc54x0xx lpc54x0xx;
	};

	//size 56 bytes
	typedef struct _es_header
	{
		uint8_t gmac[16];
		uint8_t iv_gmac[12];
		uint8_t reserved_00[4];
		uint32_t imageLength;
		uint32_t imageType;
		uint32_t offsetToCertHeader;
		uint8_t reserved_01[8];
		uint32_t imageLinkAddress;
	} es_header_t;
	//size 32 bytes
	typedef struct _headerMac
	{
		uint8_t b[32];
	} _headerMac_t;

// Sizes for statically allocated arrays
#define MAX_KEY_BYTES 32
#define AES256_KEY_BYTES 32
#define AES128_KEY_BYTES 16
#define TAG_BYTES 16
#define IV_BYTES  16
#define SIG_LEN_BIT 2048
#define RSA_KEY_SIZE 2048
#define SIG_LEN     (2048/8)

	typedef struct
	{
		uint8_t Sig[SIG_LEN];
	} SignatureType;

	typedef struct
	{
		uint32_t nocrc : 1;
		uint32_t xip : 1;
		uint32_t secure_boot : 1;
		uint32_t signed_ : 1;
		uint32_t encrypted : 1;
		uint32_t montformat : 1;
		uint32_t reserved : 26;
	} ImageType;

	typedef struct
	{
		/* 0x00 - 0x03 */ uint32_t  header_marker; 
		/* 0x04 - 0x07 */ ImageType img_type;      
		/* 0x08 - 0x0B */ uint32_t  load_addr;     
		/* 0x0C - 0x0F */ uint32_t  img_len;       
		/* 0x10 - 0x13 */ uint32_t  crc_value;     
		/* 0x14 - 0x17 */ uint32_t  version;       
		/* 0x18 - 0x1B */ uint32_t  emc_timings;   
		/* 0x1C - 0x1F */ uint32_t  spi_clk_freq;  
		/* 0x20 - 0x23 */ uint32_t  reserved1;     
		/* 0x24 - 0x27 */ uint32_t  image_marker;  
		/* 0x28 - 0x2B */ uint32_t  header_offset;
		/* 0x2C - 0x2F */ uint32_t  reserved2;     
		/* 0x30 - 0x63 */ uint8_t   spifi_desc[52]; 
		/* 0x64 - 0xBF */ uint8_t   padding [92];          
		/* 0xC0 - 0xCF */ uint8_t   image_iv  [IV_BYTES];  
		/* 0xD0 - 0xDF */ uint8_t   image_tag [TAG_BYTES]; 
		/* 0xE0 - 0xEF */ uint8_t   header_iv [IV_BYTES];  
		/* 0xF0 - 0xFF */ uint8_t   header_tag[TAG_BYTES]; 
	} ImageHeaderEncrypted;

	typedef struct
	{
		/* 0x00 - 0x03 */ uint32_t    header_marker; 
		/* 0x04 - 0x07 */ ImageType   img_type;     
		/* 0x08 - 0x0B */ uint32_t    load_addr;   
		/* 0x0C - 0x0F */ uint32_t    img_len;     
		/* 0x10 - 0x13 */ uint32_t    crc_value;   
		/* 0x14 - 0x17 */ uint32_t    version;     
		/* 0x18 - 0x1B */ uint32_t    emc_timings; 
		/* 0x1C - 0x1F */ uint32_t    spi_clk_freq;
		/* 0x20 - 0x2F */ uint32_t    reserved[4];
		/* 0x30 - 0x63 */ uint8_t     spifi_desc[52]; 
	} ImageHeaderSigned;

	typedef struct
	{
		/* 0x00 - 0x03 */ uint32_t    header_marker; 
		/* 0x04 - 0x07 */ ImageType   img_type;      
		/* 0x08 - 0x0B */ uint32_t    load_addr;     
		/* 0x0C - 0x0F */ uint32_t    img_len;       
		/* 0x10 - 0x13 */ uint32_t    crc_value;     
		/* 0x14 - 0x17 */ uint32_t    version;       
		/* 0x18 - 0x1B */ uint32_t    emc_timings;   
		/* 0x1C - 0x1F */ uint32_t    spi_clk_freq;  
		/* 0x20 - 0x2F */ uint32_t    reserved[4];   
		/* 0x30 - 0x63 */ uint8_t     spifi_desc[52];
	} ImageHeaderNonSecure;

	typedef struct
	{
		/* 0x00 - 0x03 */ uint32_t  header_marker;
		/* 0x04 - 0x07 */ uint32_t  img_type;
		/* 0x08 - 0x0B */ uint32_t  key_offset;
		/* 0x0C - 0x0F */ uint32_t  key_store_len;
		/* 0x10 - 0x13 */ uint32_t  img_offset;    
		/* 0x14 - 0x17 */ uint32_t  puf_init_delay;
		/* 0x18 - 0x1B */ uint32_t  emc_timings;   
		/* 0x1C - 0x1F */ uint32_t  spi_clk_freq;  
		/* 0x20 - 0x23 */ uint32_t  reserved2;     
		/* 0x24 - 0x27 */ uint32_t  image_marker;  
		/* 0x28 - 0x2B */ uint32_t  header_offset; 
		/* 0x2C - 0x2F */ uint32_t  reserved3;     
		/* 0x30 - 0x63 */ uint8_t   spifi_desc[52];
		/* 0x64 - 0x7C */ uint8_t   reserved4[28];
	} ImageHeaderKeyStore;

	typedef union
	{
		ImageHeaderEncrypted Encrypted;
		ImageHeaderSigned    Signed;
		ImageHeaderNonSecure NonSecure;
		ImageHeaderNonSecure Common;
		ImageHeaderKeyStore  KeyStore;
	} ImageHeaderCommon;

#define MIN_IMAGE_HEADER_SIZE sizeof(ImageHeaderNonSecure)
#define MAX_IMAGE_HEADER_SIZE sizeof(ImageHeaderEncrypted)

#define IMG_HEADER_CRC_OFFSET offsetof(ImageHeaderNonSecure,crc_value)
#define IH_CRC_SIZE			sizeof(uint32_t)
#define IMG_MARKER			0xEDDC94BD
#define IMG_HEADER_MARKER	0xFEEDA5A5
#define KS_HEADER_MARKER	0x7379656B
#define KS_HEADER_IMGTYPE	0xA5A5A500
#define KS_STORE_SIZE		sizeof(skboot_key_store_lpc54x0xx_t)
#define KS_PUF_DELAY_DEFAULT  240000 

	typedef struct
	{
		uint32_t vector0;
		uint32_t vector1;
		uint32_t vector2;
		uint32_t vector3;
		uint32_t vector4;
		uint32_t vector5;
		uint32_t vector6;
		uint32_t vector7;
		uint32_t vector8;
		uint32_t image_marker;
		uint32_t header_offset;
	} ImageVectorTable;

	typedef struct
	{
		uint8_t		modulus[256];
		uint32_t	exponent;
	}
	PubKey; 
		
#pragma pack(push, 1)
	typedef struct
	{
		uint8_t		version;
		uint8_t		cert_id;
		uint16_t	marker;
		PubKey		img_key_pub;
		PubKey		rot_pub;
	} ImageKeyCertificateV2;

	typedef struct
	{
		uint8_t		version;
		uint8_t		cert_id;
		uint16_t	marker;
		PubKey		user_key_pub;
		PubKey		img_key_pub;
		PubKey		rot_pub;
	} ImageKeyCertificateV3;
#pragma pack(pop)

typedef struct
{
	PubKey					ImageKeyPublic;
	PubKey					RootOfTrustKeyPublic;
	unsigned char			Hash[SHA256_HASH_LENGTH_IN_BYTES];
	SignatureType *			pSig;
} CertificateDetails;

#define V3_CERT_SIZE (sizeof(ImageKeyCertificateV3) + SIG_LEN)
#define V2_CERT_SIZE (sizeof(ImageKeyCertificateV2) + SIG_LEN)
#define MAX_TLS_ERROR_STRING_SUPPORTED 0x200

	/*!
	* \brief Generator of Master Boot images for lpc55xx and rt6xx.
	*
	* Takes the JSON image configuration and creating bin file.
	*/
	class AuthImageGenerator {
	public:
		AuthImageGenerator();
		//! \brief Starting point of Master Boot image generation.
		void execute(const string &confFilePath, const chip_family_t family);
		enum class deviceKeySource {
			keyStore,
			OTP
		};
		struct configuration {
			string family;
			string imageFile;
			string imageType;
			AuthImageGenerator::deviceKeySource deviceKeySrc;
			string imageEncryptionKeyFile;
			uint32_t imageLinkAddress;
			bool imageLinkAddressFromImage;
			bool enableHwUserModeKeys;
			bool trustZone;
			string trustZonePresetFile;
			bool generateTrustZone;
			bool useKeyStore;
			string keyStoreFile;
			vector<string> rootCertFiles;
			vector<string> chainCertFiles;
			uint8_t mainCertId;
			string mainCertPrivateKeyFile;
			string masterBootOutputFile;
			uint32_t buildNumber;
			string deviceKeySource;
			string rootOfTrustKeyFile;
			string rootOfTrustKeyFilePassword;
			string imageKeyCertificate;
			string privateImageKey;
			string privateImageKeyPassword;
			bool   preformattedSignature;
			bool   DebugCompatability;
			bool   BypassCertificateChecks;
		};
	private:
		//! \brief Parrent function to create image.
		bool createImage(const configuration &conf, const chip_family_t family);
		//! \brief Parrent function to parse json image configuration.
		bool parseConfiguration(const std::string &confFilePath, configuration &conf, const chip_family_t family);
		//! \brief Parsing and validating input JSON configuration file for lpc55xx/rt6xx.
		bool parseConfigurationLpcSboot(const std::string &confFilePath, configuration &conf);
		//! \brief Procesing and creating image according provided configuration for lpc55xx/rt6xx.
		int createImageLpcSboot(const configuration &conf);
		//! \brief Freeing reserved resources in createImage function for lpc55xx/rt6xx.
		void cleanUpLpcSboot(uint8_t *userKey, uint8_t * keyStore, FILE * imageFile, FILE * usedRootCrtFile, FILE * outputFile, FILE * trustZonePresetFile, uint8_t * buffer, uint8_t * signature, mbedtls_x509_crt * trustCrt, mbedtls_rsa_context * rsa_priv_key_ctx);
		//! \brief Parsing and validating input JSON configuration file for k3s.
		bool parseConfigurationK3Sboot(const std::string &confFilePath, configuration &conf);
		//! \brief Procesing and creating image according provided configuration for K3S.
		int createImageK3Sboot(const configuration &conf);
		//! \brief Freeing reserved resources in createImage function for k3s.
		void cleanUpK3Sboot(FILE * imageFile, FILE * usedRootCrtFile, FILE * outputFile, uint8_t * buffer, uint8_t * signature, mbedtls_x509_crt * trustCrt, mbedtls_rsa_context * rsa_priv_key_ctx);
		//! \brief Parsing and validating input JSON configuration file for LPC540XX/LPC54S0XX.
		bool parseConfigurationLPC54X0XXSboot(const std::string &confFilePath, configuration &conf);
		//! \brief Procesing and creating image according provided configuration for LPC540XX/LPC54S0XX.
		int createImageLPC54X0XXSboot(const configuration &conf);
		//! \brief Freeing reserved resources in createImage function for LPC540XX/LPC54S0XX.
		void cleanUpLPC54X0XXSboot(FILE * outputFile);
	};

	typedef enum _boot_image_action
	{
		bia_LoadFiles,			//!< Load files
		bia_CRC,				//!< Apply CRC to image 
		bia_Encrypt,			//!< Encrypt the image 
		bia_Sign,				//!< Sign the image
		bia_PUFStore,		    //!< Insert PUF Store
		bia_OutputImage,		//!< Output the image to the file

	} boot_image_action_t;

	/*!
	* \brief Image Helper for LPC540XX/LPC54S0XX. 
	*
	*  Handler functions for manipulating LPC540XX/LPC54S0XX images 
	*/
	class LPC54X0XXImageHelper {
	public:
		LPC54X0XXImageHelper();
		~LPC54X0XXImageHelper();

		void OpenImage		(int step, const AuthImageGenerator::configuration &conf);
		void EncryptImage	(int step, const AuthImageGenerator::configuration &conf);
		void SignImage		(int step, const AuthImageGenerator::configuration &conf);
		void ImageInsertPUF(int step, const AuthImageGenerator::configuration &conf);
		void CRCImage       (int step, const AuthImageGenerator::configuration &conf);
		size_t GetImageSize();
		const uint8_t * const GetImage();

	private:
		uint32_t							CRC_Update(uint32_t initial, size_t offset, size_t len);
		void GetPrivateKeyFromFile			(int step, mbedtls_pk_context &pk,mbedtls_rsa_context * &priv_ctx, const char * KeyFile, const char * Password);
		void TakeIVFromExistingFile			(const char * pImageFile, ImageHeaderEncrypted       *pImageHeader);
		void GenerateRandomEncryptionFields	(int step);
		void GetAesFileFromFile				(int step, const char * pKeyFile, uint8_t * AesKey, uint32_t &AesKeySize);
		void ImageInsertCertAndAllocSigSpace(int step, int &substep, const AuthImageGenerator::configuration &conf, mbedtls_pk_context &pk_ik, mbedtls_pk_context &pk_rk);
		void maybe_raise_mbedtls_fault		(int error, const char * Message, const char * mbedtlsfunc);
		void maybe_raise_mbedtls_fault		(int error, string Message, const char * mbedtlsfunc);
		void maybe_raise_fileaccess_fault	(const char * File, const char * FileDesc);
		void PreFormatSignature				(const SignatureType * const  pSig, const mbedtls_mpi N, const mbedtls_mpi E, SignatureType * pPFSig);
		uint8_t *			pImageBuffer;
		uint32_t			ImageBufferHeaderOffset;
		uint32_t			NoSignInitialBlock;
		size_t				ImageSize;
		size_t				plainImageSize;
		uint32_t			crc_lut_table[256];
	};
#endif /* _AUTHIMGGEN_ */