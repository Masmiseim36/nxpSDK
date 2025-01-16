/*
 * Copyright 2021,2024 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SSS_PKCS11_PAL_H__
#define __SSS_PKCS11_PAL_H__

/* ********************** Include files ********************** */

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(USE_RTOS) && USE_RTOS == 1 /* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#endif

//#define NX_LOG_ENABLE_PKCS11_DEBUG 1

#include "sss_pkcs11_utils.h"
#include <nxEnsure.h>
#include <nxLog_pkcs11.h>
#include "ex_sss.h"
/*  se05x includes. */
#include <fsl_sss_se05x_apis.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include "se05x_APDU_apis.h"
#include "se05x_enums.h"
#include "se05x_tlv.h"
#include <fsl_sss_util_asn1_der.h>
#include "ex_sss_ports.h"
#include <ex_sss_boot.h>

#if defined(PKCS11_LIBRARY)
#if (__GNUC__ && !AX_EMBEDDED)
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#endif //#if (__GNUC__ && !AX_EMBEDDED)
#endif //#if defined(PKCS11_LIBRARY)

/* Enable the object cache use in case of platforms not having any memory constraints */
#if (__GNUC__ && !AX_EMBEDDED)
#define ENABLE_OBJECT_CACHE 1
#else
#define ENABLE_OBJECT_CACHE 0
#endif

/* ********************** Global variables ********************** */
extern ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx;
#if SSS_PKCS11_ENABLE_CLOUD_DEMO
extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;
#endif

extern const uint8_t ecc_der_header_secp192[];
extern const uint8_t ecc_der_header_secp224[];
extern const uint8_t ecc_der_header_secp256[];
extern const uint8_t ecc_der_header_secp384[];
extern const uint8_t ecc_der_header_secp521[];
extern const uint8_t ecc_der_header_secp160k1[];
extern const uint8_t ecc_der_header_secp192k1[];
extern const uint8_t ecc_der_header_secp224k1[];
extern const uint8_t ecc_der_header_secp256k1[];
extern const uint8_t ecc_der_header_bp160r1[];
extern const uint8_t ecc_der_header_bp192r1[];
extern const uint8_t ecc_der_header_bp224r1[];
extern const uint8_t ecc_der_header_bp256r1[];
extern const uint8_t ecc_der_header_bp320r1[];
extern const uint8_t ecc_der_header_bp384r1[];
extern const uint8_t ecc_der_header_bp512r1[];

extern size_t const der_ecc_secp192_header_len;
extern size_t const der_ecc_secp224_header_len;
extern size_t const der_ecc_secp256_header_len;
extern size_t const der_ecc_secp384_header_len;
extern size_t const der_ecc_secp521_header_len;
extern size_t const der_ecc_secp160k1_header_len;
extern size_t const der_ecc_secp192k1_header_len;
extern size_t const der_ecc_secp224k1_header_len;
extern size_t const der_ecc_secp256k1_header_len;
extern size_t const der_ecc_bp160r1_header_len;
extern size_t const der_ecc_bp192r1_header_len;
extern size_t const der_ecc_bp224r1_header_len;
extern size_t const der_ecc_bp256r1_header_len;
extern size_t const der_ecc_bp320r1_header_len;
extern size_t const der_ecc_bp384r1_header_len;
extern size_t const der_ecc_bp512r1_header_len;

/* ********************** Defines ********************** */
#define MAX_CACHE_OBJECT 300
#define AES_BLOCK_SIZE 16
#define DES_BLOCK_SIZE 8
#define MAX_PKCS11_SESSIONS 3
#define PKCS11_TOKEN_LABEL                               \
    {                                                    \
        'S', 'S', 'S', '_', 'P', 'K', 'C', 'S', '1', '1' \
    }
#define PKCS11_MANUFACTURER \
    {                       \
        'N', 'X', 'P'       \
    }
#define PKCS11_LIBRARY_VERSION  \
    (CK_VERSION)                \
    {                           \
        .major = 4, .minor = 7, \
    }
