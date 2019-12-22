/*
 * Amazon FreeRTOS PKCS#11 for A71CH V1.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2018 NXP
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
 * @file pkcs11.c
 * @brief Windows simulator PKCS#11 implementation for software keys. This
 * file deviates from the FreeRTOS style standard for some function names and
 * data types in order to maintain compliance with the PKCS#11 standard.
 */

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if (SSS_HAVE_SSCP || SSS_HAVE_SE05X)
/* FreeRTOS includes. */
#if defined(USE_RTOS) && USE_RTOS == 1
#include "FreeRTOS.h"
#include "FreeRTOSIPConfig.h"
#include "task.h"
//#include "aws_crypto.h"
#include "iot_crypto.h"
#endif
//#include "aws_pkcs11.h"
#include "iot_pkcs11.h"

/* mbedTLS includes. */
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/sha256.h"
#include "mbedtls/base64.h"
#if defined(USE_RTOS) && USE_RTOS == 1
#include "aws_clientcredential.h"
#endif

/*Other includes */
#include "HLSEAPI.h"
#include "ex_sss.h"
#include <nxLog_App.h>
#if SSS_HAVE_ALT_A71CH
#  include "ax_mbedtls.h"
#endif
#if SSS_HAVE_ALT_SSS
#  include "sss_mbedtls.h"
#endif

#if defined(PKCS11_LIBRARY)
#include <projdefs.h>

#if (__GNUC__  && !AX_EMBEDDED)
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
    /* Only for base session with os */
    static pthread_mutex_t gSessionlock;
    static pthread_mutex_t gFilelock;
#endif

#endif

#if SSS_HAVE_ALT_A71CH
#include "tstHostCrypto.h"
#endif
/* C runtime includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if SSS_HAVE_AWS
#include "aws_iot_config.h"
#elif SSS_HAVE_AZURE
#include "azure_iot_config.h"
#elif SSS_HAVE_GCP
#include "gcp_iot_config.h"
#elif SSS_HAVE_IBM
#include "ibm_watson_iot_config.h"
#elif SSS_HAVE_GGC
#include "greengrass_iot_config.h"
#endif

#include <fsl_sss_util_asn1_der.h>

/* Remove this later */
#include <ex_sss_boot.h>
extern ex_sss_boot_ctx_t* pex_sss_demo_boot_ctx;
extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;
extern char *g_port_name;

#define pkcs11SLOT_ID                         1
#define pkcs11OBJECT_HANDLE_PUBLIC_KEY        1
#define pkcs11OBJECT_HANDLE_PRIVATE_KEY       2
#define pkcs11OBJECT_HANDLE_CERTIFICATE       3
#define pkcs11OBJECT_HANDLE_SYMMETRIC         4

#define SIZE_CLIENT_CERTIFICATE 500
#define SIZE_INTER_ROOT_CERTIFICATE 900
/**
 * @brief Definitions for parameter checking
 */
#define pkcs11CREATEOBJECT_MINIMUM_ATTRIBUTE_COUNT                    2
#define pkcs11CREATEOBJECT_CLASS_ATTRIBUTE_INDEX                      0
#define pkcs11CREATEOBJECT_CERTIFICATE_ATTRIBUTE_COUNT                4
#define pkcs11CREATEOBJECT_CERTIFICATE_VALUE_ATTRIBUTE_INDEX          1
#define pkcs11CREATEOBJECT_CERTIFICATE_ATTRIBUTE_INDEX                2
#define pkcs11CREATEOBJECT_KEY_ATTRIBUTE_COUNT                        5
#define pkcs11CREATEOBJECT_KEY_VALUE_ATTRIBUTE_INDEX                  2
#define pkcs11CREATEOBJECT_PRIVATE_KEY_VALUE_ATTRIBUTE_INDEX          3

/**
 * Defines for the set key pair through create object
 */
#define pkcs11CREATEOBJECT_SET_KEYPAIR_LENGTH                         138
#define pkcs11CREATEOBJECT_SET_PUBKEY_LENGTH                          65
#define pkcs11CREATEOBJECT_PUBLIC_KEY_INDEX                           32

#define pkcs11NO_OPERATION    ( ( CK_MECHANISM_TYPE ) 0xFFFFFFFFF )

uint8_t nist_header_start[] = { 0x30, 0x81, 0x87, 0x02, 0x01, 0x00, 0x30, 0x13,
        0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02,
        0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
        0x03, 0x01, 0x07, 0x04, 0x6D, 0x30, 0x6B, 0x02,
        0x01, 0x01, 0x04, 0x20};

uint8_t nist_header_end[] = {0xA1, 0x44, 0x03, 0x42, 0x00 };

static int sessionCount = 0;
#if (__GNUC__  && !AX_EMBEDDED)
static bool mutex_initialised = false;
#endif

#if SSS_HAVE_AWS
uint32_t CLIENT_KEY_PAIR_INDEX = SSS_KEYPAIR_INDEX_CLIENT_PRIVATE;
uint32_t CLIENT_CERT_INDEX = SSS_CERTIFICATE_INDEX_CLIENT;
uint32_t CLIENT_INTER_KEY_PAIR_INDEX = SSS_KEYPAIR_INDEX_CLIENT_INTERMEDIATE;
uint32_t CLIENT_INTER_CERT_INDEX = SSS_CERTIFICATE_INDEX_INTERMEDIATE;
uint32_t CLIENT_SYMMETRIC_KEY_INDEX = 0;
#elif SSS_HAVE_AZURE
uint32_t CLIENT_KEY_PAIR_INDEX = AZURE_IOT_KEY_INDEX_SM;
uint32_t CLIENT_CERT_INDEX = AZURE_IOT_CLIENT_CERT_INDEX_SM;
uint32_t CLIENT_INTER_KEY_PAIR_INDEX = 0;
uint32_t CLIENT_INTER_CERT_INDEX = 0;
uint32_t CLIENT_SYMMETRIC_KEY_INDEX = 0;
#elif SSS_HAVE_GCP
uint32_t CLIENT_KEY_PAIR_INDEX = SSS_KEYPAIR_INDEX_CLIENT_PRIVATE;
uint32_t CLIENT_CERT_INDEX = SSS_CERTIFICATE_INDEX;
uint32_t CLIENT_INTER_KEY_PAIR_INDEX = 0;
uint32_t CLIENT_INTER_CERT_INDEX = 0;
uint32_t CLIENT_SYMMETRIC_KEY_INDEX = 0;
#elif SSS_HAVE_IBM
uint32_t CLIENT_KEY_PAIR_INDEX = SSS_KEYPAIR_INDEX_CLIENT_PRIVATE;
uint32_t CLIENT_CERT_INDEX = SSS_CERTIFICATE_INDEX;
uint32_t CLIENT_INTER_KEY_PAIR_INDEX = 0;
uint32_t CLIENT_INTER_CERT_INDEX = 0;
uint32_t CLIENT_SYMMETRIC_KEY_INDEX = 0;
#elif SSS_HAVE_GGC
uint32_t CLIENT_KEY_PAIR_INDEX = GREENGRASS_CLIENT_KEY_INDEX;
uint32_t CLIENT_CERT_INDEX = GREENGRASS_CLIENT_CERT_INDEX;
uint32_t CLIENT_INTER_KEY_PAIR_INDEX = 0;
uint32_t CLIENT_INTER_CERT_INDEX = 0;
uint32_t CLIENT_SYMMETRIC_KEY_INDEX = GREENGRASS_SYMM_KEY_INDEX;
#else
#error "Set Key Index"
#endif


typedef int( *pfnMbedTlsSign )(
    void *ctx,
    mbedtls_md_type_t md_alg,
    const unsigned char *hash,
    size_t hash_len,
    unsigned char *sig,
    size_t *sig_len,
    int( *f_rng )( void *, unsigned char *, size_t ),
    void *p_rng );



/**
 * @brief Key structure.
 */
typedef struct P11Key
{
    mbedtls_pk_context xMbedPkCtx;
    mbedtls_x509_crt xMbedX509Cli;
    mbedtls_pk_info_t xMbedPkInfo;
    pfnMbedTlsSign pfnSavedMbedSign;
    void *pvSavedMbedPkCtx;
    uint16_t pem_len_client;
    uint16_t pem_len_client_ca;
    unsigned char * certificate_buf; //[2500];
} P11Key_t, * P11KeyPtr_t;

/**
 * @brief Session structure.
 */
typedef struct P11Session
{
    P11KeyPtr_t pxCurrentKey;
    CK_ULONG ulState;
    CK_BBOOL xOpened;
    CK_MECHANISM_TYPE xOperationInProgress;
    CK_OBJECT_HANDLE xOperationKeyHandle;
    CK_BBOOL xFindObjectInit;
    CK_BBOOL xFindObjectComplete;
    CK_OBJECT_CLASS xFindObjectClass;
    mbedtls_ctr_drbg_context xMbedDrbgCtx;
    mbedtls_entropy_context xMbedEntropyContext;
    mbedtls_pk_context xPublicKey;
    mbedtls_sha256_context xSHA256Context;
    CK_BBOOL labelPresent;
    void* mechParameter;
    CK_ULONG mechParameterLen;
} P11Session_t, * P11SessionPtr_t;


/*-----------------------------------------------------------*/
static void prvFreeKey( P11KeyPtr_t pxKey );
static int awsRetreieve_certificate(unsigned char *pbuf, uint8_t index, uint32_t *outbuflen);
static int convertdertopemformat(const unsigned char * int_ca_cert_der, int len_der, unsigned char * int_ca_cert_pem, int dst_buf_len, U16 *outlength);
// static int convertpemtoder( const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen );
CK_RV ParseSignMechanism(P11SessionPtr_t pxSession, sss_algorithm_t* algorithm);
CK_RV ParseEncryptionMechanism(P11SessionPtr_t pxSession, sss_algorithm_t* algorithm);
CK_RV ParseDigestMechanism(P11SessionPtr_t pxSession, sss_algorithm_t* algorithm);
CK_RV AsymmetricEncrypt(P11SessionPtr_t pxSessionObj, sss_algorithm_t algorithm, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen);
CK_RV AsymmetricDecrypt(P11SessionPtr_t pxSessionObj, sss_algorithm_t algorithm, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);
CK_RV SymmetricEncrypt(P11SessionPtr_t pxSessionObj, sss_algorithm_t algorithm, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen);
CK_RV SymmetricDecrypt(P11SessionPtr_t pxSessionObj, sss_algorithm_t algorithm, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen);
CK_RV GetAttributeParameterIndex(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ATTRIBUTE_TYPE type, CK_ULONG_PTR index);

#if SSS_HAVE_ALT_A71CH
static U16 HLSE_Create_token(uint32_t keyId, HLSE_OBJECT_TYPE objType, void *buff, unsigned long bufflen,  HLSE_OBJECT_HANDLE handle_object);
static mbedtls_ecp_group_id curve_list[16] = { 0, };
#endif



#if SSS_HAVE_ALT_SSS
static sss_status_t sss_create_token(sss_key_store_t *keystore,
                              sss_object_t *CreateObject,
                              U32 ObjectId,
                              sss_key_part_t KeyPart,
                              sss_cipher_type_t CipherType,
                              U8 *buffer,
                              U32 bufferLen,
                              U32 bitLen);

static mbedtls_ecp_group_id curve_list[16] = { 0, };
#endif


typedef struct _object_identifiers
{
    uint32_t identifier[16];
    size_t indentifier_len;
    int groupId;
    char* name;
} object_identifiers_t;
static object_identifiers_t object_identifiers[] = {
{ { 1, 2, 840, 10045, 3, 1, 1 },       7,    MBEDTLS_ECP_DP_SECP192R1, "MBEDTLS_ECP_DP_SECP192R1" },
{ { 1, 3, 132, 0, 33 },                5,    MBEDTLS_ECP_DP_SECP224R1, "MBEDTLS_ECP_DP_SECP224R1" },
{ { 1, 2, 840, 10045, 3, 1, 7 },       7,    MBEDTLS_ECP_DP_SECP256R1, "MBEDTLS_ECP_DP_SECP256R1" },
{ { 1, 3, 132, 0, 34 },                5,    MBEDTLS_ECP_DP_SECP384R1, "MBEDTLS_ECP_DP_SECP384R1" },
{ { 1, 3, 132, 0, 35 },                5,    MBEDTLS_ECP_DP_SECP521R1, "MBEDTLS_ECP_DP_SECP521R1" },

{ { 1, 3, 36, 3, 3, 2, 8, 1, 1, 7 },   10,    MBEDTLS_ECP_DP_BP256R1, "MBEDTLS_ECP_DP_BP256R1" },
{ { 1, 3, 24, 3, 3, 2, 8, 1, 1, 7 },   10,    MBEDTLS_ECP_DP_BP256R1, "MBEDTLS_ECP_DP_BP256R1" },
{ { 1, 3, 36, 3, 3, 2, 8, 1, 1, 11 },  10,    MBEDTLS_ECP_DP_BP384R1, "MBEDTLS_ECP_DP_BP384R1" },
{ { 1, 3, 36, 3, 3, 2, 8, 1, 1, 13 },  10,    MBEDTLS_ECP_DP_BP512R1, "MBEDTLS_ECP_DP_BP512R1" },

{ { 1, 3, 132, 0, 31 },                5,    MBEDTLS_ECP_DP_SECP192K1, "MBEDTLS_ECP_DP_SECP192K1" },
{ { 1, 3, 132, 0, 32 },                5,    MBEDTLS_ECP_DP_SECP224K1, "MBEDTLS_ECP_DP_SECP224K1" },
{ { 1, 3, 132, 0, 10 },                5,    MBEDTLS_ECP_DP_SECP256K1, "MBEDTLS_ECP_DP_SECP256K1" },
{ { 0, }, 0, 0 },
};

int mbedtls_ssl_set_curve_list(mbedtls_ssl_config *conf)
{
    sss_status_t status = kStatus_SSS_Fail;
    sss_object_t clientKey = { 0, };
    uint32_t objectId[16] = {
        0,
    };
    uint8_t objectIdLen = 0;
    int i = 0;

    status = sss_key_object_init(&clientKey, &pex_sss_demo_boot_ctx->ks);
    if (status != kStatus_SSS_Success) {
        return kStatus_SSS_Fail;
    }

    status = sss_key_object_get_handle(&clientKey, CLIENT_KEY_PAIR_INDEX);
    if (status != kStatus_SSS_Success) {
        return kStatus_SSS_Fail;
    }

    status = sss_util_asn1_get_oid_from_sssObj(
        &clientKey, objectId, &objectIdLen);
    if (status != kStatus_SSS_Success) {
        return 1;
    }

    while (1) {
        if (object_identifiers[i].indentifier_len == 0) {
            break;
        }

        if (object_identifiers[i].indentifier_len != objectIdLen) {
            i++;
            continue;
        }

        if (0 == memcmp(object_identifiers[i].identifier, objectId, (objectIdLen * sizeof(uint32_t))))
        {
            curve_list[0] = object_identifiers[i].groupId;
            curve_list[1] = MBEDTLS_ECP_DP_NONE;
            mbedtls_ssl_conf_curves(conf, curve_list);
            return 0;
        }

        i++;
    }

    return 1;
}


// sss_status_t get_ec_point_from_secp256r1(uint8_t* data, uint16_t* outKeyIndex, size_t* pubKeyLen)
// {
//     sss_status_t status = kStatus_SSS_Fail;
//     // uint8_t data[100] = { 0 };
//     size_t dataLen = sizeof(data);
//     // uint16_t outKeyIndex = 0;
//     // size_t pubKeyLen = 0;
//     status = sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &pex_sss_demo_boot_ctx->obj, &data[0], &dataLen, 256);
//     if(status != kStatus_SSS_Success) {
//         return status;
//     }
//     status = sss_util_pkcs8_asn1_get_ec_public_key_index(&data[0], dataLen, &outKeyIndex, &pubKeyLen);
//     if(status != kStatus_SSS_Success) {
//         return status;
//     }

