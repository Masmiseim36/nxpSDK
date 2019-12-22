/* Copyright 2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*/

#ifndef SE05x_ENUMS_H
#define SE05x_ENUMS_H

/* + Machine Generated */

typedef enum
{ /** Invalid */
    kSE05x_AppletResID_NA = 0,
    kSE05x_AppletResID_TRANSPORT = 0x7FFF0200,
    kSE05x_AppletResID_KP_FASTSCP_USER = 0x7FFF0201,
    kSE05x_AppletResID_KP_FASTSCP_IMPORT = 0x7FFF0202,
    kSE05x_AppletResID_PUB_FASTSCP_IMPORT = 0x7FFF0203,
    kSE05x_AppletResID_FEATURE = 0x7FFF0204,
    kSE05x_AppletResID_FACTORY_RESET = 0x7FFF0205,
    kSE05x_AppletResID_UNIQUE_ID = 0x7FFF0206,
    kSE05x_AppletResID_PLATFORM_SCP = 0x7FFF0207,
} SE05x_AppletResID_t;

typedef enum
{ /** Invalid */
    kSE05x_SW12_NA = 0,
    /** No Error */
    kSE05x_SW12_NO_ERROR = 0x9000,
    /** Conditions not satisfied */
    kSE05x_SW12_CONDITIONS_NOT_SATISFIED = 0x6985,
    /** Security status not satisfied. */
    kSE05x_SW12_SECURITY_STATUS = 0x6982,
    /** Wrong data provided. */
    kSE05x_SW12_WRONG_DATA = 0x6A80,
    /** Data invalid - policy set invalid for the given object */
    kSE05x_SW12_DATA_INVALID = 0x6984,
    /** Command not allowed - access denied based on object policy */
    kSE05x_SW12_COMMAND_NOT_ALLOWED = 0x6986,
} SE05x_SW12_t;

typedef enum
{ /** Invalid */
    kSE05x_INS_NA = 0,
    /** 3 MSBit for instruction characteristics. */
    kSE05x_INS_MASK_INS_CHAR = 0xE0,
    /** 5 LSBit for instruction */
    kSE05x_INS_MASK_INSTRUCTION = 0x1F,

    /** Mask for transient object creation, can only be combined with INS_WRITE. */
    kSE05x_INS_TRANSIENT = 0x80,
    /** Mask for authentication object creation, can only be combined with INS_WRITE */
    kSE05x_INS_AUTH_OBJECT = 0x40,
    /** Mask for getting attestation data. */
    kSE05x_INS_ATTEST = 0x20,

    /** Write or create a persistent object. */
    kSE05x_INS_WRITE = 0x01,
    /** Read the object */
    kSE05x_INS_READ = 0x02,
    /** Perform Security Operation */
    kSE05x_INS_CRYPTO = 0x03,
    /** General operation */
    kSE05x_INS_MGMT = 0x04,
    /** Process session command */
    kSE05x_INS_PROCESS = 0x05,
} SE05x_INS_t;

typedef enum
{ /** Invalid */
    kSE05x_P1_NA = 0,
    /** Highest bit not used */
    kSE05x_P1_UNUSED = 0x80,
    /** 2 MSBit for key type */
    kSE05x_P1_MASK_KEY_TYPE = 0x60,
    /** 5 LSBit for credential type */
    kSE05x_P1_MASK_CRED_TYPE = 0x1F,

    /** Key pair (private key + public key) */
    kSE05x_P1_KEY_PAIR = 0x60,
    /** Private key */
    kSE05x_P1_PRIVATE = 0x40,
    /** Public key */
    kSE05x_P1_PUBLIC = 0x20,

    kSE05x_P1_DEFAULT = 0x00,
    kSE05x_P1_EC = 0x01,
    kSE05x_P1_RSA = 0x02,
    kSE05x_P1_AES = 0x03,
    kSE05x_P1_DES = 0x04,
    kSE05x_P1_HMAC = 0x05,
    kSE05x_P1_BINARY = 0x06,
    kSE05x_P1_UserID = 0x07,
    kSE05x_P1_COUNTER = 0x08,
    kSE05x_P1_PCR = 0x09,
    kSE05x_P1_CURVE = 0x0B,
    kSE05x_P1_SIGNATURE = 0x0C,
    kSE05x_P1_MAC = 0x0D,
    kSE05x_P1_CIPHER = 0x0E,
    kSE05x_P1_TLS = 0x0F,
    kSE05x_P1_CRYPTO_OBJ = 0x10,
} SE05x_P1_t;

