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

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if (SSS_HAVE_SSCP || SSS_HAVE_APPLET_SE05X_IOT)
/* FreeRTOS includes. */
#if defined(USE_RTOS) && USE_RTOS == 1
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#endif
#include "core_pkcs11.h"

/* mbedTLS includes. */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/base64.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/oid.h"
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"
#include "pkcs11_mbedtls_utils.h"

/*Other includes */
#include <nxEnsure.h>
#include <nxLog_App.h>

#include "HLSEAPI.h"
#include "ex_sss.h"
#if SSS_HAVE_ALT_A71CH
#include "ax_mbedtls.h"
#endif
#if SSS_HAVE_ALT_SSS
#include "sss_mbedtls.h"
#endif

#if SSS_HAVE_APPLET_SE05X_IOT
#include <fsl_sss_se05x_apis.h>
#include <se05x_APDU.h>
#include <se05x_const.h>

#include "se05x_APDU_apis.h"
#include "se05x_enums.h"
#endif

#if defined(PKCS11_LIBRARY)
#include <projdefs.h>

#if (__GNUC__ && !AX_EMBEDDED)
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
/* Only for base session with os */
static pthread_mutex_t gSessionlock;
static pthread_mutex_t gFilelock;
#endif

#endif

#if defined(USE_RTOS) && USE_RTOS == 1
static SemaphoreHandle_t xSemaphore;
#endif

/* C runtime includes. */
#include <PlugAndTrust_Pkg_Ver.h>
#include <fsl_sss_util_asn1_der.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Remove this later */
#include <ex_sss_boot.h>
extern ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx;
extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;
extern const char *g_port_name;

#undef DEBUG_PKCS11_PAL

#define PKCS11_TOKEN_LABEL                               \
    {                                                    \
        'S', 'S', 'S', '_', 'P', 'K', 'C', 'S', '1', '1' \
    }
#define PKCS11_MANUFACTURER \
    {                       \
        'N', 'X', 'P'       \
    }

#define PKCS11_LIBRARY_VERSION                                            \
    (CK_VERSION)                                                          \
    {                                                                     \
        .major = PLUGANDTRUST_VER_MAJOR, .minor = PLUGANDTRUST_VER_MINOR, \
    }

#define pkcs11SLOT_ID 1
#define pkcs11OBJECT_HANDLE_PUBLIC_KEY 1
#define pkcs11OBJECT_HANDLE_PRIVATE_KEY 2
#define pkcs11OBJECT_HANDLE_CERTIFICATE 3
#define pkcs11OBJECT_HANDLE_SYMMETRIC 4

#define SIZE_CLIENT_CERTIFICATE 500
#define SIZE_INTER_ROOT_CERTIFICATE 900

#define CKA_SSS_ID CKA_VENDOR_DEFINED + CKA_OBJECT_ID
/**
 * @brief Definitions for parameter checking
 */
#define pkcs11CREATEOBJECT_MINIMUM_ATTRIBUTE_COUNT 2

/**
 * Defines for the set key pair through create object
 */
#define pkcs11CREATEOBJECT_SET_KEYPAIR_LENGTH 138
#define pkcs11CREATEOBJECT_SET_PUBKEY_LENGTH 65
#define pkcs11CREATEOBJECT_PUBLIC_KEY_INDEX 32

#define pkcs11NO_OPERATION ((CK_MECHANISM_TYPE)0xFFFFFFFFF)
#define pkcs11INVALID_OBJECT_CLASS ((CK_OBJECT_CLASS)0x0FFFFFFF)
#define pkcs11INVALID_KEY_TYPE ((CK_KEY_TYPE)0x0FFFFFFF)
#if defined(USE_RTOS) && USE_RTOS == 1
#define LOCK_MUTEX_FOR_RTOS if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
#else
#define LOCK_MUTEX_FOR_RTOS
#endif

#if defined(USE_RTOS) && USE_RTOS == 1
#define UNLOCK_MUTEX_FOR_RTOS_RET(RET) \
    xSemaphoreGive(xSemaphore);        \
    return RET;
#else
#define UNLOCK_MUTEX_FOR_RTOS_RET(RET) return RET;
#endif

#if defined(USE_RTOS) && USE_RTOS == 1
#define UNLOCK_MUTEX_FOR_RTOS xSemaphoreGive(xSemaphore);
#else
#define UNLOCK_MUTEX_FOR_RTOS
#endif

#if defined(USE_RTOS) && USE_RTOS == 1
#define UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(CONDITION) \
    if (!(CONDITION)) {                               \
        NX_ENSURE_MESSAGE(#CONDITION);                \
        xSemaphoreGive(xSemaphore);                   \
        goto exit;                                    \
    }
#else
#define UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(CONDITION) ENSURE_OR_GO_EXIT(CONDITION)
#endif

#define ASN1_SKIP_TO_NEXT_TAG(pTLV, taglen)            \
    {                                                  \
        if (taglen < 0x7F) {                           \
            pTLV += taglen + 1 + 1 /* Length byte */;  \
        }                                              \
        else if (taglen < 0xFF) {                      \
            pTLV += taglen + 1 + 2 /* Length bytes */; \
        }                                              \
        else {                                         \
            pTLV += taglen + 1 + 3 /* Length bytes */; \
        }                                              \
    }

/**
 * Defines OpenSC NON_REPUDIATION attribute
 */
#define SC_VENDOR_DEFINED 0x4F534300 /* OSC */
// CKA_OPENSC_NON_REPUDIATION for OpenSC 0.17
#define CKA_OPENSC_NON_REPUDIATION_0_17 (CKA_VENDOR_DEFINED | 1UL)
// CKA_OPENSC_NON_REPUDIATION for OpenSC 0.21
#define CKA_OPENSC_NON_REPUDIATION_0_21 (CKA_VENDOR_DEFINED | SC_VENDOR_DEFINED | 1UL)

// uint8_t nist_header_start[] = { 0x30, 0x81, 0x87, 0x02, 0x01, 0x00, 0x30, 0x13,
//         0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02,
//         0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
//         0x03, 0x01, 0x07, 0x04, 0x6D, 0x30, 0x6B, 0x02,
//         0x01, 0x01, 0x04, 0x20};

// uint8_t nist_header_end[] = {0xA1, 0x44, 0x03, 0x42, 0x00 };

#define DEFAULT_POLICY_SYMM_KEY                                                                                       \
    (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_SIGN | POLICY_OBJ_ALLOW_VERIFY | POLICY_OBJ_ALLOW_ENC |               \
        POLICY_OBJ_ALLOW_DEC | POLICY_OBJ_ALLOW_KDF | POLICY_OBJ_ALLOW_WRAP | POLICY_OBJ_ALLOW_WRITE |                \
        POLICY_OBJ_ALLOW_GEN | POLICY_OBJ_ALLOW_DESFIRE_AUTHENTICATION | POLICY_OBJ_ALLOW_DESFIRE_DUMP_SESSION_KEYS | \
        POLICY_OBJ_ALLOW_IMPORT_EXPORT)
#define DEFAULT_POLICY_ASYMM_KEY                                                                        \
    (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_SIGN | POLICY_OBJ_ALLOW_VERIFY | POLICY_OBJ_ALLOW_ENC | \
        POLICY_OBJ_ALLOW_DEC | POLICY_OBJ_ALLOW_KDF | POLICY_OBJ_ALLOW_WRAP | POLICY_OBJ_ALLOW_WRITE |  \
        POLICY_OBJ_ALLOW_GEN | POLICY_OBJ_ALLOW_KA | POLICY_OBJ_ALLOW_READ | POLICY_OBJ_ALLOW_IMPORT_EXPORT)
#define DEFAULT_POLICY_BIN_COUNT_PCR (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE | POLICY_OBJ_ALLOW_READ)
#define DEFAULT_POLICY_USERID (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE)

static int sessionCount         = 0;
static bool cryptokiInitialized = false;
static bool mutex_initialised   = false;
// #if (__GNUC__  && !AX_EMBEDDED)
// #endif

typedef int (*pfnMbedTlsSign)(void *ctx,
    mbedtls_md_type_t md_alg,
    const unsigned char *hash,
    size_t hash_len,
    unsigned char *sig,
    size_t *sig_len,
    int (*f_rng)(void *, unsigned char *, size_t),
    void *p_rng);

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
    unsigned char *certificate_buf; //[2500];
} P11Key_t, *P11KeyPtr_t;

/**
 * @brief Session structure.
 */
typedef struct P11Session
{
    P11KeyPtr_t pxCurrentKey;
    CK_ULONG ulState;
    CK_BBOOL xOpened;
    CK_MECHANISM_TYPE xOperationInProgress;
    CK_BBOOL digestUpdateCalled;
    CK_OBJECT_HANDLE xOperationKeyHandle;
    CK_BBOOL xFindObjectInit;
    CK_BBOOL xFindObjectComplete;
    CK_OBJECT_CLASS xFindObjectClass;
    uint32_t xFindObjectTotalFound;
    CK_KEY_TYPE xFindObjectKeyType;
    mbedtls_ctr_drbg_context xMbedDrbgCtx;
    mbedtls_entropy_context xMbedEntropyContext;
    mbedtls_pk_context xPublicKey;
    mbedtls_sha256_context xSHA256Context;
    CK_BBOOL labelPresent;
    CK_BBOOL keyIdPresent;
    uint32_t keyId;
    char label[32];
    size_t labelLen;
    void *mechParameter;
    CK_ULONG mechParameterLen;
#if SSS_HAVE_ALT_SSS
    sss_digest_t digest_ctx;
#endif
} P11Session_t, *P11SessionPtr_t;

#if SSS_HAVE_SE05X_VER_GTE_06_00
/**
 * SE05x Attribute.
 */
typedef struct _se05x_object_attribute
{
    uint32_t obj_id;                /**< Object identifier. */
    uint8_t obj_type;               /**< Object type. */
    SE05x_SetIndicator_t auth_attr; /**< Authentication attribute. */
    uint16_t auth_attempts;         /**< Authentication attempts counter. Only for Authentication Object*/
    uint16_t min_AEAD_tag_len;      /**< Minimum tag length for AEAD operations. Only for non-Authentication Object*/
    uint32_t auth_obj_id;           /**< Authentication object identifier */
    uint16_t max_attempts;          /**< Maximum authentication attempts */
    SE05x_Origin_t origin;          /**< The origin of the Secure Object */
    uint32_t version;               /**< The Secure Object version. */
    uint32_t policy_num;            /**< Number of policy */
} se05x_object_attribute;
#endif

/*-----------------------------------------------------------*/
// static void prvFreeKey( P11KeyPtr_t pxKey );
// static int awsRetreieve_certificate(unsigned char *pbuf, uint8_t index, uint32_t *outbuflen);
// static int convertdertopemformat(const unsigned char * int_ca_cert_der, int len_der, unsigned char * int_ca_cert_pem, int dst_buf_len, U16 *outlength);
CK_RV ParseSignMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV ParseEncryptionMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV ParseDigestMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV AsymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV AsymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen);
CK_RV SymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV SymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV GetAttributeParameterIndex(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ATTRIBUTE_TYPE type, CK_ULONG_PTR index);
CK_RV LabelToKeyId(unsigned char *label, size_t labelSize, uint32_t *keyId);
CK_RV CreateRawPrivateKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key, size_t *keyLen);
CK_RV CreateRawPublicKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key, size_t *keyLen);
CK_RV EcSignatureToRandS(uint8_t *signature, size_t *sigLen);
CK_RV EcRandSToSignature(uint8_t *rands, const size_t rands_len, uint8_t *output, size_t *outputLen);
CK_RV EcPublickeyGetEcParams(uint8_t *input, size_t *dataLen);
CK_RV parseCertificateGetAttribute(
    uint32_t xObject, CK_ATTRIBUTE_TYPE attributeType, uint8_t *pData, CK_ULONG *ulAttrLength);
CK_BBOOL isX509Certificate(uint32_t xObject);

#if SSS_HAVE_ALT_A71CH
static U16 HLSE_Create_token(
    uint32_t keyId, HLSE_OBJECT_TYPE objType, void *buff, unsigned long bufflen, HLSE_OBJECT_HANDLE handle_object);
static mbedtls_ecp_group_id curve_list[16] = {
    0,
};
#endif

#if SSS_HAVE_APPLET_SE05X_IOT
static smStatus_t read_id_list(uint32_t *idlist, size_t *idlistlen);
static CK_RV read_object_size_with_lock(uint32_t keyId, uint16_t *keyLen);
static CK_RV read_object_size(uint32_t keyId, uint16_t *keyLen);
static uint8_t CheckIfKeyIdExists(uint32_t keyId, pSe05xSession_t session_ctx);
#endif

#if SSS_HAVE_ALT_SSS
static int convertpemtoder(const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen);
static sss_status_t sss_create_token(sss_key_store_t *keystore,
    sss_object_t *CreateObject,
    U32 ObjectId,
    sss_key_part_t KeyPart,
    sss_cipher_type_t CipherType,
    U8 *buffer,
    U32 bufferLen,
    U32 bitLen);

static mbedtls_ecp_group_id curve_list[16] = {
    0,
};
#endif

#define ID_ECPUBLICKEY                           \
    {                                            \
        0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01 \
    }

typedef struct _object_identifiers
{
    uint32_t identifier[16];
    size_t indentifier_len;
    int groupId;
    char *name;
} object_identifiers_t;
static object_identifiers_t object_identifiers[] = {
    {{1, 2, 840, 10045, 3, 1, 1}, 7, MBEDTLS_ECP_DP_SECP192R1, "MBEDTLS_ECP_DP_SECP192R1"},
    {{1, 3, 132, 0, 33}, 5, MBEDTLS_ECP_DP_SECP224R1, "MBEDTLS_ECP_DP_SECP224R1"},
    {{1, 2, 840, 10045, 3, 1, 7}, 7, MBEDTLS_ECP_DP_SECP256R1, "MBEDTLS_ECP_DP_SECP256R1"},
    {{1, 3, 132, 0, 34}, 5, MBEDTLS_ECP_DP_SECP384R1, "MBEDTLS_ECP_DP_SECP384R1"},
    {{1, 3, 132, 0, 35}, 5, MBEDTLS_ECP_DP_SECP521R1, "MBEDTLS_ECP_DP_SECP521R1"},

    {{1, 3, 36, 3, 3, 2, 8, 1, 1, 7}, 10, MBEDTLS_ECP_DP_BP256R1, "MBEDTLS_ECP_DP_BP256R1"},
    {{1, 3, 24, 3, 3, 2, 8, 1, 1, 7}, 10, MBEDTLS_ECP_DP_BP256R1, "MBEDTLS_ECP_DP_BP256R1"},
    {{1, 3, 36, 3, 3, 2, 8, 1, 1, 11}, 10, MBEDTLS_ECP_DP_BP384R1, "MBEDTLS_ECP_DP_BP384R1"},
    {{1, 3, 36, 3, 3, 2, 8, 1, 1, 13}, 10, MBEDTLS_ECP_DP_BP512R1, "MBEDTLS_ECP_DP_BP512R1"},

    {{1, 3, 132, 0, 31}, 5, MBEDTLS_ECP_DP_SECP192K1, "MBEDTLS_ECP_DP_SECP192K1"},
    {{1, 3, 132, 0, 32}, 5, MBEDTLS_ECP_DP_SECP224K1, "MBEDTLS_ECP_DP_SECP224K1"},
    {{1, 3, 132, 0, 10}, 5, MBEDTLS_ECP_DP_SECP256K1, "MBEDTLS_ECP_DP_SECP256K1"},
    {
        {
            0,
        },
        0,
        0,
    },
};

#if SSS_HAVE_ALT
// int mbedtls_ssl_set_curve_list(mbedtls_ssl_config *conf, const char * pcLabelName)
int mbedtls_ssl_set_curve_list(mbedtls_ssl_config *conf, uint32_t keyIndex)
{
    sss_status_t status    = kStatus_SSS_Fail;
    sss_object_t clientKey = {
        0,
    };
    uint32_t objectId[16] = {
        0,
    };
    uint8_t objectIdLen = sizeof(objectId);
    int i               = 0;
    size_t compareLen   = 0;
    // uint32_t keyIndex = 0;
    // LabelToKeyId((unsigned char*)pcLabelName, strlen(pcLabelName), &keyIndex);

    // printf(" \n\n%s\n\n", __FUNCTION__);
    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&clientKey, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(kStatus_SSS_Fail);
            // return kStatus_SSS_Fail;
        }

        status = sss_key_object_get_handle(&clientKey, keyIndex);
        if (status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(kStatus_SSS_Fail);
            // return kStatus_SSS_Fail;
        }

        status = sss_util_asn1_get_oid_from_sssObj(&clientKey, objectId, &objectIdLen);
        if (status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(1);
            // return 1;
        }

        UNLOCK_MUTEX_FOR_RTOS
    }
    while (1) {
        if (object_identifiers[i].indentifier_len == 0) {
            break;
        }

        if (object_identifiers[i].indentifier_len != objectIdLen) {
            i++;
            continue;
        }

        if (objectIdLen * sizeof(uint32_t) > 64) {
            compareLen = 64;
        }
        else {
            compareLen = objectIdLen * sizeof(uint32_t);
        }
        if (0 == memcmp(object_identifiers[i].identifier, objectId, compareLen)) {
            curve_list[0] = object_identifiers[i].groupId;
            curve_list[1] = MBEDTLS_ECP_DP_NONE;
            mbedtls_ssl_conf_curves(conf, curve_list);
            return 0;
        }

        i++;
    }

    return 1;
}

#endif //SSS_HAVE_ALT

/**
 * @brief Maps an opaque caller session handle into its internal state structure.
 */
static P11SessionPtr_t prvSessionPointerFromHandle(CK_SESSION_HANDLE xSession)
{
    return (P11SessionPtr_t)xSession; /*lint !e923 Allow casting integer type to pointer for handle. */
}

/*-----------------------------------------------------------*/

/**
 * @brief Initializes a key structure.
 */

/*-----------------------------------------------------------*/

/**
 * @brief Load the default key and certificate from storage.
 */