// }


/**
 * @brief Maps an opaque caller session handle into its internal state structure.
 */
static P11SessionPtr_t prvSessionPointerFromHandle( CK_SESSION_HANDLE xSession )
{
    return ( P11SessionPtr_t ) xSession; /*lint !e923 Allow casting integer type to pointer for handle. */
}


/*-----------------------------------------------------------*/

/**
 * @brief Initializes a key structure.
 */
static CK_RV prvInitializeKey( P11SessionPtr_t pxSessionObj,
    const char * pcEncodedKey,
    const uint32_t ulEncodedKeyLength,
    const char * pcEncodedCertificate,
    const uint32_t ulEncodedCertificateLength )
{
    CK_RV xResult = CKR_OK;

    /*
     * Create the key structure, but allow an existing one to be used.
     */
    if( NULL == pxSessionObj->pxCurrentKey )
    {
#if defined(USE_RTOS) && USE_RTOS == 1
        if( NULL == ( pxSessionObj->pxCurrentKey = ( P11KeyPtr_t )pvPortMalloc(
            sizeof( P11Key_t ) ) ) ) /*lint !e9087 Allow casting void* to other types. */
#else
        if ( NULL == ( pxSessionObj->pxCurrentKey = ( P11KeyPtr_t )malloc(sizeof( P11Key_t ) ) ) )
#endif
        {
            xResult = CKR_HOST_MEMORY;
        }
    }

    if( CKR_OK == xResult )
    {
        pxSessionObj->pxCurrentKey->pem_len_client = 0;
        pxSessionObj->pxCurrentKey->pem_len_client_ca = 0;
        pxSessionObj->pxCurrentKey->certificate_buf = NULL;

#if SSS_HAVE_ALT_A71CH
        xResult = (CK_RV)(ax_mbedtls_associate_keypair(
            0, &(pxSessionObj->pxCurrentKey->xMbedPkCtx)));
#endif
#if SSS_HAVE_ALT_SSS

        xResult = (CK_RV)(sss_mbedtls_associate_keypair(
             &(pxSessionObj->pxCurrentKey->xMbedPkCtx), &pex_sss_demo_tls_ctx->obj));
#endif
        if (xResult != CKR_OK)
        {
            xResult = CKR_FUNCTION_FAILED;
        }

        pxSessionObj->pxCurrentKey->pvSavedMbedPkCtx = pxSessionObj->pxCurrentKey->xMbedPkCtx.pk_ctx;
    }
    return xResult;
}

/*-----------------------------------------------------------*/

/**
 * @brief Load the default key and certificate from storage.
 */
static CK_RV prvLoadAndInitializeDefaultCertificateAndKey( P11SessionPtr_t pxSession )
{
    CK_RV xResult = 0;
    uint8_t * pucCertificateData = NULL;
    uint32_t ulCertificateDataLength = 0;
    uint8_t * pucKeyData = NULL;
    uint32_t ulKeyDataLength = 0;
    int ret = 0;
    uint8_t  index;
    unsigned char * aclient_cer; //[500];
    unsigned char * aintca_cer; //[1500];
    uint32_t client_crt_len = 600; //sizeof(aclient_cer);
    uint32_t intca_crt_len = 1000; //sizeof(aintca_cer);

#if defined(USE_RTOS) && USE_RTOS == 1
    if( NULL == (aclient_cer = (unsigned char *)pvPortMalloc(client_crt_len)))    {
        xResult = CKR_HOST_MEMORY;
    }
    if( NULL == (aintca_cer = (unsigned char *)pvPortMalloc(intca_crt_len)))    {
        xResult = CKR_HOST_MEMORY;
    }
#else
    if( NULL == (aclient_cer = (unsigned char *)malloc(client_crt_len)))    {
        xResult = CKR_HOST_MEMORY;
    }
    if( NULL == (aintca_cer = (unsigned char *)malloc(intca_crt_len)))    {
        xResult = CKR_HOST_MEMORY;
    }
#endif


    if (CKR_OK == xResult)
    {
    /* Attach the certificate and key to the session. */
        xResult = prvInitializeKey( pxSession,
                                ( const char * ) pucKeyData,
                                ulKeyDataLength,
                                ( const char * ) pucCertificateData,
                                ulCertificateDataLength );
    }

        if (CKR_OK == xResult)
        {
            mbedtls_x509_crt_init( &pxSession->pxCurrentKey->xMbedX509Cli );

            index = 0;
            ret = awsRetreieve_certificate(aclient_cer, index, &client_crt_len);
            if (ret != 0)    {
                xResult = CKR_FUNCTION_FAILED;
                /*failure */
            }

#if AWS_IOT
            index = 1;
            ret = awsRetreieve_certificate(aintca_cer, index, &intca_crt_len);
            if ( ret == 0 ){
                xResult = CKR_OK;
            }
            else
#endif
            {
                // greceful exit.
                xResult = CKR_OK;
                intca_crt_len = 0; // Not Using intca_crt_len
            }

        }

    if (CKR_OK == xResult)
    {

#if defined(USE_RTOS) && USE_RTOS == 1
            if( NULL == (pxSession->pxCurrentKey->certificate_buf = (unsigned char *)pvPortMalloc(2000)))    {
                   xResult = CKR_HOST_MEMORY;
               }
#else
            if( NULL == (pxSession->pxCurrentKey->certificate_buf = (unsigned char *)malloc(2000)))    {
                  xResult = CKR_HOST_MEMORY;
              }
#endif
            ret = convertdertopemformat(aclient_cer, client_crt_len, \
                    pxSession->pxCurrentKey->certificate_buf, \
                    sizeof(pxSession->pxCurrentKey->certificate_buf), \
                    &(pxSession->pxCurrentKey->pem_len_client));

            if (ret == 0 && intca_crt_len != 0)
            {
                (pxSession->pxCurrentKey->pem_len_client) -= 1;
                ret = convertdertopemformat(aintca_cer, intca_crt_len, \
                        &(pxSession->pxCurrentKey->certificate_buf[pxSession->pxCurrentKey->pem_len_client]),\
                        sizeof(pxSession->pxCurrentKey->certificate_buf)-(pxSession->pxCurrentKey->pem_len_client), \
                        &(pxSession->pxCurrentKey->pem_len_client_ca));
            }

            if (ret != 0)        {
                xResult = CKR_FUNCTION_FAILED;
                /*failure */
            }


        /* Stir the random pot. */
        mbedtls_entropy_update_manual( &pxSession->xMbedEntropyContext,
                                       pucKeyData,
                                       ulKeyDataLength );

            mbedtls_entropy_update_manual( &pxSession->xMbedEntropyContext,
                                           pucCertificateData,
                                           ulCertificateDataLength );

    }

    if ( NULL != aclient_cer )    {
#if defined(USE_RTOS) && USE_RTOS == 1
        vPortFree(aclient_cer);
#else
        if(aclient_cer)
            free(aclient_cer);
#endif
    }

    if ( NULL != aintca_cer )    {
#if defined(USE_RTOS) && USE_RTOS == 1
        vPortFree(aintca_cer);
#else
        if(aintca_cer)
            free(aintca_cer);
#endif
    }
    /* certificate_buf Freed later */

    return xResult;
}

CK_RV GetAttributeParameterIndex(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ATTRIBUTE_TYPE type, CK_ULONG_PTR index)
{
    CK_RV xResult = CKR_ARGUMENTS_BAD;
    CK_ULONG i = 0;
    CK_BBOOL foundType = CK_FALSE;
    for(i = 0; i < ulCount; i++)
    {
        if(pTemplate[i].type == type)
        {
            foundType = CK_TRUE;
            xResult = CKR_OK;
            break;
        }
    }
    if(foundType)
    {
        *index = i;
    }
    return xResult;
}

CK_RV AsymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV xResult = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t data[2048] = { 0 };
    size_t dataLen = sizeof(data);
    // sss_algorithm_t digest_algorithm = kAlgorithm_None;
    sss_asymmetric_t asymmCtx;

    if(pxSessionObj->xOperationInProgress == CKM_RSA_PKCS) {
        xResult = (2048 >= (ulDataLen + 11)) ? CKR_OK : CKR_MECHANISM_INVALID;
        if(xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }
    }

#if 0 //Check if hashing is performed in applet because we are not calculating hash in AKM
    if(pxSessionObj->xOperationInProgress != CKM_RSA_PKCS) {
        xResult = GetSSSAlgorithm(algorithm, &digest_algorithm);
        if(xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }

        sss_digest_t digestCtx;
        status = sss_digest_context_init(&digestCtx,
            &pex_sss_demo_boot_ctx->session,
            digest_algorithm,
            kMode_SSS_Digest);
        if(status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_DEVICE_ERROR;
        }
        status = sss_digest_one_go(&digestCtx,
            pData,
            (size_t) ulDataLen,
            &data[0],
            &dataLen);
        if(status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_DEVICE_ERROR;
        }
        sss_digest_context_free(&digestCtx);
    }
    else
#endif
    {
        memcpy(&data[0], pData, ulDataLen);
        dataLen = ulDataLen;
    }

    status = sss_asymmetric_context_init(&asymmCtx,
        &pex_sss_demo_boot_ctx->session,
        &pex_sss_demo_tls_ctx->obj,
        algorithm,
        kMode_SSS_Encrypt);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_asymmetric_context_ sign init Failed...");
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_FUNCTION_FAILED;
    }

    /* Do Encryption */
    uint8_t encData[256] = { 0 };
    size_t encDataLen = sizeof(encData);
    status = sss_asymmetric_encrypt(&asymmCtx, &data[0], dataLen, &encData[0], &encDataLen);

    if (status != kStatus_SSS_Success) {
        LOG_E("sss_asymmetric_encrypt failed");
        xResult = CKR_FUNCTION_FAILED;
    }
    if(xResult == CKR_OK) {
        if(pEncryptedData){
            if(*pulEncryptedDataLen < encDataLen) {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else {
                memcpy(pEncryptedData, &encData[0], encDataLen);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
        *pulEncryptedDataLen = encDataLen;
    }

    sss_asymmetric_context_free(&asymmCtx);

    return xResult;
}

CK_RV AsymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen)
{
    CK_RV xResult = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t data[2048] = { 0 };
    size_t dataLen = sizeof(data);
    // sss_algorithm_t digest_algorithm = kAlgorithm_None;
    sss_asymmetric_t asymmCtx;

    // if(pxSessionObj->xOperationInProgress == CKM_RSA_PKCS) {
    //     xResult = (2048 >= (ulEncryptedDataLen + 11)) ? CKR_OK : CKR_MECHANISM_INVALID;
    //     if(xResult != CKR_OK) {
    //         pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
    //         return xResult;
    //     }
    // }



#if 0 // Not required for decryption. Not sure how to process Algorithm with hashing.
    if(pxSessionObj->xOperationInProgress != CKM_RSA_PKCS /*&&
        pxSessionObj->xOperationInProgress != CKM_RSA_PKCS_PSS*/) {
        xResult = GetSSSAlgorithm(algorithm, &digest_algorithm);
        if(xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }

        sss_digest_t digestCtx;
        status = sss_digest_context_init(&digestCtx,
            &pex_sss_demo_boot_ctx->session,
            digest_algorithm,
            kMode_SSS_Digest);
        if(status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_DEVICE_ERROR;
        }
        status = sss_digest_one_go(&digestCtx,
            pEncryptedData,
            (size_t) ulEncryptedDataLen,
            &data[0],
            &dataLen);
        if(status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_DEVICE_ERROR;
        }
        sss_digest_context_free(&digestCtx);
    }
    else
#endif
    {
        memcpy(&data[0], pEncryptedData, ulEncryptedDataLen);
        dataLen = ulEncryptedDataLen;
    }

    status = sss_asymmetric_context_init(&asymmCtx,
         &pex_sss_demo_boot_ctx->session,
         &pex_sss_demo_tls_ctx->obj,
            algorithm,
            kMode_SSS_Decrypt);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_asymmetric_context_ sign init Failed...");
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_FUNCTION_FAILED;
    }

    /* Do Signing */
    uint8_t signature[256] = { 0 };
    size_t sigLen = sizeof(signature);
    status = sss_asymmetric_decrypt(&asymmCtx, &data[0], dataLen, &signature[0], &sigLen);
    // status = sss_asymmetric_sign_digest(
    //     &asymmCtx, &data[0], dataLen, &signature[0], &sigLen);

    if (status != kStatus_SSS_Success) {
        LOG_E("sss_asymmetric_decrypt failed");
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        xResult = CKR_FUNCTION_FAILED;
    }
    if(xResult == CKR_OK) {
        if(pData){
            if(*pulDataLen < sigLen) {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else {
                memcpy(pData, &signature[0], sigLen);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
        *pulDataLen = sigLen;
    }

    sss_asymmetric_context_free(&asymmCtx);

    return xResult;

}

CK_RV SymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV xResult = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t data[2048] = { 0 };
    // size_t dataLen = sizeof(data);
    sss_symmetric_t symmCtx;
    sss_object_t symmObject;

    status = sss_key_object_init(&symmObject, &pex_sss_demo_boot_ctx->ks);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_DEVICE_ERROR;
    }

    status = sss_key_object_get_handle(&symmObject, CLIENT_SYMMETRIC_KEY_INDEX);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_DEVICE_ERROR;
    }

    memcpy(&data[0], pData, ulDataLen);
    // dataLen = ulDataLen;
    uint8_t iv[16] = { 0 };
    size_t ivLen = sizeof(iv);
    uint8_t encData[16] = { 0 };
    size_t encDataLen = sizeof(encData);
#if 0
    //PKCS11 Documentation not clear. Ignoring currently
    if(algorithm == kAlgorithm_SSS_AES_CTR) {
        if(!pxSessionObj->mechParameterLen) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_ARGUMENTS_BAD;
        }
        else {
            CK_AES_CTR_PARAMS_PTR ctrParams = (CK_AES_CTR_PARAMS_PTR) pxSessionObj->mechParameter;
            memcpy(&iv[0], &pxSessionObj->mechParameter.cb[0], sizeof(iv));
        }
    }
#endif

    status = sss_symmetric_context_init(&symmCtx,
        &pex_sss_demo_boot_ctx->session, &symmObject, algorithm, kMode_SSS_Encrypt);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_DEVICE_ERROR;
    }

    /*Do Encryption*/
    status = sss_cipher_one_go(
        &symmCtx, &iv[0], ivLen, &data[0], &encData[0], encDataLen);
    if (status != kStatus_SSS_Success) {
        LOG_E("sss_cipher_one_go failed");
        xResult = CKR_FUNCTION_FAILED;
    }
    if(xResult == CKR_OK) {
        if(pEncryptedData){
            if(*pulEncryptedDataLen < encDataLen) {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else {
                memcpy(pEncryptedData, &encData[0], encDataLen);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
        *pulEncryptedDataLen = encDataLen;
    }

    sss_symmetric_context_free(&symmCtx);

    return xResult;

}

CK_RV SymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDecryptedDataLen)
{
        CK_RV xResult = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t data[2048] = { 0 };
    // size_t dataLen = sizeof(data);
    sss_symmetric_t symmCtx;
    sss_object_t symmObject;

    status = sss_key_object_init(&symmObject, &pex_sss_demo_boot_ctx->ks);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_DEVICE_ERROR;
    }

    status = sss_key_object_get_handle(&symmObject, CLIENT_SYMMETRIC_KEY_INDEX);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_DEVICE_ERROR;
    }

    memcpy(&data[0], pEncryptedData, ulEncryptedDataLen);
    // dataLen = ulEncryptedDataLen;
    uint8_t iv[16] = { 0 };
    size_t ivLen = sizeof(iv);
    uint8_t encData[16] = { 0 };
    size_t encDataLen = sizeof(encData);
