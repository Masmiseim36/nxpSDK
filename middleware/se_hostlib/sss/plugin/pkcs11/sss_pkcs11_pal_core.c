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

#include "sss_pkcs11_pal.h"
#if (SSS_HAVE_SSCP || SSS_HAVE_APPLET_SE05X_IOT)

/* Remove this later */
extern char *g_port_name;

#undef DEBUG_PKCS11_PAL

#define pkcs11SLOT_ID 1

#define DEFAULT_POLICY_BIN_COUNT_PCR (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE | POLICY_OBJ_ALLOW_READ)
#define DEFAULT_POLICY_USERID (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE)

static int sessionCount         = 0;
static bool cryptokiInitialized = false;
static bool mutex_initialised   = false;

#if defined(USE_RTOS) && USE_RTOS == 1
SemaphoreHandle_t xSemaphore;
#endif

// #if (__GNUC__  && !AX_EMBEDDED)
// #endif

/*-----------------------------------------------------------*/
// static void prvFreeKey( P11KeyPtr_t pxKey );
// static int awsRetreieve_certificate(unsigned char *pbuf, uint8_t index, uint32_t *outbuflen);
// static int convertdertopemformat(const unsigned char * int_ca_cert_der, int len_der, unsigned char * int_ca_cert_pem,
// int dst_buf_len, U16 *outlength);

#if SSS_HAVE_MBEDTLS_ALT
static mbedtls_ecp_group_id curve_list[16] = {
    0,
};
#endif // SSS_HAVE_MBEDTLS_ALT

#if SSS_HAVE_APPLET_SE05X_IOT
static CK_RV read_object_size_with_lock(uint32_t keyId, uint16_t *keyLen);
static uint8_t CheckIfKeyIdExists(uint32_t keyId, pSe05xSession_t session_ctx);
#endif

static int convertpemtoder(const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen);

static P11SessionPtr_t pkcs11_sessions[MAX_PKCS11_SESSIONS] = {0};

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

#if SSS_HAVE_MBEDTLS_ALT
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

#endif // SSS_HAVE_MBEDTLS_ALT

/**
 * @brief Maps an opaque caller session handle into its internal state structure.
 */
