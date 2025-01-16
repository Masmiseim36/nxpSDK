/*
 * Copyright 2021,2024 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ********************** Include files ********************** */
#include "sss_pkcs11_pal.h"

/* ********************** Defines ********************** */
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

/* ********************** Global variables ********************** */
#if ENABLE_OBJECT_CACHE
extern sss_object_t cache_sssObjects[MAX_CACHE_OBJECT];
#endif

/* ********************** Public Functions ********************** */

/** @brief ParseSign Mechanism.
 * This function mapping sign mechanism from PKCS#11.
 *
 * @param pxSession - Pointer to handle PKCS11 session.
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_ARGUMENTS_BAD The arguments supplied to the function are not appropriate.
 * @retval #CKR_MECHANISM_INVALID If unknown mechanism specified.
 *
 */
CK_RV pkcs11_parse_sign_mechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm)
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
        /* Default */
        *algorithm = kAlgorithm_SSS_ECDSA_SHA256;
        break;
    case CKM_ECDSA_SHA1:
        *algorithm = kAlgorithm_SSS_ECDSA_SHA1;
        break;
    case CKM_ECDSA_SHA224:
        *algorithm = kAlgorithm_SSS_ECDSA_SHA224;
        break;
    case CKM_ECDSA_SHA256:
        *algorithm = kAlgorithm_SSS_ECDSA_SHA256;
        break;
    case CKM_ECDSA_SHA384:
        *algorithm = kAlgorithm_SSS_ECDSA_SHA384;
        break;
    case CKM_ECDSA_SHA512:
        *algorithm = kAlgorithm_SSS_ECDSA_SHA512;
        break;
    case CKM_SHA_1_HMAC:
        *algorithm = kAlgorithm_SSS_HMAC_SHA1;
        break;
    case CKM_SHA256_HMAC:
        *algorithm = kAlgorithm_SSS_HMAC_SHA256;
        break;
    case CKM_SHA384_HMAC:
        *algorithm = kAlgorithm_SSS_HMAC_SHA384;
        break;
    case CKM_SHA512_HMAC:
        *algorithm = kAlgorithm_SSS_HMAC_SHA512;
        break;
    default:
        xResult = CKR_MECHANISM_INVALID;
        break;
    }
    return xResult;
}

/** @brief ParseEncryption Mechanism.
 * This function mapping encryption mechanism from PKCS#11.
 *
 * @param pxSession - Pointer to handle PKCS#11 session.
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_ARGUMENTS_BAD The arguments supplied to the function are not appropriate.
 * @retval #CKR_MECHANISM_INVALID If unknown mechanism specified.
 *
 */
