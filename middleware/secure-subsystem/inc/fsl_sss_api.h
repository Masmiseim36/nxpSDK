/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_SSS_H_
#define _FSL_SSS_H_

#if !defined(SSS_CONFIG_FILE)
#include "fsl_sss_config.h"
#else
#include SSS_CONFIG_FILE
#endif

#include "fsl_sss_types.h"

#define SSS_API_VERSION (0x00000001u)

#define SSS_AES_BLOCK_SIZE (16u)
#define SSS_DES_BLOCK_SIZE (8u)
#define SSS_DES_KEY_SIZE (8u)
#define SSS_DES_IV_SIZE (8u)

typedef enum _sss_status
{
    kStatus_SSS_Success = (int)0x5a5a5a5a,
    kStatus_SSS_Fail = (int)0xc3c30000,
    kStatus_SSS_InvalidArgument = (int)0xc3c30001,
    kStatus_SSS_ResourceBusy = (int)0xc3c30002,
} sss_status_t;

typedef enum _sss_type
{
    kType_SSS_SubSystem_NONE,
    kType_SSS_Software,
    kType_SSS_mbedTLS,
    kType_SSS_OpenSSL,
    kType_SSS_SECO,
    kType_SSS_Sentinel200,
    kType_SSS_Sentinel300,
    kType_SSS_Sentinel400,
    kType_SSS_Sentinel500,
    kType_SSS_SecureElement,
    kType_SSS_SE_A71CH,
    kType_SSS_SE_SE050,
    kType_SSS_HW,
    kType_SSS_SubSystem_LAST
} sss_type_t;

typedef enum _sss_algorithm
{
    /* AES */
    kAlgorithm_SSS_AES_ECB,
    kAlgorithm_SSS_AES_CBC,
    kAlgorithm_SSS_AES_CTR,
    kAlgorithm_SSS_AES_GCM,
    kAlgorithm_SSS_AES_CCM,
    /* CHACHA_POLY */
    kAlgorithm_SSS_CHACHA_POLY,
    /* DES3 */
    kAlgorithm_SSS_DES3_ECB,
    kAlgorithm_SSS_DES3_CBC,
    /* digest */
    kAlgorithm_SSS_SHA1,
    kAlgorithm_SSS_SHA224,
    kAlgorithm_SSS_SHA256,
    kAlgorithm_SSS_SHA384,
    kAlgorithm_SSS_SHA512,
    /* MAC */
    kAlgorithm_SSS_CMAC_AES,
    kAlgorithm_SSS_HMAC_SHA256,
    /* Diffie-Helmann */
    kAlgorithm_SSS_DH,
    kAlgorithm_SSS_ECDH,
    /* DSA */
    kAlgorithm_SSS_DSA_SHA1,
    kAlgorithm_SSS_DSA_SHA224,
    kAlgorithm_SSS_DSA_SHA256,
    /* RSA */
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512,
    /* ECDSA */
    kAlgorithm_SSS_ECDSA_SHA1,
    kAlgorithm_SSS_ECDSA_SHA224,
    kAlgorithm_SSS_ECDSA_SHA256,
    kAlgorithm_SSS_ECDSA_SHA384,
    kAlgorithm_SSS_ECDSA_SHA512,
} sss_algorithm_t;

typedef enum _sss_mode
{
    kMode_SSS_Encrypt,
    kMode_SSS_Decrypt,
    kMode_SSS_Sign,
    kMode_SSS_Verify,
    kMode_SSS_ComputeSharedSecret,
    kMode_SSS_Digest,
    kMode_SSS_Mac,
} sss_mode_t;

typedef enum _sss_access_permission
{
    kAccessPermission_SSS_Read = (1u << 0),
    kAccessPermission_SSS_Write = (1u << 1),
    kAccessPermission_SSS_Use = (1u << 2),
    kAccessPermission_SSS_Delete = (1u << 3),
    kAccessPermission_SSS_ChangeAttributes = (1u << 4),
} sss_access_permission_t;

typedef enum _sss_key_object_mode
{
    kKeyObject_Mode_None = 0,
    kKeyObject_Mode_Persistent,
    kKeyObject_Mode_Transient,
} sss_key_object_mode_t;