typedef enum
{ /** Invalid */
    kSE05x_P2_NA = 0,
    kSE05x_P2_DEFAULT = 0x00,
    kSE05x_P2_GENERATE = 0x03,
    kSE05x_P2_CREATE = 0x04,
    kSE05x_P2_SIZE = 0x07,
    kSE05x_P2_SIGN = 0x09,
    kSE05x_P2_VERIFY = 0x0A,
    kSE05x_P2_INIT = 0x0B,
    kSE05x_P2_UPDATE = 0x0C,
    kSE05x_P2_FINAL = 0x0D,
    kSE05x_P2_ONESHOT = 0x0E,
    kSE05x_P2_DH = 0x0F,
    kSE05x_P2_DIVERSIFY = 0x10,
    kSE05x_P2_AUTH_PART1 = 0x11,
    kSE05x_P2_AUTH_FIRST_PART2 = 0x12,
    kSE05x_P2_AUTH_NONFIRST_PART2 = 0x13,
    kSE05x_P2_DUMP_KEY = 0x14,
    kSE05x_P2_CHANGE_KEY_PART1 = 0x15,
    kSE05x_P2_CHANGE_KEY_PART2 = 0x16,
    kSE05x_P2_KILL_AUTH = 0x17,
    kSE05x_P2_IMPORT = 0x18,
    kSE05x_P2_EXPORT = 0x19,
    kSE05x_P2_SESSION_CREATE = 0x1B,
    kSE05x_P2_SESSION_CLOSE = 0x1C,
    kSE05x_P2_SESSION_REFRESH = 0x1E,
    kSE05x_P2_SESSION_POLICY = 0x1F,
    kSE05x_P2_VERSION = 0x20,
    kSE05x_P2_MEMORY = 0x22,
    kSE05x_P2_LIST = 0x25,
    kSE05x_P2_TYPE = 0x26,
    kSE05x_P2_EXIST = 0x27,
    kSE05x_P2_DELETE_OBJECT = 0x28,
    kSE05x_P2_DELETE_ALL = 0x2A,
    kSE05x_P2_SESSION_UserID = 0x2C,
    kSE05x_P2_HKDF = 0x2D,
    kSE05x_P2_PBKDF = 0x2E,
    kSE05x_P2_I2CM = 0x30,
    kSE05x_P2_I2CM_ATTESTED = 0x31,
    kSE05x_P2_MAC = 0x32,
    kSE05x_P2_UNLOCK_CHALLENGE = 0x33,
    kSE05x_P2_CURVE_LIST = 0x34,
    kSE05x_P2_SIGN_ECDAA = 0x35,
    kSE05x_P2_ID = 0x36,
    kSE05x_P2_ENCRYPT_ONESHOT = 0x37,
    kSE05x_P2_DECRYPT_ONESHOT = 0x38,
    kSE05x_P2_ATTEST = 0x3A,
    kSE05x_P2_ATTRIBUTES = 0x3B,
    kSE05x_P2_CPLC = 0x3C,
    kSE05x_P2_TIME = 0x3D,
    kSE05x_P2_TRANSPORT = 0x3E,
    kSE05x_P2_VARIANT = 0x3F,
    kSE05x_P2_PARAM = 0x40,
    kSE05x_P2_DELETE_CURVE = 0x41,
    kSE05x_P2_ENCRYPT = 0x42,
    kSE05x_P2_DECRYPT = 0x43,
    kSE05x_P2_VALIDATE = 0x44,
    kSE05x_P2_GENERATE_ONESHOT = 0x45,
    kSE05x_P2_VALIDATE_ONESHOT = 0x46,
    kSE05x_P2_CRYPTO_LIST = 0x47,
    kSE05x_P2_RANDOM = 0x49,
    kSE05x_P2_TLS_PMS = 0x4A,
    kSE05x_P2_TLS_PRF_CLI_HELLO = 0x4B,
    kSE05x_P2_TLS_PRF_SRV_HELLO = 0x4C,
    kSE05x_P2_TLS_PRF_CLI_RND = 0x4D,
    kSE05x_P2_TLS_PRF_SRV_RND = 0x4E,
    kSE05x_P2_RAW = 0x4F,
    kSE05x_P2_IMPORT_EXT = 0x51,
    kSE05x_P2_SCP = 0x52,
    kSE05x_P2_AUTH_FIRST_PART1 = 0x53,
    kSE05x_P2_AUTH_NONFIRST_PART1 = 0x54,
} SE05x_P2_t;

// typedef enum
// { /** Invalid */
//     kSE05x_SecureObject_NA = 0,
//     kSE05x_SecureObject_EC_KEY_PAIR = 0x01,
//     kSE05x_SecureObject_EC_PRIV_KEY = 0x02,
//     kSE05x_SecureObject_EC_PUB_KEY = 0x03,
//     kSE05x_SecureObject_RSA_KEY_PAIR = 0x04,
//     kSE05x_SecureObject_RSA_KEY_PAIR_CRT = 0x05,
//     kSE05x_SecureObject_RSA_PRIV_KEY = 0x06,
//     kSE05x_SecureObject_RSA_PRIV_KEY_CRT = 0x07,
//     kSE05x_SecureObject_RSA_PUB_KEY = 0x08,
//     kSE05x_SecureObject_AES_KEY = 0x09,
//     kSE05x_SecureObject_DES_KEY = 0x0A,
//     kSE05x_SecureObject_BINARY_FILE = 0x0B,
//     kSE05x_SecureObject_UserID = 0x0C,
//     kSE05x_SecureObject_COUNTER = 0x0D,
//     kSE05x_SecureObject_PCR = 0x0F,
//     kSE05x_SecureObject_CURVE = 0x10,
//     kSE05x_SecureObject_HMAC_KEY = 0x11,
// } SE05x_SecureObject_t;

