/*
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (c) 2019-2020 Arm Limited. All Rights Reserved.
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
 */

/*
 * This file is derivative of amazon-freertos\vendors\st\boards\stm32l475_discovery\
 * ports\pkcs11\iot_pkcs11_pal.c(amazon-freertos commit 74875b1d2)
 */

/**
 * @file iot_pkcs11_psa_object_management.c
 * @brief Functions of managing the objects stored in TF-M.
 */

#include <string.h>
#include "iot_pkcs11_psa_object_management.h"
#include "iot_pkcs11_psa_input_format.h"

/*
 * This is the context of the PKCS#11 PSA object. It is placed in a section.
 * named "tasks_share". If MPU is enabled, tasks that call PKCS#11 APIs should
 * be configured to have access to this region.
 */
P11KeyConfig_t P11KeyConfig __attribute__(( section( "tasks_share" ) ));

/**
* @brief Writes a file to local storage.
*
* Port-specific file write for crytographic information.
*
* @param[in] pxClass       Class of the object to be saved.
* @param[in] pxLabel       Label of the object to be saved.
* @param[in] pucData       Data buffer to be written to file
* @param[in] ulDataSize    Size (in bytes) of data to be saved.
* @param[in] pvContext     Context of the key.
*
* @return The file handle of the object that was stored.
*/
CK_OBJECT_HANDLE PKCS11PSASaveObject( CK_ATTRIBUTE_PTR pxClass,
    CK_ATTRIBUTE_PTR pxLabel,
    uint8_t * pucData,
    uint32_t ulDataSize,
    mbedtls_pk_context *pvContext )
{
    CK_OBJECT_HANDLE xHandle = eInvalidHandle;
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    CK_ULONG uxKeyType;
    psa_key_handle_t uxKeyHandle;
    psa_algorithm_t uxAlgorithm;
    CK_RV xBytesWritten = 0;
    CK_RV xReturn;
    psa_status_t uxStatus = PSA_SUCCESS;
    psa_ecc_curve_t curve_id;
    const mbedtls_ecp_keypair *ec;
    unsigned char cPrivateKeyRaw[EC_PRIVATE_KEY_MAX_LENGTH];
    size_t xPrivateKeyRawSize = 0;
    unsigned char *pcPublicKeyUncompressedData = NULL;
    size_t xPublicKeySizeUncompressed = 0;
    uint8_t * pucKeyData = NULL;
    uint32_t ulKeyDataSize = 0;
    mbedtls_pk_type_t uxPrivateKeyTypePKCS11 = 0;

    if( ulDataSize <= pkcs11OBJECT_MAX_SIZE )
    {
        /*
         * write client certificate.
         */
        if( ( *( ( uint32_t * )pxClass->pValue ) == CKO_CERTIFICATE ) &&
            ( strcmp( pxLabel->pValue,
                    pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ) == 0 ) )
        {
            uxStatus = psa_ps_set( PSA_DEVICE_CERTIFICATE_UID,
                                   ulDataSize,
                                   pucData,
                                   ( psa_storage_create_flags_t ) PSA_STORAGE_FLAG_NONE );

            if( uxStatus == PSA_SUCCESS )
            {
                xHandle = eAwsDeviceCertificate;

                /* Import the object into P11KeyConfig context. */
                PKCS11PSAContextImportObject( pxLabel->pValue,
                                              strlen( ( const char * ) pxLabel->pValue ),
                                              pkcs11OBJECT_HANDLE_NA );
            }
        }

        /*
         * write Jitp certificate.
         */
        else if( ( *( ( uint32_t * )pxClass->pValue ) == CKO_CERTIFICATE ) &&
            ( strcmp( pxLabel->pValue,
                    pkcs11configLABEL_JITP_CERTIFICATE ) == 0 ) )
        {
            uxStatus = psa_ps_set( PSA_JITP_CERTIFICATE_UID,
                                   ulDataSize,
                                   pucData,
                                   ( psa_storage_create_flags_t ) PSA_STORAGE_FLAG_NONE );

            if( uxStatus == PSA_SUCCESS )
            {
                xHandle = eAwsJitpCertificate;

                /* Import the object into P11KeyConfig context. */
                PKCS11PSAContextImportObject( pxLabel->pValue,
                                              strlen( ( const char * ) pxLabel->pValue ),
                                              pkcs11OBJECT_HANDLE_NA );
            }
        }

        /*
         * write Root certificate.
         */
        else if( ( *( ( uint32_t * )pxClass->pValue ) == CKO_CERTIFICATE ) &&
            ( strcmp( pxLabel->pValue,
                    pkcs11configLABEL_ROOT_CERTIFICATE ) == 0 ) )
        {
            uxStatus = psa_ps_set( PSA_ROOT_CERTIFICATE_UID,
                                   ulDataSize,
                                   pucData,
                                   ( psa_storage_create_flags_t ) PSA_STORAGE_FLAG_NONE );

            if( uxStatus == PSA_SUCCESS )
            {
                xHandle = eAwsRootCertificate;

                /* Import the object into P11KeyConfig context. */
                PKCS11PSAContextImportObject( pxLabel->pValue,
                                              strlen( ( const char * ) pxLabel->pValue ),
                                              pkcs11OBJECT_HANDLE_NA );
            }
        }

        /*
         * Store device private key by crypto service of PSA.
         */

        else if( ( *( ( uint32_t * )pxClass->pValue ) == CKO_PRIVATE_KEY ) &&
            ( strcmp( pxLabel->pValue,
                    pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ) == 0 ) )
        {
            /*
             * Device private key is only used to make a signature,
             * only support RSA alg, only support MBEDTLS_RSA_PKCS_V15
             * padding mode, only support SHA256 md alg.
             */
            uxPrivateKeyTypePKCS11 = mbedtls_pk_get_type( pvContext );
            switch ( uxPrivateKeyTypePKCS11 )
            {
                case MBEDTLS_PK_RSA:
                    uxKeyType = PSA_KEY_TYPE_RSA_KEY_PAIR;
                    pucKeyData = pucData;
                    ulKeyDataSize = ulDataSize;
                    switch ( ( ( mbedtls_rsa_context * ) ( pvContext->pk_ctx ) )->padding )
                    {
                        case MBEDTLS_RSA_PKCS_V15:
                            uxAlgorithm = PSA_ALG_RSA_PKCS1V15_SIGN( PSA_ALG_SHA_256 );
                            break;

                        case MBEDTLS_RSA_PKCS_V21:
                            uxAlgorithm = PSA_ALG_RSA_PSS( PSA_ALG_SHA_256 );
                            break;

                        default:
                            uxAlgorithm = 0;
                            uxStatus = PSA_ERROR_INVALID_ARGUMENT;
                            break;
                    }
                    break;

                case MBEDTLS_PK_ECKEY:
                case MBEDTLS_PK_ECDSA:
                    ec = (mbedtls_ecp_keypair *) (pvContext->pk_ctx );
                    curve_id = mbedtls_ecp_curve_info_from_grp_id( ec->grp.id )->tls_id;
                    uxKeyType = PSA_KEY_TYPE_ECC_KEY_PAIR(curve_id);
                    uxAlgorithm = PSA_ALG_ECDSA( PSA_ALG_SHA_256 );
                    xPrivateKeyRawSize = ( ec->grp.nbits + 7 ) / 8;
                    if( 0 != mbedtls_mpi_write_binary( &ec->d,
                                                        cPrivateKeyRaw,
                                                        xPrivateKeyRawSize ) )
                    {
                        uxStatus = PSA_ERROR_GENERIC_ERROR;
                    }
                    pucKeyData = cPrivateKeyRaw;
                    ulKeyDataSize = xPrivateKeyRawSize;
                    break;
                default:
                    uxAlgorithm = 0;
                    uxKeyType = 0;
                    uxStatus = PSA_ERROR_INVALID_ARGUMENT;
                    break;
            }
            if ( uxStatus == PSA_SUCCESS )
            {
                /* Device private key is saved as persistent key. */
                psa_set_key_id( &key_attributes, PSA_DEVICE_PRIVATE_KEY_ID );
                psa_set_key_usage_flags( &key_attributes, PSA_KEY_USAGE_SIGN );
                psa_set_key_algorithm( &key_attributes, uxAlgorithm );
                psa_set_key_type( &key_attributes, uxKeyType );
                uxStatus = psa_import_key( &key_attributes,
                                           ( const uint8_t * )pucKeyData,
                                           ulKeyDataSize,
                                           &uxKeyHandle );
            }
            if ( uxStatus == PSA_SUCCESS )
            {
                xHandle = eAwsDevicePrivateKey;

                /* Import the object into P11KeyConfig context. */
                PKCS11PSAContextImportObject( pxLabel->pValue,
                                              strlen( ( const char * ) pxLabel->pValue ),
                                              uxKeyHandle );
            }
        }
        else if( ( *( ( uint32_t * )pxClass->pValue ) == CKO_PUBLIC_KEY ) &&
            ( strcmp( pxLabel->pValue,
                    pkcs11configLABEL_CODE_VERIFICATION_KEY ) == 0 ) )
        {
            /*
             * Code verify key is only used to make a verify,
             * only SHA256 md alg.
             */
            switch ( mbedtls_pk_get_type( pvContext ) )
            {
                case MBEDTLS_PK_RSA:
                    /**
                     * The RSA private key should contain the public key. So it should not go here.
                     */
                    uxStatus = PSA_ERROR_INVALID_ARGUMENT;
                    break;

                case MBEDTLS_PK_ECKEY:
                case MBEDTLS_PK_ECDSA:
                    ec = (mbedtls_ecp_keypair *) (pvContext->pk_ctx );
                    curve_id = mbedtls_ecp_curve_info_from_grp_id( ec->grp.id )->tls_id;
                    uxKeyType = PSA_KEY_TYPE_ECC_PUBLIC_KEY(curve_id);
                    uxAlgorithm = PSA_ALG_ECDSA( PSA_ALG_SHA_256 );
                    if( 0 !=get_public_key_ECPoint( pucData,
                                                    ulDataSize,
                                                    &pcPublicKeyUncompressedData,
                                                    &xPublicKeySizeUncompressed ) )
                    {
                        uxStatus = PSA_ERROR_GENERIC_ERROR;
                    }

                    pucKeyData = pcPublicKeyUncompressedData;
                    ulKeyDataSize = xPublicKeySizeUncompressed;
                    break;
                default:
                    uxAlgorithm = 0;
                    uxStatus = PSA_ERROR_INVALID_ARGUMENT;
                    break;
            }
            if ( uxStatus == PSA_SUCCESS )
            {
                psa_set_key_usage_flags( &key_attributes, PSA_KEY_USAGE_VERIFY );
                psa_set_key_algorithm( &key_attributes, uxAlgorithm );
                psa_set_key_type( &key_attributes, uxKeyType );
                uxStatus = psa_import_key( &key_attributes,
                                           ( const uint8_t * )pucKeyData,
                                           ulKeyDataSize,
                                           &uxKeyHandle );
            }
            if ( uxStatus == PSA_SUCCESS )
            {
                xHandle = eAwsCodeSigningKey;

                /* Import the object into P11KeyConfig context. */
                PKCS11PSAContextImportObject( pxLabel->pValue,
                                              strlen( ( const char * ) pxLabel->pValue ),
                                              uxKeyHandle );
            }
        }
        else if( ( *( ( uint32_t * )pxClass->pValue ) == CKO_PUBLIC_KEY ) &&
            ( strcmp( pxLabel->pValue,
                    pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS ) == 0 ) )
        {
            /*
             * Code verify key is only used to make a verify,
             * only SHA256 md alg.
             */
            switch ( mbedtls_pk_get_type( pvContext ) )
            {
                case MBEDTLS_PK_RSA:
                    uxStatus = PSA_ERROR_INVALID_ARGUMENT;
                    break;

                case MBEDTLS_PK_ECKEY:
                case MBEDTLS_PK_ECDSA:
                    ec = (mbedtls_ecp_keypair *) (pvContext->pk_ctx );
                    curve_id = mbedtls_ecp_curve_info_from_grp_id( ec->grp.id )->tls_id;
                    uxKeyType = PSA_KEY_TYPE_ECC_PUBLIC_KEY(curve_id);
                    uxAlgorithm = PSA_ALG_ECDSA( PSA_ALG_SHA_256 );
                    if( 0 !=get_public_key_ECPoint( pucData,
                                                    ulDataSize,
                                                    &pcPublicKeyUncompressedData,
                                                    &xPublicKeySizeUncompressed ) )
                    {
                        uxStatus = PSA_ERROR_GENERIC_ERROR;
                    }
                    pucKeyData = pcPublicKeyUncompressedData;
                    ulKeyDataSize = xPublicKeySizeUncompressed;
                    break;
                default:
                    uxStatus = PSA_ERROR_INVALID_ARGUMENT;
                    uxAlgorithm = 0;
                    break;
            }
            if( uxStatus == PSA_SUCCESS )
            {
                /* Device public key is saved as persistent key. */
                psa_set_key_id( &key_attributes, PSA_DEVICE_PUBLIC_KEY_ID );
                psa_set_key_usage_flags( &key_attributes, PSA_KEY_USAGE_VERIFY );
                psa_set_key_algorithm( &key_attributes, uxAlgorithm );
                psa_set_key_type( &key_attributes, uxKeyType );
                uxStatus = psa_import_key( &key_attributes,
                                           ( const uint8_t * )pucKeyData,
                                           ulKeyDataSize,
                                           &uxKeyHandle );

            }
            if ( uxStatus == PSA_SUCCESS )
            {
                xHandle = eAwsDevicePublicKey;

                /* Import the object into P11KeyConfig context. */
                PKCS11PSAContextImportObject( pxLabel->pValue,
                                              strlen( ( const char * ) pxLabel->pValue ),
                                              uxKeyHandle );
            }
        }
    }

    return xHandle;
}

