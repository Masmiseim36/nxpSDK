/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include "sss_pkcs11_pal.h"

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

/* clang-format off */

/* RSA Header */
const uint8_t rsa512PubHeader[] = {
    0x30, 0x5C, 0x30, 0x0D,     0x06, 0x09, 0x2A, 0x86,  \
    0x48, 0x86, 0xF7, 0x0D,     0x01, 0x01, 0x01, 0x05,  \
    0x00, 0x03, 0x4B, 0x00,     };

const uint8_t rsa1kPubHeader[] = {
    0x30, 0x81, 0x9F, 0x30,     0x0D, 0x06, 0x09, 0x2A,  \
    0x86, 0x48, 0x86, 0xF7,     0x0D, 0x01, 0x01, 0x01,  \
    0x05, 0x00, 0x03, 0x81,     0x8D, };

const uint8_t rsa1152PubHeader[] = {
    0x30, 0x81, 0xAF, 0x30,     0x0D, 0x06, 0x09, 0x2A,  \
    0x86, 0x48, 0x86, 0xF7,     0x0D, 0x01, 0x01, 0x01,  \
    0x05, 0x00, 0x03, 0x81,     0x9D, 0x00, };

const uint8_t rsa2kPubHeader[] = {
    0x30, 0x82, 0x01, 0x22,     0x30, 0x0D, 0x06, 0x09,  \
    0x2A, 0x86, 0x48, 0x86,     0xF7, 0x0D, 0x01, 0x01,  \
    0x01, 0x05, 0x00, 0x03,     0x82, 0x01, 0x0F, 0x00,  \
    };

const uint8_t rsa3kPubHeader[] = {
    0x30, 0x82, 0x01, 0xA2,     0x30, 0x0D, 0x06, 0x09,  \
    0x2A, 0x86, 0x48, 0x86,     0xF7, 0x0D, 0x01, 0x01,  \
    0x01, 0x05, 0x00, 0x03,     0x82, 0x01, 0x8F, 0x00,  \
    };

const uint8_t rsa4kPubHeader[] = {
    0x30, 0x82, 0x02, 0x22,     0x30, 0x0D, 0x06, 0x09,  \
    0x2A, 0x86, 0x48, 0x86,     0xF7, 0x0D, 0x01, 0x01,  \
    0x01, 0x05, 0x00, 0x03,     0x82, 0x02, 0x0F, 0x00,  \
    };

/* clang-format on */


/* Mutex handling */

/**
 * @brief Mutex Init.
 * Return - 0:Success, 1:Error
 */
int sss_pkcs11_mutex_init(void)
{
    return 0;
    //TBU
}

/**
 * @brief Mutex Lock.
 * Return - 0:Success, 1:Error
 */
int sss_pkcs11_mutex_lock(void)
{
    return 0;
    //TBU
}

/**
 * @brief Mutex Init.
 * Return - 0:Success, 1:Error
 */
int sss_pkcs11_mutex_unlock(void)
{
    return 0;
    //TBU
}

/**
 * @brief Mutex Destroy.
 * Return - 0:Success, 1:Error
 */
int sss_pkcs11_mutex_destroy(void)
{
    return 0;
    //TBU
}