CK_RV GetAttributeParameterIndex(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ATTRIBUTE_TYPE type, CK_ULONG_PTR index)
{
    CK_RV xResult      = CKR_ARGUMENTS_BAD;
    CK_ULONG i         = 0;
    CK_BBOOL foundType = CK_FALSE;
    for (i = 0; i < ulCount; i++) {
        if (pTemplate[i].type == type) {
            foundType = CK_TRUE;
            xResult   = CKR_OK;
            break;
        }
    }
    if (foundType) {
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
    CK_RV xResult       = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t data[2048]  = {0};
    size_t dataLen      = sizeof(data);
    // sss_algorithm_t digest_algorithm = kAlgorithm_None;
    sss_asymmetric_t asymmCtx;
    sss_object_t sss_object = {0};

    if (pxSessionObj->xOperationInProgress == CKM_RSA_PKCS) {
        xResult = (2048 >= (ulDataLen + 11)) ? CKR_OK : CKR_MECHANISM_INVALID;
        if (xResult != CKR_OK) {
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

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_key_object_get_handle(&sss_object, pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_asymmetric_context_init(
            &asymmCtx, &pex_sss_demo_boot_ctx->session, &sss_object, algorithm, kMode_SSS_Encrypt);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_asymmetric_context_ sign init Failed...");
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            // return CKR_FUNCTION_FAILED;
        }

        /* Do Encryption */
        uint8_t encData[256] = {0};
        size_t encDataLen    = sizeof(encData);
        status               = sss_asymmetric_encrypt(&asymmCtx, &data[0], dataLen, &encData[0], &encDataLen);

        if (status != kStatus_SSS_Success) {
            LOG_E("sss_asymmetric_encrypt failed");
            xResult = CKR_FUNCTION_FAILED;
        }
        if (xResult == CKR_OK) {
            if (pEncryptedData) {
                if (*pulEncryptedDataLen < encDataLen) {
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

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}

CK_RV AsymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen)
{
    CK_RV xResult       = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t data[2048]  = {0};
    size_t dataLen      = sizeof(data);
    // sss_algorithm_t digest_algorithm = kAlgorithm_None;
    sss_asymmetric_t asymmCtx;
    sss_object_t sss_object = {0};

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

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_key_object_get_handle(&sss_object, pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_asymmetric_context_init(
            &asymmCtx, &pex_sss_demo_boot_ctx->session, &sss_object, algorithm, kMode_SSS_Decrypt);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_asymmetric_context_ sign init Failed...");
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            // return CKR_FUNCTION_FAILED;
        }

        /* Do Signing */
        uint8_t signature[256] = {0};
        size_t sigLen          = sizeof(signature);
        status                 = sss_asymmetric_decrypt(&asymmCtx, &data[0], dataLen, &signature[0], &sigLen);
        // status = sss_asymmetric_sign_digest(
        //     &asymmCtx, &data[0], dataLen, &signature[0], &sigLen);

        if (status != kStatus_SSS_Success) {
            LOG_E("sss_asymmetric_decrypt failed");
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            xResult                            = CKR_FUNCTION_FAILED;
        }
        if (xResult == CKR_OK) {
            if (pData) {
                if (*pulDataLen < sigLen) {
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

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}

CK_RV SymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV xResult       = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    // uint8_t data[2048] = { 0 };
    // size_t dataLen = sizeof(data);
    sss_symmetric_t symmCtx;
    sss_object_t symmObject;

    // memcpy(&data[0], pData, ulDataLen);
    // dataLen = ulDataLen;
    uint8_t iv[16]       = {0};
    size_t ivLen         = sizeof(iv);
    uint8_t encData[256] = {0};
    size_t encDataLen    = sizeof(encData);
    size_t tempOutBufLen = encDataLen;
    uint8_t *pOut        = &encData[0];
    if (algorithm == kAlgorithm_SSS_AES_CBC) {
        if (pxSessionObj->mechParameterLen != 0) {
            memcpy(iv, pxSessionObj->mechParameter, ivLen);
        }
    }
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

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&symmObject, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        status = sss_key_object_get_handle(&symmObject, pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        status = sss_symmetric_context_init(
            &symmCtx, &pex_sss_demo_boot_ctx->session, &symmObject, algorithm, kMode_SSS_Encrypt);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        /*Do Encryption*/
        status = sss_cipher_init(&symmCtx, iv, ivLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_cipher_update(&symmCtx, (const uint8_t *)pData, (size_t)ulDataLen, pOut, &tempOutBufLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        pOut          = pOut + tempOutBufLen;
        encDataLen    = tempOutBufLen;
        tempOutBufLen = sizeof(encData) - tempOutBufLen;

        status = sss_cipher_finish(&symmCtx, NULL, 0, pOut, &tempOutBufLen);

        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            LOG_E("sss_cipher_one_go failed");
            xResult = CKR_FUNCTION_FAILED;
        }
        encDataLen = encDataLen + tempOutBufLen;
        if (xResult == CKR_OK) {
            if (pEncryptedData) {
                if (*pulEncryptedDataLen < encDataLen) {
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

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}

CK_RV SymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDecryptedDataLen)
{
    CK_RV xResult       = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    // uint8_t data[2048] = { 0 };
    // size_t dataLen = sizeof(data);
    sss_symmetric_t symmCtx;
    sss_object_t symmObject;

    // status = sss_key_object_get_handle(&symmObject, CLIENT_SYMMETRIC_KEY_INDEX);
    // if(status != kStatus_SSS_Success) {
    //     pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
    //     return CKR_DEVICE_ERROR;
    // }

    // memcpy(&data[0], pEncryptedData, ulEncryptedDataLen);
    // dataLen = ulEncryptedDataLen;
    uint8_t iv[16]       = {0};
    size_t ivLen         = sizeof(iv);
    uint8_t encData[256] = {0};
    size_t encDataLen    = sizeof(encData);
    size_t tempOutBufLen = encDataLen;
    uint8_t *pOut        = &encData[0];
    if (algorithm == kAlgorithm_SSS_AES_CBC) {
        if (pxSessionObj->mechParameterLen != 0) {
            memcpy(iv, pxSessionObj->mechParameter, ivLen);
        }
    }
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

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&symmObject, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_key_object_get_handle(&symmObject, pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_symmetric_context_init(
            &symmCtx, &pex_sss_demo_boot_ctx->session, &symmObject, algorithm, kMode_SSS_Decrypt);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        /*Do Decryption*/
        status = sss_cipher_init(&symmCtx, iv, ivLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_cipher_update(
            &symmCtx, (const uint8_t *)pEncryptedData, (size_t)ulEncryptedDataLen, pOut, &tempOutBufLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        pOut          = pOut + tempOutBufLen;
        encDataLen    = tempOutBufLen;
        tempOutBufLen = sizeof(encData) - tempOutBufLen;

        status = sss_cipher_finish(&symmCtx, NULL, 0, pOut, &tempOutBufLen);

        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            LOG_E("sss_cipher_one_go failed");
            xResult = CKR_FUNCTION_FAILED;
        }
        encDataLen = encDataLen + tempOutBufLen;
        if (xResult == CKR_OK) {
            size_t i = 0;
            while ((encData[encDataLen - 1 - i] == 0) && i < encDataLen) {
                i++;
            }
            encDataLen = encDataLen - i;
            if (pData) {
                if (*pulDecryptedDataLen < encDataLen) {
                    xResult = CKR_BUFFER_TOO_SMALL;
                }
                else {
                    if (encDataLen > 0) {
                        memcpy(pData, &encData[0], encDataLen);
                    }
                    else {
                        LOG_E("memcpy failed");
                    }
                    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                }
            }
            *pulDecryptedDataLen = encDataLen;
        }

        sss_symmetric_context_free(&symmCtx);

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}

CK_RV GetSSSAlgorithm(const sss_algorithm_t algorithm, sss_algorithm_t *digest_algo)
{
    switch (algorithm) {
    case kAlgorithm_SSS_SHA1:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1:
    case kAlgorithm_SSS_ECDSA_SHA1:
        *digest_algo = kAlgorithm_SSS_SHA1;
        break;
    case kAlgorithm_SSS_SHA224:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA224:
    case kAlgorithm_SSS_ECDSA_SHA224:
        *digest_algo = kAlgorithm_SSS_SHA224;
        break;
    case kAlgorithm_SSS_SHA256:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA256:
    case kAlgorithm_SSS_ECDSA_SHA256:
        *digest_algo = kAlgorithm_SSS_SHA256;
        break;
    case kAlgorithm_SSS_SHA384:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA384:
    case kAlgorithm_SSS_ECDSA_SHA384:
        *digest_algo = kAlgorithm_SSS_SHA384;
        break;
    case kAlgorithm_SSS_SHA512:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA512:
    case kAlgorithm_SSS_ECDSA_SHA512:
        *digest_algo = kAlgorithm_SSS_SHA512;
        break;
    default:
        return CKR_ARGUMENTS_BAD;
    }
    return CKR_OK;
}

CK_RV LabelToKeyId(unsigned char *label, size_t labelSize, uint32_t *keyId)
{
    CK_RV result            = CKR_OK;
    sss_status_t status     = kStatus_SSS_Fail;
    sss_digest_t digest_ctx = {0};
    uint8_t digest[64]      = {0};
    size_t digest_size      = sizeof(digest);
    if (labelSize == 0) {
        LOCK_MUTEX_FOR_RTOS
        {
            sss_rng_context_t sss_rng_ctx;
            uint8_t rngData[10] = {0};
            size_t rngDataLen   = sizeof(rngData);
            status              = sss_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->session /* Session */);
            status              = sss_rng_get_random(&sss_rng_ctx, rngData, rngDataLen);
            if (status != kStatus_SSS_Success) {
                result = CKR_DEVICE_ERROR;
                UNLOCK_MUTEX_FOR_RTOS
                goto exit;
            }
            *keyId =
                (rngData[6] << (3 * 8)) | (rngData[7] << (2 * 8)) | (rngData[8] << (1 * 8)) | (rngData[9] << (0 * 8));

            UNLOCK_MUTEX_FOR_RTOS
        }
        goto exit;
    }
    else if (strncmp((char *)label, "sss:", strlen("sss:")) == 0) {
        char labelCopy[32] = {0};
        memset(labelCopy, '\0', sizeof(labelCopy));
        strncpy(labelCopy, (char *)label, labelSize);
        unsigned long long_id = 0;
        char *id              = (char *)&labelCopy[0];
        long_id               = strtoul(id + 4, NULL, 16);
        *keyId                = long_id;
        goto exit;
    }

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_digest_context_init(
            &digest_ctx, &pex_sss_demo_boot_ctx->session, kAlgorithm_SSS_SHA512, kMode_SSS_Digest);
        if (status != kStatus_SSS_Success) {
            result = CKR_DEVICE_ERROR;
            UNLOCK_MUTEX_FOR_RTOS
            goto exit;
        }

        status = sss_digest_one_go(&digest_ctx, label, labelSize, &digest[0], &digest_size);
        if (status != kStatus_SSS_Success) {
            result = CKR_DEVICE_ERROR;
        }

        sss_digest_context_free(&digest_ctx);

        if (status == kStatus_SSS_Success) {
            *keyId =
                (digest[60] << (3 * 8)) | (digest[61] << (2 * 8)) | (digest[62] << (1 * 8)) | (digest[63] << (0 * 8));
        }

        UNLOCK_MUTEX_FOR_RTOS
    }

exit:
    return result;
}

/*
 This is a simple function to insert a TLV into a buffer.
 params:
    tag             - ASN.1 Tag
    component       - byte array to be inserted
    componentLen    - Size of component to be inserted
    key             - Buffer into which component will be inserted
    keyLen          - Size of the buffer (key).

 Note : This function inserts the component at the end of the buffer and updates the
        keyLen to where the component is inserted with tag. (Points to the tag)
*/
CK_RV SetASNTLV(uint8_t tag, uint8_t *component, const size_t componentLen, uint8_t *key, size_t *keyLen)
{
    if (componentLen <= 0) {
        return CKR_ARGUMENTS_BAD;
    }

    if (*keyLen < componentLen) {
        return CKR_ARGUMENTS_BAD;
    }

    *keyLen = *keyLen - componentLen;
    memcpy(&key[*keyLen], component, componentLen);

    if (componentLen <= 127) {
        if (*keyLen < 1) {
            return CKR_ARGUMENTS_BAD;
        }
        *keyLen      = *keyLen - 1;
        key[*keyLen] = componentLen;
    }
    else if (componentLen <= 255) {
        if (*keyLen < 2) {
            return CKR_ARGUMENTS_BAD;
        }
        *keyLen          = *keyLen - 2;
        key[*keyLen]     = 0x81;
        key[*keyLen + 1] = componentLen;
    }
    else {
        if (*keyLen < 3) {
            return CKR_ARGUMENTS_BAD;
        }
        *keyLen          = *keyLen - 3;
        key[*keyLen]     = 0x82;
        key[*keyLen + 1] = (componentLen & 0x00FF00) >> 8;
        key[*keyLen + 2] = (componentLen & 0x00FF);
    }
    if (*keyLen < 1) {
        return CKR_ARGUMENTS_BAD;
    }
    *keyLen = *keyLen - 1;

    key[*keyLen] = tag;

    return CKR_OK;
}

CK_RV CreateRawPrivateKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key_buffer, size_t *keyLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    CK_ULONG keyTypeIndex;
    CK_KEY_TYPE key_type = CKK_RSA;

    xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_KEY_TYPE, &keyTypeIndex);
    if (xResult != CKR_OK) {
        goto exit;
    }

    memcpy(&key_type, pxTemplate[keyTypeIndex].pValue, pxTemplate[keyTypeIndex].ulValueLen);
    if (key_type == CKK_RSA) {
        CK_ULONG componentIndex;
        if (GetAttributeParameterIndex(pxTemplate, ulCount, CKA_MODULUS, &componentIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PUBLIC_EXPONENT, &componentIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PRIVATE_EXPONENT, &componentIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PRIME_1, &componentIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PRIME_2, &componentIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EXPONENT_1, &componentIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EXPONENT_2, &componentIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_COEFFICIENT, &componentIndex)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        /**
            N = CKA_MODULUS             |   Nlen = Length of CKA_MODULUS
            P = CKA_PRIME_1             |   Plen = Length of CKA_PRIME_1
            Q = CKA_PRIME_2             |   Qlen = Length of CKA_PRIME_2
            D = CKA_PRIVATE_EXPONENT    |   Dlen = Length of CKA_PRIVATE_EXPONENT
            E = CKA_PUBLIC_EXPONENT     |   Elen = Length of CKA_PUBLIC_EXPONENT
        */
        size_t bufferSize_copy = *keyLen;
        size_t componentLen    = 0;
        uint8_t tag            = ASN_TAG_INT;
        uint8_t key[4096]      = {0};

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_COEFFICIENT, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EXPONENT_2, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EXPONENT_1, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PRIME_2, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PRIME_1, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PRIVATE_EXPONENT, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PUBLIC_EXPONENT, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_MODULUS, &componentIndex);
        componentLen = (size_t)pxTemplate[componentIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[componentIndex].pValue, componentLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        uint8_t int_val = 0x00;
        xResult         = SetASNTLV(tag, &int_val, 1, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        size_t totalLen = bufferSize_copy - *keyLen;

        if (totalLen <= 127) {
            if (*keyLen < 1) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 1;

            key[*keyLen] = totalLen;
        }
        else if (totalLen <= 255) {
            if (*keyLen < 2) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 2;

            key[*keyLen]     = 0x81;
            key[*keyLen + 1] = totalLen;
        }
        else {
            if (*keyLen < 3) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 3;

            key[*keyLen]     = 0x82;
            key[*keyLen + 1] = (totalLen & 0x00FF00) >> 8;
            key[*keyLen + 2] = (totalLen & 0x00FF);
        }
        if (*keyLen < 1) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        *keyLen = *keyLen - 1;

        key[*keyLen] = ASN_TAG_SEQUENCE;

        totalLen = bufferSize_copy - *keyLen;
        memcpy(&key_buffer[0], &key[*keyLen], totalLen);
        *keyLen = totalLen;
    }
    else if (key_type == CKK_EC) {
        CK_ULONG parameterIndex;
        if (GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EC_PARAMS, &parameterIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &parameterIndex)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        uint8_t key[1024]      = {0};
        size_t bufferSize_copy = *keyLen;
        size_t parameterLen    = 0;
        uint8_t tag            = ASN_TAG_CRL_EXTENSIONS;

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EC_PARAMS, &parameterIndex);
        parameterLen = (size_t)pxTemplate[parameterIndex].ulValueLen;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[parameterIndex].pValue, parameterLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &parameterIndex);
        parameterLen = (size_t)pxTemplate[parameterIndex].ulValueLen;
        tag          = ASN_TAG_OCTETSTRING;
        xResult      = SetASNTLV(tag, (uint8_t *)pxTemplate[parameterIndex].pValue, parameterLen, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        tag             = ASN_TAG_INT;
        uint8_t int_val = 0x01;
        xResult         = SetASNTLV(tag, &int_val, 1, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        size_t totalLen = bufferSize_copy - *keyLen;

        if (totalLen <= 127) {
            if (*keyLen < 1) {
                return CKR_ARGUMENTS_BAD;
            }
            *keyLen = *keyLen - 1;

            key[*keyLen] = totalLen;
        }
        else if (totalLen <= 255) {
            if (*keyLen < 2) {
                return CKR_ARGUMENTS_BAD;
            }
            *keyLen          = *keyLen - 2;
            key[*keyLen]     = 0x81;
            key[*keyLen + 1] = totalLen;
        }
        else {
            if (*keyLen < 3) {
                return CKR_ARGUMENTS_BAD;
            }
            *keyLen = *keyLen - 3;

            key[*keyLen]     = 0x82;
            key[*keyLen + 1] = (totalLen & 0x00FF00) >> 8;
            key[*keyLen + 2] = (totalLen & 0x00FF);
        }

        if (*keyLen < 1) {
            return CKR_ARGUMENTS_BAD;
        }
        *keyLen = *keyLen - 1;

        key[*keyLen] = ASN_TAG_SEQUENCE;

        totalLen = bufferSize_copy - *keyLen;

        uint8_t temp[1024] = {0};
        size_t tLen        = sizeof(temp);
        int ret            = 0;
        mbedtls_pk_context pk;
        mbedtls_pk_init(&pk);
        ret = mbedtls_pk_parse_key(&pk, &key[*keyLen], totalLen, NULL, 0);
        if (ret != 0) {
            xResult = CKR_ARGUMENTS_BAD;
            mbedtls_pk_free(&pk);
            goto exit;
        }
        ret = mbedtls_pk_write_key_der(&pk, temp, tLen);
        if (ret < 0) {
            xResult = CKR_FUNCTION_FAILED;
            mbedtls_pk_free(&pk);
            goto exit;
        }

        mbedtls_pk_free(&pk);

        memcpy(&key_buffer[0], &temp[tLen - ret], ret);
        *keyLen = ret;
    }
    else {
        xResult = CKR_ARGUMENTS_BAD;
    }

exit:
    return xResult;
}

mbedtls_ecp_group_id EcParametersToGrpId(uint8_t *ecparameters, size_t len)
{
    mbedtls_ecp_group ecp_group;
    mbedtls_ecp_group_init(&ecp_group);
    mbedtls_asn1_buf params;
    unsigned char *ptr = &ecparameters[0];
    if ((pk_get_ecparams(&ptr, ptr + len, &params)) != 0 || (pk_use_ecparams(&params, &ecp_group)) != 0) {
        LOG_E("FAILURE");
        return MBEDTLS_ECP_DP_NONE;
    }
    LOG_I("mbedtls_mpi_read_binary successfull");
    return ecp_group.id;
}

CK_RV CreateRawPublicKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key_buffer, size_t *keyLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    CK_ULONG keyTypeIndex;
    CK_KEY_TYPE key_type = CKK_RSA;
    CK_ULONG parameterIndex;

    xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_KEY_TYPE, &keyTypeIndex);
    if (xResult != CKR_OK) {
        goto exit;
    }

    memcpy(&key_type, pxTemplate[keyTypeIndex].pValue, pxTemplate[keyTypeIndex].ulValueLen);
    if (key_type == CKK_RSA) {
        uint8_t publicExponent[] = {0x01, 0x00, 0x01};
        if (GetAttributeParameterIndex(pxTemplate, ulCount, CKA_PUBLIC_EXPONENT, &parameterIndex) == 0) {
            if ((pxTemplate[parameterIndex].ulValueLen == 1) ||
                (memcmp((void *)publicExponent, pxTemplate[parameterIndex].pValue, sizeof(publicExponent)))) {
                LOG_E("Public Exponent not supported");
                xResult = CKR_ARGUMENTS_BAD;
                goto exit;
            }
        }
        if (GetAttributeParameterIndex(pxTemplate, ulCount, CKA_MODULUS, &parameterIndex)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        uint8_t key[2048]      = {0};
        uint8_t modulus[520]   = {0};
        uint8_t *pInputModulus = (uint8_t *)pxTemplate[parameterIndex].pValue;
        size_t modulusLen      = (size_t)pxTemplate[parameterIndex].ulValueLen;
        size_t bufferSize_copy = *keyLen;
        uint8_t tag            = ASN_TAG_INT;
        if (pInputModulus[0] != 0x00) {
            memcpy(&modulus[1], pInputModulus, modulusLen);
            modulusLen++;
        }
        else {
            memcpy(modulus, pInputModulus, modulusLen);
        }
        xResult = SetASNTLV(tag, publicExponent, sizeof(publicExponent), key, keyLen);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        xResult = SetASNTLV(tag, modulus, modulusLen, key, keyLen);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);

        size_t totalLen = bufferSize_copy - *keyLen;

        if (totalLen <= 127) {
            if (*keyLen < 1) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 1;

            key[*keyLen] = totalLen;
        }
        else if (totalLen <= 255) {
            if (*keyLen < 2) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 2;

            key[*keyLen]     = 0x81;
            key[*keyLen + 1] = totalLen;
        }
        else {
            if (*keyLen < 3) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 3;

            key[*keyLen]     = 0x82;
            key[*keyLen + 1] = (totalLen & 0x00FF00) >> 8;
            key[*keyLen + 2] = (totalLen & 0x00FF);
        }

        if (*keyLen < 1) {
            return CKR_ARGUMENTS_BAD;
        }
        *keyLen = *keyLen - 1;

        key[*keyLen] = ASN_TAG_SEQUENCE;

        totalLen = bufferSize_copy - *keyLen;

        mbedtls_pk_context pk;
        mbedtls_pk_init(&pk);
        int ret = mbedtls_pk_parse_public_key(&pk, &key[*keyLen], totalLen);
        if (ret != 0) {
            LOG_E("mbedtls_pk_parse_public_key failed");
            xResult = CKR_FUNCTION_FAILED;
            goto exit;
        }
        ret = mbedtls_pk_write_pubkey_der(&pk, key, sizeof(key));
        if (ret < 0) {
            xResult = CKR_FUNCTION_FAILED;
        }
        else {
            memcpy(key_buffer, &key[sizeof(key) - ret], ret);
            *keyLen = ret;
        }
        mbedtls_pk_free(&pk);
    }
    else if (key_type == CKK_EC) {
        if (GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EC_PARAMS, &parameterIndex) ||
            GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EC_POINT, &parameterIndex)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        uint8_t key[2048]      = {0};
        size_t bufferSize_copy = *keyLen;
        size_t parameterLen    = 0;
        uint8_t tag            = ASN_TAG_BITSTRING;

        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EC_POINT, &parameterIndex);
        parameterLen = (size_t)pxTemplate[parameterIndex].ulValueLen;

        /**
            CKA_EC_POINT passed is DER encoded under Octet String tag. Decode the tag, length
            and parse the value.
        */

        uint8_t ecPointInput[150] = {0};
        size_t ecPointInput_size  = sizeof(ecPointInput);
        if (ecPointInput_size < (size_t)pxTemplate[parameterIndex].ulValueLen) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        memcpy(&ecPointInput[0],
            (uint8_t *)pxTemplate[parameterIndex].pValue,
            (size_t)pxTemplate[parameterIndex].ulValueLen);
        int i = 0;
        if (ecPointInput[i++] != ASN_TAG_OCTETSTRING) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        size_t len = ecPointInput[i++];

        if ((len & 0x80) == 0x80) {
            if ((len & 0x7F) == 0x01) {
                len = ecPointInput[i++];
            }
            else if ((len & 0x7F) == 0x02) {
                len = (ecPointInput[i] << 8) | (ecPointInput[i + 1]);
                i   = i + 2;
            }
            else {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
        }

        uint8_t ecPoint[150] = {0};
        // size_t ecPoint_size = sizeof(ecPoint);
        memcpy(&ecPoint[1], &ecPointInput[i], len);

        // xResult = SetASNTLV(tag, (uint8_t*) pxTemplate[parameterIndex].pValue, parameterLen, key, keyLen);
        xResult = SetASNTLV(tag, &ecPoint[0], len + 1, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        uint8_t ecPubParams[50] = {0};
        size_t ecPubParams_size = sizeof(ecPubParams);

        tag          = ASN_TAG_OBJ_IDF;
        xResult      = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_EC_PARAMS, &parameterIndex);
        parameterLen = (size_t)pxTemplate[parameterIndex].ulValueLen;

        if (ecPubParams_size < parameterLen) {
            mbedtls_ecp_group_id grp_id =
                EcParametersToGrpId((uint8_t *)pxTemplate[parameterIndex].pValue, parameterLen);
            /* TODO: Add conditions for other grp_id */
            if (grp_id == MBEDTLS_ECP_DP_SECP521R1) {
                uint8_t oid[] = {MBEDTLS_OID_EC_GRP_SECP521R1};
                size_t oidLen = sizeof(oid) - 1;
                // ecPubParams_size = ecPubParams_size - oidLen - 1;
                // memcpy(&ecPubParams[ecPubParams_size], oid, oidLen);
                xResult = SetASNTLV(tag, &oid[0], oidLen, ecPubParams, &ecPubParams_size);
                if (xResult != CKR_OK) {
                    goto exit;
                }
            }
            else {
                LOG_W("Parsing ECParameters supported only for SECP521R1");
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
        }
        else {
            ecPubParams_size = ecPubParams_size - parameterLen;
            memcpy(&ecPubParams[ecPubParams_size], (uint8_t *)pxTemplate[parameterIndex].pValue, parameterLen);
        }
        // // xResult = SetASNTLV(tag, (uint8_t*) pxTemplate[parameterIndex].pValue, parameterLen, ecPubParams, &ecPubParams_size);
        // if(xResult != CKR_OK) {
        //     goto exit;
        // }

        uint8_t id_ecPublicKey[] = ID_ECPUBLICKEY;
        xResult = SetASNTLV(tag, &id_ecPublicKey[0], sizeof(id_ecPublicKey), ecPubParams, &ecPubParams_size);
        if (xResult != CKR_OK) {
            goto exit;
        }

        tag     = ASN_TAG_SEQUENCE;
        xResult = SetASNTLV(tag, &ecPubParams[ecPubParams_size], sizeof(ecPubParams) - ecPubParams_size, key, keyLen);
        if (xResult != CKR_OK) {
            goto exit;
        }

        size_t totalLen = bufferSize_copy - *keyLen;

        if (totalLen <= 127) {
            if (*keyLen < 1) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 1;

            key[*keyLen] = totalLen;
        }
        else if (totalLen <= 255) {
            if (*keyLen < 2) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 2;

            key[*keyLen]     = 0x81;
            key[*keyLen + 1] = totalLen;
        }
        else {
            if (*keyLen < 3) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            *keyLen = *keyLen - 3;

            key[*keyLen]     = 0x82;
            key[*keyLen + 1] = (totalLen & 0x00FF00) >> 8;
            key[*keyLen + 2] = (totalLen & 0x00FF);
        }

        if (*keyLen < 1) {
            return CKR_ARGUMENTS_BAD;
        }
        *keyLen = *keyLen - 1;

        key[*keyLen] = ASN_TAG_SEQUENCE;

        totalLen = bufferSize_copy - *keyLen;
        memcpy(&key_buffer[0], &key[*keyLen], totalLen);
        *keyLen = totalLen;
    }
    else {
        xResult = CKR_ARGUMENTS_BAD;
    }

exit:
    return xResult;
}

CK_RV EcSignatureToRandS(uint8_t *signature, size_t *sigLen)
{
    CK_RV xResult      = CKR_FUNCTION_FAILED;
    uint8_t rands[128] = {0};
    int index          = 0;
    size_t i           = 0;
    size_t len         = 0;
    if (signature[index++] != 0x30)
        goto exit;
    if (signature[index++] != (*sigLen - 2))
        goto exit;
    if (signature[index++] != 0x02)
        goto exit;

    len = signature[index++];
    if (len & 0x01) {
        len--;
        index++;
    }

    for (i = 0; i < len; i++) {
        rands[i] = signature[index++];
    }

    if (signature[index++] != 0x02) {
        goto exit;
    }

    len = signature[index++];
    if (len & 0x01) {
        len--;
        index++;
    }

    len = len + i;
    for (; i < len; i++) {
        rands[i] = signature[index++];
    }

    memcpy(&signature[0], &rands[0], i);
    *sigLen = i;

    xResult = CKR_OK;

exit:
    return xResult;
}

CK_RV EcRandSToSignature(uint8_t *rands, const size_t rands_len, uint8_t *output, size_t *outputLen)
{
    CK_RV xResult          = CKR_FUNCTION_FAILED;
    uint8_t signature[256] = {0};
    size_t signatureLen    = sizeof(signature);
    size_t componentLen    = (rands_len) / 2;
    uint8_t tag            = ASN_TAG_INT;
    size_t totalLen;

    xResult = SetASNTLV(tag, &rands[componentLen], componentLen, signature, &signatureLen);
    if (xResult != CKR_OK) {
        goto exit;
    }

    xResult = SetASNTLV(tag, &rands[0], componentLen, signature, &signatureLen);
    if (xResult != CKR_OK) {
        goto exit;
    }

    totalLen = sizeof(signature) - signatureLen;

    if (totalLen <= 127) {
        if (signatureLen < 1) {
            xResult = CKR_FUNCTION_FAILED;
            goto exit;
        }
        signatureLen = signatureLen - 1;

        signature[signatureLen] = totalLen;
    }
    else if (totalLen <= 255) {
        if (signatureLen < 2) {
            xResult = CKR_FUNCTION_FAILED;
            goto exit;
        }
        signatureLen = signatureLen - 2;

        signature[signatureLen]     = 0x81;
        signature[signatureLen + 1] = totalLen;
    }
    else {
        if (signatureLen < 3) {
            xResult = CKR_FUNCTION_FAILED;
            goto exit;
        }
        signatureLen = signatureLen - 3;

        signature[signatureLen]     = 0x82;
        signature[signatureLen + 1] = (totalLen & 0x00FF00) >> 8;
        signature[signatureLen + 2] = (totalLen & 0x00FF);
    }
    if (signatureLen < 1) {
        return CKR_ARGUMENTS_BAD;
    }
    signatureLen = signatureLen - 1;

    signature[signatureLen] = ASN_TAG_SEQUENCE;

    totalLen = sizeof(signature) - signatureLen;
    memcpy(&output[0], &signature[signatureLen], totalLen);
    *outputLen = totalLen;

    xResult = CKR_OK;

exit:
    return xResult;
}

CK_RV EcPublickeyGetEcParams(uint8_t *input, size_t *dataLen)
{
    CK_RV xResult      = CKR_FUNCTION_FAILED;
    size_t index       = 0;
    uint8_t data[1024] = {0};
    int len            = 0;
    memcpy(&data[0], input, *dataLen);

    uint8_t tag = data[index++];
    if (tag != ASN_TAG_SEQUENCE) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    len = data[index++];

    if ((len & 0x80) == 0x80) {
        if ((len & 0x7F) == 0x01) {
            // len = data[index++];
        }
        else if ((len & 0x7F) == 0x02) {
            // len   = (data[index] << 8) | data[index + 1];
            index = index + 2;
        }
    }

    if (index > *dataLen) {
        goto exit;
    }

    tag = data[index++];
    if (tag != ASN_TAG_SEQUENCE) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    len = data[index++];

    if ((len & 0x80) == 0x80) {
        if ((len & 0x7F) == 0x01) {
            // len = data[index++];
        }
        else if ((len & 0x7F) == 0x02) {
            // len   = (data[index] << 8) | data[index + 1];
            index = index + 2;
        }
    }

    if (index > *dataLen) {
        goto exit;
    }

    tag = data[index++];
    if (tag != ASN_TAG_OBJ_IDF) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    len = data[index++];

    if ((len & 0x80) == 0x80) {
        if ((len & 0x7F) == 0x01) {
            len = data[index++];
        }
        else if ((len & 0x7F) == 0x02) {
            len   = (data[index] << 8) | data[index + 1];
            index = index + 2;
        }
    }

    index = index + len;

    if (index > *dataLen) {
        goto exit;
    }

    tag = data[index];
    if (tag != ASN_TAG_OBJ_IDF) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    len = data[index + 1];

    if ((len & 0x80) == 0x80) {
        if ((len & 0x7F) == 0x01) {
            len = data[index + 2];
            len++;
        }
        else if ((len & 0x7F) == 0x02) {
            len = (data[index + 2] << 8) | data[index + 3];
            len = len + 2;
        }
    }

    len = len + 2;

    if ((index + len) > *dataLen) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    memcpy(&input[0], &data[index], len);
    *dataLen = len;
    xResult  = CKR_OK;

exit:
    return xResult;
}

CK_BBOOL isX509Certificate(uint32_t xObject)
{
    CK_BBOOL xResult        = CK_FALSE;
    sss_object_t sss_object = {0};
    uint8_t data[2048]      = {0};
    size_t dataLen          = sizeof(data);
    size_t KeyBitLen        = 0;
    mbedtls_x509_crt certificate;

    /* NOTE: MUTEX LOCK IS NOT USED HERE BECAUSE THIS FUNCTION IS CALLED ONLY WHEN WE HAVE ALREADY LOCKED THE MUTEX */

    if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
        return xResult;
    }
    if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
        return xResult;
    }
    if (kStatus_SSS_Success !=
        sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
        return xResult;
    }

    mbedtls_x509_crt_init(&certificate);
    if (0 != mbedtls_x509_crt_parse(&certificate, (const unsigned char *)data, dataLen)) {
        return xResult;
    }

    xResult = CK_TRUE;
    return xResult;
}

CK_RV parseCertificateGetAttribute(
    uint32_t xObject, CK_ATTRIBUTE_TYPE attributeType, uint8_t *pData, CK_ULONG *ulAttrLength)
{
    CK_RV xResult                    = CKR_FUNCTION_FAILED;
    sss_status_t status              = kStatus_SSS_Fail;
    sss_object_t sss_object          = {0};
    sss_digest_t digestCtx           = {0};
    sss_algorithm_t digest_algorithm = kAlgorithm_SSS_SHA1;
    uint8_t data[2048]               = {0};
    size_t dataLen                   = sizeof(data);
    size_t KeyBitLen                 = 0;
    mbedtls_x509_crt certificate;
    mbedtls_pk_context *pk;
    uint8_t pubdata[2048] = {0};
    size_t pubdataLen     = sizeof(data);
    size_t i              = 0;
    int len;

    /* NOTE: MUTEX LOCK IS NOT USED HERE BECAUSE THIS FUNCTION IS CALLED ONLY WHEN WE HAVE ALREADY LOCKED THE MUTEX */

    if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
        return xResult;
    }
    if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
        return xResult;
    }
    if (kStatus_SSS_Success !=
        sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
        return xResult;
    }

    mbedtls_x509_crt_init(&certificate);
    if (0 != mbedtls_x509_crt_parse(&certificate, (const unsigned char *)data, dataLen)) {
        LOG_E("Unable to parse certificate");
        return xResult;
    }

    xResult = CKR_OK;
    switch (attributeType) {
    case CKA_HASH_OF_ISSUER_PUBLIC_KEY:
        if ((certificate.issuer_raw.tag != certificate.subject_raw.tag) ||
            (certificate.issuer_raw.len != certificate.subject_raw.len) ||
            (memcmp((void *)certificate.issuer_raw.p, (void *)certificate.subject_raw.p, certificate.subject_raw.len) !=
                0)) {
            xResult = CKR_ATTRIBUTE_SENSITIVE;
        }
        else {
            pk  = &certificate.pk;
            len = mbedtls_pk_write_pubkey_der(pk, pubdata, pubdataLen);
            if (len < 0) {
                xResult = CKR_FUNCTION_FAILED;
                break;
            }

            if ((int)(*ulAttrLength) < len) {
                LOG_E("Buffer too small");
                xResult = CKR_BUFFER_TOO_SMALL;
                break;
            }
            memcpy(pData, &pubdata[pubdataLen - len], len);
            *ulAttrLength = len;
        }
        break;
    case CKA_HASH_OF_SUBJECT_PUBLIC_KEY:
        pk  = &certificate.pk;
        len = mbedtls_pk_write_pubkey_der(pk, pubdata, pubdataLen);
        if (len < 0) {
            xResult = CKR_FUNCTION_FAILED;
            break;
        }

        if ((int)(*ulAttrLength) < len) {
            LOG_E("Buffer too small");
            xResult = CKR_BUFFER_TOO_SMALL;
            break;
        }
        memcpy(pData, &pubdata[pubdataLen - len], len);
        *ulAttrLength = len;

        break;

    case CKA_SUBJECT:
        if (certificate.subject_raw.p != NULL) {
            if ((int)(*ulAttrLength) < certificate.subject_raw.len) {
                LOG_E("Buffer too small");
                xResult = CKR_BUFFER_TOO_SMALL;
                break;
            }
            memcpy(pData, certificate.subject_raw.p, certificate.subject_raw.len);
            *ulAttrLength = certificate.subject_raw.len;
            break;
        }
        else {
            xResult = CKR_FUNCTION_FAILED;
            break;
        }

    default:
        LOG_W("Attribute required : 0x%08lx\n", attributeType);
        xResult = CKR_ATTRIBUTE_SENSITIVE;
    }

    ENSURE_OR_GO_EXIT(xResult == CKR_OK);

    if ((attributeType == CKA_HASH_OF_ISSUER_PUBLIC_KEY) || (attributeType == CKA_HASH_OF_SUBJECT_PUBLIC_KEY)) {
        xResult       = CKR_FUNCTION_FAILED;
        uint8_t *pTLV = &pData[0];
        ENSURE_OR_GO_EXIT(*pTLV == 0x30);

        /*
        *  Public key will be of the following format
        *  30 ZZ
        *      30 XX
        *          KEY_PARAMETERS
        *      03 YY
        *          PUBLIC_KEY
        */

        size_t tagLen = 0, bufindex = 0;
        int ret = asn_1_parse_tlv(pTLV, &tagLen, &bufindex); /* Parse initial sequence */
        ENSURE_OR_GO_EXIT(ret == 0);
        pTLV = pTLV + bufindex;
        ENSURE_OR_GO_EXIT(*pTLV == 0x30);
        bufindex = 0;
        ret      = asn_1_parse_tlv(pTLV, &tagLen, &bufindex); /* Parse key parameters */
        ENSURE_OR_GO_EXIT(ret == 0);
        /* Parse next tag */
        ASN1_SKIP_TO_NEXT_TAG(pTLV, tagLen)
        ENSURE_OR_GO_EXIT(*pTLV == 0x03);
        bufindex = 0;
        ret      = asn_1_parse_tlv(pTLV, &tagLen, &bufindex);
        ENSURE_OR_GO_EXIT(ret == 0);
        pTLV += bufindex;
        if (*pTLV == 0x00) {
            pTLV++;
            tagLen--;
        }

        xResult = CKR_OK;

        status =
            sss_digest_context_init(&digestCtx, &pex_sss_demo_boot_ctx->session, digest_algorithm, kMode_SSS_Digest);
        if (status != kStatus_SSS_Success) {
            return CKR_DEVICE_ERROR;
        }
        status = sss_digest_init(&digestCtx);
        if (status != kStatus_SSS_Success) {
            sss_digest_context_free(&digestCtx);
            return CKR_DEVICE_ERROR;
        }
        while (tagLen > 500) {
            status = sss_digest_update(&digestCtx, &pTLV[0 + i * 500], 500);
            if (status != kStatus_SSS_Success) {
                sss_digest_context_free(&digestCtx);
                return CKR_DEVICE_ERROR;
            }
            i++;
            tagLen -= 500;
        }
        status = sss_digest_update(&digestCtx, &pTLV[0 + i * 500], tagLen);
        if (status != kStatus_SSS_Success) {
            sss_digest_context_free(&digestCtx);
            return CKR_DEVICE_ERROR;
        }

        *ulAttrLength = 20 /* SHA-1 data length */;
        status        = sss_digest_finish(&digestCtx, &pData[0], (size_t *)ulAttrLength);
        if (status != kStatus_SSS_Success) {
            sss_digest_context_free(&digestCtx);
            return CKR_DEVICE_ERROR;
        }
        sss_digest_context_free(&digestCtx);
    }

exit:
    return xResult;
}

/*-----------------------------------------------------------*/

/*
 * PKCS#11 module implementation.
 */
/**
 * @brief PKCS#11 interface functions implemented by this Cryptoki module.
 */
CK_FUNCTION_LIST prvP11FunctionList = {{CRYPTOKI_VERSION_MAJOR, CRYPTOKI_VERSION_MINOR},
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
    C_WaitForSlotEvent};

CK_DEFINE_FUNCTION(CK_RV, C_DigestFinal)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    else if (pxSession->xOperationInProgress == pkcs11NO_OPERATION) {
        xResult                         = CKR_OPERATION_NOT_INITIALIZED;
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
    }
    else if (!pulDigestLen) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        xResult                         = CKR_ARGUMENTS_BAD;
    }

    if (xResult == CKR_OK) {
#if SSS_HAVE_ALT_SSS
        LOCK_MUTEX_FOR_RTOS
        {
            size_t outputLen = 0;
            switch (pxSession->xOperationInProgress) {
            case CKM_SHA_1:
                outputLen = 20;
                break;
            case CKM_SHA224:
                outputLen = 28;
                break;
            case CKM_SHA256:
                outputLen = 32;
                break;
            case CKM_SHA384:
                outputLen = 48;
                break;
            case CKM_SHA512:
                outputLen = 64;
                break;

            default:
                xResult = CKR_FUNCTION_FAILED;
                break;
            }
            if (xResult != CKR_OK) {
                pxSession->xOperationInProgress = pkcs11NO_OPERATION;
                sss_digest_context_free(&pxSession->digest_ctx);
                UNLOCK_MUTEX_FOR_RTOS_RET(xResult)
                // return xResult;
            }

            if (pDigest) {
                if (*pulDigestLen < outputLen) {
                    xResult = CKR_BUFFER_TOO_SMALL;
                }
                else {
                    uint8_t digest[64]      = {0};
                    size_t digestLen        = sizeof(digest);
                    sss_status_t sss_status = kStatus_SSS_Fail;
                    sss_status              = sss_digest_finish(&pxSession->digest_ctx, digest, &digestLen);
                    sss_digest_context_free(&pxSession->digest_ctx);
                    if (sss_status != kStatus_SSS_Success) {
                        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
                        xResult                         = CKR_FUNCTION_FAILED;
                    }
                    else {
                        memcpy(pDigest, digest, digestLen);
                        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
                    }
                }
            }
            *pulDigestLen = outputLen;
            UNLOCK_MUTEX_FOR_RTOS_RET(xResult)
            // return xResult;
        }

#else
        if (pDigest == NULL) {
            /* Supply the required buffer size. */
            *pulDigestLen = pkcs11SHA256_DIGEST_LENGTH;
        }
        else {
            if (*pulDigestLen < pkcs11SHA256_DIGEST_LENGTH) {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else {
                if (0 != mbedtls_sha256_finish_ret(&pxSession->xSHA256Context, pDigest)) {
                    xResult = CKR_FUNCTION_FAILED;
                }

                pxSession->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
#endif
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestUpdate)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    else if (pxSession->xOperationInProgress == pkcs11NO_OPERATION) {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
    }

    if (xResult == CKR_OK) {
        pxSession->digestUpdateCalled = CK_TRUE;
#if SSS_HAVE_ALT_SSS
        sss_status_t sss_status = kStatus_SSS_Fail;
        LOCK_MUTEX_FOR_RTOS
        {
            sss_status = sss_digest_update(&pxSession->digest_ctx, pPart, ulPartLen);
            if (sss_status != kStatus_SSS_Success) {
                sss_digest_context_free(&pxSession->digest_ctx);
                pxSession->xOperationInProgress = pkcs11NO_OPERATION;
                xResult                         = CKR_FUNCTION_FAILED;
            }
            UNLOCK_MUTEX_FOR_RTOS
        }

        // &pxSession->digest_ctx
#else
        if (0 != mbedtls_sha256_update_ret(&pxSession->xSHA256Context, pPart, ulPartLen)) {
            xResult                         = CKR_FUNCTION_FAILED;
            pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        }
#endif
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pMechanism)
{
    CK_RV xResult = CKR_OK;
    // return CKR_DEVICE_ERROR;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

    if (NULL == pMechanism) {
        xResult = CKR_ARGUMENTS_BAD;
        return xResult;
    }

    else if (pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

#if SSS_HAVE_ALT_SSS
    sss_status_t sss_status         = kStatus_SSS_Fail;
    sss_algorithm_t algorithm       = kAlgorithm_None;
    pxSession->xOperationInProgress = pMechanism->mechanism;
    xResult                         = ParseDigestMechanism(pxSession, &algorithm);
    if (xResult != CKR_OK) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }
    LOCK_MUTEX_FOR_RTOS
    {
        sss_status = sss_digest_context_init(
            &pxSession->digest_ctx, &pex_sss_demo_boot_ctx->session, algorithm, kMode_SSS_Digest);
        if (sss_status != kStatus_SSS_Success) {
            pxSession->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            // return CKR_FUNCTION_FAILED;
        }
        sss_status = sss_digest_init(&pxSession->digest_ctx);
        if (sss_status != kStatus_SSS_Success) {
            pxSession->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            // return CKR_FUNCTION_FAILED;
        }
        UNLOCK_MUTEX_FOR_RTOS
    }

#else
    if (pMechanism->mechanism != CKM_SHA256) {
        xResult = CKR_MECHANISM_INVALID;
    }

    /*
    * Initialize the requested hash type
    */
    if (xResult == CKR_OK) {
        mbedtls_sha256_init(&pxSession->xSHA256Context);

        if (0 != mbedtls_sha256_starts_ret(&pxSession->xSHA256Context, 0)) {
            xResult = CKR_FUNCTION_FAILED;
        }
        else {
            pxSession->xOperationInProgress = pMechanism->mechanism;
        }
    }
#endif

    if (xResult == CKR_OK) {
        pxSession->digestUpdateCalled = CK_FALSE;
    }

    return xResult;
}

/**
 * @brief Generate cryptographically random bytes.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GenerateRandom)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pucRandomData, CK_ULONG ulRandomLen)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    /*lint !e9072 It's OK to have different parameter name. */
    // return CKR_DEVICE_ERROR;
#if SSS_HAVE_ALT_SSS
    (void)(xSession);
#else
    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(xSession);
#endif

    if( ( NULL == pucRandomData )/* ||
        ( ulRandomLen == 0 )*/ )
    {
        xResult = CKR_ARGUMENTS_BAD;
        return xResult;
    }
    if (ulRandomLen == 0) {
        return CKR_OK;
    }
    else {
#if SSS_HAVE_ALT_SSS
        LOCK_MUTEX_FOR_RTOS
        {
            sss_status_t sss_status = kStatus_SSS_Fail;
            sss_rng_context_t sss_rng_ctx;
            sss_status = sss_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->session /* Session */);
            if (sss_status != kStatus_SSS_Success) {
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
                // return CKR_FUNCTION_FAILED;
            }
            sss_status = sss_rng_get_random(&sss_rng_ctx, pucRandomData, ulRandomLen);
            sss_rng_context_free(&sss_rng_ctx);
            if (sss_status != kStatus_SSS_Success) {
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
                // return CKR_FUNCTION_FAILED;
            }
            UNLOCK_MUTEX_FOR_RTOS
        }

#else
        if (0 != mbedtls_ctr_drbg_random(&pxSessionObj->xMbedDrbgCtx, pucRandomData, ulRandomLen)) {
            xResult = CKR_FUNCTION_FAILED;
        }
#endif
    }

    // UNLOCK_MUTEX_FOR_RTOS_RET(xResult)
    return xResult;
}

/**
 * @brief Verify the digital signature of the specified data using the public
 * key attached to this session.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Verify)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pucData, CK_ULONG ulDataLen, CK_BYTE_PTR pucSignature, CK_ULONG ulSignatureLen)
{
    /*lint !e9072 It's OK to have different parameter name. */
    /* pkcs11tb*/
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(xSession);
#if SSS_HAVE_ALT_A71CH
    (void)(pxSessionObj);
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
    sss_algorithm_t digest_algorithm;
#endif

    /*
         * Check parameters.
         */
    if ((NULL == pucData) || (NULL == pucSignature)) {
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

    err = HLSE_VerifySignature(&mechInfo, handles[0], (U8 *)pucData, ulDataLen, (U8 *)pucSignature, ulSignatureLen);

    if (err != HLSE_SW_OK) {
        xResult = CKR_SIGNATURE_INVALID;
    }
#endif

#if SSS_HAVE_ALT_SSS

    xResult = ParseSignMechanism(pxSessionObj, &algorithm);
    if (xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }
    if (pxSessionObj->xOperationInProgress == CKM_ECDSA) {
        switch (ulDataLen) {
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
            if (ulDataLen < 20) {
                /* ECDSA Always needs a hash value. Hash value should be deduced from
                     * input size. In case the input size is less than any supported hash
                     * value, we will pad the data with 0s and default to kAlgorithm_SSS_SHA256
                     */
                algorithm = kAlgorithm_SSS_ECDSA_SHA256;
                break;
            }
            else {
                return CKR_DATA_LEN_RANGE;
            }
        }
    }

    uint8_t data[2048] = {0};
    size_t dataLen     = sizeof(data);
    if (pxSessionObj->xOperationInProgress != CKM_RSA_PKCS_PSS && pxSessionObj->xOperationInProgress != CKM_ECDSA &&
        pxSessionObj->xOperationInProgress != CKM_RSA_PKCS) {
        sss_digest_t digestCtx = {0};
        size_t i               = 0;

        xResult = GetSSSAlgorithm(algorithm, &digest_algorithm);
        if (xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }

        LOCK_MUTEX_FOR_RTOS
        {
            status = sss_digest_context_init(
                &digestCtx, &pex_sss_demo_boot_ctx->session, digest_algorithm, kMode_SSS_Digest);
            if (status != kStatus_SSS_Success) {
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }
            status = sss_digest_init(&digestCtx);
            if (status != kStatus_SSS_Success) {
                sss_digest_context_free(&digestCtx);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }
            while (ulDataLen > 500) {
                status = sss_digest_update(&digestCtx, &pucData[0 + i * 500], 500);
                if (status != kStatus_SSS_Success) {
                    sss_digest_context_free(&digestCtx);
                    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                    UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
                }
                i++;
                ulDataLen -= 500;
            }
            status = sss_digest_update(&digestCtx, &pucData[0 + i * 500], ulDataLen);
            if (status != kStatus_SSS_Success) {
                sss_digest_context_free(&digestCtx);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }

            status = sss_digest_finish(&digestCtx, &data[0], &dataLen);
            if (status != kStatus_SSS_Success) {
                sss_digest_context_free(&digestCtx);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }
            sss_digest_context_free(&digestCtx);

            UNLOCK_MUTEX_FOR_RTOS
        }
    }
    else {
        memcpy(&data[0], pucData, ulDataLen);
        dataLen = ulDataLen;
        if (algorithm == kAlgorithm_SSS_ECDSA_SHA256 && ulDataLen < 20) {
            dataLen = 32;
        }
    }

    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_init Failed...");
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_get_handle Failed...");
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        status = sss_asymmetric_context_init(
            &asymmCtx, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Verify);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_asymmetric_context_init verify context Failed...");
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        uint8_t buff[2048] = {0};
        size_t buffLen     = sizeof(buff);
        if (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_P) {
            xResult = EcRandSToSignature((uint8_t *)pucSignature, (size_t)ulSignatureLen, &buff[0], &buffLen);
        }
        else {
            memcpy(&buff[0], pucSignature, ulSignatureLen);
            buffLen = ulSignatureLen;
        }

        if (xResult == CKR_OK) {
            status = sss_asymmetric_verify_digest(&asymmCtx, &data[0], dataLen, buff, buffLen);
            if (status != kStatus_SSS_Success) {
                LOG_E(" sss_asymmetric_verify_digest Failed... %08x", status);
                xResult = CKR_SIGNATURE_INVALID;
            }
        }

        sss_asymmetric_context_free(&asymmCtx);

        UNLOCK_MUTEX_FOR_RTOS
    }

#endif
    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_Sign)
(CK_SESSION_HANDLE xSession,
    CK_BYTE_PTR pucData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pucSignature,
    CK_ULONG_PTR pulSignatureLen)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(xSession);
    if (!pucData) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        xResult                            = CKR_ARGUMENTS_BAD;
        return xResult;
    }
    if (!pulSignatureLen) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        xResult                            = CKR_ARGUMENTS_BAD;
        return xResult;
    }

    sss_status_t status;
    sss_object_t object;
    sss_asymmetric_t asymmCtx;
    sss_algorithm_t algorithm = kAlgorithm_None;
    sss_algorithm_t digest_algorithm;

    xResult = ParseSignMechanism(pxSessionObj, &algorithm);
    // printf("\nParsed algorithm = %d\n", algorithm);
    if (xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }
    if (pxSessionObj->xOperationInProgress == CKM_ECDSA) {
        switch (ulDataLen) {
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
            if (ulDataLen < 20) {
                /* ECDSA Always needs a hash value. Hash value should be deduced from
                     * input size. In case the input size is less than any supported hash
                     * value, we will pad the data with 0s and default to kAlgorithm_SSS_SHA256
                     */
                algorithm = kAlgorithm_SSS_ECDSA_SHA256;
                break;
            }
            else {
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                return CKR_DATA_LEN_RANGE;
            }
        }
    }
    if (pxSessionObj->xOperationInProgress == CKM_RSA_PKCS) {
        uint16_t keySizeBytes = 0;
#if SSS_HAVE_APPLET_SE05X_IOT
        xResult = read_object_size_with_lock((uint32_t)pxSessionObj->xOperationKeyHandle, &keySizeBytes);
        if (xResult != CKR_OK) {
            return xResult;
        }
#endif
        xResult = (keySizeBytes >= (ulDataLen + 11)) ? CKR_OK : CKR_MECHANISM_INVALID;
        if (xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }
    }

    uint8_t data[2048] = {0};
    size_t dataLen     = sizeof(data);
    if (pxSessionObj->xOperationInProgress == CKM_RSA_PKCS_PSS || pxSessionObj->xOperationInProgress == CKM_ECDSA ||
        pxSessionObj->xOperationInProgress == CKM_RSA_PKCS) {
        if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1) ||
            /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1) ||*/ (algorithm == kAlgorithm_SSS_ECDSA_SHA1)) {
            //Check input size
            if (ulDataLen != 20) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* FIXME:PKCS11 Spec states that operation should be terminated in this case.
                         AWS PKCS11 testbench states that Operation should not be terminated.
                 */
                return CKR_DATA_LEN_RANGE;
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA224)) {
            //Check input size
            if (ulDataLen != 28) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* FIXME:PKCS11 Spec states that operation should be terminated in this case.
                         AWS PKCS11 testbench states that Operation should not be terminated.
                 */
                return CKR_DATA_LEN_RANGE;
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA256)) {
            //Check input size
            if (ulDataLen != 32) {
                if (algorithm == kAlgorithm_SSS_ECDSA_SHA256 && ulDataLen < 20) {
                    /* Nothing to do */
                }
                else {
                    // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                    /* FIXME:PKCS11 Spec states that operation should be terminated in this case.
                             AWS PKCS11 testbench states that Operation should not be terminated.
                     */
                    return CKR_DATA_LEN_RANGE;
                }
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA384)) {
            //Check input size
            if (ulDataLen != 48) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* FIXME:PKCS11 Spec states that operation should be terminated in this case.
                         AWS PKCS11 testbench states that Operation should not be terminated.
                 */
                return CKR_DATA_LEN_RANGE;
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA512)) {
            //Check input size
            if (ulDataLen != 64) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* FIXME:PKCS11 Spec states that operation should be terminated in this case.
                         AWS PKCS11 testbench states that Operation should not be terminated.
                 */
                return CKR_DATA_LEN_RANGE;
            }
        }
        memcpy(&data[0], pucData, ulDataLen);
        dataLen = ulDataLen;
        if (algorithm == kAlgorithm_SSS_ECDSA_SHA256 && ulDataLen < 20) {
            dataLen = 32;
        }
    }
    else {
        sss_digest_t digestCtx = {0};
        size_t i               = 0;

        xResult = GetSSSAlgorithm(algorithm, &digest_algorithm);
        if (xResult != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            return xResult;
        }

        LOCK_MUTEX_FOR_RTOS
        {
            status = sss_digest_context_init(
                &digestCtx, &pex_sss_demo_boot_ctx->session, digest_algorithm, kMode_SSS_Digest);
            if (status != kStatus_SSS_Success) {
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }
            status = sss_digest_init(&digestCtx);
            if (status != kStatus_SSS_Success) {
                sss_digest_context_free(&digestCtx);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }
            while (ulDataLen > 500) {
                status = sss_digest_update(&digestCtx, &pucData[0 + i * 500], 500);
                if (status != kStatus_SSS_Success) {
                    sss_digest_context_free(&digestCtx);
                    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                    UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
                }
                i++;
                ulDataLen -= 500;
            }
            status = sss_digest_update(&digestCtx, &pucData[0 + i * 500], ulDataLen);
            if (status != kStatus_SSS_Success) {
                sss_digest_context_free(&digestCtx);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }

            status = sss_digest_finish(&digestCtx, &data[0], &dataLen);
            if (status != kStatus_SSS_Success) {
                sss_digest_context_free(&digestCtx);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR);
            }
            sss_digest_context_free(&digestCtx);

            UNLOCK_MUTEX_FOR_RTOS
        }

        // dataLen = 0;
    }

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_init Failed...");
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_key_object_get_handle Failed...");
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        status =
            sss_asymmetric_context_init(&asymmCtx, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Sign);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_asymmetric_context_ sign init Failed...");
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        /* Do Signing */
        uint8_t signature[512] = {0};
        size_t sigLen          = sizeof(signature);

        status = sss_asymmetric_sign_digest(&asymmCtx, &data[0], dataLen, &signature[0], &sigLen);
        if (status != kStatus_SSS_Success) {
            LOG_E(" sss_asymmetric_sign_digest Failed...");
            xResult = CKR_FUNCTION_FAILED;
        }
        if (xResult == CKR_OK) {
            if (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_P) {
                xResult = EcSignatureToRandS(signature, &sigLen);
            }
            if (xResult == CKR_OK) {
                if (pucSignature) {
                    if (*pulSignatureLen < sigLen) {
                        xResult = CKR_BUFFER_TOO_SMALL;
                    }
                    else {
                        memcpy(pucSignature, &signature[0], sigLen);
                        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                    }
                }
                *pulSignatureLen = sigLen;
            }
        }

        sss_asymmetric_context_free(&asymmCtx);

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}