typedef enum
{ /** Invalid */
    kSE05x_MemoryType_NA = 0,
    /** Persistent memory */
    kSE05x_MemoryType_PERSISTENT = 0x01,
    /** Transient memory, clear on reset */
    kSE05x_MemoryType_TRANSIENT_RESET = 0x02,
    /** Transient memory, clear on deselect */
    kSE05x_MemoryType_TRANSIENT_DESELECT = 0x03,
} SE05x_MemoryType_t;

typedef enum
{ /** Invalid */
    kSE05x_Origin_NA = 0,
    /** Generated outside the module. */
    kSE05x_Origin_EXTERNAL = 0x01,
    /** Generated inside the module. */
    kSE05x_Origin_INTERNAL = 0x02,
    /** Trust provisioned by NXP */
    kSE05x_Origin_PROVISIONED = 0x03,
} SE05x_Origin_t;

typedef enum
{ /** Invalid */
    kSE05x_TAG_NA = 0,
    kSE05x_TAG_SESSION_ID = 0x10,
    kSE05x_TAG_POLICY = 0x11,
    kSE05x_TAG_MAX_ATTEMPTS = 0x12,
    kSE05x_TAG_1 = 0x41,
    kSE05x_TAG_2 = 0x42,
    kSE05x_TAG_3 = 0x43,
    kSE05x_TAG_4 = 0x44,
    kSE05x_TAG_5 = 0x45,
    kSE05x_TAG_6 = 0x46,
    kSE05x_TAG_7 = 0x47,
    kSE05x_TAG_8 = 0x48,
    kSE05x_TAG_9 = 0x49,
    kSE05x_TAG_10 = 0x4A,
    kSE05x_TAG_GP_CONTRL_REF_PARM = 0xA6,
    kSE05x_GP_TAG_AID = 0x4F,
    kSE05x_GP_TAG_KEY_TYPE = 0x80,
    kSE05x_GP_TAG_KEY_LEN = 0x81,
    kSE05x_GP_TAG_GET_DATA = 0x83,
    kSE05x_GP_TAG_DR_SE = 0x85,
    kSE05x_GP_TAG_RECEIPT = 0x86,
    kSE05x_GP_TAG_SCP_PARMS = 0x90,
} SE05x_TAG_t;

typedef enum
{ /** Invalid */
    kSE05x_ECSignatureAlgo_NA = 0,
    /** NOT SUPPORTED */
    kSE05x_ECSignatureAlgo_PLAIN = 0x09,
    kSE05x_ECSignatureAlgo_SHA = 0x11,
    kSE05x_ECSignatureAlgo_SHA_224 = 0x25,
    kSE05x_ECSignatureAlgo_SHA_256 = 0x21,
    kSE05x_ECSignatureAlgo_SHA_384 = 0x22,
    kSE05x_ECSignatureAlgo_SHA_512 = 0x26,
} SE05x_ECSignatureAlgo_t;

typedef enum
{ /** Invalid */
    kSE05x_EDSignatureAlgo_NA = 0,
    /** Message input must be prehashed (using SHA512). */
    kSE05x_EDSignatureAlgo_ED25519PH_SHA_512 = 0xA3,
} SE05x_EDSignatureAlgo_t;

typedef enum
{ /** Invalid */
    kSE05x_ECDAASignatureAlgo_NA = 0,
    /** Message input must be prehashed (using SHA256) */
    kSE05x_ECDAASignatureAlgo_ECDAA = 0xF4,
} SE05x_ECDAASignatureAlgo_t;

typedef enum
{ /** Invalid */
    kSE05x_RSASignatureAlgo_NA = 0,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignatureAlgo_SHA1_PKCS1_PSS = 0x15,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignatureAlgo_SHA224_PKCS1_PSS = 0x2B,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignatureAlgo_SHA256_PKCS1_PSS = 0x2C,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignatureAlgo_SHA384_PKCS1_PSS = 0x2D,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignatureAlgo_SHA512_PKCS1_PSS = 0x2E,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignatureAlgo_SHA1_PKCS1 = 0x0A,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignatureAlgo_SHA_224_PKCS1 = 0x27,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignatureAlgo_SHA_256_PKCS1 = 0x28,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignatureAlgo_SHA_384_PKCS1 = 0x29,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignatureAlgo_SHA_512_PKCS1 = 0x2A,
} SE05x_RSASignatureAlgo_t;

typedef enum
{ /** Invalid */
    kSE05x_RSAEncryptionAlgo_NA = 0,
    /** Plain RSA, padding required on host. */
    kSE05x_RSAEncryptionAlgo_NO_PAD = 0x0C,
    /** RFC8017: RSAES-PKCS1-v1_5 */
    kSE05x_RSAEncryptionAlgo_PKCS1 = 0x0A,
    /** RFC8017: RSAES-OAEP */
    kSE05x_RSAEncryptionAlgo_PKCS1_OAEP = 0x0F,
} SE05x_RSAEncryptionAlgo_t;