static int parsecert(uint8_t *pCert, size_t certLen)
{
    int ret            = -1;
    unsigned char *p   = pCert;
    unsigned char *end = pCert + certLen;
    size_t len         = 0;
    size_t cmpLen      = 0;

    /* Parse first sequence tag */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_D("Error parsing ASN.1 data : %d", __LINE__);
        return ret;
    }
    /* Incrementing p for extracting len */
    p++;
    if ((*p & 0x80) == 0x80) {
        if ((*p & 0x7F) == 0x01) {
            cmpLen = *(p + 1);
        }
        else if ((*(p)&0x7F) == 0x02) {
            cmpLen = ((*(p + 1) << 8) + *(p + 2));
        }
    }
    else {
        cmpLen = *p;
    }
    p--;
    /* p now points to TBS bytes */
    /* Parse sequence tag of TBSCertificate */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_D("Error parsing ASN.1 data : %d", __LINE__);
        return ret;
    }
    if (cmpLen != len) {
        LOG_E("TBSCertificate length mismatch : %d", __LINE__);
        return ret;
    }
    p += len;
    /* Incrementing p for extracting len */
    p++;
    if ((*p & 0x80) == 0x80) {
        if ((*p & 0x7F) == 0x01) {
            cmpLen = *(p + 1);
        }
        else if ((*(p)&0x7F) == 0x02) {
            cmpLen = ((*(p + 1) << 8) + *(p + 2));
        }
    }
    else {
        cmpLen = *p;
    }
    p--;
    /* p now points to Certificate signature algorithm */
    /* Parse sequence tag of Certificate version */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_D("Error parsing ASN.1 data : %d", __LINE__);
        return ret;
    }
    if (cmpLen != len) {
        LOG_E("Cert signature algo length mismatch : %d", __LINE__);
        return ret;
    }
    p += len;
    /* Incrementing p for extracting len */
    p++;
    if ((*p & 0x80) == 0x80) {
        if ((*p & 0x7F) == 0x01) {
            cmpLen = *(p + 1);
        }
        else if ((*(p)&0x7F) == 0x02) {
            cmpLen = ((*(p + 1) << 8) + *(p + 2));
        }
    }
    else {
        cmpLen = *p;
    }
    p--;
    /* p now points to Certificate signature*/
    /* Parse sequence tag of Certificate signature */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_BIT_STRING);
    if (ret != 0) {
        LOG_D("Error parsing ASN.1 data : %d", __LINE__);
        return ret;
    }
    if (cmpLen != len) {
        LOG_E("Cert signature length mismatch : %d", __LINE__);
        return ret;
    }
    return ret;
}

/*Function for parsing the private key*/
int private_key_parse(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, uint8_t *pKey, size_t keyLen)
{
    CK_RV xResult        = CKR_OK;
    CK_KEY_TYPE key_type = CKK_RSA;
    int ret              = 1;
    CK_ULONG index;
    xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_KEY_TYPE, &index);
    if (xResult != CKR_OK) {
        return 1;
    }
    memcpy(&key_type, pTemplate[index].pValue, pTemplate[index].ulValueLen);
    if (key_type == CKK_RSA) {
        CK_ULONG modulusIndex;
        xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_MODULUS, &modulusIndex);
        if (xResult != CKR_OK) {
            return 1;
        }
        ret = 0;
    }
    else if (key_type == CKK_EC) { /*CKK_EC also means CKK_ECDSA both enum values are same*/
                                   // Currently only for NIST-P curves
#if CKK_EC != CKK_ECDSA
#error "Assumed to be equal"
#endif

        unsigned char *p   = pKey;
        unsigned char *end = pKey + keyLen;
        size_t len         = 0;

        /* Parse first sequence tag */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
        if (ret != 0) {
            LOG_D("Error parsing ASN.1 data : %d", __LINE__);
            return 1;
        }
        /* p now points to version */
        /* Parse integer tag of version */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_INTEGER);
        if (ret != 0) {
            LOG_D("Error parsing ASN.1 data : %d", __LINE__);
            return 1;
        }
        p += len;

        /* p now points to octet String */
        /* Parse 0x04 tag of octet String */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_OCTET_STRING);
        if (ret != 0) {
            LOG_D("Error parsing ASN.1 data : %d", __LINE__);
            return 1;
        }
        p += len;
    }
    else {
        return 1;
    }

    return ret;
}

/*Function for parsing the public key*/
int public_key_parse(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, uint8_t *pKey, size_t keyLen)
{
    CK_RV xResult        = CKR_OK;
    CK_KEY_TYPE key_type = CKK_RSA;
    int ret              = 1;
    CK_ULONG index;
    xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_KEY_TYPE, &index);
    if (xResult != CKR_OK) {
        return 1;
    }
    memcpy(&key_type, pTemplate[index].pValue, pTemplate[index].ulValueLen);
    if (key_type == CKK_RSA) {
        CK_ULONG modulusIndex;
        xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_MODULUS, &modulusIndex);
        if (xResult != CKR_OK) {
            return 1;
        }
        ret = 0;
    }
    else if (key_type == CKK_EC) { /*CKK_EC also means CKK_ECDSA both enum values are same*/
                                   // Currently only for NIST-P curves
#if CKK_EC != CKK_ECDSA
#error "Assumed to be equal"
#endif

        unsigned char *p   = pKey;
        unsigned char *end = pKey + keyLen;
        size_t len         = 0;

        /* Parse first sequence tag */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
        if (ret != 0) {
            LOG_D("Error parsing ASN.1 data : %d", __LINE__);
            return ret;
        }

        /* p now points to EC Params */
        /* Parse sequence tag of EC Params */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
        if (ret != 0) {
            LOG_D("Error parsing ASN.1 data : %d", __LINE__);
            return ret;
        }
        p += len;

        /* p now points to Bit String */
        /* Parse 0x03 tag of Bit String */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_BIT_STRING);
        if (ret != 0) {
            LOG_D("Error parsing ASN.1 data : %d", __LINE__);
            return ret;
        }
    }
    else {
        return 1;
    }

    return ret;
}

