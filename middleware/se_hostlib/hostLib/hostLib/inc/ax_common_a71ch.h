/**
 * @file ax_common.h
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
 * Common data types and defines of the AX-family host library.
 * @par History
 * 1.0   27-march-2014 : Initial version
 *
 */
#ifndef _AX_COMMON_A71CH_
#define _AX_COMMON_A71CH_

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include "scp.h"
#include "sm_types.h"

#if SSS_HAVE_SCP_SCP03_SSS
#include "fsl_sss_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TGT_A70CM
#define DEFAULT_PRIVATE_KEY_INDEX   0
#define DEFAULT_PUBLIC_KEY_INDEX    0
#define DEFAULT_CERTIFICATE_INDEX   0
#endif

// Security module lifecycle states
#ifdef TGT_A70CI
#define SM_FACTORY            (0x6996)
#define SM_CONFIGURE_INITIAL  (0x5AA5)
#define SM_CONFIGURE          (0x4BB4)
#define SM_OPERATE            (0x3CC3)
#define SM_DEAD               (0x1EE1)
#define SM_UNKNOWN            (0x1234)  // State does not exist
#elif defined(TGT_A70CM)
#define SM_FACTORY            0x6996
#define SM_CONFIGURE_INITIAL  0x5AA5
#define SM_CONFIGURE          0x4BB4
#define SM_OPERATE            0x3CC3
#define SM_RE_CONFIGURE       0x2DD2
#define SM_DEAD               0x1EE1
#define SM_UNKNOWN            0x1234  // State does not exist
#elif defined(TGT_A71CH)
// NOTE: A71CH does not support an explicit lifecycle state concept
#else
#define SM_FACTORY           0x00
#define SM_LOCKED            0x11
#define SM_CONFIGURE_INITIAL 0x21
#define SM_CONFIGURE         0x22
#define SM_OPERATE           0x31
#define SM_DIAGNOSTIC        0x41
#define SM_DEACTIVATED       0x51
#define SM_DEAD              0x61
#endif // TGT_A70CI

/**
 * Upto two SCP03 channels can be managed: either a Host or an Admin channel.
 * The A71CH only supports one channel: AX_HOST_CHANNEL
 */
typedef enum ChannelId
{
    AX_HOST_CHANNEL            = 0x01,  //!< SCP03 channel identifier
    AX_ADMIN_CHANNEL           = 0x02   //!< SCP03 channel identifier
} ChannelId_t;

typedef enum ScpEvent
{
    SCP_WRONG_PADDING   = 0x01,
    SCP_WRONG_RESPMAC   = 0x02,
    SCP_GENERIC_FAILURE = 0x03
} ScpEvent_t;

typedef enum
{
  AX_ENCRYPT = 0x01,
  AX_DECRYPT = 0x02
} AES_Direction_t;

typedef enum
{
   AX_VERIFY_SUCCESS = 0x01,
   AX_VERIFY_FAILURE = 0x02
} Verification_t;

// typedef U8 SST_Index_t;

/**
 * Contains information required to resume an SCP03 channel with the Security Module.
 * The content of this data structure is only to be interpreted by the Host Library.
 */
typedef struct {
    U8 sEnc[SCP_KEY_SIZE];  //!< SCP03 session channel encryption key
    U8 sMac[SCP_KEY_SIZE];  //!< SCP03 session command authentication key
    U8 sRMac[SCP_KEY_SIZE]; //!< SCP03 session response authentication key
    U8 mcv[SCP_MCV_LEN];    //!< SCP03 MAC chaining value
    U8 cCounter[16];        //!< SCP03 command counter
    ChannelId_t channel;
} Scp03SessionState_t;

/**
 * Stores the security sensitive static SCP03 keyset.
 */
typedef struct {
    U8 keyEnc[SCP_KEY_SIZE]; //!< SCP03 static secure channel encryption key
    U8 keyMac[SCP_KEY_SIZE]; //!< SCP03 static secure channel authentication key
    U8 keyDek[SCP_KEY_SIZE]; //!< SCP03 data encryption key
    Scp03SessionState_t session; //!< SCP03 session state
} ScpState_t;

