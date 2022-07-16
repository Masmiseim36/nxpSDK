/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include "sss_pkcs11_pal.h"

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

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

/** @brief Create Raw Private Key.
 * This function generates a raw private key.
 *
 * @param pxTemplate - Pointer to a search template that specifies the attribute values to match.
 * @param ulCount - Number of attributes in the search template.
 * @param key_buffer - Buffer containing the key data.
 * @param keyLen - size of the key_buffer in bytes.
 * 
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_ARGUMENTS_BAD The arguments supplied to the function are not appropriate.
 */
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

/** @brief Create Raw Public Key.
 * This function generates a raw public key.
 *
 * @param pxTemplate - Pointer to a search template that specifies the attribute values to match.
 * @param ulCount - Number of attributes in the search template.
 * @param key_buffer - Buffer containing the key data.
 * @param keyLen - size of the key_buffer in bytes.
 * 
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_ARGUMENTS_BAD The arguments supplied to the function are not appropriate.
 */
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
        // // xResult = SetASNTLV(tag, (uint8_t*) pxTemplate[parameterIndex].pValue, parameterLen, ecPubParams,
        // &ecPubParams_size); if(xResult != CKR_OK) {
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

/** @brief Ec Signature To RandS.
 * This function generates RandS from signature.
 *
 * @param signature - Buffer containing the signature to read and verify.
 * @param sigLen - Size of signature in bytes.
 * 
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 */

CK_RV EcSignatureToRandS(uint8_t *signature, size_t *sigLen)
{
    CK_RV xResult      = CKR_FUNCTION_FAILED;
    uint8_t rands[128] = {0};
    int index          = 0;
    size_t i           = 0;
    size_t len         = 0;
    if (signature[index++] != 0x30) {
        goto exit;
    }
    if (signature[index++] != (*sigLen - 2)) {
        goto exit;
    }
    if (signature[index++] != 0x02) {
        goto exit;
    }

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

/** @brief Ec RandS To Signature.
 * This function generates signature from RandS.
 *
 * @param rands - Pointer to a location where rands recieved.
 * @param rands_len - Length in bytes of generated rands.
 * @param output - Output buffer containing the signature data.
 * @param outputLen - Size of the output in bytes.
 * 
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_ARGUMENTS_BAD The arguments supplied to the function are not appropriate.
 */

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

/** @brief Ec Public key Get Ec Params.
 * This function is to get Ec params.
 *
 * @param input - Pointer to a location where rands recieved.
 * @param dataLen - Length in bytes of generated rands.
 * 
 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 */

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

/** @brief Asymmetric Encryption.
 * This function generates a random IV (initialization vector), nonce
 * or initial counter value for the encryption operation as appropriate
 * for the chosen algorithm, key type and key size.
 *
 * @param pxSessionObj - Pointer to mechParameter for initializing initialization vector.
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 * @param pData - Buffer containing the input data.
 * @param ulDataLen - Length of the buffer pData.
 * @param pEncryptedData - Buffer containing the encrypted data.
 * @param pulEncryptedDataLen - Length of the buffer pEncryptedData.

 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_DEVICE_ERROR If some problem has occured with the token or slot.
 * @retval #CKR_BUFFER_TOO_SMALL The output of function is too large to fit in supplied buffer.
 * @retval #CKR_MECHANISM_INVALID If unknown mechanism specified.
 */
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

/** @brief Asymmetric Decryption.
 * This function generates a random IV (initialization vector), nonce
 * or initial counter value for the decryption operation as appropriate
 * for the chosen algorithm, key type and key size.
 *
 * @param pxSessionObj - Pointer to mechParameter for initializing initialization vector.
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 * @param pEncryptedData - Buffer containing the encrypted data.
 * @param ulEncryptedDataLen - Length of the buffer pEncryptedData.
 * @param pData - Buffer containing the decrypted data.
 * @param pulDataLen - Length of the buffer pData.

 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_DEVICE_ERROR If some problem has occured with the token or slot.
 * @retval #CKR_BUFFER_TOO_SMALL The output of function is too large to fit in supplied buffer.
 */
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
