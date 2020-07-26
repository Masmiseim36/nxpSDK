/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_SSS_CONFIG_SNT_H_
#define _FSL_SSS_CONFIG_SNT_H_

#include "sab.h"

/* clang-format off */
#define SSS_SESSION_MAX_CONTEXT_SIZE        (16)
#define SSS_KEY_STORE_MAX_CONTEXT_SIZE      (16 + 80)
#define SSS_KEY_OBJECT_MAX_CONTEXT_SIZE     (16)
#define SSS_SYMMETRIC_MAX_CONTEXT_SIZE      (16 + 80)
#define SSS_AEAD_MAX_CONTEXT_SIZE           (16)
#define SSS_DIGEST_MAX_CONTEXT_SIZE         (16 + 80)
#define SSS_MAC_MAX_CONTEXT_SIZE            (16 + 16)
#define SSS_ASYMMETRIC_MAX_CONTEXT_SIZE     (16)
#define SSS_TUNNEL_MAX_CONTEXT_SIZE         (16)
#define SSS_DERIVE_KEY_MAX_CONTEXT_SIZE     (16)
#define SSS_RNG_MAX_CONTEXT_SIZE            (16)
#define SSS_PH_CLSHA_CTX_MAX_CONTEXT_SIZE   (216)
/* clang-format on */

#define SSS_TYPE_ENUM_ALT
typedef enum _sss_type
{
    kType_SSS_Software      = SAB_SE_TYPE_Software,
    kType_SSS_SECO          = SAB_SE_TYPE_SECO,
    kType_SSS_Sentinel200   = SAB_SE_TYPE_S200,
    kType_SSS_Sentinel300   = SAB_SE_TYPE_S300,
    kType_SSS_Sentinel400   = SAB_SE_TYPE_S400,
    kType_SSS_Sentinel500   = SAB_SE_TYPE_S500,
    kType_SSS_SecureElement = SAB_SE_TYPE_SECURE_ELEMENT,
} sss_type_t;

#define SSS_MODE_ENUM_ALT
typedef enum _sss_mode
{
    kMode_SSS_Encrypt             = SAB_MODE_ENCRYPT,
    kMode_SSS_Decrypt             = SAB_MODE_DECRYPT,
    kMode_SSS_Sign                = SAB_MODE_SIGN,
    kMode_SSS_Verify              = SAB_MODE_VERIFY,
    kMode_SSS_ComputeSharedSecret = SAB_MODE_COMPUTE_SHARED_SECRET,
    kMode_SSS_Digest              = SAB_MODE_DIGEST,
    kMode_SSS_Mac                 = SAB_MODE_MAC,
} sss_mode_t;

#define SSS_ALGORITHM_ENUM_ALT
typedef enum _sss_algorithm
{
    /* AES */
    kAlgorithm_SSS_AES_ECB = SAB_ALGO_AES_ECB,
    kAlgorithm_SSS_AES_CBC = SAB_ALGO_AES_CBC,
    kAlgorithm_SSS_AES_CTR = SAB_ALGO_AES_CTR,
    kAlgorithm_SSS_AES_GCM = SAB_ALGO_AES_GCM,
    kAlgorithm_SSS_AES_CCM = SAB_ALGO_AES_CCM,
    /* CHACHA_POLY */
    kAlgorithm_SSS_CHACHA_POLY = SAB_ALGO_CHACHA_POLY,
    /* DES3 */
    kAlgorithm_SSS_DES3_ECB = SAB_ALGO_DES3_ECB,
    kAlgorithm_SSS_DES3_CBC = SAB_ALGO_DES3_CBC,
    /* digest */
    kAlgorithm_SSS_SHA1   = SAB_ALGO_SHA1,
    kAlgorithm_SSS_SHA224 = SAB_ALGO_SHA224,
    kAlgorithm_SSS_SHA256 = SAB_ALGO_SHA256,
    kAlgorithm_SSS_SHA384 = SAB_ALGO_SHA384,
    kAlgorithm_SSS_SHA512 = SAB_ALGO_SHA512,
    /* MAC */
    kAlgorithm_SSS_CMAC_AES    = SAB_ALGO_CMAC_AES,
    kAlgorithm_SSS_HMAC_SHA256 = SAB_ALGO_HMAC_SHA256,
    /* Diffie-Helmann */
    kAlgorithm_SSS_DH   = SAB_ALGO_DH,
    kAlgorithm_SSS_ECDH = SAB_ALGO_ECDH,
    /* DSA */
    kAlgorithm_SSS_DSA_SHA1   = SAB_ALGO_DSA_SHA1,
    kAlgorithm_SSS_DSA_SHA224 = SAB_ALGO_DSA_SHA224,
    kAlgorithm_SSS_DSA_SHA256 = SAB_ALGO_DSA_SHA256,
    /* RSA */
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1       = SAB_ALGO_RSASSA_PKCS1_V1_5_SHA1,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224     = SAB_ALGO_RSASSA_PKCS1_V1_5_SHA224,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256     = SAB_ALGO_RSASSA_PKCS1_V1_5_SHA256,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384     = SAB_ALGO_RSASSA_PKCS1_V1_5_SHA384,
    kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512     = SAB_ALGO_RSASSA_PKCS1_V1_5_SHA512,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1   = SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA1,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224 = SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA224,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256 = SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA256,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384 = SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA384,
    kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512 = SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA512,
    /* ECDSA */
    kAlgorithm_SSS_ECDSA_SHA1   = SAB_ALGO_ECDSA_SHA1,
    kAlgorithm_SSS_ECDSA_SHA224 = SAB_ALGO_ECDSA_SHA224,
    kAlgorithm_SSS_ECDSA_SHA256 = SAB_ALGO_ECDSA_SHA256,
    kAlgorithm_SSS_ECDSA_SHA384 = SAB_ALGO_ECDSA_SHA384,
    kAlgorithm_SSS_ECDSA_SHA512 = SAB_ALGO_ECDSA_SHA512,
} sss_algorithm_t;

