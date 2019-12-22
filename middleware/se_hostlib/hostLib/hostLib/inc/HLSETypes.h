/**
 *
 * @file HLSETypes.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Host Lib SE API Types
 *
 * @par HISTORY
 *
 */
#ifndef _HLSE_TYPES_H_
#define _HLSE_TYPES_H_

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include "sm_types.h"
#if SSS_HAVE_SCP_SCP03_SSS
#include <fsl_sss_api.h>
#endif

/** @addtogroup ghlse_types HLSE Types
 *
 * @{
 *
 */

/**
 * Return Codes.
 */

typedef U16  HLSE_RET_CODE;


// Error/status word
#define HLSE_SW_OK                              (0x9000) //!< Operation successfull

#define HLSE_ERR_CONNECT_LINK_FAILED            (0x7001) //!< Connect Link Failed
#define HLSE_ERR_CONNECT_SELECT_FAILED          (0x7002) //!< Connect Select Failed
#define HLSE_ERR_COMM_ERROR                     (0x7003) //!< Generic communication error
#define HLSE_ERR_NO_VALID_IP_PORT_PATTERN       (0x8000) //!< Invalid IP Port pattern

#define HLSE_SMCOM_OK                           0x9000  //!< Communication successful
#define HLSE_SMCOM_SND_FAILED                   0x7010  //!< Communication failed while sending data
#define HLSE_SMCOM_RCV_FAILED                   0x7011  //!< Communication failed while receiving data
#define HLSE_SMCOM_COM_FAILED                   0x7012  //!< Cannot open communication link with ax device
#define HLSE_SMCOM_PROTOCOL_FAILED              0x7013  //!< APDU exchange protocol failed to be established successfully
#define HLSE_SMCOM_NO_ATR                       0x7014  //!< No ATR can be retrieved
#define HLSE_SMCOM_NO_PRIOR_INIT                0x7015  //!< The callbacks doing the actual transfer have not been installed
#define HLSE_SMCOM_COM_ALREADY_OPEN             0x7016  //!< Communication link is already open with device

#define HLSE_ERR_NOT_SUPPORTED                  (0x7080)//!< The function and/or parameters are not supported by the library

#define HLSE_ERR_MEMORY                         (0x7020) //!< Memory allocation error
#define HLSE_ERR_GENERAL_ERROR                  (0x7021) //!< Non-specific error code
#define HLSE_ERR_WRONG_RESPONSE                 (0x7022) //!< Semantic error discovered while parsing APDU response
#define HLSE_ERR_API_ERROR                      (0x7023) //!< Illegal parameter value passed to API
#define HLSE_ERR_TLV_MISSING                    (0x7024) //!< Specific TAG is missing from APDU response
#define HLSE_ERR_HASH_COMPARE_FAILS             (0x7025) //!< Hash comparison failed
#define HLSE_ERR_BUF_TOO_SMALL                  (0x7026) //!< Buffer provided is too small
#define HLSE_ERR_CRYPTO_ENGINE_FAILED           (0x7027) //!< The crypto engine (implemented underneath a crypto abstraction layer) failed to provide a crypto service.
#define HLSE_ERR_PATTERN_COMPARE_FAILED         (0x7028) //!< Pattern compare failed
#define HLSE_ERR_NOT_IMPLEMENTED                (0x7029) //!< Not implemented error
#define HLSE_ERR_FILE_SYSTEM                    (0x7030) //!< File System error
#define HLSE_ERR_NO_PRIVATE_KEY                 (0x7031) //!< No Private Key error
#define HLSE_ERR_IDENT_IDX_RANGE                (0x7032) //!< Identifier or Index of Reference Key is out of bounds
#define HLSE_ERR_NO_PUBLIC_KEY                  (0x7033) //!< No Public key error

#define HLSE_SCP_OK                              (SW_OK)  //!< SCP OK
#define HLSE_SCP_UNDEFINED_CHANNEL_ID            (0x7041) //!< SCP Undefined channel ID
#define HLSE_SCP_FAIL                            (0x7042) //!< SCP Failure
#define HLSE_SCP_CARD_CRYPTOGRAM_FAILS_TO_VERIFY (0x7043) //!< SCP Card cryptogram failed to verify
#define HLSE_SCP_PARAMETER_ERROR                 (0x7044) //!< SCP Parameter error