#if 0
    //PKCS11 Documentation not clear. Ignoring currently
    if(algorithm == kAlgorithm_SSS_AES_CTR) {
        if(!pxSessionObj->mechParameterLen) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_ARGUMENTS_BAD;
        }
        else {
            CK_AES_CTR_PARAMS_PTR ctrParams = (CK_AES_CTR_PARAMS_PTR) pxSessionObj->mechParameter;
            memcpy(&iv[0], &pxSessionObj->mechParameter.cb[0], sizeof(iv));
        }
    }
#endif

    status = sss_symmetric_context_init(&symmCtx,
        &pex_sss_demo_boot_ctx->session, &symmObject, algorithm, kMode_SSS_Decrypt);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_DEVICE_ERROR;
    }

    /*Do Encryption*/
    status = sss_cipher_one_go(
        &symmCtx, &iv[0], ivLen, &data[0], &encData[0], encDataLen);
    if (status != kStatus_SSS_Success) {
        LOG_E("sss_cipher_one_go failed");
        xResult = CKR_FUNCTION_FAILED;
    }
    if(xResult == CKR_OK) {
        if(pData){
            if(*pulDecryptedDataLen < encDataLen) {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else {
                memcpy(pData, &encData[0], encDataLen);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
        *pulDecryptedDataLen = encDataLen;
    }

    sss_symmetric_context_free(&symmCtx);

    return xResult;

}

CK_RV GetSSSAlgorithm(const sss_algorithm_t algorithm, sss_algorithm_t* digest_algo) {
    switch (algorithm) {
    case kAlgorithm_SSS_SHA1:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1:
    case kAlgorithm_SSS_ECDSA_SHA1:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA1:
        *digest_algo = kAlgorithm_SSS_SHA1;
        break;
    case kAlgorithm_SSS_SHA224:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA224:
    case kAlgorithm_SSS_ECDSA_SHA224:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA224:
        *digest_algo = kAlgorithm_SSS_SHA224;
        break;
    case kAlgorithm_SSS_SHA256:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA256:
    case kAlgorithm_SSS_ECDSA_SHA256:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA256:
        *digest_algo = kAlgorithm_SSS_SHA256;
        break;
    case kAlgorithm_SSS_SHA384:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA384:
    case kAlgorithm_SSS_ECDSA_SHA384:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA384:
        *digest_algo = kAlgorithm_SSS_SHA384;
        break;
    case kAlgorithm_SSS_SHA512:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA512:
    case kAlgorithm_SSS_ECDSA_SHA512:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA512:
        *digest_algo = kAlgorithm_SSS_SHA512;
        break;
    default:
        return CKR_ARGUMENTS_BAD;
    }
    return CKR_OK;
}


/*-----------------------------------------------------------*/


/*
 * PKCS#11 module implementation.
 */
/**
 * @brief PKCS#11 interface functions implemented by this Cryptoki module.
 */
CK_FUNCTION_LIST prvP11FunctionList =
{
    { CRYPTOKI_VERSION_MAJOR, CRYPTOKI_VERSION_MINOR },
    C_Initialize,
    C_Finalize,
    C_GetInfo,
    C_GetFunctionList,
    C_GetSlotList,
    C_GetSlotInfo,
    C_GetTokenInfo,
    C_GetMechanismList,
    C_GetMechanismInfo,
    C_InitToken,
    C_InitPIN,
    C_SetPIN,
    C_OpenSession,
    C_CloseSession,
    C_CloseAllSessions,
    C_GetSessionInfo,
    C_GetOperationState,
    C_SetOperationState,
    C_Login,
    C_Logout,
    C_CreateObject,
    C_CopyObject,
    C_DestroyObject,
    C_GetObjectSize,
    C_GetAttributeValue,
    C_SetAttributeValue,
    C_FindObjectsInit,
    C_FindObjects,
    C_FindObjectsFinal,
    C_EncryptInit,
    C_Encrypt,
    C_EncryptUpdate,
    C_EncryptFinal,
    C_DecryptInit,
    C_Decrypt,
    C_DecryptUpdate,
    C_DecryptFinal,
    C_DigestInit,
    C_Digest,
    C_DigestUpdate,
    C_DigestKey,
    C_DigestFinal,
    C_SignInit,
    C_Sign,
    C_SignUpdate,
    C_SignFinal,
    C_SignRecoverInit,
    C_SignRecover,
    C_VerifyInit,
    C_Verify,
    C_VerifyUpdate,
    C_VerifyFinal,
    C_VerifyRecoverInit,
    C_VerifyRecover,
    C_DigestEncryptUpdate,
    C_DecryptDigestUpdate,
    C_SignEncryptUpdate,
    C_DecryptVerifyUpdate,
    C_GenerateKey,
    C_GenerateKeyPair,
    C_WrapKey,
    C_UnwrapKey,
    C_DeriveKey,
    C_SeedRandom,
    C_GenerateRandom,
    C_GetFunctionStatus,
    C_CancelFunction,
    C_WaitForSlotEvent
};

CK_DEFINE_FUNCTION(CK_RV, C_DigestFinal)(CK_SESSION_HANDLE xSession,
    CK_BYTE_PTR pDigest,
    CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    if (pxSession == NULL)
    {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    else if (pxSession->xOperationInProgress != CKM_SHA256)
    {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
    }

    if (xResult == CKR_OK)
    {
        if (pDigest == NULL)
        {
            /* Supply the required buffer size. */
            *pulDigestLen = pkcs11SHA256_DIGEST_LENGTH;
        }
        else
        {
            if (*pulDigestLen < pkcs11SHA256_DIGEST_LENGTH)
            {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else
            {
                if (0 != mbedtls_sha256_finish_ret(&pxSession->xSHA256Context, pDigest))
                {
                    xResult = CKR_FUNCTION_FAILED;
                }

                pxSession->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestUpdate)(CK_SESSION_HANDLE xSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen)
{
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    if (pxSession == NULL)
    {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    else if (pxSession->xOperationInProgress != CKM_SHA256)
    {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
    }

    if (xResult == CKR_OK)
    {
        if (0 != mbedtls_sha256_update_ret(&pxSession->xSHA256Context, pPart, ulPartLen))
        {
            xResult = CKR_FUNCTION_FAILED;
            pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestInit)(CK_SESSION_HANDLE xSession,
    CK_MECHANISM_PTR pMechanism)
{
    CK_RV xResult = CKR_OK;
    // return CKR_DEVICE_ERROR;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    if (pxSession == NULL)
    {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

    if( NULL == pMechanism )
    {
        xResult = CKR_ARGUMENTS_BAD;
        return xResult;
    }

    else if(pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }


    if (pMechanism->mechanism != CKM_SHA256)
    {
        xResult = CKR_MECHANISM_INVALID;
    }

    /*
    * Initialize the requested hash type
    */
    if (xResult == CKR_OK)
    {
        mbedtls_sha256_init(&pxSession->xSHA256Context);

        if (0 != mbedtls_sha256_starts_ret(&pxSession->xSHA256Context, 0))
        {
            xResult = CKR_FUNCTION_FAILED;
        }
        else
        {
            pxSession->xOperationInProgress = pMechanism->mechanism;
        }
    }

    return xResult;
}

/**
 * @brief Generate cryptographically random bytes.
 */
CK_DEFINE_FUNCTION( CK_RV, C_GenerateRandom )( CK_SESSION_HANDLE xSession,
                                               CK_BYTE_PTR pucRandomData,
                                               CK_ULONG ulRandomLen )
{
    CK_RV xResult = CKR_OK;

    /*lint !e9072 It's OK to have different parameter name. */
    // return CKR_DEVICE_ERROR;
    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle( xSession );

    if( ( NULL == pucRandomData ) ||
        ( ulRandomLen == 0 ) )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else
    {
        if( 0 != mbedtls_ctr_drbg_random( &pxSessionObj->xMbedDrbgCtx, pucRandomData, ulRandomLen ) )
        {
            xResult = CKR_FUNCTION_FAILED;
        }
    }

    return xResult;
}

/**
 * @brief Verify the digital signature of the specified data using the public
 * key attached to this session.
 */
CK_DEFINE_FUNCTION( CK_RV, C_Verify )( CK_SESSION_HANDLE xSession,
                                       CK_BYTE_PTR pucData,
                                       CK_ULONG ulDataLen,
                                       CK_BYTE_PTR pucSignature,
                                       CK_ULONG ulSignatureLen )
{
       /*lint !e9072 It's OK to have different parameter name. */
        /* pkcs11tb*/
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle( xSession );
#if SSS_HAVE_ALT_A71CH
    ( void ) (pxSessionObj);
        U8 index = 0;
        HLSE_OBJECT_HANDLE handles[5];
        HLSE_MECHANISM_INFO mechInfo;
        U16 handleNum = 3;
        U16 err;
#endif

#if SSS_HAVE_ALT_SSS
    sss_status_t status;
    sss_object_t object;
    sss_asymmetric_t asymmCtx;
    sss_algorithm_t algorithm;
    // sss_algorithm_t digest_algorithm;
#endif

        /*
         * Check parameters.
         */
        if( ( NULL == pucData ) ||
            ( NULL == pucSignature ) )
        {
            xResult = CKR_ARGUMENTS_BAD;
            return xResult;
        }
#if SSS_HAVE_ALT_A71CH
        err = HLSE_EnumerateObjects(HLSE_PUBLIC_KEY, handles, &handleNum);

        if ((err != HLSE_SW_OK) || (handleNum <= index)) {
            xResult = CKR_ARGUMENTS_BAD;
            return xResult;
        }

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_VERIFY;

        err = HLSE_VerifySignature(
            &mechInfo,
            handles[0],
            (U8*)pucData, ulDataLen,
            (U8*)pucSignature, ulSignatureLen);

        if(err != HLSE_SW_OK)
        {
            xResult = CKR_SIGNATURE_INVALID;

        }
#endif

#if SSS_HAVE_ALT_SSS

    xResult = ParseSignMechanism(pxSessionObj, &algorithm);
    if(xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }

    uint8_t data[2048] = { 0 };
    size_t dataLen = sizeof(data);
    // if(0){
#if 0   //Not required
    if(pxSessionObj->xOperationInProgress != CKM_RSA_PKCS &&
        pxSessionObj->xOperationInProgress != CKM_RSA_PKCS_PSS) {
        xResult = GetSSSAlgorithm(algorithm, &digest_algorithm);
        if(xResult != CKR_OK) {
            return xResult;
        }

        sss_digest_t digestCtx;
        status = sss_digest_context_init(&digestCtx,
            &pex_sss_demo_boot_ctx->session,
            digest_algorithm,
            kMode_SSS_Digest);
        if(status != kStatus_SSS_Success) {
            return CKR_DEVICE_ERROR;
        }
        status = sss_digest_one_go(&digestCtx,
            pucData,
            (size_t) ulDataLen,
            &data[0],
            &dataLen);
        if(status != kStatus_SSS_Success) {
            return CKR_DEVICE_ERROR;
        }
        sss_digest_context_free(&digestCtx);
    }
    else
#endif
    {
        memcpy(&data[0], pucData, ulDataLen);
        dataLen = ulDataLen;
    }

    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;

    status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_key_object_init Failed...");
        return CKR_FUNCTION_FAILED;
    }

    status = sss_key_object_get_handle(&object, (uint32_t) pxSessionObj->xOperationKeyHandle);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_key_object_get_handle Failed...");
        return CKR_FUNCTION_FAILED;
    }

    status = sss_asymmetric_context_init(&asymmCtx,
        &pex_sss_demo_boot_ctx->session,
        &object,
        algorithm,
        kMode_SSS_Verify);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_asymmetric_context_init verify context Failed...");
        return CKR_FUNCTION_FAILED;
    }

    status = sss_asymmetric_verify_digest(
        &asymmCtx, &data[0], dataLen, pucSignature, ulSignatureLen);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_asymmetric_verify_digest Failed... ");
        xResult = CKR_SIGNATURE_INVALID;
    }

    sss_asymmetric_context_free(&asymmCtx);
#endif
        return xResult;
}

CK_DEFINE_FUNCTION( CK_RV, C_Sign )( CK_SESSION_HANDLE xSession,
                                     CK_BYTE_PTR pucData,
                                     CK_ULONG ulDataLen,
                                     CK_BYTE_PTR pucSignature,
                                     CK_ULONG_PTR pulSignatureLen )
{

    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle( xSession );



    sss_status_t status;
    sss_asymmetric_t asymmCtx;
    sss_algorithm_t algorithm;
    //sss_algorithm_t digest_algorithm;




    xResult = ParseSignMechanism(pxSessionObj, &algorithm);
    if(xResult != CKR_OK) {
        return xResult;
    }

    if(pxSessionObj->xOperationInProgress == CKM_ECDSA) {
        switch(ulDataLen)
        {
            case 20:
                algorithm = kAlgorithm_SSS_ECDSA_SHA1;
                break;
            case 28:
                algorithm = kAlgorithm_SSS_ECDSA_SHA224;
                break;
            case 32:
                algorithm = kAlgorithm_SSS_ECDSA_SHA256;
                break;
            case 48:
                algorithm = kAlgorithm_SSS_ECDSA_SHA384;
                break;
            case 64:
                algorithm = kAlgorithm_SSS_ECDSA_SHA512;
                break;
            default:
                return CKR_DATA_LEN_RANGE;
        }
    }

    if(pxSessionObj->xOperationInProgress == CKM_RSA_PKCS) {
        xResult = (2048 >= (ulDataLen + 11)) ? CKR_OK : CKR_MECHANISM_INVALID;
        if(xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }
    }

    uint8_t data[2048] = { 0 };
    size_t dataLen = sizeof(data);
    if(pxSessionObj->xOperationInProgress != CKM_RSA_PKCS &&
        pxSessionObj->xOperationInProgress != CKM_RSA_PKCS_PSS) {
#if 0   //To be reviewed
        xResult = GetSSSAlgorithm(algorithm, &digest_algorithm);
        if(xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }

        sss_digest_t digestCtx;
        status = sss_digest_context_init(&digestCtx,
            &pex_sss_demo_boot_ctx->session,
            digest_algorithm,
            kMode_SSS_Digest);
        if(status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_DEVICE_ERROR;
        }
        status = sss_digest_one_go(&digestCtx,
            pucData,
            (size_t) ulDataLen,
            &data[0],
            &dataLen);
        if(status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return CKR_DEVICE_ERROR;
        }
        sss_digest_context_free(&digestCtx);
#endif
        memcpy(&data[0], pucData, ulDataLen);
        dataLen = ulDataLen;
    }
    else {

        /*
         * PKCS1_V1_5 Padding with no DigestInfo structure
         * TODO: Add support for this algorithm in SSS
         */

        memset(data, 0xFF, dataLen);
        dataLen = 2048 / 8; /*2048 / 8 RSA key*/
        data[0] = 0x00;
        data[1] = 0x01;
        data[dataLen - ulDataLen - 1] = 0x00;
        memcpy(&data[dataLen - ulDataLen], pucData, ulDataLen);

    }

    status = sss_asymmetric_context_init(&asymmCtx,
         &pex_sss_demo_boot_ctx->session,
         &pex_sss_demo_tls_ctx->obj,
            algorithm,
            kMode_SSS_Sign);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_asymmetric_context_ sign init Failed...");
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_FUNCTION_FAILED;
    }

    /* Do Signing */
    uint8_t signature[256] = { 0 };
    size_t sigLen = sizeof(signature);

    status = sss_asymmetric_sign_digest(
        &asymmCtx, &data[0], dataLen, &signature[0], &sigLen);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_asymmetric_sign_digest Failed...");
        xResult = CKR_FUNCTION_FAILED;
    }
    if(xResult == CKR_OK) {
        if(pucSignature){
            if(*pulSignatureLen < sigLen) {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else {
                memcpy(pucSignature, &signature[0], sigLen);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
        *pulSignatureLen = sigLen;
    }

    sss_asymmetric_context_free(&asymmCtx);


    return xResult;

}

/**
 * @brief Free resources attached to an object handle.
 */
CK_DEFINE_FUNCTION( CK_RV, C_DestroyObject )( CK_SESSION_HANDLE xSession,
                                              CK_OBJECT_HANDLE xObject )
{   /*lint !e9072 It's OK to have different parameter name. */
    ( void ) ( xSession );

    //( void ) ( xObject );
    CK_RV xResult = CKR_OK;
    /*
     * This implementation uses virtual handles, and the certificate and
     * private key data are attached to the session, so nothing to do here.
     */
    if (xObject == 0 || xObject == 1)
    {
        return CKR_OK;
    }

    return xResult;
}

#if SSS_HAVE_ALT_A71CH

static U16 HLSE_Create_token(uint32_t keyId, HLSE_OBJECT_TYPE objType, void *buff, unsigned long bufflen,  HLSE_OBJECT_HANDLE handle_object)
{
    HLSE_OBJECT_INDEX index = keyId;
    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = buff;
    attr[2].valueLen = bufflen;

    return HLSE_CreateObject(attr, templateSize, &handle_object);
}

#endif

#if SSS_HAVE_ALT_SSS

static sss_status_t sss_create_token(sss_key_store_t *keystore,
                                  sss_object_t *CreateObject,
                                  U32 ObjectId,
                                  sss_key_part_t KeyPart,
                                  sss_cipher_type_t CipherType,
                                  U8 *buffer,
                                  U32 bufferLen,
                                  U32 bitLen)
{

    sss_status_t status;

    status = sss_key_object_init(CreateObject, keystore);
            if (status != kStatus_SSS_Success) {
                return status;
            }

    status = sss_key_object_allocate_handle(CreateObject,
                                            ObjectId,
                                            KeyPart,
                                            CipherType,
                                            bufferLen,
                                            kKeyObject_Mode_Persistent);
             if (status != kStatus_SSS_Success) {
                 return status;
             }

    status = sss_key_store_set_key(keystore,
                                   CreateObject,
                                   buffer,
                                   bufferLen,
                                   bitLen,
                                   NULL,
                                   0);
             if (status != kStatus_SSS_Success) {
                 return status;
             }
    return status;
}
#endif

/**
 * Extend the byte array \p pStore with 0x00 byte(s). This is typically required when
 * a big integer has - previously - been stripped from its (superfluous) sign bits.
 * The caller must ensure \p expectedLength is bigger than \p actualLength
 * @param[in,out]   pStore  Array representation of big number, to be zero sign extended.
 *      Size of corresponding buffer must be at least \p expectedLength
 * @param[in]       actualLength Length of incoming array \p pStore
 * @param[in]       expectedLength Zero sign extend until this length.
 *
 * @retval SW_OK In case of successfull execution
 * @retval ERR_API_ERROR Requested adjustment would result in truncation
 */
U16 axZeroSignExtend(U8* pStore, U16 actualLength, U16 expectedLength)
{
    U16 sw = SW_OK;

    int numExtraByte = (int)expectedLength - (int)actualLength;

    if (numExtraByte == 0) {
        // Do nothing
    }
    else if (numExtraByte < 0) {
        // Flag an API error
        sw = ERR_API_ERROR;
    }
    else {
        memmove(pStore + numExtraByte, pStore, actualLength);
        memset(pStore, 0x00, numExtraByte);
    }

    return sw;
}

#if SSS_HAVE_ALT_A71CH
/**
 * Extract the public key - as a byte array in uncompress format - from an ECC key
 * @param[in] pKey Reference to ECC key.
 * @param[in,out] pPublicKeyData IN: Buffer to contain public key; OUT: Public key
 * @param[out] pPublicKeyLen Length of public key \p pPublicKeyData retrieved
 * @param[in] maxPublicKeyLen Size of buffer (\p pPublicKeyData) provided to contain public key
*/
U16 HOSTCRYPTO_GetPublicKey(EC_KEY *pKey, U8 *pPublicKeyData, U16 *pPublicKeyLen, U16 maxPublicKeyLen)
{
    int res;
    size_t keylen = 0;
    mbedtls_ecp_keypair * pEcCtx = mbedtls_pk_ec( *pKey );

    res = mbedtls_ecp_check_pubkey(  &(pEcCtx->grp), &(pEcCtx->Q) );
    if ((res != 0) || (pPublicKeyData == NULL) || (pPublicKeyLen == NULL))
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    res = mbedtls_ecp_point_write_binary(&(pEcCtx->grp),&(pEcCtx->Q),MBEDTLS_ECP_PF_UNCOMPRESSED,&keylen,pPublicKeyData,maxPublicKeyLen);
    *pPublicKeyLen = (U16)keylen;

    if ((*pPublicKeyLen == 0) || (res != 0))
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    return SW_OK;
}

/**
 * Extract the private key - as a byte array restored to nominal key size by sign extension - from an ECC key (in an mbed TLS specific format)
 * @param[in] pKey Reference to ECC key.
 * @param[in,out] pPrivateKeyData IN: Buffer to contain private key; OUT: Private key
 * @param[out] pPrivateKeyLen Length of private key \p pPrivateKeyData retrieved
 * @param[in] maxPrivateKeyLen Size of buffer (\p pPrivateKeyData) provided to contain private key
*/
U16 HOSTCRYPTO_GetPrivateKey(EC_KEY *pKey, U8 *pPrivateKeyData, U16 *pPrivateKeyLen, U16 maxPrivateKeyLen)
{

    int significantBytes = 0;
    int keyLen = 0;
    U16 res = SW_OK;
    U8 keyArray[256];
    const mbedtls_ecp_curve_info *p_curve_info = NULL;
    mbedtls_ecp_keypair * pEcCtx = mbedtls_pk_ec( *pKey );

    /*TODO check sign extension part */
    res = mbedtls_ecp_check_privkey( &(pEcCtx->grp), &(pEcCtx->d));
    if ((res != 0) || (pPrivateKeyData == NULL) || (pPrivateKeyLen == NULL))
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    significantBytes = mbedtls_mpi_size( &(pEcCtx->d) );
    p_curve_info = mbedtls_ecp_curve_info_from_grp_id( pEcCtx->grp.id );
    keyLen = ((p_curve_info->bit_size + 7))/8;
    if (keyLen > maxPrivateKeyLen)
    {
        return ERR_BUF_TOO_SMALL;
    }
    res = mbedtls_mpi_write_binary(&(pEcCtx->d),keyArray,keyLen);
    if (res == 0)
    {
        // Extend byte array with leading 0x00 byte in case private key had
        // been truncated because the MSB were not significant
        if (significantBytes > 0)
        {
            res = axZeroSignExtend(keyArray, (U16)significantBytes, (U16)keyLen);
            if (res == SW_OK)
            {
                memcpy(pPrivateKeyData, keyArray, keyLen);
                *pPrivateKeyLen = (U16)keyLen;
            }
            else
            {
                *pPrivateKeyLen = 0;

            }
        }
        else
        {
            *pPrivateKeyLen = 0;
            res = ERR_GENERAL_ERROR;
        }
    }
    else
    {
        res = ERR_CRYPTO_ENGINE_FAILED;
    }


    return res;
}
#endif

/**
 * @brief Provides import and storage of a single client certificate and
 * associated private key.
 */
CK_DEFINE_FUNCTION( CK_RV, C_CreateObject )( CK_SESSION_HANDLE xSession,
                                             CK_ATTRIBUTE_PTR pxTemplate,
                                             CK_ULONG ulCount,
                                             CK_OBJECT_HANDLE_PTR pxObject )
{
    /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;


#if SSS_HAVE_ALT_A71CH
    HLSE_OBJECT_HANDLE HLSE_Handle_Create_obj = 0x0;
    U16 err = 0;
#endif
    int ret;
    U8 buff[2000];
    mbedtls_pk_context pk;
    CK_ULONG Valueindex = 0;
    U32 keyidindex;
    uint32_t keyId = 0xffffffff;
    CK_ULONG i = 0;

#if SSS_HAVE_ALT_SSS
    size_t buff_len =  sizeof(buff);
    sss_status_t status;
#endif

    /* Unreferenced variables.*/
    // ( void ) ( xSession );


    /*
     * Check parameters.
     */

    if( ( pkcs11CREATEOBJECT_MINIMUM_ATTRIBUTE_COUNT > ulCount ) ||
        ( NULL == pxTemplate ) )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }

    if( CKR_OK == xResult )
    {
        if( ( CKA_CLASS != pxTemplate[ pkcs11CREATEOBJECT_CLASS_ATTRIBUTE_INDEX ].type ) ||
            ( sizeof( CK_OBJECT_CLASS ) != pxTemplate[ pkcs11CREATEOBJECT_CLASS_ATTRIBUTE_INDEX ].ulValueLen ) )
        {
            xResult = CKR_ARGUMENTS_BAD;
        }
    }

    /*Find the key id as it's needed while provisiong keys and certificate*/
    for( keyidindex = 0; keyidindex < ulCount; keyidindex++)
    {
        if (pxTemplate[keyidindex].type == CKA_ID) {
            keyId = *(U32 *)pxTemplate[keyidindex].pValue;
            break;
        }
    }
    if(keyId == 0xffffffff) {
        keyId = 0x20181018;
    }


    /*
     * Handle the object by class.
     */

    if( CKR_OK == xResult )
    {
        switch( *( ( uint32_t * ) pxTemplate[ pkcs11CREATEOBJECT_CLASS_ATTRIBUTE_INDEX ].pValue ) )
        {
            case CKO_CERTIFICATE:

                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &i);
                if(xResult != CKR_OK) {
                    return xResult;
                }

#if SSS_HAVE_ALT_A71CH

                err = HLSE_Create_token(keyId,
                                        HLSE_CERTIFICATE,
                                        pxTemplate[ i ].pValue,
                                        pxTemplate[ i ].ulValueLen,
                                        HLSE_Handle_Create_obj);

                if(err != HLSE_SW_OK)
                {
                   xResult = CKR_DEVICE_ERROR;
                   return xResult;
                }

                *pxObject = (CK_OBJECT_HANDLE) HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_ALT_SSS

                if (0 != pxTemplate[i].ulValueLen)
                {
                    keyId = 0x20181017;
                    status = sss_create_token(&pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->dev_cert, keyId, kSSS_KeyPart_Default, kSSS_CipherType_Binary, buff, buff_len, buff_len*8);
                    if (status != kStatus_SSS_Success) {
                        xResult = CKR_DEVICE_ERROR;
                        return xResult;
                    }

                    *pxObject = keyId;
                }
                else
                {
                    status = sss_key_object_init(&pex_sss_demo_tls_ctx->dev_cert, &pex_sss_demo_boot_ctx->ks);
                    if (status != kStatus_SSS_Success) {
                        LOG_E(" sss_key_object_init for clientCert Failed...");
                        return CKR_FUNCTION_FAILED;
                    }

                    status = sss_key_object_get_handle(&pex_sss_demo_tls_ctx->dev_cert, CLIENT_CERT_INDEX);
                    if (status != kStatus_SSS_Success) {
                        LOG_E(" sss_key_object_get_handle  for client Cert Failed...");
                        return CKR_FUNCTION_FAILED;
                    }

                    *pxObject = CLIENT_CERT_INDEX;
                }
#endif
                break;

            case CKO_PRIVATE_KEY:

                /*Parses the private key in PEM format and converts it to DER format.
                    * This is required because as SE shall require a key pair for storing keys*/

                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &Valueindex);
                if(xResult != CKR_OK) {
                    return xResult;
                }

                mbedtls_pk_init( &pk );

                if (0 != pxTemplate[Valueindex].ulValueLen)
                {
                    ret = mbedtls_pk_parse_key(&pk, pxTemplate[Valueindex].pValue, pxTemplate[Valueindex].ulValueLen, NULL, 0);
                    if (ret != 0)
                    {
                        xResult = CKR_ARGUMENTS_BAD;
                        break;
                    }
                }


#if SSS_HAVE_ALT_A71CH

                err = HLSE_Create_token(keyId,
                                        HLSE_KEY_PAIR,
                                        buff,
                                        pkcs11CREATEOBJECT_SET_KEYPAIR_LENGTH,
                                        HLSE_Handle_Create_obj);
               if(err != HLSE_SW_OK)
               {
                   xResult = CKR_DEVICE_ERROR;
                   return xResult;
               }

               pxObject = (CK_OBJECT_HANDLE_PTR) &HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_ALT_SSS

               if (0 != pxTemplate[Valueindex].ulValueLen)
               {
                    memset(buff, 0, sizeof(buff));
                    memcpy(buff, pxTemplate[Valueindex].pValue, pxTemplate[Valueindex].ulValueLen);
                    buff_len = (size_t) pxTemplate[Valueindex].ulValueLen;

                    keyId = 0x20181015;
                    sss_cipher_type_t cipherType = kSSS_CipherType_RSA;
                    CK_KEY_TYPE key_type = CKK_RSA;
                    size_t keybitlen = 0;
                    CK_ULONG index = 0;
                    xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_KEY_TYPE, &index);
                    if(xResult != CKR_OK) {
                        return xResult;
                    }
                    memcpy(&key_type, pxTemplate[index].pValue, pxTemplate[index].ulValueLen);
                    if(key_type == CKK_RSA) {
                        cipherType = kSSS_CipherType_RSA;
                        mbedtls_rsa_context* pk_rsa = mbedtls_pk_rsa(pk);
                        keybitlen = pk_rsa->len * 8;
                    }
                    else if(key_type == CKK_EC) {
                        //Currently only for NIST-P curves
                        cipherType = kSSS_CipherType_EC_NIST_P;
                        mbedtls_ecp_keypair* pk_ec = mbedtls_pk_ec(pk);
                        switch(pk_ec->grp.id)
                        {
                            case MBEDTLS_ECP_DP_SECP192R1:
                                keybitlen = 192;
                                break;
                            case MBEDTLS_ECP_DP_SECP224R1:
                                keybitlen = 224;
                                break;
                            case MBEDTLS_ECP_DP_SECP256R1:
                                keybitlen = 256;
                                break;
                            case MBEDTLS_ECP_DP_SECP384R1:
                                keybitlen = 384;
                                break;
                            case MBEDTLS_ECP_DP_SECP521R1:
                                keybitlen = 521;
                                break;
                            default:
                                return CKR_ARGUMENTS_BAD;
                        }
                    }
                    else {
                        return CKR_ARGUMENTS_BAD;
                    }

                   status = sss_create_token(&pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->obj, keyId, kSSS_KeyPart_Pair, cipherType, buff, buff_len, keybitlen);

                   if (status != kStatus_SSS_Success) {
                       xResult = CKR_DEVICE_ERROR;
                       return xResult;
                   }

                   *pxObject = keyId;
               }
               else
               {
                   status = sss_key_object_init(&pex_sss_demo_tls_ctx->obj, &pex_sss_demo_boot_ctx->ks);
                   if (status != kStatus_SSS_Success) {
                       LOG_E(" sss_key_object_init for clientCert Failed...");
                       return CKR_FUNCTION_FAILED;
                   }

                   status = sss_key_object_get_handle(&pex_sss_demo_tls_ctx->obj, CLIENT_KEY_PAIR_INDEX);
                   if (status != kStatus_SSS_Success) {
                       LOG_E(" sss_key_object_get_handle  for keyPair Failed...");
                       return CKR_FUNCTION_FAILED;
                   }

                   *pxObject = CLIENT_KEY_PAIR_INDEX;
               }
#endif
             break;

            case CKO_PUBLIC_KEY:

                /*Parses the public key in PEM format and converts it to DER format.
                * This is required because as HLSE understands DER only*/

                // CK_ULONG Valueindex = 0;
                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &Valueindex);
                if(xResult != CKR_OK) {
                    return xResult;
                }

                mbedtls_pk_init( &pk );
                ret = mbedtls_pk_parse_public_key(&pk,pxTemplate[ Valueindex ].pValue,pxTemplate[ Valueindex ].ulValueLen);
                if(ret != 0)
                {
                    xResult = CKR_ARGUMENTS_BAD;
                    break;
                }
                // err = HOSTCRYPTO_GetPublicKey(&pk, pub, &Publen, sizeof(pub)/sizeof(pub[0]));

#if SSS_HAVE_ALT_A71CH

                void *pub = NULL;
                unsigned long Publen = 0;
                err = HLSE_Create_token(keyId,
                                        HLSE_PUBLIC_KEY,
                                        pub,
                                        Publen,
                                        HLSE_Handle_Create_obj);
                if(err != HLSE_SW_OK)
                {
                    xResult = CKR_DEVICE_ERROR;
                    return xResult;
                }

                pxObject = (CK_OBJECT_HANDLE_PTR) &HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_ALT_SSS
                keyId = 0x20181016;
                memset(buff, 0, sizeof(buff));
                memcpy(buff, pxTemplate[Valueindex].pValue, pxTemplate[Valueindex].ulValueLen);
                buff_len = (size_t) pxTemplate[Valueindex].ulValueLen;

                sss_cipher_type_t cipherType = kSSS_CipherType_RSA;
                CK_KEY_TYPE key_type = CKK_RSA;
                size_t keybitlen = 0;
                CK_ULONG index = 0;
                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_KEY_TYPE, &index);
                if(xResult != CKR_OK) {
                    return xResult;
                }
                memcpy(&key_type, pxTemplate[index].pValue, pxTemplate[index].ulValueLen);
                if(key_type == CKK_RSA) {
                    cipherType = kSSS_CipherType_RSA;
                    mbedtls_rsa_context* pk_rsa = mbedtls_pk_rsa(pk);
                    keybitlen = pk_rsa->len * 8;
                }
                else if(key_type == CKK_EC) {
                    //Currently only for NIST-P curves
                    cipherType = kSSS_CipherType_EC_NIST_P;
                    mbedtls_ecp_keypair* pk_ec = mbedtls_pk_ec(pk);
                    switch(pk_ec->grp.id)
                    {
                        case MBEDTLS_ECP_DP_SECP192R1:
                            keybitlen = 192;
                            break;
                        case MBEDTLS_ECP_DP_SECP224R1:
                            keybitlen = 224;
                            break;
                        case MBEDTLS_ECP_DP_SECP256R1:
                            keybitlen = 256;
                            break;
                        case MBEDTLS_ECP_DP_SECP384R1:
                            keybitlen = 384;
                            break;
                        case MBEDTLS_ECP_DP_SECP521R1:
                            keybitlen = 521;
                            break;
                        default:
                            return CKR_ARGUMENTS_BAD;
                    }
                }
                else {
                    return CKR_ARGUMENTS_BAD;
                }

                status = sss_create_token(&pex_sss_demo_boot_ctx->ks,
                                         &pex_sss_demo_tls_ctx->pub_obj,
                                        keyId,
                                        kSSS_KeyPart_Public,
                                        cipherType,
                                        buff,
                                        buff_len,
                                        keybitlen);

                if (status != kStatus_SSS_Success)
                {
                    xResult = CKR_DEVICE_ERROR;
                    return xResult;
                }

                *pxObject =  keyId;
#endif
                break;

            case CKO_SECRET_KEY:

#if SSS_HAVE_ALT_SSS
                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_KEY_TYPE, &i);
                if(xResult != CKR_OK) {
                    return xResult;
                }

                if( 0 != pxTemplate[i].ulValueLen ) {

                    keyId = CLIENT_SYMMETRIC_KEY_INDEX;
                    sss_object_t secretObject = { 0 };
                    status = sss_create_token(&pex_sss_demo_boot_ctx->ks,
                                         &secretObject,
                                        keyId,
                                        kSSS_KeyPart_Default,
                                        kSSS_CipherType_AES,
                                        (uint8_t*) pxTemplate[i].pValue,
                                        pxTemplate[i].ulValueLen,
                                        pxTemplate[i].ulValueLen * 8);

                    // status = sss_key_object_init(&secretObject, &pex_sss_demo_boot_ctx->ks);
                    // if(status != kStatus_SSS_Success) {
                    //     LOG_E("Secret Object init failed");
                    //     break;
                    // }
                    // status = sss_key_object_allocate_handle(&secretObject,
                    //     CLIENT_SYMMETRIC_KEY_INDEX,
                    //     kSSS_KeyPart_Default,
                    //     kSSS_CipherType_AES,
                    //     pxTemplate[i].ulValueLen,
                    //     kKeyObject_Mode_Transient);
                    // if(status != kStatus_SSS_Success) {
                    //     LOG_E("Secret Object init failed");
                    //     break;
                    // }
                    // status = sss_key_store_set_key(&pex_sss_demo_boot_ctx->ks,
                    //     &secretObject,
                    //     (uint8_t*) pxTemplate[i].pValue,
                    //     pxTemplate[i].ulValueLen,
                    //     pxTemplate[i].ulValueLen * 8,
                    //     NULL,
                    //     0);
                    if(status != kStatus_SSS_Success) {
                        LOG_E("Secret Object init failed");
                        break;
                    }
                    *pxObject =  CLIENT_SYMMETRIC_KEY_INDEX;
                }
#endif

                break;

            default:
                xResult = CKR_ARGUMENTS_BAD;
                break;
        }
    }

    return xResult;
}


 /**
 * @brief Query the list of interface function pointers.
 */
CK_DEFINE_FUNCTION( CK_RV, C_GetFunctionList )( CK_FUNCTION_LIST_PTR_PTR ppxFunctionList )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;


    if( NULL == ppxFunctionList )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else
    {
        *ppxFunctionList = &prvP11FunctionList;
    }

    return xResult;
}


