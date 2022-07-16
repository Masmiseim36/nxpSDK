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
#include "FreeRTOSIPConfig.h"
#include "iot_crypto.h"
#include "semphr.h"
#include "task.h"
#endif
#include "core_pkcs11.h"

/* mbedTLS includes. */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/base64.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/oid.h"
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"
#include "pkcs11_mbedtls_utils.h"
#if defined(USE_RTOS) && USE_RTOS == 1
#include "aws_clientcredential.h"
#endif

/*Other includes */
#include <nxEnsure.h>
// #define NX_LOG_ENABLE_PKCS11_DEBUG 1
#include <nxLog_pkcs11.h>

#include "HLSEAPI.h"
#include "ex_sss.h"
#if SSS_HAVE_MBEDTLS_ALT_A71CH
#include "ax_mbedtls.h"
#endif
#if SSS_HAVE_MBEDTLS_ALT_SSS
#include "sss_mbedtls.h"
#endif

#if SSS_HAVE_APPLET_SE05X_IOT
#include <fsl_sss_se05x_apis.h>
#include <se05x_APDU.h>
#include <se05x_const.h>

#include "se05x_APDU_apis.h"
#include "se05x_enums.h"
#include "se05x_tlv.h"
#endif

#if defined(PKCS11_LIBRARY)
#include <projdefs.h>

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

typedef int (*pfnMbedTlsSign)(void *ctx,
    mbedtls_md_type_t md_alg,
    const unsigned char *hash,
    size_t hash_len,
    unsigned char *sig,
    size_t *sig_len,
    int (*f_rng)(void *, unsigned char *, size_t),
    void *p_rng);

typedef struct
{
    sss_object_t SSSObjects[USER_MAX_ID_LIST_SIZE];
    size_t keyIdListLen;
} SwKeyStore_t, *SwKeyStorePtr_t;

/**
 * @brief Key structure.
 */
typedef struct P11Key
{
    mbedtls_pk_context xMbedPkCtx;
    mbedtls_x509_crt xMbedX509Cli;
    mbedtls_pk_info_t xMbedPkInfo;
    pfnMbedTlsSign pfnSavedMbedSign;
    void *pvSavedMbedPkCtx;
    uint16_t pem_len_client;
    uint16_t pem_len_client_ca;
    unsigned char *certificate_buf; //[2500];
} P11Key_t, *P11KeyPtr_t;

/**
 * @brief Session structure.
 */
typedef struct P11Session
{
    P11KeyPtr_t pxCurrentKey;
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
    mbedtls_ctr_drbg_context xMbedDrbgCtx;
    mbedtls_entropy_context xMbedEntropyContext;
    mbedtls_pk_context xPublicKey;
    mbedtls_sha256_context xSHA256Context;
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
CK_RV GetSSSAlgorithm(const sss_algorithm_t algorithm, sss_algorithm_t *digest_algo);
CK_RV SetASNTLV(uint8_t tag, uint8_t *component, const size_t componentLen, uint8_t *key, size_t *keyLen);
#if SSS_HAVE_APPLET_SE05X_IOT
CK_RV read_object_size(uint32_t keyId, uint16_t *keyLen);
#endif // SSS_HAVE_APPLET_SE05X_IOT
CK_RV CreateRawPrivateKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key, size_t *keyLen);
CK_RV CreateRawPublicKey(CK_ATTRIBUTE_PTR pxTemplate, CK_ULONG ulCount, uint8_t *key, size_t *keyLen);
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