typedef enum _sss_key_type
{
    kSSS_KeyType_NONE,
    kSSS_KeyType_Certificate,
    kSSS_KeyType_AES,
    kSSS_KeyType_DES,
    kSSS_KeyType_MAC,
    kSSS_KeyType_RSA_Public,
    kSSS_KeyType_ECC_Public, /*! Weierstrass form elliptic curve public key  */
    kSSS_KeyType_ECM_Public, /*! Montgomery form elliptic curve public key  */
    kSSS_KeyType_ECT_Public, /*! twisted Edwards form elliptic curve public key  */
    kSSS_KeyType_RSA_Private,
    kSSS_KeyType_ECC_Private,
    kSSS_KeyType_ECM_Private,
    kSSS_KeyType_ECT_Private,
    kSSS_KeyType_RSA_Pair,
    kSSS_KeyType_ECC_Pair,
    kSSS_KeyType_ECM_Pair,
    kSSS_KeyType_ECT_Pair,
} sss_key_type_t;

typedef struct _sss_ecc_point
{
    uint8_t *X;
    uint8_t *Y;
} sss_ecc_point_t;

typedef struct _sss_eccgfp_group
{
    uint8_t *p;
    uint8_t *a;
    uint8_t *b;
    sss_ecc_point_t *G;
    uint8_t *n;
    uint8_t *h;
} sss_eccgfp_group_t;

typedef struct _sss_session
{
    /*! Indicates which security subsystem is selected to be used. */
    sss_type_t subsystem;

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_SESSION_MAX_CONTEXT_SIZE];
    } context;
} sss_session_t;

typedef struct _sss_key_store
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_session_t *session;
    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_KEY_STORE_MAX_CONTEXT_SIZE];
    } context;
} sss_key_store_t;

typedef struct _sss_object
{
    /*! key store holding the data and other properties */
    sss_key_store_t *keyStore;
    /*! TODO define object types */
    uint32_t objectType;
    /*! Application specific key identifier. The keyId is kept in the key  store
     * along with the key data and other properties. */
    uint32_t keyId;

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_KEY_OBJECT_MAX_CONTEXT_SIZE];
    } context;
} sss_object_t;

/*! @brief Typedef for the symmetric crypto context */
typedef struct _sss_symmetric
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_session_t *session;
    sss_object_t *keyObject;   /*!< Reference to key and it's properties. */
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_SYMMETRIC_MAX_CONTEXT_SIZE];
    } context;
} sss_symmetric_t;

typedef struct _sss_aead
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_session_t *session;
    sss_object_t *keyObject;   /*! Reference to key and it's properties. */
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_AEAD_MAX_CONTEXT_SIZE];
    } context;
} sss_aead_t;

typedef struct _sss_digest
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_session_t *session;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */
    /*! Full digest length per algorithm definition. This field is initialized along with algorithm. */
    size_t digestFullLen;
    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_DIGEST_MAX_CONTEXT_SIZE];
    } context;
} sss_digest_t;

typedef struct _sss_mac
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_session_t *session;
    sss_object_t *keyObject;   /*! Reference to key and it's properties. */
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_MAC_MAX_CONTEXT_SIZE];
    } context;
} sss_mac_t;

typedef struct _sss_asymmetric
{
    sss_session_t *session;
    sss_object_t *keyObject;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */
    size_t signatureFullLen;

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_ASYMMETRIC_MAX_CONTEXT_SIZE];
    } context;
} sss_asymmetric_t;

typedef struct _sss_tunnel
{
    sss_session_t *session;
    uint32_t tunnelType;

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_TUNNEL_MAX_CONTEXT_SIZE];
    } context;
} sss_tunnel_t;