P11SessionPtr_t prvSessionPointerFromHandle(CK_SESSION_HANDLE xSession)
{
    if (xSession == 0) {
        return NULL;
    }
    if (xSession > MAX_PKCS11_SESSIONS) {
        return NULL;
    }
    return pkcs11_sessions[xSession - 1];
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

/**
 * @brief Finishes a multiple-part digesting operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestFinal)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

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
#if SSS_HAVE_APPLET
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

/**
 * @brief Continues digesting operation in multiple parts.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestUpdate)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    else if (pxSession->xOperationInProgress == pkcs11NO_OPERATION) {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
    }

    if (xResult == CKR_OK) {
        pxSession->digestUpdateCalled = CK_TRUE;
#if SSS_HAVE_APPLET
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

/**
 * @brief initializes a message-digesting operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pMechanism)
{
    CK_RV xResult = CKR_OK;
    // return CKR_DEVICE_ERROR;
    LOG_D("%s", __FUNCTION__);

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

#if SSS_HAVE_APPLET
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
    LOG_D("%s", __FUNCTION__);

    /*lint !e9072 It's OK to have different parameter name. */
    // return CKR_DEVICE_ERROR;
#if SSS_HAVE_APPLET
    (void)(xSession);
#else
    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(xSession);

    if (pxSessionObj == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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
#if SSS_HAVE_APPLET
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
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(xSession);

    if (pxSessionObj == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

#if SSS_HAVE_SSCP
    (void)(pxSessionObj);
    U8 index = 0;
    HLSE_OBJECT_HANDLE handles[5];
    HLSE_MECHANISM_INFO mechInfo;
    U16 handleNum = 3;
    U16 err;
#endif

#if SSS_HAVE_APPLET_SE05X_IOT
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
#if SSS_HAVE_SSCP
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

#if SSS_HAVE_APPLET_SE05X_IOT

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

/**
 * @brief To do sign operation in single shot.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Sign)
(CK_SESSION_HANDLE xSession,
    CK_BYTE_PTR pucData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pucSignature,
    CK_ULONG_PTR pulSignatureLen)
{
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(xSession);

    if (pxSessionObj == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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
            // Check input size
            if (ulDataLen != 20) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* Note:PKCS11 Spec states that operation should be terminated in this case.
                   AWS PKCS11 testbench expects operation not terminated.
                 */
                return CKR_DATA_LEN_RANGE;
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA224)) {
            // Check input size
            if (ulDataLen != 28) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* Note:PKCS11 Spec states that operation should be terminated in this case.
                   AWS PKCS11 testbench expects operation not terminated.
                 */
                return CKR_DATA_LEN_RANGE;
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA256)) {
            // Check input size
            if (ulDataLen != 32) {
                if (algorithm == kAlgorithm_SSS_ECDSA_SHA256 && ulDataLen < 20) {
                    /* Nothing to do */
                }
                else {
                    // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                    /* Note:PKCS11 Spec states that operation should be terminated in this case.
                       AWS PKCS11 testbench expects operation not terminated.
                     */
                    return CKR_DATA_LEN_RANGE;
                }
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA384)) {
            // Check input size
            if (ulDataLen != 48) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* Note:PKCS11 Spec states that operation should be terminated in this case.
                   AWS PKCS11 testbench expects operation not terminated.
                 */
                return CKR_DATA_LEN_RANGE;
            }
        }
        else if ((algorithm == kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512) ||
                 /* (algorithm == kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512) ||*/
                 (algorithm == kAlgorithm_SSS_ECDSA_SHA512)) {
            // Check input size
            if (ulDataLen != 64) {
                // pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                /* Note:PKCS11 Spec states that operation should be terminated in this case.
                   AWS PKCS11 testbench expects operation not terminated.
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

#if SSS_HAVE_SSCP

U16 HLSE_Create_token(
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

sss_status_t sss_create_token(sss_key_store_t *keystore,
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
        // Data was in PEM format. Nothing to be done.
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

/**
 * @brief Query the list of interface function pointers.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionList)
(CK_FUNCTION_LIST_PTR_PTR ppxFunctionList)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

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
    LOG_D("%s", __FUNCTION__);

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
                // If mutex pointers are not null, library can use either OS locking or provided functions
                // return CKR_GENERAL_ERROR;
            }
            else if (!(initArgs->CreateMutex) && !(initArgs->DestroyMutex) && !(initArgs->LockMutex) &&
                     !(initArgs->UnlockMutex)) {
                // If mutex pointers are null, library must use OS locking.
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
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief Start a session for a cryptographic command sequence.
 */
CK_DEFINE_FUNCTION(CK_RV, C_OpenSession)
(CK_SLOT_ID xSlotID, CK_FLAGS xFlags, CK_VOID_PTR pvApplication, CK_NOTIFY xNotify, CK_SESSION_HANDLE_PTR pxSession)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult                = CKR_OK;
    P11SessionPtr_t pxSessionObj = NULL;
    bool foundEmptySessionSlot   = false;
    size_t i                     = 0;
    LOG_D("%s", __FUNCTION__);

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
    }

    if (NULL != pxSessionObj) {
        if (CKR_OK != xResult) {
            SSS_FREE(pxSessionObj);
            return CKR_FUNCTION_FAILED;
        }
        for (i = 0; i < MAX_PKCS11_SESSIONS; i++) {
            if (pkcs11_sessions[i] == NULL) {
                foundEmptySessionSlot = true;
                break;
            }
        }
        if (foundEmptySessionSlot == true) {
            pkcs11_sessions[i] = pxSessionObj;
            *pxSession         = (CK_SESSION_HANDLE)(i + 1); // To skip session_id 0
        }
        else {
            SSS_FREE(pxSessionObj);
            return CKR_DEVICE_MEMORY;
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

#ifdef PKCS11_SESSION_OPEN
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
#endif
    if (xResult == CKR_OK) {
        sessionCount++;
    }
/*Unlock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_unlock(&gSessionlock);
    LOG_D("Destroyed mutex");
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
    LOG_D("%s", __FUNCTION__);

    if (NULL != pxSession) {
        /*
         * Tear down the session.
         */
#if defined(USE_RTOS) && USE_RTOS == 1
        vPortFree(pxSession);
#else
        SSS_FREE(pxSession);
#endif

        pkcs11_sessions[xSession - 1] = NULL;
/*Lock for session open - required because multiple session_open will be attempted*/
#if (__GNUC__ && !AX_EMBEDDED)
        pthread_mutex_lock(&gSessionlock);
#endif
        if (CKR_OK == xResult) {
#ifdef PKCS11_SESSION_OPEN
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
#endif
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
    LOG_D("Destroyted mutex");
    LOG_D("SessionCount = %d", sessionCount);
#endif

    return xResult;
}

/**
 * @brief Cleans up a key structure.
 */

static int convertpemtoder(const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen)
{
    int ret;
    const unsigned char *s1, *s2, *end = input + ilen;
    size_t len = 0;

    s1 = (unsigned char *)strstr((const char *)input, "-----BEGIN");
    if (s1 == NULL) {
        return (-1);
    }

    s2 = (unsigned char *)strstr((const char *)input, "-----END");
    if (s2 == NULL) {
        return (-1);
    }

    s1 += 10;
    while (s1 < end && *s1 != '-')
        s1++;
    while (s1 < end && *s1 == '-')
        s1++;
    if (*s1 == '\r') {
        s1++;
    }
    if (*s1 == '\n') {
        s1++;
    }

    if (s2 <= s1 || s2 > end) {
        return (-1);
    }

    ret = mbedtls_base64_decode(NULL, 0, &len, (const unsigned char *)s1, s2 - s1);
    if (ret == MBEDTLS_ERR_BASE64_INVALID_CHARACTER) {
        return (ret);
    }

    if (len > *olen) {
        return (-1);
    }

    if ((ret = mbedtls_base64_decode(output, len, &len, (const unsigned char *)s1, s2 - s1)) != 0) {
        return (ret);
    }

    *olen = len;

    return (0);
}

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
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    // pxSession->xOperationInProgress = pxMechanism->mechanism;

    /*lint !e9072 It's OK to have different parameter name. */
    // ( void ) ( xSession );
    // ( void ) ( xKey );

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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
 * @brief Begin a digital signature generation session.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pxMechanism, CK_OBJECT_HANDLE xKey)
{
    CK_RV xResult = CKR_OK;
    // LOG_I("KeyId = %ld", (uint32_t) xKey);
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    // pxSession->xOperationInProgress = pxMechanism->mechanism;

    /*lint !e9072 It's OK to have different parameter name. */
    // ( void ) ( xSession );
    // ( void ) ( xKey );

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief Decrypts single-part encrypted data.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Decrypt)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen)
{
    CK_RV xResult = CKR_OK;
    // return CKR_ARGUMENTS_BAD;
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    if (pxSessionObj == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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

/**
 * @brief Finishes a multiple-part decryption operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DecryptFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes a decryption operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DecryptInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(hSession);

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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

/**
 * @brief Continues if there is multiple-part of data.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DecryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG ulEncryptedPartLen,
    CK_BYTE_PTR pPart,
    CK_ULONG_PTR pulPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Derives a key from a base key and creates a new key object.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DeriveKey)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_OBJECT_HANDLE hBaseKey,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulAttributeCount,
    CK_OBJECT_HANDLE_PTR phKey)
{
    LOG_D("%s", __FUNCTION__);
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
#if SSS_HAVE_SE05X_VER_GTE_06_00
            uint8_t derived_key_dummy[100] = {1, 2, 3};
#endif
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

#if SSS_HAVE_SE05X_VER_GTE_06_00
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sizeof(derived_key_dummy) >= derivedKeyLen);
            sss_status = sss_key_store_set_key(&pex_sss_demo_boot_ctx->ks,
                &derivedObject,
                derived_key_dummy,
                derivedKeyLen,
                derivedKeyLen * 8,
                NULL,
                0);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
#endif

            sss_status = sss_derive_key_context_init(
                &ctx_derive_key, &pex_sss_demo_boot_ctx->session, &privKeyObj, algorithm, mode);
            UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(sss_status == kStatus_SSS_Success);
            // ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            sss_status = sss_derive_key_dh(&ctx_derive_key, &pubKeyObj, &derivedObject);
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

/**
 * @brief To digest data in single-part.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Digest)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    if (!pData) {
        return CKR_ARGUMENTS_BAD;
    }

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    if (pxSessionObj == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

    sss_algorithm_t algorithm;
    xResult = ParseDigestMechanism(pxSessionObj, &algorithm);
    if (xResult != CKR_OK) {
        return xResult;
    }
#if SSS_HAVE_APPLET
    uint8_t *input = (uint8_t *)SSS_MALLOC(ulDataLen * sizeof(uint8_t));
    memset(input, 0, (ulDataLen * sizeof(uint8_t)));
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t output[64]  = {0};
    size_t inputLen     = ulDataLen;
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
                memcpy(input, pData, inputLen);
                status = sss_digest_update(&pxSessionObj->digest_ctx, input, inputLen);
                if (status != kStatus_SSS_Success) {
                    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                    xResult                            = CKR_DEVICE_ERROR;
                    UNLOCK_MUTEX_FOR_RTOS
                    goto cleanup;
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
    if (input) {
        SSS_FREE(input);
    }
#endif

    return xResult;
}

/**
 * @brief Continues a multiple-part message-digesting operation by digesting the value of a secret key.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestKey)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes the encryption for single-part data.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Encrypt)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    if (pxSessionObj == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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

/**
 * @brief Finishes a multiple-part data encryption operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_EncryptFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes an encryption operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_EncryptInit)

(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    CK_RV xResult = CKR_OK;
    LOG_W("%s", __FUNCTION__);
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(hSession);

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

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

/**
 * @brief continues a multiple-part encryption operation, processing another data part.
 */
CK_DEFINE_FUNCTION(CK_RV, C_EncryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG_PTR pulEncryptedPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Obtains general information about cryptoki.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetInfo)(CK_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief Obtains information about a particular mechanism supported by a token.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismInfo)
(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief Obtains a list of mechanism types supported by a token.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismList)
(CK_SLOT_ID slotID, CK_MECHANISM_TYPE_PTR pMechanismList, CK_ULONG_PTR pulCount)
{
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief Obtains information about a particular slot in the system.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetSlotInfo)
(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief Obtains information about a particular token in the system.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetTokenInfo)
(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief  Logs a user into a token along with usertype and pin.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Login)
(CK_SESSION_HANDLE hSession, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    LOG_D("%s", __FUNCTION__);
    return CKR_OK;
}

/**
 * @brief Logs a user out from a token.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Logout)(CK_SESSION_HANDLE hSession)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_OK;
}

/**
 * @brief Mixes additional seed material into the token’s random number generator.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SeedRandom)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_OK;
}

/**
 * @brief Modifies the value of one or more attributes of an object.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SetAttributeValue)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Finishes a multiple-part signature operation, returning the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Continues a multiple-part signature operation, processing another data part.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignUpdate)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief unwraps a wrapped key, creating a new private key or secret key object.
 */
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
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Finishes a multiple-part verification operation and check's the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Continues a multiple-part verification operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyUpdate)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Wraps (i.e., encrypts) a private or secret key.
 */
CK_DEFINE_FUNCTION(CK_RV, C_WrapKey)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_OBJECT_HANDLE hWrappingKey,
    CK_OBJECT_HANDLE hKey,
    CK_BYTE_PTR pWrappedKey,
    CK_ULONG_PTR pulWrappedKeyLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes a token along with slotID and Pin.
 */
CK_DEFINE_FUNCTION(CK_RV, C_InitToken)
(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes the normal user’s PIN.
 */
CK_DEFINE_FUNCTION(CK_RV, C_InitPIN)
(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Modifies the PIN of the user that is currently logged in.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SetPIN)
(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Closes all sessions an application has with a token..
 */
CK_DEFINE_FUNCTION(CK_RV, C_CloseAllSessions)(CK_SLOT_ID slotID)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Obtains information about the session.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetSessionInfo)
(CK_SESSION_HANDLE hSession, CK_SESSION_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);
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

/**
 * @brief Obtains a copy of the cryptographic operations state of a session, encoded as a string of bytes.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetOperationState)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pOperationState, CK_ULONG_PTR pulOperationStateLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Restores the cryptographic operations state of a session from a string of bytes obtained with C_GetOperationState.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SetOperationState)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pOperationState,
    CK_ULONG ulOperationStateLen,
    CK_OBJECT_HANDLE hEncryptionKey,
    CK_OBJECT_HANDLE hAuthenticationKey)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes a signature operation, where the data can be recovered from the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignRecoverInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Signs data in a single operation, where the data can be recovered from the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignRecover)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pSignature,
    CK_ULONG_PTR pulSignatureLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes a signature verification operation, where the data is recovered from the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecoverInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Verifies a signature in a single-part operation, where the data is recovered from the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecover)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pSignature,
    CK_ULONG ulSignatureLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Continues multiple-part digest and encryption operations.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestEncryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG_PTR pulEncryptedPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Continues a multiple-part combined decryption and digest operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DecryptDigestUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG ulEncryptedPartLen,
    CK_BYTE_PTR pPart,
    CK_ULONG_PTR pulPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Continues a multiple-part combined signature and encryption operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignEncryptUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG_PTR pulEncryptedPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Continues a multiple-part combined decryption and verification operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DecryptVerifyUpdate)
(CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG ulEncryptedPartLen,
    CK_BYTE_PTR pPart,
    CK_ULONG_PTR pulPartLen)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Legacy function which always return value CKR_FUNCTION_NOT_PARALLEL.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionStatus)(CK_SESSION_HANDLE hSession)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_PARALLEL;
}

/**
 * @brief Legacy function which always return value CKR_FUNCTION_NOT_PARALLEL.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CancelFunction)(CK_SESSION_HANDLE hSession)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_PARALLEL;
}

/**
 * @brief Waits for a slot event to occur.
 */
CK_DEFINE_FUNCTION(CK_RV, C_WaitForSlotEvent)
(CK_FLAGS flags, CK_SLOT_ID_PTR pSlot, CK_VOID_PTR pReserved)
{
    LOG_D("%s", __FUNCTION__);

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