#define HLSE_SCP_RSP_MAC_FAIL                   (0x7050) //!< MAC on APDU response is not correct
#define HLSE_SCP_DECODE_FAIL                    (0x7051) //!< Encrypted Response did not decode to correctly padded plaintext

#define HLSE_OBJ_ALREADY_EXISTS                 (0x7060) //!< Object Already Exists
#define HLSE_OBJ_GP_TABLE_LOCKED                (0x7061) //!< Gp Table is locked - could not be updated

/**
 *  A type for all the defines.
 */
typedef U32 HLSE_TYPE;

/**
 *  An Object Type definition.
 */
typedef HLSE_TYPE HLSE_OBJECT_TYPE;

/** @name
 *
 * Enumerates the various logical objects existing on the Secure Element.
 *
 * @{ */

#define HLSE_ANY_TYPE       0x00000000       /**< For the Enumeration of all the objects */
#define HLSE_KEY_PAIR       0x00010000       /**< Asymmetric Key Pairs (e.g. ECC NIST P-256) */
#define HLSE_PUBLIC_KEY     0x00020000       /**< Asymmetric Public Key in Uncompressed format (e.g. ECC NIST P-256 Public Key) */
#define HLSE_SYMMETRIC_KEY  0x00030000       /**< Symmetric Key (e.g. AES128) */
#ifdef TGT_A71CL
#define HLSE_KEK_KEY        0x00040000       /**< Key Encryption Key for loading wrapped data */
#endif
#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH || SSS_HAVE_SE05X)
#define HLSE_CONFIG_KEY     0x00040000       /**< Configuration keys used for key wrapping and module unlocking */
#endif
#define HLSE_COUNTER        0x00050000       /**< Counter with rollback protection; size  4 bytes */
//#define HLSE_GP_DATA      0x00060000       /**< General Purpose data */
#define HLSE_MODULE         0x00070000       /**< The applet Module itself: True Random Generator, Hash, Chip Info */
#define HLSE_SM_KEYS        0x00080000       /**< Secure Channel Key */
#define HLSE_CERTIFICATE    0x00090000       /**< Certificate (e.g. X.509) */
#define HLSE_DATA           0x000A0000       /**< Logical Data object */
/**
*  Enumerates the Applet operational modes.
*/
#define HLSE_AUTHENTICATION_MODE       0x00       /**< Authentication (KEK) Mode */
#define HLSE_DEFAULT_OR_PLAIN_MODE     0x01       /**< Default or Plain Injection Mode */


/**
 *  An Object Handle.
 */
typedef HLSE_TYPE HLSE_OBJECT_HANDLE;

/** An Object index.
 * Note: the value may represent a Constructed Secret index.
 */
typedef HLSE_TYPE HLSE_OBJECT_INDEX;

/** @name An Attribute Type.
 * @{ */
typedef HLSE_TYPE HLSE_ATTRIBUTE_TYPE;

#define HLSE_ATTR_OBJECT_TYPE                   0    //!< The object type (Mandatory in Create)
#define HLSE_ATTR_OBJECT_INDEX                  1    //!< The object index (Mandatory in Create)
#define HLSE_ATTR_OBJECT_VALUE                  2    //!< The object value
#define HLSE_ATTR_WRAPPED_OBJECT_VALUE          3    //!< The object value wrapped
#define HLSE_ATTR_DIRECT_ACCESS_OBJECT_VALUE    4    //!< The object value with direct access to sub-sections
#define HLSE_ATTR_READ_ONLY                     6    //!< The object data can only be read and not modified (passed only in Create, cannot be changed)
#define HLSE_ATTR_OBJECT_OFFSET                 7    //!< The object offset in GP

#ifdef TGT_A71CL
#define HLSE_ATTR_KEY_CL_ID                     7    //!< The unique CL string
#define HLSE_ATTR_KEY_TYPE                      8    //!< The key type - values are from \ref clKeyType
#define HLSE_ATTR_KEY_LENGTH                    9    //!< The key length, in bits
#define HLSE_ATTR_KEK_VALUE                    10    //!< The KEK value
#endif