static int parsePrivateKey(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ULONG_PTR index, sss_pkcs11_key_parse_t *keyParse)
{
    CK_RV xResult = CKR_OK;
    int ret       = 1;
    CK_KEY_TYPE key_type;

    xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_VALUE, index);
    if (xResult != CKR_OK) {
        uint8_t key[4096] = {0};
        size_t keyLen = sizeof(key);
        xResult       = CreateRawPrivateKey(pTemplate, ulCount, &key[0], &keyLen);
        if (xResult != CKR_OK) {
            return ret;
        }
        memcpy(keyParse->pbuff, key, keyLen);
        keyParse->buffLen = (size_t)keyLen;
        ret               = 0;
    }
    else {
        if (0 != pTemplate[*index].ulValueLen) {
            ret = private_key_parse(pTemplate, ulCount, pTemplate[*index].pValue, pTemplate[*index].ulValueLen);
            if (ret != 0) {
                uint8_t key[1024] = {0};
                size_t keyLen     = sizeof(key);
                xResult           = CreateRawPrivateKey(pTemplate, ulCount, &key[0], &keyLen);
                if (xResult != CKR_OK) {
                    return ret;
                }
                memcpy(keyParse->pbuff, key, keyLen);
                keyParse->buffLen = (size_t)keyLen;
            }
            else {
                memcpy(keyParse->pbuff, pTemplate[*index].pValue, pTemplate[*index].ulValueLen);
                keyParse->buffLen = (size_t)pTemplate[*index].ulValueLen;
            }
        }
    }

