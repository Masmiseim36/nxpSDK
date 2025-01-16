/*
 * Amazon FreeRTOS PKCS#11 for NXP Secure element
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2018,2024 NXP
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

/* ********************** Include files ********************** */
#include "sss_pkcs11_pal.h"
#include <limits.h>

/* ********************** Constants ********************** */
#undef DEBUG_PKCS11_PAL
#define pkcs11SLOT_ID 1
#define PKCS11_MAX_DIGEST_INPUT_DATA 512
#define PKCS11_MAX_HMAC_INPUT_DATA 512
#define PKCS11_MAX_INPUT_DATA 800
#define DEFAULT_POLICY_BIN_COUNT_PCR (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE | POLICY_OBJ_ALLOW_READ)
#define DEFAULT_POLICY_USERID (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE)

/* ********************** Global variables ********************** */
int sessionCount         = 0;
bool cryptokiInitialized = false;
bool mutex_initialised   = false;
CK_RV pkcs11_read_object_size(uint32_t keyId, uint16_t *keyLen);
static uint8_t pkcs11_check_if_keyId_exists(uint32_t keyId, pSe05xSession_t session_ctx);
static P11SessionPtr_t pkcs11_sessions[MAX_PKCS11_SESSIONS] = {0};

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
 * @brief Maps an opaque caller session handle into its internal state structure.
 */
P11SessionPtr_t prvSessionPointerFromHandle(CK_SESSION_HANDLE xSession)
{
    if ((xSession == 0) || (xSession > MAX_PKCS11_SESSIONS)) {
        return NULL;
    }
    return pkcs11_sessions[xSession - 1];
}

/**
 * @brief Load the default key and certificate from storage.
 */
CK_RV pkcs11_get_attribute_parameter_index(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ATTRIBUTE_TYPE type, CK_ULONG_PTR index)
{
    CK_RV xResult      = CKR_ARGUMENTS_BAD;
    CK_ULONG i         = 0;
    CK_BBOOL foundType = CK_FALSE;

    ENSURE_OR_RETURN_ON_ERROR(NULL != pTemplate, CKR_ARGUMENTS_BAD);

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

/**
 * @brief Query the list of interface function pointers.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionList)
(CK_FUNCTION_LIST_PTR_PTR ppxFunctionList)
{
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
{
    CK_RV status = CKR_OK;

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == false, CKR_CRYPTOKI_ALREADY_INITIALIZED);

    if (pvInitArgs) {
        CK_C_INITIALIZE_ARGS_PTR initArgs = (CK_C_INITIALIZE_ARGS_PTR)(pvInitArgs);

        ENSURE_OR_RETURN_ON_ERROR(initArgs->pReserved == NULL, CKR_ARGUMENTS_BAD);

        if (initArgs->flags & CKF_OS_LOCKING_OK) {
            // Application will call from multiple threads. Library should use locks.
            if ((initArgs->CreateMutex) && (initArgs->DestroyMutex) && (initArgs->LockMutex) &&
                (initArgs->UnlockMutex)) {
                // If mutex pointers are not null, library can use either OS locking or provided functions
                LOG_D("Warning: Init Mutex patameters are ignored. Using OS locking \n");
            }
            else if (!(initArgs->CreateMutex) && !(initArgs->DestroyMutex) && !(initArgs->LockMutex) &&
                     !(initArgs->UnlockMutex)) {
                // If mutex pointers are null, library must use OS locking.
                LOG_D("Info: Using OS locking \n");
            }
        }

        if (initArgs->flags & CKF_LIBRARY_CANT_CREATE_OS_THREADS) {
            // no threads a required.
            status = CKR_OK;
        }
    }

    if (!mutex_initialised) {
        if (sss_pkcs11_mutex_init() != 0) {
            status = CKR_CANT_LOCK;
            goto exit;
        }
        mutex_initialised = true;
    }

    cryptokiInitialized = true;
exit:
    return status;
}

/**
 * @brief Finishes a multiple-part digesting operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestFinal)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    LOG_D("%s", __FUNCTION__);
    uint8_t digest[64]      = {0}; /* MAX-SHA512 */
    size_t digestLen        = sizeof(digest);
    sss_status_t sss_status = kStatus_SSS_Fail;
    size_t outputLen        = 0;

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);

    if (pulDigestLen == NULL) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        xResult                         = CKR_ARGUMENTS_BAD;
        goto exit;
    }

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
        xResult = CKR_OPERATION_NOT_INITIALIZED;
        goto exit;
    }

    if (pDigest == NULL) {
        *pulDigestLen = outputLen;
        return CKR_OK;
    }
    else {
        if (pxSession->digestUpdateCalled != CK_TRUE) {
            pxSession->xOperationInProgress = pkcs11NO_OPERATION;
            xResult                         = CKR_OPERATION_ACTIVE;
            goto exit;
        }
        if (*pulDigestLen < outputLen) {
            /*required length should be returned*/
            *pulDigestLen = outputLen;
            return CKR_BUFFER_TOO_SMALL;
        }
        if (sss_pkcs11_mutex_lock() != 0) {
            pxSession->xOperationInProgress = pkcs11NO_OPERATION;
            xResult                         = CKR_CANT_LOCK;
            goto exit;
        }

        sss_status = sss_digest_finish(&pxSession->digest_ctx, digest, &digestLen);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        ENSURE_OR_GO_EXIT(*pulDigestLen >= digestLen);
        memcpy(pDigest, digest, digestLen);
        *pulDigestLen                   = digestLen;
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
    }
    xResult = CKR_OK;
exit:
    if (pxSession->digest_ctx.session != NULL) {
        sss_digest_context_free(&pxSession->digest_ctx);
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Continues digesting operation in multiple parts.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestUpdate)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    sss_status_t sss_status   = kStatus_SSS_Fail;
    size_t chunk              = 0;
    size_t offset             = 0;

    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pxSession->xOperationInProgress != pkcs11NO_OPERATION, CKR_OPERATION_NOT_INITIALIZED);

    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);

    if (pxSession->digestUpdateCalled != CK_TRUE) {
        sss_status = sss_digest_init(&pxSession->digest_ctx);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
    }
    pxSession->digestUpdateCalled = CK_TRUE;
    do {
        chunk = (ulPartLen > PKCS11_MAX_DIGEST_INPUT_DATA) ? PKCS11_MAX_DIGEST_INPUT_DATA : ulPartLen;

        sss_status = sss_digest_update(&pxSession->digest_ctx, pPart + offset, chunk);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        ENSURE_OR_GO_EXIT((UINT_MAX - offset) >= chunk);
        offset += chunk;
        ulPartLen -= chunk;
    } while (ulPartLen > 0);

    xResult = CKR_OK;
exit:
    if (xResult != CKR_OK) {
        if (pxSession->digest_ctx.session != NULL) {
            sss_digest_context_free(&pxSession->digest_ctx);
        }
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief initializes a message-digesting operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pMechanism)
{
    CK_RV xResult             = CKR_FUNCTION_FAILED;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    sss_status_t sss_status   = kStatus_SSS_Fail;
    sss_algorithm_t algorithm = kAlgorithm_None;

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pMechanism != NULL, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(pxSession->xOperationInProgress == pkcs11NO_OPERATION, CKR_SESSION_HANDLE_INVALID);

    pxSession->xOperationInProgress = pMechanism->mechanism;

    if (pkcs11_parse_digest_mechanism(pxSession, &algorithm) != CKR_OK) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }

    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);

#if SSS_HAVE_APPLET
    LOG_W("This will cause NVM flash writes !!");
    sss_status =
        sss_digest_context_init(&pxSession->digest_ctx, &pex_sss_demo_boot_ctx->session, algorithm, kMode_SSS_Digest);
#elif SSS_HAVE_HOSTCRYPTO_ANY
    sss_status = sss_digest_context_init(
        &pxSession->digest_ctx, &pex_sss_demo_boot_ctx->host_session, algorithm, kMode_SSS_Digest);
#else
    sss_status         = kStatus_SSS_Fail;
#endif
    ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

    xResult = CKR_OK;
exit:
    if (xResult == CKR_OK) {
        pxSession->digestUpdateCalled = CK_FALSE;
    }
    else {
        /* Error */
        if (pxSession->digest_ctx.session != NULL) {
            sss_digest_context_free(&pxSession->digest_ctx);
        }
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Generate cryptographically random bytes.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GenerateRandom)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pucRandomData, CK_ULONG ulRandomLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    LOG_D("%s", __FUNCTION__);
    sss_status_t sss_status       = kStatus_SSS_Fail;
    sss_rng_context_t sss_rng_ctx = {0};

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(xSession <= MAX_PKCS11_SESSIONS, CKR_SESSION_HANDLE_INVALID);

    if (NULL == pucRandomData) {
        return CKR_ARGUMENTS_BAD;
    }

    if (ulRandomLen == 0) {
        return CKR_OK;
    }

    if (sss_pkcs11_mutex_lock() != 0) {
        return CKR_CANT_LOCK;
    }

#if SSS_HAVE_APPLET
    sss_status = sss_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->session /* Session */);
#elif SSS_HAVE_HOSTCRYPTO_ANY
    sss_status = sss_host_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->host_session /* host Session */);
#else
    sss_status         = kStatus_SSS_Fail;
#endif
    ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

    sss_status = sss_rng_get_random(&sss_rng_ctx, pucRandomData, ulRandomLen);
    ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

    xResult = CKR_OK;
exit:
    if (sss_rng_ctx.session != NULL) {
        sss_rng_context_free(&sss_rng_ctx);
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Verify the digital signature of the specified data using the public
 * key attached to this session.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Verify)
(CK_SESSION_HANDLE xSession, CK_BYTE_PTR pucData, CK_ULONG ulDataLen, CK_BYTE_PTR pucSignature, CK_ULONG ulSignatureLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj     = prvSessionPointerFromHandle(xSession);
    sss_status_t status              = kStatus_SSS_Fail;
    sss_object_t object              = {0};
    sss_asymmetric_t asymmCtx        = {0};
    sss_algorithm_t algorithm        = kAlgorithm_None;
    sss_algorithm_t digest_algorithm = kAlgorithm_None;
    sss_mac_t ctx_hmac               = {0};
    uint8_t data[1024]               = {0};
    size_t dataLen                   = sizeof(data);
    sss_digest_t digestCtx           = {0};
    uint8_t signature_tmp[512]       = {0};
    size_t signature_tmp_len         = sizeof(signature_tmp);
    size_t chunk                     = 0;
    size_t ulDataLen_tmp             = ulDataLen;
    size_t offset                    = 0;
    uint16_t keyLen                  = 0;
    uint16_t ecKeyLen                = 0;
    bool secp521r1_sign              = false;

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);
    /*
     * Check parameters.
     */
    ENSURE_OR_RETURN_ON_ERROR(NULL != pucData, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pucSignature, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);

    /* Compare the signature length with the keysize for RSA */
    if (pxSessionObj->xOperationInProgress == CKM_RSA_PKCS || pxSessionObj->xOperationInProgress == CKM_SHA1_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA256_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA384_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA512_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA224_RSA_PKCS) {
        ENSURE_OR_GO_EXIT(pxSessionObj->xOperationKeyHandle <= UINT32_MAX);
        if (pkcs11_read_object_size((uint32_t)pxSessionObj->xOperationKeyHandle, &keyLen) != CKR_OK) {
            LOG_E("Reading of object size failed !!");
            xResult = CKR_FUNCTION_FAILED;
            goto exit;
        }
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(ulSignatureLen == (CK_ULONG)keyLen, xResult, CKR_SIGNATURE_LEN_RANGE);
    }

    if (pkcs11_parse_sign_mechanism(pxSessionObj, &algorithm) != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        xResult                            = CKR_MECHANISM_INVALID;
        goto exit;
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
                xResult = CKR_DATA_LEN_RANGE;
                goto exit;
            }
        }
    }
    /* Check for the digest required mechanisms */
    if (pxSessionObj->xOperationInProgress == CKM_ECDSA_SHA1 ||
        pxSessionObj->xOperationInProgress == CKM_ECDSA_SHA224 ||
        pxSessionObj->xOperationInProgress == CKM_ECDSA_SHA256 ||
        pxSessionObj->xOperationInProgress == CKM_ECDSA_SHA384 ||
        pxSessionObj->xOperationInProgress == CKM_ECDSA_SHA512 ||
        pxSessionObj->xOperationInProgress == CKM_SHA1_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA224_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA256_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA384_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA512_RSA_PKCS ||
        pxSessionObj->xOperationInProgress == CKM_SHA1_RSA_PKCS_PSS ||
        pxSessionObj->xOperationInProgress == CKM_SHA224_RSA_PKCS_PSS ||
        pxSessionObj->xOperationInProgress == CKM_SHA256_RSA_PKCS_PSS ||
        pxSessionObj->xOperationInProgress == CKM_SHA384_RSA_PKCS_PSS ||
        pxSessionObj->xOperationInProgress == CKM_SHA512_RSA_PKCS_PSS) {
        if (pkcs11_get_digest_algorithm(algorithm, &digest_algorithm) != CKR_OK) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            xResult                            = CKR_ARGUMENTS_BAD;
            goto exit;
        }

