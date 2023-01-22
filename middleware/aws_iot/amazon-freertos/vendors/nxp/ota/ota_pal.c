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

#include "ota_pal.h"
#include "iot_crypto.h"

#include "fsl_common.h"
#include "mflash_drv.h"

#include "mcuboot_app_support.h"
#include "task.h"

/* Specify the OTA signature algorithm we support on this platform. */
const char OTA_JsonFileSignatureKey[OTA_FILE_SIG_KEY_STR_MAX_LENGTH] = "sig-sha256-ecdsa";

/* Code signing certificate */
static const char codeSigningCertificatePEM[] = otapalconfigCODE_SIGNING_CERTIFICATE;

/* PAL file context structure */
typedef struct
{
    const OtaFileContext_t *FileXRef;
    uint32_t partition_log_addr;
    uint32_t partition_phys_addr;
    uint32_t partition_size;
    uint32_t next_erase_addr;
    uint32_t file_size;
    uint32_t page_size;
} PAL_FileContext_t;

static PAL_FileContext_t prvPAL_CurrentFileContext;


static char *prvPAL_GetCertificate(const uint8_t *pucCertName, uint32_t *ulSignerCertSize)
{
    if (NULL != ulSignerCertSize)
    {
        *ulSignerCertSize = sizeof(codeSigningCertificatePEM);
    }
    return (char *)codeSigningCertificatePEM;
}


static PAL_FileContext_t *prvPAL_GetPALFileContext(OtaFileContext_t *const C)
{
    PAL_FileContext_t *PalFileContext;

    if ((C == NULL) || (C->pFile == NULL))
    {
        return NULL;
    }

    PalFileContext = (PAL_FileContext_t *)C->pFile;

    if ((PalFileContext == NULL) || (PalFileContext->FileXRef != C))
    {
        return NULL;
    }

    return PalFileContext;
}


static OtaPalStatus_t prvPAL_CheckFileSignature(OtaFileContext_t *const C)
{
    PAL_FileContext_t *PalFileContext;
    void *VerificationContext;

    uint8_t *file_data;

    char *cert = NULL;
    uint32_t certsize;

    int ret;
    size_t size;
    uint32_t offset;
    uint32_t buf[128 / sizeof(uint32_t)];

    LogDebug(("[OTA-NXP] CheckFileSignature"));

    PalFileContext = prvPAL_GetPALFileContext(C);
    if (PalFileContext == NULL)
    {
        return OTA_PAL_COMBINE_ERR(OtaPalSignatureCheckFailed, 1);
    }

    cert = prvPAL_GetCertificate((const uint8_t *)C->pCertFilepath, &certsize);
    if (cert == NULL)
    {
        return OTA_PAL_COMBINE_ERR(OtaPalBadSignerCert, 0);
    }

    if (CRYPTO_SignatureVerificationStart(&VerificationContext, cryptoASYMMETRIC_ALGORITHM_ECDSA,
                                          cryptoHASH_ALGORITHM_SHA256) != pdTRUE)
    {
        return OTA_PAL_COMBINE_ERR(OtaPalSignatureCheckFailed, 3);
    }

    /* On MCU's that support Flash Remap the flash content must be read using physical
     * address. This is because when remap is active the area used for download is not
     * accessible through a logical address (memory pointer).
     */

    offset = PalFileContext->partition_phys_addr;
    size = PalFileContext->file_size;

    if (offset % 4)
    {
        /* mflash has limitation for unaligned access */
        return OTA_PAL_COMBINE_ERR(OtaPalSignatureCheckFailed, 5);
    }

    while (size > 0)
    {
        size_t chunk = (size > sizeof(buf)) ? sizeof(buf) : size;

        /* mflash demands size to be in multiples of 4 */
        size_t chunkAlign4 = (chunk + 3) & (~3);

        ret = mflash_drv_read(offset, buf, chunkAlign4);
        if (ret != kStatus_Success)
        {
            return OTA_PAL_COMBINE_ERR(OtaPalSignatureCheckFailed, 6);
        }

        CRYPTO_SignatureVerificationUpdate(VerificationContext, (uint8_t *)buf, chunk);

        size -= chunk;
        offset += chunk;
    }

    if (CRYPTO_SignatureVerificationFinal(VerificationContext, cert, certsize, C->pSignature->data,
                                          C->pSignature->size) != pdTRUE)
    {
        return OTA_PAL_COMBINE_ERR(OtaPalSignatureCheckFailed, 4);
    }

    return OTA_PAL_COMBINE_ERR(OtaPalSuccess, 0);
}


OtaPalStatus_t otaPal_Abort(OtaFileContext_t *const C)
{
    OtaPalStatus_t result = OTA_PAL_COMBINE_ERR(OtaPalSuccess, 0);

    LogInfo(("[OTA-NXP] Abort"));

    C->pFile = NULL;
    return result;
}


