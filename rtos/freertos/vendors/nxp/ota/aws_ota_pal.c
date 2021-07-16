/*
 * FreeRTOS OTA PAL V1.0.0
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

/* OTA PAL implementation for NXP MCUXpresso SDK. */

#include <string.h>

#include "aws_iot_ota_pal.h"
#include "iot_crypto.h"
#include "aws_ota_codesigner_certificate.h"

#include "fsl_common.h"
#include "mflash_drv.h"

#include "ota_bootloader_supp.h"

/* Specify the OTA signature algorithm we support on this platform. */
const char cOTA_JSON_FileSignatureKey[OTA_FILE_SIG_KEY_STR_MAX_LENGTH] = "sig-sha256-ecdsa";


/* PAL file context structure */
typedef struct
{
    const OTA_FileContext_t *FileXRef;
    uint32_t partition_log_addr;
    uint32_t partition_phys_addr;
    uint32_t partition_size;
    uint32_t next_erase_addr;
    uint32_t file_size;
    uint32_t page_size;
    uint32_t *first_page_buffer;
} PAL_FileContext_t;

static PAL_FileContext_t prvPAL_CurrentFileContext;


static char *prvPAL_GetCertificate(const uint8_t *pucCertName, uint32_t *ulSignerCertSize)
{
    if (NULL != ulSignerCertSize)
    {
        *ulSignerCertSize = sizeof(signingcredentialSIGNING_CERTIFICATE_PEM);
    }
    return (char *)signingcredentialSIGNING_CERTIFICATE_PEM;
}


static PAL_FileContext_t *prvPAL_GetPALFileContext(OTA_FileContext_t *const C)
{
    PAL_FileContext_t *FileContext;

    if ((C == NULL) || (C->pucFile == NULL))
    {
        return NULL;
    }

    FileContext = (PAL_FileContext_t *)C->pucFile;

    if ((FileContext == NULL) || (FileContext->FileXRef != C))
    {
        return NULL;
    }

    return FileContext;
}


static OTA_Err_t prvPAL_CheckFileSignature(OTA_FileContext_t *const C)
{
    PAL_FileContext_t *FileContext;
    void *VerificationContext;

    int first_page_len;
    uint8_t *next_pages;

    char *cert = NULL;
    uint32_t certsize;

    OTA_LOG_L1("[OTA-NXP] CheckFileSignature\r\n");

    FileContext = prvPAL_GetPALFileContext(C);
    if (FileContext == NULL)
    {
        return kOTA_Err_SignatureCheckFailed;
    }

    first_page_len = FileContext->file_size < FileContext->page_size ? FileContext->file_size : FileContext->page_size;
    next_pages = mflash_drv_phys2log(FileContext->partition_phys_addr + FileContext->page_size, FileContext->file_size - first_page_len);
    if (next_pages == NULL)
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

    CRYPTO_SignatureVerificationUpdate(VerificationContext, (uint8_t *)FileContext->first_page_buffer, first_page_len);
    CRYPTO_SignatureVerificationUpdate(VerificationContext, next_pages, FileContext->file_size - first_page_len);

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
    PAL_FileContext_t *FileContext;

    OTA_LOG_L1("[OTA-NXP] Abort\r\n");

    FileContext = prvPAL_GetPALFileContext(C);
    if ((FileContext != NULL) && (FileContext->first_page_buffer != NULL))
    {
        vPortFree(FileContext->first_page_buffer);
    }

    C->pucFile = NULL;
    return result;
}


