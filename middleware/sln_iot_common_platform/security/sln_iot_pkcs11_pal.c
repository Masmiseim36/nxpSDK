/*
 * Amazon FreeRTOS PKCS #11 PAL for LPC54018 IoT Module V1.0.3
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2018-2020 NXP
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


/**
 * @file iot_pkcs11_pal.c
 * @brief NXP LPC54018 IoT module file save and read implementation
 * for PKCS#11  based on mbedTLS with for software keys. This
 * file deviates from the FreeRTOS style standard for some function names and
 * data types in order to maintain compliance with the PKCS#11 standard.
 */

/* Board includes */
#include "board.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "FreeRTOSIPConfig.h"
#include "iot_crypto.h"
#include "task.h"
#include "iot_pkcs11.h"
#include "iot_pkcs11_config.h"

/* mbedTLS Includes */
#include "mbedtls/base64.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"

/* Flash write */
#include "sln_flash_mgmt.h"
#include "device_utils.h"

/* C runtime includes. */
#include <stdio.h>
#include <string.h>

#define pkcs11palFILE_NAME_CLIENT_CERTIFICATE    "cert.dat"
#define pkcs11palFILE_NAME_KEY                   "pkey.dat"
#define pkcs11palFILE_CODE_SIGN_PUBLIC_KEY       "FreeRTOS_P11_CodeSignKey.dat"

enum eObjectHandles
{
    eInvalidHandle = 0, /* According to PKCS #11 spec, 0 is never a valid object handle. */
    eAwsDevicePrivateKey = 1,
    eAwsDevicePublicKey,
    eAwsDeviceCertificate,
    eAwsCodeSigningKey
};

/*-----------------------------------------------------------*/

/* Converts a label to its respective filename and handle. */
void prvLabelToFilenameHandle( uint8_t * pcLabel,
                               char ** pcFileName,
                               CK_OBJECT_HANDLE_PTR pHandle )
{
    if( pcLabel != NULL )
    {
        /* Translate from the PKCS#11 label to local storage file name. */
        if( 0 == memcmp( pcLabel,
                         &pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
                         sizeof( pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ) ) )
        {
            *pcFileName = pkcs11palFILE_NAME_CLIENT_CERTIFICATE;
            *pHandle = eAwsDeviceCertificate;
        }
        else if( 0 == memcmp( pcLabel,
                              &pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
                              sizeof( pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ) ) )
        {
            *pcFileName = pkcs11palFILE_NAME_KEY;
            *pHandle = eAwsDevicePrivateKey;
        }
        else if( 0 == memcmp( pcLabel,
                              &pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
                              sizeof( pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS ) ) )
        {
            *pcFileName = pkcs11palFILE_NAME_KEY;
            *pHandle = eAwsDevicePublicKey;
        }
        else if( 0 == memcmp( pcLabel,
                              &pkcs11configLABEL_CODE_VERIFICATION_KEY,
                              sizeof( pkcs11configLABEL_CODE_VERIFICATION_KEY ) ) )
        {
            *pcFileName = pkcs11palFILE_CODE_SIGN_PUBLIC_KEY;
            *pHandle = eAwsCodeSigningKey;
        }
        else
        {
            *pcFileName = NULL;
            *pHandle = eInvalidHandle;
        }
    }
}

