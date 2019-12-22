/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SSS_APIS_INC_FSL_SSS_SE05X_TYPES_H_
#define SSS_APIS_INC_FSL_SSS_SE05X_TYPES_H_

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <fsl_sss_api.h>
#include <fsl_sss_policy.h>

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_SE05X
#include "nxScp03_Types.h"
#include "se05x_const.h"
#include "se05x_tlv.h"
#include "sm_api.h"
#if (__GNUC__ && !AX_EMBEDDED)
#include <pthread.h>
/* Only for base session with os */
#endif

/*!
 * @addtogroup sss_sw_se05x
 * @{
 */

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

#define SSS_SUBSYSTEM_TYPE_IS_SE05X(subsystem) (subsystem == kType_SSS_SE_SE05x)

#define SSS_SESSION_TYPE_IS_SE05X(session) \
    (session && SSS_SUBSYSTEM_TYPE_IS_SE05X(session->subsystem))

#define SSS_KEY_STORE_TYPE_IS_SE05X(keyStore) \
    (keyStore && SSS_SESSION_TYPE_IS_SE05X(keyStore->session))

#define SSS_OBJECT_TYPE_IS_SE05X(pObject) \
    (pObject && SSS_KEY_STORE_TYPE_IS_SE05X(pObject->keyStore))

#define SSS_ASYMMETRIC_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_DERIVE_KEY_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_SYMMETRIC_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_MAC_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_RNG_CONTEXT_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_DIGEST_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_AEAD_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_TUNNEL_CONTEXT_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

#define SSS_TUNNEL_TYPE_IS_SE05X(context) \
    (context && SSS_SESSION_TYPE_IS_SE05X(context->session))

/* ************************************************************************** */
/* Structrues and Typedefs                                                    */
/* ************************************************************************** */

struct _sss_se05x_session;

typedef struct _sss_se05x_tunnel_context
{
    struct _sss_se05x_session *se05x_session;
    sss_tunnel_dest_t tunnelDest;
#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_t channelLock;
#endif
} sss_se05x_tunnel_context_t;

typedef struct _sss_se05x_session
{
    /*! Indicates which security subsystem is selected to be used. */
    sss_type_t subsystem;
    Se05xSession_t s_ctx;
    sss_se05x_tunnel_context_t *ptun_ctx;
} sss_se05x_session_t;

struct _sss_se05x_object;

typedef struct _sss_se05x_key_store
{
    sss_se05x_session_t *session;
    struct _sss_se05x_object *kekKey;

} sss_se05x_key_store_t;

typedef struct _sss_se05x_object
{
    /*! key store holding the data and other properties */
    sss_se05x_key_store_t *keyStore;
    /*! Object types */
    uint32_t objectType;
    uint32_t cipherType;
    /*! Application specific key identifier. The keyId is kept in the key  store
     * along with the key data and other properties. */
    uint32_t keyId;

#if APPLET_SE050_VER_MAJOR_MINOR >= 20000u
    SE05x_ECCurve_t curve_id;
#else
    uint32_t curve_id;
#endif

    uint8_t isPersistant : 1;

} sss_se05x_object_t;

typedef struct _sss_se05x_derive_key
{
    sss_se05x_session_t *session;
    sss_se05x_object_t *keyObject;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

} sss_se05x_derive_key_t;

typedef struct _sss_se05x_asymmetric
{
    sss_se05x_session_t *session;
    sss_se05x_object_t *keyObject;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

} sss_se05x_asymmetric_t;

typedef struct _sss_se05x_symmetric
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_se05x_session_t *session;
    sss_se05x_object_t *keyObject; /*!< Reference to key and it's properties. */
    sss_algorithm_t algorithm;     /*!  */
    sss_mode_t mode;               /*!  */

    SE05x_CryptoObjectID_t cryptoObjectId;
    uint8_t cache_data[16];
    size_t cache_data_len;
} sss_se05x_symmetric_t;

typedef struct
{
    sss_se05x_session_t *session;
    sss_se05x_object_t *keyObject; /*!< Reference to key and it's properties. */
    sss_algorithm_t algorithm;     /*!  */
    sss_mode_t mode;               /*!  */
    SE05x_CryptoObjectID_t cryptoObjectId;
} sss_se05x_mac_t;