#if SSS_HAVE_HOSTCRYPTO_ANY
        status = sss_digest_context_init(
            &digestCtx, &pex_sss_demo_boot_ctx->host_session, digest_algorithm, kMode_SSS_Digest);
#else
        status = kStatus_SSS_Fail;
#endif
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_digest_init(&digestCtx);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        while (ulDataLen_tmp > 0) {
            chunk  = (ulDataLen_tmp > PKCS11_MAX_DIGEST_INPUT_DATA) ? (PKCS11_MAX_DIGEST_INPUT_DATA) : (ulDataLen_tmp);
            status = sss_digest_update(&digestCtx, &pucData[offset], chunk);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            ENSURE_OR_GO_EXIT((SIZE_MAX - offset) >= chunk);
            offset += chunk;
            ulDataLen_tmp -= chunk;
        }

        status = sss_digest_finish(&digestCtx, &data[0], &dataLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
    }
    else {
        ENSURE_OR_GO_EXIT(ulDataLen <= sizeof(data));
        memcpy(&data[0], pucData, ulDataLen);
        dataLen = ulDataLen;
        if (algorithm == kAlgorithm_SSS_ECDSA_SHA256 && ulDataLen < 20) {
            dataLen = 32;
        }
    }

    status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    ENSURE_OR_GO_EXIT(pxSessionObj->xOperationKeyHandle <= UINT32_MAX);
    status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    /* Checking for HMAC and validating */
    if ((algorithm == kAlgorithm_SSS_HMAC_SHA1) || (algorithm == kAlgorithm_SSS_HMAC_SHA256) ||
        (algorithm == kAlgorithm_SSS_HMAC_SHA384) || (algorithm == kAlgorithm_SSS_HMAC_SHA512)) {
        size_t macLen = ulSignatureLen;
        status        = sss_mac_context_init(
            &ctx_hmac, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Mac_Validate);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        LOG_D("MAC Verify using SE05x");

        if (dataLen > PKCS11_MAX_INPUT_DATA) {
            status = sss_mac_init(&ctx_hmac);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            do {
                chunk = (dataLen > PKCS11_MAX_HMAC_INPUT_DATA) ? PKCS11_MAX_HMAC_INPUT_DATA : dataLen;

                status = sss_mac_update(&ctx_hmac, &data[0] + offset, chunk);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                ENSURE_OR_GO_EXIT((UINT_MAX - offset) >= chunk);
                offset += chunk;
                dataLen -= chunk;
            } while (dataLen > 0);

            status = sss_mac_finish(&ctx_hmac, pucSignature, &macLen);
            if (status != kStatus_SSS_Success) {
                xResult = CKR_SIGNATURE_INVALID;
                goto exit;
            }
        }
        else {
            status = sss_mac_one_go(&ctx_hmac, &data[0], dataLen, pucSignature, &macLen);
            if (status != kStatus_SSS_Success) {
                xResult = CKR_SIGNATURE_INVALID;
                goto exit;
            }
        }
    }
    else { /* ECC and RSA */
        status = sss_asymmetric_context_init(
            &asymmCtx, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Verify);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        ENSURE_OR_GO_EXIT(asymmCtx.keyObject != NULL);
        if ((asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_P) ||
            (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_K) ||
            (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_BRAINPOOL)) {
            /* Check for eckey length */
            if (pkcs11_read_object_size((uint32_t)pxSessionObj->xOperationKeyHandle, &ecKeyLen) != CKR_OK) {
                LOG_E("Reading of object size failed !!");
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
        }
        /* check for secp521r1 raw signature */
        if (((ecKeyLen == 66) || (ecKeyLen == 65)) && (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_P)) {
            if (((CK_ULONG)(ecKeyLen * 2) >= ulSignatureLen) && (ulSignatureLen >= 130)) {
                secp521r1_sign = true;
            }
        }

        /* Check on keylength and Signature length will ensure whether signature is converted in to RandS or not */
        if (((asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_P) ||
                (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_K) ||
                (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_BRAINPOOL)) &&
            (((CK_ULONG)(ecKeyLen * 2) == ulSignatureLen) || (secp521r1_sign))) {
            if (CKR_OK != pkcs11_ecRandSToSignature(
                              (uint8_t *)pucSignature, (size_t)ulSignatureLen, &signature_tmp[0], &signature_tmp_len)) {
                goto exit;
            }
        }
        else {
            ENSURE_OR_GO_EXIT(ulSignatureLen <= sizeof(signature_tmp));
            memcpy(&signature_tmp[0], pucSignature, ulSignatureLen);
            signature_tmp_len = ulSignatureLen;
        }

        LOG_D("Verify using SE05x");
        LOG_D("Verify using keyid - %x \n", object.keyId);
        status = sss_asymmetric_verify_digest(&asymmCtx, &data[0], dataLen, signature_tmp, signature_tmp_len);
        if (status != kStatus_SSS_Success) {
            xResult = CKR_SIGNATURE_INVALID;
            goto exit;
        }
    }

    xResult = CKR_OK;
exit:
    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;

    if (asymmCtx.session != NULL) {
        sss_asymmetric_context_free(&asymmCtx);
    }
    if (digestCtx.session != NULL) {
        sss_digest_context_free(&digestCtx);
    }
    if (ctx_hmac.session != NULL) {
        sss_mac_context_free(&ctx_hmac);
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
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
    CK_RV xResult                    = CKR_FUNCTION_FAILED;
    P11SessionPtr_t pxSessionObj     = prvSessionPointerFromHandle(xSession);
    sss_status_t status              = kStatus_SSS_Fail;
    sss_object_t object              = {0};
    sss_asymmetric_t asymmCtx        = {0};
    sss_algorithm_t algorithm        = kAlgorithm_None;
    sss_algorithm_t digest_algorithm = kAlgorithm_None;
    sss_mac_t ctx_hmac               = {0};
    uint16_t keySizeBytes            = 0;
    uint8_t data[1024]               = {0};
    size_t dataLen                   = sizeof(data);
    sss_digest_t digestCtx           = {0};
    uint8_t hmacOutput[64]           = {0};
    size_t hmacOutputLen             = sizeof(hmacOutput);
    uint8_t signature[512]           = {0};
    size_t sigLen                    = sizeof(signature);
    size_t chunk                     = 0;
    size_t ulDataLen_tmp             = ulDataLen;
    size_t offset                    = 0;

    LOG_D("%s", __FUNCTION__);
    LOG_D(" Input data length = %ld", ulDataLen);

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);
    /*
     * Check parameters.
     */
    ENSURE_OR_RETURN_ON_ERROR(NULL != pucData, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pulSignatureLen, CKR_ARGUMENTS_BAD);

    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);
    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
        pkcs11_parse_sign_mechanism(pxSessionObj, &algorithm) == CKR_OK, xResult, CKR_MECHANISM_INVALID);

    if (pxSessionObj->xOperationInProgress == CKM_ECDSA) {
        switch (ulDataLen) {
        case 20:
            algorithm = kAlgorithm_SSS_ECDSA_SHA1;
            LOG_D("Using - kAlgorithm_SSS_ECDSA_SHA1 algorithm");
            break;
        case 28:
            algorithm = kAlgorithm_SSS_ECDSA_SHA224;
            LOG_D("Using - kAlgorithm_SSS_ECDSA_SHA224 algorithm");
            break;
        case 32:
            algorithm = kAlgorithm_SSS_ECDSA_SHA256;
            LOG_D("Using - kAlgorithm_SSS_ECDSA_SHA256 algorithm");
            break;
        case 48:
            algorithm = kAlgorithm_SSS_ECDSA_SHA384;
            LOG_D("Using - kAlgorithm_SSS_ECDSA_SHA384 algorithm");
            break;
        case 64:
            algorithm = kAlgorithm_SSS_ECDSA_SHA512;
            LOG_D("Using - kAlgorithm_SSS_ECDSA_SHA512 algorithm");
            break;
        default:
            if (ulDataLen < 20) {
                /* ECDSA Always needs a hash value. Hash value should be deduced from
                     * input size. In case the input size is less than any supported hash
                     * value, we will pad the data with 0s and default to kAlgorithm_SSS_SHA256
                     */
                algorithm = kAlgorithm_SSS_ECDSA_SHA256;
                LOG_D("Using default - kAlgorithm_SSS_ECDSA_SHA256 algorithm");
                break;
            }
            else {
                xResult                            = CKR_DATA_LEN_RANGE;
                pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                goto exit;
            }
        }
    }

    if (pxSessionObj->xOperationInProgress == CKM_RSA_PKCS) {
        ENSURE_OR_GO_EXIT((pxSessionObj->xOperationKeyHandle) <= UINT32_MAX);
        ENSURE_OR_GO_EXIT(
            pkcs11_read_object_size((uint32_t)pxSessionObj->xOperationKeyHandle, &keySizeBytes) == CKR_OK);

        if (keySizeBytes < (ulDataLen + 11)) {
            xResult = CKR_MECHANISM_INVALID;
            goto exit;
        }
    }

    if (pxSessionObj->xOperationInProgress == CKM_RSA_PKCS_PSS || pxSessionObj->xOperationInProgress == CKM_ECDSA ||
        pxSessionObj->xOperationInProgress == CKM_RSA_PKCS || pxSessionObj->xOperationInProgress == CKM_SHA_1_HMAC ||
        pxSessionObj->xOperationInProgress == CKM_SHA256_HMAC ||
        pxSessionObj->xOperationInProgress == CKM_SHA384_HMAC ||
        pxSessionObj->xOperationInProgress == CKM_SHA512_HMAC) {
        /* Use RAW data for sign */
        ENSURE_OR_GO_EXIT(ulDataLen <= sizeof(data));
        memcpy(&data[0], pucData, ulDataLen);
        dataLen = ulDataLen;
        if (algorithm == kAlgorithm_SSS_ECDSA_SHA256 && ulDataLen < 20) {
            dataLen = 32;
        }
    }
    else {
        ENSURE_OR_GO_EXIT(pkcs11_get_digest_algorithm(algorithm, &digest_algorithm) == CKR_OK);

#if SSS_HAVE_HOSTCRYPTO_ANY
        status = sss_digest_context_init(
            &digestCtx, &pex_sss_demo_boot_ctx->host_session, digest_algorithm, kMode_SSS_Digest);
#else
        status = kStatus_SSS_Fail;
#endif
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_DEVICE_ERROR);

        LOG_D("Calculate digest(%d) of input", digest_algorithm);
        status = sss_digest_init(&digestCtx);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_DEVICE_ERROR);

        while (ulDataLen_tmp > 0) {
            chunk  = (ulDataLen_tmp > PKCS11_MAX_DIGEST_INPUT_DATA) ? (PKCS11_MAX_DIGEST_INPUT_DATA) : (ulDataLen_tmp);
            status = sss_digest_update(&digestCtx, &pucData[offset], chunk);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            ENSURE_OR_GO_EXIT((SIZE_MAX - offset) >= chunk);
            offset += chunk;
            ulDataLen_tmp -= chunk;
        }

        status = sss_digest_finish(&digestCtx, &data[0], &dataLen);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_DEVICE_ERROR);
    }

    status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    ENSURE_OR_GO_EXIT((pxSessionObj->xOperationKeyHandle) <= UINT_MAX);

    status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    /* Checking for HMAC and performing MAC operation */
    if ((algorithm == kAlgorithm_SSS_HMAC_SHA1) || (algorithm == kAlgorithm_SSS_HMAC_SHA256) ||
        (algorithm == kAlgorithm_SSS_HMAC_SHA384) || (algorithm == kAlgorithm_SSS_HMAC_SHA512)) {
        status = sss_mac_context_init(&ctx_hmac, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Mac);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        LOG_D("MAC using SE05x");
        if (dataLen > PKCS11_MAX_INPUT_DATA) {
            status = sss_mac_init(&ctx_hmac);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            chunk  = 0;
            offset = 0;
            do {
                chunk = (dataLen > PKCS11_MAX_HMAC_INPUT_DATA) ? PKCS11_MAX_HMAC_INPUT_DATA : dataLen;

                status = sss_mac_update(&ctx_hmac, &data[0] + offset, chunk);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                ENSURE_OR_GO_EXIT((UINT_MAX - offset) >= chunk);
                offset += chunk;
                dataLen -= chunk;
            } while (dataLen > 0);

            status = sss_mac_finish(&ctx_hmac, &hmacOutput[0], &hmacOutputLen);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
        }
        else {
            status = sss_mac_one_go(&ctx_hmac, &data[0], dataLen, &hmacOutput[0], &hmacOutputLen);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
        }

        if (NULL != pucSignature) {
            ENSURE_OR_GO_EXIT(*pulSignatureLen >= hmacOutputLen);
            memcpy(pucSignature, &hmacOutput[0], hmacOutputLen);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        }
        *pulSignatureLen = hmacOutputLen;
    }
    else {
        status =
            sss_asymmetric_context_init(&asymmCtx, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Sign);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        LOG_D("Sign using SE05x");
        status = sss_asymmetric_sign_digest(&asymmCtx, &data[0], dataLen, &signature[0], &sigLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        ENSURE_OR_GO_EXIT(asymmCtx.keyObject != NULL);
        if ((asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_P) ||
            (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_NIST_K) ||
            (asymmCtx.keyObject->cipherType == kSSS_CipherType_EC_BRAINPOOL)) {
            ENSURE_OR_GO_EXIT(pkcs11_ecSignatureToRandS(signature, &sigLen) == CKR_OK);
        }

        if (NULL != pucSignature) {
            ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(*pulSignatureLen >= sigLen, xResult, CKR_BUFFER_TOO_SMALL);
            memcpy(pucSignature, &signature[0], sigLen);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        }
        *pulSignatureLen = sigLen;
    }

    xResult = CKR_OK;
exit:
    if (digestCtx.session != NULL) {
        sss_digest_context_free(&digestCtx);
    }
    if (ctx_hmac.session != NULL) {
        sss_mac_context_free(&ctx_hmac);
    }
    if (asymmCtx.session != NULL) {
        sss_asymmetric_context_free(&asymmCtx);
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

sss_status_t pkcs11_sss_create_token(sss_key_store_t *keystore,
    sss_object_t *CreateObject,
    U32 ObjectId,
    sss_key_part_t KeyPart,
    sss_cipher_type_t CipherType,
    U8 *buffer,
    U32 bufferLen,
    U32 bitLen)
{
    sss_status_t status  = kStatus_SSS_Fail;
    uint8_t output[2048] = {0};
    size_t olen          = sizeof(output);

    if (pkcs11_parse_Convert_PemToDer((unsigned char *)buffer, (size_t)bufferLen, &output[0], &olen) == 0) {
        status = sss_key_object_init(CreateObject, keystore);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_key_object_allocate_handle(
            CreateObject, ObjectId, KeyPart, CipherType, olen, kKeyObject_Mode_Persistent);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_key_store_set_key(keystore, CreateObject, output, olen, bitLen, NULL, 0);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
    }
    else {
        status = sss_key_object_init(CreateObject, keystore);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_key_object_allocate_handle(
            CreateObject, ObjectId, KeyPart, CipherType, (size_t)bufferLen, kKeyObject_Mode_Persistent);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_key_store_set_key(keystore, CreateObject, buffer, (size_t)bufferLen, bitLen, NULL, 0);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
    }

exit:
    sss_key_object_free(CreateObject);
    return status;
}

CK_RV pkcs11_read_object_size(uint32_t keyId, uint16_t *keyLen)
{
    CK_RV xResult                      = CKR_FUNCTION_FAILED;
    smStatus_t sm_status               = SM_NOT_OK;
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;

    sm_status = Se05x_API_ReadSize(&se05x_session->s_ctx, keyId, keyLen);
    ENSURE_OR_GO_EXIT(sm_status == SM_OK);

    xResult = CKR_OK;
exit:
    return xResult;
}

static uint8_t pkcs11_check_if_keyId_exists(uint32_t keyId, pSe05xSession_t session_ctx)
{
    smStatus_t retStatus    = SM_NOT_OK;
    SE05x_Result_t IdExists = kSE05x_Result_NA;
    uint8_t exists          = 0;

    retStatus = Se05x_API_CheckObjectExists(session_ctx, keyId, &IdExists);
    if (retStatus == SM_OK) {
        if (IdExists == kSE05x_Result_SUCCESS) {
            exists = 1;
        }
    }
    else {
        LOG_E("Error in Se05x_API_CheckObjectExists");
    }

    return exists;
}

/**
 * @brief Un-initialize the Cryptoki module.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Finalize)(CK_VOID_PTR pvReserved)
{
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(NULL == pvReserved, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);

    if (mutex_initialised) {
        if (sss_pkcs11_mutex_destroy() != 0) {
            LOG_W("unable to destroy mutex lock");
        }
        else {
            mutex_initialised = false;
        }
    }

    cryptokiInitialized = false;
    return xResult;
}

/**
 * @brief Start a session for a cryptographic command sequence.
 */
CK_DEFINE_FUNCTION(CK_RV, C_OpenSession)
(CK_SLOT_ID xSlotID, CK_FLAGS xFlags, CK_VOID_PTR pvApplication, CK_NOTIFY xNotify, CK_SESSION_HANDLE_PTR pxSession)
{
    AX_UNUSED_ARG(pvApplication);
    AX_UNUSED_ARG(xNotify);
    CK_RV xResult                = CKR_FUNCTION_FAILED;
    P11SessionPtr_t pxSessionObj = NULL;
    bool foundEmptySessionSlot   = false;
    size_t i                     = 0;

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pxSession, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(xSlotID == pkcs11SLOT_ID, CKR_SLOT_ID_INVALID);
    ENSURE_OR_RETURN_ON_ERROR((xFlags & CKF_SERIAL_SESSION), CKR_SESSION_PARALLEL_NOT_SUPPORTED);

    /*
     * Make space for the context.
     */
#if defined(USE_RTOS) && USE_RTOS == 1
    if (NULL == (pxSessionObj = (P11SessionPtr_t)pvPortMalloc(
                     sizeof(P11Session_t)))) /*lint !e9087 Allow casting void* to other types. */
    {
        xResult = CKR_HOST_MEMORY;
        goto exit;
    }
#else
    if (NULL == (pxSessionObj = (P11SessionPtr_t)SSS_MALLOC(
                     sizeof(P11Session_t)))) /*lint !e9087 Allow casting void* to other types. */
    {
        xResult = CKR_HOST_MEMORY;
        goto exit;
    }
#endif
    memset(pxSessionObj, 0, sizeof(P11Session_t));

    /*
    * Assign the session.
    */
    pxSessionObj->ulState = 0u != (xFlags & CKF_RW_SESSION) ? CKS_RW_PUBLIC_SESSION : CKS_RO_PUBLIC_SESSION;
    pxSessionObj->xOpened = CK_TRUE;
    pxSessionObj->xFlags  = xFlags;

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
        xResult = CKR_DEVICE_MEMORY;
        goto exit;
    }

    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;

/* Lock for session open - required because multiple session_open will be attempted */
#ifdef PKCS11_SESSION_OPEN
    if (sss_pkcs11_mutex_lock() != 0) {
        xResult = CKR_CANT_LOCK;
        goto exit;
    }
    if (sessionCount == 0) {
        sss_status_t sss_status = kStatus_SSS_Fail;
        char *portName          = NULL;

#if defined(T1oI2C)
        SM_Close(NULL, 0);
#endif
        /* If portname is given in ENV */
        sss_status = ex_sss_boot_connectstring(0, NULL, &portName);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = ex_sss_boot_open(pex_sss_demo_boot_ctx, portName);
#if defined(_MSC_VER)
        if (portName) {
            char *dummy_portName = NULL;
            size_t dummy_sz      = 0;
            _dupenv_s(&dummy_portName, &dummy_sz, EX_SSS_BOOT_SSS_PORT);
            if (NULL != dummy_portName) {
                free(dummy_portName);
                free(portName);
            }
        }
#endif // _MSC_VER
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(sss_status == kStatus_SSS_Success, xResult, CKR_GENERAL_ERROR);

#if defined(EX_SSS_BOOT_DO_ERASE) && (EX_SSS_BOOT_DO_ERASE == 1)
        sss_status = ex_sss_boot_factory_reset(pex_sss_demo_boot_ctx);
        if (sss_status != kStatus_SSS_Success) {
            LOG_E("Factory Reset failed");
        }
#endif

#if SSS_HAVE_HOSTCRYPTO_ANY
        if ((pex_sss_demo_boot_ctx->host_session.subsystem) == kType_SSS_SubSystem_NONE) {
            sss_type_t hostsubsystem = kType_SSS_SubSystem_NONE;
#if SSS_HAVE_HOSTCRYPTO_MBEDTLS
            hostsubsystem = kType_SSS_mbedTLS;
#elif SSS_HAVE_HOSTCRYPTO_OPENSSL
            hostsubsystem = kType_SSS_OpenSSL;
#endif
            sss_status = sss_host_session_open(
                &pex_sss_demo_boot_ctx->host_session, hostsubsystem, 0, kSSS_ConnectionType_Plain, NULL);
            ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
        }
#endif
        sss_status = ex_sss_key_store_and_object_init(pex_sss_demo_boot_ctx);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

#if SSS_PKCS11_ENABLE_CLOUD_DEMO
#if SSS_HAVE_HOSTCRYPTO_ANY
        pex_sss_demo_tls_ctx->pHost_ks = &pex_sss_demo_boot_ctx->host_ks;
#endif
        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->obj, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->dev_cert, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->interCaCert, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_init(&pex_sss_demo_tls_ctx->pub_obj, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
#endif
    }
#endif

    sessionCount++;
    xResult = CKR_OK;
exit:
    if (xResult != CKR_OK) {
        if (pxSessionObj != NULL) {
#if defined(USE_RTOS) && USE_RTOS == 1
            vPortFree(pxSessionObj);
#else
            SSS_FREE(pxSessionObj);
#endif
        }
        if (pex_sss_demo_boot_ctx->session.subsystem != kType_SSS_SubSystem_NONE) {
            ex_sss_session_close(pex_sss_demo_boot_ctx);
        }
#if SSS_HAVE_HOSTCRYPTO_ANY
        if ((pex_sss_demo_boot_ctx->host_session.subsystem) != kType_SSS_SubSystem_NONE) {
            sss_host_session_close(&pex_sss_demo_boot_ctx->host_session);
        }
#endif
    }
#ifdef PKCS11_SESSION_OPEN
    /* Unlock for session open - required because multiple session_open will be attempted */
    if (sss_pkcs11_mutex_unlock() != 0) {
        LOG_W("sss_pkcs11_mutex_unlock failed ");
    }
#endif
    return xResult;
}

/**
 * @brief Terminate a session and release resources.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CloseSession)(CK_SESSION_HANDLE xSession)
{
    CK_RV xResult             = CKR_OK;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pxSession, CKR_SESSION_HANDLE_INVALID);

    /*
    * Tear down the session.
    */

#if defined(USE_RTOS) && USE_RTOS == 1
    vPortFree(pxSession);
#else
    SSS_FREE(pxSession);
#endif

    pkcs11_sessions[xSession - 1] = NULL;

#ifdef PKCS11_SESSION_OPEN
    if (sessionCount == 1) {
        if (sss_pkcs11_mutex_lock() != 0) {
            xResult = CKR_CANT_LOCK;
            return xResult;
        }
#if SSS_PKCS11_ENABLE_CLOUD_DEMO
        sss_key_object_free(&pex_sss_demo_tls_ctx->obj);
        sss_key_object_free(&pex_sss_demo_tls_ctx->dev_cert);
        sss_key_object_free(&pex_sss_demo_tls_ctx->interCaCert);
        sss_key_object_free(&pex_sss_demo_tls_ctx->pub_obj);
#endif
        ex_sss_session_close(pex_sss_demo_boot_ctx);
#if SSS_HAVE_HOSTCRYPTO_ANY
        if ((pex_sss_demo_boot_ctx->host_session.subsystem) != kType_SSS_SubSystem_NONE) {
            sss_host_session_close(&pex_sss_demo_boot_ctx->host_session);
        }
#endif
        if (sss_pkcs11_mutex_unlock() != 0) {
            LOG_W("sss_pkcs11_mutex_unlock failed ");
        }
    }
#endif

    sessionCount--;

    return xResult;
}

/**
 * @brief Query the list of slots. A single default slot is implemented.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetSlotList)
(CK_BBOOL xTokenPresent, CK_SLOT_ID_PTR pxSlotList, CK_ULONG_PTR pulCount)
{
    AX_UNUSED_ARG(xTokenPresent);
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    if (NULL == pulCount) {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else if (NULL == pxSlotList) {
        *pulCount = 1;
    }
    else {
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
    CK_RV xResult                = CKR_FUNCTION_FAILED;
    sss_algorithm_t algorithm    = kAlgorithm_None;
    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);
    if (pulDataLen == NULL) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }
    if (pEncryptedData == NULL) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }
    if (pkcs11_parse_encryption_mechanism(pxSessionObj, &algorithm) != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }
    if (algorithm == kAlgorithm_None) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }

    if ((algorithm == kAlgorithm_SSS_RSAES_PKCS1_V1_5) || (algorithm == kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1)) {
        /*RSA Encryption*/
        xResult = pkcs11_se05x_asymmetric_decrypt(
            pxSessionObj, algorithm, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
    }
    else {
        /*Symmetric Encryption*/
        xResult = pkcs11_se05x_symmetric_decrypt(
            pxSessionObj, algorithm, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
    }

    return xResult;
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
    sss_algorithm_t algorithm = kAlgorithm_None;
    sss_status_t status       = kStatus_SSS_Fail;
    sss_object_t obj          = {0};
    sss_cipher_type_t cipher  = 0;

    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pMechanism != NULL, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(pxSession->xOperationInProgress == pkcs11NO_OPERATION, CKR_OPERATION_ACTIVE);

    status = pkcs11_get_validated_sss_object(pxSession, hKey, &obj);
    if (status != kStatus_SSS_Success) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_KEY_HANDLE_INVALID;
    }
    pxSession->xOperationInProgress = pMechanism->mechanism;

    if (pMechanism->ulParameterLen % 8 != 0) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_PARAM_INVALID;
    }
    pxSession->xOperationKeyHandle = hKey;
    if (pMechanism->pParameter) {
        pxSession->mechParameter    = pMechanism->pParameter;
        pxSession->mechParameterLen = pMechanism->ulParameterLen;
    }
    else {
        pxSession->mechParameterLen = 0;
    }

    if (pkcs11_parse_encryption_mechanism(pxSession, &algorithm) != CKR_OK) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }

    xResult = pkcs11_is_valid_keytype(algorithm, &cipher, &obj);
    if (xResult != CKR_OK) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }

    return xResult;
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
    AX_UNUSED_ARG(hSession);
    CK_RV xResult                      = CKR_FUNCTION_FAILED;
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;
    sss_status_t sss_status            = kStatus_SSS_Fail;
    sss_object_t privKeyObj            = {0};
    sss_object_t pubKeyObj             = {0};
    sss_object_t derivedObject         = {0};
    uint32_t private_keyId             = 0;
    uint32_t derivedKeyId              = 0;
    uint32_t public_KeyId              = 0;
    sss_derive_key_t ctx_derive_key    = {0};
    size_t derivedKeyLen               = 0;
    uint16_t keySize                   = 0;
    uint8_t publicKeyBuffer[256]       = {0};
    size_t publicKeyBufferLen          = sizeof(publicKeyBuffer);
    CK_ULONG attributeIndex            = 0;
    CK_MECHANISM_TYPE mechType         = 0;
    size_t KeyBitLen                   = 0;
    sss_cipher_type_t sharedObjCipher  = kSSS_CipherType_NONE;
    size_t keyByteLen                  = 0;