#define SSS_KEY_CIPHER_TYPE_ENUM_ALT
typedef enum _sss_cipher_type
{
    kSSS_CipherType_NONE = SAB_KEY_TYPE_SYMMETRIC,
    kSSS_CipherType_AES  = SAB_KEY_TYPE_SYMMETRIC,
    kSSS_CipherType_DES  = SAB_KEY_TYPE_SYMMETRIC,

    kSSS_CipherType_CMAC = SAB_KEY_TYPE_SYMMETRIC,
    kSSS_CipherType_HMAC = SAB_KEY_TYPE_SYMMETRIC,

    kSSS_CipherType_MAC     = SAB_KEY_TYPE_SYMMETRIC,
    kSSS_CipherType_RSA     = SAB_KEY_TYPE_ASYMMETRIC, /*! RSA RAW format      */
    kSSS_CipherType_RSA_CRT = SAB_KEY_TYPE_ASYMMETRIC, /*! RSA CRT format      */

    /* The following keys can be identified
     * solely by the *Family* and bit length
     */
    kSSS_CipherType_EC_NIST_P = SAB_KEY_TYPE_ASYMMETRIC, /*! Keys Part of NIST-P Family */
    kSSS_CipherType_EC_NIST_K = SAB_KEY_TYPE_ASYMMETRIC, /*! Keys Part of NIST-K Family */

    /* The following keys need their full curve parameters (p,a,b,x,y,n,h)
     */
    /*! Montgomery Key,   */
    kSSS_CipherType_EC_MONTGOMERY = SAB_KEY_TYPE_ASYMMETRIC,
    /*! twisted Edwards form elliptic curve public key */
    kSSS_CipherType_EC_TWISTED_ED = SAB_KEY_TYPE_ASYMMETRIC,
    /*! Brainpool form elliptic curve public key */
    kSSS_CipherType_EC_BRAINPOOL = SAB_KEY_TYPE_ASYMMETRIC,
    /*! Barreto Naehrig curve */
    kSSS_CipherType_EC_BARRETO_NAEHRIG = SAB_KEY_TYPE_ASYMMETRIC,

    kSSS_CipherType_UserID      = SAB_KEY_TYPE_ASYMMETRIC,
    kSSS_CipherType_Certificate = SAB_KEY_TYPE_ASYMMETRIC,
    kSSS_CipherType_Binary      = SAB_KEY_TYPE_ASYMMETRIC,
    kSSS_CipherType_Count       = SAB_KEY_TYPE_ASYMMETRIC,
    kSSS_CipherType_PCR         = SAB_KEY_TYPE_ASYMMETRIC,
    kSSS_CipherType_ReservedPin = SAB_KEY_TYPE_ASYMMETRIC,
} sss_cipher_type_t;

#endif /* _FSL_SSS_CONFIG_H_ */