/**
* @brief Gets the value of an object in storage, by handle.
*
* Port-specific file access for cryptographic information.
*
* This call dynamically allocates the buffer which object value
* data is copied into.  PKCS11PSAGetObjectValueCleanup()
* should be called after each use to free the dynamically allocated
* buffer.
*
* @sa PKCS11PSAGetObjectValueCleanup
*
* @param[in] pcFileName    The name of the file to be read.
* @param[out] pucData     Pointer to buffer for file data.
* @param[out] pulDataSize  Size (in bytes) of data located in file.
* @param[out] pIsPrivate   Boolean indicating if value is private (CK_TRUE)
*                          or exportable (CK_FALSE)
*
* @return CKR_OK if operation was successful.  CKR_KEY_HANDLE_INVALID if
* no such object handle was found, CKR_DEVICE_MEMORY if memory for
* buffer could not be allocated, CKR_FUNCTION_FAILED for device driver
* error.
*/
CK_RV PKCS11PSAGetObjectValue( CK_OBJECT_HANDLE xHandle,
    uint8_t * pucData,
    uint32_t * pulDataSize,
    CK_BBOOL * pIsPrivate )
{
    CK_RV ulReturn = CKR_OBJECT_HANDLE_INVALID;
    uint32_t ulDataSize = 0;
    psa_status_t uxStatus;
    struct psa_storage_info_t info = {0};
    psa_key_type_t key_type;
    size_t key_bits;
    size_t buffer_size;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

    /*
     * Read client certificate.
     */
    if( xHandle == eAwsDeviceCertificate )
    {
        /*
         * return reference and size only if certificates are present in the device.
         */
        if( P11KeyConfig.xDeviceCertificateMark == pdTRUE )
        {
            /* Get the size of the data associated with the certificate UID firstly. */
            uxStatus = psa_ps_get_info( PSA_DEVICE_CERTIFICATE_UID, &info );
            if( uxStatus == PSA_SUCCESS )
            {
                uxStatus = psa_ps_get( PSA_DEVICE_CERTIFICATE_UID, 0, info.size, pucData, &ulDataSize );
                if( uxStatus == PSA_SUCCESS )
                {
                    *pulDataSize = ulDataSize;
                    *pIsPrivate = CK_FALSE;
                    ulReturn = CKR_OK;
                }
                else
                {
                    ulReturn = CKR_FUNCTION_FAILED;
                }
            }
            else
            {
                ulReturn = CKR_FUNCTION_FAILED;
            }
        }
    }

    /*
     * Read Jitp certificate.
     */
    if( xHandle == eAwsJitpCertificate )
    {
        /*
         * return reference and size only if certificates are present in the device.
         */
        if( P11KeyConfig.xJitpCertificateMark == pdTRUE )
        {
            /* Get the size of the data associated with the certificate UID firstly. */
            uxStatus = psa_ps_get_info( PSA_JITP_CERTIFICATE_UID, &info );
            if( uxStatus == PSA_SUCCESS )
            {
                uxStatus = psa_ps_get( PSA_JITP_CERTIFICATE_UID, 0, info.size, pucData, &ulDataSize );
                if( uxStatus == PSA_SUCCESS )
                {
                    *pulDataSize = ulDataSize;
                    *pIsPrivate = CK_FALSE;
                    ulReturn = CKR_OK;
                }
                else
                {
                    ulReturn = CKR_FUNCTION_FAILED;
                }
            }
            else
            {
                ulReturn = CKR_FUNCTION_FAILED;
            }
        }
    }

    /*
     * Read Root certificate.
     */
    if( xHandle == eAwsRootCertificate )
    {
        /*
         * return reference and size only if certificates are present in the device.
         */
        if( P11KeyConfig.xRootCertificateMark  == pdTRUE )
        {
            /* Get the size of the data associated with the certificate UID firstly. */
            uxStatus = psa_ps_get_info( PSA_ROOT_CERTIFICATE_UID, &info );
            if( uxStatus == PSA_SUCCESS )
            {
                uxStatus = psa_ps_get( PSA_ROOT_CERTIFICATE_UID, 0, info.size, pucData, &ulDataSize );
                if( uxStatus == PSA_SUCCESS )
                {
                    *pulDataSize = ulDataSize;
                    *pIsPrivate = CK_FALSE;
                    ulReturn = CKR_OK;
                }
                else
                {
                    ulReturn = CKR_FUNCTION_FAILED;
                }
            }
            else
            {
                ulReturn = CKR_FUNCTION_FAILED;
            }
        }
    }

    /*
     * Read client key.
     */
    else if( xHandle == eAwsDevicePrivateKey )
    {
        /*
         * return reference and size only if key is present in the device and is not private
         */
        if( P11KeyConfig.xDevicePrivateKeyMark == pdTRUE )
        {
            uxStatus = psa_get_key_attributes( P11KeyConfig.uxDevicePrivateKey, &attributes );
            if( uxStatus == PSA_SUCCESS )
            {
                key_type = psa_get_key_type( &attributes );
                key_bits = psa_get_key_bits( &attributes );
                buffer_size = PSA_KEY_EXPORT_MAX_SIZE( key_type, key_bits );
                uxStatus = psa_export_key( P11KeyConfig.uxDevicePrivateKey, pucData, buffer_size, pulDataSize );
                if ( uxStatus == PSA_ERROR_NOT_PERMITTED )
                {
                    *pIsPrivate = CK_TRUE;
                    ulReturn = CKR_OK;
                }
                else if ( uxStatus == PSA_SUCCESS )
                {
                    *pIsPrivate = CK_FALSE;
                    ulReturn = CKR_OK;
                }
                else
                {
                    ulReturn = CKR_FUNCTION_FAILED;
                }
            }
            else
            {
                ulReturn = CKR_FUNCTION_FAILED;
            }
        }
    }

    else if( xHandle == eAwsDevicePublicKey )
    {
        /*
         * return reference and size only if key is present in the device and is not private
         */
        if( P11KeyConfig.xDevicePublicKeyMark == pdTRUE )
        {
            uxStatus = psa_get_key_attributes( P11KeyConfig.uxDevicePublicKey, &attributes );
            if( uxStatus == PSA_SUCCESS )
            {
                key_type = psa_get_key_type( &attributes );
                key_bits = psa_get_key_bits( &attributes );
                buffer_size = PSA_KEY_EXPORT_MAX_SIZE( key_type, key_bits );
                uxStatus = psa_export_key( P11KeyConfig.uxDevicePublicKey, pucData, buffer_size, pulDataSize );
                if ( uxStatus == PSA_ERROR_NOT_PERMITTED )
                {
                    *pIsPrivate = CK_TRUE;
                    ulReturn = CKR_OK;
                }
                else if ( uxStatus == PSA_SUCCESS )
                {
                    *pIsPrivate = CK_FALSE;
                    ulReturn = CKR_OK;
                }
                else
                {
                    ulReturn = CKR_FUNCTION_FAILED;
                }
            }
            else
            {
                ulReturn = CKR_FUNCTION_FAILED;
            }
        }
    }

    else if( xHandle == eAwsCodeSigningKey )
    {
        /*
         * return reference and size only if key is present in the device and is not private
         */
        if( P11KeyConfig.xCodeVerifyKeyMark == pdTRUE )
        {
            uxStatus = psa_get_key_attributes( P11KeyConfig.uxCodeVerifyKey, &attributes );
            if( uxStatus == PSA_SUCCESS )
            {
                key_type = psa_get_key_type( &attributes );
                key_bits = psa_get_key_bits( &attributes );
                buffer_size = PSA_KEY_EXPORT_MAX_SIZE( key_type, key_bits );
                uxStatus = psa_export_key( P11KeyConfig.uxCodeVerifyKey, pucData, buffer_size, pulDataSize );
                if ( uxStatus == PSA_ERROR_NOT_PERMITTED )
                {
                    *pIsPrivate = CK_TRUE;
                    ulReturn = CKR_OK;
                }
                else if ( uxStatus == PSA_SUCCESS )
                {
                    *pIsPrivate = CK_FALSE;
                    ulReturn = CKR_OK;
                }
                else
                {
                    ulReturn = CKR_FUNCTION_FAILED;
                }
            }
            else
            {
                ulReturn = CKR_FUNCTION_FAILED;
            }
        }
    }

    return ulReturn;
}