#if SSS_HAVE_SE05X_VER_GTE_07_02
    uint8_t derived_key_dummy[256] = {1, 2, 3};
    uint8_t derivedSessionKey[256] = {0};
    size_t derivedSessionKeyLen    = sizeof(derivedSessionKey);
    uint32_t hmac_keyId            = 0;
    smStatus_t sm_status           = SM_NOT_OK;
    sss_object_t hmacKeyObj        = {0};
    sss_se05x_session_t *pSession  = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;
    SE05x_Result_t IdExists        = kSE05x_Result_NA;
    SE05x_MACAlgo_t macAlgo        = kSE05x_MACAlgo_NA;
#endif

    LOG_D("%s", __FUNCTION__);
    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);
    ENSURE_OR_RETURN_ON_ERROR(hBaseKey <= UINT32_MAX, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(pMechanism != NULL, CKR_ARGUMENTS_BAD);

    mechType = pMechanism->mechanism;

    if ((mechType != CKM_ECDH1_DERIVE) && (mechType != CKM_PKCS5_PBKD2)) {
        /*
         * We support ECDH and HKDF mechanisms for key derivation.
         * As per PKCS#11 v2.40, HKDF mechanism is not supported by
         * PKCS#11. CKM_HKDF_DERIVE is added in PKCS#11 v3.0
         */
        return CKR_MECHANISM_INVALID;
    }

    if (mechType == CKM_ECDH1_DERIVE) {
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(pMechanism->ulParameterLen != 0, xResult, CKR_ARGUMENTS_BAD);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(pMechanism->pParameter != NULL, xResult, CKR_ARGUMENTS_BAD);

        private_keyId                            = (uint32_t)hBaseKey;
        CK_ECDH1_DERIVE_PARAMS_PTR p_ecdh1Params = (CK_ECDH1_DERIVE_PARAMS_PTR)pMechanism->pParameter;
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            !(!p_ecdh1Params->ulPublicDataLen || !p_ecdh1Params->pPublicData), xResult, CKR_ARGUMENTS_BAD);

        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            !((p_ecdh1Params->kdf != CKD_SHA1_KDF) && (p_ecdh1Params->kdf != CKD_SHA224_KDF) &&
                (p_ecdh1Params->kdf != CKD_SHA256_KDF) && (p_ecdh1Params->kdf != CKD_SHA384_KDF) &&
                (p_ecdh1Params->kdf != CKD_SHA512_KDF) && (p_ecdh1Params->kdf != CKD_NULL)),
            xResult,
            CKR_ARGUMENTS_BAD);

        if (!pkcs11_check_if_keyId_exists(private_keyId, &se05x_session->s_ctx)) {
            xResult = CKR_KEY_HANDLE_INVALID;
            goto exit;
        }

        if ((pkcs11_get_attribute_parameter_index(pTemplate, ulAttributeCount, CKA_CLASS, &attributeIndex) != CKR_OK) ||
            (*((CK_OBJECT_CLASS_PTR)pTemplate[attributeIndex].pValue) != CKO_SECRET_KEY)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        /* Passed keytype CKK_GENERIC_SECRET or CKK_AES will create shared secret key of AES/HMAC */
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR((pkcs11_get_attribute_parameter_index(
                                                 pTemplate, ulAttributeCount, CKA_KEY_TYPE, &attributeIndex) == CKR_OK),
            xResult,
            CKR_ARGUMENTS_BAD);

        if (*((CK_KEY_TYPE *)pTemplate[attributeIndex].pValue) == CKK_AES) {
            sharedObjCipher = kSSS_CipherType_AES;
        }
        else if (*((CK_KEY_TYPE *)pTemplate[attributeIndex].pValue) == CKK_GENERIC_SECRET) {
            sharedObjCipher = kSSS_CipherType_HMAC;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            (pkcs11_get_attribute_parameter_index(pTemplate, ulAttributeCount, CKA_VALUE_LEN, &attributeIndex) ==
                CKR_OK),
            xResult,
            CKR_ARGUMENTS_BAD);

        keyByteLen = *((size_t *)pTemplate[attributeIndex].pValue);

        if ((sharedObjCipher == kSSS_CipherType_AES) &&
            ((keyByteLen != 16) && (keyByteLen != 24) && (keyByteLen != 32))) {
            LOG_E("Unsupported key length %d", keyByteLen);
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        derivedKeyLen = keyByteLen;

        if (pkcs11_get_attribute_parameter_index(pTemplate, ulAttributeCount, CKA_LABEL, &attributeIndex) != CKR_OK) {
            /* Label not passed. Create random keyID */
            ENSURE_OR_GO_EXIT(CKR_OK == pkcs11_label_to_keyId((unsigned char *)"", 0, &derivedKeyId));
        }
        else {
            ENSURE_OR_GO_EXIT(
                CKR_OK == pkcs11_label_to_keyId(
                              pTemplate[attributeIndex].pValue, pTemplate[attributeIndex].ulValueLen, &derivedKeyId));
        }

        sss_status = sss_key_object_init(&privKeyObj, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(&privKeyObj, private_keyId);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        ENSURE_OR_GO_EXIT(pkcs11_read_object_size(private_keyId, &keySize) == CKR_OK);
        /* Using the ciphertype kSSS_CipherType_EC_NIST_P/NIST_K/BRAINPOOL only as mechanism CKM_EC_KEY_PAIR_GEN is only supported */
        if ((privKeyObj.cipherType != kSSS_CipherType_EC_NIST_P) &&
            (privKeyObj.cipherType != kSSS_CipherType_EC_NIST_K) &&
            (privKeyObj.cipherType != kSSS_CipherType_EC_BRAINPOOL)) {
            LOG_E("In the current implementation only NISTP/NISTK/BRAINPOOL curve is supported !!");
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        ENSURE_OR_GO_EXIT(pkcs11_add_ec_header(keySize,
                              privKeyObj.cipherType,
                              publicKeyBuffer,
                              &publicKeyBufferLen,
                              p_ecdh1Params->pPublicData,
                              p_ecdh1Params->ulPublicDataLen,
                              &KeyBitLen) == CKR_OK);

        /* Import the public key now */
        ENSURE_OR_GO_EXIT(pkcs11_label_to_keyId((unsigned char *)"", 0, &public_KeyId) == CKR_OK);

        sss_status = sss_key_object_init(&pubKeyObj, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_allocate_handle(&pubKeyObj,
            public_KeyId,
            kSSS_KeyPart_Public,
            (sss_cipher_type_t)privKeyObj.cipherType,
            KeyBitLen,
            kKeyObject_Mode_Persistent);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_store_set_key(
            &pex_sss_demo_boot_ctx->ks, &pubKeyObj, publicKeyBuffer, publicKeyBufferLen, KeyBitLen, NULL, 0);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        /* Start process for derivation now */
        sss_status = sss_key_object_init(&derivedObject, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_allocate_handle(&derivedObject,
            derivedKeyId,
            kSSS_KeyPart_Default,
            sharedObjCipher,
            derivedKeyLen * 8,
            kKeyObject_Mode_Persistent);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

#if SSS_HAVE_SE05X_VER_GTE_07_02
        if (!(sizeof(derived_key_dummy) >= derivedKeyLen)) {
            xResult = CKR_BUFFER_TOO_SMALL;
            goto exit;
        }
        sss_status = sss_key_store_set_key(
            &pex_sss_demo_boot_ctx->ks, &derivedObject, derived_key_dummy, derivedKeyLen, derivedKeyLen * 8, NULL, 0);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
#endif

        sss_status = sss_derive_key_context_init(&ctx_derive_key,
            &pex_sss_demo_boot_ctx->session,
            &privKeyObj,
            kAlgorithm_SSS_ECDH,
            kMode_SSS_ComputeSharedSecret);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_derive_key_dh(&ctx_derive_key, &pubKeyObj, &derivedObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        LOG_I("KeyId for shared secret is 0x%x", derivedKeyId);
        *phKey = derivedKeyId;
    }
#if SSS_HAVE_SE05X_VER_GTE_07_02
    else if (mechType == CKM_PKCS5_PBKD2) {
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(pMechanism->ulParameterLen != 0, xResult, CKR_ARGUMENTS_BAD);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(pMechanism->pParameter != NULL, xResult, CKR_ARGUMENTS_BAD);

        hmac_keyId = (uint32_t)hBaseKey;
        /* Checking and deleting the key if already present */
        if (SM_OK == Se05x_API_CheckObjectExists(&pSession->s_ctx, hmac_keyId, &IdExists)) {
            if (IdExists == kSE05x_Result_SUCCESS) {
                LOG_W("Key id 0x%X already exists!!", hmac_keyId);
                ENSURE_OR_GO_EXIT(SM_OK == Se05x_API_DeleteSecureObject(&pSession->s_ctx, hmac_keyId));
            }
        }
        else {
            LOG_E("Se05x_API_CheckObjectExists Failed !!");
            goto exit;
        }

        CK_PKCS5_PBKD2_PARAMS2_PTR p_pbkd2Params = (CK_PKCS5_PBKD2_PARAMS2_PTR)pMechanism->pParameter;

        /* Select mac algorithm based on passed params */
        if (p_pbkd2Params->prf == CKP_PKCS5_PBKD2_HMAC_SHA1) {
            macAlgo = kSE05x_MACAlgo_HMAC_SHA1;
        }
        else if (p_pbkd2Params->prf == CKP_PKCS5_PBKD2_HMAC_SHA256) {
            macAlgo = kSE05x_MACAlgo_HMAC_SHA256;
        }
        else if (p_pbkd2Params->prf == CKP_PKCS5_PBKD2_HMAC_SHA384) {
            macAlgo = kSE05x_MACAlgo_HMAC_SHA384;
        }
        else if (p_pbkd2Params->prf == CKP_PKCS5_PBKD2_HMAC_SHA512) {
            macAlgo = kSE05x_MACAlgo_HMAC_SHA512;
        }
        else {
            LOG_E("unsupported algorithm passed !!");
            goto exit;
        }

        if ((pkcs11_get_attribute_parameter_index(pTemplate, ulAttributeCount, CKA_CLASS, &attributeIndex) != CKR_OK) ||
            (*((CK_OBJECT_CLASS_PTR)pTemplate[attributeIndex].pValue) != CKO_SECRET_KEY)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        if ((pkcs11_get_attribute_parameter_index(pTemplate, ulAttributeCount, CKA_KEY_TYPE, &attributeIndex) ==
                CKR_OK)) {
            if ((*((CK_KEY_TYPE *)pTemplate[attributeIndex].pValue) != CKK_AES) &&
                (*((CK_KEY_TYPE *)pTemplate[attributeIndex].pValue) != CKK_GENERIC_SECRET)) {
                xResult = CKR_ARGUMENTS_BAD;
                goto exit;
            }
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        if ((pkcs11_get_attribute_parameter_index(pTemplate, ulAttributeCount, CKA_VALUE_LEN, &attributeIndex) !=
                CKR_OK) ||
            (*((size_t *)pTemplate[attributeIndex].pValue) == 0)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        derivedKeyLen = *((size_t *)pTemplate[attributeIndex].pValue);

        if (derivedKeyLen > MAX_PBKDF_REQ_LEN) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        if (pkcs11_get_attribute_parameter_index(pTemplate, ulAttributeCount, CKA_LABEL, &attributeIndex) != CKR_OK) {
            /* Label not passed. Create random keyID */
            ENSURE_OR_GO_EXIT(CKR_OK == pkcs11_label_to_keyId((unsigned char *)"", 0, &derivedKeyId));
        }
        else {
            ENSURE_OR_GO_EXIT(
                CKR_OK == pkcs11_label_to_keyId(
                              pTemplate[attributeIndex].pValue, pTemplate[attributeIndex].ulValueLen, &derivedKeyId));
        }

        /* Create a hmac object */
        const sss_policy_u common = {.type = KPolicy_Common,
            .auth_obj_id                   = 0,
            .policy                        = {.common = {
                           .req_Sm     = 0,
                           .can_Delete = 1,
                           .can_Read   = 1,
                           .can_Write  = 1,
                       }}};

        const sss_policy_u hmackeyPol = {.type = KPolicy_Sym_Key,
            .auth_obj_id                       = 0,
            .policy                            = {.symmkey = {
                           .can_Write = 1,
                           .can_PBKDF = 1,
                           .can_KD    = 1,
                       }}};

        sss_policy_t policy_for_hmac_key = {.nPolicies = 2, .policies = {&hmackeyPol, &common}};

        sss_status = sss_key_object_init(&hmacKeyObj, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_allocate_handle(&hmacKeyObj,
            hmac_keyId,
            kSSS_KeyPart_Default,
            kSSS_CipherType_HMAC,
            (size_t)p_pbkd2Params->ulPasswordLen,
            kKeyObject_Mode_Persistent);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_store_set_key(&pex_sss_demo_boot_ctx->ks,
            &hmacKeyObj,
            (const uint8_t *)p_pbkd2Params->pPassword,
            p_pbkd2Params->ulPasswordLen,
            (p_pbkd2Params->ulPasswordLen) * 8,
            &policy_for_hmac_key,
            sizeof(policy_for_hmac_key));
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sm_status = Se05x_API_PBKDF2_extended(&pSession->s_ctx,
            hmac_keyId,
            (const uint8_t *)p_pbkd2Params->pSaltSourceData,
            (size_t)p_pbkd2Params->ulSaltSourceDataLen,
            0, // Salt id
            p_pbkd2Params->iterations,
            macAlgo,
            derivedKeyLen,
            derivedKeyId, // Derived Session Key id
            derivedSessionKey,
            &derivedSessionKeyLen);
        ENSURE_OR_GO_EXIT(sm_status == SM_OK);
        LOG_AU8_I(derivedSessionKey, derivedSessionKeyLen);
        LOG_I("KeyId for derived key is 0x%x", derivedKeyId);
        *phKey = derivedKeyId;
    }
#endif //SSS_HAVE_SE05X_VER_GTE_07_02
    else {
        goto exit;
    }

    xResult = CKR_OK;
exit:
    if (sss_status != kStatus_SSS_Success) {
        xResult = CKR_FUNCTION_FAILED;
    }
    if (pubKeyObj.keyStore) {
        sss_key_store_erase_key(pubKeyObj.keyStore, &pubKeyObj);
    }
    if (ctx_derive_key.session != NULL) {
        sss_derive_key_context_free(&ctx_derive_key);
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief To digest data in single-part.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Digest)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV xResult                = CKR_FUNCTION_FAILED;
    sss_algorithm_t algorithm    = kAlgorithm_None;
    sss_status_t status          = kStatus_SSS_Fail;
    size_t outputLen             = 0;
    size_t inputLen              = ulDataLen;
    size_t chunk                 = 0;
    size_t offset                = 0;
    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);

    if ((pData == NULL) || (pulDigestLen == NULL)) {
        xResult                            = CKR_ARGUMENTS_BAD;
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        goto exit;
    }

    if (sss_pkcs11_mutex_lock() != 0) {
        xResult                            = CKR_CANT_LOCK;
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        goto exit;
    }

    if (pxSessionObj->digestUpdateCalled == CK_TRUE) {
        xResult                            = CKR_OPERATION_ACTIVE;
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        goto exit;
    }
    switch (pxSessionObj->xOperationInProgress) {
    case CKM_SHA_1:
        outputLen = 20;
        algorithm = kAlgorithm_SSS_SHA1;
        break;
    case CKM_SHA224:
        outputLen = 28;
        algorithm = kAlgorithm_SSS_SHA224;
        break;
    case CKM_SHA256:
        outputLen = 32;
        algorithm = kAlgorithm_SSS_SHA256;
        break;
    case CKM_SHA384:
        outputLen = 48;
        algorithm = kAlgorithm_SSS_SHA384;
        break;
    case CKM_SHA512:
        outputLen = 64;
        algorithm = kAlgorithm_SSS_SHA512;
        break;
    default:
        xResult                            = CKR_OPERATION_NOT_INITIALIZED;
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        goto exit;
    }

    if (pDigest == NULL) {
        /* Return the required length */
        *pulDigestLen = outputLen;
    }
    else {
        status = sss_digest_context_init(
            &pxSessionObj->digest_ctx, &pex_sss_demo_boot_ctx->session, algorithm, kMode_SSS_Digest);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        if (outputLen > *pulDigestLen) {
            xResult = CKR_BUFFER_TOO_SMALL;
            /* Return the required length */
            *pulDigestLen = outputLen;
            goto exit;
        }
        if (inputLen > PKCS11_MAX_INPUT_DATA) {
            status = sss_digest_init(&pxSessionObj->digest_ctx);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            do {
                chunk = (inputLen > PKCS11_MAX_DIGEST_INPUT_DATA) ? PKCS11_MAX_DIGEST_INPUT_DATA : inputLen;

                status = sss_digest_update(&pxSessionObj->digest_ctx, pData + offset, chunk);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                ENSURE_OR_GO_EXIT((UINT_MAX - offset) >= chunk);
                offset += chunk;
                inputLen -= chunk;
            } while (inputLen > 0);

            status = sss_digest_finish(&pxSessionObj->digest_ctx, pDigest, &outputLen);
            ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_DEVICE_ERROR);
        }
        else {
            status = sss_digest_one_go(&pxSessionObj->digest_ctx, pData, inputLen, pDigest, &outputLen);
            ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_DEVICE_ERROR);
        }
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(outputLen <= *pulDigestLen, xResult, CKR_BUFFER_TOO_SMALL);
        *pulDigestLen                      = outputLen;
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
    }

    xResult = CKR_OK;
exit:
    if (pxSessionObj->digest_ctx.session != NULL) {
        sss_digest_context_free(&pxSessionObj->digest_ctx);
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
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
    CK_RV xResult             = CKR_FUNCTION_FAILED;
    sss_algorithm_t algorithm = kAlgorithm_None;

    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);
    if (pData == NULL) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }
    if (pulEncryptedDataLen == NULL) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_ARGUMENTS_BAD;
    }
    if (pkcs11_parse_encryption_mechanism(pxSessionObj, &algorithm) != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }
    if (algorithm == kAlgorithm_None) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }

    if ((algorithm == kAlgorithm_SSS_RSAES_PKCS1_V1_5) || (algorithm == kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1)) {
        /*RSA Encryption*/
        xResult = pkcs11_se05x_asymmetric_encrypt(
            pxSessionObj, algorithm, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
        if (xResult != CKR_OK) {
            goto exit;
        }
    }
    else {
        /*Symmetric Encryption*/
        xResult = pkcs11_se05x_symmetric_encrypt(
            pxSessionObj, algorithm, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
        if (xResult != CKR_OK) {
            goto exit;
        }
    }

    xResult = CKR_OK;
exit:
    return xResult;
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
    sss_algorithm_t algorithm = kAlgorithm_None;
    sss_status_t status       = kStatus_SSS_Fail;
    sss_object_t obj          = {0};
    sss_cipher_type_t cipher  = 0;

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pMechanism != NULL, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(pxSession->xOperationInProgress == pkcs11NO_OPERATION, CKR_OPERATION_ACTIVE);

    status = pkcs11_get_validated_sss_object(pxSession, hKey, &obj);
    if (status != kStatus_SSS_Success) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_KEY_HANDLE_INVALID;
    }
    pxSession->xOperationInProgress = pMechanism->mechanism;

    if (pMechanism->ulParameterLen % 8 != 0) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_PARAM_INVALID;
    }
    pxSession->xOperationKeyHandle = hKey;
    if (pMechanism->pParameter) {
        pxSession->mechParameter    = pMechanism->pParameter;
        pxSession->mechParameterLen = pMechanism->ulParameterLen;
    }
    else {
        pxSession->mechParameterLen = 0;
    }

    if (pkcs11_parse_encryption_mechanism(pxSession, &algorithm) != CKR_OK) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_MECHANISM_INVALID;
    }

    xResult = pkcs11_is_valid_keytype(algorithm, &cipher, &obj);
    if (xResult != CKR_OK) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return xResult;
    }

    return xResult;
}

/**
 * @brief Obtains general information about cryptoki.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetInfo)(CK_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(pInfo != NULL, CKR_ARGUMENTS_BAD);

    pInfo->cryptokiVersion.major = CRYPTOKI_VERSION_MAJOR;
    pInfo->cryptokiVersion.minor = CRYPTOKI_VERSION_MINOR;
    memset(pInfo->manufacturerID, ' ', sizeof(pInfo->manufacturerID));
    memset(pInfo->libraryDescription, ' ', sizeof(pInfo->libraryDescription));
    pInfo->flags          = 0;
    pInfo->libraryVersion = PKCS11_LIBRARY_VERSION;
    return CKR_OK;
}

/**
 * @brief Obtains information about a particular mechanism supported by a token.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismInfo)
(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo)
{
    AX_UNUSED_ARG(slotID);
    LOG_D("%s", __FUNCTION__);

    CK_RV xResult               = CKR_MECHANISM_INVALID;
    CK_MECHANISM_INFO mech_info = {.ulMinKeySize = 0, .ulMaxKeySize = 0, .flags = CKF_HW};

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(slotID == pkcs11SLOT_ID, CKR_SLOT_ID_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pInfo != NULL, CKR_ARGUMENTS_BAD);

    if (type == CKM_RSA_PKCS) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA1_RSA_PKCS || type == CKM_SHA224_RSA_PKCS || type == CKM_SHA256_RSA_PKCS ||
             type == CKM_SHA384_RSA_PKCS || type == CKM_SHA512_RSA_PKCS) {
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
    else if (type == CKM_SHA1_RSA_PKCS_PSS || type == CKM_SHA224_RSA_PKCS_PSS || type == CKM_SHA256_RSA_PKCS_PSS ||
             type == CKM_SHA384_RSA_PKCS_PSS || type == CKM_SHA512_RSA_PKCS_PSS) {
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
    else if (type == CKM_AES_ECB || type == CKM_AES_CBC || type == CKM_AES_CTR) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_ENCRYPT | CKF_DECRYPT;
        xResult                = CKR_OK;
    }
    else if (type == CKM_SHA_1 || type == CKM_SHA224 || type == CKM_SHA256 || type == CKM_SHA384 ||
             type == CKM_SHA512) {
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
    else if (type == CKM_ECDSA_SHA1 || type == CKM_ECDSA_SHA224 || type == CKM_ECDSA_SHA256 ||
             type == CKM_ECDSA_SHA384 || type == CKM_ECDSA_SHA512) {
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
    else if (type == CKM_RSA_X_509) {
        mech_info.ulMinKeySize = 1024;
        mech_info.ulMaxKeySize = 4096;
        mech_info.flags        = mech_info.flags | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_AES_KEY_GEN || type == CKM_DES2_KEY_GEN || type == CKM_DES3_KEY_GEN) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_GENERATE;
        xResult                = CKR_OK;
    }
    else if (type == CKM_ECDH1_DERIVE) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_DERIVE;
        xResult                = CKR_OK;
    }
    else if ((type == CKM_SHA_1_HMAC) || (type == CKM_SHA256_HMAC) || (type == CKM_SHA384_HMAC) ||
             (type == CKM_SHA512_HMAC)) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 512;
        mech_info.flags        = mech_info.flags | CKF_SIGN | CKF_VERIFY;
        xResult                = CKR_OK;
    }
    else if (type == CKM_GENERIC_SECRET_KEY_GEN) {
        mech_info.ulMinKeySize = 128;
        mech_info.ulMaxKeySize = 256;
        mech_info.flags        = mech_info.flags | CKF_GENERATE;
        xResult                = CKR_OK;
    }
    else {
        // do nothing.
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
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(slotID == pkcs11SLOT_ID, CKR_SLOT_ID_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pulCount != NULL, CKR_ARGUMENTS_BAD);

    CK_MECHANISM_TYPE mechanisms[] = {
    /* RSA Algorithms */
#if SSS_HAVE_RSA
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
#endif //SSS_HAVE_RSA
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
        CKM_ECDSA_SHA224,
        CKM_ECDSA_SHA256,
        CKM_ECDSA_SHA384,
        CKM_ECDSA_SHA512,
        /* Key Generation algorithms */
        CKM_EC_KEY_PAIR_GEN,
#if SSS_HAVE_RSA
        CKM_RSA_PKCS_KEY_PAIR_GEN,
        CKM_RSA_X_509,
#endif //SSS_HAVE_RSA
        CKM_AES_KEY_GEN,
        CKM_DES2_KEY_GEN,
        CKM_DES3_KEY_GEN,
        /* Key Derivation algorithms */
        CKM_ECDH1_DERIVE,
        /* HMAC algorithms */
        CKM_SHA_1_HMAC,
        CKM_SHA256_HMAC,
        CKM_SHA384_HMAC,
        CKM_SHA512_HMAC,
        CKM_GENERIC_SECRET_KEY_GEN,
    };

    CK_ULONG numOfMechs = sizeof(mechanisms) / sizeof(mechanisms[0]);

    if (pMechanismList) {
        if (*pulCount < numOfMechs) {
            xResult = CKR_BUFFER_TOO_SMALL;
        }
        else {
            memcpy(pMechanismList, &mechanisms[0], sizeof(mechanisms));
        }
    }

    *pulCount = numOfMechs;
    return xResult;
}

