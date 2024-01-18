/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SSS_PKCS11_PAL_H__
#define __SSS_PKCS11_PAL_H__

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if (SSS_HAVE_SSCP || SSS_HAVE_APPLET_SE05X_IOT || SSS_HAVE_APPLET_NONE)
/* FreeRTOS includes. */
#if defined(USE_RTOS) && USE_RTOS == 1
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#endif

#include "sss_pkcs11_utils.h"
#include "core_pkcs11.h"

/*Other includes */
#include <nxEnsure.h>
// #define NX_LOG_ENABLE_PKCS11_DEBUG 1
#include <nxLog_pkcs11.h>

#include "ex_sss.h"

/*  se05x includes. */
#if SSS_HAVE_APPLET_SE05X_IOT
#include <fsl_sss_se05x_apis.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include "se05x_APDU_apis.h"
#include "se05x_enums.h"
#include "se05x_tlv.h"
#endif

#if defined(PKCS11_LIBRARY)

#if (__GNUC__ && !AX_EMBEDDED)
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
/* Only for base session with os */
static pthread_mutex_t gSessionlock;
static pthread_mutex_t gFilelock;
#endif

#endif

#if defined(USE_RTOS) && USE_RTOS == 1
extern SemaphoreHandle_t xSemaphore;
#endif

/* C runtime includes. */
#include <PlugAndTrust_Pkg_Ver.h>
#include <fsl_sss_util_asn1_der.h>
#include "ex_sss_ports.h"
#include <ex_sss_boot.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AES_BLOCK_SIZE 16

/* Definitions */

#define PKCS11_TOKEN_LABEL                               \
    {                                                    \
        'S', 'S', 'S', '_', 'P', 'K', 'C', 'S', '1', '1' \
    }

#define PKCS11_MANUFACTURER \
    {                       \
        'N', 'X', 'P'       \
    }

#define PKCS11_LIBRARY_VERSION                                            \
    (CK_VERSION)                                                          \
    {                                                                     \
        .major = PLUGANDTRUST_VER_MAJOR, .minor = PLUGANDTRUST_VER_MINOR, \
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

/* Mutex locking */
#if defined(USE_RTOS) && USE_RTOS == 1
#define LOCK_MUTEX_FOR_RTOS if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
#else
#define LOCK_MUTEX_FOR_RTOS
#endif

#if defined(USE_RTOS) && USE_RTOS == 1
#define UNLOCK_MUTEX_FOR_RTOS_RET(RET) \
    xSemaphoreGive(xSemaphore);        \
    return RET;
#else
#define UNLOCK_MUTEX_FOR_RTOS_RET(RET) return RET;
#endif

#if defined(USE_RTOS) && USE_RTOS == 1
#define UNLOCK_MUTEX_FOR_RTOS xSemaphoreGive(xSemaphore);
#else
#define UNLOCK_MUTEX_FOR_RTOS
#endif

#if defined(USE_RTOS) && USE_RTOS == 1
#define UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(CONDITION) \
    if (!(CONDITION)) {                               \
        NX_ENSURE_MESSAGE(#CONDITION);                \
        xSemaphoreGive(xSemaphore);                   \
        goto exit;                                    \
    }
#else
#define UNLOCK_MUTEX_FOR_RTOS_EXIT_ON_FAIL(CONDITION) ENSURE_OR_GO_EXIT(CONDITION)
#endif

#define MAX_ID_LIST_SIZE 200
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
#define MBEDTLS_OID_ISO_MEMBER_BODIES "\x2a"  /* {iso(1) member-body(2)} */
#define MBEDTLS_OID_ISO_IDENTIFIED_ORG "\x2b" /* {iso(1) identified-organization(3)} */
#define MBEDTLS_OID_ISO_CCITT_DS "\x55"       /* {joint-iso-ccitt(2) ds(5)} */
#define MBEDTLS_OID_ISO_ITU_COUNTRY "\x60"    /* {joint-iso-itu-t(2) country(16)} */

/*
 * ISO Member bodies OID parts
 */
#define MBEDTLS_OID_COUNTRY_US "\x86\x48"                /* {us(840)} */
#define MBEDTLS_OID_ORG_RSA_DATA_SECURITY "\x86\xf7\x0d" /* {rsadsi(113549)} */
#define MBEDTLS_OID_RSA_COMPANY                          \
    MBEDTLS_OID_ISO_MEMBER_BODIES MBEDTLS_OID_COUNTRY_US \
        MBEDTLS_OID_ORG_RSA_DATA_SECURITY     /* {iso(1) member-body(2) us(840) rsadsi(113549)} */
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

/* State of the Keypair*/
typedef enum
{
    PrivateKeySize = 0, // This state returns the size of private key
    PrivateKeyAttr,     // This state returns the attributes of private key
    PublicKeySize,      // This state returns the size of public key
    PublicKeyAttr,      // This state returns the attributes of public key
} key_state_t;

typedef struct
{
    sss_object_t SSSObjects[USER_MAX_ID_LIST_SIZE];
    size_t keyIdListLen;
} SwKeyStore_t, *SwKeyStorePtr_t;

/**
 * @brief Handling keypair structure.
 */
typedef struct HandleP11KeyPair
{
    CK_BBOOL xSetPublicKey;
    uint8_t keyState;
    CK_OBJECT_HANDLE keyPairObjHandle;
} HandleP11KeyPair_t, *HandleP11KeyPairPtr_t;

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
    HandleP11KeyPairPtr_t pFindObject;
    HandleP11KeyPairPtr_t pAttrKey;
    CK_BBOOL labelPresent;
    CK_BBOOL keyIdPresent;
    uint32_t keyId;
    char label[32];
    size_t labelLen;
    void *mechParameter;
    CK_ULONG mechParameterLen;
    sss_digest_t digest_ctx;
    SwKeyStorePtr_t pCurrentKs;
} P11Session_t, *P11SessionPtr_t;

#define MAX_PKCS11_SESSIONS 3

#if SSS_HAVE_SE05X_VER_GTE_06_00
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
#endif

/* Function declarations */
CK_RV ParseSignMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV ParseEncryptionMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV ParseDigestMechanism(P11SessionPtr_t pxSession, sss_algorithm_t *algorithm);
CK_RV GetAttributeParameterIndex(
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_ATTRIBUTE_TYPE type, CK_ULONG_PTR index);
CK_RV GetDigestAlgorithm(const sss_algorithm_t algorithm, sss_algorithm_t *digest_algo);
CK_RV SetASNTLV(uint8_t tag, uint8_t *component, const size_t componentLen, uint8_t *key, size_t *keyLen);
#if SSS_HAVE_APPLET_SE05X_IOT
CK_RV read_object_size(uint32_t keyId, uint16_t *keyLen);
#endif // SSS_HAVE_APPLET_SE05X_IOT
CK_RV EcSignatureToRandS(uint8_t *signature, size_t *sigLen);
CK_RV EcRandSToSignature(uint8_t *rands, const size_t rands_len, uint8_t *output, size_t *outputLen);
CK_RV EcPublickeyGetEcParams(uint8_t *input, size_t *dataLen);
CK_BBOOL isX509Certificate(uint32_t xObject);
CK_RV AsymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV AsymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDataLen);
CK_RV SymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV SymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen);
CK_RV LabelToKeyId(unsigned char *label, size_t labelSize, uint32_t *keyId);
CK_RV parseCertificateGetAttribute(
    uint32_t xObject, CK_ATTRIBUTE_TYPE attributeType, uint8_t *pData, CK_ULONG *ulAttrLength);