/**
 * @brief Free resources attached to an object handle.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DestroyObject)
(CK_SESSION_HANDLE xSession, CK_OBJECT_HANDLE xObject)
{ /*lint !e9072 It's OK to have different parameter name. */
    (void)(xSession);
    // printf("\n\n%s\n\n", __FUNCTION__);

    sss_status_t sss_status = kStatus_SSS_Fail;
    sss_object_t object     = {0};

    LOCK_MUTEX_FOR_RTOS
    {
        sss_status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            // return CKR_FUNCTION_FAILED;
        }
        sss_status = sss_key_object_get_handle(&object, (uint32_t)xObject);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            // return CKR_FUNCTION_FAILED;
        }
        sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &object);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            // return CKR_FUNCTION_FAILED;
        }

        UNLOCK_MUTEX_FOR_RTOS
    }

    return CKR_OK;
}

#if SSS_HAVE_ALT_A71CH

static U16 HLSE_Create_token(
    uint32_t keyId, HLSE_OBJECT_TYPE objType, void *buff, unsigned long bufflen, HLSE_OBJECT_HANDLE handle_object)
{
    HLSE_OBJECT_INDEX index = keyId;
    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    attr[0].type     = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value    = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type     = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value    = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type     = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value    = buff;
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
    sss_status_t status  = kStatus_SSS_Fail;
    uint8_t output[4096] = {0};
    size_t olen          = sizeof(output);

    int a = convertpemtoder((unsigned char *)buffer, (size_t)bufferLen, &output[0], &olen);
    if (a == 0) {
        //Data was in PEM format. Nothing to be done.
    }
    else {
        memcpy(&output[0], buffer, bufferLen);
        olen = bufferLen;
    }
    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(CreateObject, keystore);
        if (status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(status)
        }

        status = sss_key_object_allocate_handle(
            CreateObject, ObjectId, KeyPart, CipherType, olen, kKeyObject_Mode_Persistent);
        if (status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(status)
        }

        status = sss_key_store_set_key(keystore, CreateObject, output, olen, bitLen, NULL, 0);
        if (status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(status)
        }
        UNLOCK_MUTEX_FOR_RTOS
    }
    return status;
}