void PKCS11_KeyGen(char **keyPhrase)
{
    mbedtls_md_context_t mdCtx;
    const mbedtls_md_info_t *mdInfo;
    char *uniqueID = NULL;

    mbedtls_md_init(&mdCtx);

    volatile uint32_t salt1 = USB_ANALOG->DIGPROG;
    volatile uint32_t salt2 = (OCOTP->CFG3) | (1U << 22U);
    volatile uint64_t salt3 = (uint64_t)((uint64_t)salt1 << 32ULL) | salt2;

    char *textSalt      = NULL;
    char *outText       = NULL;
    uint32_t saltCdLen  = 0;
    uint32_t saltOutLen = 0;
    uint32_t keyCdLen   = 0;
    uint32_t keyOutLen  = 0;

    if (NULL == keyPhrase)
    {
        goto exit;
    }

    APP_GetUniqueID(&uniqueID, false);

    mdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if (mdInfo == NULL)
    {
        configPRINTF(("MBEDTLS_ERR_PKCS5_FEATURE_UNAVAILABLE\r\n"));
        goto exit;
    }

    if (mbedtls_md_setup(&mdCtx, mdInfo, 1) != 0)
    {
        configPRINTF(("Setup error\r\n"));
        goto exit;
    }

    /* Encode salt */
    mbedtls_base64_encode(NULL, 0, &saltCdLen, (const unsigned char *)&salt3, sizeof(uint64_t));
    textSalt = (char *)pvPortMalloc(saltCdLen + 1);
    if (NULL == textSalt)
    {
        goto exit;
    }
    memset(textSalt, 0, saltCdLen + 1);
    mbedtls_base64_encode((unsigned char *)textSalt, saltCdLen, &saltOutLen, (const unsigned char *)&salt3,
                          sizeof(uint64_t));

    /* Generate key phrase */
    outText = (char *)pvPortMalloc(32 + 1);
    if (NULL == outText)
    {
        goto exit;
    }
    mbedtls_pkcs5_pbkdf2_hmac(&mdCtx, (uint8_t *)uniqueID, strlen(uniqueID), (uint8_t *)textSalt, saltOutLen, 1000, 32,
                              (uint8_t *)outText);

    /* Encode key phrase */
    mbedtls_base64_encode(NULL, 0, &keyCdLen, (const unsigned char *)&outText, 32);
    *keyPhrase = (char *)pvPortMalloc(keyCdLen + 1);
    if (NULL == *keyPhrase)
    {
        goto exit;
    }
    memset(*keyPhrase, 0, keyCdLen + 1);
    mbedtls_base64_encode((unsigned char *)*keyPhrase, keyCdLen, &keyOutLen, (const unsigned char *)outText, 32);

exit:
	vPortFree(uniqueID);
    vPortFree(textSalt);
    vPortFree(outText);
    uniqueID = NULL;
    textSalt = NULL;
    outText  = NULL;
    mbedtls_md_free(&mdCtx);
}

/**
 * @brief Writes a file to local storage.
 *
 * Port-specific file write for crytographic information.
 *
 * @param[in] pxLabel       Label of the object to be saved.
 * @param[in] pucData       Data buffer to be written to file
 * @param[in] ulDataSize    Size (in bytes) of data to be saved.
 *
 * @return The file handle of the object that was stored.
 */
CK_OBJECT_HANDLE PKCS11_PAL_SaveObject( CK_ATTRIBUTE_PTR pxLabel,
                                        uint8_t * pucData,
                                        uint32_t ulDataSize )
{
    CK_OBJECT_HANDLE xHandle = eInvalidHandle;
    char * pcFileName = NULL;


    /* Translate from the PKCS#11 label to local storage file name. */
    prvLabelToFilenameHandle( pxLabel->pValue, &pcFileName, &xHandle );

    if( xHandle != eInvalidHandle )
    {
#if 0
        if( pdFALSE == mflash_save_file( pcFileName, pucData, ulDataSize ) )
        {
            xHandle = eInvalidHandle;
        }
#endif
    }

    return xHandle;
}

/*-----------------------------------------------------------*/

/**
 * @brief Translates a PKCS #11 label into an object handle.
 *
 * Port-specific object handle retrieval.
 *
 *
 * @param[in] pLabel         Pointer to the label of the object
 *                           who's handle should be found.
 * @param[in] usLength       The length of the label, in bytes.
 *
 * @return The object handle if operation was successful.
 * Returns eInvalidHandle if unsuccessful.
 */
CK_OBJECT_HANDLE PKCS11_PAL_FindObject( uint8_t * pLabel,
                                        uint8_t usLength )
{
    CK_OBJECT_HANDLE xHandle = eInvalidHandle;
    char * pcFileName = NULL;

    /* Translate from the PKCS#11 label to local storage file name. */
    prvLabelToFilenameHandle( pLabel, &pcFileName, &xHandle );

    /*TODO: check if file actually there.
     * Note: g_cert_files only seems to check if the entry in the array is present */

    return xHandle;
}


/*-----------------------------------------------------------*/

/**
 * @brief Gets the value of an object in storage, by handle.
 *
 * Port-specific file access for cryptographic information.
 *
 * This call dynamically allocates the buffer which object value
 * data is copied into.  PKCS11_PAL_GetObjectValueCleanup()
 * should be called after each use to free the dynamically allocated
 * buffer.
 *
 * @sa PKCS11_PAL_GetObjectValueCleanup
 *
 * @param[in] pcFileName    The name of the file to be read.
 * @param[out] ppucData     Pointer to buffer for file data.
 * @param[out] pulDataSize  Size (in bytes) of data located in file.
 * @param[out] pIsPrivate   Boolean indicating if value is private (CK_TRUE)
 *                          or exportable (CK_FALSE)
 *
 * @return CKR_OK if operation was successful.  CKR_KEY_HANDLE_INVALID if
 * no such object handle was found, CKR_DEVICE_MEMORY if memory for
 * buffer could not be allocated, CKR_FUNCTION_FAILED for device driver
 * error.
 */