/**
* @brief Cleanup after PKCS11PSAGetObjectValue().
*
* @param[in] pucData       The buffer to free.
*                          (*ppucData from PKCS11PSAGetObjectValue())
* @param[in] ulDataSize    The length of the buffer to free.
*                          (*pulDataSize from PKCS11PSAGetObjectValue())
*/
void PKCS11PSAGetObjectValueCleanup( uint8_t * pucData,
    uint32_t ulDataSize )
{
    /* Unused parameters. */
    ( void ) pucData;
    ( void ) ulDataSize;

    /* Since no buffer was allocated on heap, there is no cleanup
     * to be done. */
}

/**
* @brief Import an object into P11KeyConfig.
*
* @param[in] pLableValue        The lable of the object.
*
* @param[in] ulLableLen         The length of the object label.
*
* @param[in] uxKeyHandle        The key handle.
*
*/
void PKCS11PSAContextImportObject( CK_VOID_PTR pLableValue,
                                   CK_LONG  ulLableLen,
                                   psa_key_handle_t uxKeyHandle )
{
    if( strcmp( pLableValue,
                pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ) == 0 )
    {
        P11KeyConfig.uxDevicePrivateKey = uxKeyHandle;

        /*change key present mark'*/
        P11KeyConfig.xDevicePrivateKeyMark = pdTRUE;
    }
    else if( strcmp( pLableValue,
                pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS ) == 0 )
    {
        P11KeyConfig.uxDevicePublicKey = uxKeyHandle;

        /*change key present mark'*/
        P11KeyConfig.xDevicePublicKeyMark = pdTRUE;
    }
    else if( strcmp( pLableValue,
                pkcs11configLABEL_CODE_VERIFICATION_KEY ) == 0 )
    {
        P11KeyConfig.uxCodeVerifyKey = uxKeyHandle;

        /*change key present mark'*/
        P11KeyConfig.xCodeVerifyKeyMark = pdTRUE;
    }
    else if( strcmp( pLableValue,
                pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ) == 0 )
    {
        /*change certificate present mark'*/
        P11KeyConfig.xDeviceCertificateMark = pdTRUE;
    }
    else if( strcmp( pLableValue,
                pkcs11configLABEL_JITP_CERTIFICATE ) == 0 )
    {
        /*change certificate present mark'*/
        P11KeyConfig.xJitpCertificateMark = pdTRUE;
    }
    else if( strcmp( pLableValue,
                pkcs11configLABEL_ROOT_CERTIFICATE ) == 0 )
    {
        /*change certificate present mark'*/
        P11KeyConfig.xRootCertificateMark = pdTRUE;
    }
    else
    {
        /* Do nothing. */
    }
}