#define HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE      10   //!< Locked/Unlocked/Allow Lock
#define HLSE_ATTR_MODULE_SCP_LOCK_STATE            11   //!< Get only Mandatory/NotRequired/KeysSet
#define HLSE_ATTR_MODULE_INJECTION_LOCK_STATE      12   //!< Plain/Wrapped
#define HLSE_ATTR_MODULE_UNIQUE_ID                 13   //!< Get only usage
#define HLSE_ATTR_MODULE_FREE_PERSISTENT_MEM       14   //!< Get only usage - debug mode only
#define HLSE_ATTR_MODULE_FREE_TRANSIENT_MEM        15   //!< Get only usage - debug mode only
#define HLSE_ATTR_MODULE_APPLET_INFO               16   //!< Get only usage
#define HLSE_ATTR_MODULE_APPLET_NAME               17   //!< Get only usage
#define HLSE_ATTR_MODULE_CREDENTIAL_INFO           18   //!< Get only usage
#define HLSE_ATTR_MODULE_TOTAL_GP_SIZE             19   //!< Get only usage
#define HLSE_ATTR_MODULE_RANDOM                    20   //!< Get only usage
#define HLSE_ATTR_MODULE_UNLOCK_CHALLENGE          21   //!< Get only usage (for Module Unlock)
#define HLSE_ATTR_MODULE_RESTRICTED_KEYPAIR_INDEX  22   //!< Get only usage

#ifdef TGT_A71CL
#define HLSE_ATTR_MODULE_CL_ID                     13   //!< Get only usage
#define HLSE_ATTR_MODULE_MODE                      18  //!< Get only usage - The current mode of the Applet (e.g. Authentication mode or Default mode)
#define HLSE_ATTR_MODULE_RANDOM                    20   //!< Get only usage
#define HLSE_ATTR_VENDOR_INFO                      21   //!< Get only usage
#define HLSE_ATTR_EXTERNAL_AUTH                    22   //!< Set only usage for the KEK
#endif

/** @} */

/** @name RSA key attributes
 *
 * @{ */

#define HLSE_ATTR_RSA_MODULUS           30  //!< RSA modulus
#define HLSE_ATTR_RSA_BIT_LENGTH        31  //!< RSA Bit Length
#define HLSE_ATTR_RSA_PUBLIC_EXPONENT   32  //!< RSA Public exponent
#define HLSE_ATTR_RSA_PRIVATE_EXPONENT  33  //!< RSA Private exponent
#define HLSE_ATTR_RSA_P                 34  //!< RSA P parameter
#define HLSE_ATTR_RSA_Q                 35  //!< RSA Q parameter
#define HLSE_ATTR_RSA_DP                36  //!< RSA DP parameter
#define HLSE_ATTR_RSA_DQ                37  //!< RSA DQ parameter
#define HLSE_ATTR_RSA_DPQ               38  //!< RSA DPQ parameter
/** @} */

/**
 * Stores all the information required for an object's attribute - its type, value and value length.
 */
typedef struct HLSE_ATTRIBUTE{
    HLSE_ATTRIBUTE_TYPE     type;         //!< The attribute's type

    void*                   value;        //!< The attribute's value
    U16                     valueLen;     //!< The length in bytes of \p value.
} HLSE_ATTRIBUTE;

/**
 * Stores all the information required to use the HLSE_ATTR_DIRECT_ACCESS_OBJECT_VALUE attribute, to be passed in the attribute's value member.
 */
typedef struct HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE {
    U16                     offset;       //!< Offest inside the object data
    U16                     bytes;        //!< Number of bytes to read/write

    void*                   buffer;       //!< Buffer of data to be written/read to
    U16                     bufferLen;    //!< The length in bytes of \p buffer.
} HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE;

//*******************************************************************
// Cryptographic Operations
//*******************************************************************

/** A Context Handle - may point to any stucture.
 */
typedef void* HLSE_CONTEXT_HANDLE;

/** Mechanism type.
 */
typedef HLSE_TYPE HLSE_MECHANISM_TYPE;