#if SSS_HAVE_SCP_SCP03_SSS
typedef struct {
    sss_object_t sEnckeyObj;  //!< SCP03 session channel encryption key obj
    sss_object_t sMackeyObj;  //!< SCP03 session command authentication key obj
    sss_object_t sRMackeyObj; //!< SCP03 session response authentication key obj
    U8 mcv[SCP_MCV_LEN];    //!< SCP03 MAC chaining value
    U8 cCounter[16];        //!< SCP03 command counter
    ChannelId_t channel;
} Scp03_SSS_SessionState_t;

typedef struct {
    sss_object_t keyEnc; //!< SCP03 static secure channel encryption key obj
    sss_object_t keyMac; //!< SCP03 static secure channel authentication key obj
    sss_object_t keyDek; //!< SCP03 data encryption key obj
    Scp03_SSS_SessionState_t session; //!< SCP03 session state
} Scp_SSS_State_t;
#endif

// AX Product Family Specific APDU Instructions (INS) and parameters (P1/P2)
// *************************************************************************

#ifdef TGT_A70CI
#define A70CI_CLA                               (0x80)

#define A70CI_INS_ACCESS                        (0x01)
#define A70CI_INS_SWITCH                        (0x02)
#define A70CI_INS_SST_GENERATE                  (0x03)
#define A70CI_INS_SST_SET                       (0x04)
#define A70CI_INS_SST_GET                       (0x05)
#define A70CI_INS_SST_ERASE                     (0x06)
#define A70CI_INS_ECB                           (0x07)
#define A70CI_INS_CBC                           (0x08)
#define A70CI_INS_GCM                           (0x09)
#define A70CI_INS_GMAC                          (0x0A)
#define A70CI_INS_RSA                           (0x0B)
#define A70CI_INS_ECC                           (0x0C)
#define A70CI_INS_RND                           (0x0D)
#define A70CI_INS_DEBUG                         (0xFF)

#define INS_AX_DEBUG                            A70CI_INS_DEBUG
#define INS_AX_RND_GET_RANDOM                   A70CI_INS_RND

#define P1_GET_CHALLENGE                        (0x01)
#define P1_FREEZE_CREDENTIAL                    (0x02)

#define P1_CONFIGURE                            (0x01)
#define P1_OPERATE                              (0x02)

#define P1_KEY_PAIR                             (0x01)
#define P1_AES_KEY                              (0x02)
#define P1_PUBLIC_KEY                           (0x03)
#define P1_PRIVATE_KEY                          (0x04)
#define P1_DEVICE_CERTIFICATE                   (0x05)
#define P1_ERASE                                (0x06)

#define P1_NEW                                  (0x01)
#define P1_PROCESS                              (0x02)
#define P1_FINISH                               (0x03)
#define P1_CLOSE                                (0x04)

#define P1_SIGN                                 (0x01)
#define P1_VERIFY                               (0x02)
#define P1_ENCRYPT                              (0x03)
#define P1_DECRYPT                              (0x04)
#define P1_OAEP_ENCRYPT                         (0x07)
#define P1_OAEP_DECRYPT                         (0x08)

#define P1_GENERATE_SHARED_SECRET               (0x03)
#define P1_GENERATE_STATIC_SHARED_SECRET        (0x04)

#define P1_GENERATE_RANDOM                      (0x01)

#define P1_DISABLE_DEBUG                        (0x00)
#define P1_GET_LIFECYCLE_STATE                  (0x01)
#define P1_GET_HIGHWATER_LEVEL                  (0x03)
#define P1_RESET                                (0x05)
#define P1_SELECT_GCM_CIPHER                    (0x09)
#define P1_GET_FREE_MEMORY                      (0x0B)

#define P2_PERSISTENT_MEM                       (0x01)
#define P2_TRANS_MEM_DESELECT                   (0x02)
#define P2_TRANS_MEM_RESET                      (0x03)

#define P2_CIPHER_APPLET                        (0x01)
#define P2_CIPHER_SECURE_BOX                    (0x02)
#define P2_RESET                                (0x00)
#elif defined(TGT_A70CM)
#define A70CM_CLA                     (0x80)