CK_RV pkcs11_parse_encryption_mechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm)
{
    CK_RV xResult = CKR_OK;
    CK_RSA_PKCS_OAEP_PARAMS_PTR param;
    switch (pxSession->xOperationInProgress) {
    case CKM_RSA_PKCS:
        *algorithm = kAlgorithm_SSS_RSAES_PKCS1_V1_5;
        break;

    case CKM_RSA_PKCS_OAEP:
        /* Also need the details about hash algorithm used */

        /* Also see how to use source, pSourceData, ulSourceDataLen
         * in RSA_PKCS_OAEP_PARAMS
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
    case CKM_DES_ECB:
        *algorithm = kAlgorithm_SSS_DES_ECB;
        break;
    case CKM_DES_CBC:
        *algorithm = kAlgorithm_SSS_DES_CBC;
        break;
    case CKM_DES3_CBC:
        *algorithm = kAlgorithm_SSS_DES3_CBC;
        break;
    case CKM_DES3_ECB:
        *algorithm = kAlgorithm_SSS_DES3_ECB;
        break;
    default:
        xResult = CKR_MECHANISM_INVALID;
        break;
    }
    return xResult;
}

/** @brief ParseDigest Mechanism.
 *This function mapping digest mechanism from PKCS#11.
 *
 * @param pxSession - Pointer to handle PKCS#11 session.
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_MECHANISM_INVALID If unknown mechanism specified.
 *
 */
CK_RV pkcs11_parse_digest_mechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm)
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

/** @brief Get SSS Algorithm.
 * This function defines the digest algorithm.
 *
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 * @param digest_algo - Algorithm to calculate the digest.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_ARGUMENTS_BAD The arguments supplied to the function are not appropriate.
 *
 */
CK_RV pkcs11_get_digest_algorithm(const sss_algorithm_t algorithm, sss_algorithm_t *digest_algo)
{
    switch (algorithm) {
    case kAlgorithm_SSS_SHA1:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1:
    case kAlgorithm_SSS_ECDSA_SHA1:
    case kAlgorithm_SSS_HMAC_SHA1:
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
    case kAlgorithm_SSS_HMAC_SHA256:
        *digest_algo = kAlgorithm_SSS_SHA256;
        break;
    case kAlgorithm_SSS_SHA384:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA384:
    case kAlgorithm_SSS_ECDSA_SHA384:
    case kAlgorithm_SSS_HMAC_SHA384:
        *digest_algo = kAlgorithm_SSS_SHA384;
        break;
    case kAlgorithm_SSS_SHA512:
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512:
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA512:
    case kAlgorithm_SSS_ECDSA_SHA512:
    case kAlgorithm_SSS_HMAC_SHA512:
        *digest_algo = kAlgorithm_SSS_SHA512;
        break;
    default:
        return CKR_ARGUMENTS_BAD;
    }
    return CKR_OK;
}

/** @brief isX509 Certificate.
 * Helper function to check for certificates at particular keyID.
 *
 * @param xObject - Pointer to handle PKCS#11 object.
 *
 * @returns Status of the operation
 * @retval #CK_FALSE The operation returns zero value.
 */
CK_BBOOL pkcs11_is_X509_certificate(uint32_t xObject)
{
    CK_BBOOL xResult        = CK_FALSE;
    sss_object_t sss_object = {0};
    uint8_t data[2048]      = {0};
    size_t dataLen          = sizeof(data);
    size_t KeyBitLen        = 0;

    /* NOTE: MUTEX LOCK IS NOT USED HERE BECAUSE
    * THIS FUNCTION IS CALLED ONLY WHEN WE HAVE ALREADY LOCKED THE MUTEX
    */

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

    if (0 != pkcs11_parse_Cert(&data[0], dataLen)) {
        return xResult;
    }

    return CK_TRUE;
}

/** @brief Label To KeyId.
 * This function defines the label to the KeyId by using three different ways.
 *
 * @param label - The array containing the label.
 * @param labelSize - The size of the label.
 * @param keyId - Buffer containing generated keyId.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_DEVICE_ERROR If some problem has occured with the token or slot.
 * @retval #CKR_ARGUMENTS_BAD The arguments supplied to the function are not appropriate.
 */
CK_RV pkcs11_label_to_keyId(unsigned char *label, size_t labelSize, uint32_t *keyId)
{
    CK_RV result                  = CKR_FUNCTION_FAILED;
    sss_status_t status           = kStatus_SSS_Fail;
    sss_digest_t digest_ctx       = {0};
    uint8_t digest[64]            = {0};
    size_t digest_size            = sizeof(digest);
    sss_rng_context_t sss_rng_ctx = {0};
    uint8_t rngData[10]           = {0};
    size_t rngDataLen             = sizeof(rngData);

    if (labelSize == 0) {
        LOG_W("CKA_LABEL was not provided so generating a random keyId !!");
        status = sss_rng_context_init(&sss_rng_ctx, &pex_sss_demo_boot_ctx->session /* Session */);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_rng_get_random(&sss_rng_ctx, rngData, rngDataLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        *keyId = (rngData[6] << (3 * 8)) | (rngData[7] << (2 * 8)) | (rngData[8] << (1 * 8)) | (rngData[9] << (0 * 8));
    }
    else if (strncmp((char *)label, "sss:", strlen("sss:")) == 0) {
        char labelCopy[32] = {0};
        memset(labelCopy, '\0', sizeof(labelCopy));
        strncpy(labelCopy, (char *)label, labelSize);
        unsigned long long_id = 0;
        char *id              = (char *)&labelCopy[0];
        long_id               = strtoul(id + 4, NULL, 16);
        if (long_id == 0) {
            LOG_E("Invalid Label : %s", label);
            result = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        *keyId = (uint32_t)(
            (((long_id >> (8 * 0)) & 0x000000FF) << (8 * 0)) | (((long_id >> (8 * 1)) & 0x000000FF) << (8 * 1)) |
            (((long_id >> (8 * 2)) & 0x000000FF) << (8 * 2)) | (((long_id >> (8 * 3)) & 0x000000FF) << (8 * 3)));
    }
    else {
        status = sss_digest_context_init(
            &digest_ctx, &pex_sss_demo_boot_ctx->session, kAlgorithm_SSS_SHA512, kMode_SSS_Digest);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_digest_one_go(&digest_ctx, label, labelSize, &digest[0], &digest_size);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        *keyId = (digest[60] << (3 * 8)) | (digest[61] << (2 * 8)) | (digest[62] << (1 * 8)) | (digest[63] << (0 * 8));
    }

    result = CKR_OK;
exit:
    if (digest_ctx.session != NULL) {
        sss_digest_context_free(&digest_ctx);
    }
    return result;
}

/** @brief parseCertificate GetAttribute.
 * Helper function for parsing the device certificates.
 *
 * @param xObject - Pointer to handle PKCS#11 object.
 * @param attributeType - This identifies the attribute type.
 * @param pData - Buffer containing the input data.
 * @param ulAttrLength - Length of the attribute type.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_DEVICE_ERROR If some problem has occured with the token or slot.
 * @retval #CKR_BUFFER_TOO_SMALL The output of function is too large to fit in supplied buffer.
 * @retval #CKR_ATTRIBUTE_SENSITIVE The value of an attribute of an object which cannot be satisfied because the object is either sensitive or un-extractable.
 */
CK_RV pkcs11_parse_certificate_get_attribute(
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
    size_t i                         = 0;

    /* NOTE: MUTEX LOCK IS NOT USED HERE BECAUSE THIS FUNCTION IS CALLED ONLY WHEN WE HAVE ALREADY LOCKED THE MUTEX */

    if (kStatus_SSS_Success != (sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks))) {
        goto exit;
    }
    if (kStatus_SSS_Success != (sss_key_object_get_handle(&sss_object, xObject))) {
        goto exit;
    }
    if (kStatus_SSS_Success !=
        sss_key_store_get_key(&pex_sss_demo_boot_ctx->ks, &sss_object, &data[0], &dataLen, &KeyBitLen)) {
        goto exit;
    }

    ENSURE_OR_GO_EXIT(pkcs11_parseCert_GetAttr(attributeType, &data[0], dataLen, pData, ulAttrLength) == CKR_OK);

    if ((attributeType == CKA_HASH_OF_ISSUER_PUBLIC_KEY) || (attributeType == CKA_HASH_OF_SUBJECT_PUBLIC_KEY)) {
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

        status =
            sss_digest_context_init(&digestCtx, &pex_sss_demo_boot_ctx->session, digest_algorithm, kMode_SSS_Digest);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        status = sss_digest_init(&digestCtx);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        while (tagLen > 500) {
            status = sss_digest_update(&digestCtx, &pTLV[0 + i * 500], 500);
            ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

            i++;
            tagLen -= 500;
        }
        status = sss_digest_update(&digestCtx, &pTLV[0 + i * 500], tagLen);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

        *ulAttrLength = 20 /* SHA-1 data length */;
        status        = sss_digest_finish(&digestCtx, &pData[0], (size_t *)ulAttrLength);
        ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);
    }

    xResult = CKR_OK;
exit:
    if (digestCtx.session != NULL) {
        sss_digest_context_free(&digestCtx);
    }
    return xResult;
}

#if SSS_HAVE_SE05X_VER_GTE_07_02

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
sss_status_t pkcs11_parse_atrribute(se05x_object_attribute *pAttribute,
    uint8_t *rsp,
    size_t rspLen,
    uint32_t objectType,
    uint32_t cipherType,
    uint32_t policy_map,
    CK_BBOOL *pAllow)
{
    uint32_t i;
    uint32_t policyEnd      = 0;
    uint32_t policyStart    = 14;
    uint8_t policy_len      = 0; /**< Length of policy. */
    uint32_t auth_obj_id    = 0; /**< Authentication object identifier */
    uint32_t ar_header      = 0; /**< AR Header */
    CK_BBOOL found_policy   = CK_FALSE;
    uint32_t default_policy = 0;

    if (rsp == NULL) {
        LOG_E("Incomplete Object Attribute");
        return kStatus_SSS_Fail;
    }

    if (rspLen < 19) {
        // Object attribute should be at least 19 bytes (without policy).
        LOG_E("Incomplete Object Attribute");
        return kStatus_SSS_Fail;
    }
    if ((rspLen - 5) > UINT32_MAX) {
        LOG_E("Invalid Object Attribute");
        return kStatus_SSS_Fail;
    }
    policyEnd = (uint32_t)(rspLen - 5);

    // Parse Attribute
    pAttribute->obj_id    = ((rsp[0] << 8 * 3) | (rsp[1] << 8 * 2) | (rsp[2] << 8 * 1) | (rsp[3]));
    pAttribute->obj_type  = rsp[4];
    pAttribute->auth_attr = (SE05x_SetIndicator_t)rsp[5];
    if (pAttribute->auth_attr == kSE05x_SetIndicator_SET) {
        pAttribute->auth_attempts = ((rsp[6] << 8 * 1) | (rsp[7]));
        pAttribute->max_attempts  = ((rsp[12] << 8 * 1) | (rsp[13]));
    }
    else {
        pAttribute->min_AEAD_tag_len = ((rsp[6] << 8 * 1) | (rsp[7]));
    }
    pAttribute->auth_obj_id = ((rsp[8] << 8 * 3) | (rsp[9] << 8 * 2) | (rsp[10] << 8 * 1) | (rsp[11]));
    pAttribute->origin      = (SE05x_Origin_t)rsp[policyEnd];
    if ((UINT32_MAX - 4) < policyEnd) {
        return kStatus_SSS_Fail;
    }
    pAttribute->version = (rsp[policyEnd + 1] << 8 * 3) | (rsp[policyEnd + 2] << 8 * 2) |
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
            policy_len = rsp[i];
            if (i + 8 < rspLen) {
                auth_obj_id = ((rsp[i + 1] << 8 * 3) | (rsp[i + 2] << 8 * 2) | (rsp[i + 3] << 8 * 1) | (rsp[i + 4]));
                ar_header   = ((rsp[i + 5] << 8 * 3) | (rsp[i + 6] << 8 * 2) | (rsp[i + 7] << 8 * 1) | (rsp[i + 8]));
            }
            else {
                return kStatus_SSS_Fail;
            }
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
            else {
                default_policy = DEFAULT_POLICY_SYMM_KEY;
            }
        }
#ifdef DEBUG_PKCS11_PAL
        LOG_I("Default policy : 0x%02X, Required policy : 0x%02X", default_policy, policy_map);
#endif

        if (default_policy & policy_map) {
            *pAllow = CK_TRUE;
        }
        else {
            *pAllow = CK_FALSE;
        }
    }

    return kStatus_SSS_Success;
}
#endif