typedef enum
{ /** Invalid */
    kSE05x_RSABitLength_NA = 0,
    kSE05x_RSABitLength_512 = 512,
    kSE05x_RSABitLength_1024 = 1024,
    kSE05x_RSABitLength_1152 = 1152,
    kSE05x_RSABitLength_2048 = 2048,
    kSE05x_RSABitLength_3072 = 3072,
    kSE05x_RSABitLength_4096 = 4096,
} SE05x_RSABitLength_t;

typedef enum
{ /** Invalid */
    kSE05x_RSAKeyComponent_NA = 0,
    /** Modulus */
    kSE05x_RSAKeyComponent_MOD = 0x00,
    /** Public key exponent */
    kSE05x_RSAKeyComponent_PUB_EXP = 0x01,
    /** Private key exponent */
    kSE05x_RSAKeyComponent_PRIV_EXP = 0x02,
    /** CRT component p */
    kSE05x_RSAKeyComponent_P = 0x03,
    /** CRT component q */
    kSE05x_RSAKeyComponent_Q = 0x04,
    /** CRT component dp */
    kSE05x_RSAKeyComponent_DP = 0x05,
    /** CRT component dq */
    kSE05x_RSAKeyComponent_DQ = 0x06,
    /** CRT component q_inv */
    kSE05x_RSAKeyComponent_INVQ = 0x07,
} SE05x_RSAKeyComponent_t;

typedef enum
{ /** Invalid */
    kSE05x_DigestMode_NA = 0,
    kSE05x_DigestMode_NO_HASH = 0x00,
    kSE05x_DigestMode_SHA = 0x01,
    /** Not supported */
    kSE05x_DigestMode_SHA224 = 0x07,
    kSE05x_DigestMode_SHA256 = 0x04,
    kSE05x_DigestMode_SHA384 = 0x05,
    kSE05x_DigestMode_SHA512 = 0x06,
} SE05x_DigestMode_t;

typedef enum
{ /** Invalid */
    kSE05x_MACAlgo_NA = 0,
    kSE05x_MACAlgo_HMAC_SHA1 = 0x18,
    kSE05x_MACAlgo_HMAC_SHA256 = 0x19,
    kSE05x_MACAlgo_HMAC_SHA384 = 0x1A,
    kSE05x_MACAlgo_HMAC_SHA512 = 0x1B,
    kSE05x_MACAlgo_CMAC_128 = 0x31,
} SE05x_MACAlgo_t;

typedef enum
{ /** Invalid */
    kSE05x_ECCurve_NA = 0,
    kSE05x_ECCurve_UNUSED = 0x00,
    kSE05x_ECCurve_NIST_P192 = 0x01,
    kSE05x_ECCurve_NIST_P224 = 0x02,
    kSE05x_ECCurve_NIST_P256 = 0x03,
    kSE05x_ECCurve_NIST_P384 = 0x04,
    kSE05x_ECCurve_NIST_P521 = 0x05,
    kSE05x_ECCurve_Brainpool160 = 0x06,
    kSE05x_ECCurve_Brainpool192 = 0x07,
    kSE05x_ECCurve_Brainpool224 = 0x08,
    kSE05x_ECCurve_Brainpool256 = 0x09,
    kSE05x_ECCurve_Brainpool320 = 0x0A,
    kSE05x_ECCurve_Brainpool384 = 0x0B,
    kSE05x_ECCurve_Brainpool512 = 0x0C,
    kSE05x_ECCurve_Secp160k1 = 0x0D,
    kSE05x_ECCurve_Secp192k1 = 0x0E,
    kSE05x_ECCurve_Secp224k1 = 0x0F,
    kSE05x_ECCurve_Secp256k1 = 0x10,
    kSE05x_ECCurve_TPM_ECC_BN_P256 = 0x11,
    kSE05x_ECCurve_RESERVED_ID_ECC_ED_25519 = 0x40,
    kSE05x_ECCurve_RESERVED_ID_ECC_MONT_DH_25519 = 0x41,
} SE05x_ECCurve_t;

#define kSE05x_ECCurve_Total_Weierstrass_Curves kSE05x_ECCurve_TPM_ECC_BN_P256

typedef enum
{   /** Invalid */
    kSE05x_ECCurveParam_NA = 0,
    kSE05x_ECCurveParam_PARAM_A = 0x01,
    kSE05x_ECCurveParam_PARAM_B = 0x02,
    kSE05x_ECCurveParam_PARAM_G = 0x04,
    kSE05x_ECCurveParam_PARAM_N = 0x08,
    kSE05x_ECCurveParam_PARAM_PRIME = 0x10,
} SE05x_ECCurveParam_t;

