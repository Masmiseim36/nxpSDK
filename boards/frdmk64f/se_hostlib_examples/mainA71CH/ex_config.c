/**
 * @file ex_config.c
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
 * Example invocation of configure key specific functionality of the A71CH
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "a71ch_ex.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "ax_util.h"
#include "tst_sm_util.h"
#include "tst_a71ch_util.h"
#include "axHostCrypto.h"
#include "tstHostCrypto.h"
#include "HostCryptoAPI.h"
#include "nxLog_hostLib.h"
static U8 exSetConfigKey(U8 initMode);
static U8 exSstPubEnc(U8 initMode);
static U8 exSstPubEraseAfterInjectLock(void);
static U8 exSstKeyPairEnc(U8 initMode);

static U8 configKeyModuleLock[16];  //!< Configuration Key for Module, acting as transport key (index ::A71CH_CFG_KEY_IDX_MODULE_LOCK)
static U8 configKeyPrivateKey[16];  //!< Configuration Key for Private Key encryption (index ::A71CH_CFG_KEY_IDX_PRIVATE_KEYS)
static U8 configKeyPublicKey[16];   //!< Configuration Key for Public Key encryption (index ::A71CH_CFG_KEY_IDX_PUBLIC_KEYS)

#define EX_CONFIG_DEMO_LOCK_UNLOCK
#define EX_CONFIG_ALSO_USE_SCP03

/**
 * Demonstrate storage and usage of config keys:
 * - ::exSetConfigKey
 * - ::exSstPubEnc
 * - ::exSstKeyPairEnc
 * - ::exSstPubEraseAfterInjectLock
 *
 */
U8 exConfig()
{
    U8 result = 1;
    int i = 0;
    LOG_I( "-----------Start exConfig()------------");

    DEV_ClearChannelState();

    // Choose a (unique) pattern for each config key
    for (i=0; i<16; i++) {
        configKeyModuleLock[i] = (U8)(i ^ A71CH_CFG_KEY_IDX_MODULE_LOCK);
        configKeyPrivateKey[i] = (U8)(i ^ A71CH_CFG_KEY_IDX_PRIVATE_KEYS);
        configKeyPublicKey[i] = (U8)(i ^ A71CH_CFG_KEY_IDX_PUBLIC_KEYS);
    }

    // No channel encryption
    // ---------------------
    // - First set the config keys
    result &= exSetConfigKey(INIT_MODE_RESET);

    // - Set wrapped Public keys
    result &= exSstPubEnc(INIT_MODE_NO_RESET);
    // - Set wrapped key pairs (only private key is wrapped)
    result &= exSstKeyPairEnc(INIT_MODE_NO_RESET);
    // - Check on erasing Public keys after Module Inject Lock has been set
    result &= exSstPubEraseAfterInjectLock();

#ifdef EX_CONFIG_ALSO_USE_SCP03
    // Using channel encryption
    // ------------------------
    // - First set the config keysSym keys
    result &= exSetConfigKey(INIT_MODE_RESET_DO_SCP03);

    // - Set wrapped Public keys
    result &= exSstPubEnc(INIT_MODE_NO_RESET);

    // - Set wrapped key pairs (only private key is wrapped)
    result &= exSstKeyPairEnc(INIT_MODE_NO_RESET);

    // - Check on erasing Public keys after Module Inject Lock has been set
    result &= exSstPubEraseAfterInjectLock();
#endif

    // overall result
    LOG_I( "-----------End exConfig(), result = %s------------", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate
 * - setting config keys
 * - use of transport key (config key at index ::A71CH_CFG_KEY_IDX_MODULE_LOCK) to lock/unlock A71CH
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exSetConfigKey(U8 initMode)
{
    U8 result = 1;
    U16 err;

    U8 uid[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
    U16 uidLen = sizeof(uid);

    U8 challenge[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};
    U16 challengeLen = sizeof(challenge);

    U8 unlockCode[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};
    S32 hcRet = 0;

    U8 indexCfgKey = A71CH_CFG_KEY_IDX_MODULE_LOCK;

    LOG_I("-----------Start exSetConfigKey(%s)------------", getInitModeAsString(initMode));
    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);

    // Store all configure keys (value is stored in variables with module scope)
    indexCfgKey = A71CH_CFG_KEY_IDX_MODULE_LOCK;
    LOG_I("A71_SetConfigKey(0x%02x)", indexCfgKey);
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, configKeyModuleLock, sizeof(configKeyModuleLock));
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyModuleLock", configKeyModuleLock, sizeof(configKeyModuleLock), AX_COLON_32);

    indexCfgKey = A71CH_CFG_KEY_IDX_PRIVATE_KEYS;
    LOG_I("A71_SetConfigKey(0x%02x)", indexCfgKey);
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, configKeyPrivateKey, sizeof(configKeyPrivateKey));
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyPrivateKey", configKeyPrivateKey, sizeof(configKeyPrivateKey), AX_COLON_32);

    indexCfgKey = A71CH_CFG_KEY_IDX_PUBLIC_KEYS;
    LOG_I("A71_SetConfigKey(0x%02x)", indexCfgKey);
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, configKeyPublicKey, sizeof(configKeyPublicKey));
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyPublicKey", configKeyPublicKey, sizeof(configKeyPublicKey), AX_COLON_32);