typedef struct _sss_derive_key
{
    sss_session_t *session;
    sss_object_t *keyObject;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_DERIVE_KEY_MAX_CONTEXT_SIZE];
    } context;
} sss_derive_key_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup sss_crypto_symmetric
 * @{
 */

/*! @brief Symmetric context init.
 *  The function initializes symmetric context with initial values.
 *
 * @param context Pointer to symmetric crypto context.
 * @param session Associate SSS session with symmetric context.
 * @param keyObject Associate SSS key object with symmetric context.
 * @param algorithm One of the symmetric algorithms defined by @ref sss_algorithm_t.
 * @param mode One of the modes defined by @ref sss_mode_t.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_symmetric_context_init(sss_symmetric_t *context,
                                        sss_session_t *session,
                                        sss_object_t *keyObject,
                                        sss_algorithm_t algorithm,
                                        sss_mode_t mode);

/*! @brief Symmetric cipher in one blocking function call.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to symmetric crypto context.
 * @param iv Buffer containing the symmetric operation Initialization Vector.
 * @param ivLen Length of the Initialization Vector in bytes.
 * @param srcData Buffer containing the input data.
 * @param destData Buffer containing the output data.
 * @param dataLen Size of input and output data buffer in bytes.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_cipher_one_go(
    sss_symmetric_t *context, uint8_t *iv, size_t ivLen, const uint8_t *srcData, uint8_t *destData, size_t dataLen);

/*! @brief Symmetric cipher init.
 *  The function starts the symmetric cipher operation.
 *
 * @param context Pointer to symmetric crypto context.
 * @param iv Buffer containing the symmetric operation Initialization Vector.
 * @param ivLen Length of the Initialization Vector in bytes.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_cipher_init(sss_symmetric_t *context, uint8_t *iv, size_t ivLen);

/*! @brief Symmetric cipher update.
 * Input data does not have to be a multiple of block size. Subsequent calls to this function are possible.
 * Unless one or more calls of this function have supplied sufficient input data, no output is generated.
 * The cipher operation is finalized with a call to @ref sss_cipher_finish().
 *
 * @param context Pointer to symmetric crypto context.
 * @param srcData Buffer containing the input data.
 * @param srcLen Length of the input data in bytes.
 * @param destData Buffer containing the output data.
 * @param[in,out] destLen Length of the output data in bytes. Buffer length on entry, reflects actual output size on
 * return.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_cipher_update(
    sss_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/*! @brief Symmetric cipher finalize.
 *
 * @param context Pointer to symmetric crypto context.
 * @param srcData Buffer containing final chunk of input data.
 * @param srcLen Length of final chunk of input data in bytes.
 * @param destData Buffer containing output data.
 * @param[in,out] destLen Length of output data in bytes. Buffer length on entry, reflects actual output size on
 * return.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_cipher_finish(
    sss_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/*! @brief Symmetric AES in Counter mode in one blocking function call.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to symmetric crypto context.
 * @param srcData Buffer containing the input data.
 * @param destData Buffer containing the output data.
 * @param size Size of source and destination data buffers in bytes.
 * @param[in,out] initialCounter Input counter (updates on return)
 * @param[out] lastEncryptedCounter Output cipher of last counter, for chained CTR calls. NULL can be passed if
 * chained calls are not used.
 * @param[out] szLeft Output number of bytes in left unused in lastEncryptedCounter block. NULL can be passed if
 * chained calls are not used.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_cipher_crypt_ctr(sss_symmetric_t *context,
                                  const uint8_t *srcData,
                                  uint8_t *destData,
                                  size_t size,
                                  uint8_t *initialCounter,
                                  uint8_t *lastEncryptedCounter,
                                  size_t *szLeft);

/*! @brief Symmetric context release.
 *  The function frees symmetric context.
 *
 * @param context Pointer to symmetric crypto context.
 */
void sss_symmetric_context_free(sss_symmetric_t *context);
/*!
 *@}
 */ /* end of sss_crypto_symmetric */

/*!
 * @addtogroup sss_crypto_aead
 * @{
 */

/*! @brief AEAD context init.
 *  The function initializes aead context with initial values.
 *
 * @param context Pointer to aead crypto context.
 * @param session Associate SSS session with aead context.
 * @param keyObject Associate SSS key object with aead context.
 * @param algorithm One of the aead algorithms defined by @ref sss_algorithm_t.
 * @param mode One of the modes defined by @ref sss_mode_t.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_aead_context_init(
    sss_aead_t *context, sss_session_t *session, sss_object_t *keyObject, sss_algorithm_t algorithm, sss_mode_t mode);

/*! @brief AEAD in one blocking function call.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to aead crypto context.
 * @param srcData Buffer containing the input data.
 * @param destData Buffer containing the output data.
 * @param size Size of input and output data buffer in bytes.
 * @param nonce The operation nonce or IV.
 * @param nonceLen The length of nonce in bytes. For AES-GCM it must be >= 1. For AES-CCM it must be 7, 8, 9, 10,
 * 11, 12, or 13.
 * @param aad Input additional authentication data AAD
 * @param aadLen Input size in bytes of AAD
 * @param tag Encryption: Output buffer filled with computed tag
 *            Decryption: Input buffer filled with received tag
 * @param tagLen Length of the tag in bytes.
 *               For AES-GCM it must be 4,8,12,13,14,15 or 16.
 *               For AES-CCM it must be 4,6,8,10,12,14 or 16.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_aead_one_go(sss_aead_t *context,
                             const uint8_t *srcData,
                             uint8_t *destData,
                             size_t size,
                             uint8_t *nonce,
                             size_t nonceLen,
                             const uint8_t *aad,
                             size_t aadLen,
                             uint8_t *tag,
                             size_t *tagLen);

/*! @brief AEAD init.
 *  The function starts the aead operation.
 *
 * @param context Pointer to aead crypto context.
 * @param nonce The operation nonce or IV.
 * @param nonceLen The length of nonce in bytes. For AES-GCM it must be >= 1. For AES-CCM it must be 7, 8, 9, 10,
 * 11, 12, or 13.
 * @param tagLen Length of the computed or received tag in bytes.
 *               For AES-GCM it must be 4,8,12,13,14,15 or 16.
 *               For AES-CCM it must be 4,6,8,10,12,14 or 16.
 * @param aadLen Input size in bytes of AAD. Used only for AES-CCM. Ignored for AES-GCM.
 * @param payloadLen Length in bytes of the payload. Used only for AES-CCM. Ignored for AES-GCM.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_aead_init(
    sss_aead_t *context, uint8_t *nonce, size_t nonceLen, size_t tagLen, size_t aadLen, size_t payloadLen);

/*! @brief Feeds a new chunk of the AAD.
 *  Subsequent calls of this function are possible.
 *
 * @param context Pointer to aead crypto context
 * @param aadData Input buffer containing the chunk of AAD
 * @param aadDataLen Length of the AAD data in bytes.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_aead_update_aad(sss_aead_t *context, const uint8_t *aadData, size_t aadDataLen);

/*! @brief AEAD data update.
 * Feeds a new chunk of the data payload.
 * Input data does not have to be a multiple of block size. Subsequent calls to this function are possible.
 * Unless one or more calls of this function have supplied sufficient input data, no output is generated.
 * The integration check is done by @ref sss_aead_finish(). Until then it is not sure if the decrypt data is
 * authentic.
 *
 * @param context Pointer to aead crypto context.
 * @param srcData Buffer containing the input data.
 * @param srcLen Length of the input data in bytes.
 * @param destData Buffer containing the output data.
 * @param[in,out] destLen Length of the output data in bytes. Buffer length on entry, reflects actual output size on
 * return.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_aead_update(
    sss_aead_t *context, const uint8_t *srcData, size_t srcLen, uint8_t destData, size_t *destLen);

/*! @brief Finalize AEAD.
 * The functions processes data that has not been processed by previous calls to sss_aead_update() as well as
 * srcData. It finalizes the AEAD operations and computes the tag (encryption) or compares the computed tag with the
 * tag supplied in the parameter (decryption).
 *
 * @param context Pointer to aead crypto context.
 * @param srcData Buffer containing final chunk of input data.
 * @param srcLen Length of final chunk of input data in bytes.
 * @param destData Buffer containing output data.
 * @param[in,out] destLen Length of output data in bytes. Buffer length on entry, reflects actual output size on
 * return.
 * @param tag Encryption: Output buffer filled with computed tag
 *            Decryption: Input buffer filled with received tag
 * @param tagLen Length of the computed or received tag in bytes.
 *               For AES-GCM it must be 4,8,12,13,14,15 or 16.
 *               For AES-CCM it must be 4,6,8,10,12,14 or 16.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_aead_finish(sss_aead_t *context,
                             const uint8_t *srcData,
                             size_t srcLen,
                             uint8_t destData,
                             size_t *destLen,
                             uint8_t *tag,
                             size_t *tagLen);

/*! @brief AEAD context release.
 *  The function frees aead context.
 *
 * @param context Pointer to aead context.
 */
void sss_aead_context_free(sss_aead_t *context);
/*!
 *@}
 */ /* end of sss_crypto_aead */

/*!
 * @addtogroup sss_crypto_digest
 * @{
 */

/*! @brief Digest context init.
 *  The function initializes digest context with initial values.
 *
 * @param context Pointer to digest context.
 * @param session Associate SSS session with digest context.
 * @param algorithm One of the digest algorithms defined by @ref sss_algorithm_t.
 * @param mode One of the modes defined by @ref sss_mode_t.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_digest_context_init(sss_digest_t *context,
                                     sss_session_t *session,
                                     sss_algorithm_t algorithm,
                                     sss_mode_t mode);

/*! @brief Message digest in one blocking function call.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to digest context.
 * @param message Input message
 * @param messageLen Length of the input message in bytes
 * @param digest Output message digest
 * @param digestLen Message digest byte length
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_digest_one_go(
    sss_digest_t *context, const uint8_t *message, size_t messageLen, uint8_t *digest, size_t *digestLen);

/*! @brief Init digest for a message.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to digest context.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_digest_init(sss_digest_t *context);

/*! @brief Update digest for a message.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to digest context.
 * @message Buffer with a message chunk.
 * @messageLen Length of the input buffer in bytes.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_digest_update(sss_digest_t *context, const uint8_t *message, size_t messageLen);

/*! @brief Finish digest for a message.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to digest context.
 * @param digest Output message digest
 * @param digestLen Message digest byte length
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_digest_finish(sss_digest_t *context, uint8_t *digest, size_t *digestLen);

/*! @brief Digest context release.
 *  The function frees digest context.
 *
 * @param context Pointer to digest context.
 */
void sss_digest_context_free(sss_digest_t *context);

/*!
 *@}
 */ /* end of sss_crypto_digest */

/*!
 * @addtogroup sss_crypto_mac
 * @{
 */

/*! @brief MAC context init.
 *  The function initializes mac context with initial values.
 *
 * @param context Pointer to mac context.
 * @param session Associate SSS session with mac context.
 * @param keyObject Associate SSS key object with mac context.
 * @param algorithm One of the mac algorithms defined by @ref sss_algorithm_t.
 * @param mode One of the modes defined by @ref sss_mode_t.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_mac_context_init(
    sss_mac_t *context, sss_session_t *session, sss_object_t *keyObject, sss_algorithm_t algorithm, sss_mode_t mode);

/*! @brief Message MAC in one blocking function call.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to mac context.
 * @param message Input message
 * @param messageLen Length of the input message in bytes
 * @param mac Output message MAC
 * @param macLen Computed MAC byte length
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_mac_one_go(
    sss_mac_t *context, const uint8_t *message, size_t messageLen, uint8_t *mac, size_t *macLen);

/*! @brief Init mac for a message.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to mac context.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_mac_init(sss_mac_t *context);

/*! @brief Update mac for a message.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to mac context.
 * @message Buffer with a message chunk.
 * @messageLen Length of the input buffer in bytes.
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_mac_update(sss_mac_t *context, const uint8_t *message, size_t messageLen);

/*! @brief Finish mac for a message.
 *  The function blocks current thread until the operation completes or an error occurs.
 *
 * @param context Pointer to mac context.
 * @param mac Output message MAC
 * @param macLen Computed MAC byte length
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 */
sss_status_t sss_mac_finish(sss_mac_t *context, uint8_t *mac, size_t *macLen);

/*! @brief MAC context release.
 *  The function frees mac context.
 *
 * @param context Pointer to mac context.
 */
void sss_mac_context_free(sss_mac_t *context);
/*!
 *@}
 */ /* end of sss_crypto_mac */

/*!
 * @addtogroup sss_crypto_asymmetric
 * @{
 */

/*! @brief Asymmetric context init.
 *  The function initializes asymmetric context with initial values.
 *
 * @param context Pointer to asymmetric crypto context.
 * @param session Associate SSS session with asymmetric context.
 * @param keyObject Associate SSS key object with asymmetric context.
 * @param algorithm One of the asymmetric algorithms defined by @ref sss_algorithm_t.
 * @param mode One of the modes defined by @ref sss_mode_t.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_asymmetric_context_init(sss_asymmetric_t *context,
                                         sss_session_t *session,
                                         sss_object_t *keyObject,
                                         sss_algorithm_t algorithm,
                                         sss_mode_t mode);

/*! @brief Asymmetric encryption
 *  The function uses asymmetric algorithm to encrypt data. Public key portion of a key pair is used for encryption.
 *
 * @param context Pointer to asymmetric context.
 * @param srcData Input buffer
 * @param srcLen Length of the input in bytes
 * @param destData Output buffer
 * @param destLen Length of the output in bytes
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_asymmetric_encrypt(
    sss_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/*! @brief Asymmetric decryption
 *  The function uses asymmetric algorithm to decrypt data. Private key portion of a key pair is used for
 * decryption.
 *
 * @param context Pointer to asymmetric context.
 * @param srcData Input buffer
 * @param srcLen Length of the input in bytes
 * @param destData Output buffer
 * @param destLen Length of the output in bytes
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_asymmetric_decrypt(
    sss_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/*! @brief Asymmetric signature of a message digest
 *  The function signs a message digest.
 *
 * @param context Pointer to asymmetric context.
 * @param digest Input buffer containing the input message digest
 * @param digestLen Length of the digest in bytes
 * @param signature Output buffer written with the signature of the digest
 * @param signatureLen Length of the signature in bytes
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_asymmetric_sign_digest(
    sss_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t *signatureLen);

/*! @brief Asymmetric verify of a message digest
 *  The function verifies a message digest.
 *
 * @param context Pointer to asymmetric context.
 * @param digest Input buffer containing the input message digest
 * @param digestLen Length of the digest in bytes
 * @param signature Input buffer containing the signature to verify
 * @param signatureLen Length of the signature in bytes
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_asymmetric_verify_digest(
    sss_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t signatureLen);

/*! @brief Asymmetric context release.
 *  The function frees asymmetric context.
 *
 * @param context Pointer to asymmetric context.
 */
void sss_asymmetric_context_free(sss_asymmetric_t *context);
/*!
 *@}
 */ /* end of sss_crypto_asymmetric */

/*!
 * @addtogroup sss_crypto_tunnelling
 * @{
 */

/*! @brief Constructor for the tunnelling service context.
 *
 * @param[out] context Pointer to tunnel context. Tunnel context is updated on function return.
 * @param session Pointer to session this tunnelling service belongs to.
 */
sss_status_t sss_init_tunnel_context(sss_tunnel_t *context, sss_session_t *session);

/*! @brief Tunnelling service.
 *
 * @param[in,out] context Pointer to tunnel context.
 * @param data Pointer to data to be send to subsystem.
 * @param dataLen Length of the data in bytes.
 * @param keyObjects Objects references used by the service.
 * @param keyObjectCount Number of key references at @ref keyObjects.
 * @param tunnelType Implementation specific id of the service.
 */
sss_status_t sss_tunnel(sss_tunnel_t *context,
                        uint8_t *data,
                        size_t dataLen,
                        sss_object_t *keyObjects,
                        uint32_t keyObjectCount,
                        uint32_t tunnelType);

/*! @brief Destructor for the tunnelling service context.
 *
 * @param[out] context Pointer to tunnel context. */
void sss_free_tunnel_context(sss_tunnel_t *context);

/*!
 *@}
 */ /* end of sss_crypto_tunnelling */

/*!
 * @addtogroup sss_crypto_derive_key
 * @{
 */

/*! @brief Derive key context init.
 *  The function initializes derive key context with initial values.
 *
 * @param context Pointer to derive key context.
 * @param session Associate SSS session with the derive key context.
 * @param keyObject Associate SSS key object with the derive key context.
 * @param algorithm One of the derive key algorithms defined by @ref sss_algorithm_t.
 * @param mode One of the modes defined by @ref sss_mode_t.
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_derive_key_context_init(sss_derive_key_t *context,
                                         sss_session_t *session,
                                         sss_object_t *keyObject,
                                         sss_algorithm_t algorithm,
                                         sss_mode_t mode);

/*! @brief Symmetric key derivation
 *  The function cryptographically derives a key from another key.
 *  For example MIFARE key derivation, PRF, HKDF-Extract.
 *
 * @param context Pointer to derive key context.
 * @param saltData Input data buffer, typically with some random data.
 * @param saltLen Length of saltData buffer in bytes.
 * @param info Input data buffer, typically with some fixed info.
 * @param infoLen Length of info buffer in bytes.
 * @param[in,out] derivedKeyObject Reference to a derived key
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_derive_key(sss_derive_key_t *context,
                            const uint8_t *saltData,
                            size_t saltLen,
                            const uint8_t *info,
                            size_t infoLen,
                            sss_object_t *derivedKeyObject);

/*! @brief Asymmetric key derivation Diffie-Helmann
 *  The function cryptographically derives a key from another key.
 *  For example Diffie-Helmann.
 *
 * @param context Pointer to derive key context.
 * @param otherPartyKeyObject Publick key of the other party in the Diffie-Helmann algorithm
 * @param[in,out] derivedKeyObject Reference to a derived key
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_asymmetric_dh_derive_key(sss_derive_key_t *context,
                                          sss_object_t *otherPartyKeyObject,
                                          sss_object_t *derivedKeyObject);

/*! @brief Derive key context release.
 *  The function frees derive key context.
 *
 * @param context Pointer to derive key context.
 */
void sss_derive_key_context_free(sss_derive_key_t *context);
/*!
 *@}
 */ /* end of sss_crypto_derive_key */

/*!
 * @addtogroup sss_key_object
 * @{
 */

/*! @brief Constructor for a key object data structure
 *  The function initializes keyObject data structure and associates it with a key store
 *  in which the plain key and other attributes are stored.
 *
 * @param keyObject
 * @param keyStore
 *
 * @returns Status of the operation
 * @retval kStatus_SSS_Success The operation has completed successfully.
 * @retval kStatus_SSS_Fail The operation has failed.
 * @retval kStatus_SSS_InvalidArgument One of the arguments is invalid for the function to execute.
 */
sss_status_t sss_key_object_init(sss_object_t *keyObject, sss_key_store_t *keyStore);

/**  @brief create new key
 *
 * @param[in,out] keyObject The object
 *        If required, update implementation defined values
 *        inside the keyObject
 * @param keyId External Key ID.  Later on this may be used by
 *        @ref sss_key_object_get_handle
 * @param keyType See @ref sss_key_type_t
 * @param keyByteLenMax Maximum storage this type of key may need.
 *        For systems that have their own internal allocation table
 *        this would help
 * @param options 0 = Persistant Key (Default) or Transient Key.
 *        FIXME: Check valid values of options
 * @return Status of object allocation.
 */
sss_status_t sss_key_object_allocate_handle(
    sss_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, uint32_t keyByteLenMax, uint32_t options);

/*! @brief get handle to existing
 *
 * See @ref sss_key_object_allocate_handle.
 *
 * Ideally keyObject should be same for sss_key_object_allocate_handle and
 * sss_key_object_get_handle
 * */
sss_status_t sss_key_object_get_handle(sss_object_t *keyObject, uint32_t keyId);

/*! @brief Assign user to a key object.
 * @param user Assign User id for a key object. The user is kept in the key
 *		  store along with the key data and other properties.
 * @param options Transient or persistent update. Allows for transient update
 * of persistent attributes.
 */
sss_status_t sss_key_object_set_user(sss_object_t *keyObject, uint32_t user, uint32_t options);

/*! @brief Assign purpose to a key object.
 *  @param purpose Usage of the key.
 *  @param options Transient or persistent update. Allows for transient update of persistent attributes.
 */
sss_status_t sss_key_object_set_purpose(sss_object_t *keyObject, sss_mode_t purpose, uint32_t options);

/*! @brief Assign access permissions to a key object.
 *  @param access Logical OR of read, write, delete, use, change attributes defined by enum _sss_access_permission.
 *  @param options Transient or persistent update. Allows for transient update of persistent attributes.
 */
sss_status_t sss_key_object_set_access(sss_object_t *keyObject, uint32_t access, uint32_t options);

/*! @brief Set elliptic curve domain parameters over Fp for a key object
 *  When the key object is a reference to one of ECC Private, ECC Public or ECC Pair key types,
 *  this function shall be used to specify the exact domain parameters prior to using the key object
 *  for ECDSA or ECDH algorithms.
 *  @param keyObject The destination key object
 *  @param group Pointer to elliptic curve domain parameters over Fp (sextuple p,a,b,G,n,h)
 */
sss_status_t sss_key_object_set_eccgfp_group(sss_object_t *keyObject, sss_eccgfp_group_t *group);

/*! @brief get attributes */
sss_status_t sss_key_object_get_user(sss_object_t *keyObject, uint32_t *user);
sss_status_t sss_key_object_get_purpose(sss_object_t *keyObject, sss_mode_t *purpose);
sss_status_t sss_key_object_get_access(sss_object_t *keyObject, uint32_t *access);

/*! @brief Destructor for the key object.
 *  The function frees key object context.
 *
 * @param keyObject Pointer to key object context.
 */
void sss_key_object_free(sss_object_t *keyObject);

/*!
 *@}
 */ /* end of sss_key_object */

/*!
 * @addtogroup sss_key_store
 * @{
 */

/*! @brief Constructor for the key store context data structure.
 *
 * @param[out] keyStore Pointer to key store context. Key store context is updated on function return.
 * @param session Session context.
 */
sss_status_t sss_key_store_context_init(sss_key_store_t *keyStore, sss_session_t *session);

/*! @brief Get handle to key store.
 *  If the key store already exists, nothing is allocated.
 *  If the key store does not exists, new empty key store is created and initialized.
 *  Key store context structure is updated with actual information.
 *
 * @param[out] keyStore Pointer to key store context. Key store context is updated on function return.
 * @param keyStoreId Implementation specific ID, can be used in case security subsystem manages multiple different
 * key stores.
 */
sss_status_t sss_key_store_allocate(sss_key_store_t *keyStore, uint32_t keyStoreId);

/*! @brief Save all cached persistent objects to persistent memory.
 */
sss_status_t sss_key_store_save(sss_key_store_t *keyStore);

/*! @brief Load from persistent memory to cached objects.
 */
sss_status_t sss_key_store_load(sss_key_store_t *keyStore);

/*! @brief This function moves key[] from memory to the destination key store.
 *  The function moves key[] from memory to the destination key store.
 *
 * @param keyStore Key store context
 * @param keyObject Reference to a key and it's properties
 * @param data Input data buffer with the plain key
 * @param dataLen Length of the Input data buffer in bytes
 * @param keyBitLen Crypto algorithm key bit length
 * @param options Pointer to implementation specific options
 * @param optionsLen Length of the options in bytes
 */
sss_status_t sss_key_store_set_key(sss_key_store_t *keyStore,
                                   sss_object_t *keyObject,
                                   const uint8_t *key,
                                   uint32_t keyBitLen,
                                   void *options,
                                   size_t optionsLen);

/*! @brief This function generates key[] in the destination key store. */
sss_status_t sss_key_store_generate_key(sss_key_store_t *keyStore,
                                        sss_object_t *keyObject,
                                        uint32_t keyBitLen,
                                        void *options);

/*! @brief This function exports plain key[] from key store (if constraints and user id allows reading) */
sss_status_t sss_key_store_get_key(sss_key_store_t *keyStore,
                                   sss_object_t *keyObject,
                                   uint8_t *key,
                                   size_t *pKeyBitLen);

/*! @brief This function outputs referenced plain key[] to destination security subsystem - e.g.to secret key bus or
 * CryptoLib context */
sss_status_t sss_key_store_open_key(sss_key_store_t *keyStore, sss_object_t *keyObject);

/*! @brief The referenced plain key[] cannot be updated any more. */
sss_status_t sss_key_store_freeze_key(sss_key_store_t *keyStore, sss_object_t *keyObject);

/*! @brief The referenced plain key[] is discarded. */
sss_status_t sss_key_store_erase_key(sss_key_store_t *keyStore, sss_object_t *keyObject);

// sss_status_t sss_key_store_clear_all(sss_key_store_t *keyStore);

/*! @brief Destructor for the key store context. */
void sss_key_store_context_free(sss_key_store_t *keyStore);

/*!
 *@}
 */ /* end of sss_key_store */

/*!
 * @addtogroup sss_session
 * @{
 */

/*!
 * @brief Open session between application and a security subsystem.
 *
 * Open virtual session between application (user context) and a security subsystem and function thereof.
 * Pointer to session shall be supplied to all SSS APIs as argument.
 * Low level SSS functions can provide implementation specific behaviour based on the session argument.
 *
 * @param   session Session context.
 * @param   subsystem Indicates which security subsystem is selected to be used.
 * @param   additionalApplicationId Additional ID that is supplied to the subsystem, implementation specific.
 * @param   connectionMethod Selects which identity credentials about application are used to determine access
 * control permissions to SSS functionality or SSS data.
 * @param   connectionData Data required by connectionMethod
 * @return  status
 */
sss_status_t sss_open_session(sss_session_t *session,
                              sss_type_t subsystem,
                              uint32_t additionalApplicationId,
                              uint32_t connectionMethod,
                              const void *connectionData);

/*!
 * @brief Close session between application and security subsystem.
 *
 * This function closes a session which has been opened with a security subsystem.
 * All commands within the session must have completed before this function can be called.
 * The implementation must do nothing if the input @ref session parameter is NULL.
 *
 * @param   session Session context.
 */
void sss_close_session(sss_session_t *session);

/*!
 *@}
 */ /* end of sss_session */

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_SSS_H_ */