typedef enum
{ /** Invalid */
    kSE05x_CipherMode_NA = 0,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_CBC_NOPAD = 0x01,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_CBC_ISO9797_M1 = 0x02,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_CBC_ISO9797_M2 = 0x03,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_CBC_PKCS5 = 0x04,
    /** Typically using DESKey identifiers */
    kSE05x_CipherMode_DES_ECB_NOPAD = 0x05,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_ECB_ISO9797_M1 = 0x06,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_ECB_ISO9797_M2 = 0x07,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_DES_ECB_PKCS5 = 0x08,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_ECB_NOPAD = 0x0E,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CBC_NOPAD = 0x0D,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CBC_ISO9797_M1 = 0x16,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CBC_ISO9797_M2 = 0x17,
    /** NOT SUPPORTED */
    kSE05x_CipherMode_AES_CBC_PKCS5 = 0x18,
    /** Typically using AESKey identifiers */
    kSE05x_CipherMode_AES_CTR = 0xF0,
} SE05x_CipherMode_t;

typedef enum {
    /** Invalid */
    kSE05x_AppletConfig_NA = 0,
    /** Use of curve TPM_ECC_BN_P256 */
    kSE05x_AppletConfig_ECDAA = 0x0001,
    /** EC DSA and DH support */
    kSE05x_AppletConfig_ECDSA_ECDH_ECDHE = 0x0002,
    /** Use of curve RESERVED_ID_ECC_ED_25519 */
    kSE05x_AppletConfig_EDDSA = 0x0004,
    /** Use of curve RESERVED_ID_ECC_MONT_DH_25519 */
    kSE05x_AppletConfig_DH_MONT = 0x0008,
    /** Writing HMACKey objects */
    kSE05x_AppletConfig_HMAC = 0x0010,
    /** Writing RSAKey objects */
    kSE05x_AppletConfig_RSA_PLAIN = 0x0020,
    /** Writing RSAKey objects */
    kSE05x_AppletConfig_RSA_CRT = 0x0040,
    /** Writing AESKey objects */
    kSE05x_AppletConfig_AES = 0x0080,
    /** Writing DESKey objects */
    kSE05x_AppletConfig_DES = 0x0100,
    /** PBKDF2 */
    kSE05x_AppletConfig_PBKDF = 0x0200,
    /** TLS Handshake support commands (see 4.16) in APDU Spec*/
    kSE05x_AppletConfig_TLS = 0x0400,
    /** Mifare DESFire support (see 4.15)  in APDU Spec*/
    kSE05x_AppletConfig_MIFARE = 0x0800,
    /** RFU1 */
    kSE05x_AppletConfig_RFU1 = 0x1000,
    /** I2C Master support (see 4.17)  in APDU Spec*/
    kSE05x_AppletConfig_I2CM = 0x2000,
    /** RFU2 */
    kSE05x_AppletConfig_RFU2 = 0x4000,
} SE05x_AppletConfig_t;

typedef enum
{ /** Invalid */
    kSE05x_LockIndicator_NA = 0,
    kSE05x_LockIndicator_TRANSIENT_LOCK = 0x01,
    //    kSE05x_LockIndicator_PERSISTENT_LOCK = Any except 0x01,
} SE05x_LockIndicator_t;

typedef enum
{ /** Invalid */
    kSE05x_LockState_NA = 0,
    kSE05x_LockState_LOCKED = 0x01,
    //    kSE05x_LockState_UNLOCKED = Any except 0x01,
} SE05x_LockState_t;

typedef enum
{ /** Invalid */
    kSE05x_CryptoContext_NA = 0,
    /** For DigestInit/DigestUpdate/DigestFinal */
    kSE05x_CryptoContext_DIGEST = 0x01,
    /** For CipherInit/CipherUpdate/CipherFinal */
    kSE05x_CryptoContext_CIPHER = 0x02,
    /** For MACInit/MACUpdate/MACFinal */
    kSE05x_CryptoContext_SIGNATURE = 0x03,
} SE05x_CryptoContext_t;

typedef enum
{ /** Invalid */
    kSE05x_Result_NA = 0,
    kSE05x_Result_SUCCESS = 0x01,
    kSE05x_Result_FAILURE = 0x02,
} SE05x_Result_t;

typedef enum
{ /** Invalid */
    kSE05x_TransientIndicator_NA = 0,
    kSE05x_TransientIndicator_PERSISTENT = 0x01,
    kSE05x_TransientIndicator_TRANSIENT = 0x02,
} SE05x_TransientIndicator_t;

typedef enum
{ /** Invalid */
    kSE05x_SetIndicator_NA = 0,
    kSE05x_SetIndicator_NOT_SET = 0x01,
    kSE05x_SetIndicator_SET = 0x02,
} SE05x_SetIndicator_t;

typedef enum
{ /** Invalid */
    kSE05x_MoreIndicator_NA = 0,
    /** No more data available */
    kSE05x_MoreIndicator_NO_MORE = 0x01,
    /** More data available */
    kSE05x_MoreIndicator_MORE = 0x02,
} SE05x_MoreIndicator_t;

typedef enum
{ /** Invalid */
    kSE05x_PlatformSCPRequest_NA = 0,
    /** Platform SCP is required (full enc & MAC) */
    kSE05x_PlatformSCPRequest_REQUIRED = 0x01,
    /** No platform SCP required. */
    kSE05x_PlatformSCPRequest_NOT_REQUIRED = 0x02,
} SE05x_PlatformSCPRequest_t;