#if SSS_HAVE_APPLET_SE05X_IOT

    key_type = CKK_RSA;
    *index   = 0;
    xResult  = GetAttributeParameterIndex(pTemplate, ulCount, CKA_KEY_TYPE, index);
    if (xResult != CKR_OK) {
        return 1;
    }
    memcpy(&key_type, pTemplate[*index].pValue, pTemplate[*index].ulValueLen);
    if (key_type == CKK_RSA) {
        keyParse->cipherType = kSSS_CipherType_RSA;
        CK_ULONG modulusIndex;
        xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_MODULUS, &modulusIndex);
        if (xResult != CKR_OK) {
            return 1;
        }
        uint8_t *pInputModulus = (uint8_t *)pTemplate[modulusIndex].pValue;
        size_t modulusLen      = (size_t)pTemplate[modulusIndex].ulValueLen;

        if (pInputModulus[0] == 0x00) {
            if (modulusLen < 1) {
                return 1;
            }
            modulusLen--;
        }
        if ((SIZE_MAX / 8) < modulusLen) {
            return 1;
        }
        keyParse->keyBitLen = modulusLen * 8;
    }
    else if (key_type == CKK_EC) { /*CKK_EC also means CKK_ECDSA both enum values are same*/
                                   // Currently only for NIST-P curves
#if CKK_EC != CKK_ECDSA
#error "Assumed to be equal"
#endif
        keyParse->cipherType = kSSS_CipherType_EC_NIST_P;

        /*To calculate the keyBitLen*/
        size_t keyLen = 0;
        *index        = 0;
        xResult       = GetAttributeParameterIndex(pTemplate, ulCount, CKA_VALUE, index);
        if (xResult == CKR_OK) {
            keyLen = (size_t)pTemplate[*index].ulValueLen;
            ret    = 0;
        }
        else {
            unsigned char *p   = keyParse->pbuff;
            unsigned char *end = keyParse->pbuff + keyParse->buffLen;
            size_t len         = 0;

            /* Parse first sequence tag */
            ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
            if (ret != 0) {
                LOG_E("Error parsing ASN.1 data : %d", __LINE__);
            }
            /* p now points to version */
            /* Parse integer tag of version */
            ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_INTEGER);
            if (ret != 0) {
                LOG_E("Error parsing ASN.1 data : %d", __LINE__);
            }
            p += len;

            /* p now points to octet String */
            /* Parse 0x04 tag of octet String */
            ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_OCTET_STRING);
            if (ret != 0) {
                LOG_E("Error parsing ASN.1 data : %d", __LINE__);
            }
            p += len;

            /* p now points to extension */
            /* Parse 0xA0 tag */
            ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_CONSTRUCTED | ASN1_TAG_CONTEXT_SPECIFIC);
            if (ret != 0) {
                LOG_E("Error parsing ASN.1 data : %d", __LINE__);
            }
            p += len;

            /* p now points to context specific */
            /* Parse 0xA1 tag */
            ret = sss_util_asn1_get_tag(
                &p, end, &len, ASN1_TAG_CONSTRUCTED | ASN1_TAG_CONTEXT_SPECIFIC | ASN1_TAG_BOOLEAN);
            if (ret != 0) {
                LOG_E("Error parsing ASN.1 data : %d", __LINE__);
            }

            /* p now points to public key */
            /* Parse 0x03 tag of bit string */
            ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_BIT_STRING);
            if (ret != 0) {
                LOG_E("Error parsing ASN.1 data : %d", __LINE__);
            }
            /* check for zero padding */
            if (*p == 0x00) {
                p++;
                len--;
            }
            /* Uncompressed Key */

            if (*p == 0x04) {
                len--;
            }
            keyLen = len / 2;
        }

        if (keyLen < 64) {
            keyParse->keyBitLen = keyLen * 8;
        }
        else if (keyLen == 66) {
            /*ECP_DP_SECP521R1 Case*/
            keyParse->keyBitLen = 521;
        }
        else {
            LOG_E("Invalid KeyLen");
            return 1;
        }
    }
    else {
        LOG_E("Key Type not supported");
        return 1;
    }
#endif

    return ret;
}

static int parsePublicKey(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ULONG_PTR index, sss_pkcs11_key_parse_t *keyParse)
{
    CK_RV xResult = CKR_OK;
    int ret       = 1;

    xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_VALUE, index);
    if (xResult != CKR_OK) {
        uint8_t key[2048] = {0};
        size_t keyLen = sizeof(key);
        xResult       = CreateRawPublicKey(pTemplate, ulCount, &key[0], &keyLen);
        if (xResult != CKR_OK) {
            return 1;
        }

        memcpy(keyParse->pbuff, &key[0], keyLen);
        keyParse->buffLen = keyLen;
        ret               = 0;
    }
    else {
        ret = public_key_parse(pTemplate, ulCount, pTemplate[*index].pValue, pTemplate[*index].ulValueLen);
        if (ret != 0) {
            xResult = CKR_ARGUMENTS_BAD;
            return ret;
        }

        memcpy(keyParse->pbuff, pTemplate[*index].pValue, pTemplate[*index].ulValueLen);
        keyParse->buffLen = (size_t)pTemplate[*index].ulValueLen;
    }