OTA_Err_t prvPAL_CreateFileForRx(OTA_FileContext_t *const C)
{
    partition_t update_partition;
    PAL_FileContext_t *FileContext = &prvPAL_CurrentFileContext;

    OTA_LOG_L1("[OTA-NXP] CreateFileForRx\r\n");

    if (bl_get_update_partition_info(&update_partition) != kStatus_Success)
    {
        OTA_LOG_L1("[OTA-NXP] could not get update partition information\r\n");
        return kOTA_Err_RxFileCreateFailed;
    }

    /* Keep partition info in the file context */
    FileContext->partition_log_addr = update_partition.start;
    FileContext->partition_size = update_partition.size;

    /* Obtain physical address to perform flash operations with */
    FileContext->partition_phys_addr = mflash_drv_log2phys((void *)update_partition.start, update_partition.size);
    if (FileContext->partition_phys_addr == MFLASH_INVALID_ADDRESS)
    {
        OTA_LOG_L1("[OTA-NXP] could not get update partition FLASH address\r\n");
        return kOTA_Err_RxFileCreateFailed;
    }

    /* Check partition alignment */
    if (!mflash_drv_is_sector_aligned(FileContext->partition_phys_addr) || !mflash_drv_is_sector_aligned(FileContext->partition_size))
    {
        OTA_LOG_L1("[OTA-NXP] invalid update partition\r\n");
        return kOTA_Err_RxFileCreateFailed;
    }

    /* Check whether the file fits at all */
    if (C->ulFileSize > update_partition.size)
    {
        OTA_LOG_L1("[OTA-NXP] file too large\r\n");
        return kOTA_Err_RxFileTooLarge;
    }

    /* Actual size of the file according to data received */
    FileContext->file_size = 0;

    /* Allocate buffer to keep data for the page containing image header until the rest of the image is received */
    FileContext->page_size = MFLASH_PAGE_SIZE;
    FileContext->first_page_buffer = pvPortMalloc(FileContext->page_size);
    if (FileContext->first_page_buffer == NULL)
    {
        OTA_LOG_L1("[OTA-NXP] could not allocate page buffer\r\n");
        return kOTA_Err_RxFileCreateFailed;
    }
    memset(FileContext->first_page_buffer, 0xff, FileContext->page_size);

    /* Pre-set address of area not erased so far */
    FileContext->next_erase_addr = FileContext->partition_phys_addr;

    FileContext->FileXRef = C; /* pointer cross reference for integrity check */
    C->pucFile = (uint8_t *)FileContext;

    return kOTA_Err_None;
}


OTA_Err_t prvPAL_CloseFile(OTA_FileContext_t *const C)
{
    OTA_Err_t result = kOTA_Err_None;
    PAL_FileContext_t *FileContext;

    OTA_LOG_L1("[OTA-NXP] CloseFile\r\n");

    FileContext = prvPAL_GetPALFileContext(C);
    if (FileContext == NULL)
    {
        return kOTA_Err_FileClose;
    }

    if (FileContext->file_size != C->ulFileSize)
    {
        OTA_LOG_L1("[OTA-NXP] Actual file size is not as expected\r\n");
    }

    result = prvPAL_CheckFileSignature(C);
    if (result != kOTA_Err_None)
    {
        OTA_LOG_L1("[OTA-NXP] CheckFileSignature failed\r\n");
    }

#if OTA_DEVEL_MODE
    /* Fixes image header required by ota_bootloader by setting the length and generating the proper checksum.
    * This is to provide easy way of testing/debugging of OTA enabled application.
    * Should not be used in production environment! */

    /* Assuming the image header always fits into single FLASH page */
    assert(FileContext->page_size > sizeof(boot_image_header_t));

    if (FileContext->file_size > FileContext->page_size)
    {
        boot_image_header_t *bih = (boot_image_header_t *)FileContext->first_page_buffer;
        /* The image data should start in the following page thanks to header padding - required for checksum calculation */
        if ((bih->image_size == 0) && (bih->header_size >= FileContext->page_size) && (bih->header_size < FileContext->file_size))
        {
            uint8_t *image_content = mflash_drv_phys2log(FileContext->partition_phys_addr + bih->header_size, FileContext->file_size - bih->header_size);
            if (image_content != NULL)
            {
                OTA_LOG_L1("[OTA-NXP] validating the image in development mode\r\n");
                bih->image_size  = FileContext->file_size - bih->header_size;
                bih->checksum[0] = bl_crc32(image_content, bih->image_size);
                bih->algorithm   = IMG_CHK_ALG_CRC32;
            }
        }
    }
#endif

    /* Put the first page of the partition into place */
    if ((result == kOTA_Err_None) && (mflash_drv_page_program(FileContext->partition_phys_addr, FileContext->first_page_buffer) != 0))
    {
        result = kOTA_Err_BootInfoCreateFailed;
    }

    /* Sanity check of the image and its header solely from the flash as the bootloader would do */
    if (result == kOTA_Err_None)
    {
        uint8_t *file_data = mflash_drv_phys2log(FileContext->partition_phys_addr, FileContext->file_size);
        if ((file_data == NULL) || (bl_verify_image(file_data, FileContext->file_size) <= 0))
            result = kOTA_Err_BootInfoCreateFailed;
    }

    /* Prepare image to be booted in test mode */
    if ((result == kOTA_Err_None) && (bl_update_image_state(kSwapType_ReadyForTest) != kStatus_Success))
    {
        result = kOTA_Err_BootInfoCreateFailed;
    }

    if (FileContext->first_page_buffer != NULL)
        vPortFree(FileContext->first_page_buffer);

    C->pucFile = NULL;
    return result;
}