/**
 * @brief
*/
smStatus_t pkcs11_read_id_list(
    CK_SESSION_HANDLE xSession, uint32_t *idlist, size_t *idlistlen, CK_ULONG ulMaxObjectCount)
{
    P11SessionPtr_t pxSession = prvSessionPointerFromHandle(xSession);
    uint8_t pmore             = kSE05x_MoreIndicator_NA;
    uint8_t list[1024]        = {0};
    size_t listlen            = sizeof(list);
    size_t i, k = 0;
    smStatus_t retStatus          = SM_NOT_OK;
    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;
    pSe05xSession_t session_ctx   = &pSession->s_ctx;

    /* This condition would be checked by the calling API */
    ENSURE_OR_GO_EXIT(pxSession != NULL);
    ENSURE_OR_GO_EXIT(idlist != NULL);
    ENSURE_OR_GO_EXIT(idlistlen != NULL);

    ENSURE_OR_GO_EXIT(
        Se05x_API_ReadIDList(session_ctx, pxSession->xFindObjectOutputOffset, 0xFF, &pmore, list, &listlen) == SM_OK);

    if (listlen == 0) {
        *idlistlen = 0;
        retStatus  = SM_OK;
        goto exit;
    }

    for (i = 0; (i < listlen) && (k < ulMaxObjectCount); i += 4) {
        uint32_t id = 0 | (list[i + 0] << (3 * 8)) | (list[i + 1] << (2 * 8)) | (list[i + 2] << (1 * 8)) |
                      (list[i + 3] << (0 * 8));
        idlist[k++] = id;
        *idlistlen  = k;
    }

    retStatus = SM_OK;
exit:
    return retStatus;
}