#define CKA_SSS_ID CKA_VENDOR_DEFINED + CKA_OBJECT_ID
/**
 * @brief Definitions for parameter checking
 */
#define pkcs11CREATEOBJECT_MINIMUM_ATTRIBUTE_COUNT 2
/* Public key identifier for EC Keys */
#define ID_ECPUBLICKEY                           \
    {                                            \
        0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01 \
    }
#define pkcs11NO_OPERATION ((CK_MECHANISM_TYPE)0xFFFFFFFFF)
#define pkcs11INVALID_OBJECT_CLASS ((CK_OBJECT_CLASS)0x0FFFFFFF)
#define pkcs11INVALID_KEY_TYPE ((CK_KEY_TYPE)0x0FFFFFFF)
#define MAX_ID_LIST_SIZE 200
#define OID_START_INDEX 2
#define MAX_KEY_ID_LENGTH 4
#define MAX_PBKDF_REQ_LEN 512
#define MAX_RAW_SIGNATURE_SIZE 132

/* doc:start:pkcs11-max-obj-read */
/* Define max objects to read during C_FindObjects
 * Should not be more than MAX_ID_LIST_SIZE
 */
#define USER_MAX_ID_LIST_SIZE 1
/* doc:end:pkcs11-max-obj-read */

#if (USER_MAX_ID_LIST_SIZE > MAX_ID_LIST_SIZE)
#error "USER_MAX_ID_LIST_SIZE > MAX_ID_LIST_SIZE check failed"
#endif

/* Default policies */
#if SSS_HAVE_SE05X_VER_07_02
#define DEFAULT_POLICY_SYMM_KEY                                                                                       \
    (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_SIGN | POLICY_OBJ_ALLOW_VERIFY | POLICY_OBJ_ALLOW_ENC |               \
        POLICY_OBJ_ALLOW_DEC | POLICY_OBJ_ALLOW_HKDF | POLICY_OBJ_ALLOW_WRAP | POLICY_OBJ_ALLOW_WRITE |               \
        POLICY_OBJ_ALLOW_GEN | POLICY_OBJ_ALLOW_DESFIRE_AUTHENTICATION | POLICY_OBJ_ALLOW_DESFIRE_DUMP_SESSION_KEYS | \
        POLICY_OBJ_ALLOW_IMPORT_EXPORT)
#define DEFAULT_POLICY_ASYMM_KEY                                                                           \
    (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_SIGN | POLICY_OBJ_ALLOW_VERIFY | POLICY_OBJ_ALLOW_ENC |    \
        POLICY_OBJ_ALLOW_DEC | POLICY_OBJ_ALLOW_TLS_KDF | POLICY_OBJ_ALLOW_WRAP | POLICY_OBJ_ALLOW_WRITE | \
        POLICY_OBJ_ALLOW_GEN | POLICY_OBJ_ALLOW_KA | POLICY_OBJ_ALLOW_READ | POLICY_OBJ_ALLOW_IMPORT_EXPORT)
#else
#define DEFAULT_POLICY_SYMM_KEY                                                                                       \
    (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_SIGN | POLICY_OBJ_ALLOW_VERIFY | POLICY_OBJ_ALLOW_ENC |               \
        POLICY_OBJ_ALLOW_DEC | POLICY_OBJ_ALLOW_KDF | POLICY_OBJ_ALLOW_WRAP | POLICY_OBJ_ALLOW_WRITE |                \
        POLICY_OBJ_ALLOW_GEN | POLICY_OBJ_ALLOW_DESFIRE_AUTHENTICATION | POLICY_OBJ_ALLOW_DESFIRE_DUMP_SESSION_KEYS | \
        POLICY_OBJ_ALLOW_IMPORT_EXPORT)