typedef enum
{ /** Invalid */
    kSE05x_CryptoObject_NA = 0,
    kSE05x_CryptoObject_DIGEST_SHA,
    kSE05x_CryptoObject_DIGEST_SHA224,
    kSE05x_CryptoObject_DIGEST_SHA256,
    kSE05x_CryptoObject_DIGEST_SHA384,
    kSE05x_CryptoObject_DIGEST_SHA512,
    kSE05x_CryptoObject_DES_CBC_NOPAD,
    kSE05x_CryptoObject_DES_CBC_ISO9797_M1,
    kSE05x_CryptoObject_DES_CBC_ISO9797_M2,
    kSE05x_CryptoObject_DES_CBC_PKCS5,
    kSE05x_CryptoObject_DES_ECB_NOPAD,
    kSE05x_CryptoObject_DES_ECB_ISO9797_M1,
    kSE05x_CryptoObject_DES_ECB_ISO9797_M2,
    kSE05x_CryptoObject_DES_ECB_PKCS5,
    kSE05x_CryptoObject_AES_ECB_NOPAD,
    kSE05x_CryptoObject_AES_CBC_NOPAD,
    kSE05x_CryptoObject_AES_CBC_ISO9797_M1,
    kSE05x_CryptoObject_AES_CBC_ISO9797_M2,
    kSE05x_CryptoObject_AES_CBC_PKCS5,
    kSE05x_CryptoObject_AES_CTR,
    kSE05x_CryptoObject_HMAC_SHA1,
    kSE05x_CryptoObject_HMAC_SHA256,
    kSE05x_CryptoObject_HMAC_SHA384,
    kSE05x_CryptoObject_HMAC_SHA512,
    kSE05x_CryptoObject_CMAC_128,
} SE05x_CryptoObject_t;

/* - Machine Genreated  */


typedef enum
{

    kSE05x_RES_ID_TRANSPORT = 0x7FFF0200,
    kSE05x_RES_ID_KP_FASTSCP_USER = 0x7FFF0201,
    kSE05x_RES_ID_KP_FASTSCP_IMPORT = 0x7FFF0202,
    kSE05x_RES_ID_PUB_FASTSCP_IMPORT = 0x7FFF0203,
    kSE05x_RES_ID_FEATURE = 0x7FFF0204,
    kSE05x_RES_ID_FACTORY_RESET = 0x7FFF0205,
    kSE05x_RES_ID_UNIQUE_ID = 0x7FFF0206,
} SE05x_RES_ID_t;

#define SE05x_CryptoObjectID_t SE05x_CryptoObject_t

#define SE050_MAX_NUMBER_OF_SESSIONS 3
#define SE050_OBJECT_IDENTIFIER_SIZE 4
#define SE050_MAX_I2CM_COMMAND_LENGTH 255
#define SE050_MAX_APDU_PAYLOAD_LENGTH 896
#define SE050_DEFAULT_MAX_ATTEMPTS 10

/* 3 MSBit for instruction characteristics. */
#define SE050_INS_MASK_INS_CHAR 0xE0
/* 5 LSBit for instruction */
#define SE050_INS_MASK_INSTRUCTION 0x1F

typedef enum
{
    /**  */
    kSE05x_SecObjTyp_EC_KEY_PAIR = 0x01,
    /**  */
    kSE05x_SecObjTyp_EC_PRIV_KEY = 0x02,
    /**  */
    kSE05x_SecObjTyp_EC_PUB_KEY = 0x03,
    /**  */
    kSE05x_SecObjTyp_RSA_KEY_PAIR = 0x04,
    /**  */
    kSE05x_SecObjTyp_RSA_KEY_PAIR_CRT = 0x05,
    /**  */
    kSE05x_SecObjTyp_RSA_PRIV_KEY = 0x06,
    /**  */
    kSE05x_SecObjTyp_RSA_PRIV_KEY_CRT = 0x07,
    /**  */
    kSE05x_SecObjTyp_RSA_PUB_KEY = 0x08,
    /**  */
    kSE05x_SecObjTyp_AES_KEY = 0x09,
    /**  */
    kSE05x_SecObjTyp_DES_KEY = 0x0A,
    /**  */
    kSE05x_SecObjTyp_BINARY_FILE = 0x0B,
    /**  */
    kSE05x_SecObjTyp_UserID = 0x0C,
    /**  */
    kSE05x_SecObjTyp_COUNTER = 0x0D,
    /**  */
    kSE05x_SecObjTyp_PCR = 0x0F,
    /**  */
    kSE05x_SecObjTyp_CURVE = 0x10,
    /**  */
    kSE05x_SecObjTyp_HMAC_KEY = 0x11,
} SE05x_SecObjTyp_t;

typedef SE05x_SecObjTyp_t SE05x_SecureObjectType_t;

typedef enum
{
    /** Transient memory, clear on reset */
    kSE05x_MemTyp_TRANSIENT_RESET = 0x01,
    /** Transient memory, clear on deselect */
    kSE05x_MemTyp_TRANSIENT_DESELECT = 0x02,
    /** Persistent memory */
    kSE05x_MemTyp_PERSISTENT = 0x03,
} SE05x_MemTyp_t;