/**
 * @brief Initialize the Cryptoki module for use.
 */
CK_DEFINE_FUNCTION( CK_RV, C_Initialize )( CK_VOID_PTR pvInitArgs )
{   /*lint !e9072 It's OK to have different parameter name. */
    // ( void ) ( pvInitArgs );
    CK_RV status = CKR_OK;


    // typedef struct CK_C_INITIALIZE_ARGS {
    //   CK_CREATEMUTEX CreateMutex;
    //   CK_DESTROYMUTEX DestroyMutex;
    //   CK_LOCKMUTEX LockMutex;
    //   CK_UNLOCKMUTEX UnlockMutex;
    //   CK_FLAGS flags;
    //   CK_VOID_PTR pReserved;
    // } CK_C_INITIALIZE_ARGS;
    if(pvInitArgs) {

        CK_C_INITIALIZE_ARGS_PTR initArgs = (CK_C_INITIALIZE_ARGS_PTR)(pvInitArgs);
        if(initArgs->flags & CKF_LIBRARY_CANT_CREATE_OS_THREADS) {
            // return CKR_GENERAL_ERROR;
        //     //Library should not create OS threads. If this is not possible, return CKR_NEED_TO_CREATE_THREADS
#if (__GNUC__  && !AX_EMBEDDED)
            status = CKR_NEED_TO_CREATE_THREADS;
            goto exit;
#endif
        }
        if(initArgs->flags & CKF_OS_LOCKING_OK) {
            // Application will call from multiple threads. Library should use locks.
            if((initArgs->CreateMutex) && (initArgs->DestroyMutex) && (initArgs->LockMutex) && (initArgs->UnlockMutex)) {
                //If mutex pointers are not null, library can use either OS locking or provided functions
                // return CKR_GENERAL_ERROR;
            }
            else if(!(initArgs->CreateMutex) && !(initArgs->DestroyMutex) && !(initArgs->LockMutex) && !(initArgs->UnlockMutex)) {
                //If mutex pointers are null, library must use OS locking.
                // return CKR_CANT_LOCK;
            }
        }
        else {
            // return CKR_GENERAL_ERROR;
        //     //If mutex pointers are null, application would not access the library simultaneously.
        //     //If mutex pointers are not null, library should use provided functions to create threads.
        //      CKR_CANT_LOCK
        //     // status = CKR_ARGUMENTS_BAD;
        //     // return status;
        }
    }

#if (__GNUC__  && !AX_EMBEDDED)
    if(!mutex_initialised){
        int ret = EBUSY;
        while(ret == EBUSY){
            ret = pthread_mutex_init(&gSessionlock, NULL);
        }
        if(ret != 0) {
            LOG_E("Could not initialize mutex");
            status = CKR_CANT_LOCK;
            goto exit;
        }
        while(ret == EBUSY){
            ret = pthread_mutex_init(&gFilelock, NULL);
        }
            // ret = pthread_mutex_init(&gSessionlock, NULL);
    }
    mutex_initialised = true;

exit:
#endif

    return status;
}



