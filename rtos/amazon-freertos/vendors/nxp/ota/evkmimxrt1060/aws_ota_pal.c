/*
 * Amazon FreeRTOS OTA PAL V1.0.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/* OTA PAL implementation for NXP RT10xx. */

#include <string.h>

#include "aws_ota_pal.h"
#include "iot_crypto.h"
#include "aws_ota_codesigner_certificate.h"

#include "fsl_common.h"
#include "mflash_drv.h"

/* Specify the OTA signature algorithm we support on this platform. */
const char cOTA_JSON_FileSignatureKey[OTA_FILE_SIG_KEY_STR_MAX_LENGTH] = "sig-sha256-ecdsa";


#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

#define BOOTLOADER_META_TAG FOUR_CHAR_CODE('B', 'L', 'M', 'T')
#define SWAP_META_TAG FOUR_CHAR_CODE('S', 'W', 'A', 'P')
#define IMG_HDR_TAG FOUR_CHAR_CODE('I', 'H', 'D', 'R')

#define IMG_TYPE_XIP (1u << 0)
#define IMG_TYPE_NONXIP (1u << 1)

#define IMG_CHK_ALG_CRC32 (1)
#define IMG_CHK_ALG_SHA256 (2)
#define IMG_CHK_ALG_HAB (3)
#define INVALID_VERSION (0xffffffffu)

#define BL_FEATURE_FLASH_START 0x60000000u
#define BL_FEATURE_FLASH_SIZE 0x800000u
#define BL_FEATURE_BOOTLOADER_START BL_FEATURE_FLASH_START
#define BL_FEATURE_BOOTLOADER_SIZE 0x40000
#define BL_FEATURE_FLASH_SECTOR_SIZE MFLASH_SECTOR_SIZE
#define BL_FEATURE_BOOT_META_START \
    (BL_FEATURE_BOOTLOADER_START + BL_FEATURE_BOOTLOADER_SIZE - 4 * BL_FEATURE_FLASH_SECTOR_SIZE)
#define BL_FEATURE_SWAP_META_START (BL_FEATURE_BOOT_META_START + 2 * BL_FEATURE_FLASH_SECTOR_SIZE)
#define BL_FEATURE_PRIMARY_IMG_START (BL_FEATURE_BOOTLOADER_START + BL_FEATURE_BOOTLOADER_SIZE)
#define BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE 0x100000
#define BL_FEATURE_SECONDARY_IMG_START (BL_FEATURE_PRIMARY_IMG_START + BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE)
#define BL_FEATURE_SCRATCH_PARTITION_SIZE (0x1000)
#define BL_FEATURE_SECOND_IMG_PARTITION_SIZE (BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE + BL_FEATURE_SCRATCH_PARTITION_SIZE)
#define BL_FEATURE_USER_DATA_PARTITION_START (BL_FEATURE_SECONDARY_IMG_START + BL_FEATURE_SECOND_IMG_PARTITION_SIZE)
#define BL_FEATURE_USER_DATA_PARTITION_SIZE \
    (BL_FEATURE_FLASH_START + BL_FEATURE_FLASH_SIZE - BL_FEATURE_USER_DATA_PARTITION_START)


#define BL_IMG_HEADER_SIZE 0x400
#define BL_RELIABLE_UPDATE_IF_ADDR 0x6000201c

#define OTA_UPDATE_PART (BL_FEATURE_SECONDARY_IMG_START - FlexSPI_AMBA_BASE)
#define OTA_MAX_IMAGE_SIZE (BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE - BL_IMG_HEADER_SIZE)
#define OTA_IMAGE_LOAD_ADDR (BL_FEATURE_PRIMARY_IMG_START + BL_IMG_HEADER_SIZE)


enum
{
    kSwapType_None,        // Default value when there is no upgradable image
    kSwapType_ReadyForTest, // The application needs to program this when finishing the update operation
    kSwapType_Test,        // The bootloader needs to set the swap_info to this value before running the test image
    kSwapType_Permanent,   // The application needs to set the swap_info to this type when the self-test is okay
    kSwapType_Fail,
    kSwapType_Fatal,
    kSwapType_Max,
};


typedef struct
{
    uint32_t start;
    uint32_t size;
    uint32_t image_state;   // Active/ReadyForTest/UnderTest
    uint32_t attribute;
    uint32_t reserved[12]; // Reserved for future use
} partition_t;