/** @name  Mechanism Type enum
 *
 * Enumerates the various Cryptographic Mechanisms that may be supported by the library.
 *
 * @{ */

// Hash mechanisms
#define     HLSE_SHA1             0         //!< Sha1
#define     HLSE_SHA256           1         //!< Sha256
// Symmetric key Encrypt/Decrypt mechanisms
#define     HLSE_AES_ECB_ENCRYPT  2         //!< AES ECB Encryption
#define     HLSE_AES_ECB_DECRYPT  3         //!< AES ECB Decryption
#define     HLSE_AES_CBC_ENCRYPT  4         //!< AES CBC Encryption
#define     HLSE_AES_CBC_DECRYPT  5         //!< AES CBC Decryption
#define     HLSE_DES_ECB_ENCRYPT  6         //!< DES ECB Encryption
#define     HLSE_DES_ECB_DECRYPT  7         //!< DES ECB Decryption
#define     HLSE_DES_CBC_ENCRYPT  8         //!< DES CBC Encryption
#define     HLSE_DES_CBC_DECRYPT  9         //!< DES CBC Decryption
#define		HLSE_DES_CBC_ISO9797_M1 10		//!< DES CBC Signature
#define		HLSE_DES_CBC_ISO9797_M2 11		//!< DES CBC Signature
#define		HLSE_AES_CBC_ISO9797_M1 12		//!< AES CBC Signature
#define		HLSE_AES_CBC_ISO9797_M2 13		//!< AES CBC Signature
// Sign mechanisms
#define     HLSE_DES_CMAC  20 /**< Ref. RFC 4493 */
#define     HLSE_AES_CMAC  21 /**< Ref. RFC 4493 */
// ECC Signature
#define     HLSE_ECDSA_SIGN  22                 //!< ECDSA_SIGN
#define     HLSE_ECDSA_NORMALIZE_ASN_SIGN  23   //!< ECDSA_NORMALIZE_ASN_SIGN
// ECC signature verification
#define     HLSE_ECDSA_VERIFY  24               //!< ECDSA_VERIFY
// TLS 1.2
#define     HLSE_TLS_1_2  30                    //!< TLS_1_2
// Symmetric key derivation function using SHA256
#define     HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND  40    //!< HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND
#define     HLSE_HKDF_HMAC_SHA256_SKIP_EXTRACT  41          //!< HKDF_HMAC_SHA256_SKIP_EXTRACT
/// Symmetric key HMAC using AES and SHA256
#define     HLSE_HMAC_AES_SHA256  42
/// Symmetric AES key wrapping
#define     HLSE_RFC3394_AES_KEY_WRAPPING  50
// TLS_PSK Master Key derivation
#define     HLSE_TLS_PSK_MASTER_KEY_DERIVE  60              //!< TLS_PSK_MASTER_KEY_DERIVE
#define     HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC  61       //!< TLS_PSK_MASTER_KEY_DERIVE_DH_ECC
/// ECC Diffie-Helman key exchange
#define     HLSE_ECDH  70
/// True random genarator
#define     HLSE_TRNG  80
// Global Platform Secure Channel
#define     HLSE_SCP03_ALG  90      //!<  Global Platform Secure Channel SCP 03 Alg
#define     HLSE_SCP02_ALG  91      //!<  Global Platform Secure Channel SCP 02 Alg


// RSA
#define     HLSE_RSA_KEY_GEN		100		//!< RSA Key Generation
#define     HLSE_RSA_CRT_KEY_GEN	101		//!< RSA CRT Key Generation
#define     HLSE_RSA_NO_PADDING		102		//!< RSA Encryption/Signature
#define     HLSE_RSA_PKCS1_SHA1		103		//!< RSA Encryption/Signature
#define     HLSE_RSA_PKCS1_SHA256	104		//!< RSA Encryption/Signature
#define     HLSE_RSA_PKCS1_SHA1_PREHASH		105		//!< RSA Signature with pre-hashed data
#define     HLSE_RSA_PKCS1_SHA256_PREHASH	106		//!< RSA Signature with pre-hashed data


/** @} */