#if SSS_HAVE_APPLET_SE05X_IOT

    CK_KEY_TYPE key_type = CKK_RSA;
    *index               = 0;
    xResult              = GetAttributeParameterIndex(pTemplate, ulCount, CKA_KEY_TYPE, index);
    if (xResult != CKR_OK) {
        return 1;
    }
    memcpy(&key_type, pTemplate[*index].pValue, pTemplate[*index].ulValueLen);
    if (key_type == CKK_RSA) {
        keyParse->cipherType = kSSS_CipherType_RSA;
        CK_ULONG modulusIndex;
        xResult = GetAttributeParameterIndex(pTemplate, ulCount, CKA_MODULUS, &modulusIndex);
        if (xResult != CKR_OK) {
            return 1;
        }
        uint8_t *pInputModulus = (uint8_t *)pTemplate[modulusIndex].pValue;
        size_t modulusLen      = (size_t)pTemplate[modulusIndex].ulValueLen;

        if (pInputModulus[0] == 0x00) {
            if (modulusLen < 1) {
                return 1;
            }
            modulusLen--;
        }
        if ((SIZE_MAX / 8) < modulusLen) {
            return 1;
        }
        keyParse->keyBitLen = modulusLen * 8;
    }
    else if (key_type == CKK_EC) { /*CKK_EC also means CKK_ECDSA both enum values are same*/
                                   // Currently only for NIST-P curves
#if CKK_EC != CKK_ECDSA
#error "Assumed to be equal"
#endif
        keyParse->cipherType = kSSS_CipherType_EC_NIST_P;

        /*To calculate the keyBitLen*/

        unsigned char *p   = keyParse->pbuff;
        unsigned char *end = keyParse->pbuff + keyParse->buffLen;
        size_t len         = 0;
        size_t keyLen      = 0;

        /* Parse first sequence tag */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
        if (ret != 0) {
            LOG_E("Error parsing ASN.1 data : %d", __LINE__);
        }

        /* p now points to EC Params */
        /* Parse sequence tag of EC Params */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
        if (ret != 0) {
            LOG_E("Error parsing ASN.1 data : %d", __LINE__);
        }
        p += len;

        /* p now points to Bit String */
        /* Parse 0x03 tag of Bit String */
        ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_BIT_STRING);
        if (ret != 0) {
            LOG_E("Error parsing ASN.1 data : %d", __LINE__);
        }

        /* check for zero padding */
        if (*p == 0x00) {
            p++;
            if (len < 1) {
                LOG_E("Invalid Key length");
                return 1;
            }
            len--;
        }
        /* Uncompressed Key */

        if (*p == 0x04) {
            if (len < 1) {
                LOG_E("Invalid Key length");
                return 1;
            }
            len--;
        }
        keyLen = len / 2;

        if (keyLen < 64) {
            keyParse->keyBitLen = keyLen * 8;
        }
        else if (keyLen == 66) {
            /*ECP_DP_SECP521R1 Case*/
            keyParse->keyBitLen = 521;
        }
        else {
            LOG_E("Invalid KeyLen");
            return 1;
        }
    }
    else {
        LOG_E("Key Type not supported");
        return 1;
    }

#endif

    return ret;
}

/**
 * @brief Cleans up a key structure.
 */

static int convertPemToDer(const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen)
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

    ret = base64_decode(NULL, 0, &len, (const unsigned char *)s1, s2 - s1);
    if (ret == ERR_BASE64_INVALID_CHARACTER) {
        return (ret);
    }

    if (len > *olen) {
        return (-1);
    }

    if ((ret = base64_decode(output, len, &len, (const unsigned char *)s1, s2 - s1)) != 0) {
        return (ret);
    }

    *olen = len;

    return (0);
}