// Image header is platform specific
typedef struct
{
    uint32_t tag;
    uint32_t load_addr;
    uint32_t image_type;
    uint32_t image_size;
    uint32_t algorithm;
    // Note: considering the address alignment requirement on the vector table, take RT1060 for example, the vector
    // table must be 1KB aligned
    //      the header_size can be set to 1KB in this case, and the actual image will follow the header
    uint32_t header_size;
    uint32_t image_version;
    uint32_t reserved[1];
    uint32_t checksum[32]; // Can be CRC checksum
    uint32_t padding[];
} boot_image_header_t;


typedef struct
{
    uint32_t version;
    status_t (*get_partition_info)(partition_t *partition);
    status_t (*update_image_state)(uint32_t state);
    status_t (*get_image_state)(uint32_t *state);
} reliable_update_interface_t;


//! @brief State information for the CRC32 algorithm.
typedef struct Crc32Data
{
    uint32_t currentCrc;   //!< Current CRC value.
    uint32_t byteCountCrc; //!< Number of bytes processed.
} crc32_data_t;

static const uint32_t s_crc32Table[] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005, 0x2608edb8,
    0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
    0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75, 0x6a1936c8, 0x6ed82b7f, 0x639b0da6,
    0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84,
    0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d, 0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
    0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a,
    0xec7dd02d, 0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca, 0x7897ab07,
    0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
    0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba, 0xaca5c697, 0xa864db20, 0xa527fdf9,
    0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b,
    0xf771768c, 0xfa325055, 0xfef34de2, 0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
    0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c,
    0x774bb0eb, 0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b, 0x0315d626,
    0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
    0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b, 0xd727bbb6, 0xd3e6a601, 0xdea580d8,
    0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a,
    0x8cf30bad, 0x81b02d74, 0x857130c3, 0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
    0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093,
    0x65c52d24, 0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654, 0xc5a92679,
    0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
    0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4, 0x89b8fd09, 0x8d79e0be, 0x803ac667,
    0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

// initialize the members of the allocated crc32_data_t struct
void crc32_init(crc32_data_t *crc32Config)
{
    // initialize running crc and byte count
    crc32Config->currentCrc = 0xFFFFFFFF;
    crc32Config->byteCountCrc = 0;
}

// "running" crc32 calculation
void crc32_update(crc32_data_t *crc32Config, const uint8_t *src, uint32_t lengthInBytes)
{
    assert(src);
    uint32_t crc = crc32Config->currentCrc;
    crc32Config->byteCountCrc += lengthInBytes;

    while (lengthInBytes--)
    {
        uint8_t c = *src++ & 0xff;
        crc = (crc << 8) ^ s_crc32Table[(crc >> 24) ^ c];
    }

    crc32Config->currentCrc = crc;
}

// finalize the crc32 calculation for non-word-aligned counts
void crc32_finalize(crc32_data_t *crc32Config, uint32_t *hash)
{
    uint32_t crc = crc32Config->currentCrc;
    uint32_t byteCount = crc32Config->byteCountCrc;

    // pad with zeroes
    if (byteCount % 4)
    {
        uint32_t i;
        for (i = byteCount % 4; i < 4; i++)
        {
            crc = (crc << 8) ^ s_crc32Table[(crc >> 24) ^ 0];
        }
    }

    crc32Config->currentCrc = crc;

    *hash = crc32Config->currentCrc;
}


/* low level file context structure */
typedef struct
{
    const OTA_FileContext_t *FileXRef;
    uint32_t Addr;
    uint32_t Size;
} LL_FileContext_t;

static LL_FileContext_t prvPAL_CurrentFileContext;


static char *prvPAL_GetCertificate(const uint8_t *pucCertName, uint32_t *ulSignerCertSize)
{
    if (NULL != ulSignerCertSize)
    {
        *ulSignerCertSize = sizeof(signingcredentialSIGNING_CERTIFICATE_PEM);
    }
    return (char *)signingcredentialSIGNING_CERTIFICATE_PEM;
}


static LL_FileContext_t *prvPAL_GetLLFileContext(OTA_FileContext_t *const C)
{
    LL_FileContext_t *FileContext;

    if ((C == NULL) || (C->pucFile == NULL))
    {
        return NULL;
    }

    FileContext = (LL_FileContext_t *)C->pucFile;

    if ((FileContext == NULL) || (FileContext->FileXRef != C))
    {
        return NULL;
    }

    return FileContext;
}