/**
 * @brief
*/
sss_status_t pkcs11_get_validated_object_id(P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, uint8_t *keyIdbuff)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    sss_object_t sss_object = {0};

    if (xObject > UINT32_MAX) {
        LOG_E("Key id cannot be greater than 4 bytes");
        sss_status = kStatus_SSS_Fail;
        goto exit;
    }

    if (pxSession->xFindObjectInit == CK_TRUE) {
        /* Find Objects operation is going on. Read from cache */

#if ENABLE_OBJECT_CACHE
        for (size_t i = 0; i < MAX_CACHE_OBJECT; i++) {
            if (cache_sssObjects[i].keyId == (uint32_t)xObject) {
                /* True */
                ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
                    (UINTPTR_MAX - 4) > (uintptr_t)keyIdbuff, sss_status, kStatus_SSS_Fail);
                *keyIdbuff++ = (uint8_t)((xObject >> 3 * 8) & 0xFF);
                *keyIdbuff++ = (uint8_t)((xObject >> 2 * 8) & 0xFF);
                *keyIdbuff++ = (uint8_t)((xObject >> 1 * 8) & 0xFF);
                *keyIdbuff++ = (uint8_t)((xObject >> 0 * 8) & 0xFF);
                sss_status   = kStatus_SSS_Success;
                break;
            }
        }