CK_RV PKCS11_PAL_GetObjectValue( CK_OBJECT_HANDLE xHandle,
                                 uint8_t ** ppucData,
                                 uint32_t * pulDataSize,
                                 CK_BBOOL * pIsPrivate )
{
    char * pcFileName = NULL;
    CK_RV ulReturn = CKR_OK;
    sln_flash_mgmt_status_t ulFlashStatus = SLN_FLASH_MGMT_OK;

    if( xHandle == eAwsDeviceCertificate )
    {
        pcFileName = pkcs11palFILE_NAME_CLIENT_CERTIFICATE;
        *pIsPrivate = CK_FALSE;
    }
    else if( xHandle == eAwsDevicePrivateKey )
    {
        pcFileName = pkcs11palFILE_NAME_KEY;
        *pIsPrivate = CK_TRUE;
    }
    else if( xHandle == eAwsDevicePublicKey )
    {
        /* Public and private key are stored together in same file. */
        pcFileName = pkcs11palFILE_NAME_KEY;
        *pIsPrivate = CK_FALSE;
    }
    else if( xHandle == eAwsCodeSigningKey )
    {
        pcFileName = pkcs11palFILE_CODE_SIGN_PUBLIC_KEY;
        *pIsPrivate = CK_FALSE;
    }
    else
    {
        ulReturn = CKR_KEY_HANDLE_INVALID;
    }

    /* Get size of file in NVM */
    if (CKR_OK == ulReturn)
	{
    	ulFlashStatus = SLN_FLASH_MGMT_Read(pcFileName, NULL, pulDataSize);

    	if (SLN_FLASH_MGMT_OK != ulFlashStatus)
    	{
    		ulReturn = CKR_FUNCTION_FAILED;
    	}
	}

    /* Allocate appropriate memory */
    if (CKR_OK == ulReturn)
    {
    	/* We will de-allocate and clean-up in PKCS11_PAL_GetObjectValueCleanup */
    	*ppucData = (uint8_t *)pvPortMalloc(*pulDataSize + 1);

        if (NULL == *ppucData)
        {
        	ulReturn = CKR_FUNCTION_FAILED;
        }
        else
        {
        	/* Wipe out memory here */
        	memset(*ppucData, 0x00, *pulDataSize);
        }
    }

    /* Copy memory from NVM to RAM */
    if (CKR_OK == ulReturn)
	{
    	ulFlashStatus = SLN_FLASH_MGMT_Read(pcFileName, *ppucData, pulDataSize);

    	if (SLN_FLASH_MGMT_OK != ulFlashStatus)
    	{
    		ulReturn = CKR_FUNCTION_FAILED;
    	}
	}

    /* Clean-up if we failed somewhere above */
    if (CKR_FUNCTION_FAILED == ulReturn)
    {
    	if (NULL != *ppucData)
    	{
    		memset(*ppucData, 0x00, *pulDataSize);
    	}
    	vPortFree(*ppucData);
    	*ppucData = NULL;
    }

    return ulReturn;
}

/**
 * @brief Cleanup after PKCS11_GetObjectValue().
 *
 * @param[in] pucData       The buffer to free.
 *                          (*ppucData from PKCS11_PAL_GetObjectValue())
 * @param[in] ulDataSize    The length of the buffer to free.
 *                          (*pulDataSize from PKCS11_PAL_GetObjectValue())
 */
void PKCS11_PAL_GetObjectValueCleanup( uint8_t * pucData,
                                       uint32_t ulDataSize )
{

	if (NULL != pucData)
	{
		memset(pucData, 0x00, ulDataSize);
	}
	vPortFree(pucData);
	pucData = NULL;

}


/**
 *      PKCS#11 Override
 *
 */

extern CK_RV prvMbedTLS_Initialize( void );

/**
 * @brief Initialize the Cryptoki module for use.
 *
 * Overrides the implementation of C_Initialize in
 * iot_pkcs11_mbedtls.c when pkcs11configC_INITIALIZE_ALT
 * is defined.
 */
#ifndef pkcs11configC_INITIALIZE_ALT
    #error LPC54018 requires alternate C_Initialization
#endif

CK_DEFINE_FUNCTION( CK_RV, C_Initialize )( CK_VOID_PTR pvInitArgs )
{   /*lint !e9072 It's OK to have different parameter name. */
    ( void ) ( pvInitArgs );

    CK_RV xResult = CKR_OK;

#if 0
    if( !mflash_is_initialized() )
    {
        /* Initialize flash storage. */
        if( pdTRUE != mflash_init( g_cert_files, 1 ) )
        {
            xResult = CKR_GENERAL_ERROR;
        }
    }
#endif

    if( xResult == CKR_OK )
    {
        xResult = prvMbedTLS_Initialize();
    }

    return xResult;
}
