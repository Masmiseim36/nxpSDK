/*
 * Copyright 2021,2024 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ********************** Include files ********************** */
#include "sss_pkcs11_pal.h"

/* ********************** Global variables ********************** */
/* Object Cache should only be used for the platforms which don't have any memory constraints */
#if ENABLE_OBJECT_CACHE
sss_object_t cache_sssObjects[MAX_CACHE_OBJECT];
#endif

/* ********************** Local Defines ********************** */

/**
 * Defines OpenSC NON_REPUDIATION attribute
 */
#define SC_VENDOR_DEFINED 0x4F534300 /* OSC */
// CKA_OPENSC_NON_REPUDIATION for OpenSC 0.17
#define CKA_OPENSC_NON_REPUDIATION_0_17 (CKA_VENDOR_DEFINED | 1UL)
// CKA_OPENSC_NON_REPUDIATION for OpenSC 0.21
#define CKA_OPENSC_NON_REPUDIATION_0_21 (CKA_VENDOR_DEFINED | SC_VENDOR_DEFINED | 1UL)

extern CK_RV pkcs11_read_object_size(uint32_t keyId, uint16_t *keyLen);
extern bool cryptokiInitialized;

/**
 * @brief Free resources attached to an object handle.
 */
CK_DEFINE_FUNCTION(CK_RV, C_DestroyObject)
(CK_SESSION_HANDLE xSession, CK_OBJECT_HANDLE xObject)
{
    LOG_D("%s", __FUNCTION__);

    CK_RV xResult                      = CKR_FUNCTION_FAILED;
    sss_status_t sss_status            = kStatus_SSS_Fail;
    sss_object_t object                = {0};
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)(&(pex_sss_demo_boot_ctx->session));
    SE05x_Result_t IdExists            = kSE05x_Result_NA;
    P11SessionPtr_t pxSession          = prvSessionPointerFromHandle(xSession);

    ENSURE_OR_RETURN_ON_ERROR(NULL != pxSession, CKR_SESSION_HANDLE_INVALID);

    ENSURE_OR_RETURN_ON_ERROR(xObject != 0, CKR_OK);
    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(sss_pkcs11_mutex_lock() == 0, xResult, CKR_CANT_LOCK);
    ENSURE_OR_GO_EXIT(xObject <= UINT32_MAX);

    if (SM_OK == Se05x_API_CheckObjectExists(&se05x_session->s_ctx, (uint32_t)xObject, &IdExists)) {
        if (IdExists == kSE05x_Result_SUCCESS) {
            sss_status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
            ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            sss_status = sss_key_object_get_handle(&object, (uint32_t)xObject);
            ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

            sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &object);
            ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
        }
    }
    else {
        LOG_E("Se05x_API_CheckObjectExists failed !!!");
        goto exit;
    }

    xResult = CKR_OK;
exit:
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Provides import and storage of a single client certificate and
 * associated private key.
 */
