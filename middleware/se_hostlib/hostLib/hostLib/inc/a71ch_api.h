/**
* @file a71ch_api.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2016 NXP
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
* This file provides the public interface of the A71CH module.
* @par History
* 1.0   1-oct-2016 : Initial version
*
*****************************************************************************/
#ifndef _A71CH_API_
#define _A71CH_API_

#include "sm_types.h"
#include "scp.h"
#include "ax_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------ */
typedef U8 SST_Index_t;

#define A71CH_INJECT_LOCK_STATE_LOCKED   0x01 //!< Locked
#define A71CH_INJECT_LOCK_STATE_UNLOCKED 0x02 //!< Unlocked

#define A71CH_TRANSPORT_LOCK_STATE_LOCKED     0x01  //!< Transport Lock set
#define A71CH_TRANSPORT_LOCK_STATE_UNLOCKED   0x02  //!< Open device, Transport Lock can no longer be set
#define A71CH_TRANSPORT_LOCK_STATE_ALLOW_LOCK 0x03  //!< Open device, Transport Lock can still be set

#define A71CH_NO_RESTRICTED_KP  0x0F  //!< No restricted keypairs available

#define A71CH_KEY_PAIR_0      0x00  //!< Index to credential storage
#define A71CH_KEY_PAIR_1      0x01  //!< Index to credential storage
#define A71CH_KEY_PAIR_2      0x02  //!< Index to credential storage (B device only)
#define A71CH_KEY_PAIR_3      0x03  //!< Index to credential storage (B device only)
#define A71CH_PUBLIC_KEY_0    0x00  //!< Index to credential storage
#define A71CH_PUBLIC_KEY_1    0x01  //!< Index to credential storage
#define A71CH_PUBLIC_KEY_2    0x02  //!< Index to credential storage (B device only)
#define A71CH_SYM_KEY_0       0x00  //!< Index to credential storage
#define A71CH_SYM_KEY_1       0x01  //!< Index to credential storage
#define A71CH_SYM_KEY_2       0x02  //!< Index to credential storage
#define A71CH_SYM_KEY_3       0x03  //!< Index to credential storage
#define A71CH_SYM_KEY_4       0x04  //!< Index to credential storage (B device only)
#define A71CH_SYM_KEY_5       0x05  //!< Index to credential storage (B device only)
#define A71CH_SYM_KEY_6       0x06  //!< Index to credential storage (B device only)
#define A71CH_SYM_KEY_7       0x07  //!< Index to credential storage (B device only)
#define A71CH_COUNTER_0       0x00  //!< Index to credential storage
#define A71CH_COUNTER_1       0x01  //!< Index to credential storage

#define A71CH_CFG_KEY_IDX_MODULE_LOCK  0x00 //!< Index of Module Transport Lock configuration key
#define A71CH_CFG_KEY_IDX_PRIVATE_KEYS 0x01 //!< Index to configuration key storage (private key wrapping key)
#define A71CH_CFG_KEY_IDX_PUBLIC_KEYS  0x02 //!< Index to configuration key storage (public key wrapping key)

#define A71CH_SYM_KEY_COMBINED_MAX  4 //!< Maximum amount of 128 bit symmetric keys that can be combined for a crypto operation in A71CH

#define A71CH_MAP_SIZE_MAX    203 //!< Maximum amount of data that can be retrieved with debug credential info
// #define A71CH_OPERATE_MATRIX_SIZE  8

#define A71CH_MAX_CMD_PAYLOAD_SIZE    255  //!< Maximum size of command data
#define A71CH_SCP03_MAX_PAYLOAD_SIZE  239  //!< Maximum size of command/response data in case SCP03 in enabled
#define A71CH_GP_STORAGE_SIZE_A      1024  //!< The general purpose storage available on dev. version A71CH (Rev. < 1.3)
#define A71CH_GP_STORAGE_SIZE_B      4096  //!< The general purpose storage available on the A71CH product
#define A71CH_GP_STORAGE_GRANULARITY   32  //!< Determines the granularity with which GP storage blocks can be locked
#define A71CH_GP_STORAGE_MAX_DATA_CHUNK   (A71CH_SCP03_MAX_PAYLOAD_SIZE) //!< GP Storage specific implementation parameter
#define DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO         192
#define DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_DERIVED_DATA 255
#define A71CH_HKDF_MAX_SALT                                 32
#define A71CH_HMAC_SHA256_MAX_DATA_CHUNK (A71CH_SCP03_MAX_PAYLOAD_SIZE) //!< HMAC specific implementation parameter
#define A71CH_SHA256_MAX_DATA_CHUNK      (A71CH_SCP03_MAX_PAYLOAD_SIZE) //!< SHA256 specific implementation parameter
#define A71CH_TLS_MAX_LABEL           24  //!< Maximum size of label that can be stored in A71CH