/// Create handle
#define     HLSE_CREATE_HANDLE(HLSE_READ_ONLY, HLSE_OBJECT_TYPE, HLSE_OBJECT_INDEX)   ((HLSE_READ_ONLY ? 0x80000000 : 0x00000000) | HLSE_OBJECT_TYPE | HLSE_OBJECT_INDEX)

/// Get Object Type
#define     HLSE_GET_OBJECT_TYPE(HLSE_OBJECT_HANDLE)                  (HLSE_OBJECT_HANDLE & 0x7FFF0000)
/// Get Object Index
#define     HLSE_GET_OBJECT_INDEX(HLSE_OBJECT_HANDLE)                 (U8)((HLSE_OBJECT_HANDLE & 0x000000FF))
/// Get Object Full Index
#define     HLSE_GET_OBJECT_FULL_INDEX(HLSE_OBJECT_HANDLE)            ((HLSE_OBJECT_HANDLE & 0x0000FFFF))

/// Get Logical object class
#define     HLSE_GET_LOGICAL_OBJECT_CLASS(HLSE_OBJECT_TYPE)           (U8)((HLSE_OBJECT_TYPE & 0x00FF0000) >> 16)
/// Get logical object type
#define     HLSE_GET_LOGICAL_OBJECT_TYPE(objClass)                    ((objClass << 16u) & 0x00FF0000u)


/*! \brief MechanismInfo - maintains the mechanism type, with an optional additional parameter.
 *
 * Specifying the required information in order to use a mechanism: its type and optional parameters.
 */
typedef struct HLSE_MECHANISM_INFO {
    /// The Mechanism type (\see MechanismType).
    HLSE_MECHANISM_TYPE    mechanism;

    /// An additional optional parameter required in using this mechanism.
    void*            pParameter;
    /// The length in bytes of \p pParameter.
    U16    ulParameterLen;
} HLSE_MECHANISM_INFO;

/*! \brief HKDF Params - maintains the parameters required for calculating HKDF.
 */
typedef struct {
    /// Amount of blocks, equivalent to the pre-shared secret length when multiplied by 16
    U8 nBlock;

    /// Salt data used in extract step
    U8* pSalt;
    /// The length in bytes of \p pSalt.
    U16  ulSaltLen;

    /// Context and application specific information used in expand step
    U8* pInfo;
    /// The length in bytes of \p pInfo.
    U16  ulInfoLen;
} HLSE_HKDF_PARAMS;

/*! \brief ECDH Params - maintains the parameters required for establishing ECDH.
 */
typedef struct {
    /// The other side's Public key
    U8* pPublicKey;
    /// The length in bytes of \p pPublicKey.
    U16  ulPublicKeyLen;
} HLSE_ECDH_PARAMS;


/*! \brief TLS PSK Params - maintains the parameters required for calculating TLS PSK.
 */
typedef struct {
    /// Amount of blocks, equivalent to the pre-shared secret length when multiplied by 16
    U8 nBlock;

    /// The PRF creating the masterSecret also takes as parameter the ServerHello.random
    U8*  pSeed;
    /// The length in bytes of \p pSeed.
    U16   ulSeedLen;
} HLSE_TLS_PSK_MASTER_KEY_DERIVE_PARAMS;

/*! \brief TLS PSK ECDH Params - maintains the parameters required for calculating TLS PSK over ECDH.
 */
typedef struct {
    /// Amount of blocks, equivalent to the PSK length when multiplied by Block Size
    U8  numBlocks;

    /// The PRF creating the masterSecret also takes as parameter the ServerHello.random
    U8*  pSeed;
    /// The length in bytes of \p pSeed.
    U16   ulSeedLen;

    /// Handle of the  Symmetric key containing the MSB part of the pre-shared secret
    HLSE_OBJECT_HANDLE symKeyHandle;

    /// The other side's Public key
    U8*  pPublicKey;
    /// The length in bytes of \p pPublicKey.
    U16   ulPublicKeyLen;
} HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC_PARAMS;