CK_DEFINE_FUNCTION(CK_RV, C_CreateObject)
(CK_SESSION_HANDLE xSession, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR pxObject)
{
    CK_RV xResult = CKR_FUNCTION_FAILED;
    LOG_D("%s", __FUNCTION__);
    sss_pkcs11_key_parse_t keyParse = {0};
    U8 buff[4096]                   = {0};
    CK_ULONG Valueindex             = 0;
    uint32_t keyId                  = 0xffffffff;
    CK_ULONG i                      = 0;
    CK_ULONG classIndex             = 0;
    size_t buff_len                 = sizeof(buff);
    CK_ULONG keyidindex;
    CK_ULONG labelIndex   = 0;
    CK_ULONG ecParamIndex = 0;
    CK_ULONG keyTypeIndex = 0;
    CK_BBOOL foundKeyId   = CK_FALSE;
    sss_status_t status;
    sss_cipher_type_t cipherType = kSSS_CipherType_RSA;
    sss_key_part_t keyPart       = kSSS_KeyPart_NONE;
    CK_KEY_TYPE key_type;
    CK_ULONG index;
    sss_object_t tmp_object            = {0};
    sss_se05x_session_t *se05x_session = (sss_se05x_session_t *)(&(pex_sss_demo_boot_ctx->session));
    SE05x_Result_t IdExists            = kSE05x_Result_NA;
    keyParse.pbuff                     = &buff[0];
    size_t keyLen                      = 0;
    sss_rng_context_t sss_rng_ctx      = {0};
    uint8_t randomKey[32]              = {0};
    sss_object_t secretObject          = {0};
    uint8_t ecParam[170]               = {0};
    size_t ecParamLen                  = sizeof(ecParam);
    size_t KeyBitLen                   = 0;

    /*
     * Check parameters.
     */
    ENSURE_OR_RETURN_ON_ERROR(xSession <= MAX_PKCS11_SESSIONS, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(pkcs11CREATEOBJECT_MINIMUM_ATTRIBUTE_COUNT <= ulCount, CKR_TEMPLATE_INCOMPLETE);
    ENSURE_OR_RETURN_ON_ERROR(pxTemplate != NULL, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(pxObject != NULL, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(ulCount != (CK_ULONG)-1, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);

    ENSURE_OR_RETURN_ON_ERROR(
        pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_CLASS, &classIndex) == CKR_OK,
        CKR_TEMPLATE_INCOMPLETE);

    /*Find the key id as it's needed while provisiong keys and certificate*/
    if (pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_SSS_ID, &keyidindex) == CKR_OK) {
        foundKeyId = CK_TRUE;
    }

    /*
     * Handle the object by class.
     */
    switch (*((uint32_t *)pxTemplate[classIndex].pValue)) {
    case CKO_CERTIFICATE:
    case CKO_DATA: {
        ENSURE_OR_GO_EXIT(pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_VALUE, &i) == CKR_OK);

        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(pxTemplate[i].ulValueLen < buff_len, xResult, CKR_HOST_MEMORY);
        memcpy(buff, pxTemplate[i].pValue, pxTemplate[i].ulValueLen);
        buff_len = (size_t)pxTemplate[i].ulValueLen;

        if (0 != pxTemplate[i].ulValueLen) {
            if (!foundKeyId) {
                if (pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_LABEL, &labelIndex) != CKR_OK) {
                    /* CKA_LABEL was not provided. Generate a random keyId */
                    ENSURE_OR_GO_EXIT(pkcs11_label_to_keyId((unsigned char *)"", 0, &keyId) == CKR_OK);
                }
                else {
                    ENSURE_OR_GO_EXIT(
                        pkcs11_label_to_keyId(
                            pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId) == CKR_OK);
                }
            }

            if (*((uint32_t *)pxTemplate[classIndex].pValue) == CKO_CERTIFICATE) {
                ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
                    pkcs11_parse_Cert(&buff[0], buff_len) == 0, xResult, CKR_ARGUMENTS_BAD);
            }
            else {
                //Do nothing for data object. Create binary file.
            }

            status = pkcs11_sss_create_token(&pex_sss_demo_boot_ctx->ks,
                &tmp_object,
                keyId,
                kSSS_KeyPart_Default,
                kSSS_CipherType_Binary,
                buff,
                buff_len,
                buff_len * 8);
            ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(status == kStatus_SSS_Success, xResult, CKR_DEVICE_ERROR);
            *pxObject = keyId;
        }
        break;
    }
    case CKO_PRIVATE_KEY: {
        /* Parses the private key in PEM format and converts it to DER format.
         * This is required because as SE shall require a key pair for storing keys
         */
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            pkcs11_parse_PrivateKey(pxTemplate, ulCount, &Valueindex, &keyParse) == 0, xResult, CKR_ARGUMENTS_BAD);

        if (!foundKeyId) {
            if (pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_LABEL, &labelIndex) != CKR_OK) {
                /* CKA_LABEL was not provided. Generate a random keyId */
                ENSURE_OR_GO_EXIT(pkcs11_label_to_keyId((unsigned char *)"", 0, &keyId) == CKR_OK);
            }
            else {
                ENSURE_OR_GO_EXIT(
                    pkcs11_label_to_keyId(pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId) ==
                    CKR_OK);
            }
        }

        ENSURE_OR_GO_EXIT(
            pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_KEY_TYPE, &keyTypeIndex) == CKR_OK);

        ENSURE_OR_GO_EXIT((pxTemplate[keyTypeIndex].ulValueLen) <= sizeof(key_type));
        memcpy(&key_type, pxTemplate[keyTypeIndex].pValue, pxTemplate[keyTypeIndex].ulValueLen);

        if (key_type == CKK_EC) {
            ENSURE_OR_GO_EXIT(
                pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_EC_PARAMS, &ecParamIndex) == CKR_OK);

            ENSURE_OR_GO_EXIT((pxTemplate[ecParamIndex].ulValueLen) <= ecParamLen);
            memcpy(ecParam, pxTemplate[ecParamIndex].pValue, pxTemplate[ecParamIndex].ulValueLen);
            /* Get the cipher type based on oid */
            ENSURE_OR_GO_EXIT(pkcs11_get_ec_info(&ecParam[0], &KeyBitLen, &cipherType) == CKR_OK);

            keyParse.cipherType = cipherType;
        }

        if ((keyParse.cipherType == kSSS_CipherType_EC_NIST_P) || (keyParse.cipherType == kSSS_CipherType_EC_NIST_K) ||
            (keyParse.cipherType == kSSS_CipherType_EC_BRAINPOOL)) {
            keyPart = kSSS_KeyPart_Private;
        }
        else {
            keyPart = kSSS_KeyPart_Pair;
        }
        ENSURE_OR_GO_EXIT((keyParse.buffLen) <= UINT32_MAX);
        ENSURE_OR_GO_EXIT((keyParse.keyBitLen) <= UINT32_MAX);
        status = pkcs11_sss_create_token(&pex_sss_demo_boot_ctx->ks,
            &tmp_object,
            keyId,
            keyPart,
            keyParse.cipherType,
            keyParse.pbuff,
            keyParse.buffLen,
            keyParse.keyBitLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
        *pxObject = keyId;
        break;
    }
    case CKO_PUBLIC_KEY: {
        /* Parses the public key in PEM format and converts it to DER format. */
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
            pkcs11_parse_PublicKey(pxTemplate, ulCount, &Valueindex, &keyParse) == 0, xResult, CKR_ARGUMENTS_BAD);

        if (!foundKeyId) {
            if (pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_LABEL, &labelIndex) != CKR_OK) {
                /* CKA_LABEL was not provided. Generate a random keyId */
                ENSURE_OR_GO_EXIT(pkcs11_label_to_keyId((unsigned char *)"", 0, &keyId) == CKR_OK);
            }
            else {
                ENSURE_OR_GO_EXIT(
                    pkcs11_label_to_keyId(pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId) ==
                    CKR_OK);
            }
        }
        ENSURE_OR_GO_EXIT(
            pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_KEY_TYPE, &keyTypeIndex) == CKR_OK);

        ENSURE_OR_GO_EXIT((pxTemplate[keyTypeIndex].ulValueLen) <= sizeof(key_type));
        memcpy(&key_type, pxTemplate[keyTypeIndex].pValue, pxTemplate[keyTypeIndex].ulValueLen);

        if (key_type == CKK_EC) {
            ENSURE_OR_GO_EXIT((keyParse.buffLen) <= ecParamLen);
            ecParamLen = keyParse.buffLen;
            memcpy(ecParam, keyParse.pbuff, ecParamLen);
            /* Get the ec params from public key */
            ENSURE_OR_GO_EXIT(pkcs11_ecPublickeyGetEcParams(&ecParam[0], &ecParamLen) == CKR_OK);
            /* Get the cipher type based on oid */
            ENSURE_OR_GO_EXIT(pkcs11_get_ec_info(&ecParam[0], &KeyBitLen, &cipherType) == CKR_OK);

            keyParse.cipherType = cipherType;
        }

        ENSURE_OR_GO_EXIT((keyParse.buffLen) <= UINT32_MAX);
        ENSURE_OR_GO_EXIT((keyParse.keyBitLen) <= UINT32_MAX);
        status = pkcs11_sss_create_token(&pex_sss_demo_boot_ctx->ks,
            &tmp_object,
            keyId,
            kSSS_KeyPart_Public,
            keyParse.cipherType,
            keyParse.pbuff,
            keyParse.buffLen,
            keyParse.keyBitLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
        *pxObject = keyId;
        break;
    }
    case CKO_SECRET_KEY: {
        if (!foundKeyId) {
            if (pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_LABEL, &labelIndex) != CKR_OK) {
                /* CKA_LABEL was not provided. Generate a random keyId */
                ENSURE_OR_GO_EXIT(pkcs11_label_to_keyId((unsigned char *)"", 0, &keyId) == CKR_OK);
            }
            else {
                ENSURE_OR_GO_EXIT(
                    pkcs11_label_to_keyId(pxTemplate[labelIndex].pValue, pxTemplate[labelIndex].ulValueLen, &keyId) ==
                    CKR_OK);
            }
        }
        ENSURE_OR_GO_EXIT(pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_VALUE, &i) == CKR_OK);
        index = 0;
        ENSURE_OR_GO_EXIT(pkcs11_get_attribute_parameter_index(pxTemplate, ulCount, CKA_KEY_TYPE, &index) == CKR_OK);
        memcpy(&key_type, pxTemplate[index].pValue, pxTemplate[index].ulValueLen);
        /* Treating generic keytype as HMAC only */
        if ((key_type == CKK_GENERIC_SECRET) || (key_type == CKK_SHA_1_HMAC) || (key_type == CKK_SHA224_HMAC) ||
            (key_type == CKK_SHA256_HMAC) || (key_type == CKK_SHA384_HMAC) || (key_type == CKK_SHA512_HMAC)) {
            cipherType = kSSS_CipherType_HMAC;
        }
        else if (key_type == CKK_AES) {
            cipherType = kSSS_CipherType_AES;
        }
        else if ((key_type == CKK_DES) || (key_type == CKK_DES3) || (key_type == CKK_DES2)) {
            cipherType = kSSS_CipherType_DES;
        }
        else {
            LOG_E("Key_type 0x%X is not supported", key_type);
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        /* Checking and deleting the key if already present */
        if (SM_OK == Se05x_API_CheckObjectExists(&se05x_session->s_ctx, keyId, &IdExists)) {
            if ((IdExists == kSE05x_Result_SUCCESS) &&
                ((cipherType == kSSS_CipherType_HMAC) || (cipherType == kSSS_CipherType_AES) ||
                    (cipherType == kSSS_CipherType_DES))) {
                LOG_W("Key id 0x%X already exists!!", keyId);
                ENSURE_OR_GO_EXIT(SM_OK == Se05x_API_DeleteSecureObject(&se05x_session->s_ctx, keyId));
            }
        }
        else {
            LOG_E("Se05x_API_CheckObjectExists Failed !!");
            goto exit;
        }

        if (0 != pxTemplate[i].ulValueLen) {
            ENSURE_OR_GO_EXIT((pxTemplate[i].ulValueLen) <= (UINT32_MAX / 8));

            status = pkcs11_sss_create_token(&pex_sss_demo_boot_ctx->ks,
                &secretObject,
                keyId,
                kSSS_KeyPart_Default,
                cipherType,
                (uint8_t *)pxTemplate[i].pValue,
                pxTemplate[i].ulValueLen,
                pxTemplate[i].ulValueLen * 8);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            *pxObject = keyId;
        }
        else { /* Key value is not provided, generate a random key */

            switch (key_type) {
            case CKK_GENERIC_SECRET:
            case CKK_AES:
                keyLen = 16;
                LOG_W("Keylength used:%lu", keyLen);
                break;
            case CKK_DES:
                keyLen = 8;
                LOG_W("Keylength used:%lu", keyLen);
                break;
            case CKK_DES2:
                keyLen = 16;
                LOG_W("Keylength used:%lu", keyLen);
                break;
            case CKK_DES3:
                keyLen = 24;
                LOG_W("Keylength used:%lu", keyLen);
                break;
            default:
                LOG_E("Key_type 0x%X is not supported", key_type);
                xResult = CKR_ARGUMENTS_BAD;
                goto exit;
            }
            /* Generate random data */

            status = sss_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->session);
            if (status != kStatus_SSS_Success) {
                xResult = CKR_DEVICE_ERROR;
                goto exit;
            }
            status = sss_rng_get_random(&sss_rng_ctx, randomKey, keyLen);
            if (status != kStatus_SSS_Success) {
                xResult = CKR_DEVICE_ERROR;
                goto exit;
            }

            /* Import secret key */
            status = pkcs11_sss_create_token(&pex_sss_demo_boot_ctx->ks,
                &secretObject,
                keyId,
                kSSS_KeyPart_Default,
                cipherType,
                randomKey,
                keyLen,
                keyLen * 8);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
            *pxObject = keyId;
        }
        break;
    }
    default:
        goto exit;
    }
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
 * @brief Begin an enumeration sequence for the objects of the specified type.
 */
CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsInit)
(CK_SESSION_HANDLE xSession, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount)
{
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    int classIndex            = 0;
    CK_BBOOL foundClass       = CK_FALSE;
    CK_ULONG i                = 0;
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(cryptokiInitialized == 1, CKR_CRYPTOKI_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);

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
        return CKR_OK;
    }

    for (i = 0; i < ulCount; i++) {
        if (pxTemplate[i].type == CKA_LABEL) {
            LOG_D("Label found \n");
            pxSession->labelPresent = CK_TRUE;
            if (snprintf(pxSession->label, sizeof(pxSession->label), "%s", (char *)pxTemplate[i].pValue) < 0) {
                LOG_E("snprintf error");
                pxSession->labelPresent = CK_FALSE;
                pxSession->labelLen     = 0;
                continue;
            }
            pxSession->labelLen = pxTemplate[i].ulValueLen;
        }
        else if (pxTemplate[i].type == CKA_CLASS) {
            classIndex = i;
            foundClass = CK_TRUE;
        }
        else if (pxTemplate[i].type == CKA_SSS_ID || pxTemplate[i].type == CKA_ID) {
            pxSession->keyIdPresent = CK_TRUE;
            memcpy(pxSession->keyId, pxTemplate[i].pValue, sizeof(pxSession->keyId));
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
{
    CK_RV xResult                  = CKR_FUNCTION_FAILED;
    bool xDone                     = false;
    P11SessionPtr_t pxSession      = prvSessionPointerFromHandle(xSession);
    uint32_t keyId                 = 0x0;
    CK_OBJECT_HANDLE_PTR ckObjects = NULL;
    sss_status_t status            = kStatus_SSS_Fail;

    LOG_D("%s", __FUNCTION__);

    // Check parameters.
    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR((CK_BBOOL)CK_FALSE != pxSession->xFindObjectInit, CKR_OPERATION_NOT_INITIALIZED);
    ENSURE_OR_RETURN_ON_ERROR(!((false == xDone) && (0u == ulMaxObjectCount)), CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(!((false == xDone) && (!pxObject || !pulObjectCount)), CKR_ARGUMENTS_BAD);

    /*
     * Implementation Logic - Load object based on whether label / keyId was passed.
     * If neither was passed while initializing FindObjects operation
     * then we list the objects present on the secure element and filter
     * out based on object type required by the application.
     */

    if (sss_pkcs11_mutex_lock() != 0) {
        return CKR_CANT_LOCK;
    }

    if ((false == xDone) && pxSession->labelPresent) /* If find object is with label */
    {
        sss_object_t object = {0};

        ENSURE_OR_GO_EXIT(pxSession->labelLen != 0);
        if (pxSession->xFindObjectTotalFound == 1) {
            *pulObjectCount = 0;
        }
        else {
            xResult = pkcs11_label_to_keyId((unsigned char *)pxSession->label, pxSession->labelLen, &keyId);
            ENSURE_OR_GO_EXIT(xResult == CKR_OK);

            status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
            if (status != kStatus_SSS_Success) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }

            status = sss_key_object_get_handle(&object, keyId);
            if (status != kStatus_SSS_Success) {
                /* Reset the count so that handle moves to next object */
                *pulObjectCount = 0;
                goto exit;
            }

            *pxObject                        = keyId;
            *pulObjectCount                  = 1;
            pxSession->xFindObjectTotalFound = 1;
#if ENABLE_OBJECT_CACHE
            memcpy(&cache_sssObjects[0], &object, sizeof(cache_sssObjects[0]));
#endif
        }
    }
    else if ((false == xDone) && pxSession->keyIdPresent) /* If find object is with key id */
    {
        keyId               = (uint32_t)((pxSession->keyId[0] << (8 * 3)) | (pxSession->keyId[1] << (8 * 2)) |
                           (pxSession->keyId[2] << (8 * 1)) | (pxSession->keyId[3] << (8 * 0)));
        sss_object_t object = {0};

        if (pxSession->xFindObjectTotalFound == 1) {
            *pulObjectCount = 0;
        }
        else {
            status = sss_key_object_init(&object, &pex_sss_demo_boot_ctx->ks);
            if (status != kStatus_SSS_Success) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }

            status = sss_key_object_get_handle(&object, keyId);
            if (status != kStatus_SSS_Success) {
                /* Reset the count so that handle moves to the next object */
                *pulObjectCount = 0;
                goto exit;
            }

            *pxObject                        = keyId;
            *pulObjectCount                  = 1;
            pxSession->xFindObjectTotalFound = 1;
#if ENABLE_OBJECT_CACHE
            memcpy(&cache_sssObjects[0], &object, sizeof(cache_sssObjects[0]));
#endif
        }
    }
    else {
        static uint32_t object_list[MAX_CACHE_OBJECT] = {0};
        static size_t object_list_size                = MAX_CACHE_OBJECT;
        size_t ulMaxObjectCountTmp                    = ulMaxObjectCount;
        size_t total_objs_reads                       = 0;

        do {
            if (total_objs_reads >= MAX_CACHE_OBJECT) {
                return CKR_FUNCTION_FAILED;
            }

            object_list_size = MAX_CACHE_OBJECT - total_objs_reads;

            smStatus_t sm_status =
                pkcs11_read_id_list(xSession, &object_list[total_objs_reads], &object_list_size, ulMaxObjectCountTmp);
            if (sm_status != SM_OK) {
                *pulObjectCount = 0;
                xDone           = true;
                return CKR_FUNCTION_FAILED;
            }

            total_objs_reads                   = total_objs_reads + object_list_size;
            pxSession->xFindObjectOutputOffset = pxSession->xFindObjectOutputOffset + object_list_size;

            if (object_list_size == 0) { /* No objects in SE */
                break;
            }

            if (total_objs_reads >= ulMaxObjectCount) { /* Max objects requested read */
                break;
            }

            ulMaxObjectCountTmp = ulMaxObjectCountTmp - object_list_size;

        } while (ulMaxObjectCountTmp > 0);

        ckObjects       = (CK_OBJECT_HANDLE_PTR)SSS_MALLOC(sizeof(CK_OBJECT_HANDLE) * total_objs_reads);
        *pulObjectCount = 0;
        ENSURE_OR_GO_EXIT(ckObjects != NULL);
        memset(ckObjects, 0, sizeof(CK_OBJECT_HANDLE) * total_objs_reads);

        for (size_t i = 0; i < total_objs_reads; i++) {
            uint32_t id              = object_list[i];
            sss_object_t *ptr_object = NULL;
            sss_object_t object      = {0};

#if ENABLE_OBJECT_CACHE
            ptr_object = &cache_sssObjects[i];

#else
            ptr_object = &object;
#endif
            status = sss_key_object_init(ptr_object, &pex_sss_demo_boot_ctx->ks);
            if (status != kStatus_SSS_Success) {
                xResult = CKR_FUNCTION_FAILED;
                goto exit;
            }
            status = sss_key_object_get_handle(ptr_object, object_list[i]);
            if (status != kStatus_SSS_Success) {
                /* Reset the count so that handle moves to next object */
                *pulObjectCount = 0;
                goto exit;
            }

            if (pxSession->xFindObjectClass == pkcs11INVALID_OBJECT_CLASS &&
                pxSession->xFindObjectKeyType == pkcs11INVALID_KEY_TYPE) {
                /* For public key attributes */
                memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                (*pulObjectCount)++;
            }
            else if (pxSession->xFindObjectClass != pkcs11INVALID_OBJECT_CLASS &&
                     pxSession->xFindObjectKeyType == pkcs11INVALID_KEY_TYPE) {
                CK_BBOOL isX509Cert = CK_FALSE;
                if (ptr_object->cipherType == kSSS_CipherType_Binary) {
                    isX509Cert = pkcs11_is_X509_certificate(id);
                }
                if ((ptr_object->cipherType == kSSS_CipherType_Binary && isX509Cert == CK_TRUE &&
                        pxSession->xFindObjectClass == CKO_CERTIFICATE) ||
                    (ptr_object->objectType == kSSS_KeyPart_Pair &&
                        (pxSession->xFindObjectClass == CKO_PRIVATE_KEY ||
                            pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) ||
                    (ptr_object->objectType == kSSS_KeyPart_Public && pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) {
                    memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                    (*pulObjectCount)++;
                }
            }
            else if (pxSession->xFindObjectClass == pkcs11INVALID_OBJECT_CLASS &&
                     pxSession->xFindObjectKeyType != pkcs11INVALID_KEY_TYPE) {
                if ((ptr_object->cipherType == kSSS_CipherType_AES && pxSession->xFindObjectKeyType == CKK_AES) ||
                    (ptr_object->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES) ||
                    (ptr_object->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES2) ||
                    (ptr_object->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES3) ||
                    (ptr_object->cipherType == kSSS_CipherType_RSA && pxSession->xFindObjectKeyType == CKK_RSA) ||
                    (ptr_object->cipherType == kSSS_CipherType_RSA_CRT && pxSession->xFindObjectKeyType == CKK_RSA) ||
                    (ptr_object->cipherType == kSSS_CipherType_EC_NIST_P && pxSession->xFindObjectKeyType == CKK_EC) ||
                    (ptr_object->cipherType == kSSS_CipherType_EC_NIST_K && pxSession->xFindObjectKeyType == CKK_EC) ||
                    (ptr_object->cipherType == kSSS_CipherType_EC_BRAINPOOL &&
                        pxSession->xFindObjectKeyType == CKK_EC)) {
                    memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                    (*pulObjectCount)++;
                }
            }
            else if (pxSession->xFindObjectClass != pkcs11INVALID_OBJECT_CLASS &&
                     pxSession->xFindObjectKeyType != pkcs11INVALID_KEY_TYPE) {
                CK_BBOOL isX509Cert = CK_FALSE;
                if (ptr_object->cipherType == kSSS_CipherType_Binary) {
                    isX509Cert = pkcs11_is_X509_certificate(id);
                }
                if ((ptr_object->cipherType == kSSS_CipherType_Binary && isX509Cert == CK_TRUE &&
                        pxSession->xFindObjectClass == CKO_CERTIFICATE) ||
                    (ptr_object->objectType == kSSS_KeyPart_Pair &&
                        (pxSession->xFindObjectClass == CKO_PRIVATE_KEY ||
                            pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) ||
                    (ptr_object->objectType == kSSS_KeyPart_Public && pxSession->xFindObjectClass == CKO_PUBLIC_KEY)) {
                    if ((ptr_object->cipherType == kSSS_CipherType_AES && pxSession->xFindObjectKeyType == CKK_AES) ||
                        (ptr_object->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES) ||
                        (ptr_object->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES2) ||
                        (ptr_object->cipherType == kSSS_CipherType_DES && pxSession->xFindObjectKeyType == CKK_DES3) ||
                        (ptr_object->cipherType == kSSS_CipherType_RSA && pxSession->xFindObjectKeyType == CKK_RSA) ||
                        (ptr_object->cipherType == kSSS_CipherType_RSA_CRT &&
                            pxSession->xFindObjectKeyType == CKK_RSA) ||
                        (ptr_object->cipherType == kSSS_CipherType_EC_NIST_P &&
                            pxSession->xFindObjectKeyType == CKK_EC) ||
                        (ptr_object->cipherType == kSSS_CipherType_EC_NIST_K &&
                            pxSession->xFindObjectKeyType == CKK_EC) ||
                        (ptr_object->cipherType == kSSS_CipherType_EC_BRAINPOOL &&
                            pxSession->xFindObjectKeyType == CKK_EC)) {
                        memcpy(&ckObjects[*pulObjectCount], &id, sizeof(id));
                        (*pulObjectCount)++;
                    }
                }
            }
        }

        if (*pulObjectCount > 0) {
            memcpy(pxObject, ckObjects, (sizeof(CK_OBJECT_HANDLE) * (*pulObjectCount)));
            pxSession->xFindObjectTotalFound = pxSession->xFindObjectTotalFound + *pulObjectCount;
        }
    }

    xResult = CKR_OK;
exit:
    if (sss_pkcs11_mutex_unlock() != 0) {
        LOG_E("sss_pkcs11_mutex_unlock failed");
    }
    if (ckObjects) {
        SSS_FREE(ckObjects);
    }
    return xResult;
}

/**
 * @brief Terminate object enumeration.
 */
CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsFinal)(CK_SESSION_HANDLE xSession)
{
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);

    ENSURE_OR_RETURN_ON_ERROR(pxSession != NULL, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR((CK_BBOOL)CK_FALSE != pxSession->xFindObjectInit, CKR_OPERATION_NOT_INITIALIZED);

    LOG_D("%s", __FUNCTION__);

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

    return CKR_OK;
}

/**
 * @brief Create a new object by copying existing object.
 */
// LCOV_EXCL_START
CK_DEFINE_FUNCTION(CK_RV, C_CopyObject)
(CK_SESSION_HANDLE hSession,
    CK_OBJECT_HANDLE hObject,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulCount,
    CK_OBJECT_HANDLE_PTR phNewObject)
{
    AX_UNUSED_ARG(hSession);
    AX_UNUSED_ARG(hObject);
    AX_UNUSED_ARG(pTemplate);
    AX_UNUSED_ARG(ulCount);
    AX_UNUSED_ARG(phNewObject);

    LOG_D("%s", __FUNCTION__);

    return CKR_FUNCTION_NOT_SUPPORTED;
}
// LCOV_EXCL_STOP

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
    CK_RV xResult                        = CKR_FUNCTION_FAILED;
    P11SessionPtr_t pxSession            = prvSessionPointerFromHandle(hSession);
    sss_status_t sss_status              = kStatus_SSS_Fail;
    sss_rng_context_t sss_rng_ctx        = {0};
    uint32_t keyId                       = 0x0;
    size_t keyLen                        = 0;
    sss_cipher_type_t cipherType         = kSSS_CipherType_NONE;
    CK_ULONG attributeIndex              = 0;
    CK_MECHANISM mech                    = {0};
    uint8_t randomKey[64]                = {0};
    sss_object_t sss_object              = {0};
    sss_se05x_session_t *se05x_session   = (sss_se05x_session_t *)(&(pex_sss_demo_boot_ctx->session));
    SE05x_Result_t IdExists              = kSE05x_Result_NA;
    uint8_t keyIdBuff[MAX_KEY_ID_LENGTH] = {0};

    AX_UNUSED_ARG(hSession);
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(NULL != pxSession, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pMechanism, CKR_MECHANISM_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pTemplate, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(NULL != phKey, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);

    mech = *pMechanism;
    if (mech.mechanism == CKM_AES_KEY_GEN) {
        ENSURE_OR_GO_EXIT(
            pkcs11_get_attribute_parameter_index(pTemplate, ulCount, CKA_VALUE_LEN, &attributeIndex) == CKR_OK);

        keyLen = *((size_t *)pTemplate[attributeIndex].pValue);
        if ((keyLen != 16) && (keyLen != 24) && (keyLen != 32)) {
            LOG_E("Unsupported key length %d", keyLen);
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        cipherType = kSSS_CipherType_AES;
    }
    else if (mech.mechanism == CKM_DES_KEY_GEN) {
        keyLen     = 8; /* Fixed length for DES key */
        cipherType = kSSS_CipherType_DES;
    }
    else if (mech.mechanism == CKM_DES2_KEY_GEN) {
        keyLen     = 16; /* Fixed length for DES key */
        cipherType = kSSS_CipherType_DES;
    }
    else if (mech.mechanism == CKM_DES3_KEY_GEN) {
        keyLen     = 24; /* Fixed length for DES key */
        cipherType = kSSS_CipherType_DES;
    }
    else if (mech.mechanism == CKM_GENERIC_SECRET_KEY_GEN) {
        ENSURE_OR_GO_EXIT(
            pkcs11_get_attribute_parameter_index(pTemplate, ulCount, CKA_VALUE_LEN, &attributeIndex) == CKR_OK);

        keyLen = *((size_t *)pTemplate[attributeIndex].pValue);
        if ((keyLen != 16) && (keyLen != 24) && (keyLen != 32)) {
            LOG_E("Unsupported key length %d", keyLen);
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }

        cipherType = kSSS_CipherType_HMAC;
    }
    else {
        LOG_E("mechanism is not supported");
        xResult = CKR_ARGUMENTS_BAD;
        goto exit;
    }
    xResult = pkcs11_get_attribute_parameter_index(pTemplate, ulCount, CKA_LABEL, &attributeIndex);
    if (xResult != CKR_OK) {
        /* CKA_LABEL was not provided. Check if CKA_ID was passed */
        xResult = pkcs11_get_attribute_parameter_index(pTemplate, ulCount, CKA_ID, &attributeIndex);
        if (CKR_OK != xResult) {
            /* CKA_ID was also not provided. Generate a random keyId */
            xResult = pkcs11_label_to_keyId((unsigned char *)"", 0, &keyId);
            if (xResult != CKR_OK) {
                return xResult;
            }
        }
        else {
            /* CKA_ID is provided. Use as keyID */
            memcpy(keyIdBuff, pTemplate[attributeIndex].pValue, sizeof(keyIdBuff));
            keyId = (uint32_t)(
                (keyIdBuff[0] << 8 * 3) | (keyIdBuff[1] << 8 * 2) | (keyIdBuff[2] << 8 * 1) | (keyIdBuff[3] << 8 * 0));
        }
    }
    else {
        xResult = pkcs11_label_to_keyId(pTemplate[attributeIndex].pValue, pTemplate[attributeIndex].ulValueLen, &keyId);
        ENSURE_OR_GO_EXIT(xResult == CKR_OK);
    }

    /* Generate random data */

    sss_status = sss_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->session);
    if (sss_status != kStatus_SSS_Success) {
        xResult = CKR_DEVICE_ERROR;
        goto exit;
    }

    sss_status = sss_rng_get_random(&sss_rng_ctx, randomKey, keyLen);
    if (sss_status != kStatus_SSS_Success) {
        xResult = CKR_DEVICE_ERROR;
        goto exit;
    }

    /* Checking and deleting the key if already present */
    if (SM_OK == Se05x_API_CheckObjectExists(&se05x_session->s_ctx, keyId, &IdExists)) {
        if ((IdExists == kSE05x_Result_SUCCESS) &&
            ((cipherType == kSSS_CipherType_HMAC) || (cipherType == kSSS_CipherType_AES) ||
                (cipherType == kSSS_CipherType_DES))) {
            LOG_W("Key id 0x%X already exists!!", keyId);
            ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
                SM_OK == Se05x_API_DeleteSecureObject(&se05x_session->s_ctx, keyId), xResult, CKR_FUNCTION_FAILED);
        }
    }
    else {
        LOG_E("Se05x_API_CheckObjectExists Failed !!");
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    /* Import secret key */
    sss_status = pkcs11_sss_create_token(&pex_sss_demo_boot_ctx->ks,
        &sss_object,
        keyId,
        kSSS_KeyPart_Default,
        cipherType,
        randomKey,
        keyLen,
        keyLen * 8);
    if (sss_status == kStatus_SSS_Success) {
        *phKey = keyId;
    }
    else {
        goto exit;
    }

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
    CK_RV xResult                        = CKR_OK;
    P11SessionPtr_t pxSession            = prvSessionPointerFromHandle(hSession);
    size_t KeyBitLen                     = 0;
    CK_ULONG privateLabelIndex           = 0;
    CK_ULONG publicLabelIndex            = 0;
    uint32_t privKeyId                   = 0;
    uint32_t pubKeyId                    = 0;
    sss_status_t sss_status              = kStatus_SSS_Fail;
    sss_object_t sss_object              = {0};
    CK_BBOOL skipPublicKey               = CK_FALSE;
    sss_se05x_session_t *se05x_session   = (sss_se05x_session_t *)(&(pex_sss_demo_boot_ctx->session));
    SE05x_Result_t IdExists              = kSE05x_Result_NA;
    sss_cipher_type_t cipherType         = kSSS_CipherType_Binary;
    uint8_t keyIdBuff[MAX_KEY_ID_LENGTH] = {0};
    CK_BBOOL ecKeyGen                    = CK_FALSE;
    CK_BBOOL rsaKeyGen                   = CK_FALSE;

    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(NULL != pxSession, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(NULL != phPublicKey, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(NULL != phPrivateKey, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pMechanism, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pPublicKeyTemplate, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(NULL != pPrivateKeyTemplate, CKR_ARGUMENTS_BAD);
    ENSURE_OR_RETURN_ON_ERROR(sss_pkcs11_mutex_lock() == 0, CKR_CANT_LOCK);

    switch (pMechanism->mechanism) {
    case CKM_EC_KEY_PAIR_GEN:
        ecKeyGen = CK_TRUE;
        break;
    case CKM_RSA_PKCS_KEY_PAIR_GEN:
        rsaKeyGen = CK_TRUE;
        break;
    default:
        return CKR_MECHANISM_INVALID;
    }

    if (ecKeyGen) {
        CK_ULONG ec_params_index = 0;
        uint8_t ec_params[40]    = {""};
        xResult                  = pkcs11_get_attribute_parameter_index(
            pPublicKeyTemplate, ulPublicKeyAttributeCount, CKA_EC_PARAMS, &ec_params_index);
        if (xResult != CKR_OK) {
            return xResult;
        }

        if (pPublicKeyTemplate[ec_params_index].ulValueLen > sizeof(ec_params)) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        memcpy(ec_params, pPublicKeyTemplate[ec_params_index].pValue, pPublicKeyTemplate[ec_params_index].ulValueLen);
        /* Get the ciphertype based on passed OID */
        if (pkcs11_get_ec_info(ec_params, &KeyBitLen, &cipherType) == 0) {
            goto cont;
        }

        return CKR_ARGUMENTS_BAD;
    }
    else if (rsaKeyGen) {
        CK_ULONG rsa_params_index = 0;
        cipherType                = kSSS_CipherType_RSA;
        xResult                   = pkcs11_get_attribute_parameter_index(
            pPublicKeyTemplate, ulPublicKeyAttributeCount, CKA_MODULUS_BITS, &rsa_params_index);
        if (xResult != CKR_OK) {
            return xResult;
        }

        CK_ULONG modulusBits = 0;
        memcpy(
            &modulusBits, pPublicKeyTemplate[rsa_params_index].pValue, pPublicKeyTemplate[rsa_params_index].ulValueLen);

        KeyBitLen = (size_t)modulusBits;

        if ((KeyBitLen != 1024) && (KeyBitLen != 2048) && (KeyBitLen != 3072) && (KeyBitLen != 4096) &&
            (KeyBitLen != 512)) {
            return CKR_ARGUMENTS_BAD;
        }
    }

cont:

    xResult = pkcs11_get_attribute_parameter_index(
        pPrivateKeyTemplate, ulPrivateKeyAttributeCount, CKA_LABEL, &privateLabelIndex);
    if (xResult != CKR_OK) {
        /* CKA_LABEL was not provided. Check if CKA_ID was passed */
        xResult = pkcs11_get_attribute_parameter_index(
            pPrivateKeyTemplate, ulPrivateKeyAttributeCount, CKA_ID, &privateLabelIndex);
        if (CKR_OK != xResult) {
            /* CKA_ID was also not provided. Generate a random keyId */
            xResult = pkcs11_label_to_keyId((unsigned char *)"", 0, &privKeyId);
            if (xResult != CKR_OK) {
                return xResult;
            }
        }
        else {
            /* CKA_ID is provided. Use as keyID */
            memcpy(keyIdBuff, pPrivateKeyTemplate[privateLabelIndex].pValue, sizeof(keyIdBuff));
            privKeyId = (uint32_t)(
                (keyIdBuff[0] << 8 * 3) | (keyIdBuff[1] << 8 * 2) | (keyIdBuff[2] << 8 * 1) | (keyIdBuff[3] << 8 * 0));
        }
    }
    else {
        xResult = pkcs11_label_to_keyId(pPrivateKeyTemplate[privateLabelIndex].pValue,
            pPrivateKeyTemplate[privateLabelIndex].ulValueLen,
            &privKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }

    xResult = pkcs11_get_attribute_parameter_index(
        pPublicKeyTemplate, ulPublicKeyAttributeCount, CKA_LABEL, &publicLabelIndex);
    if (xResult != CKR_OK) {
        /* CKA_LABEL was not provided. Check if CKA_ID was passed */
        xResult = pkcs11_get_attribute_parameter_index(
            pPrivateKeyTemplate, ulPrivateKeyAttributeCount, CKA_ID, &privateLabelIndex);
        if (CKR_OK != xResult) {
            /* CKA_ID was also not provided. Generate a random keyId */
            xResult = pkcs11_label_to_keyId((unsigned char *)"", 0, &pubKeyId);
            if (xResult != CKR_OK) {
                return xResult;
            }
        }
        else {
            /* CKA_ID is provided. Use as keyID */
            memcpy(keyIdBuff, pPrivateKeyTemplate[privateLabelIndex].pValue, sizeof(keyIdBuff));
            pubKeyId = (uint32_t)(
                (keyIdBuff[0] << 8 * 3) | (keyIdBuff[1] << 8 * 2) | (keyIdBuff[2] << 8 * 1) | (keyIdBuff[3] << 8 * 0));
        }
    }
    else {
        xResult = pkcs11_label_to_keyId(
            pPublicKeyTemplate[publicLabelIndex].pValue, pPublicKeyTemplate[publicLabelIndex].ulValueLen, &pubKeyId);
        if (xResult != CKR_OK) {
            return xResult;
        }
    }
    /* Checking and deleting the key if already present */

    if (SM_OK == Se05x_API_CheckObjectExists(&se05x_session->s_ctx, privKeyId, &IdExists)) {
        if ((IdExists == kSE05x_Result_SUCCESS) && (ecKeyGen || rsaKeyGen)) {
            LOG_W("Key id 0x%X already exists!!", privKeyId);
            if (SM_OK != Se05x_API_DeleteSecureObject(&se05x_session->s_ctx, privKeyId)) {
                LOG_E("Se05x_API_DeleteSecureObject Failed !!");
                xResult = CKR_FUNCTION_FAILED;
                return xResult;
            }
            else {
                LOG_D("Successfully deleted the key!!");
            }
        }
    }
    else {
        LOG_E("Se05x_API_CheckObjectExists Failed !!");
        xResult = CKR_FUNCTION_FAILED;
        return xResult;
    }

    if (pubKeyId == privKeyId) {
        skipPublicKey = CK_TRUE;
    }

    sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
    if (sss_status != kStatus_SSS_Success) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    sss_status = sss_key_object_allocate_handle(
        &sss_object, privKeyId, kSSS_KeyPart_Pair, cipherType, KeyBitLen * 8, kKeyObject_Mode_Persistent);
    if (sss_status != kStatus_SSS_Success) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    sss_status = sss_key_store_generate_key(&pex_sss_demo_boot_ctx->ks, &sss_object, KeyBitLen, NULL);
    if (sss_status != kStatus_SSS_Success) {
        xResult = CKR_FUNCTION_FAILED;
        goto exit;
    }

    if (!skipPublicKey) {
        uint8_t public[2048] = {0};
        size_t public_len    = sizeof(public);

        sss_status = sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, public, &public_len, &KeyBitLen);
        if (sss_status != kStatus_SSS_Success) {
            sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
            xResult    = CKR_FUNCTION_FAILED;
            goto exit;
        }

        sss_object_t sss_pub_object = {0};

        sss_status = sss_key_object_init(&sss_pub_object, &pex_sss_demo_boot_ctx->ks);
        if (sss_status != kStatus_SSS_Success) {
            sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
            xResult    = CKR_FUNCTION_FAILED;
            goto exit;
        }

        sss_status = sss_key_object_allocate_handle(
            &sss_pub_object, pubKeyId, kSSS_KeyPart_Public, cipherType, KeyBitLen * 8, kKeyObject_Mode_Persistent);
        if (sss_status != kStatus_SSS_Success) {
            sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
            xResult    = CKR_FUNCTION_FAILED;
            goto exit;
        }

        sss_status =
            sss_key_store_set_key(&pex_sss_demo_boot_ctx->ks, &sss_pub_object, public, public_len, KeyBitLen, NULL, 0);
        if (sss_status != kStatus_SSS_Success) {
            sss_status = sss_key_store_erase_key(&pex_sss_demo_boot_ctx->ks, &sss_object);
            xResult    = CKR_FUNCTION_FAILED;
            goto exit;
        }
    }
    else {
        pubKeyId = privKeyId;
    }

    *phPublicKey  = pubKeyId;
    *phPrivateKey = privKeyId;

exit:
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}

/**
 * @brief Obtains the size of an object in bytes.
 */
// LCOV_EXCL_START
CK_DEFINE_FUNCTION(CK_RV, C_GetObjectSize)
(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize)
{
    uint16_t keyLen           = 0;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(hSession);
    sss_status_t status       = kStatus_SSS_Fail;
    sss_object_t obj          = {0};
    LOG_D("%s", __FUNCTION__);

    ENSURE_OR_RETURN_ON_ERROR(NULL != pxSession, CKR_SESSION_HANDLE_INVALID);
    ENSURE_OR_RETURN_ON_ERROR(pulSize != NULL, CKR_ARGUMENTS_BAD);

    status = pkcs11_get_validated_sss_object(pxSession, hObject, &obj);
    if (status != kStatus_SSS_Success) {
        pxSession->xOperationInProgress = pkcs11NO_OPERATION;
        return CKR_OBJECT_HANDLE_INVALID;
    }

    if (CKR_OK != pkcs11_read_object_size((uint32_t)hObject, &keyLen)) {
        /* Auth objects cannot be read. Assigning them them to 0 */
        keyLen = 0;
    }
    *pulSize = keyLen;
    return CKR_OK;
}
// LCOV_EXCL_STOP

/**
 * @brief Query the value of the specified cryptographic object attribute.
 */
CK_DEFINE_FUNCTION(CK_RV, C_GetAttributeValue)
(CK_SESSION_HANDLE xSession, CK_OBJECT_HANDLE xObject, CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount)
{
    CK_RV xResult             = CKR_OK;
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    CK_VOID_PTR pvAttr        = NULL;
    CK_ULONG ulAttrLength     = 0;
    CK_ULONG xP11KeyType, iAttrib, objectClass;
    CK_BBOOL supported = CK_FALSE;
    CK_HW_FEATURE_TYPE hwFeatureType;

    LOG_D("%s", __FUNCTION__);

    if (pxSession == NULL) {
        return CKR_SESSION_HANDLE_INVALID;
    }

    if (!pxTemplate) {
        return CKR_ARGUMENTS_BAD;
    }

    if (sss_pkcs11_mutex_lock() != 0) {
        xResult = CKR_CANT_LOCK;
        return xResult;
    }

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
        uint16_t outKeyIndex = 0;
        size_t pubKeyLen;
        char label[80];
        uint32_t keyId                       = 0;
        uint8_t keyIdBuff[MAX_KEY_ID_LENGTH] = {0};
#if SSS_HAVE_SE05X_VER_GTE_07_02
        uint8_t ObjType     = 0x00;
        uint8_t tag         = 0x00;
        uint8_t ecParam[50] = {0};
#endif //SSS_HAVE_SE05X_VER_GTE_07_02
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
            CKM_ECDSA_SHA224,
            CKM_ECDSA_SHA256,
            CKM_ECDSA_SHA384,
            CKM_ECDSA_SHA512,
        };
        CK_MECHANISM_TYPE des_mechanisms[] = {
            /* DES Algorithms  */
            CKM_DES_ECB,
            CKM_DES_CBC,
        };
        CK_MECHANISM_TYPE keygen_mechanisms = 0;
        sss_se05x_session_t *se05x_session  = (sss_se05x_session_t *)(&(pex_sss_demo_boot_ctx->session));
#if SSS_HAVE_SE05X_VER_GTE_07_02
        se05x_object_attribute obj_attr = {0};
#endif

        // LOG_I("Attribute required : 0x%08lx\n", pxTemplate[ iAttrib ].type);

        switch (pxTemplate[iAttrib].type) {
        /* Common key attributes */
        case CKA_ID: {
            if (kStatus_SSS_Success != pkcs11_get_validated_object_id(pxSession, xObject, keyIdBuff)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }
            pvAttr       = (uint8_t *)&keyIdBuff[0];
            ulAttrLength = sizeof(keyIdBuff);
            break;
        }
        case CKA_CERTIFICATE_TYPE: {
            ulAttrLength = sizeof(cert_type);
            pvAttr       = &cert_type;
            break;
        }
        case CKA_LABEL: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            keyId = sss_object.keyId;
            if (snprintf(label, sizeof(label), "sss:%08X", (unsigned int)keyId) < 0) {
                LOG_E("snprintf error");
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }
            ulAttrLength = strlen(label);
            pvAttr       = (char *)&label[0];
            break;
        }
        case CKA_ALWAYS_AUTHENTICATE: {
            supported    = CK_FALSE;
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_TOKEN: {
            supported    = CK_TRUE; /* Object is always on token */
            ulAttrLength = sizeof(supported);
            pvAttr       = &(supported);
            break;
        }
        case CKA_KEY_TYPE: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
            case kSSS_CipherType_EC_BRAINPOOL:
                xP11KeyType = CKK_EC;
                break;
            case kSSS_CipherType_AES:
            case kSSS_CipherType_DES:
                xP11KeyType = CKK_AES;
                break;
            case kSSS_CipherType_HMAC:
                xP11KeyType = CKK_GENERIC_SECRET;
                break;
            default:
                xResult = CKR_ATTRIBUTE_VALUE_INVALID;
                break;
            }

            ulAttrLength = sizeof(xP11KeyType);
            pvAttr       = &xP11KeyType;
            break;
        }
        case CKA_VALUE: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            switch (sss_object.cipherType) {
            case kSSS_CipherType_Binary: {
                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                pvAttr       = (void *)&data[0];
                ulAttrLength = dataLen;
                break;
            }
            case kSSS_CipherType_RSA:
            case kSSS_CipherType_EC_NIST_P:
            case kSSS_CipherType_EC_NIST_K:
            case kSSS_CipherType_EC_BRAINPOOL: {
                if (sss_object.objectType == kSSS_KeyPart_Pair || sss_object.objectType == kSSS_KeyPart_Private) {
                    ulAttrLength = 0;
                    xResult      = CKR_ATTRIBUTE_SENSITIVE;
                    break;
                }
                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                pvAttr       = (void *)&data[0];
                ulAttrLength = dataLen;
                break;
            }
            case kSSS_CipherType_AES:
            case kSSS_CipherType_DES: {
                ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                xResult      = CKR_ATTRIBUTE_SENSITIVE;
                LOG_W("Not allowed to readout Symmetric key value");
                break;
            }
            case kSSS_CipherType_HMAC: {
                ulAttrLength = CK_UNAVAILABLE_INFORMATION;
                xResult      = CKR_ATTRIBUTE_SENSITIVE;
                LOG_W("Not allowed to readout HMAC key value");
                break;
            }
            case kSSS_CipherType_Count: {
                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
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
            }
            default: {
                ulAttrLength = 0;
                xResult      = CKR_ARGUMENTS_BAD;
                break;
            }
            }
            break;
        }
        case CKA_VALUE_LEN: {
            pvAttr       = NULL;
            ulAttrLength = 0;
            xResult      = CKR_FUNCTION_FAILED;
            if (xObject > UINT32_MAX) {
                xResult = CKR_FUNCTION_FAILED;
                break;
            }
            xResult = pkcs11_read_object_size(xObject, &outKeyIndex);
            if (xResult != CKR_OK) {
                break;
            }
            size         = (size_t)outKeyIndex;
            pvAttr       = (void *)&size;
            ulAttrLength = sizeof(size_t);
            break;
        }
        case CKA_MODULUS_BITS:
        case CKA_PRIME_BITS: { /*
            * Key strength size query, handled the same for RSA or ECDSA
            * in this port.
            */
            pvAttr       = NULL;
            ulAttrLength = 0;
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if (((pxTemplate[iAttrib].type == CKA_MODULUS_BITS) && (sss_object.cipherType != kSSS_CipherType_RSA_CRT &&
                                                                       sss_object.cipherType != kSSS_CipherType_RSA)) ||
                ((pxTemplate[iAttrib].type == CKA_PRIME_BITS) &&
                    (sss_object.cipherType != kSSS_CipherType_EC_NIST_P &&
                        sss_object.cipherType != kSSS_CipherType_EC_NIST_K &&
                        sss_object.cipherType != kSSS_CipherType_EC_BRAINPOOL))) {
                xResult = CKR_ARGUMENTS_BAD;
                break;
            }
            xResult = pkcs11_read_object_size(xObject, &outKeyIndex);
            if (xResult != CKR_OK) {
                break;
            }
            size         = (size_t)outKeyIndex * 8;
            pvAttr       = (void *)&size;
            ulAttrLength = sizeof(size_t);
            break;
        }
        case CKA_VENDOR_DEFINED: {
            supported    = CK_FALSE;
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_MODULUS: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            // CKA_MODULUS is only valid for RSA Key. Issue triggered by OpenSSH7.6(SIMW-2669)
            if ((sss_object.cipherType != kSSS_CipherType_RSA) && (sss_object.cipherType != kSSS_CipherType_RSA_CRT)) {
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
        }
        case CKA_PUBLIC_EXPONENT: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            // CKA_MODULUS is only valid for RSA Key. Issue triggered by OpenSSH7.6(SIMW-2669)
            if ((sss_object.cipherType != kSSS_CipherType_RSA) && (sss_object.cipherType != kSSS_CipherType_RSA_CRT)) {
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
        }
        case CKA_EC_PARAMS: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }
            if ((sss_object.cipherType == kSSS_CipherType_EC_NIST_P) ||
                (sss_object.cipherType == kSSS_CipherType_EC_NIST_K) ||
                (sss_object.cipherType == kSSS_CipherType_EC_BRAINPOOL)) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
                if (SM_OK != Se05x_API_ReadObjectAttributes(&se05x_session->s_ctx, sss_object.keyId, data, &dataLen)) {
                    pvAttr       = NULL;
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }
                /* Getting the SecObjType from attributes */
                ObjType = data[4];
                tag     = ASN_TAG_OBJ_IDF;

                switch (ObjType) {
                case kSE05x_SecObjTyp_EC_KEY_PAIR_NIST_P192:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_NIST_P192:
                case kSE05x_SecObjTyp_EC_PUB_KEY_NIST_P192:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP192R1, sizeof(MBEDTLS_OID_EC_GRP_SECP192R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP192R1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP192R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_NIST_P224:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_NIST_P224:
                case kSE05x_SecObjTyp_EC_PUB_KEY_NIST_P224:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP224R1, sizeof(MBEDTLS_OID_EC_GRP_SECP224R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP224R1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP224R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_NIST_P256:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_NIST_P256:
                case kSE05x_SecObjTyp_EC_PUB_KEY_NIST_P256:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP256R1, sizeof(MBEDTLS_OID_EC_GRP_SECP256R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP256R1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP256R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_NIST_P384:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_NIST_P384:
                case kSE05x_SecObjTyp_EC_PUB_KEY_NIST_P384:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP384R1, sizeof(MBEDTLS_OID_EC_GRP_SECP384R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP384R1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP384R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_NIST_P521:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_NIST_P521:
                case kSE05x_SecObjTyp_EC_PUB_KEY_NIST_P521:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP521R1, sizeof(MBEDTLS_OID_EC_GRP_SECP521R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP521R1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP521R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Secp160k1:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Secp160k1:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Secp160k1:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_SECP160K1, sizeof(OID_EC_GRP_SECP160K1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_SECP160K1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_SECP160K1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Secp192k1:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Secp192k1:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Secp192k1:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP192K1, sizeof(MBEDTLS_OID_EC_GRP_SECP192K1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP192K1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP192K1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Secp224k1:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Secp224k1:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Secp224k1:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP224K1, sizeof(MBEDTLS_OID_EC_GRP_SECP224K1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP224K1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP224K1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Secp256k1:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Secp256k1:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Secp256k1:
                    memcpy(
                        &ecParam[2], (uint8_t *)MBEDTLS_OID_EC_GRP_SECP256K1, sizeof(MBEDTLS_OID_EC_GRP_SECP256K1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(MBEDTLS_OID_EC_GRP_SECP256K1) - 1;
                    ulAttrLength = sizeof(MBEDTLS_OID_EC_GRP_SECP256K1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Brainpool160:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Brainpool160:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Brainpool160:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_BP160R1, sizeof(OID_EC_GRP_BP160R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_BP160R1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_BP160R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Brainpool192:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Brainpool192:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Brainpool192:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_BP192R1, sizeof(OID_EC_GRP_BP192R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_BP192R1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_BP192R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Brainpool224:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Brainpool224:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Brainpool224:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_BP224R1, sizeof(OID_EC_GRP_BP224R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_BP224R1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_BP224R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Brainpool256:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Brainpool256:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Brainpool256:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_BP256R1, sizeof(OID_EC_GRP_BP256R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_BP256R1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_BP256R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Brainpool320:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Brainpool320:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Brainpool320:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_BP320R1, sizeof(OID_EC_GRP_BP320R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_BP320R1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_BP320R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Brainpool384:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Brainpool384:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Brainpool384:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_BP384R1, sizeof(OID_EC_GRP_BP384R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_BP384R1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_BP384R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                case kSE05x_SecObjTyp_EC_KEY_PAIR_Brainpool512:
                case kSE05x_SecObjTyp_EC_PRIV_KEY_Brainpool512:
                case kSE05x_SecObjTyp_EC_PUB_KEY_Brainpool512:
                    memcpy(&ecParam[2], (uint8_t *)OID_EC_GRP_BP512R1, sizeof(OID_EC_GRP_BP512R1) - 1);
                    ecParam[0]   = tag;
                    ecParam[1]   = sizeof(OID_EC_GRP_BP512R1) - 1;
                    ulAttrLength = sizeof(OID_EC_GRP_BP512R1) + 1;
                    pvAttr       = &ecParam[0];
                    break;
                default:
                    if (kStatus_SSS_Success !=
                        sss_key_store_get_key(
                            &pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                        ulAttrLength = 0;
                        xResult      = CKR_FUNCTION_FAILED;
                        break;
                    }
                    xResult      = pkcs11_ecPublickeyGetEcParams(&data[0], &dataLen);
                    ulAttrLength = dataLen;
                    pvAttr       = &data[0];
                    break;
                }
#else
                LOG_E("Curve type not supported hence unable to get EC Params");
                pvAttr       = NULL;
                ulAttrLength = 0;
                xResult      = CKR_DEVICE_ERROR;

#endif //SSS_HAVE_SE05X_VER_GTE_07_02
            }
            else {
                if (kStatus_SSS_Success !=
                    sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
                    ulAttrLength = 0;
                    xResult      = CKR_FUNCTION_FAILED;
                    break;
                }

                xResult      = pkcs11_ecPublickeyGetEcParams(&data[0], &dataLen);
                ulAttrLength = dataLen;
                pvAttr       = &data[0];
            }

            break;
        }
        case CKA_EC_POINT: {
            KeyBitLen = 256;
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
        }
        case CKA_CLASS: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                    isX509Cert          = pkcs11_is_X509_certificate(sss_object.keyId);

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
        }
        case CKA_HW_FEATURE_TYPE: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if ((sss_object.objectType == kSSS_KeyPart_Default) && (sss_object.cipherType == kSSS_CipherType_Count)) {
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
        }
        case CKA_ENCRYPT: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if (sss_object.objectType == kSSS_KeyPart_Public) {
                if (sss_object.cipherType == kSSS_CipherType_RSA_CRT || sss_object.cipherType == kSSS_CipherType_RSA) {
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
        }
        case CKA_DECRYPT: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if (sss_object.objectType == kSSS_KeyPart_Private || sss_object.objectType == kSSS_KeyPart_Pair) {
                if (sss_object.cipherType == kSSS_CipherType_RSA_CRT || sss_object.cipherType == kSSS_CipherType_RSA) {
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
        }
        case CKA_SIGN: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if (sss_object.objectType == kSSS_KeyPart_Private || sss_object.objectType == kSSS_KeyPart_Pair ||
                sss_object.objectType == kSSS_KeyPart_Default) {
                if (sss_object.cipherType == kSSS_CipherType_RSA_CRT || sss_object.cipherType == kSSS_CipherType_RSA ||
                    sss_object.cipherType == kSSS_CipherType_EC_NIST_P ||
                    sss_object.cipherType == kSSS_CipherType_HMAC) {
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
        }
        case CKA_VERIFY: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if (sss_object.objectType == kSSS_KeyPart_Public || sss_object.objectType == kSSS_KeyPart_Default) {
                if (sss_object.cipherType == kSSS_CipherType_RSA_CRT || sss_object.cipherType == kSSS_CipherType_RSA ||
                    sss_object.cipherType == kSSS_CipherType_EC_NIST_P ||
                    sss_object.cipherType == kSSS_CipherType_HMAC) {
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
        }
        case CKA_WRAP:
        case CKA_UNWRAP:
        case CKA_SIGN_RECOVER:
        case CKA_VERIFY_RECOVER: {
            supported    = CK_FALSE;
            ulAttrLength = sizeof(supported);
            pvAttr       = &(supported);
            break;
        }
        case CKA_DERIVE: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
        }
        case CKA_HASH_OF_SUBJECT_PUBLIC_KEY:
        case CKA_HASH_OF_ISSUER_PUBLIC_KEY:
        case CKA_SUBJECT: {
            ulAttrLength = sizeof(data);
            if (xObject > UINT32_MAX) {
                pvAttr       = NULL;
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }
            xResult = pkcs11_parse_certificate_get_attribute(
                (uint32_t)xObject, pxTemplate[iAttrib].type, &data[0], &ulAttrLength);
            if (xResult != CKR_OK) {
                pvAttr       = NULL;
                ulAttrLength = 0;
            }
            else {
                pvAttr = &data[0];
            }
            break;
        }
        case CKA_OPENSC_NON_REPUDIATION_0_17:
        case CKA_OPENSC_NON_REPUDIATION_0_21: {
            // Not support NON-REPUDIATION signature
            supported    = CK_FALSE;
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_SENSITIVE:
        case CKA_ALWAYS_SENSITIVE: {
            supported = CK_FALSE;

            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                    supported = CK_FALSE;
                }
            }

            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_EXTRACTABLE: {
            supported = CK_TRUE;

            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

#if SSS_HAVE_SE05X_VER_GTE_07_02
            // Get attribute
            if (SM_OK != Se05x_API_ReadObjectAttributes(&se05x_session->s_ctx, sss_object.keyId, data, &dataLen)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if (kStatus_SSS_Success != pkcs11_parse_atrribute(&obj_attr,
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
#endif /* SSS_HAVE_SE05X_VER_GTE_07_02 */
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_NEVER_EXTRACTABLE: {
            // Not NEVER_EXTRACTABLE
            supported    = CK_FALSE;
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_LOCAL: {
#if SSS_HAVE_SE05X_VER_GTE_07_02
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
                pkcs11_parse_atrribute(
                    &obj_attr, data, dataLen, sss_object.objectType, sss_object.cipherType, 0, NULL)) {
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
#endif /* SSS_HAVE_SE05X_VER_GTE_07_02 */
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_ALLOWED_MECHANISMS: {
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
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
        }
        case CKA_APPLICATION:
        case CKA_OBJECT_ID: {
            // CKA_APPLICATION: Description of the application that manages the object (default empty)
            // CKA_VALUE: DER-encoding of the object identifier indicating the data object type (default empty)
            pvAttr       = NULL;
            ulAttrLength = 0;
            break;
        }
        case CKA_MODIFIABLE: {
            supported = CK_TRUE;
            if (kStatus_SSS_Success != pkcs11_get_validated_sss_object(pxSession, xObject, &sss_object)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

#if SSS_HAVE_SE05X_VER_GTE_07_02
            // Get attribute
            if (SM_OK != Se05x_API_ReadObjectAttributes(&se05x_session->s_ctx, sss_object.keyId, data, &dataLen)) {
                ulAttrLength = 0;
                xResult      = CKR_FUNCTION_FAILED;
                break;
            }

            if (kStatus_SSS_Success != pkcs11_parse_atrribute(&obj_attr,
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
#endif /* SSS_HAVE_SE05X_VER_GTE_07_02 */
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_PRIVATE: {
            // When the CKA_PRIVATE attribute is CK_TRUE, a user may not access the object until
            // the user has been authenticated to the token.
            supported    = CK_FALSE;
            pvAttr       = &supported;
            ulAttrLength = sizeof(supported);
            break;
        }
        case CKA_KEY_GEN_MECHANISM: {
            /* Generated key is not local and has no keygen mechanism */
            keygen_mechanisms = CK_UNAVAILABLE_INFORMATION;
            pvAttr            = &keygen_mechanisms;
            ulAttrLength      = sizeof(keygen_mechanisms);
            break;
        }
        default: {
            LOG_W("Attribute required : 0x%08lx\n", pxTemplate[iAttrib].type);
            ulAttrLength    = CK_UNAVAILABLE_INFORMATION;
            infoUnavailable = CK_TRUE;
            xResult         = CKR_ATTRIBUTE_SENSITIVE;
            break;
        }
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
    if (sss_pkcs11_mutex_unlock() != 0) {
        return CKR_FUNCTION_FAILED;
    }
    return xResult;
}