#if SSS_HAVE_SE05X_VER_GTE_06_00
sss_status_t parseAtrribute(se05x_object_attribute *pAttribute,
    uint8_t *rsp,
    size_t rspLen,
    uint32_t objectType,
    uint32_t cipherType,
    uint32_t policy_map,
    CK_BBOOL *pAllow);
#endif // SSS_HAVE_SE05X_VER_GTE_06_00
// smStatus_t read_id_list(uint32_t *idlist, size_t *idlistlen);
#if SSS_HAVE_APPLET_SE05X_IOT
smStatus_t read_id_list(CK_SESSION_HANDLE xSession, uint32_t *idlist, size_t *idlistlen, CK_ULONG ulMaxObjectCount);
#endif // SSS_HAVE_APPLET_SE05X_IOT
sss_status_t get_validated_object_id(P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, uint32_t *keyId);
sss_status_t get_validated_cipher_type(
    P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, sss_cipher_type_t *cipherType);
sss_status_t get_validated_sss_object(P11SessionPtr_t pxSession, CK_OBJECT_HANDLE xObject, sss_object_t *pSSSObject);
P11SessionPtr_t prvSessionPointerFromHandle(CK_SESSION_HANDLE xSession);

sss_status_t sss_create_token(sss_key_store_t *keystore,
    sss_object_t *CreateObject,
    U32 ObjectId,
    sss_key_part_t KeyPart,
    sss_cipher_type_t CipherType,
    U8 *buffer,
    U32 bufferLen,
    U32 bitLen);
#if SSS_HAVE_SSCP
U16 HLSE_Create_token(
    uint32_t keyId, HLSE_OBJECT_TYPE objType, void *buff, unsigned long bufflen, HLSE_OBJECT_HANDLE handle_object);
#endif

extern ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx;
extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;
#endif // (SSS_HAVE_SSCP || SSS_HAVE_APPLET_SE05X_IOT || SSS_HAVE_APPLET_NONE)

#endif // __SSS_PKCS11_PAL_H__