#if SSS_HAVE_APPLET_SE05X_IOT
static smStatus_t read_id_list(uint32_t *idlist, size_t *idlistlen)
{
    uint8_t pmore      = kSE05x_MoreIndicator_NA;
    uint8_t list[1024] = {0};
    size_t listlen     = sizeof(list);
    size_t i, k = 0;
    smStatus_t retStatus          = SM_NOT_OK;
    uint16_t outputOffset         = 0;
    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;
    pSe05xSession_t session_ctx   = &pSession->s_ctx;
    LOCK_MUTEX_FOR_RTOS
    {
        do {
            retStatus = Se05x_API_ReadIDList(session_ctx, outputOffset, 0xFF, &pmore, list, &listlen);
            if (retStatus != SM_OK) {
                UNLOCK_MUTEX_FOR_RTOS_RET(retStatus)
                // return retStatus;
            }
            outputOffset = (uint16_t)listlen;
            // for (i = 0; i < listlen; i += 4) {
            //     #if LOG_INFO_ENABLED
            //     uint32_t id = 0 | (list[i + 0] << (3 * 8)) | (list[i + 1] << (2 * 8)) | (list[i + 2] << (1 * 8)) |
            //         (list[i + 3] << (0 * 8));
            //         LOG_I("Found object Id - %08X", id);
            //     #endif
            // }
        } while (pmore == kSE05x_MoreIndicator_MORE);

        UNLOCK_MUTEX_FOR_RTOS
    }

    for (i = 0; ((i < listlen) && (k < *idlistlen)); i += 4) {
        idlist[k] = 0 | (list[i + 0] << (3 * 8)) | (list[i + 1] << (2 * 8)) | (list[i + 2] << (1 * 8)) |
                    (list[i + 3] << (0 * 8));
        k++;
    }
    *idlistlen = k;
    return SM_OK;
}

#endif
#endif

#if SSS_HAVE_APPLET_SE05X_IOT
static CK_RV read_object_size_with_lock(uint32_t keyId, uint16_t *keyLen)
{
    CK_RV xResult                      = CKR_FUNCTION_FAILED;
    smStatus_t sm_status               = SM_NOT_OK;
    sss_status_t sss_status            = kStatus_SSS_Fail;
    sss_object_t sss_object            = {0};
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;
    LOCK_MUTEX_FOR_RTOS
    {
        sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
        xResult    = CKR_KEY_HANDLE_INVALID;
        sss_status = sss_key_object_get_handle(&sss_object, keyId);
        UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
        xResult = CKR_FUNCTION_FAILED;

        sm_status = Se05x_API_ReadSize(&se05x_session->s_ctx, keyId, keyLen);
        UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sm_status == SM_OK);
        xResult = CKR_OK;

        UNLOCK_MUTEX_FOR_RTOS
    }

exit:
    return xResult;
}

static CK_RV read_object_size(uint32_t keyId, uint16_t *keyLen)
{
    CK_RV xResult                      = CKR_FUNCTION_FAILED;
    smStatus_t sm_status               = SM_NOT_OK;
    sss_status_t sss_status            = kStatus_SSS_Fail;
    sss_object_t sss_object            = {0};
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;

    sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
    ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
    xResult    = CKR_KEY_HANDLE_INVALID;
    sss_status = sss_key_object_get_handle(&sss_object, keyId);
    ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
    xResult = CKR_FUNCTION_FAILED;

    sm_status = Se05x_API_ReadSize(&se05x_session->s_ctx, keyId, keyLen);
    ENSURE_OR_GO_EXIT(sm_status == SM_OK);
    xResult = CKR_OK;

exit:
    return xResult;
}

static uint8_t CheckIfKeyIdExists(uint32_t keyId, pSe05xSession_t session_ctx)
{
    smStatus_t retStatus    = SM_NOT_OK;
    SE05x_Result_t IdExists = 0;
    uint8_t exists          = 0;

    LOCK_MUTEX_FOR_RTOS
    {
        retStatus = Se05x_API_CheckObjectExists(session_ctx, keyId, &IdExists);
        if (retStatus == SM_OK) {
            if (IdExists == kSE05x_Result_SUCCESS) {
                // LOG_D("Key Id 0x%X exists", keyId);
                exists = 1;
            }
        }
        else {
            LOG_E("Error in Se05x_API_CheckObjectExists");
        }

        UNLOCK_MUTEX_FOR_RTOS
    }

    return exists;
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
U16 axZeroSignExtend(U8 *pStore, U16 actualLength, U16 expectedLength)
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

// #if SSS_HAVE_ALT_A71CH

// /**
//  * Extract the public key - as a byte array in uncompress format - from an ECC key
//  * @param[in] pKey Reference to ECC key.
//  * @param[in,out] pPublicKeyData IN: Buffer to contain public key; OUT: Public key
//  * @param[out] pPublicKeyLen Length of public key \p pPublicKeyData retrieved
//  * @param[in] maxPublicKeyLen Size of buffer (\p pPublicKeyData) provided to contain public key
// */
// U16 HOSTCRYPTO_GetPublicKey(EC_KEY *pKey, U8 *pPublicKeyData, U16 *pPublicKeyLen, U16 maxPublicKeyLen)
// {
//     int res;
//     size_t keylen = 0;
//     mbedtls_ecp_keypair * pEcCtx = mbedtls_pk_ec( *pKey );

//     res = mbedtls_ecp_check_pubkey(  &(pEcCtx->grp), &(pEcCtx->Q) );
//     if ((res != 0) || (pPublicKeyData == NULL) || (pPublicKeyLen == NULL))
//     {
//         return ERR_CRYPTO_ENGINE_FAILED;
//     }

//     res = mbedtls_ecp_point_write_binary(&(pEcCtx->grp),&(pEcCtx->Q),MBEDTLS_ECP_PF_UNCOMPRESSED,&keylen,pPublicKeyData,maxPublicKeyLen);
//     *pPublicKeyLen = (U16)keylen;

//     if ((*pPublicKeyLen == 0) || (res != 0))
//     {
//         return ERR_CRYPTO_ENGINE_FAILED;
//     }

//     return SW_OK;
// }

// /**
//  * Extract the private key - as a byte array restored to nominal key size by sign extension - from an ECC key (in an mbed TLS specific format)
//  * @param[in] pKey Reference to ECC key.
//  * @param[in,out] pPrivateKeyData IN: Buffer to contain private key; OUT: Private key
//  * @param[out] pPrivateKeyLen Length of private key \p pPrivateKeyData retrieved
//  * @param[in] maxPrivateKeyLen Size of buffer (\p pPrivateKeyData) provided to contain private key
// */
// U16 HOSTCRYPTO_GetPrivateKey(EC_KEY *pKey, U8 *pPrivateKeyData, U16 *pPrivateKeyLen, U16 maxPrivateKeyLen)
// {

//     int significantBytes = 0;
//     int keyLen = 0;
//     U16 res = SW_OK;
//     U8 keyArray[256];
//     const mbedtls_ecp_curve_info *p_curve_info = NULL;
//     mbedtls_ecp_keypair * pEcCtx = mbedtls_pk_ec( *pKey );

//     /*TODO check sign extension part */
//     res = mbedtls_ecp_check_privkey( &(pEcCtx->grp), &(pEcCtx->d));
//     if ((res != 0) || (pPrivateKeyData == NULL) || (pPrivateKeyLen == NULL))
//     {
//         return ERR_CRYPTO_ENGINE_FAILED;
//     }

//     significantBytes = mbedtls_mpi_size( &(pEcCtx->d) );
//     p_curve_info = mbedtls_ecp_curve_info_from_grp_id( pEcCtx->grp.id );
//     keyLen = ((p_curve_info->bit_size + 7))/8;
//     if (keyLen > maxPrivateKeyLen)
//     {
//         return ERR_BUF_TOO_SMALL;
//     }
//     res = mbedtls_mpi_write_binary(&(pEcCtx->d),keyArray,keyLen);
//     if (res == 0)
//     {
//         // Extend byte array with leading 0x00 byte in case private key had
//         // been truncated because the MSB were not significant
//         if (significantBytes > 0)
//         {
//             res = axZeroSignExtend(keyArray, (U16)significantBytes, (U16)keyLen);
//             if (res == SW_OK)
//             {
//                 memcpy(pPrivateKeyData, keyArray, keyLen);
//                 *pPrivateKeyLen = (U16)keyLen;
//             }
//             else
//             {
//                 *pPrivateKeyLen = 0;

//             }
//         }
//         else
//         {
//             *pPrivateKeyLen = 0;
//             res = ERR_GENERAL_ERROR;
//         }
//     }
//     else
//     {
//         res = ERR_CRYPTO_ENGINE_FAILED;
//     }

//     return res;
// }

// #endif  // SSS_HAVE_ALT_A71CH

/**
 * @brief Provides import and storage of a single client certificate and
 * associated private key.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CreateObject)
(CK_SESSION_HANDLE xSession, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR pxObject)
{
    /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

#if SSS_HAVE_ALT_A71CH
    HLSE_OBJECT_HANDLE HLSE_Handle_Create_obj = 0x0;
    U16 err                                   = 0;
#endif
    int ret;
    U8 buff[4096];
    mbedtls_pk_context pk;
    CK_ULONG Valueindex = 0;
    uint32_t keyId      = 0xffffffff;
    CK_ULONG i          = 0;
    CK_ULONG classIndex = 0;
    size_t buff_len     = sizeof(buff);

#if SSS_HAVE_ALT_SSS
    CK_ULONG keyidindex;
    CK_ULONG labelIndex = 0;
    CK_BBOOL foundKeyId = CK_FALSE;
    sss_status_t status;
    sss_cipher_type_t cipherType = kSSS_CipherType_RSA;
    size_t keybitlen             = 0;
#endif

    /* Unreferenced variables.*/
    // ( void ) ( xSession );

    /*
     * Check parameters.
     */

    if ((pkcs11CREATEOBJECT_MINIMUM_ATTRIBUTE_COUNT > ulCount) || (NULL == pxTemplate) || (NULL == pxObject)) {
        return CKR_ARGUMENTS_BAD;
    }
    if (ulCount == -1) {
        return CKR_ARGUMENTS_BAD;
    }

    xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_CLASS, &classIndex);
    if (xResult != CKR_OK) {
        return xResult;
    }

    /*Find the key id as it's needed while provisiong keys and certificate*/
#if SSS_HAVE_ALT_SSS
    xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_SSS_ID, &keyidindex);
    if (xResult == CKR_OK) {
        foundKeyId = CK_TRUE;
    }
    xResult = CKR_OK;
#endif

    /*
     * Handle the object by class.
     */

    if (CKR_OK == xResult) {
        switch (*((uint32_t *)pxTemplate[classIndex].pValue)) {
        case CKO_CERTIFICATE:

            xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &i);
            if (xResult != CKR_OK) {
                return xResult;
            }

            memcpy(buff, pxTemplate[i].pValue, pxTemplate[i].ulValueLen);
            buff_len = (size_t)pxTemplate[i].ulValueLen;

#if SSS_HAVE_ALT_A71CH

            err = HLSE_Create_token(
                keyId, HLSE_CERTIFICATE, pxTemplate[i].pValue, pxTemplate[i].ulValueLen, HLSE_Handle_Create_obj);

            if (err != HLSE_SW_OK) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            pxObject = (CK_OBJECT_HANDLE_PTR)&HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_ALT_SSS

            if (0 != pxTemplate[i].ulValueLen) {
                if (!foundKeyId) {
                    xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_LABEL, &labelIndex);
                    if (xResult != CKR_OK) {
                        /* CKA_LABEL was not provided. Generate a random keyId */
                        xResult = LabelToKeyId((unsigned char *)"", 0, &keyId);
                        if (xResult != CKR_OK) {
                            return xResult;
                        }
                    }
                    else {
                        xResult =
                            LabelToKeyId(pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId);
                        if (xResult != CKR_OK) {
                            return xResult;
                        }
                    }
                }
                mbedtls_x509_crt certificate;
                mbedtls_x509_crt_init(&certificate);
                ret = mbedtls_x509_crt_parse(&certificate, (const unsigned char *)buff, buff_len);
                mbedtls_x509_crt_free(&certificate);
                if (ret != 0) {
                    LOG_E(
                        "Certificate parsing failed  !  mbedtls_x509_crt_parse "
                        "returned %d",
                        ret);
                    return CKR_ARGUMENTS_BAD;
                }

                status = sss_create_token(&pex_sss_demo_boot_ctx->ks,
                    &pex_sss_demo_tls_ctx->dev_cert,
                    keyId,
                    kSSS_KeyPart_Default,
                    kSSS_CipherType_Binary,
                    buff,
                    buff_len,
                    buff_len * 8);
                if (status != kStatus_SSS_Success) {
                    xResult = CKR_DEVICE_ERROR;
                    return xResult;
                }

                *pxObject = keyId;
            }
#endif
            break;

        case CKO_PRIVATE_KEY:

            /*Parses the private key in PEM format and converts it to DER format.
                    * This is required because as SE shall require a key pair for storing keys*/

            xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &Valueindex);
            if (xResult != CKR_OK) {
                uint8_t key[4096];
                size_t keyLen = sizeof(key);
                xResult       = CreateRawPrivateKey(pxTemplate, ulCount, &key[0], &keyLen);
                if (xResult != CKR_OK) {
                    return xResult;
                }
                mbedtls_pk_init(&pk);
                ret = mbedtls_pk_parse_key(&pk, key, keyLen, NULL, 0);
                if (ret != 0) {
                    xResult = CKR_ARGUMENTS_BAD;
                    return xResult;
                }

                memset(buff, 0, sizeof(buff));
                memcpy(buff, key, keyLen);
                buff_len = (size_t)keyLen;
            }
            else {
                mbedtls_pk_init(&pk);

                if (0 != pxTemplate[Valueindex].ulValueLen) {
                    ret = mbedtls_pk_parse_key(
                        &pk, pxTemplate[Valueindex].pValue, pxTemplate[Valueindex].ulValueLen, NULL, 0);
                    if (ret != 0) {
                        uint8_t key[1024] = {0};
                        size_t keyLen     = sizeof(key);
                        xResult           = CreateRawPrivateKey(pxTemplate, ulCount, &key[0], &keyLen);
                        if (xResult != CKR_OK) {
                            break;
                        }
                        ret = mbedtls_pk_parse_key(&pk, key, keyLen, NULL, 0);
                        if (ret != 0) {
                            xResult = CKR_ARGUMENTS_BAD;
                            break;
                        }

                        memset(buff, 0, sizeof(buff));
                        memcpy(buff, key, keyLen);
                        buff_len = (size_t)keyLen;
                    }
                    else {
                        memset(buff, 0, sizeof(buff));
                        memcpy(buff, pxTemplate[Valueindex].pValue, pxTemplate[Valueindex].ulValueLen);
                        buff_len = (size_t)pxTemplate[Valueindex].ulValueLen;
                    }
                }
            }

#if SSS_HAVE_ALT_A71CH

            err = HLSE_Create_token(keyId, HLSE_KEY_PAIR, buff, buff_len, HLSE_Handle_Create_obj);
            if (err != HLSE_SW_OK) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            pxObject = (CK_OBJECT_HANDLE_PTR)&HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_ALT_SSS

            if (!foundKeyId) {
                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_LABEL, &labelIndex);
                if (xResult != CKR_OK) {
                    /* CKA_LABEL was not provided. Generate a random keyId */
                    xResult = LabelToKeyId((unsigned char *)"", 0, &keyId);
                    if (xResult != CKR_OK) {
                        return xResult;
                    }
                }
                else {
                    xResult = LabelToKeyId(pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId);
                    if (xResult != CKR_OK) {
                        return xResult;
                    }
                }
            }

            CK_KEY_TYPE key_type = CKK_RSA;
            CK_ULONG index       = 0;
            xResult              = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_KEY_TYPE, &index);
            if (xResult != CKR_OK) {
                return xResult;
            }
            memcpy(&key_type, pxTemplate[index].pValue, pxTemplate[index].ulValueLen);
            if (key_type == CKK_RSA) {
                cipherType                  = kSSS_CipherType_RSA;
                mbedtls_rsa_context *pk_rsa = mbedtls_pk_rsa(pk);
                keybitlen                   = pk_rsa->len * 8;
            }
            else if (key_type == CKK_EC) { /*CKK_EC also means CKK_ECDSA both enum values are same*/
                                           //Currently only for NIST-P curves
#if CKK_EC != CKK_ECDSA
#error "Assumed to be equal"
#endif
                cipherType                 = kSSS_CipherType_EC_NIST_P;
                mbedtls_ecp_keypair *pk_ec = mbedtls_pk_ec(pk);
                switch (pk_ec->grp.id) {
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
                &pex_sss_demo_tls_ctx->obj,
                keyId,
                kSSS_KeyPart_Pair,
                cipherType,
                buff,
                buff_len,
                keybitlen);

            if (status != kStatus_SSS_Success) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            *pxObject = keyId;

#endif
            break;

        case CKO_PUBLIC_KEY:

            /*Parses the public key in PEM format and converts it to DER format.
                * This is required because as HLSE understands DER only*/

            xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &Valueindex);
            if (xResult != CKR_OK) {
                uint8_t key[2048];
                size_t keyLen = sizeof(key);
                xResult       = CreateRawPublicKey(pxTemplate, ulCount, &key[0], &keyLen);
                if (xResult != CKR_OK) {
                    return xResult;
                }
                mbedtls_pk_init(&pk);
                ret = mbedtls_pk_parse_public_key(&pk, &key[0], keyLen);
                if (ret != 0) {
                    xResult = CKR_ARGUMENTS_BAD;
                    break;
                }
                memset(buff, 0, sizeof(buff));
                memcpy(buff, &key[0], keyLen);
                buff_len = keyLen;
            }
            else {
                mbedtls_pk_init(&pk);
                ret =
                    mbedtls_pk_parse_public_key(&pk, pxTemplate[Valueindex].pValue, pxTemplate[Valueindex].ulValueLen);
                if (ret != 0) {
                    xResult = CKR_ARGUMENTS_BAD;
                    break;
                }
                memset(buff, 0, sizeof(buff));
                memcpy(buff, pxTemplate[Valueindex].pValue, pxTemplate[Valueindex].ulValueLen);
                buff_len = (size_t)pxTemplate[Valueindex].ulValueLen;
            }

#if SSS_HAVE_ALT_A71CH

            err = HLSE_Create_token(keyId, HLSE_PUBLIC_KEY, buff, buff_len, HLSE_Handle_Create_obj);
            if (err != HLSE_SW_OK) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            pxObject = (CK_OBJECT_HANDLE_PTR)&HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_ALT_SSS
            if (!foundKeyId) {
                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_LABEL, &labelIndex);
                if (xResult != CKR_OK) {
                    /* CKA_LABEL was not provided. Generate a random keyId */
                    xResult = LabelToKeyId((unsigned char *)"", 0, &keyId);
                    if (xResult != CKR_OK) {
                        return xResult;
                    }
                }
                else {
                    xResult = LabelToKeyId(pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId);
                    if (xResult != CKR_OK) {
                        return xResult;
                    }
                }
            }

            mbedtls_pk_type_t pk_type = MBEDTLS_PK_NONE;
            pk_type                   = pk.pk_info->type;
            switch (pk_type) {
            case MBEDTLS_PK_RSA:
                cipherType                  = kSSS_CipherType_RSA;
                mbedtls_rsa_context *pk_rsa = mbedtls_pk_rsa(pk);
                keybitlen                   = pk_rsa->len * 8;
                break;
            case MBEDTLS_PK_ECKEY:
            case MBEDTLS_PK_ECDSA:
                cipherType                 = kSSS_CipherType_EC_NIST_P;
                mbedtls_ecp_keypair *pk_ec = mbedtls_pk_ec(pk);
                switch (pk_ec->grp.id) {
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
                break;
            default:
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

            if (status != kStatus_SSS_Success) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            *pxObject = keyId;
#endif
            break;

        case CKO_SECRET_KEY:

#if SSS_HAVE_ALT_A71CH
            xResult = CKR_ARGUMENTS_BAD;
#endif
#if SSS_HAVE_ALT_SSS

            if (!foundKeyId) {
                xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_LABEL, &labelIndex);
                if (xResult != CKR_OK) {
                    /* CKA_LABEL was not provided. Generate a random keyId */
                    xResult = LabelToKeyId((unsigned char *)"", 0, &keyId);
                    if (xResult != CKR_OK) {
                        return xResult;
                    }
                }
                else {
                    xResult = LabelToKeyId(pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId);
                    if (xResult != CKR_OK) {
                        return xResult;
                    }
                }
            }

            xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &i);

            if (xResult != CKR_OK) {
                return xResult;
            }

            if (0 != pxTemplate[i].ulValueLen) {
                sss_object_t secretObject = {0};
                status                    = sss_create_token(&pex_sss_demo_boot_ctx->ks,
                    &secretObject,
                    keyId,
                    kSSS_KeyPart_Default,
                    kSSS_CipherType_AES,
                    (uint8_t *)pxTemplate[i].pValue,
                    pxTemplate[i].ulValueLen,
                    pxTemplate[i].ulValueLen * 8);

                if (status != kStatus_SSS_Success) {
                    LOG_E("Secret Object init failed");
                    break;
                }
                *pxObject = keyId;
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
CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionList)
(CK_FUNCTION_LIST_PTR_PTR ppxFunctionList)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    if (NULL == ppxFunctionList) {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else {
        *ppxFunctionList = &prvP11FunctionList;
    }

    return xResult;
}

/**
 * @brief Initialize the Cryptoki module for use.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Initialize)(CK_VOID_PTR pvInitArgs)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV status = CKR_OK;
    if (cryptokiInitialized) {
        return CKR_CRYPTOKI_ALREADY_INITIALIZED;
    }
    // printf("\n\n%s\n\n", __FUNCTION__);

    // typedef struct CK_C_INITIALIZE_ARGS {
    //   CK_CREATEMUTEX CreateMutex;
    //   CK_DESTROYMUTEX DestroyMutex;
    //   CK_LOCKMUTEX LockMutex;
    //   CK_UNLOCKMUTEX UnlockMutex;
    //   CK_FLAGS flags;
    //   CK_VOID_PTR pReserved;
    // } CK_C_INITIALIZE_ARGS;
    if (pvInitArgs) {
        CK_C_INITIALIZE_ARGS_PTR initArgs = (CK_C_INITIALIZE_ARGS_PTR)(pvInitArgs);
        if (initArgs->flags & CKF_LIBRARY_CANT_CREATE_OS_THREADS) {
            //     //Library should not create OS threads. If this is not possible, return CKR_NEED_TO_CREATE_THREADS
#if (__GNUC__ && !AX_EMBEDDED)
            return CKR_NEED_TO_CREATE_THREADS;
#endif
#if defined(USE_RTOS) && USE_RTOS == 1
            return CKR_NEED_TO_CREATE_THREADS;
#endif
        }
        if (initArgs->flags & CKF_OS_LOCKING_OK) {
            // Application will call from multiple threads. Library should use locks.
            if ((initArgs->CreateMutex) && (initArgs->DestroyMutex) && (initArgs->LockMutex) &&
                (initArgs->UnlockMutex)) {
                //If mutex pointers are not null, library can use either OS locking or provided functions
                // return CKR_GENERAL_ERROR;
            }
            else if (!(initArgs->CreateMutex) && !(initArgs->DestroyMutex) && !(initArgs->LockMutex) &&
                     !(initArgs->UnlockMutex)) {
                //If mutex pointers are null, library must use OS locking.
                // return CKR_CANT_LOCK;
            }
        }
        else {
            return CKR_CANT_LOCK;
            //     //If mutex pointers are null, application would not access the library simultaneously.
            //     //If mutex pointers are not null, library should use provided functions to create threads.
            //      CKR_CANT_LOCK
            //     // status = CKR_ARGUMENTS_BAD;
            //     // return status;
        }
    }

    if (!mutex_initialised) {
#if defined(USE_RTOS) && USE_RTOS == 1
        // memset(&xSemaphore, 0, sizeof(SemaphoreHandle_t));
        xSemaphore = xSemaphoreCreateMutex();
        if (xSemaphore == NULL) {
            status = CKR_CANT_LOCK;
            goto exit;
        }
#endif

#if (__GNUC__ && !AX_EMBEDDED)
        int ret = EBUSY;
        while (ret == EBUSY) {
            ret = pthread_mutex_init(&gSessionlock, NULL);
        }
        if (ret != 0) {
            LOG_E("Could not initialize mutex");
            status = CKR_CANT_LOCK;
            goto exit;
        }
        ret = EBUSY;
        while (ret == EBUSY) {
            ret = pthread_mutex_init(&gFilelock, NULL);
        }
        // ret = pthread_mutex_init(&gSessionlock, NULL);
#endif

        mutex_initialised = true;
    }

    cryptokiInitialized = true;
#if ((defined(USE_RTOS) && USE_RTOS == 1) || ((__GNUC__ && !AX_EMBEDDED)))
exit:
#endif

    return status;
}

/**
 * @brief Un-initialize the Cryptoki module.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Finalize)(CK_VOID_PTR pvReserved)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    if (NULL != pvReserved) {
        return CKR_ARGUMENTS_BAD;
    }

    if (!cryptokiInitialized) {
        return CKR_CRYPTOKI_NOT_INITIALIZED;
    }

    if (mutex_initialised) {
#if defined(USE_RTOS) && USE_RTOS == 1
        vSemaphoreDelete(xSemaphore);
#endif
        mutex_initialised = false;
    }

    cryptokiInitialized = false;

    return xResult;
}

#if SSS_HAVE_SE05X_VER_GTE_06_00
/**
 * @brief Parse object attribtes from Se05x_API_ReadObjectAttributes. Check specific policy is allowed
 *
 * @param[out] pAttribute                Return attribute value.
 * @param[in] rsp                        Response from Se05x_API_ReadObjectAttributes.
 * @param[in] rspLen                     Response length.
 * @param[in] objectType                 Object type.
 * @param[in] cipherType                 Cipher type.
 * @param[in] policy_map                 Policy to be checked.
 * @param[out] pAllow                    Return if policy is allowed. If NULL, policy_map will not be checked.
 *
 *
 * @return kStatus_SSS_Success if successful.
 */