/**
* @brief Close or remove an object from PSA and update P11KeyConfig accordingly.
*
* @param[in] pLableValue        The lable of the object.
*
* @param[in] ulLableLen         The length of the object label.
*
* @param[in] permanent          Destroy the key or just close the key.
*
*/
CK_RV PKCS11PSARemoveObject(uint8_t * pcLable, size_t xLabelLength, bool permanent)
{
    CK_RV xResult = CKR_OK;
    psa_status_t uxStatus;

    if( memcmp( pcLable, pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS, xLabelLength ) == 0 )
    {
        if( P11KeyConfig.xDevicePrivateKeyMark == pdTRUE )
        {
            if( permanent )
            {
                uxStatus = psa_destroy_key( P11KeyConfig.uxDevicePrivateKey );
            }
            else
            {
                uxStatus = psa_close_key( P11KeyConfig.uxDevicePrivateKey );
            }

            if( uxStatus != PSA_SUCCESS )
            {
                xResult = CKR_FUNCTION_FAILED;
            }
            else
            {
                P11KeyConfig.uxDevicePrivateKey = 0;
                P11KeyConfig.xDevicePrivateKeyMark = pdFALSE;
            }
        }
    }
    else if( memcmp( pcLable, pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS, xLabelLength ) == 0 )
    {
        if( P11KeyConfig.xDevicePublicKeyMark == pdTRUE )
        {
            if( permanent )
            {
                uxStatus = psa_destroy_key( P11KeyConfig.uxDevicePublicKey );
            }
            else
            {
                uxStatus = psa_close_key( P11KeyConfig.uxDevicePublicKey );
            }
            if( uxStatus != PSA_SUCCESS )
            {
                xResult = CKR_FUNCTION_FAILED;
            }
            else
            {
                P11KeyConfig.uxDevicePublicKey = 0;
                P11KeyConfig.xDevicePublicKeyMark = pdFALSE;
            }
        }
    }
    else if( memcmp( pcLable, pkcs11configLABEL_CODE_VERIFICATION_KEY, xLabelLength ) == 0 )
    {
        if( P11KeyConfig.xCodeVerifyKeyMark == pdTRUE )
        {
            if( permanent )
            {
                uxStatus = psa_destroy_key( P11KeyConfig.uxCodeVerifyKey );
            }
            else
            {
                uxStatus = psa_close_key( P11KeyConfig.uxCodeVerifyKey );
            }
            if( uxStatus != PSA_SUCCESS )
            {
                xResult = CKR_FUNCTION_FAILED;
            }
            else
            {
                P11KeyConfig.uxCodeVerifyKey = 0;
                P11KeyConfig.xCodeVerifyKeyMark = pdFALSE;
            }
        }
    }
    else if( memcmp( pcLable, pkcs11configLABEL_ROOT_CERTIFICATE, xLabelLength ) == 0 )
    {
        if( P11KeyConfig.xRootCertificateMark == pdTRUE )
        {
            uxStatus = psa_ps_remove( PSA_ROOT_CERTIFICATE_UID );
            if( uxStatus != PSA_SUCCESS )
            {
                xResult = CKR_FUNCTION_FAILED;
            }
            else
            {
                P11KeyConfig.xRootCertificateMark = pdFALSE;
            }
        }
    }
    else if( memcmp( pcLable, pkcs11configLABEL_JITP_CERTIFICATE, xLabelLength ) == 0 )
    {
        if( P11KeyConfig.xJitpCertificateMark == pdTRUE )
        {
            uxStatus = psa_ps_remove( PSA_JITP_CERTIFICATE_UID );
            if( uxStatus != PSA_SUCCESS )
            {
                xResult = CKR_FUNCTION_FAILED;
            }
            else
            {
                P11KeyConfig.xJitpCertificateMark = pdFALSE;
            }
        }
    }
    else if( memcmp( pcLable, pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS, xLabelLength ) == 0 )
    {
        if( P11KeyConfig.xDeviceCertificateMark == pdTRUE )
        {
            uxStatus = psa_ps_remove( PSA_DEVICE_CERTIFICATE_UID );
            if( uxStatus != PSA_SUCCESS )
            {
                xResult = CKR_FUNCTION_FAILED;
            }
            else
            {
                P11KeyConfig.xDeviceCertificateMark = pdFALSE;
            }
        }
    }
    else
    {
        xResult = CKR_ATTRIBUTE_VALUE_INVALID;
    }

    return xResult;
}