/**
 * @brief Un-initialize the Cryptoki module.
 */
CK_DEFINE_FUNCTION( CK_RV, C_Finalize )( CK_VOID_PTR pvReserved )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;

    if( NULL != pvReserved )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }

    return xResult;
}





/**
 * @brief Query the value of the specified cryptographic object attribute.
 */
CK_DEFINE_FUNCTION( CK_RV, C_GetAttributeValue )( CK_SESSION_HANDLE xSession,
    CK_OBJECT_HANDLE xObject,
    CK_ATTRIBUTE_PTR pxTemplate,
    CK_ULONG ulCount )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( xSession );
    CK_VOID_PTR pvAttr = NULL;
    CK_ULONG ulAttrLength = 0;
    mbedtls_pk_type_t xMbedPkType;
    CK_ULONG xP11KeyType, iAttrib, xKeyBitLen, objectClass;
    CK_BBOOL supported = CK_FALSE;

    uint8_t secp256r1_oid[] = {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07};

    // ( void )( xObject );

    /*
     * Enumerate the requested attributes.
     */

    for( iAttrib = 0; iAttrib < ulCount && CKR_OK == xResult; iAttrib++ )
    {
        /*
         * Get the attribute data and size.
         */

        ulAttrLength = 0;
        CK_BBOOL infoUnavailable = CK_FALSE;
        uint8_t data[1024] = { 0 };
        size_t dataLen = sizeof(data);
        size_t KeyBitLen = 2048;
        uint8_t* rsaE = NULL;
        size_t rsaElen;
        uint8_t* rsaN = NULL;
        size_t rsaNlen;
        uint16_t outKeyIndex;
        size_t pubKeyLen;

        switch( pxTemplate[ iAttrib ].type )
        {
        case CKA_KEY_TYPE:

            /*
             * Map the private key type between APIs.
             */
            // xP11KeyType = CKK_RSA;
            // ulAttrLength = sizeof( xP11KeyType );
            // pvAttr = &xP11KeyType;
            // break;

            xMbedPkType = mbedtls_pk_get_type( &pxSession->pxCurrentKey->xMbedPkCtx );

            switch( xMbedPkType )
            {
            case MBEDTLS_PK_RSA:
            case MBEDTLS_PK_RSA_ALT:
            case MBEDTLS_PK_RSASSA_PSS:
                xP11KeyType = CKK_RSA;
                break;

            case MBEDTLS_PK_ECKEY:
            case MBEDTLS_PK_ECKEY_DH:
                xP11KeyType = CKK_EC;
                break;

            case MBEDTLS_PK_ECDSA:
                xP11KeyType = CKK_ECDSA;
                break;

            default:
                xResult = CKR_ATTRIBUTE_VALUE_INVALID;
                break;
            }

            ulAttrLength = sizeof( xP11KeyType );
            pvAttr = &xP11KeyType;
            break;

        case CKA_VALUE:

             switch( xObject )
             {
            /*
             * Assume that the query is for the encoded client certificate.
             */
            case pkcs11OBJECT_HANDLE_CERTIFICATE:
            ulAttrLength = pxSession->pxCurrentKey->pem_len_client + pxSession->pxCurrentKey->pem_len_client_ca; //venky

            pvAttr = ( CK_VOID_PTR )(pxSession->pxCurrentKey->certificate_buf);
            break;

            case pkcs11OBJECT_HANDLE_PUBLIC_KEY:
                if(kStatus_SSS_Success != sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->obj, &data[0], &dataLen, &KeyBitLen))
                {
                    ulAttrLength = 0;
                    xResult = CKR_FUNCTION_FAILED;
                    break;
                }
                pvAttr = (void*) &data[0];
                ulAttrLength = dataLen;

                break;

            default:
            xResult = CKR_ARGUMENTS_BAD;
            break;
             }

            break;

        case CKA_MODULUS_BITS:
        case CKA_PRIME_BITS:

            /*
             * Key strength size query, handled the same for RSA or ECDSA
             * in this port.
             */
            xKeyBitLen = mbedtls_pk_get_bitlen(
                &pxSession->pxCurrentKey->xMbedPkCtx );
            ulAttrLength = sizeof( xKeyBitLen );
            pvAttr = &xKeyBitLen;
            break;

        case CKA_VENDOR_DEFINED:

            /*
             * Return the key context for application-layer use.
             */
            ulAttrLength = sizeof( pxSession->pxCurrentKey->xMbedPkCtx );
            pvAttr = &pxSession->pxCurrentKey->xMbedPkCtx;
            break;

        case CKA_MODULUS:
            if(kStatus_SSS_Success != sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->obj, &data[0], &dataLen, &KeyBitLen))
            {
                ulAttrLength = 0;
                xResult = CKR_FUNCTION_FAILED;
                break;
            }
            if(kStatus_SSS_Success != sss_util_asn1_rsa_parse_public(&data[0], dataLen, &rsaN, &rsaNlen, &rsaE, &rsaElen))
            {
                ulAttrLength = 0;
                xResult = CKR_FUNCTION_FAILED;
                break;
            }
            ulAttrLength = rsaNlen + 1;
            pvAttr = (void *) rsaN;

            break;

        case CKA_PUBLIC_EXPONENT:
            if(kStatus_SSS_Success != sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->obj, &data[0], &dataLen, &KeyBitLen))
            {
                ulAttrLength = 0;
                xResult = CKR_FUNCTION_FAILED;
                break;
            }
            if(kStatus_SSS_Success != sss_util_asn1_rsa_parse_public(&data[0], dataLen, &rsaN, &rsaNlen, &rsaE, &rsaElen))
            {
                ulAttrLength = 0;
                xResult = CKR_FUNCTION_FAILED;
                break;
            }

            ulAttrLength = rsaElen;
            pvAttr = (void *) rsaE;

            break;

        case CKA_EC_PARAMS:
            // ulAttrLength = object_identifiers[2].indentifier_len;
            ulAttrLength = sizeof(secp256r1_oid);
            pvAttr = &secp256r1_oid[0];
            // pvAttr = &object_identifiers[2].identifier[0];
            break;

        case CKA_EC_POINT:
            // sss_status_t status = kStatus_SSS_Fail;
            KeyBitLen = 256;
            if(kStatus_SSS_Success != sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->obj, &data[0], &dataLen, &KeyBitLen))
            {
                ulAttrLength = 0;
                xResult = CKR_FUNCTION_FAILED;
                break;
                // return status;
            }
            if(kStatus_SSS_Success != sss_util_pkcs8_asn1_get_ec_public_key_index(&data[0], dataLen, &outKeyIndex, &pubKeyLen))
            {
                ulAttrLength = 0;
                xResult = CKR_FUNCTION_FAILED;
                break;
                // return status;
            }
            outKeyIndex = outKeyIndex - 2;
            data[outKeyIndex] = 0x04;
            data[outKeyIndex + 1] = pubKeyLen;
            pubKeyLen = pubKeyLen + 2;
            ulAttrLength = pubKeyLen;
            pvAttr = &data[outKeyIndex];
            break;

        case CKA_CLASS:
            if(xObject == pkcs11OBJECT_HANDLE_PRIVATE_KEY) {
                objectClass = CKO_PRIVATE_KEY;
                pvAttr = &objectClass;
                ulAttrLength = sizeof(objectClass);
            }
            else if(xObject == pkcs11OBJECT_HANDLE_PUBLIC_KEY) {
                objectClass = CKO_PUBLIC_KEY;
                pvAttr = &objectClass;
                ulAttrLength = sizeof(objectClass);
            }
            else if(xObject == pkcs11OBJECT_HANDLE_CERTIFICATE) {
                objectClass = CKO_CERTIFICATE;
                pvAttr = &objectClass;
                ulAttrLength = sizeof(objectClass);
            }
            else {
                ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                xResult = CKR_ATTRIBUTE_SENSITIVE;
                infoUnavailable = CK_TRUE;
            }
            break;

        case CKA_ENCRYPT:
        case CKA_DECRYPT:
        case CKA_SIGN:
        case CKA_VERIFY:

            xMbedPkType = mbedtls_pk_get_type( &pxSession->pxCurrentKey->xMbedPkCtx );

            switch( xMbedPkType )
            {
            case MBEDTLS_PK_RSA:
            case MBEDTLS_PK_RSA_ALT:
            case MBEDTLS_PK_RSASSA_PSS:
                supported = CK_TRUE;
                break;

            default:
                supported = CK_FALSE;
                break;
            }

            ulAttrLength = sizeof( supported );
            pvAttr = &(supported);


            break;

        case CKA_WRAP:
        case CKA_UNWRAP:
        case CKA_SIGN_RECOVER:
        case CKA_VERIFY_RECOVER:
        case CKA_DERIVE:
            supported = CK_FALSE;
            ulAttrLength = sizeof( supported );
            pvAttr = &(supported);
            break;




        default:
            // printf("\nAttribute required : %08lx\n", pxTemplate[ iAttrib ].type);
            ulAttrLength = CK_UNAVAILABLE_INFORMATION;
            infoUnavailable = CK_TRUE;

            xResult = CKR_ATTRIBUTE_SENSITIVE;
            break;
        }

        if( CKR_OK == xResult )
        {
            /*
             * Copy out the data and size.
             */

            if( NULL != pxTemplate[ iAttrib ].pValue && !infoUnavailable )
            {
                if( pxTemplate[ iAttrib ].ulValueLen < ulAttrLength )
                {
                    xResult = CKR_BUFFER_TOO_SMALL;
                    ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                }
                else
                {
                    memcpy( pxTemplate[ iAttrib ].pValue, pvAttr, ulAttrLength );
                }
            }

        }
        pxTemplate[ iAttrib ].ulValueLen = ulAttrLength;
        if(rsaN) {
            free(rsaN);
        }
        if(rsaE) {
            free(rsaE);
        }
    }

    return xResult;
}



