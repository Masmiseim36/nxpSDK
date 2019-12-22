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

#include "ota_bootloader_supp.h"

/* Specify the OTA signature algorithm we support on this platform. */
const char cOTA_JSON_FileSignatureKey[OTA_FILE_SIG_KEY_STR_MAX_LENGTH] = "sig-sha256-ecdsa";


/* low level file context structure */
typedef struct
{
    const OTA_FileContext_t *FileXRef;
    uint32_t Addr;
    uint32_t Size;
    uint32_t MaxSize;
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
    partition_t update_partition;
    LL_FileContext_t *FileContext = &prvPAL_CurrentFileContext;

    OTA_LOG_L1("[OTA-NXP] CreateFileForRx\r\n");

    if (bl_get_update_partition_info(&update_partition) != kStatus_Success)
    {
        return kOTA_Err_RxFileCreateFailed;
    }

    if (C->ulFileSize > update_partition.size)
    {
        return kOTA_Err_RxFileTooLarge;
    }

    /* update partition address in FLASH memory and its size */
    FileContext->Addr = update_partition.start - FlexSPI_AMBA_BASE;
    FileContext->MaxSize = update_partition.size;

    /* actual size of the file according to data received */
    FileContext->Size     = 0;

    FileContext->FileXRef = C; /* pointer cross reference for integrity check */
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
        return result;
    }

    FileData = mflash_drv_mmap(FileContext->Addr, FileContext->Size);
    if (FileData == NULL)
    {
        return kOTA_Err_BootInfoCreateFailed;
    }

#if OTA_DEVEL_MODE
    /* Fixes image header required by ota_bootloader by setting the length and generating the proper checksum.
     * This is to provide easy way of testing/debugging of OTA enabled application.
     * Should not be used in production environment! */     
    if (((boot_image_header_t *)FileData)->image_size == 0)
    {
        /* The image header declares zero image size, so it is a dummy header - update it */
        boot_image_header_t bih;

        memcpy(&bih, FileData, sizeof(bih));
        bih.image_size  = FileContext->Size - bih.header_size;
        bih.checksum[0] = bl_crc32(FileData + bih.header_size, bih.image_size);
        bih.algorithm   = IMG_CHK_ALG_CRC32;

        /* Write boot image header back to FLASH */
        if (mflash_drv_write(FileContext->Addr, (uint8_t *)&bih, sizeof(bih)) != 0)
        {
            result = kOTA_Err_BootInfoCreateFailed;
        }
    }
    else /* ... */
#endif

    /* Sanity check of the image and its header */
    if (bl_verify_image(FileData, FileContext->Size) <= 0)
    {
        result = kOTA_Err_BootInfoCreateFailed;
    }

    /* Prepare image to be booted in test mode */
    if ((result == kOTA_Err_None) && (bl_update_image_state(kSwapType_ReadyForTest) != kStatus_Success))
    {
        result = kOTA_Err_BootInfoCreateFailed;
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

    if (ulOffset + ulBlockSize > FileContext->MaxSize)
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
