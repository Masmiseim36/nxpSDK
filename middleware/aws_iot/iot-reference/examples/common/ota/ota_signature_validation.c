/*
 * FreeRTOS version 202107.00-LTS
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
#include "FreeRTOS.h"
#include "task.h"
#include "ota_config.h"

#include "fsl_debug_console.h"
#include "ota_pal.h"
#include "core_pkcs11.h"
#include "core_pki_utils.h"
#include "mbedtls/sha256.h"

/**
 * @brief The crypto algorithm used for the digital signature.
 */
#define CRYPTO_ALGORITHM    cryptoASYMMETRIC_ALGORITHM_ECDSA

/**
 * @brief The signature method used for calculating the signature.
 */
#define SIGNATURE_METHOD    cryptoHASH_ALGORITHM_SHA256

/**
 * @brief Opens a PKCS11 Session.
 *
 * @param[out] xSession Handle to the session opened.
 * @return CKR_OK if successful.
 */
static CK_RV prvOpenPKCS11Session( CK_SESSION_HANDLE_PTR pSession );

/**
 * @brief Closes a PKCS11 Session.
 *
 * @param[out] xSession Session to be closed.
 * @return CKR_OK if successful.
 */
static CK_RV prvClosePKCS11Session( CK_SESSION_HANDLE xSession );

/**
 * @brief Gets the handle for a certificate stored in a PKCS11 slot.
 *
 * @param[in] xSession PKCS11 session being opened.
 * @param[in] pcLabelName String containing the label name for the certificate slot.
 * @param[out] pxCertHandle The handle for the certificate slot.
 * @return CKR_OK if successful
 */
static CK_RV prvPKCS11GetCertificateHandle( CK_SESSION_HANDLE xSession,
                                            const char * pcLabelName,
                                            CK_OBJECT_HANDLE_PTR pxCertHandle );


/**
 * @brief Verifies the firmware image signature using PKCS11 APIs.
 * Uses PKCS11 SHA256 hash APIS to calculate running checksum of the image and  verify
 * the signature using the certificate handle stored in a PKCS11 slot.
 *
 * @param[in] session PKCS11 session handle being opened.
 * @param[in] certificateHandle Certificate handle used for signature validation.
 * @param[in] pSignature Signature as received from OTA library.
 * @param[in] signatureLength Length of the signature.
 * @return CKR_OK if the firmware image is valid.
 */
CK_RV xVerifyImageSignatureUsingPKCS11( CK_SESSION_HANDLE session,
                                        CK_OBJECT_HANDLE certificateHandle,
                                        uint8_t * pData,
                                        size_t length,
                                        uint8_t * pSignature,
                                        size_t signatureLength );

static CK_RV prvPKCS11GetCertificateHandle( CK_SESSION_HANDLE xSession,
                                            const char * pcLabelName,
                                            CK_OBJECT_HANDLE_PTR pxCertHandle )
{
    CK_ATTRIBUTE xTemplate;
    CK_RV xResult = CKR_OK;
    CK_ULONG ulCount = 0;
    CK_BBOOL xFindInit = CK_FALSE;
    CK_FUNCTION_LIST_PTR xFunctionList;

    xResult = C_GetFunctionList( &xFunctionList );

    /* Get the certificate handle. */
    if( CKR_OK == xResult )
    {
        xTemplate.type = CKA_LABEL;
        xTemplate.ulValueLen = strlen( pcLabelName ) + 1;
        xTemplate.pValue = ( char * ) pcLabelName;
        xResult = xFunctionList->C_FindObjectsInit( xSession, &xTemplate, 1 );
    }

    if( CKR_OK == xResult )
    {
        xFindInit = CK_TRUE;
        xResult = xFunctionList->C_FindObjects( xSession,
                                                ( CK_OBJECT_HANDLE_PTR ) pxCertHandle,
                                                1,
                                                &ulCount );
    }

    if( ( CK_TRUE == xFindInit ) && ( xResult == 0 ) )
    {
        xResult = xFunctionList->C_FindObjectsFinal( xSession );
    }

    return xResult;
}

static CK_RV prvOpenPKCS11Session( CK_SESSION_HANDLE_PTR pSession )
{
    /* Find the certificate */
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR xFunctionList;
    CK_SLOT_ID xSlotId;
    CK_ULONG xCount = 1;

    xResult = C_GetFunctionList( &xFunctionList );

    if( CKR_OK == xResult )
    {
        xResult = xFunctionList->C_Initialize( NULL );
    }

    if( ( CKR_OK == xResult ) || ( CKR_CRYPTOKI_ALREADY_INITIALIZED == xResult ) )
    {
        xResult = xFunctionList->C_GetSlotList( CK_TRUE, &xSlotId, &xCount );
    }

    if( CKR_OK == xResult )
    {
        xResult = xFunctionList->C_OpenSession( xSlotId, CKF_SERIAL_SESSION, NULL, NULL, pSession );
    }

    return xResult;
}