CK_RV parseCertGetAttr(
    CK_ATTRIBUTE_TYPE attributeType, uint8_t *pCert, size_t certLen, uint8_t *pData, CK_ULONG *ulAttrLength)
{
    CK_RV xResult            = CKR_OK;
    uint8_t pubdata[2048]    = {0};
    size_t pubdataLen        = sizeof(pubdata);
    uint8_t issuerData[256]  = {0};
    size_t issuerDataLen     = sizeof(issuerData);
    uint8_t subjectData[256] = {0};
    size_t subjectDataLen    = sizeof(subjectData);

    int ret            = -1;
    unsigned char *p   = pCert;
    unsigned char *end = pCert + certLen;
    size_t len         = 0;
    size_t certObjLen  = 0;

    /* Parsing the certificate for getting public key*/
    /* Parse first sequence tag */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }
    /* p now points to TBS bytes */
    /* Parse sequence tag of TBSCertificate */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }
    /* p now points to Certificate version */
    /* Parse 0xA0 tag of Certificate version */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_CONTEXT_SPECIFIC | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }
    p += len;
    /* p now points to Certificate serial number */
    /* Parse ASN1_TAG_INTEGER tag of Certificate version */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_INTEGER);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }
    p += len;
    /* p now points to Certificate signature algorithm */
    /* Parse sequence tag of Certificate version */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }

    p += len;

    /* Incrementing p for extracting len */
    p++;
    if ((*p & 0x80) == 0x80) {
        if ((*p & 0x7F) == 0x01) {
            certObjLen = *(p + 1) + 3;
        }
        else if ((*(p)&0x7F) == 0x02) {
            certObjLen = ((*(p + 1) << 8) + *(p + 2)) + 4;
        }
    }
    else {
        certObjLen = *p + 2;
    }
    p--;

    do {
        if ((p + certObjLen) > end) {
            LOG_E("Invalid certificate object");
            issuerDataLen = 0;
            break;
        }
        else if (certObjLen > issuerDataLen) {
            LOG_E("Insufficient buffer");
            issuerDataLen = 0;
            break;
        }
        if (len > 0) {
            memcpy(&issuerData[0], (void *)p, certObjLen);
            issuerDataLen = certObjLen;
        }
        else {
            issuerDataLen = 0;
        }
    } while (0);

    /* p now points to Certificate Issuer */
    /* Parse sequence tag of Certificate version */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }
    p += len;
    /* p now points to Certificate Validity */
    /* Parse sequence tag of Certificate version */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }
    p += len;

    /* Incrementing p for extracting len */
    p++;
    if ((*p & 0x80) == 0x80) {
        if ((*p & 0x7F) == 0x01) {
            certObjLen = *(p + 1) + 3;
        }
        else if ((*(p)&0x7F) == 0x02) {
            certObjLen = ((*(p + 1) << 8) + *(p + 2)) + 4;
        }
    }
    else {
        certObjLen = *p + 2;
    }
    p--;

    do {
        if ((p + certObjLen) > end) {
            LOG_E("Invalid certificate object");
            subjectDataLen = 0;
            break;
        }
        else if (certObjLen > subjectDataLen) {
            LOG_E("Insufficient buffer");
            subjectDataLen = 0;
            break;
        }
        if (certObjLen > 0) {
            memcpy(&subjectData[0], (void *)p, certObjLen);
            subjectDataLen = certObjLen;
        }
        else {
            subjectDataLen = 0;
        }
    } while (0);

    /* p now points to Certificate Subject */
    /* Parse sequence tag of Certificate version */
    ret = sss_util_asn1_get_tag(&p, end, &len, ASN1_TAG_SEQUENCE | ASN1_TAG_CONSTRUCTED);
    if (ret != 0) {
        LOG_E("Error parsing ASN.1 data : %d", __LINE__);
    }
    p += len;

    /* Incrementing p for extracting len, p now points to the public key info*/
    p++;
    if ((*p & 0x80) == 0x80) {
        if ((*p & 0x7F) == 0x01) {
            certObjLen = *(p + 1) + 3;
        }
        else if ((*(p)&0x7F) == 0x02) {
            certObjLen = ((*(p + 1) << 8) + *(p + 2)) + 4;
        }
    }
    else {
        certObjLen = *p + 2;
    }
    p--;

    do {
        if ((p + certObjLen) > end) {
            LOG_E("Invalid certificate object");
            issuerDataLen = 0;
            break;
        }
        else if (certObjLen > pubdataLen) {
            LOG_E("Insufficient buffer");
            pubdataLen = 0;
            break;
        }
        if (certObjLen > 0) {
            memcpy(&pubdata[0], (void *)p, certObjLen);
            pubdataLen = certObjLen;
        }
        else {
            pubdataLen = 0;
        }
    } while (0);

    switch (attributeType) {
    case CKA_HASH_OF_ISSUER_PUBLIC_KEY:
        if ((issuerData[0] != subjectData[0]) || (issuerDataLen != subjectDataLen) ||
            (memcmp(&issuerData[0], &subjectData[0], subjectDataLen) != 0)) {
            xResult = CKR_ATTRIBUTE_SENSITIVE;
        }
        else {
            if ((size_t)(*ulAttrLength) < pubdataLen) {
                LOG_E("Buffer too small");
                xResult = CKR_BUFFER_TOO_SMALL;
                break;
            }
            memcpy(&pData[0], &pubdata[0], pubdataLen);
            *ulAttrLength = pubdataLen;
        }
        break;

    case CKA_HASH_OF_SUBJECT_PUBLIC_KEY:
        if ((size_t)(*ulAttrLength) < pubdataLen) {
            LOG_E("Buffer too small");
            xResult = CKR_BUFFER_TOO_SMALL;
            break;
        }
        memset(&pData[0], 0, pubdataLen);
        memcpy(&pData[0], &pubdata[0], pubdataLen);
        *ulAttrLength = pubdataLen;
        break;

    case CKA_SUBJECT:
        if (subjectDataLen != 0) {
            if ((size_t)(*ulAttrLength) < subjectDataLen) {
                LOG_E("Buffer too small");
                xResult = CKR_BUFFER_TOO_SMALL;
                break;
            }
            memcpy(&pData[0], &subjectData[0], subjectDataLen);
            *ulAttrLength = subjectDataLen;
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
    return xResult;
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

        if (bufferSize_copy < *keyLen) {
            xResult = CKR_FUNCTION_FAILED;
            goto exit;
        }
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
        CK_ULONG valueIndex = 0;
        xResult             = GetAttributeParameterIndex(pxTemplate, ulCount, CKA_VALUE, &valueIndex);
        if (xResult != CKR_OK) {
            return xResult;
        }
        memcpy(&key_buffer[0], pxTemplate[valueIndex].pValue, pxTemplate[valueIndex].ulValueLen);
        *keyLen = pxTemplate[valueIndex].ulValueLen;
    }
    else {
        xResult = CKR_ARGUMENTS_BAD;
    }

exit:
    return xResult;
}