#ifdef TGT_A71CL
/*! \brief KEK_WRAPPED_OBJECT_PARAMS - maintains the parameters required for setting a new value wrapped with the KEK.
*/
typedef struct {
    /// The Key Encryption Key value
    U8   KEK[16];
    /// The actual Key Encryption Key value passed , 0 if not used
    U16  KEKLen;

    /// The new value
    U8*   value;
    /// The length in bytes of \p value.
    U16  valueLen;
} HLSE_KEK_WRAPPED_OBJECT_PARAMS;
#endif

//*******************************************************************
// Communication and Secure Channel
//*******************************************************************

/**
 * Contains the information required to resume a connection with the Security Module.
 * Its content is only to be interpreted by the Host Library.
 * The semantics of the param1 and param2 fields depends on the link layer.
 */
typedef struct {
    U16 param1;          //!< Useage depends on link layer
    U16 param2;          //!< Useage depends on link layer
    U16 hostLibVersion;  //!< MSByte contains major version (::AX_HOST_LIB_MAJOR); LSByte contains minor version of HostLib (::AX_HOST_LIB_MINOR)
    U16 appletVersion;   /*!< MSByte contains major version;
                         3 leading bits of LSByte contains minor version of Applet;
                         Last bit of LSByte encodes whether Applet is in Debug Mode, a '1' means 'Debug Mode' is available */
    U16 sbVersion;       //!< Expected to be 0x0000

    U8  atr[256];       //!< Secure Element's returned ATR (Answer To Reset)
    U16 atrLen;         //!< The length in bytes of \p atr
} HLSE_COMMUNICATION_STATE;

/** Connection type.
 */
typedef HLSE_TYPE HLSE_CONNECTION_TYPE;

#define     HLSE_RJCT_CONNECTION_TYPE  0x01     //!< Connection over RJCT protocol
#define     HLSE_I2C_CONNECTION_TYPE  0x02      //!< Connection over I2C Protocol
#define     HLSE_SPI_CONNECTION_TYPE  0x03      //!< Connection over SPI Protocol
#define     HLSE_PCSC_CONNECTION_TYPE  0x04     //!< Connection over PCSC Protocol

/**
 * Contains information required for establishing connection with the Secure Element.
 */
typedef struct {
    /// The connection type
    HLSE_CONNECTION_TYPE    type;
    uint8_t connType;
    /// An additional optional parameter required in using this connection
    void*            pParameter;
    /// The length in bytes of \p pParameter.
    U16    ulParameterLen;
} HLSE_CONNECTION_PARAMS;


/** Close Connection Mode type.
 */
typedef HLSE_TYPE HLSE_CLOSE_CONNECTION_MODE;

#define     HLSE_CLOSE_CONNECTION_RESET  0      //!< Reset the connection upon closing
#define     HLSE_CLOSE_CONNECTION_NO_RESET  1   //!< Do not reset the connection upon closing


/** Channel ID type.
 */
typedef HLSE_TYPE HLSE_CHANNEL_ID;

#define     HLSE_HOST_CHANNEL  0x01     //!< Host Channel ID
#define     HLSE_ADMIN_CHANNEL  0x02    //!< Admin Channel ID

/** Secure Channel type.
 */
typedef HLSE_TYPE HLSE_SECURE_CHANNEL_TYPE;

#define     HLSE_SCP03  0x01        //!< Global Platform's Secure Channel Protocol 03
#define     HLSE_SCP02  0x02        //!< Global Platform's Secure Channel Protocol 02


/**
 * Contains information required for establishing Secure Channel with the Secure Element.
 */
typedef struct {
    /// The Secure Channel type
    HLSE_SECURE_CHANNEL_TYPE     type;

    /// An additional optional parameter required in using this secure channel
    void*            pParameter; // e.g. HLSE_SM_SCP03_ESTABLISH_PARAMS
    /// The length in bytes of \p pParameter.
    U16    ulParameterLen;
} HLSE_SECURE_CHANNEL_ESTABLISH_PARAMS;


#define SCP_KEY_SIZE                    (16)        //!< SCP key Size
#define SCP_MCV_LEN 16                              //!< MAC Chaining Length

/**
 * Contains information required for establishing SCP03.
 */