typedef struct _sss_se05x_aead
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_se05x_session_t *session;
    sss_se05x_object_t *keyObject; /*!< Reference to key and it's properties. */
    sss_algorithm_t algorithm;     /*!<  */
    sss_mode_t mode;               /*!<  */

    /*! Implementation specific part */
} sss_se05x_aead_t;

typedef struct _sss_se05x_digest
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_se05x_session_t *session;
    sss_algorithm_t algorithm; /*!<  */
    sss_mode_t mode;           /*!<  */
    /*! Full digest length per algorithm definition. This field is initialized along with algorithm. */
    size_t digestFullLen;
    /*! Implementation specific part */

    SE05x_CryptoObjectID_t cryptoObjectId;
} sss_se05x_digest_t;

typedef struct
{
    sss_se05x_session_t *session;
} sss_se05x_rng_context_t;

typedef enum
{
    kSSS_SE05x_SessionProp_CertUID = kSSS_SessionProp_au8_Proprietary_Start + 1,
} sss_s05x_sesion_prop_au8_t;

/* deprecated : Used only for backwards compatibility */
#define SE05x_Connect_Ctx_t SE_Connect_Ctx_t
/* deprecated : Used only for backwards compatibility */
#define se05x_auth_context_t SE_Connect_Ctx_t

typedef struct
{
    /** Use of curve TPM_ECC_BN_P256 */
    uint8_t AppletConfig_ECDAA : 1;
    /** EC DSA and DH support */
    uint8_t AppletConfig_ECDSA_ECDH_ECDHE : 1;
    /** Use of curve RESERVED_ID_ECC_ED_25519 */
    uint8_t AppletConfig_EDDSA : 1;
    /** Use of curve RESERVED_ID_ECC_MONT_DH_25519 */
    uint8_t AppletConfig_DH_MONT : 1;
    /** Writing HMACKey objects */
    uint8_t AppletConfig_HMAC : 1;
    /** Writing RSAKey objects */
    uint8_t AppletConfig_RSA_PLAIN : 1;
    /** Writing RSAKey objects */
    uint8_t AppletConfig_RSA_CRT : 1;
    /** Writing AESKey objects */
    uint8_t AppletConfig_AES : 1;
    /** Writing DESKey objects */
    uint8_t AppletConfig_DES : 1;
    /** PBKDF2 */
    uint8_t AppletConfig_PBKDF : 1;
    /** TLS Handshake support commands (see 4.16) in APDU Spec*/
    uint8_t AppletConfig_TLS : 1;
    /** Mifare DESFire support (see 4.15)  in APDU Spec*/
    uint8_t AppletConfig_MIFARE : 1;
    /** Allocated value undefined and reserved for future use */
    uint8_t AppletConfig_RFU1 : 1;
    /** I2C Master support (see 4.17)  in APDU Spec*/
    uint8_t AppletConfig_I2CM : 1;
    /** RFU */
    uint8_t AppletConfig_RFU21 : 1;
} SE05x_Applet_Feature_t;

typedef struct _sss_se05x_attst_comp_data
{
    uint8_t outrandom[16];
    size_t outrandomLen;
    SE05x_TimeStamp_t timeStamp;
    size_t timeStampLen;
    uint8_t chipId[SE050_MODULE_UNIQUE_ID_LEN];
    size_t chipIdLen;
    uint8_t attribute[MAX_POLICY_BUFFER_SIZE + 15];
    size_t attributeLen;
    uint8_t signature[256];
    size_t signatureLen;
} sss_se05x_attst_comp_data_t;

typedef struct _sss_se05x_attst_data
{
    sss_se05x_attst_comp_data_t data[SE05X_MAX_ATTST_DATA];
    uint8_t valid_number;
} sss_se05x_attst_data_t;

/*! @} */

/*! @addtogroup se050_i2cm
 *
 * @{ */