#define A71CH_MODULE_UNLOCK_CHALLENGE_LEN 16  //!< Length (in byte) of challenge created by secure module to get it unlocked
#define A71CH_MODULE_UNIQUE_ID_LEN        18  //!< Length (in byte) of unique identifier contained in secure module
#define A71CH_MODULE_CERT_UID_LEN         10  //!< Length (in byte) of cert unique identifier
#define A71CH_WRAPPED_KEY_LEN             24  //!< Length (in byte) of an RFC3394 wrapped AES key of 128 bits
#define A71CH_PUB_KEY_LEN                 65  //!< Length (in byte) of an ECC P-256 Public Key in ANSI X9.62 uncompressed format
#define AX_SHA256_LEN                     32  //!< Length (in byte) of the result of a SHA256 hash operation
#define AX_TLS_PSK_MASTER_SECRET_LEN      48  //!< Length (in byte) of TLS 1.2 Master Secret
#define AX_TLS_PSK_HELLO_RANDOM_LEN       32  //!< Length (in byte) of TLS 1.2 Hello
#define SESSION_ID_LEN                    4

#define MONOTONIC_COUNTER_BYTE_COUNT       4  //!< Length (in byte) of monotonic counter

#define A71CH_SCP_MANDATORY  0x01            //!< After a successful GP_ExternalAuthenticate, the usage of SCP03 becomes MANDATORY
#define A71CH_SCP_NOT_SET_UP 0x02            //!< No SCP keys written
#define A71CH_SCP_KEYS_SET   0x03            //!< Successfully written SCP keys (with GP_PutKey), SCP03 binding has not yet been set up
#define A71CH_SCP_CHANNEL_STATE_UNKNOWN 0x0F //!< SCP channel state not known

/// @cond
#define A71CH_UID_IC_TYPE_OFFSET               2
#define A71CH_UID_IC_FABRICATION_DATA_OFFSET   8
#define A71CH_UID_IC_SERIAL_NR_OFFSET         10
#define A71CH_UID_IC_BATCH_ID_OFFSET          13
/// @endcond

/* ------------------------------ */

/** \name Module functions
   @{ */
U16 A71_GetCredentialInfo(U8 *map, U16 *mapLen);
U16 A71_GetModuleInfo(U16 *selectResponse, U8 *debugOn, U8 *restrictedKpIdx, U8 *transportLockState, U8 *scpState, U8 *injectLockState, U16 *gpStorageSize);
U16 A71_GetRandom(U8 *random, U8 randomLen);
U16 A71_CreateClientHelloRandom(U8 *clientHello, U8 clientHelloLen);
U16 A71_GetRestrictedKeyPairInfo(U8 *idx, U16 *nBlocks, U8 *blockInfo, U16 *blockInfoLen);
U16 A71_GetSha256(U8 *data, U16 dataLen, U8 *sha, U16 *shaLen);
U16 A71_GetUniqueID(U8 *uid, U16 *uidLen);
U16 A71_GetCertUid(U8 *certUid, U16 *certUidLen);
U16 A71_GetKeyPairChallenge(U8 *challenge, U16 *challengeLen);
U16 A71_GetPublicKeyChallenge(U8 *challenge, U16 *challengeLen);
U16 A71_GetUnlockChallenge(U8 *challenge, U16 *challengeLen);
U16 A71_InjectLock(void);
U16 A71_LockModule(void);
U16 A71_UnlockModule(U8 *code, U16 codeLen);
U16 A71_SetTlsLabel(const U8* label, U16 labelLen);
U16 A71_EccVerifyWithKey(const U8 *pKeyData, U16 keyDataLen, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult);
/** @}*/

/** \name EccKeyPair functions
   @{ */
U16 A71_GenerateEccKeyPair(SST_Index_t index);
U16 A71_GenerateEccKeyPairWithChallenge(SST_Index_t index, const U8 *configKey, U16 configKeyLen);
U16 A71_GenerateEccKeyPairWithCode(SST_Index_t index, const U8 *code, U16 codeLen);
U16 A71_SetEccKeyPair(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen, const U8 *privateKey, U16 privateKeyLen);
U16 A71_GetPublicKeyEccKeyPair(SST_Index_t index, U8 *publicKey, U16 *publicKeyLen);
U16 A71_GetEccKeyPairUsage(SST_Index_t index, U8 *restricted, U16 *usedCnt, U16 *maxUseCnt);
U16 A71_EccSign(SST_Index_t index, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen);
U16 A71_EccNormalizedAsnSign(SST_Index_t index, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen);
U16 A71_EccRestrictedSign(SST_Index_t index, const U8 *updateBytes, U16 updateBytesLen, U8 *invocationCount);
U16 A71_EcdhGetSharedSecret(U8 index, const U8 *pOtherPublicKey, U16 otherPublicKeyLen, U8 *pSharedSecret, U16 *pSharedSecretLen);
U16 A71_FreezeEccKeyPair(SST_Index_t index);
U16 A71_FreezeEccKeyPairWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen);
U16 A71_FreezeEccKeyPairWithCode(SST_Index_t index, U8 *code, U16 codeLen);
U16 A71_EraseEccKeyPair(SST_Index_t index);
U16 A71_EraseEccKeyPairWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen);
U16 A71_EraseEccKeyPairWithCode(SST_Index_t index, U8 *code, U16 codeLen);
/** @}*/