/**
 * @brief Obtains information about a particular slot in the system.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetSlotInfo)
(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(pInfo != NULL, CKR_ARGUMENTS_BAD);

    if (slotID != 1) {
        return CKR_SLOT_ID_INVALID;
    }

    memset(&pInfo->slotDescription[0], ' ', sizeof(pInfo->slotDescription));
    memset(&pInfo->manufacturerID[0], ' ', sizeof(pInfo->manufacturerID));
    pInfo->flags                 = CKF_TOKEN_PRESENT | CKF_REMOVABLE_DEVICE | CKF_HW_SLOT;
    pInfo->hardwareVersion.major = APPLET_SE050_VER_MAJOR;
    pInfo->hardwareVersion.minor = APPLET_SE050_VER_MINOR;
    CK_VERSION libVersion        = PKCS11_LIBRARY_VERSION;
    memcpy(&pInfo->firmwareVersion, &libVersion, sizeof(CK_VERSION));
    memcpy(&pInfo->manufacturerID[0], "NXP", sizeof("NXP") - 1);
    return CKR_OK;
}

/**
 * @brief Obtains information about a particular token in the system.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetTokenInfo)
(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo)
{
    AX_UNUSED_ARG(slotID);
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(slotID == pkcs11SLOT_ID, CKR_SLOT_ID_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pInfo != NULL, CKR_ARGUMENTS_BAD);

    CK_TOKEN_INFO tokenInfo      = {0};
    unsigned char label[]        = PKCS11_TOKEN_LABEL;
    unsigned char manufacturer[] = PKCS11_MANUFACTURER;
    CK_VERSION libVersion        = PKCS11_LIBRARY_VERSION;

    memset(tokenInfo.label, ' ', sizeof(tokenInfo.label));
    memset(tokenInfo.manufacturerID, ' ', sizeof(tokenInfo.manufacturerID));
    memset(tokenInfo.model, ' ', sizeof(tokenInfo.model));
    memset(tokenInfo.serialNumber, ' ', sizeof(tokenInfo.serialNumber));
    memcpy(tokenInfo.label, label, sizeof(label));
    memcpy(tokenInfo.manufacturerID, manufacturer, sizeof(manufacturer));
    tokenInfo.ulMaxSessionCount     = 1;
    tokenInfo.ulMaxRwSessionCount   = 1;
    tokenInfo.ulMaxPinLen           = 10;
    tokenInfo.ulMinPinLen           = 0;
    tokenInfo.hardwareVersion.major = APPLET_SE050_VER_MAJOR;
    tokenInfo.hardwareVersion.minor = APPLET_SE050_VER_MINOR;
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(userType);
    AX_UNUSED_ARG(pPin);
    AX_UNUSED_ARG(ulPinLen);
    LOG_D("%s", __FUNCTION__);
    return CKR_OK;
}

/**
 * @brief Logs a user out from a token.
 */