#ifdef EX_CONFIG_DEMO_LOCK_UNLOCK
    // Lock device for transport
    LOG_I("A71_LockModule()");
    err = A71_LockModule();
    result &= AX_CHECK_SW(err, SW_OK, "err");

    LOG_I("A71_GetUniqueID().");
    err = A71_GetUniqueID(uid, &uidLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("uid", uid, uidLen, AX_COLON_32);

    LOG_I("A71_GetUnlockChallenge().");
    err = A71_GetUnlockChallenge(challenge, &challengeLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("challenge", challenge, challengeLen, AX_COLON_32);

    // Decrypt challenge
    hcRet = HOST_AES_ECB_DECRYPT(unlockCode, challenge, configKeyModuleLock, sizeof(configKeyModuleLock));
    if (hcRet != HOST_CRYPTO_OK)
    {
    	LOG_E("HOST_AES_ECB_DECRYPT: failed.");
        result = 0;
    }

    LOG_I("A71_UnlockModule()");
    err = A71_UnlockModule(unlockCode, 16);
    result &= AX_CHECK_SW(err, SW_OK, "err");
#endif

    LOG_I( "-----------End exSetConfigKey(%s), result = %s------------", getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate
 * - setting a wrapped public key
 * - demonstrate key can be used through verify operation
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exSstPubEnc(U8 initMode)
{
    U8 result = 1;
    U16 err;
    int i;

    ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    EC_KEY *eccKeyCA[A71CH_PUBLIC_KEY_MAX] = {0};
    eccKeyComponents_t eccKcCA[A71CH_PUBLIC_KEY_MAX] = {0};

    U8 fetchedPubKey[65];
    U16 fetchedPubKeyLen = sizeof(fetchedPubKey);

    // const U16 expectedPubKeyLen = 65;
    const U16 expectedPrivKeyLen = 32;

    LOG_I("-----------Start exSstPubEnc(%s)------------", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);

    // Start by creating, inserting and checking keys
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        U8 wrappedKey[64+8];
        U16 wrappedKeyLen = sizeof(wrappedKey);
        eccKeyCA[i] = NULL;
        err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyCA[i]);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        err = HOSTCRYPTO_GetPublicKey(eccKeyCA[i], eccKcCA[i].pub, &(eccKcCA[i].pubLen), (64 << 1)+1);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        err = HOSTCRYPTO_GetPrivateKey(eccKeyCA[i], eccKcCA[i].priv, &(eccKcCA[i].privLen), 64);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        eccKcCA[i].bits = expectedPrivKeyLen << 3;
        eccKcCA[i].curve = eccCurve;

        // Wrap public key (strip first character before wrapping) on host
        LOG_I("HOSTCRYPTO_AesWrapKeyRFC3394(0x%02X)", (SST_Index_t)i);
        err = HOSTCRYPTO_AesWrapKeyRFC3394(configKeyPublicKey, sizeof(configKeyPublicKey),
            wrappedKey, &wrappedKeyLen, &(eccKcCA[i].pub[1]), eccKcCA[i].pubLen - 1);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("wrappedKey", wrappedKey, wrappedKeyLen, AX_COLON_32);

        LOG_I( "A71_SetEccPublicKey(0x%02X)", (SST_Index_t)i);
        err = A71_SetEccPublicKey ((SST_Index_t) i, wrappedKey, wrappedKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Read out and verify public key
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
    	LOG_I( "A71_GetEccPublicKey(0x%02X)", (SST_Index_t)i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetEccPublicKey ((SST_Index_t) i, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Compare with reference value
        result &= AX_COMPARE_BYTE_ARRAY("eccKcCA[i].pub", eccKcCA[i].pub, eccKcCA[i].pubLen,
            "fetchedPubKey", fetchedPubKey, fetchedPubKeyLen, AX_COLON_32);
    }

    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        HOSTCRYPTO_FreeEccKey(&eccKeyCA[i]);
    }

    LOG_I( "-----------End exSstPubEnc(%s), result = %s------------", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate
 * - Once the module Inject Lock has been set, its only possible to erase a public key
 *   through a challenge response protocol
 *
 */
static U8 exSstPubEraseAfterInjectLock()
{
    U8 result = 1;
    U16 err;
    int i;

    // ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    // EC_KEY *eccKeyCA[A71CH_PUBLIC_KEY_MAX] = {0};
    // eccKeyComponents_t eccKcCA[A71CH_PUBLIC_KEY_MAX] = {0};

    U8 fetchedPubKey[65];
    U16 fetchedPubKeyLen = sizeof(fetchedPubKey);

    // const U16 expectedPubKeyLen = 65;
    // const U16 expectedPrivKeyLen = 32;

    LOG_I("-----------Start exSstPubEraseAfterInjectLock()------------");

    // Check whether ErasePublicKey - without challenge/response - fails
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
    	LOG_I( "A71_EraseEccPublicKey(0x%02X)", (SST_Index_t)i);
        err = A71_EraseEccPublicKey((SST_Index_t) i);
        result &= AX_CHECK_SW(err, SW_WRONG_DATA, "err");
    }

    // Confirm reading out public key is still possible
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
    	LOG_I("A71_GetEccPublicKey (index=0x%02X)", i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetEccPublicKey ((SST_Index_t)i, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve Public Ecc Key");
    }

    // Do a ErasePublicKey with challenge response
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
    	LOG_I( "A71_EraseEccPublicKeyWithChallenge(0x%02X)", (SST_Index_t)i);
        err = A71_EraseEccPublicKeyWithChallenge((SST_Index_t) i, configKeyPublicKey, sizeof(configKeyPublicKey));
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check whether reading out (erased) public key now fails.
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
    	LOG_I("A71_GetEccPublicKey (index=0x%02X)", i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetEccPublicKey ((SST_Index_t)i, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");
    }

    LOG_I( "-----------End exSstPubEraseAfterInjectLock(), result = %s------------",
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate
 * - setting (encrypted)/getting/erasing of key pairs
 * - demonstrate key can be used through sign operations
 * - demonstrate a plain key pair can not be inserted once 'inject lock' has been enabled
 * - demonstrate a keywrapped key pair can still be inserted
 * - demonstrate generating a key pair is conditional on a correct challenge/response
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 * @param[in] appletVersion See #SmCommState_t::appletVersion
 */
static U8 exSstKeyPairEnc(U8 initMode)
{
    U8 result = 1;
    U16 err;
    int i;
    HLSE_RET_CODE retcode;

    ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    EC_KEY *eccKeyTls[A71CH_KEY_PAIR_MAX] = {0};
    eccKeyComponents_t eccKcTls[A71CH_KEY_PAIR_MAX] = {0};

    EC_KEY *eccKeyAlt = NULL;
    eccKeyComponents_t eccKcAlt;

    U8 fetchedPubKey[65];
    U16 fetchedPubKeyLen = sizeof(fetchedPubKey);

    U8 hashSha256[32];
    U16 hashSha256Len = sizeof(hashSha256);

    U8 signature[128];
    U16 signatureLen = sizeof(signature);
    HLSE_MECHANISM_INFO mechInfo;

    // const U16 expectedPubKeyLen = 65;
    const U16 expectedPrivKeyLen = 32;

    U8 wrappedPrivKey[32+8];
    U16 wrappedPrivKeyLen = sizeof(wrappedPrivKey);

    SST_Index_t kpIndex;

    LOG_I("-----------Start exSstKeyPairEnc(%s)------------", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);

    // Start by creating, inserting and checking keys
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        eccKeyTls[i] = NULL;
        err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyTls[i]);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        err = HOSTCRYPTO_GetPublicKey(eccKeyTls[i], eccKcTls[i].pub, &(eccKcTls[i].pubLen), (64 << 1)+1);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        err = HOSTCRYPTO_GetPrivateKey(eccKeyTls[i], eccKcTls[i].priv, &(eccKcTls[i].privLen), 64);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        eccKcTls[i].bits = expectedPrivKeyLen << 3;
        eccKcTls[i].curve = eccCurve;

        // Wrap private key on host
        wrappedPrivKeyLen = sizeof(wrappedPrivKey);
        LOG_I("HOSTCRYPTO_AesWrapKeyRFC3394(0x%02X)", (SST_Index_t)i);
        err = HOSTCRYPTO_AesWrapKeyRFC3394(configKeyPrivateKey, sizeof(configKeyPrivateKey),
            wrappedPrivKey, &wrappedPrivKeyLen, eccKcTls[i].priv, eccKcTls[i].privLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("wrappedPrivKey", wrappedPrivKey, wrappedPrivKeyLen, AX_COLON_32);

        LOG_I( "A71_SetEccKeyPair(0x%02X)", (SST_Index_t)i);
        err = A71_SetEccKeyPair((SST_Index_t) i, eccKcTls[i].pub, eccKcTls[i].pubLen,
            wrappedPrivKey, wrappedPrivKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Read out and verify public key
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
    	LOG_I( "A71_GetPublicKeyEccKeyPair (0x%02X)", (SST_Index_t)i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetPublicKeyEccKeyPair  ((SST_Index_t) i, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Compare with reference value
        result &= AX_COMPARE_BYTE_ARRAY("eccKcTls[i].pub", eccKcTls[i].pub, eccKcTls[i].pubLen,
            "fetchedPubKey", fetchedPubKey, fetchedPubKeyLen, AX_COLON_32);
    }

    // Create alternative key
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyAlt);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    err = HOSTCRYPTO_GetPublicKey(eccKeyAlt, eccKcAlt.pub, &(eccKcAlt.pubLen), (64 << 1)+1);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    err = HOSTCRYPTO_GetPrivateKey(eccKeyAlt, eccKcAlt.priv, &(eccKcAlt.privLen), 64);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    eccKcAlt.bits = expectedPrivKeyLen << 3;
    eccKcAlt.curve = eccCurve;
    HOSTCRYPTO_FreeEccKey(&eccKeyAlt);

    // Fill up the hash with some reference data
    hashSha256Len = sizeof(hashSha256);
    for (i=0; i<hashSha256Len; i++) { hashSha256[i] = (U8)i; }

    // Sign a hash of correct length on the A71CH and do the verification on the Host
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        signatureLen = sizeof(signature);
        LOG_I("A71_EccSign(0x%02X)", (SST_Index_t)i);
        err = A71_EccSign((SST_Index_t)i, hashSha256, hashSha256Len, signature, &signatureLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Verify ... on opposite platform
        LOG_I("(Host)ECDSA_verify API with eccKeyTls[i] (verify signature created on A71CH).");
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_VERIFY;
        retcode = HLCRYPT_Verify(&mechInfo,(U8 *)eccKeyTls[i],0,hashSha256,hashSha256Len,signature,signatureLen);
        if (retcode == HLSE_SW_OK)
        {
        	LOG_I("Verification OK for eccKeyTls[i].");
        }
        else
        {
        	LOG_E("Return value: %d, Verification Not OK for eccKeyTls[i]. Test Failed!", retcode);
            result &= 0;
            break;
        }
    }

    // Erase the keypair at index 0 & verify the value is no longer readable
    // ** Erase **
    kpIndex = A71CH_KEY_PAIR_0;
    LOG_I("A71_EraseEccKeyPair(index=0x%02X)", kpIndex);
    err = A71_EraseEccKeyPair(kpIndex);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // ** Check whether erase was effective **
    LOG_I("A71_GetPublicKeyEccKeyPair (index=0x%02X)", kpIndex);
    fetchedPubKeyLen = sizeof(fetchedPubKey);
    err = A71_GetPublicKeyEccKeyPair ((SST_Index_t)kpIndex, fetchedPubKey, &fetchedPubKeyLen);
    result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");

    // Now disable the plain insertion/reading out of Symmetric keys & Keypairs
    LOG_I("A71_InjectLock()");
    err = A71_InjectLock();
    result &= AX_CHECK_SW(err, SW_OK, "err");
    assert(result);

    // Check whether the plain inject has been disabled (check only first key pair)
    LOG_I( "A71_SetEccKeyPair(0x%02x)", kpIndex);
    err = A71_SetEccKeyPair((SST_Index_t)kpIndex, eccKcAlt.pub, eccKcAlt.pubLen, eccKcAlt.priv, eccKcAlt.privLen);
    result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "Expected to fail, frozen credential cannot be overwritten");

    // Put back the original (reference) value in the first slot, now first wrap private key on host
    wrappedPrivKeyLen = sizeof(wrappedPrivKey);
    LOG_I("HOSTCRYPTO_AesWrapKeyRFC3394(0x%02X)", (SST_Index_t)kpIndex);
    err = HOSTCRYPTO_AesWrapKeyRFC3394(configKeyPrivateKey, sizeof(configKeyPrivateKey),
        wrappedPrivKey, &wrappedPrivKeyLen, eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("wrappedPrivKey", wrappedPrivKey, wrappedPrivKeyLen, AX_COLON_32);

    LOG_I( "A71_SetEccKeyPair(0x%02X)", (SST_Index_t)kpIndex);
    err = A71_SetEccKeyPair((SST_Index_t) kpIndex, eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
        wrappedPrivKey, wrappedPrivKeyLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Check all keys are still fine by doing a sign/verify operation
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        signatureLen = sizeof(signature);
        LOG_I("A71_EccSign(0x%02X)", (SST_Index_t)i);
        err = A71_EccSign((SST_Index_t)i, hashSha256, hashSha256Len, signature, &signatureLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Verify ... on opposite platform
        LOG_I("(Host)ECDSA_verify API with eccKeyTls[i] (verify signature created on A71CH).");
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_VERIFY;
        retcode = HLCRYPT_Verify(&mechInfo,(U8 *)eccKeyTls[i],0,hashSha256,hashSha256Len,signature,signatureLen);
        if (retcode == HLSE_SW_OK)
        {
            LOG_I("Verification OK for eccKeyTls[i].");
        }
        else
        {
            LOG_E("Return value: %d, Verification Not OK for eccKeyTls[i]. Test Failed!", retcode);
            result &= 0;
            break;
        }
    }

    // Check whether the plain inject has been disabled (for all key pair)
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        LOG_I( "A71_SetEccKeyPair(0x%02X)", (SST_Index_t)kpIndex);
        err = A71_SetEccKeyPair((SST_Index_t) kpIndex, eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
            eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "err");
    }

    // Check whether GenerateKeyPair - without challenge/response - fails
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        LOG_I( "A71_GenerateEccKeyPair(0x%02X)", (SST_Index_t)kpIndex);
        err = A71_GenerateEccKeyPair((SST_Index_t) kpIndex);
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "err");
    }

    // Do a GenerateKeyPair with challenge response
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        LOG_I( "A71_GenerateEccKeyPairWithChallenge(0x%02X)", (SST_Index_t)kpIndex);
        err = A71_GenerateEccKeyPairWithChallenge((SST_Index_t) kpIndex, configKeyPrivateKey, sizeof(configKeyPrivateKey));
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    //!< \todo Extend example program to check whether there's a new keypair inside A71CH

    // Check whether EraseKeyPair - without challenge/response - fails
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        LOG_I( "A71_EraseEccKeyPair(0x%02X)", (SST_Index_t)kpIndex);
        err = A71_EraseEccKeyPair((SST_Index_t) kpIndex);
        result &= AX_CHECK_SW(err, SW_WRONG_DATA, "err");
    }

    // Do a EraseKeyPair with challenge response
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        LOG_I( "A71_EraseEccKeyPairWithChallenge(0x%02X)", (SST_Index_t)kpIndex);
        err = A71_EraseEccKeyPairWithChallenge((SST_Index_t) kpIndex, configKeyPrivateKey, sizeof(configKeyPrivateKey));
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check whether reading out public key from erased keypairs now fails.
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        LOG_I("A71_GetPublicKeyEccKeyPair (index=0x%02X)", kpIndex);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetPublicKeyEccKeyPair ((SST_Index_t)kpIndex, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");
    }

    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        HOSTCRYPTO_FreeEccKey(&eccKeyTls[i]);
    }

    LOG_I( "-----------End exSstKeyPairEnc(%s), result = %s------------", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    return result;
}