static sss_status_t parseAtrribute(se05x_object_attribute *pAttribute,
    uint8_t *rsp,
    size_t rspLen,
    uint32_t objectType,
    uint32_t cipherType,
    uint32_t policy_map,
    CK_BBOOL *pAllow)
{
    uint32_t i;
    uint32_t policyEnd;
    uint32_t policyStart = 14;
    uint8_t policy_len;   /**< Length of policy. */
    uint32_t auth_obj_id; /**< Authentication object identifier */
    uint32_t ar_header;   /**< AR Header */
    CK_BBOOL found_policy = CK_FALSE;
    uint32_t default_policy;

    if (rsp == NULL) {
        // Object attribute should be at least 19 bytes (without policy).
        LOG_E("Incomplete Object Attribute");
        return kStatus_SSS_Fail;
    }

    if (rspLen < 19) {
        // Object attribute should be at least 19 bytes (without policy).
        LOG_E("Incomplete Object Attribute");
        return kStatus_SSS_Fail;
    }
    policyEnd = (uint32_t)(rspLen - 5);

    // Parse Attribute
    pAttribute->obj_id    = ((rsp[0] << 8 * 3) | (rsp[1] << 8 * 2) | (rsp[2] << 8 * 1) | (rsp[3]));
    pAttribute->obj_type  = rsp[4];
    pAttribute->auth_attr = rsp[5];
    if (pAttribute->auth_attr == kSE05x_SetIndicator_SET) {
        pAttribute->auth_attempts = ((rsp[6] << 8 * 1) | (rsp[7]));
        pAttribute->max_attempts  = ((rsp[12] << 8 * 1) | (rsp[13]));
    }
    else {
        pAttribute->min_AEAD_tag_len = ((rsp[6] << 8 * 1) | (rsp[7]));
    }
    pAttribute->auth_obj_id = ((rsp[8] << 8 * 3) | (rsp[9] << 8 * 2) | (rsp[10] << 8 * 1) | (rsp[11]));
    pAttribute->origin      = rsp[policyEnd];
    pAttribute->version     = (rsp[policyEnd + 1] << 8 * 3) | (rsp[policyEnd + 2] << 8 * 2) |
                          (rsp[policyEnd + 3] << 8 * 1) | (rsp[policyEnd + 4]);
#ifdef DEBUG_PKCS11_PAL
    LOG_I("#####################################################");

    LOG_I("Object identifier : 0x%08X", pAttribute->obj_id);
    LOG_I("Object type : 0x%08X", pAttribute->obj_type);
    LOG_I("Authentication attribute : 0x%08X", pAttribute->auth_attr);
    if (pAttribute->auth_attr == kSE05x_SetIndicator_SET) {
        LOG_I("No. of failed attempts for Auth : %d", pAttribute->auth_attempts);
        if (pAttribute->max_attempts == 0) {
            LOG_I("Max auth attempts : Unlimited");
        }
        else {
            LOG_I("Max auth attempts : %d", pAttribute->max_attempts);
        }
    }
    else {
        LOG_I("Minimum tag length for AEAD operations : %d", pAttribute->min_AEAD_tag_len);
    }
    LOG_I("Auth object Id : 0x%08X", pAttribute->auth_obj_id);
    LOG_I("Origin : 0x%02X", pAttribute->origin);
    LOG_I("Secure object version : 0x%08X", pAttribute->version);
    LOG_I("#####################################################");
#endif

    if (pAllow != NULL) {
        // Check policy set and decide if not allowed.
        for (i = policyStart; i < policyEnd;) {
            policy_len  = rsp[i];
            auth_obj_id = ((rsp[i + 1] << 8 * 3) | (rsp[i + 2] << 8 * 2) | (rsp[i + 3] << 8 * 1) | (rsp[i + 4]));
            ar_header   = ((rsp[i + 5] << 8 * 3) | (rsp[i + 6] << 8 * 2) | (rsp[i + 7] << 8 * 1) | (rsp[i + 8]));
#ifdef DEBUG_PKCS11_PAL
            LOG_I("Policy auth object Id : 0x%08X", auth_obj_id);
            LOG_I("Policy Access rules : 0x%08X", ar_header);
            LOG_I("Required policy : 0x%02X", policy_map);
#endif
#ifdef SSS_EX_SE05x_AUTH_ID
            // Specific user policy. It overrides the policy for all other users.
            if (auth_obj_id == SSS_EX_SE05x_AUTH_ID) {
                if ((ar_header & policy_map) == 0) {
                    *pAllow = CK_FALSE;
                    return kStatus_SSS_Success;
                }
                else {
                    *pAllow = CK_TRUE;
                    return kStatus_SSS_Success;
                }
            }
#endif
            // Policy for all user. Continue to check if there are specific user policy.
            if (auth_obj_id == 0) {
                if ((ar_header & policy_map) == 0) {
                    *pAllow = CK_FALSE;
                }
                else {
                    *pAllow = CK_TRUE;
                }
                // Found policy for all user.
                found_policy = CK_TRUE;
            }

            i += policy_len + 1;
        }

        if (found_policy == CK_TRUE) {
            // Found policy. Not need to check default policy.
            return kStatus_SSS_Success;
        }

        if (pAttribute->auth_attr == kSE05x_SetIndicator_SET) {
            // Authentication Object
            default_policy = POLICY_OBJ_ALLOW_READ;
        }
        else {
            // Generic SecureObject
            if ((objectType == kSSS_KeyPart_Pair) || (objectType == kSSS_KeyPart_Public)) {
                default_policy = DEFAULT_POLICY_ASYMM_KEY;
            }
            else if ((cipherType == kSSS_CipherType_Binary) || (cipherType == kSSS_CipherType_Count) ||
                     (cipherType == kSSS_CipherType_PCR)) {
                default_policy = DEFAULT_POLICY_BIN_COUNT_PCR;
            }
            else if (cipherType == kSSS_CipherType_UserID) {
                default_policy = DEFAULT_POLICY_USERID;
            }
            else
                default_policy = DEFAULT_POLICY_SYMM_KEY;
        }
#ifdef DEBUG_PKCS11_PAL
        LOG_I("Default policy : 0x%02X, Required policy : 0x%02X", default_policy, policy_map);
#endif

        if (default_policy & policy_map)
            *pAllow = CK_TRUE;
        else
            *pAllow = CK_FALSE;
    }

    return kStatus_SSS_Success;
}
#endif