CK_DEFINE_FUNCTION(CK_RV, C_Logout)(CK_SESSION_HANDLE hSession)
{
    AX_UNUSED_ARG(hSession);
    LOG_D("%s", __FUNCTION__);
    return CKR_OK;
}

/**
 * @brief Mixes additional seed material into the token’s random number generator.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SeedRandom)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen)
{
    AX_UNUSED_ARG(pSeed);
    AX_UNUSED_ARG(ulSeedLen);
    LOG_D("%s", __FUNCTION__);
    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(hSession <= MAX_PKCS11_SESSIONS, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pSeed != NULL, CKR_ARGUMENTS_BAD);
    /* Nothing is done */
    return CKR_OK;
}

/**
 * @brief Finishes a multiple-part signature operation, returning the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
    CK_RV xResult             = CKR_FUNCTION_FAILED;
    sss_status_t status       = kStatus_SSS_Fail;
    sss_object_t object       = {0};
    sss_asymmetric_t asymmCtx = {0};
    sss_algorithm_t algorithm = kAlgorithm_None;
    uint8_t data[64]          = {0};
    size_t dataLen            = sizeof(data);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(pulSignatureLen != NULL, xResult, CKR_ARGUMENTS_BAD);
    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
        CKR_OK == pkcs11_parse_sign_mechanism(pxSessionObj, &algorithm), xResult, CKR_MECHANISM_INVALID);

    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(sss_pkcs11_mutex_lock() == 0, xResult, CKR_CANT_LOCK);
    /* Checking for HMAC and performing MAC operation */
    if ((algorithm == kAlgorithm_SSS_HMAC_SHA1) || (algorithm == kAlgorithm_SSS_HMAC_SHA256) ||
        (algorithm == kAlgorithm_SSS_HMAC_SHA384) || (algorithm == kAlgorithm_SSS_HMAC_SHA512)) {
        LOG_D("MAC using SE05x");
        status = sss_mac_finish(&pxSessionObj->ctx_hmac, pSignature, (size_t *)pulSignatureLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
    }
    else {
        status = sss_digest_finish(&pxSessionObj->digest_ctx, &data[0], &dataLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        ENSURE_OR_GO_EXIT((pxSessionObj->xOperationKeyHandle) <= UINT32_MAX);

        status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status =
            sss_asymmetric_context_init(&asymmCtx, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Sign);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_asymmetric_sign_digest(&asymmCtx, &data[0], dataLen, pSignature, (size_t *)pulSignatureLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
    }

    xResult = CKR_OK;
exit:
    if (pxSessionObj->ctx_hmac.session != NULL) {
        sss_mac_context_free(&pxSessionObj->ctx_hmac);
    }
    if (pxSessionObj->digest_ctx.session != NULL) {
        sss_digest_context_free(&pxSessionObj->digest_ctx);
    }
    if (asymmCtx.session != NULL) {
        sss_asymmetric_context_free(&asymmCtx);
    }
    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Continues a multiple-part signature operation, processing another data part.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignUpdate)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    LOG_D("%s", __FUNCTION__);
    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);
    sss_status_t status;
    sss_algorithm_t algorithm = kAlgorithm_None;
    sss_algorithm_t digest_algorithm;
    size_t chunk        = 0;
    size_t offset       = 0;
    sss_object_t object = {0};

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(pPart != NULL, xResult, CKR_ARGUMENTS_BAD);
    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(sss_pkcs11_mutex_lock() == 0, xResult, CKR_CANT_LOCK);

    /* Check for mechanisms having multistep support */

    if (pxSessionObj->xOperationInProgress != CKM_RSA_PKCS_PSS && pxSessionObj->xOperationInProgress != CKM_ECDSA &&
        pxSessionObj->xOperationInProgress != CKM_RSA_PKCS) {
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            pkcs11_parse_sign_mechanism(pxSessionObj, &algorithm) == CKR_OK, xResult, CKR_MECHANISM_INVALID);

        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            pkcs11_get_digest_algorithm(algorithm, &digest_algorithm) == CKR_OK, xResult, CKR_MECHANISM_INVALID);

        if ((algorithm == kAlgorithm_SSS_HMAC_SHA1) || (algorithm == kAlgorithm_SSS_HMAC_SHA256) ||
            (algorithm == kAlgorithm_SSS_HMAC_SHA384) || (algorithm == kAlgorithm_SSS_HMAC_SHA512)) {
            if (pxSessionObj->ctx_hmac.session == NULL) { /* Avoid re-init of hmac context */
                status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
                ENSURE_OR_GO_EXIT((pxSessionObj->xOperationKeyHandle) <= UINT_MAX);

                status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                status = sss_mac_context_init(
                    &pxSessionObj->ctx_hmac, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Mac);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                status = sss_mac_init(&pxSessionObj->ctx_hmac);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            }
            do {
                chunk = (ulPartLen > PKCS11_MAX_HMAC_INPUT_DATA) ? PKCS11_MAX_HMAC_INPUT_DATA : ulPartLen;

                status = sss_mac_update(&pxSessionObj->ctx_hmac, pPart + offset, chunk);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                ENSURE_OR_GO_EXIT((UINT_MAX - offset) >= chunk);
                offset += chunk;
                ulPartLen -= chunk;
            } while (ulPartLen > 0);
        }
        else {
            if (pxSessionObj->digest_ctx.session == NULL) { /* Avoid re-init of digest context */
#if SSS_HAVE_HOSTCRYPTO_ANY
                status = sss_digest_context_init(&pxSessionObj->digest_ctx,
                    &pex_sss_demo_boot_ctx->host_session,
                    digest_algorithm,
                    kMode_SSS_Digest);
#else
                status = kStatus_SSS_Fail;
#endif
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                status = sss_digest_init(&pxSessionObj->digest_ctx);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            }

            do {
                chunk = (ulPartLen > PKCS11_MAX_DIGEST_INPUT_DATA) ? PKCS11_MAX_DIGEST_INPUT_DATA : ulPartLen;

                status = sss_digest_update(&pxSessionObj->digest_ctx, pPart + offset, chunk);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                ENSURE_OR_GO_EXIT((UINT_MAX - offset) >= chunk);
                offset += chunk;
                ulPartLen -= chunk;
            } while (ulPartLen > 0);
        }
    }
    else {
        LOG_E("Mechanism is unsupported");
        xResult = CKR_MECHANISM_INVALID;
        goto exit;
    }

    xResult = CKR_OK;