typedef struct {
    U8 keyEnc[SCP_KEY_SIZE]; //!< SCP03 static secure channel encryption key
    U8 keyMac[SCP_KEY_SIZE]; //!< SCP03 static secure channel authentication key
    U8 keyDek[SCP_KEY_SIZE]; //!< SCP03 data encryption key
} HLSE_SECURE_CHANNEL_SCP03_ESTABLISH_PARAMS;

/**
* Contains information required for establishing SCP02.
*/
typedef struct {
    U8 keyEnc[SCP_KEY_SIZE]; //!< SCP02 static secure channel encryption key
    U8 keyMac[SCP_KEY_SIZE]; //!< SCP02 static secure channel authentication key
    U8 keyDek[SCP_KEY_SIZE]; //!< SCP02 data encryption key
} HLSE_SECURE_CHANNEL_SCP02_ESTABLISH_PARAMS;


/**
 * Contains information required to resume an SCP03 channel with the Security Module.
 * The content of this data structure is only to be interpreted by the Host Library.
 */
typedef struct {
    U8 sEnc[SCP_KEY_SIZE];   //!< SCP03 session channel encryption key
    U8 sMac[SCP_KEY_SIZE];   //!< SCP03 session command authentication key
    U8 sRMac[SCP_KEY_SIZE];  //!< SCP03 session response authentication key
    U8 mcv[SCP_MCV_LEN];     //!< SCP03 MAC chaining value
    U8 cCounter[16];         //!< SCP03 command counter
    HLSE_CHANNEL_ID channel; //!< SCP03 channel id
} HLSE_SCP03_SESSION_STATE;

/**
 * Contains information about the Secure Channel Session state.
 */
typedef struct {
    /// The Secure Channel type
    HLSE_SECURE_CHANNEL_TYPE    type;
    /// An additional optional parameter required for this state
    void*            pParameter; // e.g. HLSE_SCP03_SESSION_STATE
    /// The length in bytes of \p pParameter.
    U16    ulParameterLen;
} HLSE_SECURE_CHANNEL_SESSION_STATE;

/**
 * Contains information about the Secure Channel state.
 */
typedef struct {
    /// The Secure Channel type
    HLSE_SECURE_CHANNEL_TYPE    type;

    /// An additional optional parameter required for this state
    void*            pParameter; // e.g. HLSE_SCP03_CHANNEL_STATE
    /// The length in bytes of \p pParameter.
    U16    ulParameterLen;
} HLSE_SECURE_CHANNEL_STATE;

/// SCP 03 Channel State
typedef struct {
    U8 cCounter[16];        //!< SCP03 command counter
} HLSE_SCP03_CHANNEL_STATE;

/// SCP 02 Channel State
typedef struct {
    U8 cCounter[3];        //!< SCP02 command counter
} HLSE_SCP02_CHANNEL_STATE;



/** Secure Channel Event type.
 */
typedef HLSE_TYPE HLSE_SCP_EVENT;

// SCP03
#define     HLSE_SCP_WRONG_PADDING  0x01    //!< SCP Wrong Padding
#define     HLSE_SCP_WRONG_RESPMAC  0x02    //!< SCP Wrong RESPMAC
#define     HLSE_SCP_GENERIC_FAILURE  0x03  //!< SCP Generic failure

/**
 * The signaling function type that can be used in \ref HLSE_SCP_Subscribe in order to receive Secure Channel events.
 */
typedef void(*HLSE_SCP_SignalFunction)  (HLSE_SCP_EVENT event, void *context);

/** Life cycle state type.
 */
typedef HLSE_TYPE HLSE_LIFE_CYCLE_STATE;

// Enumerates the various lock/unlock modes.
#define HLSE_INJECT_LOCKED      0   //!< Inject locked mode
#define HLSE_MODULE_LOCKED      1   //!< Module locked mode
#define HLSE_MODULE_UNLOCKED    2   //!< module unlocked mode

/**
 * Module lock state parame
 */
typedef struct {
    HLSE_LIFE_CYCLE_STATE   lockMode;       //!< lock mode
    U8                      response[16];   //!< response
} HLSE_A71_MODULE_UNLOCK_PARAMS;


/** @} */

#endif // _HLSE_TYPES_H_
