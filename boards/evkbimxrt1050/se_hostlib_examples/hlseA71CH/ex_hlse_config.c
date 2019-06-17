/**
 * @file ex_hlse_config.c
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

#include "a71ch_ex_hlse.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "ax_util.h"
#include "tst_sm_util.h"
#include "tst_a71ch_util.h"
#include "axHostCrypto.h"
#include "tstHostCrypto.h"

#include "tst_hlse_a71ch_util.h"

#include "HLSEAPI.h"
#include "HostCryptoAPI.h"

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
U8 exHlseConfig()
{
    U8 result = 1;
    int i = 0;
    PRINTF( "\r\n-----------\r\nStart exConfig()\r\n------------\r\n");

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
    PRINTF( "\r\n-----------\r\nEnd exConfig(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

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
    HLSE_OBJECT_HANDLE moduleHandle = 0;
    U16 moduleHandleNum = 1;

    PRINTF("\r\n-----------\r\nStart exSetConfigKey(%s)\r\n------------\r\n", getInitModeAsString(initMode));
    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);

    // Store all configure keys (value is stored in variables with module scope)
    indexCfgKey = A71CH_CFG_KEY_IDX_MODULE_LOCK;
    PRINTF("\r\nA71_SetConfigKey(0x%02x)\r\n", indexCfgKey);
#if 0
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, configKeyModuleLock, sizeof(configKeyModuleLock));
#else
    {
        HLSE_OBJECT_HANDLE handleCfgKeyModuleLock;
        HLSE_OBJECT_INDEX index = indexCfgKey;
        HLSE_OBJECT_TYPE objType = HLSE_CONFIG_KEY;

        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &configKeyModuleLock; // &smVal;
        attr[2].valueLen = sizeof(configKeyModuleLock); //sizeof(smVal);

        err = HLSE_CreateObject(attr, templateSize, &handleCfgKeyModuleLock);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyModuleLock", configKeyModuleLock, sizeof(configKeyModuleLock), AX_COLON_32);

    indexCfgKey = A71CH_CFG_KEY_IDX_PRIVATE_KEYS;
    PRINTF("\r\nA71_SetConfigKey(0x%02x)\r\n", indexCfgKey);
#if 0
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, configKeyPrivateKey, sizeof(configKeyPrivateKey));
#else
    {
        HLSE_OBJECT_HANDLE handleCfgKeyPrivateKey;
        HLSE_OBJECT_INDEX index = indexCfgKey;
        HLSE_OBJECT_TYPE objType = HLSE_CONFIG_KEY;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &configKeyPrivateKey;
        attr[2].valueLen = sizeof(configKeyPrivateKey);

        err = HLSE_CreateObject(attr, templateSize, &handleCfgKeyPrivateKey);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyPrivateKey", configKeyPrivateKey, sizeof(configKeyPrivateKey), AX_COLON_32);

    indexCfgKey = A71CH_CFG_KEY_IDX_PUBLIC_KEYS;
    PRINTF("\r\nA71_SetConfigKey(0x%02x)\r\n", indexCfgKey);
#if 0
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, configKeyPublicKey, sizeof(configKeyPublicKey));
#else
    {
        HLSE_OBJECT_HANDLE handleCfgKeyPublicKey;
        HLSE_OBJECT_INDEX index = indexCfgKey;
        HLSE_OBJECT_TYPE objType = HLSE_CONFIG_KEY;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &configKeyPublicKey;
        attr[2].valueLen = sizeof(configKeyPublicKey);

        err = HLSE_CreateObject(attr, templateSize, &handleCfgKeyPublicKey);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyPublicKey", configKeyPublicKey, sizeof(configKeyPublicKey), AX_COLON_32);

    // Get the Module's handle


    err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

#ifdef EX_CONFIG_DEMO_LOCK_UNLOCK
    // Lock device for transport
    PRINTF("\r\nA71_LockModule()\r\n");
#if 0
    err = A71_LockModule();
#else
    {
        HLSE_LIFE_CYCLE_STATE lifeCycleState = HLSE_MODULE_LOCKED;

        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE;
        attr.value = &lifeCycleState;
        attr.valueLen = sizeof(lifeCycleState);

        err = HLSE_SetObjectAttribute(moduleHandle, &attr);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");

    PRINTF("\r\nA71_GetUniqueID().\r\n");
#if 0
    err = A71_GetUniqueID(uid, &uidLen);
#else
    {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_MODULE_UNIQUE_ID;
        attr.value = uid;
        attr.valueLen = uidLen;

        err = HLSE_GetObjectAttribute(moduleHandle, &attr);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("uid", uid, uidLen, AX_COLON_32);

    PRINTF("\r\nA71_GetUnlockChallenge().\r\n");
#if 0
    err = A71_GetUnlockChallenge(challenge, &challengeLen);
#else
    {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_MODULE_UNLOCK_CHALLENGE;
        attr.value = challenge;
        attr.valueLen = challengeLen;

        err = HLSE_GetObjectAttribute(moduleHandle, &attr);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("challenge", challenge, challengeLen, AX_COLON_32);

    // Decrypt challenge
    hcRet = HOST_AES_ECB_DECRYPT(unlockCode, challenge, configKeyModuleLock, sizeof(configKeyModuleLock));
    if (hcRet != HOST_CRYPTO_OK)
    {
        PRINTF("HOST_AES_ECB_DECRYPT: failed.\r\n");
        result = 0;
    }

    PRINTF("\r\nA71_UnlockModule()\r\n");
#if 0
    err = A71_UnlockModule(unlockCode, 16);
#else
    {
        HLSE_A71_MODULE_UNLOCK_PARAMS moduleUnlockVal;
        HLSE_ATTRIBUTE attr;

        moduleUnlockVal.lockMode = HLSE_MODULE_UNLOCKED;
        memcpy(&moduleUnlockVal.response, unlockCode, 16);

        attr.type = HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE;
        attr.value = &moduleUnlockVal;
        attr.valueLen = sizeof(moduleUnlockVal);

        err = HLSE_SetObjectAttribute(moduleHandle, &attr);
    }

#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
#endif

    PRINTF( "\r\n-----------\r\nEnd exSetConfigKey(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));

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

    // to hold handles for created keys
    HLSE_OBJECT_HANDLE handles[A71CH_PUBLIC_KEY_MAX] = {0};

    PRINTF("\r\n-----------\r\nStart exSstPubEnc(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);

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
        PRINTF("\r\nHOSTCRYPTO_AesWrapKeyRFC3394(0x%02X)\r\n", (SST_Index_t)i);
        err = HOSTCRYPTO_AesWrapKeyRFC3394(configKeyPublicKey, sizeof(configKeyPublicKey),
            wrappedKey, &wrappedKeyLen, &(eccKcCA[i].pub[1]), eccKcCA[i].pubLen - 1);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("wrappedKey", wrappedKey, wrappedKeyLen, AX_COLON_32);

        PRINTF( "\r\nA71_SetEccPublicKey(0x%02X)\r\n", (SST_Index_t)i);
#if 0
        err = A71_SetEccPublicKey ((SST_Index_t) i, wrappedKey, wrappedKeyLen);
#else
        {
            HLSE_OBJECT_INDEX index = i;
            HLSE_OBJECT_TYPE objType = HLSE_PUBLIC_KEY;

            HLSE_ATTRIBUTE attr[3];
            unsigned short templateSize = 3;

            attr[0].type = HLSE_ATTR_OBJECT_TYPE;
            attr[0].value = &objType;
            attr[0].valueLen = sizeof(objType);
            attr[1].type = HLSE_ATTR_OBJECT_INDEX;
            attr[1].value = &index;
            attr[1].valueLen = sizeof(index);
            attr[2].type = HLSE_ATTR_OBJECT_VALUE;
            attr[2].value = wrappedKey;
            attr[2].valueLen = wrappedKeyLen;

            err = HLSE_CreateObject(attr, templateSize, &handles[i]);
        }
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Read out and verify public key
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        HLSE_ATTRIBUTE attr;
        PRINTF( "\r\nA71_GetEccPublicKey(0x%02X)\r\n", (SST_Index_t)i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
#if 0
        err = A71_GetEccPublicKey ((SST_Index_t) i, fetchedPubKey, &fetchedPubKeyLen);
#else
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = fetchedPubKey;
        attr.valueLen = fetchedPubKeyLen;

        err = HLSE_GetObjectAttribute(handles[i], &attr);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Compare with reference value
        result &= AX_COMPARE_BYTE_ARRAY("eccKcCA[i].pub", eccKcCA[i].pub, eccKcCA[i].pubLen,
            "fetchedPubKey", fetchedPubKey, fetchedPubKeyLen, AX_COLON_32);
    }

    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        HOSTCRYPTO_FreeEccKey(&eccKeyCA[i]);
    }

    PRINTF( "\r\n-----------\r\nEnd exSstPubEnc(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
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

    HLSE_OBJECT_HANDLE handles[A71CH_PUBLIC_KEY_MAX] = {0};
    U16 handleNum = A71CH_PUBLIC_KEY_MAX;

    err = HLSE_EnumerateObjects(HLSE_PUBLIC_KEY, handles, &handleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    if ((err != HLSE_SW_OK)) {
        return 0;
    }

    PRINTF("\r\n-----------\r\nStart exSstPubEraseAfterInjectLock()\r\n------------\r\n");

    // Check whether ErasePublicKey - without challenge/response - fails
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        PRINTF( "\r\nA71_EraseEccPublicKey(0x%02X)\r\n", (SST_Index_t)i);
#if 0
        err = A71_EraseEccPublicKey((SST_Index_t) i);
        result &= AX_CHECK_SW(err, SW_WRONG_DATA, "err");
#else
        if (i < handleNum ) {
            err = HLSE_EraseObject(handles[i]);
            result &= AX_CHECK_SW(err, SW_WRONG_DATA, "err");
        }
#endif
    }

    // Confirm reading out public key is still possible
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        PRINTF("\r\nA71_GetEccPublicKey (index=0x%02X)\r\n", i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
#if 0
        err = A71_GetEccPublicKey ((SST_Index_t)i, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve Public Ecc Key");
#else
        if (i < handleNum) {
            err = hlse_GetEccPublicKey(fetchedPubKey, &fetchedPubKeyLen, &handles[i]);
            result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve Public Ecc Key");
        }
#endif
    }

    // Do a ErasePublicKey with challenge response
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        PRINTF( "\r\nA71_EraseEccPublicKeyWithChallenge(0x%02X)\r\n", (SST_Index_t)i);
        err = A71_EraseEccPublicKeyWithChallenge((SST_Index_t) i, configKeyPublicKey, sizeof(configKeyPublicKey));
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check whether reading out (erased) public key now fails.
    for (i=0; i<A71CH_PUBLIC_KEY_MAX; i++)
    {
        PRINTF("\r\nA71_GetEccPublicKey (index=0x%02X)\r\n", i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
#if 0
        err = A71_GetEccPublicKey ((SST_Index_t)i, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");
#else
        if (i < handleNum) {
            err = hlse_GetEccPublicKey(fetchedPubKey, &fetchedPubKeyLen, &handles[i]);
            result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");
        }
#endif
    }

    PRINTF( "\r\n-----------\r\nEnd exSstPubEraseAfterInjectLock(), result = %s\r\n------------\r\n",
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
 */