exit:
    if (xResult != CKR_OK) {
        if (pxSessionObj->digest_ctx.session != NULL) {
            sss_digest_context_free(&pxSessionObj->digest_ctx);
        }
        if (pxSessionObj->ctx_hmac.session != NULL) {
            sss_mac_context_free(&pxSessionObj->ctx_hmac);
        }
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Begin a digital signature generation session.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pxMechanism, CK_OBJECT_HANDLE xKey)
{
    CK_RV xResult             = CKR_OK;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pxMechanism, CKR_ARGUMENTS_BAD);
    //ENSURE_OR_RETURN_ON_ERROR(pxSession->xOperationInProgress == pkcs11NO_OPERATION, CKR_SESSION_HANDLE_INVALID);

    pxSession->xOperationInProgress = pxMechanism->mechanism;
    pxSession->xOperationKeyHandle  = xKey;
    if (pxMechanism->pParameter) {
        pxSession->mechParameter    = pxMechanism->pParameter;
        pxSession->mechParameterLen = pxMechanism->ulParameterLen;
    }
    else {
        pxSession->mechParameterLen = 0;
    }

    if (pxSession->digest_ctx.session != NULL) {
        sss_digest_context_free(&pxSession->digest_ctx);
    }
    if (pxSession->ctx_hmac.session != NULL) {
        sss_mac_context_free(&pxSession->ctx_hmac);
    }

    return xResult;
}

