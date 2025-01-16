/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SSS_PKCS11_PORT_H__
#define __SSS_PKCS11_PORT_H__

/* ********************** Include files ********************** */
#include <sm_types.h>
#include "core_pkcs11.h"

/* ********************** Defines ********************** */
#define ERR_BASE64_BUFFER_TOO_SMALL -0x002A  /**< Output buffer too small. */
#define ERR_BASE64_INVALID_CHARACTER -0x002C /**< Invalid character in input. */
/* Enable cloud demos in standard package testing */
#define SSS_PKCS11_ENABLE_CLOUD_DEMO 1

/**
 * DER constants
 * These constants comply with the DER encoded ASN.1 type tags.
 * DER encoding uses hexadecimal representation.
 * An example DER sequence is:\n
 * - 0x02 -- tag indicating INTEGER
 * - 0x01 -- length in octets
 * - 0x05 -- value
 */
#define ASN1_TAG_BOOLEAN 0x01
#define ASN1_TAG_INTEGER 0x02
#define ASN1_TAG_BIT_STRING 0x03
#define ASN1_TAG_OCTET_STRING 0x04
#define ASN1_TAG_NULL 0x05
#define ASN1_TAG_OID 0x06
#define ASN1_TAG_ENUMERATED 0x0A
#define ASN1_TAG_UTF8_STRING 0x0C
#define ASN1_TAG_SEQUENCE 0x10
#define ASN1_TAG_SET 0x11
#define ASN1_TAG_PRINTABLE_STRING 0x13
#define ASN1_TAG_T61_STRING 0x14
#define ASN1_TAG_IA5_STRING 0x16
#define ASN1_TAG_UTC_TIME 0x17
#define ASN1_TAG_GENERALIZED_TIME 0x18
#define ASN1_TAG_UNIVERSAL_STRING 0x1C
#define ASN1_TAG_BMP_STRING 0x1E
#define ASN1_TAG_PRIMITIVE 0x00
#define ASN1_TAG_CONSTRUCTED 0x20
#define ASN1_TAG_CONTEXT_SPECIFIC 0x80

/* ********************** Structrues and Typedefs ********************** */

/**
 * Handling key parse structure.
 */
typedef struct sss_pkcs11_key_parse
{
    uint32_t cipherType;
    uint8_t *pbuff;
    size_t buffLen;
    size_t keyBitLen;
} sss_pkcs11_key_parse_t;

/* ********************** Function Declaration ********************** */

int pkcs11_parse_Cert(uint8_t *pCert, size_t certLen);

int pkcs11_parse_PrivateKey(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ULONG_PTR index, sss_pkcs11_key_parse_t *keyParse);

int pkcs11_parse_PublicKey(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ULONG_PTR index, sss_pkcs11_key_parse_t *keyParse);

int pkcs11_parse_Convert_PemToDer(const unsigned char *input, size_t ilen, unsigned char *output, size_t *olen);

CK_RV pkcs11_parseCert_GetAttr(
    CK_ATTRIBUTE_TYPE attributeType, uint8_t *pCert, size_t certLen, uint8_t *pData, CK_ULONG *ulAttrLength);

CK_RV pkcs11_create_raw_privateKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key, size_t *keyLen);

CK_RV pkcs11_create_raw_publicKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key, size_t *keyLen);

int sss_util_asn1_get_len(unsigned char **p, const unsigned char *end, size_t *len);

int sss_util_asn1_get_tag(unsigned char **p, const unsigned char *end, size_t *len, int tag);

int base64_decode(unsigned char *dst, size_t dlen, size_t *olen, const unsigned char *src, size_t slen);

int pkcs11_private_key_parse(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, uint8_t *pKey, size_t keyLen);

int pkcs11_public_key_parse(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, uint8_t *pKey, size_t keyLen);

#endif // __SSS_PKCS11_PORT_H__