typedef enum
{
    /** Invalid */
    kSE05x_RSASignAlgo_NA = 0,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignAlgo_SHA1_PKCS1_PSS = 0x15,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignAlgo_SHA224_PKCS1_PSS = 0x2B,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignAlgo_SHA256_PKCS1_PSS = 0x2C,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignAlgo_SHA384_PKCS1_PSS = 0x2D,
    /** RFC8017: RSASSA-PSS */
    kSE05x_RSASignAlgo_SHA512_PKCS1_PSS = 0x2E,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignAlgo_SHA_224_PKCS1 = 0x27,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignAlgo_SHA_256_PKCS1 = 0x28,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignAlgo_SHA_384_PKCS1 = 0x29,
    /** RFC8017: RSASSA-PKCS1-v1_5 */
    kSE05x_RSASignAlgo_SHA_512_PKCS1 = 0x2A,
} SE05x_RSASignAlgo_t;

// typedef enum
// {
//     /** Plain RSA, padding required on host. */
//     kSE05x_RSAEncrAlgo_NO_PAD = 0x0C,
//     * RFC8017: RSAES-PKCS1-v1_5
//     kSE05x_RSAEncrAlgo_PKCS1 = 0x0A,
//     /** RFC8017: RSAES-OAEP */
//     kSE05x_RSAEncrAlgo_PKCS1_OAEP = 0x0F,
// } SE05x_RSAEncrAlgo_t;

/** Public part of RSA Keys */
typedef enum
{
    kSE05x_RSAPubKeyComp_NA = 0,
    kSE05x_RSAPubKeyComp_MOD = kSE05x_RSAKeyComponent_MOD,
    kSE05x_RSAPubKeyComp_PUB_EXP = kSE05x_RSAKeyComponent_PUB_EXP,
} SE05x_RSAPubKeyComp_t;

/** Cyrpto module subtype */
typedef union {
    /** In case it's digest */
    SE05x_DigestMode_t digest;
    /** In case it's cipher */
    SE05x_CipherMode_t cipher;
    /** In case it's mac */
    SE05x_MACAlgo_t mac;
    /** Accessing 8 bit value for APDUs */
    uint8_t u8;
} SE05x_CryptoModeSubType_t;

/** @addtogroup se050_i2cm
 *
 * @{
 */
/** @brief I2C Master micro operation */
typedef enum
{
    kSE05x_TAG_I2CM_Config = 0x01,
    kSE05x_TAG_I2CM_Write = 0x03,
    kSE05x_TAG_I2CM_Read = 0x04,
} SE05x_I2CM_TAG_t;

/*!
*@}
*/ /* end of se050_i2cm */

/** Whether key is transinet of persistant */
typedef enum
{
    kSE05x_TransientType_Persistent = 0,
    kSE05x_TransientType_Transient = kSE05x_INS_TRANSIENT,
} SE05x_TransientType_t;

typedef enum
{
    kSE05x_KeyPart_NA = kSE05x_P1_DEFAULT,
    /** Key pair (private key + public key) */
    kSE05x_KeyPart_Pair = kSE05x_P1_KEY_PAIR,
    /** Private key */
    kSE05x_KeyPart_Private = kSE05x_P1_PRIVATE,
    /** Public key */
    kSE05x_KeyPart_Public = kSE05x_P1_PUBLIC,
} SE05x_KeyPart_t;

typedef enum
{
    kSE05x_Cipher_Oper_NA = 0,
    kSE05x_Cipher_Oper_Encrypt = kSE05x_P2_ENCRYPT,
    kSE05x_Cipher_Oper_Decrypt = kSE05x_P2_DECRYPT,
} SE05x_Cipher_Oper_t;

typedef enum
{
    kSE05x_Cipher_Oper_OneShot_NA = 0,
    kSE05x_Cipher_Oper_OneShot_Encrypt = kSE05x_P2_ENCRYPT_ONESHOT,
    kSE05x_Cipher_Oper_OneShot_Decrypt = kSE05x_P2_DECRYPT_ONESHOT,
} SE05x_Cipher_Oper_OneShot_t;

typedef enum
{
    kSE05x_Mac_Oper_NA = 0,
    kSE05x_Mac_Oper_Generate = kSE05x_P2_GENERATE,
    kSE05x_Mac_Oper_Validate = kSE05x_P2_VALIDATE,
} SE05x_Mac_Oper_t;

typedef enum
{
    kSE05x_AttestationType_None = 0,
    kSE05x_AttestationType_AUTH = kSE05x_INS_AUTH_OBJECT,
} SE05x_AttestationType_t;

typedef enum
{
    kSE05x_SymmKeyType_AES = kSE05x_P1_AES,
    kSE05x_SymmKeyType_DES = kSE05x_P1_DES,
    kSE05x_SymmKeyType_HMAC = kSE05x_P1_HMAC,
    kSE05x_SymmKeyType_CMAC = kSE05x_P1_AES,
} SE05x_SymmKeyType_t;