/**
* @brief Get the PSA key handle by the object label.
*
* @param[in] pLableValue        The lable of the object.
*
* @param[in] ulLableLen         The length of the object label.
*
* @param[in] uxKeyHandle        The returned key handle.
*
*/
CK_RV PKCS11PSAGetKeyHandle( uint8_t * pcLable, size_t xLabelLength, psa_key_handle_t * uxKeyHandle )
{
    CK_RV xResult = CKR_OK;

    if(( memcmp( pcLable, pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS, xLabelLength ) == 0 ) &&
       ( P11KeyConfig.xDevicePrivateKeyMark == pdTRUE ))
    {
        *uxKeyHandle = P11KeyConfig.uxDevicePrivateKey;
    }
    else if(( memcmp( pcLable, pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS, xLabelLength ) == 0 ) &&
            ( P11KeyConfig.xDevicePublicKeyMark == pdTRUE ))

    {
        *uxKeyHandle = P11KeyConfig.uxDevicePublicKey;
    }
    else if(( memcmp( pcLable, pkcs11configLABEL_CODE_VERIFICATION_KEY, xLabelLength ) == 0 ) &&
            ( P11KeyConfig.xCodeVerifyKeyMark == pdTRUE ))
    {
        *uxKeyHandle = P11KeyConfig.uxCodeVerifyKey;
    }
    else
    {
        xResult = CKR_ARGUMENTS_BAD;
        *uxKeyHandle = 0;
    }

    return xResult;
}