static OTA_Err_t prvPAL_CheckFileSignature(OTA_FileContext_t *const C)
{
    LL_FileContext_t *FileContext;
    uint8_t *FileData;

    void *VerificationContext;

    char *cert = NULL;
    uint32_t certsize;


    OTA_LOG_L1("[OTA-NXP] CheckFileSignature\r\n");

    FileContext = prvPAL_GetLLFileContext(C);
    if (FileContext == NULL)
    {
        return kOTA_Err_SignatureCheckFailed;
    }

    FileData = mflash_drv_mmap(FileContext->Addr, FileContext->Size);
    if (FileData == NULL)
    {
        return kOTA_Err_SignatureCheckFailed;
    }

    cert = prvPAL_GetCertificate((const uint8_t *)C->pucCertFilepath, &certsize);
    if (cert == NULL)
    {
        return kOTA_Err_BadSignerCert;
    }

    if (CRYPTO_SignatureVerificationStart(&VerificationContext, cryptoASYMMETRIC_ALGORITHM_ECDSA,
                                          cryptoHASH_ALGORITHM_SHA256) != pdTRUE)
    {
        return kOTA_Err_SignatureCheckFailed;
    }

    CRYPTO_SignatureVerificationUpdate(VerificationContext, FileData, FileContext->Size);

    if (CRYPTO_SignatureVerificationFinal(VerificationContext, cert, certsize, C->pxSignature->ucData,
                                          C->pxSignature->usSize) != pdTRUE)
    {
        return kOTA_Err_SignatureCheckFailed;
    }

    return kOTA_Err_None;
}


OTA_Err_t prvPAL_Abort(OTA_FileContext_t *const C)
{
    OTA_Err_t result = kOTA_Err_None;

    OTA_LOG_L1("[OTA-NXP] Abort\r\n");

    C->pucFile = NULL;
    return result;
}


OTA_Err_t prvPAL_CreateFileForRx(OTA_FileContext_t *const C)
{
    LL_FileContext_t *FileContext = &prvPAL_CurrentFileContext;

    OTA_LOG_L1("[OTA-NXP] CreateFileForRx\r\n");

    if (C->ulFileSize > OTA_MAX_IMAGE_SIZE)
    {
        return kOTA_Err_RxFileTooLarge;
    }

    FileContext->Size     = 0;
    FileContext->Addr = OTA_UPDATE_PART + BL_IMG_HEADER_SIZE;

    if (FileContext->Addr == 0)
    {
        return kOTA_Err_RxFileCreateFailed;
    }

    FileContext->FileXRef = C; /* cross reference for integrity check */
    C->pucFile = (uint8_t *)FileContext;

    return kOTA_Err_None;
}


OTA_Err_t prvPAL_CloseFile(OTA_FileContext_t *const C)
{
    OTA_Err_t result = kOTA_Err_None;
    LL_FileContext_t *FileContext;
    uint8_t *FileData;

    OTA_LOG_L1("[OTA-NXP] CloseFile\r\n");

    FileContext = prvPAL_GetLLFileContext(C);
    if (FileContext == NULL)
    {
        return kOTA_Err_FileClose;
    }

    result = prvPAL_CheckFileSignature(C);
    if (result != kOTA_Err_None)
    {
        OTA_LOG_L1("[OTA-NXP] CheckFileSignature failed\r\n");
    }
    else
    {
        /* prepare boot image header including CRC */
        crc32_data_t crc_data;

        boot_image_header_t bih;
        memset((void *)&bih, 0, sizeof(bih));

        bih.tag = IMG_HDR_TAG;
        bih.header_size = BL_IMG_HEADER_SIZE;
        bih.image_type = IMG_TYPE_XIP;
        bih.image_size = FileContext->Size;
        bih.load_addr = OTA_IMAGE_LOAD_ADDR;
        bih.algorithm = IMG_CHK_ALG_CRC32;

        FileData = mflash_drv_mmap(FileContext->Addr, FileContext->Size);
        if (FileData == NULL)
        {
            result = kOTA_Err_BootInfoCreateFailed;
        }
        else
        {
            crc32_init(&crc_data);
            crc32_update(&crc_data, FileData, FileContext->Size);
            crc32_finalize(&crc_data, &bih.checksum[0]);
        }

        if (result == kOTA_Err_None)
        {
            /* write boot image header to flash and prepare image to be executed in test mode */
            reliable_update_interface_t *update_if = *(reliable_update_interface_t**)BL_RELIABLE_UPDATE_IF_ADDR;
            if (mflash_drv_write(OTA_UPDATE_PART, (uint8_t *)&bih, sizeof(bih)) != 0)
            {
                result = kOTA_Err_BootInfoCreateFailed;
            }
            else if (update_if->update_image_state(kSwapType_ReadyForTest) != kStatus_Success)
            {
                result = kOTA_Err_BootInfoCreateFailed;
            }
        }
    }

    C->pucFile = NULL;
    return result;
}