/**
 * @brief Query the value of the specified cryptographic object attribute.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetAttributeValue)
(CK_SESSION_HANDLE xSession, CK_OBJECT_HANDLE xObject, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);
    // LOG_I("%s : 0x%08X", __FUNCTION__, (uint32_t) xObject);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    CK_VOID_PTR pvAttr        = NULL;
    CK_ULONG ulAttrLength     = 0;
    CK_ULONG xP11KeyType, iAttrib, objectClass;
    CK_BBOOL supported = CK_FALSE;
    CK_HW_FEATURE_TYPE hwFeatureType;

    if (!pxTemplate) {
        return CKR_ARGUMENTS_BAD;
    }

    /*
     * Enumerate the requested attributes.
     */

    LOCK_MUTEX_FOR_RTOS
    {
        for (iAttrib = 0; iAttrib < ulCount && CKR_OK == xResult; iAttrib++) {
            /*
             * Get the attribute data and size.
             */

            ulAttrLength             = 0;
            size_t size              = 0;
            CK_BBOOL infoUnavailable = CK_FALSE;
            sss_object_t sss_object  = {0};
            uint8_t data[2048]       = {0};
            size_t dataLen           = sizeof(data);
            size_t KeyBitLen         = 2048;
            uint8_t *rsaE            = NULL;
            size_t rsaElen;
            uint8_t *rsaN = NULL;
            size_t rsaNlen;
            uint16_t outKeyIndex;
            size_t pubKeyLen;
            char label[80];
            uint32_t keyId                     = 0;
            CK_CERTIFICATE_TYPE cert_type      = CKC_X_509;
            CK_MECHANISM_TYPE rsa_mechanisms[] = {
                /* RSA Algorithms */
                CKM_RSA_PKCS,
                CKM_SHA1_RSA_PKCS,
                CKM_SHA224_RSA_PKCS,
                CKM_SHA256_RSA_PKCS,
                CKM_SHA384_RSA_PKCS,
                CKM_SHA512_RSA_PKCS,
                CKM_RSA_PKCS_PSS,
                CKM_SHA1_RSA_PKCS_PSS,
                CKM_SHA224_RSA_PKCS_PSS,
                CKM_SHA256_RSA_PKCS_PSS,
                CKM_SHA384_RSA_PKCS_PSS,
                CKM_SHA512_RSA_PKCS_PSS,
                CKM_RSA_PKCS_OAEP,
            };
            CK_MECHANISM_TYPE aes_mechanisms[] = {
                /* AES Algorithms  */
                CKM_AES_ECB,
                CKM_AES_CBC,
                CKM_AES_CTR,
            };
            CK_MECHANISM_TYPE ecc_mechanisms[] = {
                /* ECDSA */
                CKM_ECDSA,
                CKM_ECDSA_SHA1,
            };
            CK_MECHANISM_TYPE des_mechanisms[] = {
                /* DES Algorithms  */
                CKM_DES_ECB,
                CKM_DES_CBC,
            };
#if SSS_HAVE_APPLET_SE05X_IOT
            sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)(&(pex_sss_demo_boot_ctx->session));
#endif
#if SSS_HAVE_SE05X_VER_GTE_06_00
            se05x_object_attribute obj_attr = {0};
#endif

            // LOG_I("Attribute required : 0x%08lx\n", pxTemplate[ iAttrib ].type);

            switch (pxTemplate[iAttrib].type) {
            /* Common key attributes */
            case CKA_ID:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                keyId        = sss_object.keyId;
                pvAttr       = &keyId;
                ulAttrLength = sizeof(keyId);
                break;

            case CKA_CERTIFICATE_TYPE:
                ulAttrLength = sizeof(cert_type);
                pvAttr       = &cert_type;
                break;
            case CKA_LABEL:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                keyId = sss_object.keyId;
                snprintf(label, sizeof(label), "sss:%08X", (unsigned int)keyId);
                ulAttrLength = strlen(label);
                pvAttr       = (char *)&label[0];
                break;
            case CKA_ALWAYS_AUTHENTICATE:
                supported    = CK_FALSE;
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);
                break;
            case CKA_TOKEN:
                supported    = CK_TRUE; /* Object is always on token */
                ulAttrLength = sizeof(supported);
                pvAttr       = &(supported);
                break;

            case CKA_KEY_TYPE:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                switch (sss_object.cipherType) {
                case kSSS_CipherType_RSA:
                case kSSS_CipherType_RSA_CRT:
                    xP11KeyType = CKK_RSA;
                    break;
                case kSSS_CipherType_EC_NIST_P:
                    xP11KeyType = CKK_EC;
                    break;
                case kSSS_CipherType_AES:
                case kSSS_CipherType_DES:
                    xP11KeyType = CKK_AES;
                    break;
                default:
                    xResult = CKR_ATTRIBUTE_VALUE_INVALID;
                    break;
                }

                ulAttrLength = sizeof(xP11KeyType);
                pvAttr       = &xP11KeyType;
                break;

            case CKA_VALUE:

                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                switch (sss_object.cipherType) {
                case kSSS_CipherType_Binary:
                    if (kStatus_SSS_Success !=
                        sss_key_store_get_key(
                            &pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }
                    pvAttr       = (void *)&data[0];
                    ulAttrLength = dataLen;
                    break;
                case kSSS_CipherType_RSA:
                case kSSS_CipherType_EC_NIST_P:
                    if (sss_object.objectType == kSSS_KeyPart_Pair || sss_object.objectType == kSSS_KeyPart_Private) {
                        ulAttrLength = 0;
                        xResult      = CKR_ATTRIBUTE_SENSITIVE;
                        break;
                    }
                    if (kStatus_SSS_Success !=
                        sss_key_store_get_key(
                            &pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }
                    pvAttr       = (void *)&data[0];
                    ulAttrLength = dataLen;
                    break;
                case kSSS_CipherType_AES:
                case kSSS_CipherType_DES:
                    ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                    xResult      = CKR_ATTRIBUTE_SENSITIVE;
                    LOG_W("Not allowed to readout Symmetric key value");
                    break;
#if SSS_HAVE_APPLET_SE05X_IOT
                case kSSS_CipherType_Count:
                    if (kStatus_SSS_Success !=
                        sss_key_store_get_key(
                            &pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }

                    // Follow the spec. Increase counter each time its value is read.
                    if (SM_OK != Se05x_API_IncCounter(&se05x_session->s_ctx, sss_object.keyId)) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }

                    pvAttr       = (void *)&data[0];
                    ulAttrLength = dataLen;
                    break;
#endif
                default:
                    ulAttrLength = 0;
                    xResult      = CKR_ARGUMENTS_BAD;
                    break;
                }

                break;

            case CKA_VALUE_LEN:
                pvAttr       = NULL;
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
#if SSS_HAVE_APPLET_SE05X_IOT
                xResult = read_object_size(xObject, &outKeyIndex);
                if (xResult != CKR_OK) {
                    break;
                }
                size         = (size_t)outKeyIndex;
                pvAttr       = (void *)&size;
                ulAttrLength = sizeof(size_t);
#endif
                break;
            case CKA_MODULUS_BITS:
            case CKA_PRIME_BITS:
                /*
                 * Key strength size query, handled the same for RSA or ECDSA
                 * in this port.
                 */
                pvAttr       = NULL;
                ulAttrLength = 0;
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (((pxTemplate[iAttrib].type == CKA_MODULUS_BITS) &&
                        (sss_object.cipherType != kSSS_CipherType_RSA_CRT &&
                            sss_object.cipherType != kSSS_CipherType_RSA)) ||
                    ((pxTemplate[iAttrib].type == CKA_PRIME_BITS) &&
                        sss_object.cipherType != kSSS_CipherType_EC_NIST_P)) {
                    xResult = CKR_ARGUMENTS_BAD;
                    break;
                }
#if SSS_HAVE_APPLET_SE05X_IOT
                xResult = read_object_size(xObject, &outKeyIndex);
                if (xResult != CKR_OK) {
                    break;
                }
#else
                outKeyIndex = 32;
#endif
                size         = (size_t)outKeyIndex * 8;
                pvAttr       = (void *)&size;
                ulAttrLength = sizeof(size_t);
                break;

            case CKA_VENDOR_DEFINED:

                /*
                 * Return the key context for application-layer use.
                 */
                ulAttrLength = sizeof(pxSession->pxCurrentKey->xMbedPkCtx);
                pvAttr       = &pxSession->pxCurrentKey->xMbedPkCtx;
                break;

            case CKA_MODULUS:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                // CKA_MODULUS is only valid for RSA Key. Issue triggered by OpenSSH7.6(SIMW-2669)
                if ((sss_object.cipherType != kSSS_CipherType_RSA) &&
                    (sss_object.cipherType != kSSS_CipherType_RSA_CRT)) {
                    LOG_W("Object %08X cipher type is not RSA.", (unsigned int)sss_object.keyId);
                    ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                    xResult      = CKR_ARGUMENTS_BAD;
                    break;
                }

                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success !=
                    sss_util_asn1_rsa_parse_public(&data[0], dataLen, &rsaN, &rsaNlen, &rsaE, &rsaElen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                ulAttrLength = rsaNlen;
                /* TODO: Why was this done??? */
                // ulAttrLength = rsaNlen + 1;
                pvAttr = (void *)rsaN;

                break;

            case CKA_PUBLIC_EXPONENT:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                // CKA_MODULUS is only valid for RSA Key. Issue triggered by OpenSSH7.6(SIMW-2669)
                if ((sss_object.cipherType != kSSS_CipherType_RSA) &&
                    (sss_object.cipherType != kSSS_CipherType_RSA_CRT)) {
                    LOG_W("Object %08X cipher type is not RSA.", (unsigned int)sss_object.keyId);
                    ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                    xResult      = CKR_ARGUMENTS_BAD;
                    break;
                }

                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success !=
                    sss_util_asn1_rsa_parse_public(&data[0], dataLen, &rsaN, &rsaNlen, &rsaE, &rsaElen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                ulAttrLength = rsaElen;
                pvAttr       = (void *)rsaE;

                break;

            case CKA_EC_PARAMS:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                xResult      = EcPublickeyGetEcParams(&data[0], &dataLen);
                ulAttrLength = dataLen;
                pvAttr       = &data[0];

                break;

            case CKA_EC_POINT:
                KeyBitLen = 256;

                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success !=
                    sss_util_pkcs8_asn1_get_ec_public_key_index(&data[0], dataLen, &outKeyIndex, &pubKeyLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if (pubKeyLen <= 127) {
                    if (outKeyIndex < 1) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }
                    outKeyIndex       = outKeyIndex - 1;
                    data[outKeyIndex] = pubKeyLen;
                    pubKeyLen++;
                }
                else if (pubKeyLen <= 255) {
                    if (outKeyIndex < 2) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }
                    outKeyIndex           = outKeyIndex - 2;
                    data[outKeyIndex]     = 0x81;
                    data[outKeyIndex + 1] = pubKeyLen;
                    pubKeyLen             = pubKeyLen + 2;
                }
                else {
                    if (outKeyIndex < 3) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }
                    outKeyIndex           = outKeyIndex - 3;
                    data[outKeyIndex]     = 0x82;
                    data[outKeyIndex + 1] = (pubKeyLen & 0x00FF00) >> 8;
                    data[outKeyIndex + 2] = (pubKeyLen & 0x00FF);
                    pubKeyLen             = pubKeyLen + 3;
                }

                pubKeyLen++;
                if (outKeyIndex < 1) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                outKeyIndex = outKeyIndex - 1;

                data[outKeyIndex] = ASN_TAG_OCTETSTRING;

                ulAttrLength = pubKeyLen;
                pvAttr       = &data[outKeyIndex];
                break;

            case CKA_CLASS:

                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if (sss_object.objectType == kSSS_KeyPart_Private || sss_object.objectType == kSSS_KeyPart_Pair) {
                    objectClass  = CKO_PRIVATE_KEY;
                    pvAttr       = &objectClass;
                    ulAttrLength = sizeof(objectClass);
                }
                else if (sss_object.objectType == kSSS_KeyPart_Public) {
                    objectClass  = CKO_PUBLIC_KEY;
                    pvAttr       = &objectClass;
                    ulAttrLength = sizeof(objectClass);
                }
                else if (sss_object.objectType == kSSS_KeyPart_Default) {
                    if (sss_object.cipherType == kSSS_CipherType_Binary) {
                        CK_BBOOL isX509Cert = CK_FALSE;
                        isX509Cert          = isX509Certificate(sss_object.keyId);

                        if (isX509Cert == CK_TRUE) {
                            objectClass  = CKO_CERTIFICATE;
                            pvAttr       = &objectClass;
                            ulAttrLength = sizeof(objectClass);
                        }
                        else {
                            objectClass = CKO_DATA;
                            pvAttr      = &objectClass;
                            ;
                            ulAttrLength = sizeof(objectClass);
                        }
                    }
                    else if (sss_object.cipherType == kSSS_CipherType_Count) {
                        objectClass  = CKO_HW_FEATURE;
                        pvAttr       = &objectClass;
                        ulAttrLength = sizeof(objectClass);
                    }
                    else if ((sss_object.cipherType == kSSS_CipherType_UserID) ||
                             (sss_object.cipherType == kSSS_CipherType_PCR)) {
                        objectClass  = CKO_DATA;
                        pvAttr       = &objectClass;
                        ulAttrLength = sizeof(objectClass);
                    }
                    else {
                        objectClass  = CKO_SECRET_KEY;
                        pvAttr       = &objectClass;
                        ulAttrLength = sizeof(objectClass);
                    }
                }
                else {
                    ulAttrLength    = CK_UNAVAILABLE_INFORMATION;
                    xResult         = CKR_ATTRIBUTE_SENSITIVE;
                    infoUnavailable = CK_TRUE;
                }
                break;

            case CKA_HW_FEATURE_TYPE:

                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if ((sss_object.objectType == kSSS_KeyPart_Default) &&
                    (sss_object.cipherType == kSSS_CipherType_Count)) {
                    hwFeatureType = CKH_MONOTONIC_COUNTER;
                    pvAttr        = &hwFeatureType;
                    ulAttrLength  = sizeof(hwFeatureType);
                }
                else {
                    ulAttrLength    = CK_UNAVAILABLE_INFORMATION;
                    xResult         = CKR_ATTRIBUTE_SENSITIVE;
                    infoUnavailable = CK_TRUE;
                }
                break;

            case CKA_ENCRYPT:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (sss_object.objectType == kSSS_KeyPart_Public) {
                    if (sss_object.cipherType == kSSS_CipherType_RSA_CRT ||
                        sss_object.cipherType == kSSS_CipherType_RSA) {
                        supported    = CK_TRUE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                    else {
                        supported    = CK_FALSE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                }
                else if (sss_object.objectType == kSSS_KeyPart_Default) {
                    if (sss_object.cipherType == kSSS_CipherType_AES) {
                        supported    = CK_TRUE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                    else {
                        supported    = CK_FALSE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                }
                else {
                    supported    = CK_FALSE;
                    pvAttr       = &supported;
                    ulAttrLength = sizeof(supported);
                }
                break;
            case CKA_DECRYPT:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (sss_object.objectType == kSSS_KeyPart_Private || sss_object.objectType == kSSS_KeyPart_Pair) {
                    if (sss_object.cipherType == kSSS_CipherType_RSA_CRT ||
                        sss_object.cipherType == kSSS_CipherType_RSA) {
                        supported    = CK_TRUE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                    else {
                        supported    = CK_FALSE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                }
                else if (sss_object.objectType == kSSS_KeyPart_Default) {
                    if (sss_object.cipherType == kSSS_CipherType_AES) {
                        supported    = CK_TRUE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                    else {
                        supported    = CK_FALSE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                }
                else {
                    supported    = CK_FALSE;
                    pvAttr       = &supported;
                    ulAttrLength = sizeof(supported);
                }
                break;
            case CKA_SIGN:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (sss_object.objectType == kSSS_KeyPart_Private || sss_object.objectType == kSSS_KeyPart_Pair) {
                    if (sss_object.cipherType == kSSS_CipherType_RSA_CRT ||
                        sss_object.cipherType == kSSS_CipherType_RSA ||
                        sss_object.cipherType == kSSS_CipherType_EC_NIST_P) {
                        supported    = CK_TRUE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                    else {
                        supported    = CK_FALSE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                }
                else {
                    supported    = CK_FALSE;
                    pvAttr       = &supported;
                    ulAttrLength = sizeof(supported);
                }
                break;
            case CKA_VERIFY:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if (sss_object.objectType == kSSS_KeyPart_Public) {
                    if (sss_object.cipherType == kSSS_CipherType_RSA_CRT ||
                        sss_object.cipherType == kSSS_CipherType_RSA ||
                        sss_object.cipherType == kSSS_CipherType_EC_NIST_P) {
                        supported    = CK_TRUE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                    else {
                        supported    = CK_FALSE;
                        pvAttr       = &supported;
                        ulAttrLength = sizeof(supported);
                    }
                }
                else {
                    supported    = CK_FALSE;
                    pvAttr       = &supported;
                    ulAttrLength = sizeof(supported);
                }
                break;

            case CKA_WRAP:
            case CKA_UNWRAP:
            case CKA_SIGN_RECOVER:
            case CKA_VERIFY_RECOVER:
                supported    = CK_FALSE;
                ulAttrLength = sizeof(supported);
                pvAttr       = &(supported);
                break;
            case CKA_DERIVE:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (sss_object.objectType == kSSS_KeyPart_Pair && sss_object.cipherType == kSSS_CipherType_EC_NIST_P) {
                    supported    = CK_TRUE;
                    ulAttrLength = sizeof(supported);
                    pvAttr       = &(supported);
                }
                else {
                    supported    = CK_FALSE;
                    ulAttrLength = sizeof(supported);
                    pvAttr       = &(supported);
                }
                break;

            case CKA_HASH_OF_SUBJECT_PUBLIC_KEY:
            case CKA_HASH_OF_ISSUER_PUBLIC_KEY:
            case CKA_SUBJECT:
                ulAttrLength = sizeof(data);
                xResult =
                    parseCertificateGetAttribute((uint32_t)xObject, pxTemplate[iAttrib].type, &data[0], &ulAttrLength);
                if (xResult != CKR_OK) {
                    pvAttr       = NULL;
                    ulAttrLength = 0;
                }
                else {
                    pvAttr = &data[0];
                }
                break;

            case CKA_OPENSC_NON_REPUDIATION_0_17:
            case CKA_OPENSC_NON_REPUDIATION_0_21:
                //Not support NON-REPUDIATION signature
                supported    = CK_FALSE;
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);
                break;

            case CKA_SENSITIVE:
            case CKA_ALWAYS_SENSITIVE:
                supported = CK_FALSE;
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if (sss_object.objectType == kSSS_KeyPart_Private || sss_object.objectType == kSSS_KeyPart_Pair) {
                    // Private key
                    supported = CK_TRUE;
                }
                else if (sss_object.objectType == kSSS_KeyPart_Default) {
                    if ((sss_object.cipherType != kSSS_CipherType_Binary) &&
                        (sss_object.cipherType != kSSS_CipherType_Count)) {
                        // Secret key
                        supported = CK_TRUE;
                    }
                }

                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);

                break;

            case CKA_EXTRACTABLE:
                supported = CK_TRUE;
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

#if SSS_HAVE_SE05X_VER_GTE_06_00
                // Get attribute
                if (SM_OK != Se05x_API_ReadObjectAttributes(&se05x_session->s_ctx, sss_object.keyId, data, &dataLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if (kStatus_SSS_Success != parseAtrribute(&obj_attr,
                                               data,
                                               dataLen,
                                               sss_object.objectType,
                                               sss_object.cipherType,
                                               POLICY_OBJ_ALLOW_IMPORT_EXPORT,
                                               &supported)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

#else
                // SE050 doesn't support ReadObjectAttributes, so use pre-defined value according to key type.
                supported = CK_TRUE;
                if (sss_object.objectType == kSSS_KeyPart_Private || sss_object.objectType == kSSS_KeyPart_Pair) {
                    // Private key
                    supported = CK_FALSE;
                }
                else if (sss_object.objectType == kSSS_KeyPart_Default) {
                    if ((sss_object.cipherType != kSSS_CipherType_Binary) &&
                        (sss_object.cipherType != kSSS_CipherType_Count)) {
                        // Secret key
                        supported = CK_FALSE;
                    }
                }
#endif /* SSS_HAVE_SE05X_VER_GTE_06_00 */
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);

                break;

            case CKA_NEVER_EXTRACTABLE:
                //Not NEVER_EXTRACTABLE
                supported    = CK_FALSE;
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);
                break;

            case CKA_LOCAL:

#if SSS_HAVE_SE05X_VER_GTE_06_00
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                // Get attribute
                if (SM_OK != Se05x_API_ReadObjectAttributes(&se05x_session->s_ctx, sss_object.keyId, data, &dataLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                // Parse attribute for Origin value
                if (kStatus_SSS_Success !=
                    parseAtrribute(&obj_attr, data, dataLen, sss_object.objectType, sss_object.cipherType, 0, NULL)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if (obj_attr.origin == kSE05x_Origin_INTERNAL) {
                    supported = CK_TRUE;
                }
                else
                    supported = CK_FALSE;
#else
                // SE050 doesn't support ReadObjectAttributes, so use pre-defined value.
                supported = CK_FALSE;
#endif /* SSS_HAVE_SE05X_VER_GTE_06_00 */
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);

                break;

            case CKA_ALLOWED_MECHANISMS:
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                switch (sss_object.cipherType) {
                case kSSS_CipherType_RSA:
                case kSSS_CipherType_RSA_CRT:
                    pvAttr       = (void *)rsa_mechanisms;
                    ulAttrLength = sizeof(rsa_mechanisms);

                    break;
                case kSSS_CipherType_EC_NIST_P:
                case kSSS_CipherType_EC_BRAINPOOL:
                case kSSS_CipherType_EC_NIST_K:
                case kSSS_CipherType_EC_TWISTED_ED:
                case kSSS_CipherType_EC_MONTGOMERY:
                case kSSS_CipherType_EC_BARRETO_NAEHRIG:
                    pvAttr       = (void *)ecc_mechanisms;
                    ulAttrLength = sizeof(ecc_mechanisms);

                    break;
                case kSSS_CipherType_AES:
                    pvAttr       = (void *)aes_mechanisms;
                    ulAttrLength = sizeof(aes_mechanisms);

                    break;
                case kSSS_CipherType_DES:
                    pvAttr       = (void *)des_mechanisms;
                    ulAttrLength = sizeof(des_mechanisms);

                    break;
                default:
                    ulAttrLength = 0;
                    xResult      = CKR_ARGUMENTS_BAD;
                    break;
                }

                break;

            case CKA_APPLICATION:
            case CKA_OBJECT_ID:
                // CKA_APPLICATION: Description of the application that manages the object (default empty)
                // CKA_VALUE: DER-encoding of the object identifier indicating the data object type (default empty)
                pvAttr       = NULL;
                ulAttrLength = 0;

                break;

            case CKA_MODIFIABLE:
                supported = CK_TRUE;
                if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

#if SSS_HAVE_SE05X_VER_GTE_06_00
                // Get attribute
                if (SM_OK != Se05x_API_ReadObjectAttributes(&se05x_session->s_ctx, sss_object.keyId, data, &dataLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                if (kStatus_SSS_Success != parseAtrribute(&obj_attr,
                                               data,
                                               dataLen,
                                               sss_object.objectType,
                                               sss_object.cipherType,
                                               POLICY_OBJ_ALLOW_WRITE,
                                               &supported)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

#else
                // SE050 doesn't support ReadObjectAttributes, so use pre-defined value.
                supported = CK_TRUE;
#endif /* SSS_HAVE_SE05X_VER_GTE_06_00 */
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);

                break;

            case CKA_PRIVATE:
                // When the CKA_PRIVATE attribute is CK_TRUE, a user may not access the object until
                // the user has been authenticated to the token.
                supported    = CK_FALSE;
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);

                break;

            default:
                LOG_W("Attribute required : 0x%08lx\n", pxTemplate[iAttrib].type);
                ulAttrLength    = CK_UNAVAILABLE_INFORMATION;
                infoUnavailable = CK_TRUE;

                xResult = CKR_ATTRIBUTE_SENSITIVE;
                break;
            }

            if (CKR_OK == xResult) {
                /*
                 * Copy out the data and size.
                 */

                if (NULL != pxTemplate[iAttrib].pValue && !infoUnavailable && (NULL != pvAttr)) {
                    if (pxTemplate[iAttrib].ulValueLen < ulAttrLength) {
                        xResult      = CKR_BUFFER_TOO_SMALL;
                        ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                    }
                    else {
                        memcpy(pxTemplate[iAttrib].pValue, pvAttr, ulAttrLength);
                    }
                }
            }
            pxTemplate[iAttrib].ulValueLen = ulAttrLength;
            if (rsaN) {
                SSS_FREE(rsaN);
            }
            if (rsaE) {
                SSS_FREE(rsaE);
            }
        }

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}

/**
 * @brief Begin an enumeration sequence for the objects of the specified type.
 */
CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsInit)
(CK_SESSION_HANDLE xSession, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount)
{ /*lint !e9072 It's OK to have different parameter name. */
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    // printf("\n\n%s\n\n", __FUNCTION__);

    // ( void ) ( ulCount );

    /*
     * Allow filtering on a single object class attribute.
     */
    pxSession->xFindObjectInit     = CK_TRUE;
    pxSession->xFindObjectComplete = CK_FALSE;
    pxSession->xFindObjectClass    = pkcs11INVALID_OBJECT_CLASS; /* Invalid Class */
    pxSession->xFindObjectKeyType  = pkcs11INVALID_KEY_TYPE;     /* Invalid Key Type */

    if (!pxTemplate) {
        pxSession->labelPresent          = CK_FALSE;
        pxSession->keyIdPresent          = CK_FALSE;
        pxSession->xFindObjectClass      = pkcs11INVALID_OBJECT_CLASS; /* Invalid Class */
        pxSession->xFindObjectKeyType    = pkcs11INVALID_KEY_TYPE;     /* Invalid Key Type */
        pxSession->xFindObjectTotalFound = 0;
        return CKR_OK;
        // return CKR_ARGUMENTS_BAD;
    }

    int classIndex      = 0;
    CK_BBOOL foundClass = CK_FALSE;
    CK_ULONG i          = 0;
    for (i = 0; i < ulCount; i++) {
        if (pxTemplate[i].type == CKA_LABEL) {
            pxSession->labelPresent = CK_TRUE;
            snprintf(pxSession->label, sizeof(pxSession->label), "%s", (char *)pxTemplate[i].pValue);
            // pxSession->label = (unsigned char *) pxTemplate[i].pValue;
            // memcpy(pxSession->label, pxTemplate[i].pValue, strlen((char *)pxTemplate[i].pValue));
            // memcpy(pxSession->label, pxTemplate[i].pValue, pxTemplate[i].ulValueLen);
            pxSession->labelLen = pxTemplate[i].ulValueLen;
            // printf("\nFound label\n");
        }
        else if (pxTemplate[i].type == CKA_CLASS) {
            classIndex = i;
            foundClass = CK_TRUE;
        }
        else if (pxTemplate[i].type == CKA_SSS_ID) {
            pxSession->keyIdPresent = CK_TRUE;
            pxSession->keyId        = *((uint32_t *)(pxTemplate[i].pValue));
        }
        else if (pxTemplate[i].type == CKA_KEY_TYPE) {
            memcpy(&pxSession->xFindObjectKeyType, pxTemplate[i].pValue, sizeof(CK_KEY_TYPE));
        }

        // keyIdPresent
    }
    if (foundClass) {
        // if(pxTemplate[0].type == CKA_CLASS) {
        memcpy(&pxSession->xFindObjectClass, pxTemplate[classIndex].pValue, sizeof(CK_OBJECT_CLASS));

        // }
    }

    return CKR_OK;
}

/**
 * @brief Query the objects of the requested type.
 */
CK_DEFINE_FUNCTION(CK_RV, C_FindObjects)
(CK_SESSION_HANDLE xSession, CK_OBJECT_HANDLE_PTR pxObject, CK_ULONG ulMaxObjectCount, CK_ULONG_PTR pulObjectCount)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    BaseType_t xDone          = pdFALSE;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    uint32_t keyId            = 0x0;

    /*
     * Check parameters.
     */

    if ((CK_BBOOL)CK_FALSE == pxSession->xFindObjectInit) {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
        xDone   = pdTRUE;
        return xResult;
    }

    if ((pdFALSE == xDone) && (0u == ulMaxObjectCount)) {
        xResult = CKR_ARGUMENTS_BAD;
        xDone   = pdTRUE;
        return xResult;
    }

    if ((pdFALSE == xDone) && (!pxObject || !pulObjectCount)) {
        xResult = CKR_ARGUMENTS_BAD;
        xDone   = pdTRUE;
        return xResult;
    }

    if ((pdFALSE == xDone) && ((CK_BBOOL)CK_TRUE == pxSession->xFindObjectComplete)) {
        *pulObjectCount = 0;
        xResult         = CKR_OK;
        xDone           = pdTRUE;
        return xResult;
    }

    /*
     * Load the default private key and certificate.
     */

    if ((pdFALSE == xDone) && pxSession->labelPresent) {
        if (pxSession->labelLen == 0) {
            *pulObjectCount = 0;
            xResult         = CKR_FUNCTION_FAILED;
        }
        else {
            if (pxSession->xFindObjectTotalFound == 1) {
                *pulObjectCount = 0;
            }
            else {
                xResult = LabelToKeyId((unsigned char *)pxSession->label, pxSession->labelLen, &keyId);
                if (xResult == CKR_OK) {
                    sss_status_t status = kStatus_SSS_Fail;
                    sss_object_t object = {0};

                    LOCK_MUTEX_FOR_RTOS
                    {
                        status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
                        if (status != kStatus_SSS_Success) {
                            xResult = CKR_FUNCTION_FAILED;
                        }
                        else {
                            status = sss_key_object_get_handle(&object, keyId);
                            if (status != kStatus_SSS_Success) {
                                *pulObjectCount = 0;
                                // xResult = CKR_FUNCTION_FAILED;
                            }
                            else {
                                *pxObject                        = keyId;
                                *pulObjectCount                  = 1;
                                pxSession->xFindObjectTotalFound = 1;
                            }
                        }
                        UNLOCK_MUTEX_FOR_RTOS
                    }
                }
            }
        }
        xDone = pdTRUE;
    }
    else if ((pdFALSE == xDone) && pxSession->keyIdPresent) {
        keyId               = pxSession->keyId;
        sss_status_t status = kStatus_SSS_Fail;
        sss_object_t object = {0};

        if (pxSession->xFindObjectTotalFound == 1) {
            *pulObjectCount = 0;
        }
        else {
            LOCK_MUTEX_FOR_RTOS
            {
                status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
                if (status != kStatus_SSS_Success) {
                    xResult = CKR_FUNCTION_FAILED;
                }
                else {
                    status = sss_key_object_get_handle(&object, keyId);
                    if (status != kStatus_SSS_Success) {
                        *pulObjectCount = 0;
                        // xResult = CKR_FUNCTION_FAILED;
                    }
                    else {
                        *pxObject                        = keyId;
                        *pulObjectCount                  = 1;
                        pxSession->xFindObjectTotalFound = 1;
                    }
                }
                UNLOCK_MUTEX_FOR_RTOS
            }
        }

        xDone = pdTRUE;
    }

    else if ((pdFALSE == xDone)) {
#if SSS_HAVE_ALT_SSS && SSS_HAVE_APPLET_SE05X_IOT
        uint32_t object_list[40] = {0};
        size_t object_list_size  = sizeof(object_list) / sizeof(object_list[0]);
        smStatus_t sm_status     = read_id_list(object_list, &object_list_size);
        if (sm_status != SM_OK) {
            xResult = CKR_FUNCTION_FAILED;
            xDone   = pdTRUE;
            return xResult;
        }

        size_t i                       = 0;
        uint32_t skipped               = 0;
        CK_OBJECT_HANDLE_PTR ckObjects = (CK_OBJECT_HANDLE_PTR)SSS_MALLOC(sizeof(CK_OBJECT_HANDLE) * ulMaxObjectCount);
        *pulObjectCount                = 0;

        LOCK_MUTEX_FOR_RTOS
        {
            for (i = 0; ((i < object_list_size) && (*pulObjectCount < ulMaxObjectCount)); i++) {
                uint32_t id = object_list[i];
                sss_object_t object;
                sss_status_t status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
                if (status != kStatus_SSS_Success) {
                    LOG_E("Object init failed. Should not reach here");
                    continue;
                }
                status = sss_key_object_get_handle(&object, id);
                if (status != kStatus_SSS_Success) {
                    LOG_E("Object get handle failed. Should not reach here");
                    continue;
                }
                if (pxSession->xFindObjectClass == pkcs11INVALID_OBJECT_CLASS &&
                    pxSession->xFindObjectKeyType == pkcs11INVALID_KEY_TYPE) {
                    if (skipped < pxSession->xFindObjectTotalFound) {
                        skipped++;
                        continue;
                    }
                    memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                    (*pulObjectCount)++;
                }
                else if (pxSession->xFindObjectClass != pkcs11INVALID_OBJECT_CLASS &&
                         pxSession->xFindObjectKeyType == pkcs11INVALID_KEY_TYPE) {
                    CK_BBOOL isX509Cert = CK_FALSE;
                    if (object.cipherType == kSSS_CipherType_Binary) {
                        isX509Cert = isX509Certificate(id);
                    }
                    if ((object.cipherType == kSSS_CipherType_Binary && isX509Cert == CK_TRUE &&
                            pxSession->xFindObjectClass == CKO_CERTIFICATE) ||
                        (object.objectType == kSSS_KeyPart_Pair && pxSession->xFindObjectClass == CKO_PRIVATE_KEY) ||
                        (object.objectType == kSSS_KeyPart_Public && pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) {
                        if (skipped < pxSession->xFindObjectTotalFound) {
                            skipped++;
                            continue;
                        }
                        memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                        (*pulObjectCount)++;
                    }
                }
                else if (pxSession->xFindObjectClass == pkcs11INVALID_OBJECT_CLASS &&
                         pxSession->xFindObjectKeyType != pkcs11INVALID_KEY_TYPE) {
                    if ((object.cipherType == kSSS_CipherType_AES && pxSession->xFindObjectKeyType == CKK_AES) ||
                        (object.cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES) ||
                        (object.cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES2) ||
                        (object.cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES3) ||
                        (object.cipherType == kSSS_CipherType_RSA && pxSession->xFindObjectKeyType == CKK_RSA) ||
                        (object.cipherType == kSSS_CipherType_RSA_CRT && pxSession->xFindObjectKeyType == CKK_RSA) ||
                        (object.cipherType == kSSS_CipherType_EC_NIST_P && pxSession->xFindObjectKeyType == CKK_EC)) {
                        if (skipped < pxSession->xFindObjectTotalFound) {
                            skipped++;
                            continue;
                        }
                        memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                        (*pulObjectCount)++;
                    }
                }
                else if (pxSession->xFindObjectClass != pkcs11INVALID_OBJECT_CLASS &&
                         pxSession->xFindObjectKeyType != pkcs11INVALID_KEY_TYPE) {
                    CK_BBOOL isX509Cert = CK_FALSE;
                    if (object.cipherType == kSSS_CipherType_Binary) {
                        isX509Cert = isX509Certificate(id);
                    }
                    if ((object.cipherType == kSSS_CipherType_Binary && isX509Cert == CK_TRUE &&
                            pxSession->xFindObjectClass == CKO_CERTIFICATE) ||
                        (object.objectType == kSSS_KeyPart_Pair && pxSession->xFindObjectClass == CKO_PRIVATE_KEY) ||
                        (object.objectType == kSSS_KeyPart_Public && pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) {
                        if ((object.cipherType == kSSS_CipherType_AES && pxSession->xFindObjectKeyType == CKK_AES) ||
                            (object.cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES) ||
                            (object.cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES2) ||
                            (object.cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES3) ||
                            (object.cipherType == kSSS_CipherType_RSA && pxSession->xFindObjectKeyType == CKK_RSA) ||
                            (object.cipherType == kSSS_CipherType_RSA_CRT &&
                                pxSession->xFindObjectKeyType == CKK_RSA) ||
                            (object.cipherType == kSSS_CipherType_EC_NIST_P &&
                                pxSession->xFindObjectKeyType == CKK_EC)) {
                            if (skipped < pxSession->xFindObjectTotalFound) {
                                skipped++;
                                continue;
                            }
                            memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                            (*pulObjectCount)++;
                        }
                    }
                }
            }
            UNLOCK_MUTEX_FOR_RTOS
        }

        if (*pulObjectCount > 0) {
            memcpy(pxObject, ckObjects, (sizeof(CK_OBJECT_HANDLE) * (*pulObjectCount)));
            pxSession->xFindObjectTotalFound = pxSession->xFindObjectTotalFound + *pulObjectCount;
        }
        if (ckObjects) {
            SSS_FREE(ckObjects);
        }
#endif
        xDone = pdTRUE;
    }

    return xResult;
}

/**
 * @brief Terminate object enumeration.
 */
CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsFinal)(CK_SESSION_HANDLE xSession)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    /*
     * Check parameters.
     */

    if ((CK_BBOOL)CK_FALSE == pxSession->xFindObjectInit) {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
    }
    else {
        /*
         * Clean-up find objects state.
         */

        pxSession->labelPresent          = CK_FALSE;
        pxSession->keyIdPresent          = CK_FALSE;
        pxSession->xFindObjectInit       = CK_FALSE;
        pxSession->xFindObjectComplete   = CK_FALSE;
        pxSession->xFindObjectClass      = 0;
        pxSession->xFindObjectTotalFound = 0;
        pxSession->xFindObjectKeyType    = pkcs11INVALID_KEY_TYPE;
    }

    return xResult;
}

/**
 * @brief Start a session for a cryptographic command sequence.
 */
CK_DEFINE_FUNCTION(CK_RV, C_OpenSession)
(CK_SLOT_ID xSlotID, CK_FLAGS xFlags, CK_VOID_PTR pvApplication, CK_NOTIFY xNotify, CK_SESSION_HANDLE_PTR pxSession)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult                = CKR_OK;
    P11SessionPtr_t pxSessionObj = NULL;
    // printf("\n\n%s\n\n", __FUNCTION__);

    // ( void ) ( xSlotID );
    // ( void ) ( pvApplication );
    // ( void ) ( xNotify );

    /* Check arguments. */

    if (!cryptokiInitialized) {
        return CKR_CRYPTOKI_NOT_INITIALIZED;
    }

    if (NULL == pxSession) {
        xResult = CKR_ARGUMENTS_BAD;
    }

    if (xSlotID != pkcs11SLOT_ID) {
        return CKR_ARGUMENTS_BAD;
    }

    if (!(xFlags & CKF_SERIAL_SESSION)) {
        return CKR_SESSION_PARALLEL_NOT_SUPPORTED;
    }

    /*
     * Make space for the context.
     */
#if defined(USE_RTOS) && USE_RTOS == 1
    if (NULL == (pxSessionObj = (P11SessionPtr_t)pvPortMalloc(
                     sizeof(P11Session_t)))) /*lint !e9087 Allow casting void* to other types. */
    {
        xResult = CKR_HOST_MEMORY;
    }
#else
    if (NULL == (pxSessionObj = (P11SessionPtr_t)SSS_MALLOC(
                     sizeof(P11Session_t)))) /*lint !e9087 Allow casting void* to other types. */
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

    if (CKR_OK == xResult) {
        memset(pxSessionObj, 0, sizeof(P11Session_t));
    }

    /*
     * Initialize RNG.
     */

    if (CKR_OK == xResult) {
        memset(&pxSessionObj->xMbedEntropyContext, 0, sizeof(pxSessionObj->xMbedEntropyContext));
    }

    if (CKR_OK == xResult) {
        /*
         * Assign the session.
         */

        pxSessionObj->ulState = 0u != (xFlags & CKF_RW_SESSION) ? CKS_RW_PUBLIC_SESSION : CKS_RO_PUBLIC_SESSION;
        pxSessionObj->xOpened = CK_TRUE;

        /*
         * Return the session.
         */

        *pxSession = (CK_SESSION_HANDLE)pxSessionObj; /*lint !e923 Allow casting pointer to integer type for handle. */
    }

    if (NULL != pxSessionObj) {
        if (CKR_OK != xResult) {
            SSS_FREE(pxSessionObj);
            return CKR_FUNCTION_FAILED;
        }
    }
    else {
        return CKR_FUNCTION_FAILED;
    }

    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;

/*Lock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_lock(&gSessionlock);
#endif

    LOCK_MUTEX_FOR_RTOS
    {
        if (CKR_OK == xResult) {
            // if(pex_sss_demo_boot_ctx->session.subsystem == kType_SSS_SubSystem_NONE){
            if (sessionCount == 0) {
                sss_status_t sss_status = kStatus_SSS_Fail;
                const char *portName    = g_port_name;

                SM_Close(NULL, 0);
                /* If portname is given in ENV */
                sss_status = ex_sss_boot_connectstring(0, NULL, &portName);
                sss_status = ex_sss_boot_open(pex_sss_demo_boot_ctx, portName);
                if (sss_status != kStatus_SSS_Success) {
                    LOG_E("Session Open Failed");
                    xResult = CKR_GENERAL_ERROR;
                    goto error;
                }

#if EX_SSS_BOOT_DO_ERASE
                sss_status = ex_sss_boot_factory_reset(pex_sss_demo_boot_ctx);
                if (sss_status != kStatus_SSS_Success) {
                    LOG_E("Factory Reset failed");
                }
#endif

                sss_status = ex_sss_key_store_and_object_init(pex_sss_demo_boot_ctx);
                if (sss_status != kStatus_SSS_Success) {
                    LOG_E("Keystore Init Failed");
                    xResult = CKR_FUNCTION_FAILED;
                    goto error;
                }
                pex_sss_demo_tls_ctx->pHost_ks = &pex_sss_demo_boot_ctx->host_ks;
                sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->obj, &pex_sss_demo_boot_ctx->ks);
                if (sss_status != kStatus_SSS_Success) {
                    LOG_E("Object init failed");
                }
                sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->dev_cert, &pex_sss_demo_boot_ctx->ks);
                if (sss_status != kStatus_SSS_Success) {
                    LOG_E("Cert Object init failed");
                }
                sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->interCaCert, &pex_sss_demo_boot_ctx->ks);
                if (sss_status != kStatus_SSS_Success) {
                    LOG_E("IntermCA Object init failed");
                }
                sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->pub_obj, &pex_sss_demo_boot_ctx->ks);
                if (sss_status != kStatus_SSS_Success) {
                    LOG_E("Pub Object init failed");
                }
                UNLOCK_MUTEX_FOR_RTOS
                goto exit;
            error:
                if (pxSessionObj) {
                    SSS_FREE(pxSessionObj);
                }
                if (pex_sss_demo_boot_ctx->session.subsystem != kType_SSS_SubSystem_NONE) {
                    ex_sss_session_close(pex_sss_demo_boot_ctx);
                }
            }
        }

        UNLOCK_MUTEX_FOR_RTOS
    }

exit:
    if (xResult == CKR_OK) {
        sessionCount++;
    }
/*Unlock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_unlock(&gSessionlock);
    LOG_I("Destroyed mutex");
#endif

    return xResult;
}

/**
 * @brief Terminate a session and release resources.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CloseSession)(CK_SESSION_HANDLE xSession)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult             = CKR_OK;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    // printf("\n\n%s\n\n", __FUNCTION__);

    if (NULL != pxSession) {
        /*
         * Tear down the session.
         */
        /* PKCS11TB macro is required because in one of the PKCS 11 test case causes hard fault
         * since no buffer is allocated for certificate_buf and it causes hard fault when freeing
         * TODO: not deallocate memory when buffer is not allocated */
#if (defined(PKCS11TB) && (PKCS11TB == 0))
        if (NULL != pxSession->pxCurrentKey->certificate_buf) {
#if defined(USE_RTOS) && USE_RTOS == 1
            vPortFree(pxSession->pxCurrentKey->certificate_buf);
#else
            SSS_FREE(pxSession->pxCurrentKey->certificate_buf);
#endif
        }
#endif

#if defined(USE_RTOS) && USE_RTOS == 1
        vPortFree(pxSession);
#else
        SSS_FREE(pxSession);
#endif

/*Lock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__ && !AX_EMBEDDED)
        pthread_mutex_lock(&gSessionlock);
#endif
        if (CKR_OK == xResult) {
            // static const char *g_port_name;
            // printf("\nIn session close");
            if (sessionCount == 1) {
                // printf("\nsessionCount = 1");
                // sss_status_t sss_status = kStatus_SSS_Fail;
                LOCK_MUTEX_FOR_RTOS
                {
                    sss_key_object_free(&pex_sss_demo_tls_ctx->obj);
                    sss_key_object_free(&pex_sss_demo_tls_ctx->dev_cert);
                    sss_key_object_free(&pex_sss_demo_tls_ctx->interCaCert);
                    sss_key_object_free(&pex_sss_demo_tls_ctx->pub_obj);
                    ex_sss_session_close(pex_sss_demo_boot_ctx);
                    UNLOCK_MUTEX_FOR_RTOS
                }
            }
        }
    }
    else {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    if (xResult == CKR_OK) {
        sessionCount--;
    }
/*Unlock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_unlock(&gSessionlock);
    LOG_I("Destroyted mutex");
    LOG_I("SessionCount = %d", sessionCount);
#endif

    return xResult;
}

/**
 * @brief Cleans up a key structure.
 */

// static int convertdertopemformat(const unsigned char * int_ca_cert_der, int len_der, unsigned char * int_ca_cert_pem, int dst_buf_len, U16 *outlength)
// {
//     int ret;
//     size_t len_out;
//     const char start[]= "-----BEGIN CERTIFICATE-----\n";
//     const char end[]= "\n-----END CERTIFICATE-----\n";
//     strcpy((char *)int_ca_cert_pem,start);
//     ret = mbedtls_base64_encode(&int_ca_cert_pem[sizeof(start) - 1],dst_buf_len-sizeof(start)-sizeof(end)-1,&len_out,int_ca_cert_der,len_der);
//     if (ret == 0)
//     {
//         strcpy((char *) &int_ca_cert_pem[sizeof(start) - 1 + len_out],end);
//         *outlength = len_out + sizeof(start) + sizeof(end) - 1;
//     }
//     else
//     {
//         *outlength = 0;
//         ret = 1;
//     }
//     return ret;
// }
#if SSS_HAVE_ALT_SSS

static int convertpemtoder(const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen)
{
    int ret;
    const unsigned char *s1, *s2, *end = input + ilen;
    size_t len = 0;

    s1 = (unsigned char *)strstr((const char *)input, "-----BEGIN");
    if (s1 == NULL)
        return (-1);

    s2 = (unsigned char *)strstr((const char *)input, "-----END");
    if (s2 == NULL)
        return (-1);

    s1 += 10;
    while (s1 < end && *s1 != '-')
        s1++;
    while (s1 < end && *s1 == '-')
        s1++;
    if (*s1 == '\r')
        s1++;
    if (*s1 == '\n')
        s1++;

    if (s2 <= s1 || s2 > end)
        return (-1);

    ret = mbedtls_base64_decode(NULL, 0, &len, (const unsigned char *)s1, s2 - s1);
    if (ret == MBEDTLS_ERR_BASE64_INVALID_CHARACTER)
        return (ret);

    if (len > *olen)
        return (-1);

    if ((ret = mbedtls_base64_decode(output, len, &len, (const unsigned char *)s1, s2 - s1)) != 0) {
        return (ret);
    }

    *olen = len;

    return (0);
}

#endif

#if AX_EMBEDDED
BaseType_t PKCS11_PAL_SaveFile(char *pcFileName, uint8_t *pucData, uint32_t ulDataSize)
{
    return false;
}
#endif /* AX_EMBEDDED */

/**
 * @brief Begin a digital signature verification session.
 */

CK_DEFINE_FUNCTION(CK_RV, C_VerifyInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pxMechanism, CK_OBJECT_HANDLE xKey)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    // pxSession->xOperationInProgress = pxMechanism->mechanism;

    /*lint !e9072 It's OK to have different parameter name. */
    // ( void ) ( xSession );
    // ( void ) ( xKey );

    if (NULL == pxMechanism) {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if (pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pxMechanism->mechanism;
        pxSession->xOperationKeyHandle  = xKey;
        if (pxMechanism->pParameter) {
            pxSession->mechParameter    = pxMechanism->pParameter;
            pxSession->mechParameterLen = pxMechanism->ulParameterLen;
        }
        else {
            pxSession->mechParameterLen = 0;
        }
    }

    return xResult;
}

CK_RV ParseSignMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm)
{
    CK_RV xResult = CKR_OK;
    CK_RSA_PKCS_PSS_PARAMS_PTR param;
    switch (pxSession->xOperationInProgress) {
    case CKM_RSA_PKCS:
        *algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_NO_HASH;
        break;
    case CKM_RSA_PKCS_PSS:
        /*Also need the details about hash algorithm used*/
        if (!pxSession->mechParameterLen) {
            xResult = CKR_ARGUMENTS_BAD;
            break;
        }
        param = (CK_RSA_PKCS_PSS_PARAMS_PTR)pxSession->mechParameter;
        switch (param->hashAlg) {
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
        /* We will use algorithm according to digest size.
             * Should be checked from where this function is called
             */
        break;
    case CKM_ECDSA_SHA1:
        *algorithm = kAlgorithm_SSS_ECDSA_SHA1;
        break;

    default:
        xResult = CKR_MECHANISM_INVALID;
        break;
    }
    return xResult;
}

CK_RV ParseEncryptionMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm)
{
    /*    typedef struct CK_RSA_PKCS_OAEP_PARAMS {
        CK_MECHANISM_TYPE hashAlg;
        CK_RSA_PKCS_MGF_TYPE mgf;
        CK_RSA_PKCS_OAEP_SOURCE_TYPE source;
        CK_VOID_PTR pSourceData;
        CK_ULONG ulSourceDataLen;
    } CK_RSA_PKCS_OAEP_PARAMS;*/
    CK_RV xResult = CKR_OK;
    CK_RSA_PKCS_OAEP_PARAMS_PTR param;
    switch (pxSession->xOperationInProgress) {
    case CKM_RSA_PKCS:
    case CKM_SHA1_RSA_PKCS:
    case CKM_SHA256_RSA_PKCS:
    case CKM_SHA384_RSA_PKCS:
    case CKM_SHA512_RSA_PKCS:
    case CKM_SHA224_RSA_PKCS:
        *algorithm = kAlgorithm_SSS_RSAES_PKCS1_V1_5;
        break;

    case CKM_RSA_PKCS_OAEP:
        /* Also need the details about hash algorithm used */

        /* TODO: Also see how to use source, pSourceData, ulSourceDataLen
             *       in RSA_PKCS_OAEP_PARAMS
             */

        if (!pxSession->mechParameterLen) {
            xResult = CKR_ARGUMENTS_BAD;
            break;
        }
        param = (CK_RSA_PKCS_OAEP_PARAMS_PTR)pxSession->mechParameter;
        switch (param->hashAlg) {
        case CKM_SHA_1:
            if (param->mgf == CKG_MGF1_SHA1) {
                *algorithm = kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1;
            }
            else {
                xResult = CKR_MECHANISM_INVALID;
            }
            break;
        case CKM_SHA224:
            xResult = CKR_MECHANISM_INVALID;
            /**algorithm = kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA224;*/
            break;
        case CKM_SHA256:
            xResult = CKR_MECHANISM_INVALID;
            /**algorithm = kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA256;*/
            break;
        case CKM_SHA384:
            xResult = CKR_MECHANISM_INVALID;
            /**algorithm = kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA384;*/
            break;
        case CKM_SHA512:
            xResult = CKR_MECHANISM_INVALID;
            /**algorithm = kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA512;*/
            break;
        default:
            xResult = CKR_MECHANISM_INVALID;
            break;
        }
        break;
        // *algorithm = kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1;
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

CK_RV ParseDigestMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm)
{
    CK_RV xResult = CKR_OK;
    switch (pxSession->xOperationInProgress) {
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
CK_DEFINE_FUNCTION(CK_RV, C_SignInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pxMechanism, CK_OBJECT_HANDLE xKey)
{
    CK_RV xResult = CKR_OK;
    // LOG_I("KeyId = %ld", (uint32_t) xKey);
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    // pxSession->xOperationInProgress = pxMechanism->mechanism;

    /*lint !e9072 It's OK to have different parameter name. */
    // ( void ) ( xSession );
    // ( void ) ( xKey );

    if (NULL == pxMechanism) {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if (pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pxMechanism->mechanism;
        pxSession->xOperationKeyHandle  = xKey;
        if (pxMechanism->pParameter) {
            pxSession->mechParameter    = pxMechanism->pParameter;
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
CK_DEFINE_FUNCTION(CK_RV, C_GetSlotList)
(CK_BBOOL xTokenPresent, CK_SLOT_ID_PTR pxSlotList, CK_ULONG_PTR pulCount)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    // ( void ) ( xTokenPresent );

    if (NULL == pulCount) {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else if (NULL == pxSlotList) {
        *pulCount = 1;
    }
    else {
        // if( 0u == *pulCount )
        if (*pulCount < 1) {
            xResult = CKR_BUFFER_TOO_SMALL;
        }
        else {
            pxSlotList[0] = (CK_ULONG)pkcs11SLOT_ID;
            *pulCount     = 1;
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_CopyObject)
(CK_SESSION_HANDLE hSession,
    CK_OBJECT_HANDLE hObject,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulCount,
    CK_OBJECT_HANDLE_PTR phNewObject)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_Decrypt)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen)
{
    CK_RV xResult = CKR_OK;
    // return CKR_ARGUMENTS_BAD;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    if (!pEncryptedData) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }

    // sss_status_t status;
    // sss_asymmetric_t asymmCtx;
    sss_algorithm_t algorithm = kAlgorithm_None;
    // sss_algorithm_t digest_algorithm = kAlgorithm_None;

    xResult = ParseEncryptionMechanism(pxSessionObj, &algorithm);
    // printf("\nEncrypt Mechanism = %08x\n", pxSessionObj->xOperationInProgress);
    if (xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }
    if (algorithm == kAlgorithm_None) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }
    if ((algorithm == kAlgorithm_SSS_RSAES_PKCS1_V1_5) || (algorithm == kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1)) {
        /*RSA Encryption*/
        xResult = AsymmetricDecrypt(pxSessionObj, algorithm, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
    }
    else {
        /*Symmetric Encryption*/
        xResult = SymmetricDecrypt(pxSessionObj, algorithm, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(hSession);

    if (NULL == pMechanism) {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if (pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pMechanism->mechanism;
        pxSession->xOperationKeyHandle  = hKey;
        if (pMechanism->pParameter) {
            pxSession->mechParameter    = pMechanism->pParameter;
            pxSession->mechParameterLen = pMechanism->ulParameterLen;
        }
        else {
            pxSession->mechParameterLen = 0;
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG ulEncryptedPartLen,
    CK_BYTE_PTR pPart,
    CK_ULONG_PTR pulPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DeriveKey)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_OBJECT_HANDLE hBaseKey,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulAttributeCount,
    CK_OBJECT_HANDLE_PTR phKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);
    CK_RV xResult = CKR_FUNCTION_NOT_SUPPORTED;
#if SSS_HAVE_APPLET_SE05X_IOT
    uint32_t keyId                     = (uint32_t)hBaseKey;
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;
    sss_algorithm_t algorithm          = kAlgorithm_SSS_ECDH;
    sss_mode_t mode                    = kMode_SSS_ComputeSharedSecret;
    sss_object_t privKeyObj            = {0};
    sss_object_t pubKeyObj             = {0};
    sss_object_t derivedObject         = {0};
    sss_status_t sss_status            = kStatus_SSS_Fail;
    uint16_t keySize                   = 0;
    uint8_t tag                        = 0x00;
    uint8_t publicKeyBuffer[160]       = {0};
    size_t publicKeyBufferLen          = sizeof(publicKeyBuffer);
    uint8_t ecKeyParameters[30]        = {0};
    size_t ecKeyParametersLen          = sizeof(ecKeyParameters);
    uint8_t id_ecPublicKey[]           = ID_ECPUBLICKEY;
    CK_ULONG attributeIndex            = 0;
    uint32_t derivedKeyId              = 0;
    size_t derivedKeyLen               = 0;
    sss_derive_key_t ctx_derive_key    = {0};

    CK_MECHANISM_TYPE mechType = pMechanism->mechanism;
    if (mechType != CKM_ECDH1_DERIVE) {
        /*
         * We support ECDH and HKDF mechanisms for key derivation.
         * As per PKCS#11 v2.40, HKDF mechanism is not supported by
         * PKCS#11. CKM_HKDF_DERIVE is added in PKCS#11 v3.0
         */
        return CKR_MECHANISM_INVALID;
    }
    if (mechType == CKM_ECDH1_DERIVE) {
        if (!pMechanism->ulParameterLen) {
            return CKR_ARGUMENTS_BAD;
        }
        if (!pMechanism->pParameter) {
            return CKR_ARGUMENTS_BAD;
        }
        CK_ECDH1_DERIVE_PARAMS_PTR p_ecdh1Params = (CK_ECDH1_DERIVE_PARAMS_PTR)pMechanism->pParameter;
        if (!p_ecdh1Params->ulPublicDataLen || !p_ecdh1Params->pPublicData) {
            return CKR_ARGUMENTS_BAD;
        }
        if ((p_ecdh1Params->kdf != CKD_SHA1_KDF) && (p_ecdh1Params->kdf != CKD_SHA224_KDF) &&
            (p_ecdh1Params->kdf != CKD_SHA256_KDF) && (p_ecdh1Params->kdf != CKD_SHA384_KDF) &&
            (p_ecdh1Params->kdf != CKD_SHA512_KDF) && (p_ecdh1Params->kdf != CKD_NULL)) {
            return CKR_ARGUMENTS_BAD;
        }
        if (!CheckIfKeyIdExists(keyId, &se05x_session->s_ctx)) {
            return CKR_KEY_HANDLE_INVALID;
        }
        if ((GetAttributeParameterIndex(pTemplate, ulAttributeCount, CKA_CLASS, &attributeIndex) != CKR_OK) ||
            (*((CK_OBJECT_CLASS_PTR)pTemplate[attributeIndex].pValue) != CKO_SECRET_KEY)) {
            return CKR_ARGUMENTS_BAD;
        }
        if ((GetAttributeParameterIndex(pTemplate, ulAttributeCount, CKA_KEY_TYPE, &attributeIndex) != CKR_OK) ||
            (*((CK_KEY_TYPE *)pTemplate[attributeIndex].pValue) != CKK_AES)) {
            return CKR_ARGUMENTS_BAD;
        }
        if ((GetAttributeParameterIndex(pTemplate, ulAttributeCount, CKA_VALUE_LEN, &attributeIndex) != CKR_OK) ||
            ((*((size_t *)pTemplate[attributeIndex].pValue) != 16) &&
                (*((size_t *)pTemplate[attributeIndex].pValue) != 24) &&
                (*((size_t *)pTemplate[attributeIndex].pValue) != 32))) {
            return CKR_ARGUMENTS_BAD;
        }
        derivedKeyLen = *((size_t *)pTemplate[attributeIndex].pValue);
        if (GetAttributeParameterIndex(pTemplate, ulAttributeCount, CKA_LABEL, &attributeIndex) != CKR_OK) {
            /* Label not passed. Create random keyID */
            xResult = LabelToKeyId((unsigned char *)"", 0, &derivedKeyId);
        }
        else {
            xResult =
                LabelToKeyId(pTemplate[attributeIndex].pValue, pTemplate[attributeIndex].ulValueLen, &derivedKeyId);
        }
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);

        LOCK_MUTEX_FOR_RTOS
        {
            sss_status = sss_key_object_init(&privKeyObj, &pex_sss_demo_boot_ctx->ks);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
            sss_status = sss_key_object_get_handle(&privKeyObj, keyId);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
            xResult = read_object_size(keyId, &keySize);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(xResult == CKR_OK);
            // ENSURE_OR_GO_EXIT(xResult == CKR_OK);

            UNLOCK_MUTEX_FOR_RTOS
        }

        if (privKeyObj.cipherType != kSSS_CipherType_EC_NIST_P) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        /* Create complete key */

        tag = ASN_TAG_OBJ_IDF;

        if (keySize == 24) {
            xResult = SetASNTLV(tag,
                (uint8_t *)MBEDTLS_OID_EC_GRP_SECP192R1,
                sizeof(MBEDTLS_OID_EC_GRP_SECP192R1) - 1,
                ecKeyParameters,
                &ecKeyParametersLen);
            ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        }
        else if (keySize == 28) {
            xResult = SetASNTLV(tag,
                (uint8_t *)MBEDTLS_OID_EC_GRP_SECP224R1,
                sizeof(MBEDTLS_OID_EC_GRP_SECP224R1) - 1,
                ecKeyParameters,
                &ecKeyParametersLen);
            ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        }
        else if (keySize == 32) {
            xResult = SetASNTLV(tag,
                (uint8_t *)MBEDTLS_OID_EC_GRP_SECP256R1,
                sizeof(MBEDTLS_OID_EC_GRP_SECP256R1) - 1,
                ecKeyParameters,
                &ecKeyParametersLen);
            ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        }
        else if (keySize == 48) {
            xResult = SetASNTLV(tag,
                (uint8_t *)MBEDTLS_OID_EC_GRP_SECP384R1,
                sizeof(MBEDTLS_OID_EC_GRP_SECP384R1) - 1,
                ecKeyParameters,
                &ecKeyParametersLen);
            ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        }
        else if (keySize == 65 || keySize == 66) {
            xResult = SetASNTLV(tag,
                (uint8_t *)MBEDTLS_OID_EC_GRP_SECP521R1,
                sizeof(MBEDTLS_OID_EC_GRP_SECP521R1) - 1,
                ecKeyParameters,
                &ecKeyParametersLen);
            ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        xResult = SetASNTLV(tag, id_ecPublicKey, sizeof(id_ecPublicKey), ecKeyParameters, &ecKeyParametersLen);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);

        uint8_t ecPoint[70] = {0};
        uint8_t *p_ecPoint  = &ecPoint[0];

        int i = 0;
        if (p_ecdh1Params->pPublicData[i++] != ASN_TAG_OCTETSTRING) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        size_t len = p_ecdh1Params->pPublicData[i++];

        if ((len & 0x80) == 0x80) {
            if ((len & 0x7F) == 0x01) {
                len = p_ecdh1Params->pPublicData[i++];
            }
            else if ((len & 0x7F) == 0x02) {
                len = (p_ecdh1Params->pPublicData[i] << 8) | (p_ecdh1Params->pPublicData[i + 1]);
                i   = i + 2;
            }
            else {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
        }

        if (p_ecdh1Params->pPublicData[i] != 0x00) {
            p_ecPoint++;
            memcpy(p_ecPoint, &p_ecdh1Params->pPublicData[i], len);
            len++;
        }
        else {
            memcpy(p_ecPoint, &p_ecdh1Params->pPublicData[i], len);
        }

        tag     = ASN_TAG_BITSTRING;
        xResult = SetASNTLV(tag, &ecPoint[0], len, publicKeyBuffer, &publicKeyBufferLen);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        tag     = ASN_TAG_SEQUENCE;
        xResult = SetASNTLV(tag,
            &ecKeyParameters[ecKeyParametersLen],
            sizeof(ecKeyParameters) - ecKeyParametersLen,
            publicKeyBuffer,
            &publicKeyBufferLen);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);

        len = sizeof(publicKeyBuffer) - publicKeyBufferLen;

        if (len <= 127) {
            if (publicKeyBufferLen < 1) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            publicKeyBufferLen = publicKeyBufferLen - 1;

            publicKeyBuffer[publicKeyBufferLen] = len;
        }
        else if (len <= 255) {
            if (publicKeyBufferLen < 2) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            publicKeyBufferLen                      = publicKeyBufferLen - 2;
            publicKeyBuffer[publicKeyBufferLen]     = 0x81;
            publicKeyBuffer[publicKeyBufferLen + 1] = len;
        }
        else {
            if (publicKeyBufferLen < 3) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            publicKeyBufferLen                      = publicKeyBufferLen - 3;
            publicKeyBuffer[publicKeyBufferLen]     = 0x82;
            publicKeyBuffer[publicKeyBufferLen + 1] = (len & 0x00FF00) >> 8;
            publicKeyBuffer[publicKeyBufferLen + 2] = (len & 0x00FF);
        }

        if (publicKeyBufferLen < 1) {
            return CKR_ARGUMENTS_BAD;
        }
        publicKeyBufferLen = publicKeyBufferLen - 1;

        publicKeyBuffer[publicKeyBufferLen] = ASN_TAG_SEQUENCE;

        /*
         * key  = &publicKeyBuffer[publicKeyBufferLen]
         * size = sizeof(publicKeyBuffer) - publicKeyBufferLen
         */

        /* Import the public key now */

        uint32_t publicKeyId = 0;
        xResult              = LabelToKeyId((unsigned char *)"", 0, &publicKeyId);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        xResult = CKR_FUNCTION_FAILED;

        LOCK_MUTEX_FOR_RTOS
        {
            sss_status = sss_key_object_init(&pubKeyObj, &pex_sss_demo_boot_ctx->ks);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
            sss_status = sss_key_object_allocate_handle(&pubKeyObj,
                publicKeyId,
                kSSS_KeyPart_Public,
                privKeyObj.cipherType,
                keySize * 8,
                kKeyObject_Mode_Persistent);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
            sss_status = sss_key_store_set_key(&pex_sss_demo_boot_ctx->ks,
                &pubKeyObj,
                &publicKeyBuffer[publicKeyBufferLen],
                sizeof(publicKeyBuffer) - publicKeyBufferLen,
                keySize * 8,
                NULL,
                0);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            UNLOCK_MUTEX_FOR_RTOS
        }

        /* Start process for derivation now */

        LOCK_MUTEX_FOR_RTOS
        {
            sss_status = sss_key_object_init(&derivedObject, &pex_sss_demo_boot_ctx->host_ks);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
            sss_status = sss_key_object_allocate_handle(&derivedObject,
                derivedKeyId,
                kSSS_KeyPart_Default,
                kSSS_CipherType_AES,
                derivedKeyLen * 8,
                kKeyObject_Mode_Persistent);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            sss_status = sss_derive_key_context_init(
                &ctx_derive_key, &pex_sss_demo_boot_ctx->session, &privKeyObj, algorithm, mode);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            sss_status = sss_derive_key_dh(&ctx_derive_key, &pubKeyObj, &derivedObject);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            uint8_t derivedKey[32]  = {0};
            size_t derivedKeySize   = sizeof(derivedKey);
            size_t derivedKeyBitLen = derivedKeyLen * 8;

            sss_status = sss_key_store_get_key(
                &pex_sss_demo_boot_ctx->host_ks, &derivedObject, derivedKey, &derivedKeySize, &derivedKeyBitLen);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            sss_status = sss_key_object_init(&derivedObject, &pex_sss_demo_boot_ctx->ks);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
            sss_status = sss_key_object_allocate_handle(&derivedObject,
                derivedKeyId,
                kSSS_KeyPart_Default,
                kSSS_CipherType_AES,
                derivedKeyLen * 8,
                kKeyObject_Mode_Persistent);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
            sss_status = sss_key_store_set_key(
                &pex_sss_demo_boot_ctx->ks, &derivedObject, derivedKey, derivedKeySize, derivedKeyBitLen, NULL, 0);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            UNLOCK_MUTEX_FOR_RTOS
        }

        xResult = CKR_OK;

        *phKey = derivedKeyId;
    }

exit:
    if (pubKeyObj.keyStore) {
        LOCK_MUTEX_FOR_RTOS
        {
            sss_key_store_erase_key(pubKeyObj.keyStore, &pubKeyObj);
            UNLOCK_MUTEX_FOR_RTOS
        }
    }
#endif
    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_Digest)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);
    CK_ULONG  offset = 0, updateLen = 0,  remainingLen = 0;

    if (!pData) {
        return CKR_ARGUMENTS_BAD;
    }

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    sss_algorithm_t algorithm;
    xResult = ParseDigestMechanism(pxSessionObj, &algorithm);
    if (xResult != CKR_OK) {
        return xResult;
    }
#if SSS_HAVE_ALT_SSS
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t output[64]  = {0};
    size_t outputLen    = sizeof(output);

    LOCK_MUTEX_FOR_RTOS
    {
        if (pxSessionObj->digestUpdateCalled == CK_TRUE) {
            xResult                            = CKR_FUNCTION_FAILED;
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS
            goto cleanup;
        }
        switch (pxSessionObj->xOperationInProgress) {
        case CKM_SHA_1:
            outputLen = 20;
            break;
        case CKM_SHA224:
            outputLen = 28;
            break;
        case CKM_SHA256:
            outputLen = 32;
            break;
        case CKM_SHA384:
            outputLen = 48;
            break;
        case CKM_SHA512:
            outputLen = 64;
            break;
        default:
            xResult = CKR_FUNCTION_FAILED;
            break;
        }
        if (xResult == CKR_FUNCTION_FAILED) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS
            goto cleanup;
        }

        if (pDigest) {
            if (*pulDigestLen < outputLen) {
                xResult = CKR_BUFFER_TOO_SMALL;
            }
            else {
                remainingLen = ( ulDataLen );
                while( remainingLen > 0)
                {
                    updateLen = ( remainingLen > 512) ? 512: remainingLen;
                    status = sss_digest_update(&pxSessionObj->digest_ctx,  (pData + offset), updateLen);
                    if (status != kStatus_SSS_Success) {
                        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                        xResult                            = CKR_DEVICE_ERROR;
                        UNLOCK_MUTEX_FOR_RTOS
                        goto cleanup;
                    }

                    remainingLen = remainingLen - updateLen;
                    offset = offset + updateLen;
                }
                status = sss_digest_finish(&pxSessionObj->digest_ctx, &output[0], &outputLen);
                sss_digest_context_free(&pxSessionObj->digest_ctx);
                if (status != kStatus_SSS_Success) {
                    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                    xResult                            = CKR_DEVICE_ERROR;
                    UNLOCK_MUTEX_FOR_RTOS
                    goto cleanup;
                }
                memcpy(pDigest, &output[0], outputLen);
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            }
        }
        *pulDigestLen = outputLen;

        UNLOCK_MUTEX_FOR_RTOS
    }

cleanup:

#endif

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestKey)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_Encrypt)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    if (!pData) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }

    // sss_status_t status;
    sss_algorithm_t algorithm = kAlgorithm_None;

    xResult = ParseEncryptionMechanism(pxSessionObj, &algorithm);
    // printf("\nEncrypt Mechanism = %08x\n", pxSessionObj->xOperationInProgress);
    if (xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }
    if (algorithm == kAlgorithm_None) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }
    if ((algorithm == kAlgorithm_SSS_RSAES_PKCS1_V1_5) || (algorithm == kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1)) {
        /*RSA Encryption*/
        xResult = AsymmetricEncrypt(pxSessionObj, algorithm, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
    }
    else {
        /*Symmetric Encryption*/
        xResult = SymmetricEncrypt(pxSessionObj, algorithm, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    CK_RV xResult = CKR_OK;
    // printf("\n\n%s\n\n", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(hSession);

    if (NULL == pMechanism) {
        xResult = CKR_ARGUMENTS_BAD;
    }

    else if (pxSession->xOperationInProgress != pkcs11NO_OPERATION) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }

    else {
        pxSession->xOperationInProgress = pMechanism->mechanism;
        pxSession->xOperationKeyHandle  = hKey;
        if (pMechanism->pParameter) {
            pxSession->mechParameter    = pMechanism->pParameter;
            pxSession->mechParameterLen = pMechanism->ulParameterLen;
        }
        else {
            pxSession->mechParameterLen = 0;
        }
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_EncryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG_PTR pulEncryptedPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GenerateKey)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulCount,
    CK_OBJECT_HANDLE_PTR phKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);
    /*
        Attribute.CLASS: ObjectClass.SECRET_KEY,
        Attribute.ID: id or b'',
        Attribute.LABEL: label or '',
        Attribute.TOKEN: store,
        Attribute.PRIVATE: True,
        Attribute.SENSITIVE: True,
        Attribute.ENCRYPT: MechanismFlag.ENCRYPT & capabilities,
        Attribute.DECRYPT: MechanismFlag.DECRYPT & capabilities,
        Attribute.WRAP: MechanismFlag.WRAP & capabilities,
        Attribute.UNWRAP: MechanismFlag.UNWRAP & capabilities,
        Attribute.SIGN: MechanismFlag.SIGN & capabilities,
        Attribute.VERIFY: MechanismFlag.VERIFY & capabilities,
        Attribute.DERIVE: MechanismFlag.DERIVE & capabilities,
        template_[Attribute.VALUE_LEN] = key_length // 8  # In bytes
    */
    CK_RV xResult = CKR_FUNCTION_NOT_SUPPORTED;
#if SSS_HAVE_ALT_SSS
    sss_status_t sss_status = kStatus_SSS_Fail;
    sss_rng_context_t sss_rng_ctx;
    uint32_t keyId               = 0x0;
    size_t keyLen                = 0;
    sss_cipher_type_t cipherType = kSSS_CipherType_NONE;
    CK_ULONG attributeIndex      = 0;
    CK_OBJECT_CLASS ck_object    = pkcs11INVALID_OBJECT_CLASS;
    // LOG_I("Mechanism = %zu", *pMechanism);
    CK_MECHANISM mech = *pMechanism;

    xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_CLASS, &attributeIndex);
    ENSURE_OR_GO_EXIT(xResult == CKR_OK);

    /* Ensure that CK_OBJECT_CLASS is CKO_SECRET_KEY */
    ck_object = *((CK_OBJECT_CLASS_PTR)pTemplate[attributeIndex].pValue);
    ENSURE_OR_GO_EXIT(ck_object == CKO_SECRET_KEY);

    if (mech.mechanism == CKM_AES_KEY_GEN) {
        xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_VALUE_LEN, &attributeIndex);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);
        keyLen = *((size_t *)pTemplate[attributeIndex].pValue);
        if ((keyLen != 16) && (keyLen != 24) && (keyLen != 32)) {
            LOG_E("Unsupported key length %d", keyLen);
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        cipherType = kSSS_CipherType_AES;
    }
    else if (mech.mechanism == CKM_DES2_KEY_GEN) {
        keyLen     = 16; /* Fixed length for DES key */
        cipherType = kSSS_CipherType_DES;
    }
    else if (mech.mechanism == CKM_DES3_KEY_GEN) {
        keyLen     = 24; /* Fixed length for DES key */
        cipherType = kSSS_CipherType_DES;
    }

    xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_LABEL, &attributeIndex);
    if (xResult != CKR_OK) {
        /* CKA_LABEL was not provided. Generate a random keyId */
        xResult = LabelToKeyId((unsigned char *)"", 0, &keyId);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);
    }
    else {
        xResult = LabelToKeyId(pTemplate[attributeIndex].pValue, pTemplate[attributeIndex].ulValueLen, &keyId);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);
    }

    xResult = CKR_DEVICE_ERROR;
    /* Generate random data */
    uint8_t randomKey[64] = {0};
    LOCK_MUTEX_FOR_RTOS
    {
        sss_status = sss_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->session);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS
            goto exit;
        }
        sss_status = sss_rng_get_random(&sss_rng_ctx, randomKey, keyLen);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS
            goto exit;
        }
        UNLOCK_MUTEX_FOR_RTOS
    }

    /* Import secret key */
    sss_object_t sss_object = {0};
    sss_status              = sss_create_token(&pex_sss_demo_boot_ctx->ks,
        &sss_object,
        keyId,
        kSSS_KeyPart_Default,
        cipherType,
        randomKey,
        keyLen,
        keyLen * 8);
    if (sss_status == kStatus_SSS_Success) {
        xResult = CKR_OK;
        *phKey  = keyId;
        LOG_I("Generated KeyID = %d", keyId);
    }