static CK_RV prvClosePKCS11Session( CK_SESSION_HANDLE xSession )
{
    CK_RV xResult = CKR_OK;
    CK_FUNCTION_LIST_PTR xFunctionList;

    xResult = C_GetFunctionList( &xFunctionList );

    if( xResult == CKR_OK )
    {
        xResult = xFunctionList->C_CloseSession( xSession );
    }

    return xResult;
}

CK_RV xVerifyImageSignatureUsingPKCS11( CK_SESSION_HANDLE session,
                                        CK_OBJECT_HANDLE certificateHandle,
                                        uint8_t * pData,
                                        size_t length,
                                        uint8_t * pSignature,
                                        size_t signatureLength )

{
    /* The ECDSA mechanism will be used to verify the message digest. */
    CK_MECHANISM xVerifyMechanism = { CKM_ECDSA, NULL, 0 };

    mbedtls_sha256_context xSHA256Context = { 0 };

    /* The buffer used to hold the calculated SHA25 digest of the image. */
    CK_BYTE digestResult[ pkcs11SHA256_DIGEST_LENGTH ] = { 0 };

    CK_RV result = CKR_OK;

    int xMbedtlsRet = 0;

    CK_FUNCTION_LIST_PTR functionList;

    mbedtls_sha256_init( &xSHA256Context );


    result = C_GetFunctionList( &functionList );
    configASSERT( result == CKR_OK );

    if( ( xMbedtlsRet = mbedtls_sha256_starts_ret( &xSHA256Context, 0 ) ) != 0 )
    {
        LogError( "Failed to start mbedtls digest update, error = %d", xMbedtlsRet );
        result = CKR_FUNCTION_FAILED;
    }

    if( result == CKR_OK )
    {
        if( ( xMbedtlsRet = mbedtls_sha256_update_ret( &xSHA256Context, pData, length ) ) != 0 )
        {
            LogError( "Failed to create mbedtls digest, error = %d", xMbedtlsRet );
            result = CKR_FUNCTION_FAILED;
        }
    }

    if( result == CKR_OK )
    {
        if( ( xMbedtlsRet = mbedtls_sha256_finish_ret( &xSHA256Context, digestResult ) ) != 0 )
        {
            LogError( "Failed to finalize mbedtls digest, error = %d", xMbedtlsRet );
            result = CKR_FUNCTION_FAILED;
        }
    }

    if( result == CKR_OK )
    {
        result = functionList->C_VerifyInit( session,
                                             &xVerifyMechanism,
                                             certificateHandle );
    }

    if( result == CKR_OK )
    {
        result = functionList->C_Verify( session,
                                         digestResult,
                                         pkcs11SHA256_DIGEST_LENGTH,
                                         pSignature,
                                         signatureLength );
    }

    return result;
}



OtaPalStatus_t xFlashPalValidateSignature( uint8_t * pMappedAddress,
                                           size_t mappedLength,
                                           char * pCertificatePath,
                                           uint8_t * pSignature,
                                           size_t signatureLength )
{
    OtaPalStatus_t status = OtaPalSuccess;
    CK_SESSION_HANDLE session = CKR_SESSION_HANDLE_INVALID;
    CK_RV xPKCS11Status = CKR_OK;
    CK_OBJECT_HANDLE certHandle;
    uint8_t pkcs11Signature[ pkcs11ECDSA_P256_SIGNATURE_LENGTH ] = { 0 };

    if( PKI_mbedTLSSignatureToPkcs11Signature( pkcs11Signature, pSignature ) != 0 )
    {
        LogError( "Cannot convert signature to PKCS11 format." );
        status = OtaPalSignatureCheckFailed;
    }

    if( status == OtaPalSuccess )
    {
        xPKCS11Status = prvOpenPKCS11Session( &session );

        if( xPKCS11Status == CKR_OK )
        {
            xPKCS11Status = prvPKCS11GetCertificateHandle( session, pCertificatePath, &certHandle );
        }

        if( xPKCS11Status == CKR_OK )
        {
            xPKCS11Status = xVerifyImageSignatureUsingPKCS11( session,
                                                              certHandle,
                                                              pMappedAddress,
                                                              mappedLength,
                                                              pkcs11Signature,
                                                              pkcs11ECDSA_P256_SIGNATURE_LENGTH );
        }

        if( xPKCS11Status != CKR_OK )
        {
            LogError( "Image verification failed with PKCS11 status %d", xPKCS11Status );
            status = OtaPalSignatureCheckFailed;
        }
    }

    if( session != CKR_SESSION_HANDLE_INVALID )
    {
        ( void ) prvClosePKCS11Session( session );
    }

    return status;
}