/**
 * @brief Finishes a multiple-part verification operation and check's the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
    CK_RV xResult             = CKR_FUNCTION_FAILED;
    sss_status_t status       = {0};
    sss_object_t object       = {0};
    sss_asymmetric_t asymmCtx = {0};
    sss_algorithm_t algorithm;
    uint8_t data[64] = {0};
    size_t dataLen   = sizeof(data);

    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);

    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
        pkcs11_parse_sign_mechanism(pxSessionObj, &algorithm) == CKR_OK, xResult, CKR_MECHANISM_INVALID);

    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(sss_pkcs11_mutex_lock() == 0, xResult, CKR_CANT_LOCK);

    /* Checking for HMAC and validating */
    if ((algorithm == kAlgorithm_SSS_HMAC_SHA1) || (algorithm == kAlgorithm_SSS_HMAC_SHA256) ||
        (algorithm == kAlgorithm_SSS_HMAC_SHA384) || (algorithm == kAlgorithm_SSS_HMAC_SHA512)) {
        size_t macLen = ulSignatureLen;

        LOG_D("MAC Verify using SE05x");
        status = sss_mac_finish(&pxSessionObj->ctx_hmac, pSignature, &macLen);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_SIGNATURE_INVALID);
    }
    else {
        /* ECC and RSA */
        status = sss_digest_finish(&pxSessionObj->digest_ctx, &data[0], &dataLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        ENSURE_OR_GO_EXIT((pxSessionObj->xOperationKeyHandle) <= UINT32_MAX);

        status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_asymmetric_context_init(
            &asymmCtx, &pex_sss_demo_boot_ctx->session, &object, algorithm, kMode_SSS_Verify);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_asymmetric_verify_digest(&asymmCtx, &data[0], dataLen, pSignature, ulSignatureLen);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_SIGNATURE_INVALID);
    }

    xResult = CKR_OK;