/*! @brief Types of entries in an I2CM Transaction */
typedef enum _SE05x_I2CM_TLV_type
{
    kSE05x_I2CM_None = 0,
    kSE05x_I2CM_Configure,
    //kSE05x_I2CM_Security,
    kSE05x_I2CM_Write = 3,
    kSE05x_I2CM_Read,
    kSE05x_I2CM_StructuralIssue = 0xFF
} SE05x_I2CM_TLV_type_t;

/*! @brief Status of I2CM Transaction */
typedef enum _SE05x_I2CM_status
{
    kSE05x_I2CM_Success = 0x5A,
    kSE05x_I2CM_I2C_Nack_Fail = 0x01,
    kSE05x_I2CM_I2C_Write_Error = 0x02,
    kSE05x_I2CM_I2C_Read_Error = 0x03,
    kSE05x_I2CM_I2C_Time_Out_Error = 0x05,
    kSE05x_I2CM_Invalid_Tag = 0x11,
    kSE05x_I2CM_Invalid_Length = 0x12,
    kSE05x_I2CM_Invalid_Length_Encode = 0x13,
    kSE05x_I2CM_I2C_Config = 0x21
} SE05x_I2CM_status_t;

typedef enum _SE05x_I2CM_securityReq
{
    kSE05x_Security_None = 0,
    kSE05x_Sign_Request,
    kSE05x_Sign_Enc_Request,
} SE05x_I2CM_securityReq_t;

/*! @brief Configuration for I2CM */

typedef enum
{
    kSE05x_I2CM_Baud_Rate_100Khz = 0,
    kSE05x_I2CM_Baud_Rate_400Khz,
} SE05x_I2CM_Baud_Rate_t;

/*! @brief Data Configuration for I2CM */
typedef struct _SE05x_I2CM_configData
{
    uint8_t I2C_addr;
    SE05x_I2CM_Baud_Rate_t I2C_baudRate;
    SE05x_I2CM_status_t status;
} SE05x_I2CM_configData_t;

/*! @brief Security Configuration for I2CM */
typedef struct _SE05x_I2CM_securityData
{
    SE05x_I2CM_securityReq_t operation;
    uint32_t keyObject;
} SE05x_I2CM_securityData_t;

/*! @brief Write From I2CM to I2C Slave */
typedef struct _SE05x_I2CM_writeData
{
    uint8_t writeLength;
    SE05x_I2CM_status_t wrStatus;
    uint8_t *writebuf; /* Input */
} SE05x_I2CM_writeData_t;

/*! @brief Read to I2CM from I2C Slave */
typedef struct _SE05x_I2CM_readData
{
    uint16_t readLength;
    SE05x_I2CM_status_t rdStatus;
    /* Output. rdBuf will point to Host buffer */
    uint8_t *rdBuf;
} SE05x_I2CM_readData_t;

// Used to report error response, not for outgoing command
typedef struct _SE05x_I2CM_structuralIssue
{
    SE05x_I2CM_status_t issueStatus;
} SE05x_I2CM_structuralIssue_t;

/*! @brief Individual entry in array of TLV commands */
typedef union _SE05x_I2CM_INS_type {
    SE05x_I2CM_configData_t cfg;
    SE05x_I2CM_securityData_t sec;
    SE05x_I2CM_writeData_t w;
    SE05x_I2CM_readData_t rd;
    SE05x_I2CM_structuralIssue_t issue;
} SE05x_I2CM_INS_type_t;

/*! Individual entry in array of TLV commands, with type
 *
 * @ref Se05x_i2c_master_txn would expect an array of these.
 */
typedef struct _SE05x_I2CM_cmd
{
    SE05x_I2CM_TLV_type_t type;
    SE05x_I2CM_INS_type_t cmd;
} SE05x_I2CM_cmd_t;

/*!
 *@}
 */ /* end of se050_i2cm */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Functions                                                                  */
/* ************************************************************************** */

/** MAC Validate
 *
 */
sss_status_t sss_se05x_mac_validate_one_go(sss_se05x_mac_t *context,
    const uint8_t *message,
    size_t messageLen,
    uint8_t *mac,
    size_t macLen);

