/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include "sss_pkcs11_pal.h"

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

/**
 * Defines OpenSC NON_REPUDIATION attribute
 */
#define SC_VENDOR_DEFINED 0x4F534300 /* OSC */
// CKA_OPENSC_NON_REPUDIATION for OpenSC 0.17
#define CKA_OPENSC_NON_REPUDIATION_0_17 (CKA_VENDOR_DEFINED | 1UL)
// CKA_OPENSC_NON_REPUDIATION for OpenSC 0.21
#define CKA_OPENSC_NON_REPUDIATION_0_21 (CKA_VENDOR_DEFINED | SC_VENDOR_DEFINED | 1UL)

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

/**
 * @brief Free resources attached to an object handle.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DestroyObject)
(CK_SESSION_HANDLE xSession, CK_OBJECT_HANDLE xObject)
{ /*lint !e9072 It's OK to have different parameter name. */
    (void)(xSession);
    LOG_D("%s", __FUNCTION__);

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

/**
 * @brief Provides import and storage of a single client certificate and
 * associated private key.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CreateObject)
(CK_SESSION_HANDLE xSession, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR pxObject)
{
    /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);

#if SSS_HAVE_SSCP
    HLSE_OBJECT_HANDLE HLSE_Handle_Create_obj = 0x0;
    U16 err                                   = 0;
#endif
    int ret;
    U8 buff[4096] = {0};
    mbedtls_pk_context pk;
    CK_ULONG Valueindex = 0;
    uint32_t keyId      = 0xffffffff;
    CK_ULONG i          = 0;
    CK_ULONG classIndex = 0;
    size_t buff_len     = sizeof(buff);

#if SSS_HAVE_APPLET
    CK_ULONG keyidindex;
    CK_ULONG labelIndex = 0;
    CK_BBOOL foundKeyId = CK_FALSE;
    sss_status_t status;
    sss_cipher_type_t cipherType = kSSS_CipherType_RSA;
    size_t keybitlen             = 0;
#endif

#if SSS_HAVE_APPLET_SE05X_IOT
    CK_KEY_TYPE key_type;
    CK_ULONG index;
    mbedtls_pk_type_t pk_type;
#endif // SSS_HAVE_APPLET_SE05X_IOT

    /* Unreferenced variables.*/
    // ( void ) ( xSession );

    /*
     * Check parameters.
     */

    if ((pkcs11CREATEOBJECT_MINIMUM_ATTRIBUTE_COUNT > ulCount) || (NULL == pxTemplate) || (NULL == pxObject)) {
        return CKR_ARGUMENTS_BAD;
    }
    if (ulCount == (CK_ULONG)-1) {
        return CKR_ARGUMENTS_BAD;
    }

    xResult = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_CLASS, &classIndex);
    if (xResult != CKR_OK) {
        return xResult;
    }

    /*Find the key id as it's needed while provisiong keys and certificate*/
#if SSS_HAVE_APPLET_SE05X_IOT
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
            if (pxTemplate[i].ulValueLen > buff_len) {
                LOG_E("Buffer overflow");
                xResult = CKR_HOST_MEMORY;
                return xResult;
            }
            memcpy(buff, pxTemplate[i].pValue, pxTemplate[i].ulValueLen);
            buff_len = (size_t)pxTemplate[i].ulValueLen;

#if SSS_HAVE_SSCP

            err = HLSE_Create_token(
                keyId, HLSE_CERTIFICATE, pxTemplate[i].pValue, pxTemplate[i].ulValueLen, HLSE_Handle_Create_obj);

            if (err != HLSE_SW_OK) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            pxObject = (CK_OBJECT_HANDLE_PTR)&HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_APPLET_SE05X_IOT

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

#if SSS_HAVE_SSCP

            err = HLSE_Create_token(keyId, HLSE_KEY_PAIR, buff, buff_len, HLSE_Handle_Create_obj);
            if (err != HLSE_SW_OK) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            pxObject = (CK_OBJECT_HANDLE_PTR)&HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_APPLET_SE05X_IOT

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

            key_type = CKK_RSA;
            index    = 0;
            xResult  = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_KEY_TYPE, &index);
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
                                           // Currently only for NIST-P curves
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

#if SSS_HAVE_SSCP

            err = HLSE_Create_token(keyId, HLSE_PUBLIC_KEY, buff, buff_len, HLSE_Handle_Create_obj);
            if (err != HLSE_SW_OK) {
                xResult = CKR_DEVICE_ERROR;
                return xResult;
            }

            pxObject = (CK_OBJECT_HANDLE_PTR)&HLSE_Handle_Create_obj;
#endif