/**
 * @brief Begin an enumeration sequence for the objects of the specified type.
 */
CK_DEFINE_FUNCTION( CK_RV, C_FindObjectsInit )( CK_SESSION_HANDLE xSession,
                                                CK_ATTRIBUTE_PTR pxTemplate,
                                                CK_ULONG ulCount )
{   /*lint !e9072 It's OK to have different parameter name. */
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( xSession );


    // ( void ) ( ulCount );

    /*
     * Allow filtering on a single object class attribute.
     */

    int classIndex = 0;
    CK_BBOOL foundClass = CK_FALSE;
    pxSession->xFindObjectInit = CK_TRUE;
    pxSession->xFindObjectComplete = CK_FALSE;
    CK_ULONG i = 0;
    for(i = 0; i < ulCount; i++)
    {
        if(pxTemplate[i].type == CKA_LABEL) {
            pxSession->labelPresent = CK_TRUE;
        }
        if(pxTemplate[i].type == CKA_CLASS) {
            classIndex = i;
            foundClass = CK_TRUE;
        }
    }
    if(foundClass) {
        // if(pxTemplate[0].type == CKA_CLASS) {
        memcpy( &pxSession->xFindObjectClass,
                pxTemplate[ classIndex ].pValue,
                sizeof( CK_OBJECT_CLASS ) );

    // }
    }



    return CKR_OK;
}

/**
 * @brief Query the objects of the requested type.
 */
CK_DEFINE_FUNCTION( CK_RV, C_FindObjects )( CK_SESSION_HANDLE xSession,
                                            CK_OBJECT_HANDLE_PTR pxObject,
                                            CK_ULONG ulMaxObjectCount,
                                            CK_ULONG_PTR pulObjectCount )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;

    BaseType_t xDone = pdFALSE;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( xSession );

    /*
     * Check parameters.
     */

    if( ( CK_BBOOL ) CK_FALSE == pxSession->xFindObjectInit )
    {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
        xDone = pdTRUE;
    }

    if( ( pdFALSE == xDone ) && ( 0u == ulMaxObjectCount ) )
    {
        xResult = CKR_ARGUMENTS_BAD;
        xDone = pdTRUE;
    }

    if( ( pdFALSE == xDone ) && ( ( CK_BBOOL ) CK_TRUE == pxSession->xFindObjectComplete ) )
    {
        *pulObjectCount = 0;
        xResult = CKR_OK;
        xDone = pdTRUE;
    }

    /*
     * Load the default private key and certificate.
     */

    // if(pxSession->xFindObjectClass == CKO_PUBLIC_KEY) {
    //     xResult = CKR_FUNCTION_FAILED;
    //     xDone = pdTRUE;
    // }

    if( ( pdFALSE == xDone ) && ( NULL == pxSession->pxCurrentKey ) )
    {
        sss_status_t status = sss_key_object_init(&pex_sss_demo_tls_ctx->dev_cert, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_init for clientCert Failed...");
            return CKR_FUNCTION_FAILED;
        }

        status = sss_key_object_get_handle(&pex_sss_demo_tls_ctx->dev_cert, CLIENT_CERT_INDEX);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_get_handle  for client Cert Failed...");
            return CKR_FUNCTION_FAILED;
        }

        status = sss_key_object_init(&pex_sss_demo_tls_ctx->obj, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_init for clientCert Failed...");
            return CKR_FUNCTION_FAILED;
        }

        status = sss_key_object_get_handle(&pex_sss_demo_tls_ctx->obj, CLIENT_KEY_PAIR_INDEX);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_get_handle  for keyPair Failed...");
            return CKR_FUNCTION_FAILED;
        }


        if( CKR_OK != ( xResult = prvLoadAndInitializeDefaultCertificateAndKey( pxSession ) ) )
        {
            xDone = pdTRUE;
        }
    }

    if( pdFALSE == xDone )
    {
        /*
         * Return object handles based on find type.
         */

        switch( pxSession->xFindObjectClass )
        {
            case CKO_PRIVATE_KEY:
                *pxObject = pkcs11OBJECT_HANDLE_PRIVATE_KEY;
                *pulObjectCount = 1;
                break;

            case CKO_PUBLIC_KEY:
                *pxObject = pkcs11OBJECT_HANDLE_PUBLIC_KEY;
                *pulObjectCount = 1;
                break;

            case CKO_CERTIFICATE:
                *pxObject = pkcs11OBJECT_HANDLE_CERTIFICATE;
                *pulObjectCount = 1;
                break;

            default:
                *pxObject = 0;
                *pulObjectCount = 0;
                break;
        }

        pxSession->xFindObjectComplete = CK_TRUE;
    }

    return xResult;
}

/**
 * @brief Terminate object enumeration.
 */
CK_DEFINE_FUNCTION( CK_RV, C_FindObjectsFinal )( CK_SESSION_HANDLE xSession )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( xSession );

    /*
     * Check parameters.
     */

    if( ( CK_BBOOL ) CK_FALSE == pxSession->xFindObjectInit )
    {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
    }
    else
    {
        /*
         * Clean-up find objects state.
         */

        pxSession->xFindObjectInit = CK_FALSE;
        pxSession->xFindObjectComplete = CK_FALSE;
        pxSession->xFindObjectClass = 0;
    }

    return xResult;
}






/**
 * @brief Start a session for a cryptographic command sequence.
 */
CK_DEFINE_FUNCTION( CK_RV, C_OpenSession )( CK_SLOT_ID xSlotID,
                                            CK_FLAGS xFlags,
                                            CK_VOID_PTR pvApplication,
                                            CK_NOTIFY xNotify,
                                            CK_SESSION_HANDLE_PTR pxSession )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    P11SessionPtr_t pxSessionObj = NULL;


    // ( void ) ( xSlotID );
    // ( void ) ( pvApplication );
    // ( void ) ( xNotify );

    /* Check arguments. */
    if( NULL == pxSession )
    {
       xResult = CKR_ARGUMENTS_BAD;
    }

    /*
     * Make space for the context.
     */
#if defined(USE_RTOS) && USE_RTOS == 1
    if( NULL == ( pxSessionObj = ( P11SessionPtr_t ) pvPortMalloc( sizeof( P11Session_t ) ) ) ) /*lint !e9087 Allow casting void* to other types. */
    {
       xResult = CKR_HOST_MEMORY;
    }
#else
    if( NULL == ( pxSessionObj = ( P11SessionPtr_t ) malloc( sizeof( P11Session_t ) ) ) ) /*lint !e9087 Allow casting void* to other types. */
    {
        xResult = CKR_HOST_MEMORY;
    }
#endif

    /*
     * Assume that there's no performance tradeoff in loading the default key
     * now, since that's the principal use case for opening a session in this
     * provider anyway. This way, the private key can be used for seeding the RNG,
     * especially if there's no hardware-based alternative.
     */

    if( CKR_OK == xResult )
    {
        memset( pxSessionObj, 0, sizeof( P11Session_t ) );

        if( NULL == pxSessionObj )
        {
            xResult = CKR_HOST_MEMORY;
        }
    }

    /*
     * Initialize RNG.
     */

    if( CKR_OK == xResult )
    {
        memset( &pxSessionObj->xMbedEntropyContext,
                0,
                sizeof( pxSessionObj->xMbedEntropyContext ) );

        mbedtls_entropy_init( &pxSessionObj->xMbedEntropyContext );

        mbedtls_ctr_drbg_init( &pxSessionObj->xMbedDrbgCtx );

        if( 0 != mbedtls_ctr_drbg_seed( &pxSessionObj->xMbedDrbgCtx,
                                        mbedtls_entropy_func,
                                        &pxSessionObj->xMbedEntropyContext,
                                        NULL,
                                        0 ) )
        {
            xResult = CKR_FUNCTION_FAILED;
        }
    }

    if( CKR_OK == xResult )
    {
        /*
         * Assign the session.
         */

        pxSessionObj->ulState =
            0u != ( xFlags & CKF_RW_SESSION ) ? CKS_RW_PUBLIC_SESSION : CKS_RO_PUBLIC_SESSION;
        pxSessionObj->xOpened = CK_TRUE;

        /*
         * Return the session.
         */

        *pxSession = ( CK_SESSION_HANDLE ) pxSessionObj; /*lint !e923 Allow casting pointer to integer type for handle. */
    }

    if( ( NULL != pxSessionObj ) && ( CKR_OK != xResult ) )
    {
        free( pxSessionObj );
    }

    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;

/*Lock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__  && !AX_EMBEDDED)
    pthread_mutex_lock(&gSessionlock);
#endif
#ifdef PKCS11_SESSION_OPEN
    if( CKR_OK == xResult ) {

    // if(pex_sss_demo_boot_ctx->session.subsystem == kType_SSS_SubSystem_NONE){
    if(sessionCount == 0){
        sss_status_t sss_status = kStatus_SSS_Fail;
        const char *portName = g_port_name;

        SM_Close(0);
        sss_status = ex_sss_boot_open(pex_sss_demo_boot_ctx, portName);
        if(sss_status != kStatus_SSS_Success)
        {
            LOG_E("Session Open Failed");
            xResult = CKR_GENERAL_ERROR;
            goto error;
        }

#if EX_SSS_BOOT_DO_ERASE
        sss_status = ex_sss_boot_factory_reset(pex_sss_demo_boot_ctx);
        if(sss_status != kStatus_SSS_Success) {
            LOG_E("Factory Reset failed");
        }
#endif

        sss_status = ex_sss_kestore_and_object_init(pex_sss_demo_boot_ctx);
        if(sss_status != kStatus_SSS_Success)
        {
            LOG_E("Keystore Init Failed");
            xResult = CKR_FUNCTION_FAILED;
            goto error;
        }
        pex_sss_demo_tls_ctx->pHost_ks = &pex_sss_demo_boot_ctx->host_ks;
        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->obj, &pex_sss_demo_boot_ctx->ks);
        if(sss_status != kStatus_SSS_Success) {
            LOG_E("Object init failed");
        }
        else {
            sss_status = sss_key_object_get_handle(&pex_sss_demo_tls_ctx->obj, CLIENT_KEY_PAIR_INDEX);
            if(sss_status != kStatus_SSS_Success) {
                // LOG_E("Object get handle failed");
            }
        }
        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->dev_cert, &pex_sss_demo_boot_ctx->ks);
        if(sss_status != kStatus_SSS_Success) {
            LOG_E("Cert Object init failed");
        }
        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->interCaCert, &pex_sss_demo_boot_ctx->ks);
        if(sss_status != kStatus_SSS_Success) {
            LOG_E("IntermCA Object init failed");
        }
        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->pub_obj, &pex_sss_demo_boot_ctx->ks);
        if(sss_status != kStatus_SSS_Success) {
            LOG_E("Pub Object init failed");
        }
        goto exit;
error:
        if(pxSessionObj) {
            free(pxSessionObj);
        }
        if(pex_sss_demo_boot_ctx->session.subsystem != kType_SSS_SubSystem_NONE) {
            ex_sss_session_close(pex_sss_demo_boot_ctx);
        }
    }



    }

exit:
#endif
    if(xResult == CKR_OK) {
        sessionCount++;
    }
/*Unlock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__  && !AX_EMBEDDED)
    pthread_mutex_unlock(&gSessionlock);
    LOG_I("\nDestroyed mutex");
#endif

    return xResult;
}


/**
 * @brief Terminate a session and release resources.
 */