#define A70CM_INS_ACCESS              (0x01)
#define A70CM_INS_SWITCH              (0x02)
#define A70CM_INS_SST_SET_ECC_KEY     (0x03)
#define A70CM_INS_SST_SET_RSA_KEY     (0x04)
#define A70CM_INS_SST_SET_CERTIFICATE (0x05)
#define A70CM_INS_SST_SET_AES_KEY     (0x06)
#define A70CM_INS_SST_SET_STORAGE     (0x07)
#define A70CM_INS_SST_GET_ECC_KEY     (0x08)
#define A70CM_INS_SST_GET_RSA_KEY     (0x09)
#define A70CM_INS_SST_GET_CERTIFICATE (0x0A)
#define A70CM_INS_SST_GET_AES_KEY     (0x0B)
#define A70CM_INS_SST_GET_STORAGE     (0x0C)
#define A70CM_INS_SST_ERASE           (0x0D)
#define A70CM_INS_AES_GENERATE_KEY    (0x0E)
#define A70CM_INS_AES_ECB             (0x0F)
#define A70CM_INS_AES_CBC             (0x10)
#define A70CM_INS_AES_GCM             (0x11)
#define A70CM_INS_AES_GMAC            (0x12)
#define A70CM_INS_AES_WRAP            (0x13)
#define A70CM_INS_RSA_GENERATE_KEY    (0x14)
#define A70CM_INS_RSA_SIGN            (0x15)
#define A70CM_INS_RSA_ENCRYPT         (0x16)
#define A70CM_INS_ECC_GENERATE_KEY    (0x17)
#define A70CM_INS_ECC_SIGN            (0x18)
#define A70CM_INS_ECC_ECDH            (0x19)
#define A70CM_INS_RNG_GENERATE        (0x1A)

#define P1_GET_CHALLENGE               (0x01)
#define P1_AUTHENTICATE                (0x02)

#define P1_CONFIGURE                   (0x01) // Redefined in ERDF
#define P1_OPERATE                     (0x02) // Redefined in ERDF
#define P1_RE_CONFIGURE                (0x03)

#define P1_ECC_KEY                     (0x01)

#define P1_ECC_PUBLIC_KEY              (0x01)
#define P1_ECC_PRIVATE_KEY             (0x02)

#define P1_RSA__KEY                    (0x01)

#define P1_RSA_PUBLIC_KEY              (0x01)
#define P1_RSA_PRIVATE_KEY             (0x02)

#define P1_AES_KEY                     (0x01)
#define P1_AES_WRAPPING_KEY            (0x02)
#define P1_SCP_KEY                     (0x03)

#define P1_DEVICE_CERTIFICATE          (0x01)

#define P1_GP_STORAGE                  (0x01)

#define P1_KEY                         (0x01)
#define P1_WRAPPED_KEY                 (0x02)

#define P1_WRAP                        (0x01)
#define P1_UNWRAP                      (0x02)

#define P1_ERASE                       (0x01)

#define P1_NEW                         (0x01)
#define P1_PROCESS                     (0x02)
#define P1_FINISH                      (0x03)
#define P1_CLOSE                       (0x04)
#define P1_NEW_DLMS                    (0x05)

#define P1_SIGN                        (0x01)
#define P1_VERIFY                      (0x02)
#define P1_SIGN_DATA                   (0x03)
#define P1_VERIFY_DATA                 (0x04)

#define P1_ENCRYPT                     (0x01)
#define P1_DECRYPT                     (0x02)

#define P1_GENERATE_EPHEMERAL_KEYPAIR  (0x01)
#define P1_GENERATE_SHARED_SECRET      (0x02)

#define P1_GENERATE_RANDOM             (0x01)

/* ------------------------------ */

#define A70CM_INS_DEBUG                 0xFF
#define P1_GET_LIFECYCLE_STATE          0x01
#define P1_GET_AUTHENTICATION_STATE     0x02
#define P1_GET_HIGHWATER_LEVEL          0x03
#define P1_REFLECT                      0x04
#define P1_RESET                        0x05
#define P1_SET_AUTHENTICATED            0x06
#define P1_GET_PRIVATE_KEY              0x07
#define P1_GET_CONFIGURATION            0x08
#define P1_SELECT_GCM_CIPHER            0x09
#define P1_SELECT_CBC_CIPHER            0x0A
// #define P1_MEMORY                       0x0B
#define P1_GET_FREE_MEMORY             (0x0B)
#define P1_BOUNCE                       0x0C