int16_t prvPAL_WriteBlock(OTA_FileContext_t *const C, uint32_t ulOffset, uint8_t *const pcData, uint32_t ulBlockSize)
{
    int16_t retval = 0;
    int32_t mflash_result = 0;

    uint8_t *data;
    uint32_t data_offset;
    uint32_t data_remaining;

    PAL_FileContext_t *FileContext;

    OTA_LOG_L1("[OTA-NXP] WriteBlock 0x%x : 0x%x\r\n", ulOffset, ulBlockSize);

    FileContext = prvPAL_GetPALFileContext(C);
    if (FileContext == NULL)
    {
        return -1;
    }

    /* Check for possible partition boundary overrun */
    if (ulOffset + ulBlockSize > FileContext->partition_size)
    {
        return -1;
    }

    /*
     * The block is expected to be page aligned. The otaconfigLOG2_FILE_BLOCK_SIZE should be set so that the blocks are at least of page size (or larger).
     * That way all blocks except for the last one would be block aligned in both offset and size
     */
    if (!mflash_drv_is_page_aligned(ulOffset))
    {
        OTA_LOG_L1("[OTA-NXP] block is not page aligned\r\n");
        return -1;
    }

    data = pcData;
    data_offset = ulOffset;
    data_remaining = ulBlockSize;

    if (data_offset == 0)
    {
        /* Store data for the first page into buffer and keep it there until the whole file is received */
        uint32_t len = data_remaining <  FileContext->page_size ? data_remaining : FileContext->page_size;
        memcpy(FileContext->first_page_buffer, data, len);
        data += len;
        data_offset += len;
        data_remaining -= len;
        retval += len;
    }

    /* The block may span multiple pages, process in a loop */
    while (data_remaining)
    {
        uint32_t len = data_remaining <  FileContext->page_size ? data_remaining : FileContext->page_size;

        /* Perform erase when encountering next sector */
        while (FileContext->partition_phys_addr + data_offset >= FileContext->next_erase_addr)
        {
            OTA_LOG_L1("[OTA-NXP] erasing sector 0x%x\r\n", FileContext->next_erase_addr);
            mflash_result = mflash_drv_sector_erase(FileContext->next_erase_addr);
            if (mflash_result != 0)
                break;

            FileContext->next_erase_addr += MFLASH_SECTOR_SIZE;
        }

        if (mflash_result != 0)
        {
            retval = -1;
            break;
        }

        if (len == FileContext->page_size && (((uint32_t)data % 4) == 0))
        {
            mflash_result = mflash_drv_page_program(FileContext->partition_phys_addr + data_offset, (void *)data);
        }
        else
        {
            /* Data size not aligned to page size, use temporary buffer */
            uint32_t *page_buffer = pvPortMalloc(FileContext->page_size);
            if (page_buffer == NULL)
            {
                OTA_LOG_L1("[OTA-NXP] could not allocate page buffer\r\n");
                retval = -1;
                break;
            }
            memset(page_buffer, 0xff, FileContext->page_size);
            memcpy(page_buffer, data, len);
            mflash_result = mflash_drv_page_program(FileContext->partition_phys_addr + data_offset, page_buffer);
            vPortFree(page_buffer);
        }

        if (mflash_result != 0)
        {
            retval = -1;
            break;
        }

        data += len;
        data_offset += len;
        data_remaining -= len;
        retval += len;
    }

    /* Update size of file received so far */
    if ((retval > 0) && (FileContext->file_size < data_offset))
    {
        FileContext->file_size = data_offset;
    }

    return retval;
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

    OTA_LOG_L1("[OTA-NXP] SetPlatformImageState %d\r\n", eState);

    if (prvPAL_GetPlatformImageState() == eOTA_PAL_ImageState_PendingCommit)
    {
        /* Device in test mode */
        switch (eState)
        {
            case eOTA_ImageState_Accepted:
                /* Request the bootloader to switch the image permanently */
                if (bl_update_image_state(kSwapType_Permanent) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = kOTA_Err_CommitFailed;
                }
                break;

            case eOTA_ImageState_Rejected:
                /* Invalidate the image */
                if (bl_update_image_state(kSwapType_Fail) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = kOTA_Err_RejectFailed;
                }
                break;

            case eOTA_ImageState_Aborted:
                /* Invalidate the image */
                if (bl_update_image_state(kSwapType_Fail) != kStatus_Success)
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
    uint32_t state;

    if (bl_get_image_state(&state) != kStatus_Success)
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