CK_DEFINE_FUNCTION( CK_RV, C_CloseSession )( CK_SESSION_HANDLE xSession )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( xSession );


    if( NULL != pxSession )
    {
        /*
         * Tear down the session.
         */
        /* PKCS11TB macro is required because in one of the PKCS 11 test case causes hard fault
         * since no buffer is allocated for certificate_buf and it causes hard fault when freeing
         * TODO: not deallocate memory when buffer is not allocated */
#if( defined(PKCS11TB) && (PKCS11TB == 0) )
        if ( NULL != pxSession->pxCurrentKey->certificate_buf )        {
#if defined(USE_RTOS) && USE_RTOS == 1
           vPortFree(pxSession->pxCurrentKey->certificate_buf);
#else
           free(pxSession->pxCurrentKey->certificate_buf);
#endif
        }
#endif

        if( NULL != pxSession->pxCurrentKey )        {
            prvFreeKey( pxSession->pxCurrentKey );
        }

        /* Free the public key context if it exists. */
         if(NULL != ( &pxSession->pxCurrentKey->xMbedPkCtx ))
         {
             mbedtls_pk_free( &pxSession->pxCurrentKey->xMbedPkCtx);
         }

        mbedtls_ctr_drbg_free( &pxSession->xMbedDrbgCtx );
#if defined(USE_RTOS) && USE_RTOS == 1
        vPortFree( pxSession );
#else
        free( pxSession );
#endif

/*Lock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__  && !AX_EMBEDDED)
        pthread_mutex_lock(&gSessionlock);
#endif
        if( CKR_OK == xResult ) {
#ifdef PKCS11_SESSION_OPEN//SSS_HAVE_ALT_SSS || SSS_HAVE_ALT_A71CH
        // static const char *g_port_name;
        if(sessionCount == 1) {
            // sss_status_t sss_status = kStatus_SSS_Fail;
            sss_key_object_free(&pex_sss_demo_tls_ctx->obj);
            sss_key_object_free(&pex_sss_demo_tls_ctx->dev_cert);
            sss_key_object_free(&pex_sss_demo_tls_ctx->interCaCert);
            sss_key_object_free(&pex_sss_demo_tls_ctx->pub_obj);
            ex_sss_session_close(pex_sss_demo_boot_ctx);
        }
#endif
        }
    }
    else
    {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    if(xResult == CKR_OK) {
        sessionCount--;
    }
/*Unlock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__  && !AX_EMBEDDED)
    pthread_mutex_unlock(&gSessionlock);
#endif

    return xResult;
}

/**
 * @brief Cleans up a key structure.
 */
static void prvFreeKey( P11KeyPtr_t pxKey )
{
    if( NULL != pxKey )
    {
        /* Restore the internal key context. */
        pxKey->xMbedPkCtx.pk_ctx = pxKey->pvSavedMbedPkCtx;

        /* Clean-up. */
        mbedtls_pk_free( &pxKey->xMbedPkCtx );
        mbedtls_x509_crt_free( &pxKey->xMbedX509Cli );
#if defined(USE_RTOS) && USE_RTOS == 1
        vPortFree( pxKey );
#else
        free( pxKey );
#endif
    }
}


static int awsRetreieve_certificate(unsigned char *pbuf, uint8_t index, uint32_t *outbuflen)
{
#if SSS_HAVE_ALT_A71CH
    int ret = 1;
    HLSE_RET_CODE hlseRc;
    HLSE_OBJECT_HANDLE certHandles[5];
    HLSE_ATTRIBUTE attr;
    U16 certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 certHandlesNum_copy;
    U8 i = 0;
    uint8_t certPattern[] = {0x30, 0x82};
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    if ((hlseRc == HLSE_SW_OK))
    {
        certHandlesNum_copy = certHandlesNum;
        while(certHandlesNum_copy)
        {
            if (HLSE_GET_OBJECT_INDEX(certHandles[i]) == index)
            {
                break;
            }
            i++;
            certHandlesNum_copy--;
        }
        /*can not find the index specified if i >= certHandlesNum*/
        if (i < certHandlesNum)
        {
            attr.type = HLSE_ATTR_OBJECT_VALUE;
            attr.value = pbuf;
            attr.valueLen = *outbuflen;
            hlseRc = HLSE_GetObjectAttribute(certHandles[i], &attr);
            if (hlseRc == HLSE_SW_OK)
            {
                if ((pbuf[0] == certPattern[0]) && (pbuf[1] == certPattern[1]))
                {
                    *outbuflen = attr.valueLen;
                    ret = 0;
                }
            }

        }

    }
    return ret;
#endif

#if SSS_HAVE_ALT_SSS
    sss_status_t ret = kStatus_SSS_Fail;
    int iret = 1;
    size_t KeyBitLen;
    if(index == 0)
    {
        size_t szoutbuflen = *outbuflen;
        KeyBitLen = SIZE_CLIENT_CERTIFICATE * 8;
        ret = sss_key_store_get_key(
            &pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->dev_cert, pbuf, &szoutbuflen, &KeyBitLen);
        *outbuflen = szoutbuflen;
        if (ret != kStatus_SSS_Success) {
            LOG_E(" sss_key_store_get_key Failed...");
            return iret;
        }
    }
    if (index == 1)
    {
        ret = sss_key_object_init(&pex_sss_demo_tls_ctx->interCaCert, &pex_sss_demo_boot_ctx->ks);
        if (ret != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_init for Pub key Failed...");
            return iret;
        }

        ret = sss_key_object_get_handle(&pex_sss_demo_tls_ctx->interCaCert, CLIENT_INTER_CERT_INDEX);
        if (ret != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_get_handle  for client Cert Failed...");
            return iret;
        }

        KeyBitLen = SIZE_INTER_ROOT_CERTIFICATE * 8;
        size_t szoutbuflen = *outbuflen;
        ret = sss_key_store_get_key(
           &pex_sss_demo_boot_ctx->ks, &pex_sss_demo_tls_ctx->interCaCert, pbuf, &szoutbuflen, &KeyBitLen);
        *outbuflen = szoutbuflen;
        if (ret != kStatus_SSS_Success) {
            LOG_E(" sss_key_store_get_key Failed...");
            return iret;
        }
    }

    if(ret == kStatus_SSS_Success)
    {
        iret = 0;
    }
    return iret;
#endif
}
static int convertdertopemformat(const unsigned char * int_ca_cert_der, int len_der, unsigned char * int_ca_cert_pem, int dst_buf_len, U16 *outlength)
{
    int ret;
    size_t len_out;
    const char start[]= "-----BEGIN CERTIFICATE-----\n";
    const char end[]= "\n-----END CERTIFICATE-----\n";

    strcpy((char *)int_ca_cert_pem,start);
    ret = mbedtls_base64_encode(&int_ca_cert_pem[sizeof(start) - 1],dst_buf_len-sizeof(start)-sizeof(end)-1,&len_out,int_ca_cert_der,len_der);
    if (ret == 0)
    {
        strcpy((char *) &int_ca_cert_pem[sizeof(start) - 1 + len_out],end);
        *outlength = len_out + sizeof(start) + sizeof(end) - 1;
    }
    else
    {
        *outlength = 0;
        ret = 1;
    }
    return ret;
}

// static int convertpemtoder( const unsigned char *input, size_t ilen,
//                         unsigned char *output, size_t *olen )
// {
//     int ret;
//     const unsigned char *s1, *s2, *end = input + ilen;
//     size_t len = 0;

//     s1 = (unsigned char *) strstr( (const char *) input, "-----BEGIN" );
//     if( s1 == NULL )
//         return( -1 );

//     s2 = (unsigned char *) strstr( (const char *) input, "-----END" );
//     if( s2 == NULL )
//         return( -1 );

//     s1 += 10;
//     while( s1 < end && *s1 != '-' )
//         s1++;
//     while( s1 < end && *s1 == '-' )
//         s1++;
//     if( *s1 == '\r' ) s1++;
//     if( *s1 == '\n' ) s1++;

//     if( s2 <= s1 || s2 > end )
//         return( -1 );

//     ret = mbedtls_base64_decode( NULL, 0, &len, (const unsigned char *) s1, s2 - s1 );
//     if( ret == MBEDTLS_ERR_BASE64_INVALID_CHARACTER )
//         return( ret );

//     if( len > *olen )
//         return( -1 );

//     if( ( ret = mbedtls_base64_decode( output, len, &len, (const unsigned char *) s1,
//                                s2 - s1 ) ) != 0 )
//     {
//         return( ret );
//     }

//     *olen = len;

//     return( 0 );
// }


#if AX_EMBEDDED
BaseType_t PKCS11_PAL_SaveFile( char * pcFileName,
        uint8_t * pucData,
        uint32_t ulDataSize )
{
    return false;
}
#endif /* AX_EMBEDDED */

/**
 * @brief Begin a digital signature verification session.
 */

CK_DEFINE_FUNCTION( CK_RV, C_VerifyInit )( CK_SESSION_HANDLE xSession,
                                           CK_MECHANISM_PTR pxMechanism,
                                           CK_OBJECT_HANDLE xKey )
{
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( xSession );
        // pxSession->xOperationInProgress = pxMechanism->mechanism;

    /*lint !e9072 It's OK to have different parameter name. */
    // ( void ) ( xSession );
    // ( void ) ( xKey );

    if( NULL == pxMechanism )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if(pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pxMechanism->mechanism;
        pxSession->xOperationKeyHandle = xKey;
        if(pxMechanism->pParameter) {
            pxSession->mechParameter = pxMechanism->pParameter;
            pxSession->mechParameterLen = pxMechanism->ulParameterLen;
        }
        else {
            pxSession->mechParameterLen = 0;
        }
    }

    return xResult;
}

CK_RV ParseSignMechanism(P11SessionPtr_t pxSession, sss_algorithm_t* algorithm)
{
    CK_RV xResult = CKR_OK;
    switch(pxSession->xOperationInProgress)
    {
        case CKM_RSA_PKCS:
            // xResult = CKR_DEVICE_ERROR;
            *algorithm = kAlgorithm_SSS_RSASSA_NO_PADDING;
            // *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256;
            break;
        case CKM_RSA_PKCS_PSS:
            /*Also need the details about hash algorithm used*/
            if(!pxSession->mechParameterLen) {
                xResult = CKR_ARGUMENTS_BAD;
                break;
            }
            CK_RSA_PKCS_PSS_PARAMS_PTR param = (CK_RSA_PKCS_PSS_PARAMS_PTR) pxSession->mechParameter;
            switch(param->hashAlg)
            {
                case CKM_SHA_1:
                    *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1;
                    break;
                case CKM_SHA224:
                    *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224;
                    break;
                case CKM_SHA256:
                    *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256;
                    break;
                case CKM_SHA384:
                    *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384;
                    break;
                case CKM_SHA512:
                    *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512;
                    break;
                default:
                    xResult = CKR_MECHANISM_INVALID;
                    break;
            }
            break;

        case CKM_SHA1_RSA_PKCS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1;

            break;
        case CKM_SHA1_RSA_PKCS_PSS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1;
            break;

        case CKM_SHA256_RSA_PKCS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256;

            break;
        case CKM_SHA256_RSA_PKCS_PSS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256;
            break;

        case CKM_SHA384_RSA_PKCS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384;

            break;
        case CKM_SHA384_RSA_PKCS_PSS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384;
            break;

        case CKM_SHA512_RSA_PKCS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512;

            break;
        case CKM_SHA512_RSA_PKCS_PSS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512;
            break;

        case CKM_SHA224_RSA_PKCS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224;

            break;
        case CKM_SHA224_RSA_PKCS_PSS:
            *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224;
            break;

        case CKM_ECDSA:
            break;

        default:
            xResult = CKR_MECHANISM_INVALID;
            break;
    }
    return xResult;

}

CK_RV ParseEncryptionMechanism(P11SessionPtr_t pxSession, sss_algorithm_t* algorithm)
{
    CK_RV xResult = CKR_OK;
    switch(pxSession->xOperationInProgress)
    {
        case CKM_RSA_PKCS:
        case CKM_SHA1_RSA_PKCS:
        case CKM_SHA256_RSA_PKCS:
        case CKM_SHA384_RSA_PKCS:
        case CKM_SHA512_RSA_PKCS:
        case CKM_SHA224_RSA_PKCS:
            *algorithm = kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA1;
            break;

        case CKM_AES_ECB:
            *algorithm = kAlgorithm_SSS_AES_ECB;
            break;
        case CKM_AES_CBC:
            *algorithm = kAlgorithm_SSS_AES_CBC;
            break;
        case CKM_AES_CTR:
            *algorithm = kAlgorithm_SSS_AES_CTR;
            break;

        default:
            xResult = CKR_MECHANISM_INVALID;
            break;
    }
    return xResult;

}

CK_RV ParseDigestMechanism(P11SessionPtr_t pxSession, sss_algorithm_t* algorithm)
{
    CK_RV xResult = CKR_OK;
    switch(pxSession->xOperationInProgress)
    {
        case CKM_SHA_1:
            *algorithm = kAlgorithm_SSS_SHA1;
            break;
        case CKM_SHA224:
            *algorithm = kAlgorithm_SSS_SHA224;
            break;
        case CKM_SHA256:
            *algorithm = kAlgorithm_SSS_SHA256;
            break;
        case CKM_SHA384:
            *algorithm = kAlgorithm_SSS_SHA384;
            break;
        case CKM_SHA512:
            *algorithm = kAlgorithm_SSS_SHA512;
            break;

        default:
            xResult = CKR_MECHANISM_INVALID;
            break;
    }
    return xResult;

}

/**
 * @brief Begin a digital signature generation session.
 */
CK_DEFINE_FUNCTION( CK_RV, C_SignInit )( CK_SESSION_HANDLE xSession,
                                         CK_MECHANISM_PTR pxMechanism,
                                         CK_OBJECT_HANDLE xKey )
{
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( xSession );
        // pxSession->xOperationInProgress = pxMechanism->mechanism;

    /*lint !e9072 It's OK to have different parameter name. */
    // ( void ) ( xSession );
    // ( void ) ( xKey );

    if( NULL == pxMechanism )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if(pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pxMechanism->mechanism;
        if(pxMechanism->pParameter) {
            pxSession->mechParameter = pxMechanism->pParameter;
            pxSession->mechParameterLen = pxMechanism->ulParameterLen;
        }
        else {
            pxSession->mechParameterLen = 0;
        }
    }

    return xResult;
}

/**
 * @brief Query the list of slots. A single default slot is implemented.
 */
CK_DEFINE_FUNCTION( CK_RV, C_GetSlotList )( CK_BBOOL xTokenPresent,
                                            CK_SLOT_ID_PTR pxSlotList,
                                            CK_ULONG_PTR pulCount )
{   /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;


    // ( void ) ( xTokenPresent );

    if( NULL == pulCount )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else if( NULL == pxSlotList )
    {
        *pulCount = 1;
    }
    else
    {
        // if( 0u == *pulCount )
        if(*pulCount < 1)
        {
            xResult = CKR_BUFFER_TOO_SMALL;
        }
        else
        {
            pxSlotList[ 0 ] = (CK_ULONG) pkcs11SLOT_ID;
            *pulCount = 1;
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_CopyObject)(
  CK_SESSION_HANDLE hSession,
  CK_OBJECT_HANDLE hObject,
  CK_ATTRIBUTE_PTR pTemplate,
  CK_ULONG ulCount,
  CK_OBJECT_HANDLE_PTR phNewObject)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_Decrypt)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pEncryptedData,
  CK_ULONG ulEncryptedDataLen,
  CK_BYTE_PTR pData,
  CK_ULONG_PTR pulDataLen)
{
    CK_RV xResult = CKR_OK;
    // return CKR_ARGUMENTS_BAD;

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle( hSession );

    if(!pEncryptedData) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }

    // sss_status_t status;
    // sss_asymmetric_t asymmCtx;
    sss_algorithm_t algorithm = kAlgorithm_None;
    // sss_algorithm_t digest_algorithm = kAlgorithm_None;

    xResult = ParseEncryptionMechanism(pxSessionObj, &algorithm);
    if(xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }
    if(algorithm == kAlgorithm_None) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }
    if((algorithm == kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA1) || (algorithm == kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1)) {
        /*RSA Encryption*/
        xResult = AsymmetricDecrypt(pxSessionObj, algorithm, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
    }
    else {
        /*Symmetric Encryption*/
        xResult = SymmetricDecrypt(pxSessionObj, algorithm, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
    }

    return xResult;

}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptFinal)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pLastPart,
  CK_ULONG_PTR pulLastPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptInit)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_OBJECT_HANDLE hKey)
{
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( hSession );

    if( NULL == pMechanism )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if(pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pMechanism->mechanism;
        if(pMechanism->pParameter) {
            pxSession->mechParameter = pMechanism->pParameter;
            pxSession->mechParameterLen = pMechanism->ulParameterLen;
        }
        else {
            pxSession->mechParameterLen = 0;
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pEncryptedPart,
  CK_ULONG ulEncryptedPartLen,
  CK_BYTE_PTR pPart,
  CK_ULONG_PTR pulPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DeriveKey)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_OBJECT_HANDLE hBaseKey,
  CK_ATTRIBUTE_PTR pTemplate,
  CK_ULONG ulAttributeCount,
  CK_OBJECT_HANDLE_PTR phKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_Digest)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pData,
  CK_ULONG ulDataLen,
  CK_BYTE_PTR pDigest,
  CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult = CKR_OK;


    if(!pData) {
        return CKR_ARGUMENTS_BAD;
    }

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    sss_status_t status = kStatus_SSS_Fail;
    sss_algorithm_t algorithm;
    sss_digest_t digestCtx;
    xResult = ParseDigestMechanism(pxSessionObj, &algorithm);
    if(xResult != CKR_OK) {
        return xResult;
    }
    uint8_t* input = (uint8_t*) malloc(ulDataLen * sizeof(uint8_t));
    uint8_t output[64] = { 0 };
    size_t inputLen = ulDataLen;
    size_t outputLen = sizeof(output);

    status = sss_digest_context_init(&digestCtx,
        &pex_sss_demo_boot_ctx->session,
        algorithm,
        kMode_SSS_Digest);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        xResult = CKR_DEVICE_ERROR;
        goto cleanup;
    }
    status = sss_digest_one_go(&digestCtx,
        input,
        inputLen,
        &output[0],
        &outputLen);
    if(status != kStatus_SSS_Success) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        xResult = CKR_DEVICE_ERROR;
        goto cleanup;
    }

    if(pDigest) {
        if(*pulDigestLen < outputLen) {
            xResult = CKR_BUFFER_TOO_SMALL;
        }
        else {
            memcpy(pDigest, &output[0], outputLen);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        }
    }
    *pulDigestLen = outputLen;