#if 0
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
#endif

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
        size_t index           = 0;
        size_t intModulusLen   = modulusLen;
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

        if (bufferSize_copy < (*keyLen)) {
            return CKR_ARGUMENTS_BAD;
        }
        totalLen = bufferSize_copy - *keyLen;

        /* Copy the Public Header According to key bit len*/
        if (intModulusLen == 0x40) {
            if (sizeof(rsa512PubHeader) >= *keyLen) {
                return CKR_BUFFER_TOO_SMALL;
            }
            memcpy(&key_buffer[0], rsa512PubHeader, sizeof(rsa512PubHeader));
            index += sizeof(rsa512PubHeader);
        }
        else if (intModulusLen == 0x80) {
            if (sizeof(rsa1kPubHeader) >= *keyLen) {
                return CKR_BUFFER_TOO_SMALL;
            }
            memcpy(&key_buffer[0], rsa1kPubHeader, sizeof(rsa1kPubHeader));
            index += sizeof(rsa1kPubHeader);
        }
        else if (intModulusLen == 0x90) {
            if (sizeof(rsa1152PubHeader) >= *keyLen) {
                return CKR_BUFFER_TOO_SMALL;
            }
            memcpy(&key_buffer[0], rsa1152PubHeader, sizeof(rsa1152PubHeader));
            index += sizeof(rsa1152PubHeader);
        }
        else if (intModulusLen == 0x100) {
            if (sizeof(rsa2kPubHeader) >= *keyLen) {
                return CKR_BUFFER_TOO_SMALL;
            }
            memcpy(&key_buffer[0], rsa2kPubHeader, sizeof(rsa2kPubHeader));
            index += sizeof(rsa2kPubHeader);
        }
        else if (intModulusLen == 0x180) {
            if (sizeof(rsa3kPubHeader) >= *keyLen) {
                return CKR_BUFFER_TOO_SMALL;
            }
            memcpy(&key_buffer[0], rsa3kPubHeader, sizeof(rsa3kPubHeader));
            index += sizeof(rsa3kPubHeader);
        }
        else if (intModulusLen == 0x200) {
            if (sizeof(rsa4kPubHeader) >= *keyLen) {
                return CKR_BUFFER_TOO_SMALL;
            }
            memcpy(&key_buffer[0], rsa4kPubHeader, sizeof(rsa4kPubHeader));
            index += sizeof(rsa4kPubHeader);
        }

        if ((SIZE_MAX - index) < totalLen) {
            return CKR_BUFFER_TOO_SMALL;
        }

        if ((index + totalLen) > (*keyLen)) {
            return CKR_BUFFER_TOO_SMALL;
        }

        memcpy(&key_buffer[index], &key[*keyLen], totalLen);
        *keyLen = totalLen + index;
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
        size_t i = 0;
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
        if (len > sizeof(ecPoint) - 1) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        if (ecPointInput_size <= i) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        if (len > sizeof(ecPointInput) - i) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
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
#if 0
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
#endif
        if (ecPubParams_size < parameterLen) {
            xResult = CKR_ARGUMENTS_BAD;
            goto exit;
        }
        ecPubParams_size = ecPubParams_size - parameterLen;
        memcpy(&ecPubParams[ecPubParams_size], (uint8_t *)pxTemplate[parameterIndex].pValue, parameterLen);
        //}
        // // xResult = SetASNTLV(tag, (uint8_t*) pxTemplate[parameterIndex].pValue, parameterLen, ecPubParams,
        // &ecPubParams_size); if(xResult != CKR_OK) {
        //     goto exit;
        // }

        uint8_t id_ecPublicKey[] = ID_ECPUBLICKEY;
        xResult = SetASNTLV(tag, &id_ecPublicKey[0], sizeof(id_ecPublicKey), ecPubParams, &ecPubParams_size);
        if (xResult != CKR_OK) {
            goto exit;
        }

        tag = ASN_TAG_SEQUENCE;
        if (ecPubParams_size >= sizeof(ecPubParams)) {
            goto exit;
        }

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

        if (bufferSize_copy < *keyLen) {
            xResult = CKR_FUNCTION_FAILED;
            goto exit;
        }
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