#define P2_CIPHER_APPLET                0x01
#define P2_CIPHER_SECURE_BOX            0x02
#define P2_RESET                       (0x00)
#define P2_PERSISTENT_MEM              (0x01)
#define P2_TRANS_MEM_DESELECT          (0x02)

// TODO: Remove hack
#define P1_DISABLE_DEBUG               (0x00)
#define INS_AX_DEBUG                   A70CM_INS_DEBUG

#elif (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH || SSS_HAVE_SE05X)
#define A71CH_CLA                     (0x80)

/// @cond
/*
KEY_PAIR      0x10  ECC NIST P-256 key pairs (Empty)
PUBLIC_KEY    0x20  ECC NIST P-256 public keys in Uncompressed format (Empty)
SYM_KEY       0x40  e.g. AES128 keys (Empty)
GP_DATA       0x60  General Purpose data; size = 1 KB (Filled with zeroes)
COUNTER       0x70  Counter with rollback protection; size = 4 bytes (Filled with zeroes)
MODULE        0x90  TRNG; hash; chip info (N.A.)
*/

#define A71CH_SSI_KEY_PAIR      0x10  //!< Key Pair Secure Storage Class ID
#define A71CH_SSI_PUBLIC_KEY    0x20  //!< Public Key Secure Storage Class ID
#define A71CH_SSI_CONFIG_KEY    0x30  //!< Config Key Secure Storage Class ID
#define A71CH_SSI_SYM_KEY       0x40  //!< Symmetric Key Secure Storage Class ID
#define A71CH_SSI_COUNTER       0x60
#define A71CH_SSI_GP_DATA       0x70
#define A71CH_SSI_MODULE        0x90
#define SE05X_SSI_SEC           0xA0

#define A71CH_ACTION_GET    0x01   //!< Get the value of the A71 object.
#define A71CH_ACTION_SET    0x02   //!< Set the value of the A71 object.
#define A71CH_ACTION_FREEZE 0x03   //!< Freeze the A71 object: the object cannot be modified (changed or erased) any longer.
#define A71CH_ACTION_ERASE  0x04   //!< Clear the A71 object, reset to Empty state.

#define A71CH_ACTION_SIGN_HASH     0x05
#define A71CH_ACTION_VERIFY_SIG    0x05
#define A71CH_ACTION_KDF           0x05
#define A71CH_ACTION_GET_RANDOM    0x05
#define A71CH_ACTION_GET_SHA256    0x06
#define A71CH_ACTION_MS_PSK        0x06
#define A71CH_ACTION_MS_ECDH_PSK   0x06
#define A71CH_ACTION_SHARED_SECRET 0x07
#define A71CH_ACTION_HMAC_SHA256   0x07
#define A71CH_ACTION_INJECT_LOCK   0x07

#define A71CH_INS_GET_ECC_KEYPAIR           (A71CH_SSI_KEY_PAIR | A71CH_ACTION_GET)
#define A71CH_INS_SET_ECC_KEYPAIR           (A71CH_SSI_KEY_PAIR | A71CH_ACTION_SET)
#define A71CH_INS_FREEZE_ECC_KEYPAIR        (A71CH_SSI_KEY_PAIR | A71CH_ACTION_FREEZE)
#define A71CH_INS_ERASE_ECC_KEYPAIR         (A71CH_SSI_KEY_PAIR | A71CH_ACTION_ERASE)
#define A71CH_INS_SIGN_HASH_ECC_KEYPAIR     (A71CH_SSI_KEY_PAIR | A71CH_ACTION_SIGN_HASH)
#define A71CH_INS_MS_ECDH_PSK_ECC_KEYPAIR   (A71CH_SSI_KEY_PAIR | A71CH_ACTION_MS_ECDH_PSK)
#define A71CH_INS_SHARED_SECRET_ECC_KEYPAIR (A71CH_SSI_KEY_PAIR | A71CH_ACTION_SHARED_SECRET)