/** \name EccPublicKey functions
   @{ */
U16 A71_SetEccPublicKey(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen);
U16 A71_GetEccPublicKey(SST_Index_t index, U8 *publicKey, U16 *publicKeyLen);
U16 A71_FreezeEccPublicKey(SST_Index_t index);
U16 A71_FreezeEccPublicKeyWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen);
U16 A71_FreezeEccPublicKeyWithCode(SST_Index_t index, U8 *code, U16 codeLen);
U16 A71_EraseEccPublicKey(SST_Index_t index);
U16 A71_EraseEccPublicKeyWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen);
U16 A71_EraseEccPublicKeyWithCode(SST_Index_t index, U8 *code, U16 codeLen);
U16 A71_EccVerify(SST_Index_t index, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult);
/** @}*/

/** \name Symmetric key functions
   @{ */
U16 A71_SetSymKey(SST_Index_t index, const U8 *key, U16 keyLen);
U16 A71_SetRfc3394WrappedAesKey(SST_Index_t index, const U8 *key, U16 keyLen);
U16 A71_FreezeSymKey(SST_Index_t index);
U16 A71_EraseSymKey(SST_Index_t index);
U16 A71_GetHmacSha256(SST_Index_t index, U8 nBlock, const U8 *data, U16 dataLen, U8 *hmac, U16 *hmacLen);
U16 A71_HkdfExpandSymKey(SST_Index_t index, U8 nBlock, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen);
U16 A71_HkdfSymKey(SST_Index_t index, U8 nBlock, const U8 *salt, U16 saltLen, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen);
U16 A71_PskDeriveMasterSecret(SST_Index_t index, U8 nBlock, const U8 *serverHelloRnd, U16 serverHelloRndLen, U8 *masterSecret);
/** @}*/

/** \name General purpose storage functions
   @{ */
U16 A71_SetGpData(U16 dataOffset, const U8 *data, U16 dataLen);
U16 A71_SetGpDataWithLockCheck(U16 dataOffset, const U8 *data, U16 dataLen);
U16 A71_GetGpData(U16 dataOffset, U8 *data, U16 dataLen);
U16 A71_FreezeGpData(U16 dataOffset, U16 dataLen);
/** @}*/

/** \name Monotonic Counter functions
   @{ */
U16 A71_IncrementCounter(SST_Index_t index);
U16 A71_SetCounter(SST_Index_t index, U32 value);
U16 A71_GetCounter(SST_Index_t index, U32 *pValue);
/** @}*/

/** \name Crypto functions referring to multiple stored credentials
   @{ */
U16 A71_EcdhPskDeriveMasterSecret(SST_Index_t indexKp, const U8 *publicKey, U16 publicKeyLen,
    SST_Index_t index, U8 nBlock, const U8 *serverHelloRnd, U16 serverHelloRndLen, U8 *masterSecret);
/** @}*/

/** \name Config key functions
   @{ */
U16 A71_SetConfigKey(SST_Index_t index, const U8 *Key, U16 keyLen);
U16 A71_SetRfc3394WrappedConfigKey(SST_Index_t index, const U8 *Key, U16 keyLen);
/** @}*/


// The Link Layer used to communicate between Host and Secure Element may introduce
// additional constraints on the packetsize. This constraint is set by defining MAX_CHUNK_LENGTH_LINK.
//   NOTE: In sm_apdu.h MAX_APDU_BUF_LENGTH is set. This value corresponds to the maximum APDU
//   buffer length of the Security Module.
// On memory constrained hosts these values can be reduced to a lower value.
// #define MAX_CHUNK_LENGTH_LINK    1024    // Compromise value; can be reduced to about 650
// The A71CH has been tested in combination with: SCI2C interface & SMCOM_JRCP_V1

// TODO: Have the MAX_APDU_BUF_LENGTH take precedence on MAX_CHUNK_LENGTH_LINK
// for the time being the cap on the APDU size is enforced through MAX_CHUNK_LENGTH_LINK

#if defined(SCI2C) || defined (T1oI2C)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CH applet capability
#elif defined(SPI)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CH applet capability
#elif defined(PCSC)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CH applet capability
#elif defined(TDA8029_UART)
#error "Not yet tested"
#elif defined(SMCOM_JRCP_V1)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CH applet capability
#elif defined(SMCOM_JRCP_V2)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CH applet capability
#elif defined(IPC)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CH applet capability
#elif defined(RJCT_VCOM) || defined(SMCOM_JRCP_V1) || defined(SMCOM_JRCP_V2) || defined(SMCOM_PN7150) || defined(SMCOM_THREAD) || defined(SMCOM_PCSC)
#define MAX_CHUNK_LENGTH_LINK     256    //!< Limited by A71CH applet capability
#else
#error "Define a communication layer as a preprocessor constant"
#endif

#ifdef __cplusplus
}
#endif
#endif //_A71CH_API_