int sss_util_asn1_get_len(unsigned char **p, const unsigned char *end, size_t *len)
{
    if ((end - *p) < 1)
        return (-1);

    if ((**p & 0x80) == 0)
        *len = *(*p)++;
    else {
        switch (**p & 0x7F) {
        case 1:
            if ((end - *p) < 2)
                return (-1);

            *len = (*p)[1];
            (*p) += 2;
            break;

        case 2:
            if ((end - *p) < 3)
                return (-1);

            *len = ((size_t)(*p)[1] << 8) | (*p)[2];
            (*p) += 3;
            break;

        case 3:
            if ((end - *p) < 4)
                return (-1);

            *len = ((size_t)(*p)[1] << 16) | ((size_t)(*p)[2] << 8) | (*p)[3];
            (*p) += 4;
            break;

        case 4:
            if ((end - *p) < 5)
                return (-1);

            *len = ((size_t)(*p)[1] << 24) | ((size_t)(*p)[2] << 16) | ((size_t)(*p)[3] << 8) | (*p)[4];
            (*p) += 5;
            break;

        default:
            return (-1);
        }
    }

    if (*len > (size_t)(end - *p))
        return (-1);

    return (0);
}

/**
 * @brief         Get value field address and length of a specified tag.
 *
 *         |-tag-len-value----------------------|
 * Before  ^p                                   ^end
 * After             ^p                         ^end, len = len field = end - p;
 */
int sss_util_asn1_get_tag(unsigned char **p, const unsigned char *end, size_t *len, int tag)
{
    if ((end - *p) < 1)
        return (-1);

    if (**p != tag)
        return (-1);

    (*p)++;

    return (sss_util_asn1_get_len(p, end, len));
}

/* Port to implement function which will parse certificate */
int port_parseCert(uint8_t *pCert, size_t certLen)
{
    return parsecert(pCert, certLen);
}

/* Port to implement function which will parse private key */
int port_parsePrivateKey(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ULONG_PTR index, sss_pkcs11_key_parse_t *keyParse)
{
    return parsePrivateKey(pTemplate, ulCount, index, keyParse);
}

/* Port to implement function which will parse public key */
int port_parsePublicKey(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ULONG_PTR index, sss_pkcs11_key_parse_t *keyParse)
{
    return parsePublicKey(pTemplate, ulCount, index, keyParse);
}

/* Port to implement function which will convert PEM to DER format */
int port_parseConvertPemToDer(const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen)
{
    return convertPemToDer(input, ilen, output, olen);
}

/* Port to implement function which will parse certificate to get attributes */
CK_RV port_parseCertGetAttr(
    CK_ATTRIBUTE_TYPE attributeType, uint8_t *pCert, size_t certLen, uint8_t *pData, CK_ULONG *ulAttrLength)
{
    return parseCertGetAttr(attributeType, pCert, certLen, pData, ulAttrLength);
}