#define A71CH_INS_GET_ECC_PUBLIC_KEY         (A71CH_SSI_PUBLIC_KEY | A71CH_ACTION_GET)
#define A71CH_INS_SET_ECC_PUBLIC_KEY         (A71CH_SSI_PUBLIC_KEY | A71CH_ACTION_SET)
#define A71CH_INS_FREEZE_ECC_PUBLIC_KEY      (A71CH_SSI_PUBLIC_KEY | A71CH_ACTION_FREEZE)
#define A71CH_INS_ERASE_ECC_PUBLIC_KEY       (A71CH_SSI_PUBLIC_KEY | A71CH_ACTION_ERASE)
#define A71CH_INS_VERIFY_SIG_ECC_PUBLIC_KEY  (A71CH_SSI_PUBLIC_KEY | A71CH_ACTION_VERIFY_SIG)

#define A71CH_INS_SET_CONFIG_KEY         (A71CH_SSI_CONFIG_KEY | A71CH_ACTION_SET)

#define A71CH_INS_GET_SYM_KEY            (A71CH_SSI_SYM_KEY | A71CH_ACTION_GET)
#define A71CH_INS_SET_SYM_KEY            (A71CH_SSI_SYM_KEY | A71CH_ACTION_SET)
#define A71CH_INS_FREEZE_SYM_KEY         (A71CH_SSI_SYM_KEY | A71CH_ACTION_FREEZE)
#define A71CH_INS_ERASE_SYM_KEY          (A71CH_SSI_SYM_KEY | A71CH_ACTION_ERASE)
#define A71CH_INS_KDF_SYM_KEY            (A71CH_SSI_SYM_KEY | A71CH_ACTION_KDF)
#define A71CH_INS_MS_PSK_SYM_KEY         (A71CH_SSI_SYM_KEY | A71CH_ACTION_MS_PSK)
#define A71CH_INS_HMAC_SHA256_SYM_KEY    (A71CH_SSI_SYM_KEY | A71CH_ACTION_HMAC_SHA256)

#define A71CH_INS_GET_COUNTERS          (A71CH_SSI_COUNTER | A71CH_ACTION_GET)
#define A71CH_INS_SET_COUNTERS          (A71CH_SSI_COUNTER | A71CH_ACTION_SET)
#define A71CH_INS_FREEZE_COUNTERS       (A71CH_SSI_COUNTER | A71CH_ACTION_FREEZE)
#define A71CH_INS_ERASE_COUNTERS        (A71CH_SSI_COUNTER | A71CH_ACTION_ERASE)

#define A71CH_INS_GET_GP_DATA          (A71CH_SSI_GP_DATA | A71CH_ACTION_GET)
#define A71CH_INS_SET_GP_DATA          (A71CH_SSI_GP_DATA | A71CH_ACTION_SET)
#define A71CH_INS_FREEZE_GP_DATA       (A71CH_SSI_GP_DATA | A71CH_ACTION_FREEZE)

#define A71CH_INS_GET_MODULE            (A71CH_SSI_MODULE | A71CH_ACTION_GET)
#define A71CH_INS_SET_MODULE            (A71CH_SSI_MODULE | A71CH_ACTION_SET)
#define A71CH_INS_FREEZE_MODULE         (A71CH_SSI_MODULE | A71CH_ACTION_FREEZE)
#define A71CH_INS_ERASE_MODULE          (A71CH_SSI_MODULE | A71CH_ACTION_ERASE)
#define A71CH_INS_MODULE_GET_RANDOM     (A71CH_SSI_MODULE | A71CH_ACTION_GET_RANDOM)
#define A71CH_INS_MODULE_GET_SHA256     (A71CH_SSI_MODULE | A71CH_ACTION_GET_SHA256)

#define SE05X_INS_SEC_SET_PIN            (SE05X_SSI_SEC | A71CH_ACTION_SET)
#define SE05X_INS_SEC_AUTH               (SE05X_SSI_SEC | A71CH_ACTION_GET)
#define SE05X_INS_SEC_CLOSE_SESSION      (SE05X_SSI_SEC | A71CH_ACTION_ERASE)
// #define A71CH_INS_MODULE_INJECT_LOCK    (A71CH_SSI_MODULE | A71CH_ACTION_INJECT_LOCK) // Removed in APDU Spec 1.0.2