#else
        /* Object cache is disabled */
        sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(&sss_object, xObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR((UINTPTR_MAX - 4) > (uintptr_t)keyIdbuff, sss_status, kStatus_SSS_Fail);
        *keyIdbuff++ = (uint8_t)((xObject >> 3 * 8) & 0xFF);
        *keyIdbuff++ = (uint8_t)((xObject >> 2 * 8) & 0xFF);
        *keyIdbuff++ = (uint8_t)((xObject >> 1 * 8) & 0xFF);
        *keyIdbuff++ = (uint8_t)((xObject >> 0 * 8) & 0xFF);

#endif
    }
    else {
        sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(&sss_object, xObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
        ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR((UINTPTR_MAX - 4) > (uintptr_t)keyIdbuff, sss_status, kStatus_SSS_Fail);
        *keyIdbuff++ = (uint8_t)((xObject >> 3 * 8) & 0xFF);
        *keyIdbuff++ = (uint8_t)((xObject >> 2 * 8) & 0xFF);
        *keyIdbuff++ = (uint8_t)((xObject >> 1 * 8) & 0xFF);
        *keyIdbuff++ = (uint8_t)((xObject >> 0 * 8) & 0xFF);
    }
exit:
    if (sss_status != kStatus_SSS_Success) {
        *keyIdbuff = 0;
    }
    return sss_status;
}

