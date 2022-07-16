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

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

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
CK_RV ParseEncryptionMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm)
{
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

/** @brief GetSSS Algorithm.
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

/** @brief isX509 Certificate.
 * Helper function to check for certificates at particular keyID.
 *
 * @param xObject - Pointer to handle PKCS#11 object.
 *
 * @returns Status of the operation
 * @retval #CK_FALSE The operation returns zero value.
 */

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
            sss_rng_context_t sss_rng_ctx = {0};
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
        if (long_id == 0) {
            LOG_E("Invalid Label : %s", label);
            result = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        *keyId = (uint32_t)(
            (((long_id >> (8 * 0)) & 0x000000FF) << (8 * 3)) | (((long_id >> (8 * 1)) & 0x000000FF) << (8 * 2)) |
            (((long_id >> (8 * 2)) & 0x000000FF) << (8 * 1)) | (((long_id >> (8 * 3)) & 0x000000FF) << (8 * 0)));
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
            if ((size_t)(*ulAttrLength) < certificate.subject_raw.len) {
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

#if SSS_HAVE_APPLET_SE05X_IOT
/** @brief read_object_size.
 * To get the size of secure object.
 *
 * @param keyId - Unique keyId associated with every operation.
 * @param keyLen - Pointer to location of key length.
 *
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_KEY_HANDLE_INVALID If some problem has occured with the token or slot.
 *
 */
CK_RV read_object_size(uint32_t keyId, uint16_t *keyLen)
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

#endif // SSS_HAVE_APPLET_SE05X_IOT

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
sss_status_t parseAtrribute(se05x_object_attribute *pAttribute,
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

#if SSS_HAVE_APPLET_SE05X_IOT
smStatus_t read_id_list(CK_SESSION_HANDLE xSession, uint32_t *idlist, size_t *idlistlen, CK_ULONG ulMaxObjectCount)
{
    P11SessionPtr_t pxSession          = prvSessionPointerFromHandle(xSession);
    uint8_t pmore                      = kSE05x_MoreIndicator_NA;
    uint8_t list[MAX_ID_LIST_SIZE * 4] = {0};
    size_t listlen                     = sizeof(list);
    size_t i, k = 0;
    smStatus_t retStatus          = SM_NOT_OK;
    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pex_sss_demo_boot_ctx->session;
    pSe05xSession_t session_ctx   = &pSession->s_ctx;

    /* This condition would be checked by the calling API */
    if (pxSession == NULL) {
        return SM_NOT_OK;
    }

    if (NULL == idlist || NULL == idlistlen) {
        LOG_E("Null pointer check failed %s:%d", __FUNCTION__, __LINE__);
        return SM_NOT_OK;
    }

    LOCK_MUTEX_FOR_RTOS
    {
        retStatus = Se05x_API_ReadIDList(session_ctx, pxSession->xFindObjectOutputOffset, 0xFF, &pmore, list, &listlen);
        if (retStatus != SM_OK) {
            UNLOCK_MUTEX_FOR_RTOS_RET(retStatus)
        }
        if (listlen == 0) {
            *idlistlen = 0;
            UNLOCK_MUTEX_FOR_RTOS_RET(SM_NOT_OK);
        }
        for (i = 0; (i < listlen) && (k < USER_MAX_ID_LIST_SIZE); i += 4) {
            uint32_t id = 0 | (list[i + 0] << (3 * 8)) | (list[i + 1] << (2 * 8)) | (list[i + 2] << (1 * 8)) |
                          (list[i + 3] << (0 * 8));
            idlist[k++] = id;
            *idlistlen  = k;
        }
        UNLOCK_MUTEX_FOR_RTOS
    }

    return SM_OK;
}
#endif

sss_status_t get_validated_object_id(P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, uint32_t *keyId)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    sss_object_t sss_object = {0};

    if (pxSession->xFindObjectInit == CK_TRUE) {
        /* Find Objects operation is going on. Read from SW keystore */
        SwKeyStorePtr_t pCurrentKeyStore = pxSession->pCurrentKs;
        for (size_t i = 0; i < pCurrentKeyStore->keyIdListLen; i++) {
            if (pCurrentKeyStore->SSSObjects[i].keyId == (uint32_t)xObject) {
                /* True */
                *keyId     = xObject;
                sss_status = kStatus_SSS_Success;
                break;
            }
        }
    }
    else {
        sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(&sss_object, xObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
        *keyId = xObject;
    }

exit:
    return sss_status;
}

sss_status_t get_validated_cipher_type(
    P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, sss_cipher_type_t *cipherType)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    sss_object_t sss_object = {0};
    if (NULL == cipherType) {
        LOG_E("Null pointer check failed %s:%d", __FUNCTION__, __LINE__);
        return kStatus_SSS_Fail;
    }

    if (pxSession->xFindObjectInit == CK_TRUE) {
        /* Find Objects operation is going on. Read from SW keystore */
        SwKeyStorePtr_t pCurrentKeyStore = pxSession->pCurrentKs;
        for (size_t i = 0; i < pCurrentKeyStore->keyIdListLen; i++) {
            if (pCurrentKeyStore->SSSObjects[i].keyId == (uint32_t)xObject) {
                /* True */
                *cipherType = pCurrentKeyStore->SSSObjects[i].cipherType;
                sss_status  = kStatus_SSS_Success;
                break;
            }
        }
    }
    else {
        sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(&sss_object, xObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
        *cipherType = sss_object.cipherType;
    }

exit:
    return sss_status;
}

sss_status_t get_validated_sss_object(P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, sss_object_t *pSSSObject)
{
    sss_status_t sss_status = kStatus_SSS_Fail;
    sss_object_t sss_object = {0};
    if (pxSession->xFindObjectInit == CK_TRUE) {
        /* Find Objects operation is going on. Read from SW keystore */
        SwKeyStorePtr_t pCurrentKeyStore = pxSession->pCurrentKs;
        for (size_t i = 0; (i < pCurrentKeyStore->keyIdListLen) && (i < USER_MAX_ID_LIST_SIZE); i++) {
            if (pCurrentKeyStore->SSSObjects[i].keyId == (uint32_t)xObject) {
                /* True */
                memcpy(pSSSObject, &pCurrentKeyStore->SSSObjects[i], sizeof(*pSSSObject));
                sss_status = kStatus_SSS_Success;
                break;
            }
        }
    }
    else {
        sss_status = sss_key_object_init(&sss_object, &pex_sss_demo_boot_ctx->ks);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);

        sss_status = sss_key_object_get_handle(&sss_object, xObject);
        ENSURE_OR_GO_EXIT(sss_status == kStatus_SSS_Success);
        memcpy(pSSSObject, &sss_object, sizeof(*pSSSObject));
    }

exit:
    return sss_status;
}