static U8 exSstKeyPairEnc(U8 initMode)
{
    U8 result = 1;
    U16 err;
    int i;
    HLSE_RET_CODE retcode;

    ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    EC_KEY *eccKeyTls[A71CH_KEY_PAIR_MAX] = { 0 };
    eccKeyComponents_t eccKcTls[A71CH_KEY_PAIR_MAX] = { 0 };

    EC_KEY *eccKeyAlt = NULL;
    eccKeyComponents_t eccKcAlt;

    U8 fetchedPubKey[65];
    U16 fetchedPubKeyLen = sizeof(fetchedPubKey);

    U8 hashSha256[32];
    U16 hashSha256Len = sizeof(hashSha256);

    U8 signature[128];
    U16 signatureLen = sizeof(signature);

    // const U16 expectedPubKeyLen = 65;
    const U16 expectedPrivKeyLen = 32;

    U8 wrappedPrivKey[32+8];
    U16 wrappedPrivKeyLen = sizeof(wrappedPrivKey);

    SST_Index_t kpIndex;

    // to hold handles for created keys
    HLSE_OBJECT_HANDLE handles[A71CH_KEY_PAIR_MAX] = {0};
    HLSE_OBJECT_HANDLE moduleHandle = 0;
    U16 moduleHandleNum = 1;
    HLSE_MECHANISM_INFO mechInfo;

    PRINTF("\r\n-----------\r\nStart exSstKeyPairEnc(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);

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
        PRINTF("\r\nHOSTCRYPTO_AesWrapKeyRFC3394(0x%02X)\r\n", (SST_Index_t)i);
        err = HOSTCRYPTO_AesWrapKeyRFC3394(configKeyPrivateKey, sizeof(configKeyPrivateKey),
            wrappedPrivKey, &wrappedPrivKeyLen, eccKcTls[i].priv, eccKcTls[i].privLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("wrappedPrivKey", wrappedPrivKey, wrappedPrivKeyLen, AX_COLON_32);

        PRINTF( "\r\nA71_SetEccKeyPair(0x%02X)\r\n", (SST_Index_t)i);
#if 0
        err = A71_SetEccKeyPair((SST_Index_t) i, eccKcTls[i].pub, eccKcTls[i].pubLen,
            wrappedPrivKey, wrappedPrivKeyLen);
#else
        {
            // to hold wrapped private key + public key
            U8 keyPair[40 + 65];
            HLSE_OBJECT_INDEX index = i;
            HLSE_OBJECT_TYPE objType = HLSE_KEY_PAIR;
            HLSE_ATTRIBUTE attr[3];
            unsigned short templateSize = 3;

            memcpy(keyPair, wrappedPrivKey, 40);
            memcpy(keyPair + 40, eccKcTls[i].pub, 65);

            attr[0].type = HLSE_ATTR_OBJECT_TYPE;
            attr[0].value = &objType;
            attr[0].valueLen = sizeof(objType);
            attr[1].type = HLSE_ATTR_OBJECT_INDEX;
            attr[1].value = &index;
            attr[1].valueLen = sizeof(index);
            attr[2].type = HLSE_ATTR_WRAPPED_OBJECT_VALUE;
            attr[2].value = keyPair;
            attr[2].valueLen = sizeof(keyPair);

            err = HLSE_CreateObject(attr, templateSize, &handles[i]);
        }
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Read out and verify public key
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        HLSE_ATTRIBUTE attr;
        PRINTF( "\r\nA71_GetPublicKeyEccKeyPair (0x%02X)\r\n", (SST_Index_t)i);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
#if 0
        err = A71_GetPublicKeyEccKeyPair  ((SST_Index_t) i, fetchedPubKey, &fetchedPubKeyLen);
#else
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = fetchedPubKey;
        attr.valueLen = fetchedPubKeyLen;

        err = HLSE_GetObjectAttribute(handles[i], &attr);
#endif
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
        PRINTF("\r\nA71_EccSign(0x%02X)\r\n", (SST_Index_t)i);
#if 0
        err = A71_EccSign((SST_Index_t)i, hashSha256, hashSha256Len, signature, &signatureLen);
#else
        err = hlse_EccSign(handles[i], hashSha256, hashSha256Len, signature, &signatureLen);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Verify ... on opposite platform
        PRINTF("\r\n(Host)ECDSA_verify API with eccKeyTls[i] (verify signature created on A71CH).\r\n");
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_VERIFY;
        retcode = HLCRYPT_Verify(&mechInfo,(U8 *)eccKeyTls[i],0,hashSha256,hashSha256Len,signature,signatureLen);

        if (retcode == HLSE_SW_OK)
        {
            PRINTF("Verification OK for eccKeyTls[i].\r\n");
        }
        else
        {
            PRINTF("Return value: %d, Verification Not OK for eccKeyTls[i]. Test Failed!\r\n", retcode);
            result &= 0;
            break;
        }
    }

    // Erase the keypair at index 0 & verify the value is no longer readable
    // ** Erase **
    kpIndex = A71CH_KEY_PAIR_0;
    PRINTF("\r\nA71_EraseEccKeyPair(index=0x%02X)\r\n", kpIndex);
#if 0
    err = A71_EraseEccKeyPair(kpIndex);
#else
    err = HLSE_EraseObject(handles[kpIndex]);
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // ** Check whether erase was effective **
    PRINTF("\r\nA71_GetPublicKeyEccKeyPair (index=0x%02X)\r\n", kpIndex);
    fetchedPubKeyLen = sizeof(fetchedPubKey);
#if 0
    err = A71_GetPublicKeyEccKeyPair ((SST_Index_t)kpIndex, fetchedPubKey, &fetchedPubKeyLen);
#else
    {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = fetchedPubKey;
        attr.valueLen = fetchedPubKeyLen;

        err = HLSE_GetObjectAttribute(handles[kpIndex], &attr);
    }
#endif
    result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");

    // Now disable the plain insertion/reading out of Symmetric keys & Keypairs
    PRINTF("\r\nA71_InjectLock()\r\n");
#if 0
    err = A71_InjectLock();
#else
    // Get the Module's handle
    err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    {
        HLSE_LIFE_CYCLE_STATE lifeCycleState = HLSE_INJECT_LOCKED;

        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_MODULE_INJECTION_LOCK_STATE;
        attr.value = &lifeCycleState;
        attr.valueLen = sizeof(lifeCycleState);

        err = HLSE_SetObjectAttribute(moduleHandle, &attr);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    assert(result);

    // Check whether the plain inject has been disabled (check only first key pair)
    PRINTF( "\r\nA71_SetEccKeyPair(0x%02x)\r\n", kpIndex);
#if 0
    err = A71_SetEccKeyPair((SST_Index_t)kpIndex, eccKcAlt.pub, eccKcAlt.pubLen, eccKcAlt.priv, eccKcAlt.privLen);
#else
    {
        // to hold wrapped private key + public key
        U8 keyPair[40 + 65];
        HLSE_OBJECT_INDEX index = kpIndex;
        HLSE_OBJECT_TYPE objType = HLSE_KEY_PAIR;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        memcpy(keyPair, eccKcAlt.priv, eccKcAlt.privLen);
        memcpy(keyPair + eccKcAlt.privLen, eccKcAlt.pub, 65);


        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;  // plain
        attr[2].value = keyPair;
        attr[2].valueLen = eccKcAlt.privLen + eccKcAlt.pubLen;

        err = HLSE_CreateObject(attr, templateSize, &handles[kpIndex]);
    }
#endif
    result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "Expected to fail, frozen credential cannot be overwritten");

    // Put back the original (reference) value in the first slot, now first wrap private key on host
    wrappedPrivKeyLen = sizeof(wrappedPrivKey);
    PRINTF("\r\nHOSTCRYPTO_AesWrapKeyRFC3394(0x%02X)\r\n", (SST_Index_t)kpIndex);
    err = HOSTCRYPTO_AesWrapKeyRFC3394(configKeyPrivateKey, sizeof(configKeyPrivateKey),
        wrappedPrivKey, &wrappedPrivKeyLen, eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("wrappedPrivKey", wrappedPrivKey, wrappedPrivKeyLen, AX_COLON_32);

    PRINTF( "\r\nA71_SetEccKeyPair(0x%02X)\r\n", (SST_Index_t)kpIndex);
#if 0
    err = A71_SetEccKeyPair((SST_Index_t) kpIndex, eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
        wrappedPrivKey, wrappedPrivKeyLen);
#else
    {
        // to hold wrapped private key + public key
        U8 keyPair[40 + 65];

        HLSE_OBJECT_INDEX index = kpIndex;
        HLSE_OBJECT_TYPE objType = HLSE_KEY_PAIR;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        memcpy(keyPair, wrappedPrivKey, 40);
        memcpy(keyPair + 40, eccKcTls[kpIndex].pub, 65);

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_WRAPPED_OBJECT_VALUE;
        attr[2].value = keyPair;
        attr[2].valueLen = sizeof(keyPair);

        err = HLSE_CreateObject(attr, templateSize, &handles[kpIndex]);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Check all keys are still fine by doing a sign/verify operation
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        signatureLen = sizeof(signature);
        PRINTF("\r\nA71_EccSign(0x%02X)\r\n", (SST_Index_t)i);
#if 0
        err = A71_EccSign((SST_Index_t)i, hashSha256, hashSha256Len, signature, &signatureLen);
#else
        err = hlse_EccSign(handles[i], hashSha256, hashSha256Len, signature, &signatureLen);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Verify ... on opposite platform
        PRINTF("\r\n(Host)ECDSA_verify API with eccKeyTls[i] (verify signature created on A71CH).\r\n");
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_VERIFY;
        retcode = HLCRYPT_Verify(&mechInfo,(U8 *)eccKeyTls[i],0,hashSha256,hashSha256Len,signature,signatureLen);

        if (retcode == HLSE_SW_OK)
        {
            PRINTF("Verification OK for eccKeyTls[i].\r\n");
        }
        else
        {
            PRINTF("Return value: %d, Verification Not OK for eccKeyTls[i]. Test Failed!\r\n", retcode);
            result &= 0;
            break;
        }
    }

    // Check whether the plain inject has been disabled (for all key pair)
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_SetEccKeyPair(0x%02X)\r\n", (SST_Index_t)kpIndex);
#if 0
        err = A71_SetEccKeyPair((SST_Index_t) kpIndex, eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
            eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
#else
        {
            // to hold private key + public key
            U8 keyPair[40 + 65];

            HLSE_OBJECT_INDEX index = kpIndex;
            HLSE_OBJECT_TYPE objType = HLSE_KEY_PAIR;
            HLSE_ATTRIBUTE attr[3];
            unsigned short templateSize = 3;


            memcpy(keyPair, eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
            memcpy(keyPair + eccKcTls[kpIndex].privLen, eccKcTls[kpIndex].pub, 65);

            attr[0].type = HLSE_ATTR_OBJECT_TYPE;
            attr[0].value = &objType;
            attr[0].valueLen = sizeof(objType);
            attr[1].type = HLSE_ATTR_OBJECT_INDEX;
            attr[1].value = &index;
            attr[1].valueLen = sizeof(index);
            attr[2].type = HLSE_ATTR_OBJECT_VALUE;  // plain
            attr[2].value = keyPair;
            attr[2].valueLen = eccKcTls[kpIndex].privLen + eccKcTls[kpIndex].pubLen;

            err = HLSE_CreateObject(attr, templateSize, &handles[kpIndex]);
        }
#endif
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "err");
    }

    // Check whether GenerateKeyPair - without challenge/response - fails
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_GenerateEccKeyPair(0x%02X)\r\n", (SST_Index_t)kpIndex);
#if 0
        err = A71_GenerateEccKeyPair((SST_Index_t) kpIndex);
#else
        err = hlse_GenerateEccKeyPair(kpIndex, &handles[kpIndex]);
#endif
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "err");
    }

    // Do a GenerateKeyPair with challenge response
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_GenerateEccKeyPairWithChallenge(0x%02X)\r\n", (SST_Index_t)kpIndex);
        err = A71_GenerateEccKeyPairWithChallenge((SST_Index_t) kpIndex, configKeyPrivateKey, sizeof(configKeyPrivateKey));
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    //!< \todo Extend example program to check whether there's a new keypair inside A71CH

    // Check whether EraseKeyPair - without challenge/response - fails
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_EraseEccKeyPair(0x%02X)\r\n", (SST_Index_t)kpIndex);
#if 0
        err = A71_EraseEccKeyPair((SST_Index_t) kpIndex);
#else
        err = HLSE_EraseObject(handles[kpIndex]);
#endif
        result &= AX_CHECK_SW(err, SW_WRONG_DATA, "err");
    }

    // Do a EraseKeyPair with challenge response
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_EraseEccKeyPairWithChallenge(0x%02X)\r\n", (SST_Index_t)kpIndex);
        err = A71_EraseEccKeyPairWithChallenge((SST_Index_t) kpIndex, configKeyPrivateKey, sizeof(configKeyPrivateKey));
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check whether reading out public key from erased keypairs now fails.
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF("\r\nA71_GetPublicKeyEccKeyPair (index=0x%02X)\r\n", kpIndex);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
#if 0
        err = A71_GetPublicKeyEccKeyPair ((SST_Index_t)kpIndex, fetchedPubKey, &fetchedPubKeyLen);
#else
        err = hlse_GetEccPublicKey(fetchedPubKey, &fetchedPubKeyLen, &handles[kpIndex]);
#endif
        result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");
    }

    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        HOSTCRYPTO_FreeEccKey(&eccKeyTls[i]);
    }

    PRINTF( "\r\n-----------\r\nEnd exSstKeyPairEnc(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    return result;
}