/** Similar to @ref sss_se05x_asymmetric_sign_digest,
 *
 * but hashing/digest done by SE
 */
sss_status_t sss_se05x_asymmetric_sign(sss_se05x_asymmetric_t *context,
    uint8_t *digest,
    size_t digestLen,
    uint8_t *signature,
    size_t *signatureLen);

/** Similar to @ref sss_se05x_asymmetric_verify_digest,
 * but hashing/digest done by SE
 *
 */
sss_status_t sss_se05x_asymmetric_verify(sss_se05x_asymmetric_t *context,
    uint8_t *digest,
    size_t digestLen,
    uint8_t *signature,
    size_t signatureLen);

/** Read with attestation
 *
 */
sss_status_t sss_se05x_key_store_get_key_attst(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    uint8_t *key,
    size_t *keylen,
    size_t *pKeyBitLen,
    sss_se05x_object_t *keyObject_attst,
    sss_algorithm_t algorithm_attst,
    uint8_t *random_attst,
    size_t randomLen_attst,
    sss_se05x_attst_data_t *attst_data);

uint32_t se05x_sssKeyTypeLenToCurveId(
    sss_cipher_type_t keyType, size_t keyBits);

smStatus_t Se05x_API_DeleteAll_Iterative(pSe05xSession_t sessionId);

sss_status_t sss_se05x_refresh_session(
    sss_se05x_session_t *session, void *connectionData);

/*! @addtogroup se050_i2cm
 *
 * @{
*/

/*! @brief Se05x_i2c_master_txn
*
* I2CM Transaction
*
* @param[in] sess session identifier
* @param[in/out] p Array of structure type capturing a sequence of i2c master cmd/rsp transactions.
* @param[in] noOftags Amount of structures contained in ::p
*
* @pre p describes I2C master commands.
* @post p contains execution state of I2C master commands, the I2C master commands can be overwritten to report on execution failure.
*/
smStatus_t Se05x_i2c_master_txn(
    sss_session_t *sess, SE05x_I2CM_cmd_t *p, uint8_t noOftags);

/*! @brief Se05x_i2c_master_attst_txn
 *
 * I2CM Read With Attestation
 *
 * @param[in] sess session identifier
 * @param[in] keyObject Keyobject which contains  4 byte attestaion KeyId
 * @param[in/out] p Array of structure type capturing a sequence of i2c master cmd/rsp transactions.
 * @param[in] random_attst 16-byte freshness random
 * @param[in] random_attstLen length of freshness random
 * @param[in] attst_algo 1 byte attestationAlgo
 * @param[out] ptimeStamp  timestamp
 * @param[out] timeStampLen  Length for timestamp
 * @param[out] freshness  freshness (random)
 * @param[out] pfreshnessLen Length for freshness
 * @param[out] chipId  unique chip Id
 * @param[out] pchipIdLen Length for chipId
 * @param[out] signature  signature
 * @param[out] psignatureLen Length for signature
 * @param[in] noOftags Amount of structures contained in ::p
 *
 * @pre p describes I2C master commands.
 * @post p contains execution state of I2C master commands, the I2C master commands can be overwritten to report on execution failure.
 */
smStatus_t Se05x_i2c_master_attst_txn(sss_session_t *sess,
    sss_object_t *keyObject,
    SE05x_I2CM_cmd_t *p,
    uint8_t *random_attst,
    size_t random_attstLen,
    SE05x_AttestationAlgo_t attst_algo,
    SE05x_TimeStamp_t *ptimeStamp,
    size_t *timeStampLen,
    uint8_t *freshness,
    size_t *pfreshnessLen,
    uint8_t *chipId,
    size_t *pchipIdLen,
    uint8_t *signature,
    size_t *psignatureLen,
    uint8_t noOftags);

/*!
 *@}
 */ /* end of se050_i2cm */

void get_ecc_raw_data(
    uint8_t *key, uint8_t **key_buf, size_t *key_buflen, uint32_t curve_id);

#endif /* SSS_HAVE_SE05X */

#endif /* SSS_APIS_INC_FSL_SSS_SE05X_TYPES_H_ */