int16_t prvPAL_WriteBlock(OTA_FileContext_t *const C, uint32_t ulOffset, uint8_t *const pcData, uint32_t ulBlockSize)
{
    int32_t result;
    LL_FileContext_t *FileContext;

    OTA_LOG_L1("[OTA-NXP] WriteBlock %x : %x\r\n", ulOffset, ulBlockSize);

    FileContext = prvPAL_GetLLFileContext(C);
    if (FileContext == NULL)
    {
        return -1;
    }

    if (ulOffset + ulBlockSize > OTA_MAX_IMAGE_SIZE)
    {
        return -1;
    }

    result = mflash_drv_write(FileContext->Addr + ulOffset, pcData, ulBlockSize);
    if (result == 0)
    {
        /* zero indicates no error, return number of bytes written to the caller */
        result = ulBlockSize;
        if (FileContext->Size < ulOffset + ulBlockSize)
        {
            /* extend file size according to highest offset */
            FileContext->Size = ulOffset + ulBlockSize;
        }
    }
    return result;
}


OTA_Err_t prvPAL_ActivateNewImage(void)
{
    OTA_LOG_L1("[OTA-NXP] ActivateNewImage\r\n");
    prvPAL_ResetDevice(); /* go for reboot */
    return kOTA_Err_None;
}


OTA_Err_t prvPAL_ResetDevice(void)
{
    OTA_LOG_L1("[OTA-NXP] ResetDevice\r\n");
    NVIC_SystemReset(); /* this should never return */
}


OTA_Err_t prvPAL_SetPlatformImageState(OTA_ImageState_t eState)
{
    OTA_Err_t result = kOTA_Err_None;

    reliable_update_interface_t *update_if = *(reliable_update_interface_t**)BL_RELIABLE_UPDATE_IF_ADDR;

    OTA_LOG_L1("[OTA-NXP] SetPlatformImageState %d\r\n", eState);

    if (prvPAL_GetPlatformImageState() == eOTA_PAL_ImageState_PendingCommit)
    {
        /* Device in test mode */
        switch (eState)
        {
            case eOTA_ImageState_Accepted:
                /* Request the bootloader to switch the image permanently */
                if (update_if->update_image_state(kSwapType_Permanent) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = kOTA_Err_CommitFailed;
                }
                break;

            case eOTA_ImageState_Rejected:
                /* Invalidate the image */
                if (update_if->update_image_state(kSwapType_Fail) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = kOTA_Err_RejectFailed;
                }
                break;

            case eOTA_ImageState_Aborted:
                /* Invalidate the image */
                if (update_if->update_image_state(kSwapType_Fail) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = kOTA_Err_AbortFailed;
                }
                break;

            case eOTA_ImageState_Testing:
                result = kOTA_Err_None;
                break;

            default:
                result = kOTA_Err_BadImageState;
                break;
        }
    }
    else
    {
        /* Normal mode */
        switch (eState)
        {
            case eOTA_ImageState_Accepted:
                /* No pending commit */
                result = kOTA_Err_CommitFailed;
                break;

            case eOTA_ImageState_Rejected:
                result = kOTA_Err_RejectFailed;
                break;

            case eOTA_ImageState_Aborted:
                result = kOTA_Err_AbortFailed;
                break;

            case eOTA_ImageState_Testing:
                result = kOTA_Err_BadImageState;
                break;

            default:
                result = kOTA_Err_BadImageState;
                break;
        }
    }

    return result;
}


OTA_PAL_ImageState_t prvPAL_GetPlatformImageState(void)
{
    reliable_update_interface_t *update_if = *(reliable_update_interface_t**)BL_RELIABLE_UPDATE_IF_ADDR;
    uint32_t state;

    if (update_if->get_image_state(&state) != kStatus_Success)
    {
        return eOTA_PAL_ImageState_Invalid;
    }

    switch (state)
    {
        case kSwapType_ReadyForTest:
            return eOTA_PAL_ImageState_Valid;
            break;

        case kSwapType_Test:
            return eOTA_PAL_ImageState_PendingCommit;
            break;
    }

    return eOTA_PAL_ImageState_Invalid;
}


/* Provide access to private members for testing. */
#ifdef AMAZON_FREERTOS_ENABLE_UNIT_TESTS
#include "aws_ota_pal_test_access_define.h"
#endif