/**
 * @brief
*/
sss_status_t pkcs11_get_validated_sss_object(
    P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, sss_object_t *pSSSObject)
{
    sss_status_t sss_status = kStatus_SSS_Fail;

    ENSURE_OR_GO_EXIT(pSSSObject != NULL);

    if (xObject > UINT32_MAX) {
        LOG_E("Key id cannot be greater than 4 bytes");
        sss_status = kStatus_SSS_Fail;
        goto exit;
    }

    if (pxSession->xFindObjectInit == CK_TRUE) {
        /* Find Objects operation is going on. Read from cache */
#if ENABLE_OBJECT_CACHE
        for (size_t i = 0; i < MAX_CACHE_OBJECT; i++) {
            if (cache_sssObjects[i].keyId == (uint32_t)xObject) {
                /* True */
                memcpy(pSSSObject, &cache_sssObjects[i], sizeof(*pSSSObject));
                sss_status = kStatus_SSS_Success;
                break;
            }
        }
#else
        sss_status   = sss_key_object_init(pSSSObject, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(pSSSObject, xObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
#endif
    }
    else {
        sss_status = sss_key_object_init(pSSSObject, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(pSSSObject, xObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
    }
exit:
    return sss_status;
}

/** @brief valid keytype .
 * This function checks if the algorithm is valid for the keytype.
 *
 * @param algorithm - Algorithm, e.g. kAlgorithm_SSS_AES_CBC.
 * @param cipher - cipher to be applied, e.g. kSSS_CipherType_AES.
 * @param pSSSObject - sss object to be passed to compare the ciphertype.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_MECHANISM_INVALID If unknown algorithm specified.
 * @retval CKR_KEY_TYPE_INCONSISTENT If the algrithm is invalid for the keytype.
 *
 */
CK_RV pkcs11_is_valid_keytype(sss_algorithm_t algorithm, sss_cipher_type_t *cipher, sss_object_t *pSSSObject)
{
    CK_RV xResult = CKR_OK;
    switch (algorithm) {
    case kAlgorithm_SSS_AES_ECB:
    case kAlgorithm_SSS_AES_CBC:
    case kAlgorithm_SSS_AES_CTR:
        *cipher = kSSS_CipherType_AES;
        break;
    case kAlgorithm_SSS_DES_CBC:
    case kAlgorithm_SSS_DES_ECB:
    case kAlgorithm_SSS_DES3_CBC:
    case kAlgorithm_SSS_DES3_ECB:
        *cipher = kSSS_CipherType_DES;
        break;
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1:
        *cipher = kSSS_CipherType_RSA;
        break;

    default:
        xResult = CKR_MECHANISM_INVALID;
        break;
    }

    if (*cipher != (sss_cipher_type_t)pSSSObject->cipherType) {
        xResult = CKR_KEY_TYPE_INCONSISTENT;
    }
    return xResult;
}

/**
 * @brief  Get the keybitlen and ciphertype values based on the passed ec params
 *
 */
CK_RV pkcs11_get_ec_info(uint8_t *params, size_t *KeyBitLen, sss_cipher_type_t *cipher)
{
    CK_RV xResult = CKR_ARGUMENTS_BAD;

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP192R1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP192R1) - 1) == 0) {
        *KeyBitLen = 192;
        *cipher    = kSSS_CipherType_EC_NIST_P;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP224R1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP224R1) - 1) == 0) {
        *KeyBitLen = 224;
        *cipher    = kSSS_CipherType_EC_NIST_P;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP256R1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP256R1) - 1) == 0) {
        *KeyBitLen = 256;
        *cipher    = kSSS_CipherType_EC_NIST_P;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP384R1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP384R1) - 1) == 0) {
        *KeyBitLen = 384;
        *cipher    = kSSS_CipherType_EC_NIST_P;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP521R1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP521R1) - 1) == 0) {
        *KeyBitLen = 521;
        *cipher    = kSSS_CipherType_EC_NIST_P;
        xResult    = CKR_OK;
        goto exit;
    }
    /* For NIST-K Curves */

    if (memcmp(OID_EC_GRP_SECP160K1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_SECP160K1) - 1) == 0) {
        *KeyBitLen = 160;
        *cipher    = kSSS_CipherType_EC_NIST_K;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP192K1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP192K1) - 1) == 0) {
        *KeyBitLen = 192;
        *cipher    = kSSS_CipherType_EC_NIST_K;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP224K1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP224K1) - 1) == 0) {
        *KeyBitLen = 224;
        *cipher    = kSSS_CipherType_EC_NIST_K;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(MBEDTLS_OID_EC_GRP_SECP256K1, &params[OID_START_INDEX], sizeof(MBEDTLS_OID_EC_GRP_SECP256K1) - 1) == 0) {
        *KeyBitLen = 256;
        *cipher    = kSSS_CipherType_EC_NIST_K;
        xResult    = CKR_OK;
        goto exit;
    }
    /* Brainpool curves */
    if (memcmp(OID_EC_GRP_BP160R1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_BP160R1) - 1) == 0) {
        *KeyBitLen = 160;
        *cipher    = kSSS_CipherType_EC_BRAINPOOL;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(OID_EC_GRP_BP192R1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_BP192R1) - 1) == 0) {
        *KeyBitLen = 192;
        *cipher    = kSSS_CipherType_EC_BRAINPOOL;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(OID_EC_GRP_BP224R1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_BP224R1) - 1) == 0) {
        *KeyBitLen = 224;
        *cipher    = kSSS_CipherType_EC_BRAINPOOL;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(OID_EC_GRP_BP256R1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_BP256R1) - 1) == 0) {
        *KeyBitLen = 256;
        *cipher    = kSSS_CipherType_EC_BRAINPOOL;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(OID_EC_GRP_BP320R1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_BP320R1) - 1) == 0) {
        *KeyBitLen = 320;
        *cipher    = kSSS_CipherType_EC_BRAINPOOL;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(OID_EC_GRP_BP384R1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_BP384R1) - 1) == 0) {
        *KeyBitLen = 384;
        *cipher    = kSSS_CipherType_EC_BRAINPOOL;
        xResult    = CKR_OK;
        goto exit;
    }

    if (memcmp(OID_EC_GRP_BP512R1, &params[OID_START_INDEX], sizeof(OID_EC_GRP_BP512R1) - 1) == 0) {
        *KeyBitLen = 512;
        *cipher    = kSSS_CipherType_EC_BRAINPOOL;
        xResult    = CKR_OK;
        goto exit;
    }