exit:
#endif
    return xResult;
    // return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GenerateKeyPair)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_ATTRIBUTE_PTR pPublicKeyTemplate,
    CK_ULONG ulPublicKeyAttributeCount,
    CK_ATTRIBUTE_PTR pPrivateKeyTemplate,
    CK_ULONG ulPrivateKeyAttributeCount,
    CK_OBJECT_HANDLE_PTR phPublicKey,
    CK_OBJECT_HANDLE_PTR phPrivateKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);
    CK_RV xResult              = CKR_OK;
    P11SessionPtr_t pxSession  = prvSessionPointerFromHandle(hSession);
    size_t KeyBitLen           = 0;
    CK_ULONG privateLabelIndex = 0;
    CK_ULONG publicLabelIndex  = 0;
    uint32_t privKeyId         = 0;
    uint32_t pubKeyId          = 0;
    sss_status_t sss_status    = kStatus_SSS_Fail;
    sss_object_t sss_object    = {0};
    CK_BBOOL skipPublicKey     = CK_FALSE;
    CK_BBOOL privLabelExists   = CK_FALSE;

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

    if (!pMechanism) {
        return CKR_ARGUMENTS_BAD;
    }

    sss_cipher_type_t cipherType = kSSS_CipherType_Binary;
    switch (pMechanism->mechanism) {
    // case CKM_ECDSA_KEY_PAIR_GEN:
    case CKM_EC_KEY_PAIR_GEN:
        cipherType = kSSS_CipherType_EC_NIST_P;
        break;
    case CKM_RSA_PKCS_KEY_PAIR_GEN:
        cipherType = kSSS_CipherType_RSA;
        break;
    default:
        return CKR_MECHANISM_INVALID;
    }
    if (cipherType == kSSS_CipherType_EC_NIST_P) {
        CK_ULONG ec_params_index = 0;
        xResult =
            GetAttributeParameterIndex(pPublicKeyTemplate, ulPublicKeyAttributeCount, CKA_EC_PARAMS, &ec_params_index);
        if (xResult != CKR_OK) {
            return xResult;
        }

        uint8_t ec_params[40] = {""};
        memcpy(ec_params, pPublicKeyTemplate[ec_params_index].pValue, pPublicKeyTemplate[ec_params_index].ulValueLen);

        uint8_t tag     = ASN_TAG_OBJ_IDF;
        uint8_t oid[20] = {0};
        size_t oidLen   = sizeof(oid);
        xResult         = SetASNTLV(
            tag, (uint8_t *)MBEDTLS_OID_EC_GRP_SECP192R1, sizeof(MBEDTLS_OID_EC_GRP_SECP192R1) - 1, oid, &oidLen);
        if (xResult != CKR_OK) {
            return xResult;
        }

        if (sizeof(oid) > oidLen) {
            if (memcmp(&oid[oidLen], &ec_params[0], sizeof(oid) - oidLen) == 0) {
                KeyBitLen = 192;
                goto cont;
            }
        }

        oidLen = sizeof(oid);

        xResult = SetASNTLV(
            tag, (uint8_t *)MBEDTLS_OID_EC_GRP_SECP224R1, sizeof(MBEDTLS_OID_EC_GRP_SECP224R1) - 1, oid, &oidLen);
        if (xResult != CKR_OK) {
            return xResult;
        }

        if (sizeof(oid) > oidLen) {
            if (memcmp(&oid[oidLen], &ec_params[0], sizeof(oid) - oidLen) == 0) {
                KeyBitLen = 224;
                goto cont;
            }
        }
        oidLen = sizeof(oid);

        xResult = SetASNTLV(
            tag, (uint8_t *)MBEDTLS_OID_EC_GRP_SECP256R1, sizeof(MBEDTLS_OID_EC_GRP_SECP256R1) - 1, oid, &oidLen);
        if (xResult != CKR_OK) {
            return xResult;
        }

        if (sizeof(oid) > oidLen) {
            if (memcmp(&oid[oidLen], &ec_params[0], sizeof(oid) - oidLen) == 0) {
                KeyBitLen = 256;
                goto cont;
            }
        }

        oidLen = sizeof(oid);

        xResult = SetASNTLV(
            tag, (uint8_t *)MBEDTLS_OID_EC_GRP_SECP384R1, sizeof(MBEDTLS_OID_EC_GRP_SECP384R1) - 1, oid, &oidLen);
        if (xResult != CKR_OK) {
            return xResult;
        }

        if (sizeof(oid) > oidLen) {
            if (memcmp(&oid[oidLen], &ec_params[0], sizeof(oid) - oidLen) == 0) {
                KeyBitLen = 384;
                goto cont;
            }
        }
        oidLen = sizeof(oid);

        xResult = SetASNTLV(
            tag, (uint8_t *)MBEDTLS_OID_EC_GRP_SECP521R1, sizeof(MBEDTLS_OID_EC_GRP_SECP521R1) - 1, oid, &oidLen);
        if (xResult != CKR_OK) {
            return xResult;
        }

        if (sizeof(oid) > oidLen) {
            if (memcmp(&oid[oidLen], &ec_params[0], sizeof(oid) - oidLen) == 0) {
                KeyBitLen = 521;
                goto cont;
            }
        }

        return CKR_ARGUMENTS_BAD;
    }
    else if (cipherType == kSSS_CipherType_RSA) {
        CK_ULONG rsa_params_index = 0;
        xResult                   = GetAttributeParameterIndex(
            pPublicKeyTemplate, ulPublicKeyAttributeCount, CKA_MODULUS_BITS, &rsa_params_index);
        if (xResult != CKR_OK) {
            return xResult;
        }

        CK_ULONG modulusBits = 0;
        memcpy(
            &modulusBits, pPublicKeyTemplate[rsa_params_index].pValue, pPublicKeyTemplate[rsa_params_index].ulValueLen);

        KeyBitLen = (size_t)modulusBits;

        if ((KeyBitLen != 1024) && (KeyBitLen != 2048) && (KeyBitLen != 3072) && (KeyBitLen != 4096)) {
            return CKR_ARGUMENTS_BAD;
        }
    }