#define A71CH_INS_REFLECT             (0xFE)

// #define INS_AX_DEBUG                    A71CH_INS_ERASE_MODULE

#define P1_GET_LIFECYCLE_STATE          0x01
#define P1_GET_AUTHENTICATION_STATE     0x02
#define P1_GET_HIGHWATER_LEVEL          0x03
#define P1_REFLECT                      0x04
#define P1_RESET                        0x00
#define P1_SET_AUTHENTICATED            0x06
#define P1_GET_PRIVATE_KEY              0x07
#define P1_GET_CONFIGURATION            0x08
// #define P1_MEMORY                       0x0B
#define P1_BOUNCE                       0x0C
#define P1_EEPROM                       0x00
#define P1_TRANSIENT_MEMORY             0x01

#define P1_GENERATE_RANDOM             (0x01)

#define P1_RANDOM_PLAIN         (0x00)
#define P1_RANDOM_CLIENT_HELLO  (0x01)

#define P1_MODULE_UNLOCK_CHALLENGE   (0x00)
#define P1_KEYPAIR_CHALLENGE         (0x01)
#define P1_PUBLIC_KEY_CHALLENGE      (0x02)

#define P2_SMF_LOCK_PLAIN_INJECTION  (0x01)
#define P2_SMF_LOCK_MODULE           (0x02)
#define P2_SMF_UNLOCK_MODULE         (0x03)
#define P2_SMF_LABEL                 (0x05)

#define P2_HMAC_ONE_SHOT     (0x00)
#define P2_HMAC_INIT         (0x01)
#define P2_HMAC_UPDATE       (0x02)
#define P2_HMAC_FINAL        (0x03)

#define P2_SHA256_ONE_SHOT   (0x00)
#define P2_SHA256_INIT       (0x01)
#define P2_SHA256_UPDATE     (0x02)
#define P2_SHA256_FINAL      (0x03)

#define P2_CONFIGURE_INITIAL (0x21)
#define P2_CONFIGURE         (0x22)
#define P2_OPERATE           (0x31)
#define P2_DEAD              (0x61)

#define P2_UNIQUE_ID         (0x01)
#define P2_FREE_MEMORY       (0x02)
#define P2_APPLET_INFO       (0x03)
#define P2_CREDENTIAL_MAP    (0x04)
#define P2_CHALLENGE         (0x05)
#define P2_RESTRICTED_INFO   (0x06)

#define P2_NOT_WRAPPED       (0x01)
#define P2_RFC3394_WRAPPED   (0x02)

#define P2_PUBKEY_PRESENT    (0x01)
#define P2_PUBKEY_ABSENT     (0x02)

#define P2_KEY_PAIR_USAGE_COUNTER (0x01)

#define P2_RESET             (0xEA)
#define P2_DISABLE_DEBUG     (0xDD)

#define P1_DISABLE_DEBUG                        (0x00)
#define P1_GET_FREE_MEMORY                      (0x0B)
/// @endcond

#else
// SWITCH_xxx commands
#define INS_AX_SWITCH              (0x01)
#define P1_UNLOCK                    (0x01)
#define P1_CONFIGURE                 (0x02)
#define P1_OPERATE                   (0x03)
#define P1_DEACTIVATE                (0x04)
#define P1_DIAGNOSE                  (0x05)
#define P1_DEAD                      (0x06)
#define P1_GETSTATE                  (0x07)

#define INS_AX_ECC_GENERATE_KEY    (0x10)
#define INS_AX_RND_GET_RANDOM      (0x15)

// ** Secure Storage commands **
// SST_Set_xxx commands
#define INS_AX_SST_SET_ECC           (0x02)
#define P1_ECC_PUBLIC_KEY            (0x01)
#define P1_ECC_PROTECTED_PUBLIC_KEY  (0x02)

#define INS_AX_SST_SET_CERTIFICATE (0x03)
#define P1_CERTIFICATE               (0x01)

#define INS_AX_SST_SET_AES_KEY     (0x04)
#define P1_AES_KEY                   (0x01)
#define P1_WRAPPED_AES_KEY           (0x02)