OtaPalStatus_t otaPal_CreateFileForRx(OtaFileContext_t *const C)
{
    partition_t update_partition;
    PAL_FileContext_t *PalFileContext = &prvPAL_CurrentFileContext;

    LogDebug(("[OTA-NXP] CreateFileForRx"));

    /* Obtain physical address to perform flash operations with */

    if (bl_get_update_partition_info(0, &update_partition) != kStatus_Success)
    {
        LogError(("[OTA-NXP] Could not get update partition information"));
        return OTA_PAL_COMBINE_ERR(OtaPalRxFileCreateFailed, 1);
    }

    PalFileContext->partition_phys_addr = update_partition.start;
    PalFileContext->partition_size = update_partition.size;
    PalFileContext->partition_log_addr = NULL;

    /* Check partition alignment */
    if (!mflash_drv_is_sector_aligned(PalFileContext->partition_phys_addr) || !mflash_drv_is_sector_aligned(PalFileContext->partition_size))
    {
        LogError(("[OTA-NXP] Invalid update partition"));
        return OTA_PAL_COMBINE_ERR(OtaPalRxFileCreateFailed, 3);
    }

    /* Check whether the file fits at all */
    if (C->fileSize > update_partition.size)
    {
        LogError(("[OTA-NXP] File too large"));
        return OTA_PAL_COMBINE_ERR(OtaPalRxFileTooLarge, 0);
    }

    /* Actual size of the file according to data received */
    PalFileContext->file_size = 0;

    /* Allocate buffer to keep data for the page containing image header until the rest of the image is received */
    PalFileContext->page_size = MFLASH_PAGE_SIZE;

    /* Pre-set address of area not erased so far */
    PalFileContext->next_erase_addr = PalFileContext->partition_phys_addr;

    PalFileContext->FileXRef = C; /* pointer cross reference for integrity check */
    C->pFile = (uint8_t *)PalFileContext;

    return OTA_PAL_COMBINE_ERR(OtaPalSuccess, 0);
}


OtaPalStatus_t otaPal_CloseFile(OtaFileContext_t *const C)
{
    OtaPalStatus_t result = OTA_PAL_COMBINE_ERR(OtaPalSuccess, 0);
    PAL_FileContext_t *PalFileContext;

    LogDebug(("[OTA-NXP] CloseFile"));

    PalFileContext = prvPAL_GetPALFileContext(C);
    if (PalFileContext == NULL)
    {
        return OTA_PAL_COMBINE_ERR(OtaPalFileClose, 0);
    }

    if (PalFileContext->file_size != C->fileSize)
    {
        LogWarn(("[OTA-NXP] Actual file size is not as expected"));
    }

    result = prvPAL_CheckFileSignature(C);
    if (OTA_PAL_MAIN_ERR(result) != OtaPalSuccess)
    {
        LogError(("[OTA-NXP] CheckFileSignature failed"));
    }

    /* Sanity check of the image and its header solely from the flash as the bootloader would do */
    if (OTA_PAL_MAIN_ERR(result) == OtaPalSuccess)
    {
        if ((bl_verify_image(PalFileContext->partition_phys_addr, PalFileContext->file_size) <= 0))
        {
            LogError(("[OTA-NXP] Invalid image"));
            result = OTA_PAL_COMBINE_ERR(OtaPalBootInfoCreateFailed, 1);
        }
    }

    /* Prepare image to be booted in test mode */
    if ((OTA_PAL_MAIN_ERR(result) == OtaPalSuccess) && (bl_update_image_state(0, kSwapType_ReadyForTest) != kStatus_Success))
    {
        LogError(("[OTA-NXP] Failed to set image state"));
        result = OTA_PAL_COMBINE_ERR(OtaPalBootInfoCreateFailed, 2);
    }

    C->pFile = NULL;
    return result;
}