cont:

    xResult =
        GetAttributeParameterIndex(pPrivateKeyTemplate, ulPrivateKeyAttributeCount, CKA_LABEL, &privateLabelIndex);
    if (xResult != CKR_OK) {
        /* CKA_LABEL was not provided. Generate a random keyId */
        xResult = LabelToKeyId((unsigned char *)"", 0, &privKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }
    else {
        privLabelExists = CK_TRUE;
        xResult         = LabelToKeyId(pPrivateKeyTemplate[privateLabelIndex].pValue,
            pPrivateKeyTemplate[privateLabelIndex].ulValueLen,
            &privKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }

    xResult = GetAttributeParameterIndex(pPublicKeyTemplate, ulPublicKeyAttributeCount, CKA_LABEL, &publicLabelIndex);
    if (xResult != CKR_OK) {
        /* CKA_LABEL was not provided. Generate a random keyId */
        xResult = LabelToKeyId((unsigned char *)"", 0, &pubKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }
    else {
        if (privLabelExists) {
            if (pPrivateKeyTemplate[privateLabelIndex].ulValueLen == pPublicKeyTemplate[publicLabelIndex].ulValueLen &&
                pPrivateKeyTemplate[privateLabelIndex].ulValueLen != 0) {
                if (strncmp((const char *)pPrivateKeyTemplate[privateLabelIndex].pValue,
                        (const char *)pPublicKeyTemplate[publicLabelIndex].pValue,
                        (size_t)pPrivateKeyTemplate[privateLabelIndex].ulValueLen) == 0) {
                    skipPublicKey = CK_TRUE;
                }
            }
        }
        xResult = LabelToKeyId(
            pPublicKeyTemplate[publicLabelIndex].pValue, pPublicKeyTemplate[publicLabelIndex].ulValueLen, &pubKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }

    LOCK_MUTEX_FOR_RTOS
    {
        sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        sss_status = sss_key_object_allocate_handle(
            &sss_object, privKeyId, kSSS_KeyPart_Pair, cipherType, KeyBitLen * 8, kKeyObject_Mode_Persistent);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        sss_status = sss_key_store_generate_key(&pex_sss_demo_boot_ctx->ks, &sss_object, KeyBitLen, NULL);
        if (sss_status != kStatus_SSS_Success) {
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
        }

        if (!skipPublicKey) {
            uint8_t public[2048] = {0};
            size_t public_len    = sizeof(public);

            sss_status =
                sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, public, &public_len, &KeyBitLen);
            if (sss_status != kStatus_SSS_Success) {
                sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            }

            sss_object_t sss_pub_object = {0};

            sss_status = sss_key_object_init(&sss_pub_object, &pex_sss_demo_boot_ctx->ks);
            if (sss_status != kStatus_SSS_Success) {
                sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            }

            sss_status = sss_key_object_allocate_handle(
                &sss_pub_object, pubKeyId, kSSS_KeyPart_Public, cipherType, KeyBitLen * 8, kKeyObject_Mode_Persistent);
            if (sss_status != kStatus_SSS_Success) {
                sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            }

            sss_status = sss_key_store_set_key(
                &pex_sss_demo_boot_ctx->ks, &sss_pub_object, public, public_len, KeyBitLen, NULL, 0);
            if (sss_status != kStatus_SSS_Success) {
                sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
                UNLOCK_MUTEX_FOR_RTOS_RET(CKR_FUNCTION_FAILED)
            }
        }
        else {
            pubKeyId = privKeyId;
        }

        UNLOCK_MUTEX_FOR_RTOS
    }

    *phPublicKey  = pubKeyId;
    *phPrivateKey = privKeyId;
    return CKR_OK;

    // return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetInfo)(CK_INFO_PTR pInfo)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

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
    if (!pInfo) {
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

CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismInfo)
(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo)
{
    // printf("\n\n%s\n\n", __FUNCTION__);
    // /* CK_MECHANISM_INFO provides information about a particular
    //  * mechanism */
    // typedef struct CK_MECHANISM_INFO {
    //     CK_ULONG    ulMinKeySize;
    //     CK_ULONG    ulMaxKeySize;
    //     CK_FLAGS    flags;
    // } CK_MECHANISM_INFO;

    // /* The flags are defined as follows:
    //  *      Bit Flag               Mask          Meaning */
    // #define CKF_HW                 0x00000001UL  /* performed by HW */

    // /* Specify whether or not a mechanism can be used for a particular task */
    // #define CKF_ENCRYPT            0x00000100UL
    // #define CKF_DECRYPT            0x00000200UL
    // #define CKF_DIGEST             0x00000400UL
    // #define CKF_SIGN               0x00000800UL
    //// #define CKF_SIGN_RECOVER       0x00001000UL
    // #define CKF_VERIFY             0x00002000UL
    //// #define CKF_VERIFY_RECOVER     0x00004000UL
    // #define CKF_GENERATE           0x00008000UL
    // #define CKF_GENERATE_KEY_PAIR  0x00010000UL
    //// #define CKF_WRAP               0x00020000UL
    //// #define CKF_UNWRAP             0x00040000UL
    //// #define CKF_DERIVE             0x00080000UL

    CK_RV xResult = CKR_MECHANISM_INVALID;

    CK_MECHANISM_INFO mech_info = {.ulMinKeySize = 0, .ulMaxKeySize = 0, .flags = CKF_HW};
    if (type == CKM_RSA_PKCS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA1_RSA_PKCS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA224_RSA_PKCS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA256_RSA_PKCS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA384_RSA_PKCS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA512_RSA_PKCS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_RSA_PKCS_PSS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA1_RSA_PKCS_PSS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA224_RSA_PKCS_PSS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA256_RSA_PKCS_PSS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA384_RSA_PKCS_PSS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA512_RSA_PKCS_PSS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_RSA_PKCS_OAEP) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_ENCRYPT | CKF_DECRYPT;
        xResult                = CKR_OK;
    }
    else if (type == CKM_AES_ECB) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_ENCRYPT | CKF_DECRYPT;
        xResult                = CKR_OK;
    }
    else if (type == CKM_AES_CBC) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_ENCRYPT | CKF_DECRYPT;
        xResult                = CKR_OK;
    }
    else if (type == CKM_AES_CTR) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_ENCRYPT | CKF_DECRYPT;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA_1) {
        mech_info.ulMinKeySize = 0;
        mech_info.ulMaxKeySize = 0;
        mech_info.flags        = mech_info.flags | CKF_DIGEST;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA224) {
        mech_info.ulMinKeySize = 0;
        mech_info.ulMaxKeySize = 0;
        mech_info.flags        = mech_info.flags | CKF_DIGEST;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA256) {
        mech_info.ulMinKeySize = 0;
        mech_info.ulMaxKeySize = 0;
        mech_info.flags        = mech_info.flags | CKF_DIGEST;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA384) {
        mech_info.ulMinKeySize = 0;
        mech_info.ulMaxKeySize = 0;
        mech_info.flags        = mech_info.flags | CKF_DIGEST;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA512) {
        mech_info.ulMinKeySize = 0;
        mech_info.ulMaxKeySize = 0;
        mech_info.flags        = mech_info.flags | CKF_DIGEST;
        xResult                = CKR_OK;
    }
    else if (type == CKM_ECDSA) {
        mech_info.ulMinKeySize = 192;
        mech_info.ulMaxKeySize = 521;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_ECDSA_SHA1) {
        mech_info.ulMinKeySize = 192;
        mech_info.ulMaxKeySize = 521;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_EC_KEY_PAIR_GEN) {
        mech_info.ulMinKeySize = 192;
        mech_info.ulMaxKeySize = 521;
        mech_info.flags        = mech_info.flags | CKF_GENERATE_KEY_PAIR | CKF_EC_NAMEDCURVE;
        xResult                = CKR_OK;
    }
    else if (type == CKM_RSA_PKCS_KEY_PAIR_GEN) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_GENERATE_KEY_PAIR;
        xResult                = CKR_OK;
    }
    else if (type == CKM_AES_KEY_GEN) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_GENERATE;
        xResult                = CKR_OK;
    }
    else if (type == CKM_DES2_KEY_GEN) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 128;
        mech_info.flags        = mech_info.flags | CKF_GENERATE;
        xResult                = CKR_OK;
    }
    else if (type == CKM_ECDH1_DERIVE) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_DERIVE;
        xResult                = CKR_OK;
    }
    else if (type == CKM_RSA_X_509) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_VERIFY;
        xResult                = CKR_OK;
    }

    if (xResult == CKR_OK) {
        memcpy(pInfo, &mech_info, sizeof(CK_MECHANISM_INFO));
    }

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismList)
(CK_SLOT_ID slotID, CK_MECHANISM_TYPE_PTR pMechanismList, CK_ULONG_PTR pulCount)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    CK_RV xResult = CKR_OK;

    if (!pulCount) {
        return CKR_ARGUMENTS_BAD;
    }

    CK_MECHANISM_TYPE mechanisms[] = {/* RSA Algorithms */
        CKM_RSA_PKCS,
        CKM_SHA1_RSA_PKCS,
        CKM_SHA224_RSA_PKCS,
        CKM_SHA256_RSA_PKCS,
        CKM_SHA384_RSA_PKCS,
        CKM_SHA512_RSA_PKCS,
        CKM_RSA_PKCS_PSS,
        CKM_SHA1_RSA_PKCS_PSS,
        CKM_SHA224_RSA_PKCS_PSS,
        CKM_SHA256_RSA_PKCS_PSS,
        CKM_SHA384_RSA_PKCS_PSS,
        CKM_SHA512_RSA_PKCS_PSS,
        CKM_RSA_PKCS_OAEP,
        /* AES Algorithms  */
        CKM_AES_ECB,
        CKM_AES_CBC,
        CKM_AES_CTR,
        /* Digest algorithms */
        CKM_SHA_1,
        CKM_SHA224,
        CKM_SHA256,
        CKM_SHA384,
        CKM_SHA512,
        /* ECDSA */
        CKM_ECDSA,
        CKM_ECDSA_SHA1,
        /* Key Generation algorithms */
        CKM_EC_KEY_PAIR_GEN,
        CKM_RSA_PKCS_KEY_PAIR_GEN,
        CKM_AES_KEY_GEN,
        CKM_DES2_KEY_GEN,
        CKM_DES3_KEY_GEN,
        /* Key Derivation algorithms */
        CKM_ECDH1_DERIVE};
    if (pMechanismList) {
        if (*pulCount < sizeof(mechanisms)) {
            xResult = CKR_BUFFER_TOO_SMALL;
        }
        else {
            memcpy(pMechanismList, &mechanisms[0], sizeof(mechanisms));
        }
    }
    *pulCount = sizeof(mechanisms);
    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetSlotInfo)
(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    // typedef struct CK_SLOT_INFO {
    //   /* slotDescription and manufacturerID have been changed from
    //    * CK_CHAR to CK_UTF8CHAR for v2.10 */
    //   CK_UTF8CHAR   slotDescription[64];  /* blank padded */
    //   CK_UTF8CHAR   manufacturerID[32];   /* blank padded */
    //   CK_FLAGS      flags;

    //   CK_VERSION    hardwareVersion;  /* version of hardware */
    //   CK_VERSION    firmwareVersion;  /* version of firmware */
    // } CK_SLOT_INFO;
    if (slotID != 1) {
        return CKR_SLOT_ID_INVALID;
    }
    memset(&pInfo->slotDescription[0], ' ', sizeof(pInfo->slotDescription));
    memset(&pInfo->manufacturerID[0], ' ', sizeof(pInfo->manufacturerID));
    pInfo->flags = CKF_TOKEN_PRESENT | CKF_REMOVABLE_DEVICE | CKF_HW_SLOT;

#if SSS_HAVE_APPLET_SE05X_IOT
    pInfo->hardwareVersion.major = APPLET_SE050_VER_MAJOR;
    pInfo->hardwareVersion.minor = APPLET_SE050_VER_MINOR;
#endif

    CK_VERSION libVersion = PKCS11_LIBRARY_VERSION;
    memcpy(&pInfo->firmwareVersion, &libVersion, sizeof(CK_VERSION));

    memcpy(&pInfo->manufacturerID, "NXP", sizeof("NXP"));
    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetTokenInfo)
(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

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

    CK_TOKEN_INFO tokenInfo      = {0};
    unsigned char label[]        = PKCS11_TOKEN_LABEL;
    unsigned char manufacturer[] = PKCS11_MANUFACTURER;
    memset(tokenInfo.label, ' ', sizeof(tokenInfo.label));
    memset(tokenInfo.manufacturerID, ' ', sizeof(tokenInfo.manufacturerID));
    memset(tokenInfo.model, ' ', sizeof(tokenInfo.model));
    memset(tokenInfo.serialNumber, ' ', sizeof(tokenInfo.serialNumber));
    memcpy(tokenInfo.label, label, sizeof(label));
    memcpy(tokenInfo.manufacturerID, manufacturer, sizeof(manufacturer));
    tokenInfo.ulMaxSessionCount   = 1;
    tokenInfo.ulMaxRwSessionCount = 1;
    tokenInfo.ulMaxPinLen         = 10;
    tokenInfo.ulMinPinLen         = 0;

#if SSS_HAVE_APPLET_SE05X_IOT
    tokenInfo.hardwareVersion.major = APPLET_SE050_VER_MAJOR;
    tokenInfo.hardwareVersion.minor = APPLET_SE050_VER_MINOR;
#endif

    CK_VERSION libVersion = PKCS11_LIBRARY_VERSION;
    memcpy(&tokenInfo.firmwareVersion, &libVersion, sizeof(CK_VERSION));

    tokenInfo.flags = CKF_RNG | CKF_TOKEN_INITIALIZED;

    memcpy(pInfo, &tokenInfo, sizeof(CK_TOKEN_INFO));

    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_Login)
(CK_SESSION_HANDLE hSession, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    // /* Security Officer */
    // #define CKU_SO          0UL
    // /* Normal user */
    // #define CKU_USER        1UL
    // /* Context specific */
    // #define CKU_CONTEXT_SPECIFIC    2UL
    // return CKR_GENERAL_ERROR;

    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_Logout)(CK_SESSION_HANDLE hSession)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_SeedRandom)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_OK;
}

CK_DEFINE_FUNCTION(CK_RV, C_SetAttributeValue)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignUpdate)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_UnwrapKey)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_OBJECT_HANDLE hUnwrappingKey,
    CK_BYTE_PTR pWrappedKey,
    CK_ULONG ulWrappedKeyLen,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulAttributeCount,
    CK_OBJECT_HANDLE_PTR phKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyUpdate)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_WrapKey)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_OBJECT_HANDLE hWrappingKey,
    CK_OBJECT_HANDLE hKey,
    CK_BYTE_PTR pWrappedKey,
    CK_ULONG_PTR pulWrappedKeyLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_InitToken)
(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_InitPIN)
(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SetPIN)
(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_CloseAllSessions)(CK_SLOT_ID slotID)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetSessionInfo)
(CK_SESSION_HANDLE hSession, CK_SESSION_INFO_PTR pInfo)
{
    // printf("\n\n%s\n\n", __FUNCTION__);
    CK_RV xResult = CKR_SESSION_CLOSED;
#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_lock(&gSessionlock);
#endif
    LOCK_MUTEX_FOR_RTOS
    {
        CK_SESSION_INFO session_info = {
            .slotID = pkcs11SLOT_ID, .state = CKS_RW_PUBLIC_SESSION, .flags = CKF_RW_SESSION, .ulDeviceError = 0};

#if defined(USE_RTOS) && USE_RTOS == 1
#elif (__GNUC__ && !AX_EMBEDDED)
#else
        session_info.flags = session_info.flags | CKF_SERIAL_SESSION;
#endif

        if (sessionCount) {
            memcpy(pInfo, &session_info, sizeof(CK_SESSION_INFO));
            xResult = CKR_OK;
        }

        UNLOCK_MUTEX_FOR_RTOS
    }

#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_unlock(&gSessionlock);
#endif

    return xResult;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetOperationState)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pOperationState, CK_ULONG_PTR pulOperationStateLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SetOperationState)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pOperationState,
    CK_ULONG ulOperationStateLen,
    CK_OBJECT_HANDLE hEncryptionKey,
    CK_OBJECT_HANDLE hAuthenticationKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetObjectSize)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignRecoverInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignRecover)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pSignature,
    CK_ULONG_PTR pulSignatureLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecoverInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecover)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pSignature,
    CK_ULONG ulSignatureLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DigestEncryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG_PTR pulEncryptedPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptDigestUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG ulEncryptedPartLen,
    CK_BYTE_PTR pPart,
    CK_ULONG_PTR pulPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_SignEncryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG_PTR pulEncryptedPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_DecryptVerifyUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG ulEncryptedPartLen,
    CK_BYTE_PTR pPart,
    CK_ULONG_PTR pulPartLen)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionStatus)(CK_SESSION_HANDLE hSession)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_PARALLEL;
}

CK_DEFINE_FUNCTION(CK_RV, C_CancelFunction)(CK_SESSION_HANDLE hSession)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_PARALLEL;
}

CK_DEFINE_FUNCTION(CK_RV, C_WaitForSlotEvent)
(CK_FLAGS flags, CK_SLOT_ID_PTR pSlot, CK_VOID_PTR pReserved)
{
    // printf("\n\n%s\n\n", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
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
CK_OBJECT_HANDLE PKCS11_PAL_SaveObject(CK_ATTRIBUTE_PTR pxLabel, CK_BYTE_PTR pucData, CK_ULONG ulDataSize)
{
    /*Function to be implemented if required*/
    return 0;
}

/*-----------------------------------------------------------*/

/**
 * @brief Translates a PKCS #11 label into an object handle.
 *
 * Port-specific object handle retrieval.
 *
 *
 * @param[in] pxLabel         Pointer to the label of the object
 *                           who's handle should be found.
 * @param[in] usLength       The length of the label, in bytes.
 *
 * @return The object handle if operation was successful.
 * Returns eInvalidHandle if unsuccessful.
 */
CK_OBJECT_HANDLE PKCS11_PAL_FindObject(CK_BYTE_PTR pxLabel, CK_ULONG usLength)
{
    /*Function to be implemented if required*/
    return 0;
}

CK_RV PKCS11_PAL_Initialize(void)
{
    /*Function to be implemented if required*/
    return 0;
}

/**
* @brief Cleanup after PKCS11_GetObjectValue().
*
* @param[in] pucData       The buffer to free.
*                          (*ppucData from PKCS11_PAL_GetObjectValue())
* @param[in] ulDataSize    The length of the buffer to free.
*                          (*pulDataSize from PKCS11_PAL_GetObjectValue())
*/
void PKCS11_PAL_GetObjectValueCleanup(uint8_t *pucData, uint32_t ulDataSize)
{
}

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
CK_RV PKCS11_PAL_GetObjectValue(
    CK_OBJECT_HANDLE xHandle, uint8_t **ppucData, uint32_t *pulDataSize, CK_BBOOL *pIsPrivate)
{
    /*Function to be implemented if required*/
    CK_RV xReturn = CKR_OK;
    return xReturn;
}

#endif /* TGT_A71CH */