#if SSS_HAVE_APPLET_SE05X_IOT
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

            pk_type = MBEDTLS_PK_NONE;
            pk_type = pk.pk_info->type;
            mbedtls_rsa_context *pk_rsa;
            mbedtls_ecp_keypair *pk_ec;
            switch (pk_type) {
            case MBEDTLS_PK_RSA:
                cipherType = kSSS_CipherType_RSA;
                pk_rsa     = mbedtls_pk_rsa(pk);
                keybitlen  = pk_rsa->len * 8;
                break;
            case MBEDTLS_PK_ECKEY:
            case MBEDTLS_PK_ECDSA:
                cipherType = kSSS_CipherType_EC_NIST_P;
                pk_ec      = mbedtls_pk_ec(pk);
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

#if SSS_HAVE_SSCP
            xResult = CKR_ARGUMENTS_BAD;
#endif
#if SSS_HAVE_APPLET_SE05X_IOT

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
 * @brief Begin an enumeration sequence for the objects of the specified type.
 */
CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsInit)
(CK_SESSION_HANDLE xSession, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount)
{ /*lint !e9072 It's OK to have different parameter name. */
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    LOG_D("%s", __FUNCTION__);

    // ( void ) ( ulCount );

    if (pxSession == NULL) {
        return CKR_SESSION_HANDLE_INVALID;
    }
    /*
     * Allow filtering on a single object class attribute.
     */
    pxSession->xFindObjectInit    = CK_TRUE;
    pxSession->xFindObjectClass   = pkcs11INVALID_OBJECT_CLASS; /* Invalid Class */
    pxSession->xFindObjectKeyType = pkcs11INVALID_KEY_TYPE;     /* Invalid Key Type */

    if (!pxTemplate) {
        pxSession->labelPresent          = CK_FALSE;
        pxSession->keyIdPresent          = CK_FALSE;
        pxSession->xFindObjectClass      = pkcs11INVALID_OBJECT_CLASS; /* Invalid Class */
        pxSession->xFindObjectKeyType    = pkcs11INVALID_KEY_TYPE;     /* Invalid Key Type */
        pxSession->xFindObjectTotalFound = 0;
        pxSession->xFindObjectInit       = CK_FALSE;
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
            pxSession->labelLen = pxTemplate[i].ulValueLen;
        }
        else if (pxTemplate[i].type == CKA_CLASS) {
            classIndex = i;
            foundClass = CK_TRUE;
        }
        else if (pxTemplate[i].type == CKA_SSS_ID || pxTemplate[i].type == CKA_ID) {
            pxSession->keyIdPresent = CK_TRUE;
            pxSession->keyId        = *((uint32_t *)(pxTemplate[i].pValue));
        }
        else if (pxTemplate[i].type == CKA_KEY_TYPE) {
            memcpy(&pxSession->xFindObjectKeyType, pxTemplate[i].pValue, sizeof(CK_KEY_TYPE));
        }
    }
    if (foundClass) {
        memcpy(&pxSession->xFindObjectClass, pxTemplate[classIndex].pValue, sizeof(CK_OBJECT_CLASS));
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
    LOG_D("%s", __FUNCTION__);

    BaseType_t xDone          = pdFALSE;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    uint32_t keyId            = 0x0;

    /*
     * Check parameters.
     */

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

    if ((CK_BBOOL)CK_FALSE == pxSession->xFindObjectInit) {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
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

    /*
     * Load object based on whether label / keyId was passed.
     * If neither was passed while initializing FindObjects operation
     * then we list the objects present on the secure element and filter
     * out based on object type required by the application.
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

                                pxSession->pCurrentKs = (SwKeyStorePtr_t)SSS_MALLOC(sizeof(SwKeyStore_t));
                                memset(pxSession->pCurrentKs, 0, sizeof(SwKeyStore_t));
                                memcpy(&pxSession->pCurrentKs->SSSObjects[0],
                                    &object,
                                    sizeof(pxSession->pCurrentKs->SSSObjects[0]));
                                pxSession->pCurrentKs->keyIdListLen = 1;
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

                        pxSession->pCurrentKs = (SwKeyStorePtr_t)SSS_MALLOC(sizeof(SwKeyStore_t));
                        memset(pxSession->pCurrentKs, 0, sizeof(SwKeyStore_t));
                        memcpy(&pxSession->pCurrentKs->SSSObjects[0],
                            &object,
                            sizeof(pxSession->pCurrentKs->SSSObjects[0]));
                        pxSession->pCurrentKs->keyIdListLen = 1;
                    }
                }
                UNLOCK_MUTEX_FOR_RTOS
            }
        }

        xDone = pdTRUE;
    }

    else if ((pdFALSE == xDone)) {
#if SSS_HAVE_APPLET_SE05X_IOT
    retry:
        xResult                                            = CKR_OK;
        static uint32_t object_list[USER_MAX_ID_LIST_SIZE] = {0};
        static size_t object_list_size                     = 0;
        if (pxSession->xFindObjectOutputOffset % USER_MAX_ID_LIST_SIZE == 0) {
            memset(object_list, 0, sizeof(object_list));
            object_list_size     = sizeof(object_list) / sizeof(object_list[0]);
            smStatus_t sm_status = read_id_list(xSession, object_list, &object_list_size, ulMaxObjectCount);
            if (sm_status != SM_OK) {
                *pulObjectCount = 0;
                xResult         = CKR_OK;
                xDone           = pdTRUE;
                return xResult;
            }
            /* Read ID List was successful. Update SW Keystore for further operations */
            pxSession->pCurrentKs = (SwKeyStorePtr_t)SSS_MALLOC(sizeof(SwKeyStore_t));
            memset(pxSession->pCurrentKs, 0, sizeof(SwKeyStore_t));
            for (size_t i = 0; i < object_list_size; i++) {
                pxSession->pCurrentKs->keyIdListLen = i + 1;
                sss_object_t object;
                sss_status_t status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
                if (status != kStatus_SSS_Success) {
                    LOG_E("Object init failed. Should not reach here");
                    continue;
                }
                status = sss_key_object_get_handle(&object, object_list[i]);
                if (status != kStatus_SSS_Success) {
                    LOG_E("Object get handle failed for 0x%08X", object_list[i]);
                    continue;
                }

                memcpy(&pxSession->pCurrentKs->SSSObjects[i], &object, sizeof(pxSession->pCurrentKs->SSSObjects[0]));
            }
        }

        size_t i                       = 0;
        CK_OBJECT_HANDLE_PTR ckObjects = (CK_OBJECT_HANDLE_PTR)SSS_MALLOC(sizeof(CK_OBJECT_HANDLE) * ulMaxObjectCount);
        *pulObjectCount                = 0;

        LOCK_MUTEX_FOR_RTOS
        {
            for (i = (pxSession->xFindObjectOutputOffset % USER_MAX_ID_LIST_SIZE);
                 ((i < object_list_size) && (*pulObjectCount < ulMaxObjectCount));
                 i++) {
                uint32_t id = object_list[i];
                sss_object_t *pObject;
                pxSession->xFindObjectOutputOffset++;
                pObject = &pxSession->pCurrentKs->SSSObjects[i];

                if (pObject->keyId == 0) {
                    continue;
                }
                if (pxSession->xFindObjectClass == pkcs11INVALID_OBJECT_CLASS &&
                    pxSession->xFindObjectKeyType == pkcs11INVALID_KEY_TYPE) {
                    memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                    (*pulObjectCount)++;
                }
                else if (pxSession->xFindObjectClass != pkcs11INVALID_OBJECT_CLASS &&
                         pxSession->xFindObjectKeyType == pkcs11INVALID_KEY_TYPE) {
                    CK_BBOOL isX509Cert = CK_FALSE;
                    if (pObject->cipherType == kSSS_CipherType_Binary) {
                        isX509Cert = isX509Certificate(id);
                    }
                    if ((pObject->cipherType == kSSS_CipherType_Binary && isX509Cert == CK_TRUE &&
                            pxSession->xFindObjectClass == CKO_CERTIFICATE) ||
                        (pObject->objectType == kSSS_KeyPart_Pair && pxSession->xFindObjectClass == CKO_PRIVATE_KEY) ||
                        (pObject->objectType == kSSS_KeyPart_Public && pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) {
                        memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                        (*pulObjectCount)++;
                    }
                }
                else if (pxSession->xFindObjectClass == pkcs11INVALID_OBJECT_CLASS &&
                         pxSession->xFindObjectKeyType != pkcs11INVALID_KEY_TYPE) {
                    if ((pObject->cipherType == kSSS_CipherType_AES && pxSession->xFindObjectKeyType == CKK_AES) ||
                        (pObject->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES) ||
                        (pObject->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES2) ||
                        (pObject->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES3) ||
                        (pObject->cipherType == kSSS_CipherType_RSA && pxSession->xFindObjectKeyType == CKK_RSA) ||
                        (pObject->cipherType == kSSS_CipherType_RSA_CRT && pxSession->xFindObjectKeyType == CKK_RSA) ||
                        (pObject->cipherType == kSSS_CipherType_EC_NIST_P && pxSession->xFindObjectKeyType == CKK_EC)) {
                        memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                        (*pulObjectCount)++;
                    }
                }
                else if (pxSession->xFindObjectClass != pkcs11INVALID_OBJECT_CLASS &&
                         pxSession->xFindObjectKeyType != pkcs11INVALID_KEY_TYPE) {
                    CK_BBOOL isX509Cert = CK_FALSE;
                    if (pObject->cipherType == kSSS_CipherType_Binary) {
                        isX509Cert = isX509Certificate(id);
                    }
                    if ((pObject->cipherType == kSSS_CipherType_Binary && isX509Cert == CK_TRUE &&
                            pxSession->xFindObjectClass == CKO_CERTIFICATE) ||
                        (pObject->objectType == kSSS_KeyPart_Pair && pxSession->xFindObjectClass == CKO_PRIVATE_KEY) ||
                        (pObject->objectType == kSSS_KeyPart_Public && pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) {
                        if ((pObject->cipherType == kSSS_CipherType_AES && pxSession->xFindObjectKeyType == CKK_AES) ||
                            (pObject->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES) ||
                            (pObject->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES2) ||
                            (pObject->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES3) ||
                            (pObject->cipherType == kSSS_CipherType_RSA && pxSession->xFindObjectKeyType == CKK_RSA) ||
                            (pObject->cipherType == kSSS_CipherType_RSA_CRT &&
                                pxSession->xFindObjectKeyType == CKK_RSA) ||
                            (pObject->cipherType == kSSS_CipherType_EC_NIST_P &&
                                pxSession->xFindObjectKeyType == CKK_EC)) {
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
        else if (pxSession->xFindObjectOutputOffset % USER_MAX_ID_LIST_SIZE == 0) {
            if (ckObjects) {
                SSS_FREE(ckObjects);
            }
            goto retry;
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
    LOG_D("%s", __FUNCTION__);

    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    /*
     * Check parameters.
     */

    if (pxSession == NULL) {
        xResult = CKR_SESSION_HANDLE_INVALID;
        return xResult;
    }

    if ((CK_BBOOL)CK_FALSE == pxSession->xFindObjectInit) {
        xResult = CKR_OPERATION_NOT_INITIALIZED;
    }
    else {
        /*
         * Clean-up find objects state.
         */

        pxSession->labelPresent            = CK_FALSE;
        pxSession->keyIdPresent            = CK_FALSE;
        pxSession->xFindObjectInit         = CK_FALSE;
        pxSession->xFindObjectClass        = 0;
        pxSession->xFindObjectTotalFound   = 0;
        pxSession->xFindObjectKeyType      = pkcs11INVALID_KEY_TYPE;
        pxSession->xFindObjectOutputOffset = 0;

        if (NULL != pxSession->pCurrentKs) {
            SSS_FREE(pxSession->pCurrentKs);
            pxSession->pCurrentKs = NULL;
        }
    }

    return xResult;
}

/**
 * @brief Create a new object by copying existing object.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CopyObject)
(CK_SESSION_HANDLE hSession,
    CK_OBJECT_HANDLE hObject,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulCount,
    CK_OBJECT_HANDLE_PTR phNewObject)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Generates a secret key.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GenerateKey)
(CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulCount,
    CK_OBJECT_HANDLE_PTR phKey)
{
    LOG_D("%s", __FUNCTION__);
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

    sss_status_t sss_status = kStatus_SSS_Fail;
    sss_rng_context_t sss_rng_ctx;
    uint32_t keyId               = 0x0;
    size_t keyLen                = 0;
    sss_cipher_type_t cipherType = kSSS_CipherType_NONE;
    CK_ULONG attributeIndex      = 0;
    CK_OBJECT_CLASS ck_object    = pkcs11INVALID_OBJECT_CLASS;
    // LOG_I("Mechanism = %zu", *pMechanism);
    CK_MECHANISM mech       = *pMechanism;
    uint8_t randomKey[64]   = {0};
    sss_object_t sss_object = {0};

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
    sss_status = sss_create_token(&pex_sss_demo_boot_ctx->ks,
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

    return xResult;
    // return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Generates a public-key/private-key pair.
 */
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
    LOG_D("%s", __FUNCTION__);
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
        /* CKA_LABEL was not provided. Check if CKA_ID was passed */
        xResult =
            GetAttributeParameterIndex(pPrivateKeyTemplate, ulPrivateKeyAttributeCount, CKA_ID, &privateLabelIndex);
        if (CKR_OK != xResult) {
            /* CKA_ID was also not provided. Generate a random keyId */
            xResult = LabelToKeyId((unsigned char *)"", 0, &privKeyId);
            if (xResult != CKR_OK) {
                return xResult;
            }
        }
        else {
            /* CKA_ID is provided. Use as keyID */
            memcpy((void *)&privKeyId, pPrivateKeyTemplate[privateLabelIndex].pValue, sizeof(privKeyId));
        }
    }
    else {
        xResult = LabelToKeyId(pPrivateKeyTemplate[privateLabelIndex].pValue,
            pPrivateKeyTemplate[privateLabelIndex].ulValueLen,
            &privKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }

    xResult = GetAttributeParameterIndex(pPublicKeyTemplate, ulPublicKeyAttributeCount, CKA_LABEL, &publicLabelIndex);
    if (xResult != CKR_OK) {
        /* CKA_LABEL was not provided. Check if CKA_ID was passed */
        xResult =
            GetAttributeParameterIndex(pPrivateKeyTemplate, ulPrivateKeyAttributeCount, CKA_ID, &privateLabelIndex);
        if (CKR_OK != xResult) {
            /* CKA_ID was also not provided. Generate a random keyId */
            xResult = LabelToKeyId((unsigned char *)"", 0, &pubKeyId);
            if (xResult != CKR_OK) {
                return xResult;
            }
        }
        else {
            /* CKA_ID is provided. Use as keyID */
            memcpy((void *)&pubKeyId, pPrivateKeyTemplate[privateLabelIndex].pValue, sizeof(pubKeyId));
        }
    }
    else {
        xResult = LabelToKeyId(
            pPublicKeyTemplate[publicLabelIndex].pValue, pPublicKeyTemplate[publicLabelIndex].ulValueLen, &pubKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }

    if (pubKeyId == privKeyId) {
        skipPublicKey = CK_TRUE;
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

/**
 * @brief Obtains the size of an object in bytes.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetObjectSize)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize)
{
    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}

/**
 * @brief Query the value of the specified cryptographic object attribute.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetAttributeValue)
(CK_SESSION_HANDLE xSession, CK_OBJECT_HANDLE xObject, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount)
{ /*lint !e9072 It's OK to have different parameter name. */
    CK_RV xResult = CKR_OK;
    LOG_D("%s", __FUNCTION__);
    // LOG_I("%s : 0x%08X", __FUNCTION__, (uint32_t) xObject);
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    CK_VOID_PTR pvAttr        = NULL;
    CK_ULONG ulAttrLength     = 0;
    CK_ULONG xP11KeyType, iAttrib, objectClass;
    CK_BBOOL supported = CK_FALSE;
    CK_HW_FEATURE_TYPE hwFeatureType;

    if (pxSession == NULL) {
        return CKR_SESSION_HANDLE_INVALID;
    }

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
                if (kStatus_SSS_Success != get_validated_object_id(pxSession, xObject, &keyId)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                pvAttr       = &keyId;
                ulAttrLength = sizeof(keyId);
                break;

            case CKA_CERTIFICATE_TYPE:
                ulAttrLength = sizeof(cert_type);
                pvAttr       = &cert_type;
                break;
            case CKA_LABEL:
                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                keyId = (uint32_t)((((sss_object.keyId >> (8 * 0)) & 0x000000FF) << (8 * 3)) |
                                   (((sss_object.keyId >> (8 * 1)) & 0x000000FF) << (8 * 2)) |
                                   (((sss_object.keyId >> (8 * 2)) & 0x000000FF) << (8 * 1)) |
                                   (((sss_object.keyId >> (8 * 3)) & 0x000000FF) << (8 * 0)));
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
                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                case kSSS_CipherType_EC_NIST_K:
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
                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                pvAttr       = (void *)rsaN;

                break;

            case CKA_PUBLIC_EXPONENT:

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                // Not support NON-REPUDIATION signature
                supported    = CK_FALSE;
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);
                break;

            case CKA_SENSITIVE:
            case CKA_ALWAYS_SENSITIVE:
                supported = CK_FALSE;

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                // Not NEVER_EXTRACTABLE
                supported    = CK_FALSE;
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);
                break;

            case CKA_LOCAL:

#if SSS_HAVE_SE05X_VER_GTE_06_00

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                else {
                    supported = CK_FALSE;
                }
#else
                // SE050 doesn't support ReadObjectAttributes, so use pre-defined value.
                supported = CK_FALSE;
#endif /* SSS_HAVE_SE05X_VER_GTE_06_00 */
                pvAttr       = &supported;
                ulAttrLength = sizeof(supported);

                break;

            case CKA_ALLOWED_MECHANISMS:

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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

                if (kStatus_SSS_Success != get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