#define DEFAULT_POLICY_ASYMM_KEY                                                                        \
    (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_SIGN | POLICY_OBJ_ALLOW_VERIFY | POLICY_OBJ_ALLOW_ENC | \
        POLICY_OBJ_ALLOW_DEC | POLICY_OBJ_ALLOW_KDF | POLICY_OBJ_ALLOW_WRAP | POLICY_OBJ_ALLOW_WRITE |  \
        POLICY_OBJ_ALLOW_GEN | POLICY_OBJ_ALLOW_KA | POLICY_OBJ_ALLOW_READ | POLICY_OBJ_ALLOW_IMPORT_EXPORT)
#endif // SSS_HAVE_SE05X_VER_07_02
#define DEFAULT_POLICY_BIN_COUNT_PCR (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE | POLICY_OBJ_ALLOW_READ)
#define DEFAULT_POLICY_USERID (POLICY_OBJ_ALLOW_DELETE | POLICY_OBJ_ALLOW_WRITE)

/*
 * Top level OID tuples
 */
#define MBEDTLS_OID_ISO_MEMBER_BODIES "\x2a" /* {iso(1) member-body(2)} */
#define MBEDTLS_OID_ISO_IDENTIFIED_ORG "\x2b" /* {iso(1) identified-organization(3)} */
#define MBEDTLS_OID_ISO_CCITT_DS "\x55" /* {joint-iso-ccitt(2) ds(5)} */
#define MBEDTLS_OID_ISO_ITU_COUNTRY "\x60" /* {joint-iso-itu-t(2) country(16)} */

/*
 * ISO Member bodies OID parts
 */
#define MBEDTLS_OID_COUNTRY_US "\x86\x48" /* {us(840)} */
#define MBEDTLS_OID_ORG_RSA_DATA_SECURITY "\x86\xf7\x0d" /* {rsadsi(113549)} */
#define MBEDTLS_OID_RSA_COMPANY                          \
    MBEDTLS_OID_ISO_MEMBER_BODIES MBEDTLS_OID_COUNTRY_US \
        MBEDTLS_OID_ORG_RSA_DATA_SECURITY /* {iso(1) member-body(2) us(840) rsadsi(113549)} */
#define MBEDTLS_OID_ORG_ANSI_X9_62 "\xce\x3d" /* ansi-X9-62(10045) */
#define MBEDTLS_OID_ANSI_X9_62 MBEDTLS_OID_ISO_MEMBER_BODIES MBEDTLS_OID_COUNTRY_US MBEDTLS_OID_ORG_ANSI_X9_62

/*
 * ISO Identified organization OID parts
 */
#define MBEDTLS_OID_ORG_DOD "\x06" /* {dod(6)} */
#define MBEDTLS_OID_ORG_OIW "\x0e"
#define MBEDTLS_OID_OIW_SECSIG MBEDTLS_OID_ORG_OIW "\x03"
#define MBEDTLS_OID_OIW_SECSIG_ALG MBEDTLS_OID_OIW_SECSIG "\x02"
#define MBEDTLS_OID_OIW_SECSIG_SHA1 MBEDTLS_OID_OIW_SECSIG_ALG "\x1a"
#define MBEDTLS_OID_ORG_CERTICOM "\x81\x04" /* certicom(132) */
#define MBEDTLS_OID_CERTICOM MBEDTLS_OID_ISO_IDENTIFIED_ORG MBEDTLS_OID_ORG_CERTICOM
#define MBEDTLS_OID_ORG_TELETRUST "\x24" /* teletrust(36) */
#define MBEDTLS_OID_TELETRUST MBEDTLS_OID_ISO_IDENTIFIED_ORG MBEDTLS_OID_ORG_TELETRUST

/*
 * ECParameters namedCurve identifiers, from RFC 5480, RFC 5639, and SEC2
 */

/* secp192r1 OBJECT IDENTIFIER ::= {
 *   iso(1) member-body(2) us(840) ansi-X9-62(10045) curves(3) prime(1) 1 } */
#define MBEDTLS_OID_EC_GRP_SECP192R1 MBEDTLS_OID_ANSI_X9_62 "\x03\x01\x01"

/* secp224r1 OBJECT IDENTIFIER ::= {
 *   iso(1) identified-organization(3) certicom(132) curve(0) 33 } */