int16_t otaPal_WriteBlock(OtaFileContext_t *const C, uint32_t ulOffset, uint8_t *const pcData, uint32_t ulBlockSize)
{
    int16_t retval = 0;
    int32_t mflash_result = 0;

    uint8_t *data;
    uint32_t data_offset;
    uint32_t data_remaining;

    PAL_FileContext_t *PalFileContext;

    LogDebug(("[OTA-NXP] WriteBlock 0x%x : 0x%x", ulOffset, ulBlockSize));

    PalFileContext = prvPAL_GetPALFileContext(C);
    if (PalFileContext == NULL)
    {
        return -1;
    }

    /* Check for possible partition boundary overrun */
    if (ulOffset + ulBlockSize > PalFileContext->partition_size)
    {
        return -1;
    }

    /*
     * The block is expected to be page aligned. The otaconfigLOG2_FILE_BLOCK_SIZE should be set so that the blocks are at least of page size (or larger).
     * That way all blocks except for the last one would be block aligned in both offset and size
     */
    if (!mflash_drv_is_page_aligned(ulOffset))
    {
        LogError(("[OTA-NXP] Block is not page aligned"));
        return -1;
    }

    data = pcData;
    data_offset = ulOffset;
    data_remaining = ulBlockSize;

    /* The block may span multiple pages, process in a loop */
    while (data_remaining)
    {
        uint32_t len = data_remaining <  PalFileContext->page_size ? data_remaining : PalFileContext->page_size;

        /* Perform erase when encountering next sector */
        while (PalFileContext->partition_phys_addr + data_offset >= PalFileContext->next_erase_addr)
        {
            LogDebug(("[OTA-NXP] Erasing sector 0x%x", PalFileContext->next_erase_addr));
            mflash_result = mflash_drv_sector_erase(PalFileContext->next_erase_addr);
            if (mflash_result != 0)
                break;

            PalFileContext->next_erase_addr += MFLASH_SECTOR_SIZE;
        }

        if (mflash_result != 0)
        {
            retval = -1;
            break;
        }

        if (len == PalFileContext->page_size && (((uint32_t)data % 4) == 0))
        {
            mflash_result = mflash_drv_page_program(PalFileContext->partition_phys_addr + data_offset, (void *)data);
        }
        else
        {
            /* Data size not aligned to page size, use temporary buffer */
            uint32_t *page_buffer = pvPortMalloc(PalFileContext->page_size);
            if (page_buffer == NULL)
            {
                LogError(("[OTA-NXP] Could not allocate page buffer"));
                retval = -1;
                break;
            }
            memset(page_buffer, 0xff, PalFileContext->page_size);
            memcpy(page_buffer, data, len);
            mflash_result = mflash_drv_page_program(PalFileContext->partition_phys_addr + data_offset, page_buffer);
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
    if ((retval > 0) && (PalFileContext->file_size < data_offset))
    {
        PalFileContext->file_size = data_offset;
    }

    return retval;
}


OtaPalStatus_t otaPal_ActivateNewImage(OtaFileContext_t * const C)
{
    OtaPalStatus_t result = OTA_PAL_COMBINE_ERR(OtaPalUninitialized, 0);

    LogInfo(("[OTA-NXP] ActivateNewImage"));
    otaPal_ResetDevice(C); /* go for reboot, this should never return */

    return result; /* to supress compiler warning and eventually report the unexpected state */
}


OtaPalStatus_t otaPal_ResetDevice(OtaFileContext_t * const C)
{
    LogInfo(("[OTA-NXP] SystemReset"));
    vTaskDelay(100 / portTICK_PERIOD_MS);
    NVIC_SystemReset(); /* this should never return */
}


OtaPalStatus_t otaPal_SetPlatformImageState(OtaFileContext_t * const C, OtaImageState_t eState)
{
    OtaPalStatus_t result = OTA_PAL_COMBINE_ERR(OtaPalSuccess, 0);

    LogDebug(("[OTA-NXP] SetPlatformImageState %d", eState));

    if (otaPal_GetPlatformImageState(C) == OtaPalImageStatePendingCommit)
    {
        /* Device in test mode */
        switch (eState)
        {
            case OtaImageStateAccepted:
                /* Request the bootloader to switch the image permanently */
                if (bl_update_image_state(0, kSwapType_Permanent) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = OTA_PAL_COMBINE_ERR(OtaPalCommitFailed, 0);
                }
                break;

            case OtaImageStateRejected:
                /* Invalidate the image */
                if (bl_update_image_state(0, kSwapType_Fail) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = OTA_PAL_COMBINE_ERR(OtaPalRejectFailed, 0);
                }
                break;

            case OtaImageStateAborted:
                /* Invalidate the image */
                if (bl_update_image_state(0, kSwapType_Fail) != kStatus_Success)
                {
                    /* Override result code by a state specific one */
                    result = OTA_PAL_COMBINE_ERR(OtaPalAbortFailed, 0);
                }
                break;

            case OtaImageStateTesting:
                result = OTA_PAL_COMBINE_ERR(OtaPalSuccess, 0);
                break;

            default:
                result = OTA_PAL_COMBINE_ERR(OtaPalBadImageState, 0);
                break;
        }
    }
    else
    {
        /* Normal mode */
        switch (eState)
        {
            case OtaImageStateAccepted:
                /* No pending commit */
                result = OTA_PAL_COMBINE_ERR(OtaPalCommitFailed, 1);
                break;

            case OtaImageStateRejected:
                result = OTA_PAL_COMBINE_ERR(OtaPalRejectFailed, 1);
                break;

            case OtaImageStateAborted:
                result = OTA_PAL_COMBINE_ERR(OtaPalAbortFailed, 1);
                break;

            case OtaImageStateTesting:
                result = OTA_PAL_COMBINE_ERR(OtaPalBadImageState, 1);
                break;

            default:
                result = OTA_PAL_COMBINE_ERR(OtaPalBadImageState, 1);
                break;
        }
    }

    return result;
}


OtaPalImageState_t otaPal_GetPlatformImageState(OtaFileContext_t * const C)
{
    uint32_t state;

    if (bl_get_image_state(0, &state) != kStatus_Success)
    {
        return OtaPalImageStateInvalid;
    }

    switch (state)
    {
        case kSwapType_ReadyForTest:
            return OtaPalImageStateValid;
            break;

        case kSwapType_Testing:
            return OtaPalImageStatePendingCommit;
            break;
    }

    return OtaPalImageStateInvalid;
}


/* Provide access to private members for testing. */
#ifdef AMAZON_FREERTOS_ENABLE_UNIT_TESTS
#include "aws_ota_pal_test_access_define.h"
#endif