#define INS_AX_SST_SET_STORAGE     (0x05)
#define P1_GP_STORAGE                (0x01)

// SST_Get_xxx commands
#define INS_AX_SST_GET_ECC         (0x06)
#define INS_AX_SST_GET_CERTIFICATE (0x07)
#define INS_AX_SST_GET_AES_KEY     (0x08)
#define INS_AX_SST_GET_STORAGE     (0x09)

#define INS_AX_SST_GET_IDENTIFIER  (0x0A)
#define P1_UNIQUE_IDENTIFIER         (0x01)
#define P1_CPLC_DATA                 (0x02)

#define INS_AX_SST_SET_COUNTER     (0x0D)
#define P1_SET_COUNTER               (0x01)
#define P1_INCREMENT_COUNTER         (0x02)
#define P1_SET_PROTECTED_COUNTER     (0x03)

#define INS_AX_SST_GET_COUNTER     (0x0E)
#define P1_GET_COUNTER               (0x01)

// SST_Erase_xxx commands
#define INS_AX_SST_ERASE           (0x0B)
#define P1_ERASE_KEY                 (0x01)
#define P1_ERASE_CERTIFICATE         (0x02)

// SST_Copy_xxx commands
#define INS_AX_SST_COPY            (0x0C)
#define P1_COPY_KEY                 (0x01)

// ** Crypto API **
// ECC commands
#define INS_AX_ECC_GENERATE        (0x10)
#define P1_ECC_KEY                   (0x01)

#define INS_AX_ECC_SIGN            (0x11)
#define P1_ECDSA_SIGN                (0x01)
#define P1_VERIFY                    (0x02)
#define P1_SIGN_PARTIAL_HASH         (0x03)
#define P1_VERIFY_WITH_KEY           (0x04)

#define INS_AX_ECC_ECDH            (0x12)
#define P1_GENERATE_SHARED_SECRET          (0x01)
#define P1_GENERATE_SHARED_SECRET_TO_STORE (0x02)
#define P1_DERIVE_KEY_DATA                 (0x03)

// AES commands
#define INS_AX_AES_GENERATE_KEY    (0x13)
#define INS_AX_AES_WRAP            (0x14)
#define P1_WRAP                      (0x01)
#define P1_UNWRAP                    (0x02)

// RNG commands
#define INS_AX_RNG_GENERATE        (0x15)
#define P1_GENERATE_RANDOM           (0x01)

// SEQUENCE counter command
#define INS_AX_SCP                 (0x16)
#define P1_GET_SEQ_COUNTER           (0x02)

// Soft reset command
#define INS_AX_SM                  (0x17)
#define P1_SOFT_RESET                (0x01)

// DEBUG commands
#define INS_AX_DEBUG               (0xFF)
#define P1_DISABLE_DEBUG             (0x00)
#define P1_GET_LIFECYCLE_STATE       (0x01)
#define P1_GET_AUTHENTICATION_STATE  (0x02)
#define P1_GET_HIGHWATER_LEVEL       (0x03)
#define P1_REFLECT                   (0x04)
#define P1_RESET                     (0x05)
#define P1_SET_USER                  (0x06)
#define P1_SWITCH                    (0x07)
// #define P1_GET_CONFIGURATION         (0x08)
// #define P1_SELECT_GCM_CIPHER (0x09)
// #define P1_SELECT_CBC_CIPHER (0x0A)
#define P1_GET_FREE_MEMORY           (0x0B)
#define P1_SET_SCP_CARD_CHALLENGE    (0x0C)
#define P1_SET_DISABLE_MAC_CHAINING  (0x0D)
#define P1_SET_DISABLE_DEBUG_MODE    (0x0E)
#define P1_SET_KEY_DEK               (0x0F)
#define P1_GET_MASTER_KEYS           (0x10)
#define P1_GET_CURRENT_SEC_LEVEL     (0x11)

// TEST Commands
#define INS_TEST                     (0xFE)
#define P1_REFLECT                   (0x04)
#define P1_REFLECT_SW                (0x14)

#define P2_RESET                     (0x00)
#endif

#ifdef __cplusplus
}
#endif
#endif //_AX_COMMON_A71CH_