#define MBEDTLS_OID_EC_GRP_SECP224R1 MBEDTLS_OID_CERTICOM "\x00\x21"

/* secp256r1 OBJECT IDENTIFIER ::= {
 *   iso(1) member-body(2) us(840) ansi-X9-62(10045) curves(3) prime(1) 7 } */
#define MBEDTLS_OID_EC_GRP_SECP256R1 MBEDTLS_OID_ANSI_X9_62 "\x03\x01\x07"

/* secp384r1 OBJECT IDENTIFIER ::= {
 *   iso(1) identified-organization(3) certicom(132) curve(0) 34 } */
#define MBEDTLS_OID_EC_GRP_SECP384R1 MBEDTLS_OID_CERTICOM "\x00\x22"

/* secp521r1 OBJECT IDENTIFIER ::= {
 *   iso(1) identified-organization(3) certicom(132) curve(0) 35 } */
#define MBEDTLS_OID_EC_GRP_SECP521R1 MBEDTLS_OID_CERTICOM "\x00\x23"

/* secp192k1 OBJECT IDENTIFIER ::= {
 *   iso(1) identified-organization(3) certicom(132) curve(0) 31 } */
#define MBEDTLS_OID_EC_GRP_SECP192K1 MBEDTLS_OID_CERTICOM "\x00\x1f"

/* secp224k1 OBJECT IDENTIFIER ::= {
 *   iso(1) identified-organization(3) certicom(132) curve(0) 32 } */
#define MBEDTLS_OID_EC_GRP_SECP224K1 MBEDTLS_OID_CERTICOM "\x00\x20"

/* secp256k1 OBJECT IDENTIFIER ::= {
 *   iso(1) identified-organization(3) certicom(132) curve(0) 10 } */
#define MBEDTLS_OID_EC_GRP_SECP256K1 MBEDTLS_OID_CERTICOM "\x00\x0a"

/* secp160k1 */
#define OID_EC_GRP_SECP160K1 "\x2B\x81\x04\x00\x09"

/* brainpoolP160r1 */
#define OID_EC_GRP_BP160R1 "\x2B\x24\x03\x03\x02\x08\x01\x01\x01"

/* brainpoolP192r1 */
#define OID_EC_GRP_BP192R1 "\x2B\x24\x03\x03\x02\x08\x01\x01\x03"

/* brainpoolP224r1 */
#define OID_EC_GRP_BP224R1 "\x2B\x24\x03\x03\x02\x08\x01\x01\x05"

/* brainpoolP256r1 */
#define OID_EC_GRP_BP256R1 "\x2B\x24\x03\x03\x02\x08\x01\x01\x07"

/* brainpoolP320r1 */
#define OID_EC_GRP_BP320R1 "\x2B\x24\x03\x03\x02\x08\x01\x01\x09"

/* brainpoolP384r1 */
#define OID_EC_GRP_BP384R1 "\x2B\x24\x03\x03\x02\x08\x01\x01\x0B"

/* brainpoolP512r1 */
#define OID_EC_GRP_BP512R1 "\x2B\x24\x03\x03\x02\x08\x01\x01\x0D"

/* ********************** structure definition *************** */

/**
 * @brief Session structure.
 */
typedef struct P11Session
{
    CK_ULONG ulState;
    CK_BBOOL xOpened;
    CK_MECHANISM_TYPE xOperationInProgress;
    CK_BBOOL digestUpdateCalled;
    CK_OBJECT_HANDLE xOperationKeyHandle;
    CK_BBOOL xFindObjectInit;
    CK_OBJECT_CLASS xFindObjectClass;
    uint32_t xFindObjectTotalFound;
    uint16_t xFindObjectOutputOffset;
    CK_KEY_TYPE xFindObjectKeyType;
    CK_BBOOL labelPresent;
    CK_BBOOL keyIdPresent;
    char keyId[MAX_KEY_ID_LENGTH];
    char label[32];
    size_t labelLen;
    void *mechParameter;
    CK_ULONG mechParameterLen;
    sss_digest_t digest_ctx;
    CK_FLAGS xFlags;
    sss_mac_t ctx_hmac;
} P11Session_t, *P11SessionPtr_t;