typedef SE05x_AppletConfig_t SE05x_Variant_t;

typedef enum
{
    kSE05x_TLS_PRF_NA = 0,
    kSE05x_TLS_PRF_CLI_HELLO = kSE05x_P2_TLS_PRF_CLI_HELLO,
    kSE05x_TLS_PRF_SRV_HELLO = kSE05x_P2_TLS_PRF_SRV_HELLO,
    kSE05x_TLS_PRF_CLI_RND = kSE05x_P2_TLS_PRF_CLI_RND,
    kSE05x_TLS_PRF_SRV_RND = kSE05x_P2_TLS_PRF_SRV_RND,
} SE05x_TLSPerformPRFType_t;

typedef enum
{
    kSE05x_AttestationAlgo_NA = 0,
    kSE05x_AttestationAlgo_EC_PLAIN = kSE05x_ECSignatureAlgo_PLAIN,
    kSE05x_AttestationAlgo_EC_SHA = kSE05x_ECSignatureAlgo_SHA,
    kSE05x_AttestationAlgo_EC_SHA_224 = kSE05x_ECSignatureAlgo_SHA_224,
    kSE05x_AttestationAlgo_EC_SHA_256 = kSE05x_ECSignatureAlgo_SHA_256,
    kSE05x_AttestationAlgo_EC_SHA_384 = kSE05x_ECSignatureAlgo_SHA_384,
    kSE05x_AttestationAlgo_EC_SHA_512 = kSE05x_ECSignatureAlgo_SHA_512,
    kSE05x_AttestationAlgo_ED25519PH_SHA_512 = kSE05x_EDSignatureAlgo_ED25519PH_SHA_512,
    kSE05x_AttestationAlgo_ECDAA = kSE05x_ECDAASignatureAlgo_ECDAA,
    kSE05x_AttestationAlgo_RSA_SHA1_PKCS1_PSS = kSE05x_RSASignatureAlgo_SHA1_PKCS1_PSS,
    kSE05x_AttestationAlgo_RSA_SHA224_PKCS1_PSS = kSE05x_RSASignatureAlgo_SHA224_PKCS1_PSS,
    kSE05x_AttestationAlgo_RSA_SHA256_PKCS1_PSS = kSE05x_RSASignatureAlgo_SHA256_PKCS1_PSS,
    kSE05x_AttestationAlgo_RSA_SHA384_PKCS1_PSS = kSE05x_RSASignatureAlgo_SHA384_PKCS1_PSS,
    kSE05x_AttestationAlgo_RSA_SHA512_PKCS1_PSS = kSE05x_RSASignatureAlgo_SHA512_PKCS1_PSS,
    kSE05x_AttestationAlgo_RSA_SHA_224_PKCS1 = kSE05x_RSASignatureAlgo_SHA_224_PKCS1,
    kSE05x_AttestationAlgo_RSA_SHA_256_PKCS1 = kSE05x_RSASignatureAlgo_SHA_256_PKCS1,
    kSE05x_AttestationAlgo_RSA_SHA_384_PKCS1 = kSE05x_RSASignatureAlgo_SHA_384_PKCS1,
    kSE05x_AttestationAlgo_RSA_SHA_512_PKCS1 = kSE05x_RSASignatureAlgo_SHA_512_PKCS1,

} SE05x_AttestationAlgo_t;

typedef enum
{
    kSE05x_RSAKeyFormat_CRT = kSE05x_P2_DEFAULT,
    kSE05x_RSAKeyFormat_RAW = kSE05x_P2_RAW,
} SE05x_RSAKeyFormat_t;


typedef SE05x_MACAlgo_t SE05x_MacOperation_t;

typedef uint32_t SE05x_KeyID_t;
/** Case when thes is no KEK */
#define SE05x_KeyID_KEK_NONE 0

/** [Optional: if the authentication key is the same as the key to be replaced, this TAG should not be present]. */
#define SE05x_KeyID_MFDF_NONE 0

typedef uint16_t SE05x_MaxAttemps_t;
/* Fall back to applet default */
#define SE05x_MaxAttemps_UNLIMITED 0
/* Identify in code that this is not an AUTH obect and hence not applicable */
#define SE05x_MaxAttemps_NA 0


#define kSE05x_INS_READ_With_Attestation (kSE05x_INS_READ | kSE05x_INS_ATTEST)

#define kSE05x_INS_I2CM_Attestation (kSE05x_INS_CRYPTO | kSE05x_INS_ATTEST)

#define SE05X_RSA_NO_p /* Skip */ NULL, 0
#define SE05X_RSA_NO_q /* Skip */ NULL, 0
#define SE05X_RSA_NO_dp /* Skip */ NULL, 0
#define SE05X_RSA_NO_dq /* Skip */ NULL, 0
#define SE05X_RSA_NO_qInv /* Skip */ NULL, 0
#define SE05X_RSA_NO_pubExp /* Skip */ NULL, 0
#define SE05X_RSA_NO_priv /* Skip */ NULL, 0
#define SE05X_RSA_NO_pubMod /* Skip */ NULL, 0

#endif /* SE05x_ENUMS_H */