exit:
    return xResult;
}

/**
 * @brief  Add ecc header to the public key data based on ciphertype and keysize
 *
 */
CK_RV pkcs11_add_ec_header(uint16_t keySize,
    sss_cipher_type_t cipher,
    uint8_t *pubKeyBuf,
    size_t *pubKeyBufLen,
    CK_BYTE_PTR pubData,
    CK_ULONG pubDataLen,
    size_t *keyBitLen)
{
    CK_RV xResult      = CKR_ARGUMENTS_BAD;
    const uint8_t *hdr = NULL;
    size_t hdrLen      = 0;

    switch (keySize) {
    case 20: {
        if (cipher == kSSS_CipherType_EC_NIST_K) {
            hdr        = ecc_der_header_secp160k1;
            hdrLen     = der_ecc_secp160k1_header_len;
            *keyBitLen = 160;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_BRAINPOOL) {
            hdr        = ecc_der_header_bp160r1;
            hdrLen     = der_ecc_bp160r1_header_len;
            *keyBitLen = 160;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    case 24: {
        if (cipher == kSSS_CipherType_EC_NIST_P) {
            hdr        = ecc_der_header_secp192;
            hdrLen     = der_ecc_secp192_header_len;
            *keyBitLen = 192;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_NIST_K) {
            hdr        = ecc_der_header_secp192k1;
            hdrLen     = der_ecc_secp192k1_header_len;
            *keyBitLen = 192;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_BRAINPOOL) {
            hdr        = ecc_der_header_bp192r1;
            hdrLen     = der_ecc_bp192r1_header_len;
            *keyBitLen = 192;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    case 28: {
        if (cipher == kSSS_CipherType_EC_NIST_P) {
            hdr        = ecc_der_header_secp224;
            hdrLen     = der_ecc_secp224_header_len;
            *keyBitLen = 224;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_NIST_K) {
            hdr        = ecc_der_header_secp224k1;
            hdrLen     = der_ecc_secp224k1_header_len;
            *keyBitLen = 224;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_BRAINPOOL) {
            hdr        = ecc_der_header_bp224r1;
            hdrLen     = der_ecc_bp224r1_header_len;
            *keyBitLen = 224;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    case 32: {
        if (cipher == kSSS_CipherType_EC_NIST_P) {
            hdr        = ecc_der_header_secp256;
            hdrLen     = der_ecc_secp256_header_len;
            *keyBitLen = 256;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_NIST_K) {
            hdr        = ecc_der_header_secp256k1;
            hdrLen     = der_ecc_secp256k1_header_len;
            *keyBitLen = 256;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_BRAINPOOL) {
            hdr        = ecc_der_header_bp256r1;
            hdrLen     = der_ecc_bp256r1_header_len;
            *keyBitLen = 256;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    case 40: {
        if (cipher == kSSS_CipherType_EC_BRAINPOOL) {
            hdr        = ecc_der_header_bp320r1;
            hdrLen     = der_ecc_bp320r1_header_len;
            *keyBitLen = 320;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    case 48: {
        if (cipher == kSSS_CipherType_EC_NIST_P) {
            hdr        = ecc_der_header_secp384;
            hdrLen     = der_ecc_secp384_header_len;
            *keyBitLen = 384;
            break;
        }
        else if (cipher == kSSS_CipherType_EC_BRAINPOOL) {
            hdr        = ecc_der_header_bp384r1;
            hdrLen     = der_ecc_bp384r1_header_len;
            *keyBitLen = 384;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    case 64: {
        if (cipher == kSSS_CipherType_EC_BRAINPOOL) {
            hdr        = ecc_der_header_bp512r1;
            hdrLen     = der_ecc_bp512r1_header_len;
            *keyBitLen = 512;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    case 65:
    case 66: {
        if (cipher == kSSS_CipherType_EC_NIST_P) {
            hdr        = ecc_der_header_secp521;
            hdrLen     = der_ecc_secp521_header_len;
            *keyBitLen = 521;
            break;
        }
        else {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
    }
    default: {
        xResult = CKR_ARGUMENTS_BAD;
        goto exit;
    }
    }

    ENSURE_OR_GO_EXIT(*pubKeyBufLen >= hdrLen);
    memcpy(pubKeyBuf, hdr, hdrLen);
    ENSURE_OR_GO_EXIT((*pubKeyBufLen - hdrLen) >= pubDataLen);
    memcpy((pubKeyBuf + hdrLen), pubData, pubDataLen);
    *pubKeyBufLen = hdrLen + pubDataLen;
    xResult       = CKR_OK;

exit:
    return xResult;
}