exit:
    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
    if (pxSessionObj->ctx_hmac.session != NULL) {
        sss_mac_context_free(&pxSessionObj->ctx_hmac);
    }
    if (asymmCtx.session != NULL) {
        sss_asymmetric_context_free(&asymmCtx);
    }
    if (pxSessionObj->digest_ctx.session != NULL) {
        sss_digest_context_free(&pxSessionObj->digest_ctx);
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Continues a multiple-part verification operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyUpdate)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_algorithm_t digest_algorithm;
    size_t chunk        = 0;
    size_t offset       = 0;
    sss_object_t object = {0};

    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSessionObj = prvSessionPointerFromHandle(hSession);

    ENSURE_OR_RETURN_ON_ERROR(pxSessionObj != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pPart != NULL, CKR_ARGUMENTS_BAD);

    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(sss_pkcs11_mutex_lock() == 0, xResult, CKR_CANT_LOCK);

    /* Check for mechanisms having multistep support */
    if (pxSessionObj->xOperationInProgress != CKM_RSA_PKCS_PSS && pxSessionObj->xOperationInProgress != CKM_ECDSA &&
        pxSessionObj->xOperationInProgress != CKM_RSA_PKCS) {
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            CKR_OK == pkcs11_parse_sign_mechanism(pxSessionObj, &algorithm), xResult, CKR_MECHANISM_INVALID);

        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            CKR_OK == pkcs11_get_digest_algorithm(algorithm, &digest_algorithm), xResult, CKR_MECHANISM_INVALID);

        if ((algorithm == kAlgorithm_SSS_HMAC_SHA1) || (algorithm == kAlgorithm_SSS_HMAC_SHA256) ||
            (algorithm == kAlgorithm_SSS_HMAC_SHA384) || (algorithm == kAlgorithm_SSS_HMAC_SHA512)) {
            if (pxSessionObj->ctx_hmac.session == NULL) { /* Avoid re-init of hmac context */
                status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
                ENSURE_OR_GO_EXIT((pxSessionObj->xOperationKeyHandle) <= UINT_MAX);

                status = sss_key_object_get_handle(&object, (uint32_t)pxSessionObj->xOperationKeyHandle);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                status = sss_mac_context_init(&pxSessionObj->ctx_hmac,
                    &pex_sss_demo_boot_ctx->session,
                    &object,
                    algorithm,
                    kMode_SSS_Mac_Validate);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                status = sss_mac_init(&pxSessionObj->ctx_hmac);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            }
            do {
                chunk = (ulPartLen > PKCS11_MAX_HMAC_INPUT_DATA) ? PKCS11_MAX_HMAC_INPUT_DATA : ulPartLen;

                status = sss_mac_update(&pxSessionObj->ctx_hmac, pPart + offset, chunk);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                ENSURE_OR_GO_EXIT((UINT_MAX - offset) >= chunk);
                offset += chunk;
                ulPartLen -= chunk;
            } while (ulPartLen > 0);
        }
        else {
            if (pxSessionObj->digest_ctx.session == NULL) { /* Avoid re-init of digest context */
#if SSS_HAVE_HOSTCRYPTO_ANY
                status = sss_digest_context_init(&pxSessionObj->digest_ctx,
                    &pex_sss_demo_boot_ctx->host_session,
                    digest_algorithm,
                    kMode_SSS_Digest);
#else
                status = kStatus_SSS_Fail;
#endif
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                status = sss_digest_init(&pxSessionObj->digest_ctx);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            }

            do {
                chunk = (ulPartLen > PKCS11_MAX_DIGEST_INPUT_DATA) ? PKCS11_MAX_DIGEST_INPUT_DATA : ulPartLen;

                status = sss_digest_update(&pxSessionObj->digest_ctx, pPart + offset, chunk);
                ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

                if ((UINT_MAX - offset) < chunk) {
                    goto exit;
                }
                offset += chunk;
                ulPartLen -= chunk;
            } while (ulPartLen > 0);
        }
    }
    else {
        LOG_E("Mechanism is unsupported");
        xResult = CKR_MECHANISM_INVALID;
        goto exit;
    }

    xResult = CKR_OK;
exit:
    if (xResult != CKR_OK) {
        pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
        if (pxSessionObj->digest_ctx.session != NULL) {
            sss_digest_context_free(&pxSessionObj->digest_ctx);
        }
        if (pxSessionObj->ctx_hmac.session != NULL) {
            sss_mac_context_free(&pxSessionObj->ctx_hmac);
        }
    }
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Begin a digital signature verification session.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyInit)
(CK_SESSION_HANDLE xSession, CK_MECHANISM_PTR pxMechanism, CK_OBJECT_HANDLE xKey)
{
    CK_RV xResult             = CKR_OK;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pxMechanism, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(pxSession->xOperationInProgress == pkcs11NO_OPERATION, CKR_SESSION_HANDLE_INVALID);

    pxSession->xOperationInProgress = pxMechanism->mechanism;
    pxSession->xOperationKeyHandle  = xKey;

    if (pxMechanism->pParameter) {
        pxSession->mechParameter    = pxMechanism->pParameter;
        pxSession->mechParameterLen = pxMechanism->ulParameterLen;
    }
    else {
        pxSession->mechParameterLen = 0;
    }

    if (pxSession->digest_ctx.session != NULL) {
        sss_digest_context_free(&pxSession->digest_ctx);
    }
    if (pxSession->ctx_hmac.session != NULL) {
        sss_mac_context_free(&pxSession->ctx_hmac);
    }

    return xResult;
}

/**
 * @brief Obtains information about the session.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetSessionInfo)
(CK_SESSION_HANDLE hSession, CK_SESSION_INFO_PTR pInfo)
{
    LOG_D("%s", __FUNCTION__);
    CK_RV xResult            = CKR_SESSION_CLOSED;
    P11SessionPtr_t pSession = NULL;
    CK_FLAGS ro_flags        = CKF_SERIAL_SESSION;

    ENSURE_OR_RETURN_ON_ERROR(NULL != pInfo, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(hSession <= MAX_PKCS11_SESSIONS, CKR_SESSION_HANDLE_INVALID);

    ENSURE_OR_RETURN_ON_ERROR(hSession > 0, CKR_SESSION_HANDLE_INVALID);
    pSession = pkcs11_sessions[hSession - 1];
    ENSURE_OR_RETURN_ON_ERROR(pSession != NULL, CKR_SESSION_HANDLE_INVALID);

    if (sss_pkcs11_mutex_lock() != 0) {
        xResult = CKR_FUNCTION_FAILED;
        return xResult;
    }

    CK_SESSION_INFO session_info = {.slotID = pkcs11SLOT_ID,
        .state         = ((pSession->xFlags == ro_flags) ? CKS_RO_PUBLIC_SESSION : CKS_RW_PUBLIC_SESSION),
        .flags         = pSession->xFlags,
        .ulDeviceError = 0};

#if defined(USE_RTOS) && USE_RTOS == 1
#elif (__GNUC__ && !AX_EMBEDDED)
#else
    session_info.flags = session_info.flags | CKF_SERIAL_SESSION;
#endif

    if (sessionCount) {
        memcpy(pInfo, &session_info, sizeof(CK_SESSION_INFO));
        xResult = CKR_OK;
    }

    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }

    return xResult;
}

// LCOV_EXCL_START
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pMechanism);
    AX_UNUSED_ARG(hWrappingKey);
    AX_UNUSED_ARG(hKey);
    AX_UNUSED_ARG(pWrappedKey);
    AX_UNUSED_ARG(pulWrappedKeyLen);
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes a token along with slotID and Pin.
 */
CK_DEFINE_FUNCTION(CK_RV, C_InitToken)
(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel)
{
    AX_UNUSED_ARG(slotID);
    AX_UNUSED_ARG(pPin);
    AX_UNUSED_ARG(ulPinLen);
    AX_UNUSED_ARG(pLabel);
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes the normal user’s PIN.
 */
CK_DEFINE_FUNCTION(CK_RV, C_InitPIN)
(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pPin);
    AX_UNUSED_ARG(ulPinLen);
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Modifies the PIN of the user that is currently logged in.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SetPIN)
(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewLen)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pOldPin);
    AX_UNUSED_ARG(ulOldLen);
    AX_UNUSED_ARG(pNewPin);
    AX_UNUSED_ARG(ulNewLen);
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Closes all sessions an application has with a token..
 */
CK_DEFINE_FUNCTION(CK_RV, C_CloseAllSessions)(CK_SLOT_ID slotID)
{
    LOG_D("%s", __FUNCTION__);
    ENSURE_OR_RETURN_ON_ERROR(slotID == pkcs11SLOT_ID, CKR_SLOT_ID_INVALID);
    C_CloseSession(1);
    C_CloseSession(2);
    C_CloseSession(3);
    return CKR_OK;
}

/**
 * @brief Obtains a copy of the cryptographic operations state of a session, encoded as a string of bytes.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetOperationState)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pOperationState, CK_ULONG_PTR pulOperationStateLen)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pOperationState);
    AX_UNUSED_ARG(pulOperationStateLen);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pOperationState);
    AX_UNUSED_ARG(ulOperationStateLen);
    AX_UNUSED_ARG(hEncryptionKey);
    AX_UNUSED_ARG(hAuthenticationKey);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes a signature operation, where the data can be recovered from the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SignRecoverInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pMechanism);
    AX_UNUSED_ARG(hKey);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pData);
    AX_UNUSED_ARG(ulDataLen);
    AX_UNUSED_ARG(pSignature);
    AX_UNUSED_ARG(pulSignatureLen);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Initializes a signature verification operation, where the data is recovered from the signature.
 */
CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecoverInit)
(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pMechanism);
    AX_UNUSED_ARG(hKey);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pSignature);
    AX_UNUSED_ARG(ulSignatureLen);
    AX_UNUSED_ARG(pData);
    AX_UNUSED_ARG(pulDataLen);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pPart);
    AX_UNUSED_ARG(ulPartLen);
    AX_UNUSED_ARG(pEncryptedPart);
    AX_UNUSED_ARG(pulEncryptedPartLen);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pEncryptedPart);
    AX_UNUSED_ARG(ulEncryptedPartLen);
    AX_UNUSED_ARG(pPart);
    AX_UNUSED_ARG(pulPartLen);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pPart);
    AX_UNUSED_ARG(ulPartLen);
    AX_UNUSED_ARG(pEncryptedPart);
    AX_UNUSED_ARG(pulEncryptedPartLen);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pEncryptedPart);
    AX_UNUSED_ARG(ulEncryptedPartLen);
    AX_UNUSED_ARG(pPart);
    AX_UNUSED_ARG(pulPartLen);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Legacy function which always return value CKR_FUNCTION_NOT_PARALLEL.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionStatus)(CK_SESSION_HANDLE hSession)
{
    AX_UNUSED_ARG(hSession);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_PARALLEL;
}

/**
 * @brief Legacy function which always return value CKR_FUNCTION_NOT_PARALLEL.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CancelFunction)(CK_SESSION_HANDLE hSession)
{
    AX_UNUSED_ARG(hSession);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_PARALLEL;
}

/**
 * @brief Waits for a slot event to occur.
 */
CK_DEFINE_FUNCTION(CK_RV, C_WaitForSlotEvent)
(CK_FLAGS flags, CK_SLOT_ID_PTR pSlot, CK_VOID_PTR pReserved)
{
    AX_UNUSED_ARG(flags);
    AX_UNUSED_ARG(pSlot);
    AX_UNUSED_ARG(pReserved);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Finishes a multiple-part decryption operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DecryptFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pLastPart);
    AX_UNUSED_ARG(pulLastPartLen);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pEncryptedPart);
    AX_UNUSED_ARG(ulEncryptedPartLen);
    AX_UNUSED_ARG(pPart);
    AX_UNUSED_ARG(pulPartLen);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Continues a multiple-part message-digesting operation by digesting the value of a secret key.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DigestKey)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(hKey);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pPart);
    AX_UNUSED_ARG(ulPartLen);
    AX_UNUSED_ARG(pEncryptedPart);
    AX_UNUSED_ARG(pulEncryptedPartLen);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Finishes a multiple-part data encryption operation.
 */
CK_DEFINE_FUNCTION(CK_RV, C_EncryptFinal)
(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pLastEncryptedPart);
    AX_UNUSED_ARG(pulLastEncryptedPartLen);
    LOG_D("%s", __FUNCTION__);
    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Modifies the value of one or more attributes of an object.
 */
CK_DEFINE_FUNCTION(CK_RV, C_SetAttributeValue)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(hObject);
    AX_UNUSED_ARG(pTemplate);
    AX_UNUSED_ARG(ulCount);
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
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(pMechanism);
    AX_UNUSED_ARG(hUnwrappingKey);
    AX_UNUSED_ARG(pWrappedKey);
    AX_UNUSED_ARG(ulWrappedKeyLen);
    AX_UNUSED_ARG(pTemplate);
    AX_UNUSED_ARG(ulAttributeCount);
    AX_UNUSED_ARG(phKey);

    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}
// LCOV_EXCL_STOP