#if SSS_HAVE_SE05X_VER_GTE_07_02
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
#endif //#if SSS_HAVE_SE05X_VER_GTE_07_02

/* ********************** FUnction declarations ********************** */

CK_RV pkcs11_parse_sign_mechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV pkcs11_parse_encryption_mechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV pkcs11_parse_digest_mechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV pkcs11_get_attribute_parameter_index(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ATTRIBUTE_TYPE type, CK_ULONG_PTR index);
CK_RV pkcs11_get_digest_algorithm(const sss_algorithm_t algorithm, sss_algorithm_t *digest_algo);
CK_RV pkcs11_setASNTLV(uint8_t tag, uint8_t *component, const size_t componentLen, uint8_t *key, size_t *keyLen);
CK_RV pkcs11_read_object_size(uint32_t keyId, uint16_t *keyLen);
CK_RV pkcs11_ecSignatureToRandS(uint8_t *signature, size_t *sigLen);
CK_RV pkcs11_ecRandSToSignature(uint8_t *rands, const size_t rands_len, uint8_t *output, size_t *outputLen);
CK_RV pkcs11_ecPublickeyGetEcParams(uint8_t *input, size_t *dataLen);
CK_BBOOL pkcs11_is_X509_certificate(uint32_t xObject);
CK_RV pkcs11_is_valid_keytype(sss_algorithm_t algorithm, sss_cipher_type_t *cipher, sss_object_t *pSSSObject);
CK_RV pkcs11_se05x_asymmetric_encrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV pkcs11_se05x_asymmetric_decrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen);
CK_RV pkcs11_se05x_symmetric_encrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV pkcs11_se05x_symmetric_decrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV pkcs11_label_to_keyId(unsigned char *label, size_t labelSize, uint32_t *keyId);
CK_RV pkcs11_parse_certificate_get_attribute(
    uint32_t xObject, CK_ATTRIBUTE_TYPE attributeType, uint8_t *pData, CK_ULONG *ulAttrLength);
#if SSS_HAVE_SE05X_VER_GTE_07_02
sss_status_t pkcs11_parse_atrribute(se05x_object_attribute *pAttribute,
    uint8_t *rsp,
    size_t rspLen,
    uint32_t objectType,
    uint32_t cipherType,
    uint32_t policy_map,
    CK_BBOOL *pAllow);
#endif // #if SSS_HAVE_SE05X_VER_GTE_07_02
smStatus_t pkcs11_read_id_list(
    CK_SESSION_HANDLE xSession, uint32_t *idlist, size_t *idlistlen, CK_ULONG ulMaxObjectCount);
sss_status_t pkcs11_get_validated_object_id(P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, uint8_t *keyIdbuff);
sss_status_t pkcs11_get_validated_sss_object(
    P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, sss_object_t *pSSSObject);
P11SessionPtr_t prvSessionPointerFromHandle(CK_SESSION_HANDLE xSession);
sss_status_t pkcs11_sss_create_token(sss_key_store_t *keystore,
    sss_object_t *CreateObject,
    U32 ObjectId,
    sss_key_part_t KeyPart,
    sss_cipher_type_t CipherType,
    U8 *buffer,
    U32 bufferLen,
    U32 bitLen);

CK_RV pkcs11_get_ec_info(uint8_t *params, size_t *KeyBitLen, sss_cipher_type_t *cipher);
CK_RV pkcs11_add_ec_header(uint16_t keySize, sss_cipher_type_t cipher, uint8_t *pubKeyBuf, size_t *pubKeyBufLen, CK_BYTE_PTR pubData, CK_ULONG pubDataLen, size_t *keyBitLen);

/* Mutex handling function */
int sss_pkcs11_mutex_init(void);
int sss_pkcs11_mutex_lock(void);
int sss_pkcs11_mutex_unlock(void);
int sss_pkcs11_mutex_destroy(void);

#endif // __SSS_PKCS11_PAL_H__