cleanup:
    sss_digest_context_free(&digestCtx);
    if(input) {
        free(input);
    }

    return xResult;

}

CK_DEFINE_FUNCTION(CK_RV, C_DigestKey)(
  CK_SESSION_HANDLE hSession,
  CK_OBJECT_HANDLE hKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_Encrypt)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pData,
  CK_ULONG ulDataLen,
  CK_BYTE_PTR pEncryptedData,
  CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle( hSession );

    if(!pData) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }

    // sss_status_t status;
    sss_algorithm_t algorithm = kAlgorithm_None;

    xResult = ParseEncryptionMechanism(pxSessionObj, &algorithm);
    if(xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }
    if(algorithm == kAlgorithm_None) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }
    if((algorithm == kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA1) || (algorithm == kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1)) {
        /*RSA Encryption*/
        xResult = AsymmetricEncrypt(pxSessionObj, algorithm, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
    }
    else {
        /*Symmetric Encryption*/
        xResult = SymmetricEncrypt(pxSessionObj, algorithm, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
    }


    return xResult;

}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptFinal)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pLastEncryptedPart,
  CK_ULONG_PTR pulLastEncryptedPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptInit)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_OBJECT_HANDLE hKey)
{
    CK_RV xResult = CKR_OK;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle( hSession );

    if( NULL == pMechanism )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if(pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pMechanism->mechanism;
        if(pMechanism->pParameter) {
            pxSession->mechParameter = pMechanism->pParameter;
            pxSession->mechParameterLen = pMechanism->ulParameterLen;
        }
        else {
            pxSession->mechParameterLen = 0;
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pPart,
  CK_ULONG ulPartLen,
  CK_BYTE_PTR pEncryptedPart,
  CK_ULONG_PTR pulEncryptedPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GenerateKey)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_ATTRIBUTE_PTR pTemplate,
  CK_ULONG ulCount,
  CK_OBJECT_HANDLE_PTR phKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GenerateKeyPair)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_ATTRIBUTE_PTR pPublicKeyTemplate,
  CK_ULONG ulPublicKeyAttributeCount,
  CK_ATTRIBUTE_PTR pPrivateKeyTemplate,
  CK_ULONG ulPrivateKeyAttributeCount,
  CK_OBJECT_HANDLE_PTR phPublicKey,
  CK_OBJECT_HANDLE_PTR phPrivateKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetInfo)(CK_INFO_PTR pInfo)
{

    /*typedef struct CK_INFO {
      // manufacturerID and libraryDecription have been changed from
      // CK_CHAR to CK_UTF8CHAR for v2.10
      CK_VERSION    cryptokiVersion;     // Cryptoki interface ver
      CK_UTF8CHAR   manufacturerID[32];  // blank padded
      CK_FLAGS      flags;               // must be zero

      CK_UTF8CHAR   libraryDescription[32];  // blank padded
      CK_VERSION    libraryVersion;          // version of library
    } CK_INFO;
    */
    if(!pInfo) {
        LOG_E("Null pointer");
        return CKR_ARGUMENTS_BAD;
    }

    pInfo->cryptokiVersion.major = CRYPTOKI_VERSION_MAJOR;
    pInfo->cryptokiVersion.minor = CRYPTOKI_VERSION_MINOR;
    memset(&pInfo->manufacturerID[0], ' ', sizeof(pInfo->manufacturerID));
    memset(&pInfo->libraryDescription[0], ' ', sizeof(pInfo->libraryDescription));
    pInfo->flags = 0;
    // { CRYPTOKI_VERSION_MAJOR, CRYPTOKI_VERSION_MINOR },
    return CKR_OK;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismInfo)(
  CK_SLOT_ID slotID,
  CK_MECHANISM_TYPE type,
  CK_MECHANISM_INFO_PTR pInfo)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismList)(
  CK_SLOT_ID slotID,
  CK_MECHANISM_TYPE_PTR pMechanismList,
  CK_ULONG_PTR pulCount)
{

    // return CKR_FUNCTION_NOT_SUPPORTED;
    CK_RV xResult = CKR_OK;

    if(!pulCount) {
        return CKR_ARGUMENTS_BAD;
    }

    CK_MECHANISM_TYPE mechanisms[] = {CKM_RSA_PKCS, CKM_SHA1_RSA_PKCS, CKM_SHA224_RSA_PKCS, \
        CKM_SHA256_RSA_PKCS, CKM_SHA384_RSA_PKCS, CKM_SHA512_RSA_PKCS, \
        CKM_RSA_PKCS_PSS, CKM_SHA1_RSA_PKCS_PSS, CKM_SHA224_RSA_PKCS_PSS, \
        CKM_SHA256_RSA_PKCS_PSS, CKM_SHA384_RSA_PKCS_PSS, CKM_SHA512_RSA_PKCS_PSS, \
        CKM_AES_ECB, CKM_AES_CBC, CKM_AES_CTR,
        CKM_SHA_1, CKM_SHA224, CKM_SHA256, CKM_SHA384, CKM_SHA512};
    if(pMechanismList) {
        if(*pulCount < sizeof(mechanisms)) {
            xResult = CKR_BUFFER_TOO_SMALL;
        }
        else {
            memcpy(pMechanismList, &mechanisms[0], sizeof(mechanisms));
        }
    }
    *pulCount = sizeof(mechanisms);
    return xResult;
}



CK_DEFINE_FUNCTION(CK_RV, C_GetSlotInfo)(
  CK_SLOT_ID slotID,
  CK_SLOT_INFO_PTR pInfo)
{

    // typedef struct CK_SLOT_INFO {
    //   /* slotDescription and manufacturerID have been changed from
    //    * CK_CHAR to CK_UTF8CHAR for v2.10 */
    //   CK_UTF8CHAR   slotDescription[64];  /* blank padded */
    //   CK_UTF8CHAR   manufacturerID[32];   /* blank padded */
    //   CK_FLAGS      flags;

    //   CK_VERSION    hardwareVersion;  /* version of hardware */
    //   CK_VERSION    firmwareVersion;  /* version of firmware */
    // } CK_SLOT_INFO;
    if(slotID != 1) {
        return CKR_SLOT_ID_INVALID;
    }
    memset(&pInfo->slotDescription[0], ' ', sizeof(pInfo->slotDescription));
    memset(&pInfo->manufacturerID[0], ' ', sizeof(pInfo->manufacturerID));
    pInfo->flags = CKF_TOKEN_PRESENT | CKF_REMOVABLE_DEVICE | CKF_HW_SLOT;
    memcpy(&pInfo->manufacturerID, "abcd", sizeof("abcd"));
    return CKR_OK;
}



CK_DEFINE_FUNCTION(CK_RV, C_GetTokenInfo)(
  CK_SLOT_ID slotID,
  CK_TOKEN_INFO_PTR pInfo)
{

    // typedef struct CK_TOKEN_INFO {
    //   /* label, manufacturerID, and model have been changed from
    //    * CK_CHAR to CK_UTF8CHAR for v2.10 */
    //   CK_UTF8CHAR   label[32];           /* blank padded */
    //   CK_UTF8CHAR   manufacturerID[32];  /* blank padded */
    //   CK_UTF8CHAR   model[16];           /* blank padded */
    //   CK_CHAR       serialNumber[16];    /* blank padded */
    //   CK_FLAGS      flags;               /* see below */

    //   CK_ULONG      ulMaxSessionCount;     /* max open sessions */
    //   CK_ULONG      ulSessionCount;        /* sess. now open */
    //   CK_ULONG      ulMaxRwSessionCount;    max R/W sessions
    //   CK_ULONG      ulRwSessionCount;      /* R/W sess. now open */
    //   CK_ULONG      ulMaxPinLen;           /* in bytes */
    //   CK_ULONG      ulMinPinLen;           /* in bytes */
    //   CK_ULONG      ulTotalPublicMemory;   /* in bytes */
    //   CK_ULONG      ulFreePublicMemory;    /* in bytes */
    //   CK_ULONG      ulTotalPrivateMemory;  /* in bytes */
    //   CK_ULONG      ulFreePrivateMemory;   /* in bytes */
    //   CK_VERSION    hardwareVersion;       /* version of hardware */
    //   CK_VERSION    firmwareVersion;       /* version of firmware */
    //   CK_CHAR       utcTime[16];           /* time */
    // } CK_TOKEN_INFO;
    // memset(&pInfo->label[0], ' ', sizeof(pInfo->label));
    memset(&pInfo->manufacturerID[0], ' ', sizeof(pInfo->label));
    // memset(&pInfo->model[0], ' ', sizeof(pInfo->label));
    // memset(&pInfo->serialNumber[0], ' ', sizeof(pInfo->label));
    uint8_t label[32] = {"greengrass                      "};
    memcpy(&pInfo->label[0], label, sizeof(label));
    // memset(&pInfo->label[sizeof(label)], ' ', sizeof(pInfo->label) - sizeof(label));
    pInfo->ulMaxSessionCount = 1;
    pInfo->ulMaxRwSessionCount = 1;
    pInfo->ulMaxPinLen = 10;
    pInfo->ulMinPinLen = 0;
    pInfo->flags = CKF_RNG | CKF_LOGIN_REQUIRED | CKF_TOKEN_INITIALIZED;

    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_Login)(
  CK_SESSION_HANDLE hSession,
  CK_USER_TYPE userType,
  CK_UTF8CHAR_PTR pPin,
  CK_ULONG ulPinLen)
{

    // /* Security Officer */
    // #define CKU_SO          0UL
    // /* Normal user */
    // #define CKU_USER        1UL
    // /* Context specific */
    // #define CKU_CONTEXT_SPECIFIC    2UL
    // return CKR_GENERAL_ERROR;

    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_Logout)(
  CK_SESSION_HANDLE hSession)
{

    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_SeedRandom)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pSeed,
  CK_ULONG ulSeedLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SetAttributeValue)(
  CK_SESSION_HANDLE hSession,
  CK_OBJECT_HANDLE hObject,
  CK_ATTRIBUTE_PTR pTemplate,
  CK_ULONG ulCount)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignFinal)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pSignature,
  CK_ULONG_PTR pulSignatureLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pPart,
  CK_ULONG ulPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_UnwrapKey)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_OBJECT_HANDLE hUnwrappingKey,
  CK_BYTE_PTR pWrappedKey,
  CK_ULONG ulWrappedKeyLen,
  CK_ATTRIBUTE_PTR pTemplate,
  CK_ULONG ulAttributeCount,
  CK_OBJECT_HANDLE_PTR phKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyFinal)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pSignature,
  CK_ULONG ulSignatureLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pPart,
  CK_ULONG ulPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_WrapKey)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_OBJECT_HANDLE hWrappingKey,
  CK_OBJECT_HANDLE hKey,
  CK_BYTE_PTR pWrappedKey,
  CK_ULONG_PTR pulWrappedKeyLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}



CK_DEFINE_FUNCTION(CK_RV, C_InitToken)(
  CK_SLOT_ID slotID,
  CK_UTF8CHAR_PTR pPin,
  CK_ULONG ulPinLen,
  CK_UTF8CHAR_PTR pLabel)
{

    char label[32] = { 0 };
    char* p_label = &label[0];
    memcpy(&label[0], pLabel, sizeof(label));
    if(!strncmp(p_label, "greengrass", sizeof("greengrass")))
    {
        LOG_I("\nGreengrass label found!\n");
        // sss_status_t sss_status = kStatus_SSS_Fail;
        // sss_object_t sss_object = { 0 };
        // sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        // if(sss_status != kStatus_SSS_Success)
        // {
        //     return CKR_FUNCTION_FAILED;
        //     //CK_TOKEN_INFO
        // }
        // sss_status = sss_key_object_get_handle(&sss_object, 0x20181001);
        // if(sss_status != kStatus_SSS_Success)
        // {
        //     return CKR_TOKEN_NOT_PRESENT;
        // }
    }
    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_InitPIN)(
  CK_SESSION_HANDLE hSession,
  CK_UTF8CHAR_PTR pPin,
  CK_ULONG ulPinLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SetPIN)(
  CK_SESSION_HANDLE hSession,
  CK_UTF8CHAR_PTR pOldPin,
  CK_ULONG ulOldLen,
  CK_UTF8CHAR_PTR pNewPin,
  CK_ULONG ulNewLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_CloseAllSessions)(
  CK_SLOT_ID slotID)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetSessionInfo)(
  CK_SESSION_HANDLE hSession,
  CK_SESSION_INFO_PTR pInfo)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetOperationState)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pOperationState,
  CK_ULONG_PTR pulOperationStateLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SetOperationState)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pOperationState,
  CK_ULONG ulOperationStateLen,
  CK_OBJECT_HANDLE hEncryptionKey,
  CK_OBJECT_HANDLE hAuthenticationKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetObjectSize)(
  CK_SESSION_HANDLE hSession,
  CK_OBJECT_HANDLE hObject,
  CK_ULONG_PTR pulSize)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignRecoverInit)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_OBJECT_HANDLE hKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignRecover)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pData,
  CK_ULONG ulDataLen,
  CK_BYTE_PTR pSignature,
  CK_ULONG_PTR pulSignatureLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecoverInit)(
  CK_SESSION_HANDLE hSession,
  CK_MECHANISM_PTR pMechanism,
  CK_OBJECT_HANDLE hKey)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecover)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pSignature,
  CK_ULONG ulSignatureLen,
  CK_BYTE_PTR pData,
  CK_ULONG_PTR pulDataLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestEncryptUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pPart,
  CK_ULONG ulPartLen,
  CK_BYTE_PTR pEncryptedPart,
  CK_ULONG_PTR pulEncryptedPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptDigestUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pEncryptedPart,
  CK_ULONG ulEncryptedPartLen,
  CK_BYTE_PTR pPart,
  CK_ULONG_PTR pulPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignEncryptUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pPart,
  CK_ULONG ulPartLen,
  CK_BYTE_PTR pEncryptedPart,
  CK_ULONG_PTR pulEncryptedPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptVerifyUpdate)(
  CK_SESSION_HANDLE hSession,
  CK_BYTE_PTR pEncryptedPart,
  CK_ULONG ulEncryptedPartLen,
  CK_BYTE_PTR pPart,
  CK_ULONG_PTR pulPartLen)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionStatus)(
  CK_SESSION_HANDLE hSession)
{

    return CKR_FUNCTION_NOT_PARALLEL;
}

CK_DEFINE_FUNCTION(CK_RV, C_CancelFunction)(
  CK_SESSION_HANDLE hSession)
{

    return CKR_FUNCTION_NOT_PARALLEL;
}

CK_DEFINE_FUNCTION(CK_RV, C_WaitForSlotEvent)(
  CK_FLAGS flags,
  CK_SLOT_ID_PTR pSlot,
  CK_VOID_PTR pReserved)
{

    return CKR_FUNCTION_NOT_SUPPORTED;
}


#endif /* TGT_A71CH */
