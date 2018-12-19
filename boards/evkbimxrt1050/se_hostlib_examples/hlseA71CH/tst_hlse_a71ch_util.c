/**
 * @file tst_hlse_a71ch_util.c
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
 * This module implements test bench utility functions specific to the hlse layer on top of a71ch
 * @par History
 * 1.0   2016-Oct-1 : Initial version
 */

/*******************************************************************
 * standard include files
 *******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*******************************************************************
 * project specific include files
 *******************************************************************/

#include "scp.h"
#include "sm_apdu.h"
#include "sm_errors.h"

#include "tst_sm_util.h"
#include "tst_a71ch_util.h"
#include "tst_hlse_a71ch_util.h"
#include "ax_util.h"
#include "a71_debug.h"
#include "tstHostCrypto.h"
#include "global_platf.h"

// #include "a71ch_ex_hlse.h"

#include "HLSEAPI.h"

// TODO : consider putting the below struct in HLSE header
// Key Operations
typedef struct {
    U8 keyVersion;
    U8 keyEnc[16];
    U8 keyMac[16];
    U8 keyDek[16];
    U8* currentKeyDek;
    U16 currentKeyDekLen;
} HLSE_A71_SM_KEYS_DATA;

/**
 * Initializes the A71CH module.
 * All initialization states are only reachable when the A71CH is in Debug Mode
 *
 * \param[in] initMode Either ::INIT_MODE_RESET, ::INIT_MODE_RESET_SELECT, ::INIT_MODE_NO_RESET,
 *    ::INIT_MODE_RESET_DO_SCP03, ::INIT_MODE_RESET_SELECT_DO_SCP03 or ::INIT_MODE_NO_RESET_DO_SCP03
 * \retval 0 failure
 * \retval 1 success
 */
U8 hlse_a71chInitModule(U8 initMode)
{
    U8 result = 1;
    U16 err = 0;
    U8 resetMode = 0;
    U8 scp03Mode = 0;

    U8 appletName[] = { 0x61, 0x37, 0x31, 0x63, 0x68 };
    U16 appletNameLen = sizeof(appletName);
    U8 response[256];
    U16 responseLen = sizeof(response);

    PRINTF("\na71chInitModule(%s)\n", getInitModeAsString(initMode));

    resetMode = initMode & INIT_MODE_RESET_MASK;

    switch(resetMode)
    {
    case INIT_MODE_PATTERN_RESET:
        PRINTF("Reset A71CH.\n");
#if 0
        err = A71_DbgReset();
#else
        err = HLSE_DbgReset();
#endif
        result &= AX_CHECK_SW(err, SW_OK, "Failed to reset module");
        break;
    case INIT_MODE_PATTERN_RESET_SELECT:
        PRINTF("Reset A71CH.\n");
#if 0
        err = A71_DbgReset();
#else
        err = HLSE_DbgReset();
#endif
        result &= AX_CHECK_SW(err, SW_OK, "Failed to reset module");
        PRINTF("Select applet.\n");
        err = GP_Select(appletName, appletNameLen, response, &responseLen);
        result &= AX_CHECK_SW(err, SW_OK, "Failed to select applet");
        break;
    case INIT_MODE_PATTERN_NO_RESET:
        result &= 1;
        break;
    }

    scp03Mode = initMode & INIT_MODE_SCP03_MASK;

    switch(scp03Mode)
    {
    case INIT_MODE_PATTERN_PLAIN_COM:
        result &= 1;
        break;
    case INIT_MODE_PATTERN_DO_SCP03:
        result = hlse_a71chSetupScp03();
        break;
    }

    return result;
}


/**
 * Establish an SCP03 channel with random keys.
 * Best called implicitly via ::a71chInitModule
 *
 * \retval 0 failure
 * \retval 1 success
 */
U8 hlse_a71chSetupScp03()
{
#if defined(NO_SECURE_CHANNEL_SUPPORT)
    U8 result = 0;
#else
    U8 result = 1;
    U16 err = 0;
    U8 random[3*SCP_KEY_SIZE];
    U8 randomLen = (U8)sizeof(random);
    U8 *currentKeyDek = NULL;
    U8 keyVersion = 1;
    U8 keyEnc[SCP_KEY_SIZE];
    U8 keyMac[SCP_KEY_SIZE];
    U8 keyDek[SCP_KEY_SIZE];
    U8 sCounter[3];
    U16 sCounterLen = sizeof(sCounter);
    HLSE_OBJECT_HANDLE moduleHandle;
    U16 moduleHandleNum = 1;
    HLSE_ATTRIBUTE attr;
    HLSE_SECURE_CHANNEL_SCP03_ESTABLISH_PARAMS scp03Params;
    HLSE_SECURE_CHANNEL_ESTABLISH_PARAMS scpParams;
    HLSE_SCP03_CHANNEL_STATE scp03ChannelState;
    HLSE_SECURE_CHANNEL_STATE scpState;

    PRINTF( "\n-----------\nStart a71chSetupScp03()\n------------\n");

    DEV_ClearChannelState();

    // Security module generates random data for initial SCP03 keys
    sm_printf(CONSOLE, "\nA71_GetRandom(randomLen=%d)\n", randomLen);
#if 0
    err = A71_GetRandom(random, randomLen);
#else
    // Get the Module's handle

    err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    attr.type = HLSE_ATTR_MODULE_RANDOM;
    attr.value = random;
    attr.valueLen = randomLen;

    err = HLSE_GetObjectAttribute(moduleHandle, &attr);
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    if (result != 1) { goto SCP03_EXIT; }

    // Storing Static Keys
    memcpy(keyEnc, random, SCP_KEY_SIZE);
    memcpy(keyMac, random + SCP_KEY_SIZE, SCP_KEY_SIZE);
    memcpy(keyDek, random + (2*SCP_KEY_SIZE), SCP_KEY_SIZE);

    keyVersion = (U8) (SST_HOST_SCP_KEYSET >> 8);
    sm_printf(CONSOLE, "\nSCP_GP_PutKeys(keyVersion=0x%02X)\n", keyVersion);
#if 0
    err = SCP_GP_PutKeys(keyVersion, keyEnc, keyMac, keyDek, currentKeyDek, AES_KEY_LEN_nBYTE);
#else
    {
        HLSE_OBJECT_HANDLE objHandle;

        HLSE_OBJECT_INDEX index = 0;
        HLSE_OBJECT_TYPE objType = HLSE_SM_KEYS;

        HLSE_A71_SM_KEYS_DATA smVal;
        HLSE_ATTRIBUTE attr[3];
        unsigned short templateSize = 3;

        memcpy(&smVal.keyEnc, keyEnc, AES_KEY_LEN_nBYTE);
        memcpy(&smVal.keyMac, keyMac, AES_KEY_LEN_nBYTE);
        memcpy(&smVal.keyDek, keyDek, AES_KEY_LEN_nBYTE);
        smVal.currentKeyDek = currentKeyDek;
        smVal.currentKeyDekLen = 0;
        smVal.keyVersion = keyVersion;

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &smVal;
        attr[2].valueLen = sizeof(smVal);


        err = HLSE_CreateObject(attr, templateSize, &objHandle);
    }
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    if (result != 1) { goto SCP03_EXIT; }

    // Authenticate Channel
    sm_printf(CONSOLE, "\nSCP_Authenticate()\n");
#if 0
    err = SCP_Authenticate(keyEnc, keyMac, keyDek, SCP_KEY_SIZE, sCounter, &sCounterLen);
#else

    memcpy(&scp03Params.keyEnc, keyEnc, AES_KEY_LEN_nBYTE);
    memcpy(&scp03Params.keyMac, keyMac, AES_KEY_LEN_nBYTE);
    memcpy(&scp03Params.keyDek, keyDek, AES_KEY_LEN_nBYTE);

    scpParams.type = HLSE_SCP03;
    scpParams.pParameter = &scp03Params;
    scpParams.ulParameterLen = sizeof(scp03Params);

    memset(&scp03ChannelState, 0, sizeof(scp03ChannelState));
    memcpy(&scp03ChannelState.cCounter, sCounter, sCounterLen);

    scpState.type = HLSE_SCP03;
    scpState.pParameter = &scp03ChannelState;
    scpState.ulParameterLen = sizeof(scp03ChannelState);

    err = HLSE_SMChannelAuthenticate(&scpParams, &scpState);
    sCounterLen = scpState.ulParameterLen;
#endif
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(sCounterLen, 0, "Only expected when SCP03 is configured for pseudo-random challenge");

SCP03_EXIT:
    PRINTF( "\n-----------\nEnd a71chSetupScp03(), result = %s\n------------\n",
            ((result == 1)? "OK": "FAILED"));
#endif
    return result;
}

/**
 * Issue an A71_GetModuleInfo call, print status to stdout and return select parameters to caller.
 *
 * \param[out] scpState Either ::A71CH_SCP_MANDATORY, ::A71CH_SCP_NOT_SET_UP or ::A71CH_SCP_KEYS_SET
 *
 * \retval 0 failure
 * \retval 1 success
 */
U8 hlse_a71chShowModuleInfo(U8 *scpState)
{
    U8 result = 1;
    U16 err;
    U16 selectResponse = 0;
    U8 debugOn = 0;
    U8 restrictedKpIdx = 0;
    U8 transportLockState = 0;
    U8 injectLockState = 0;
    U16 gpStorageSize = 0;

    sm_printf(CONSOLE, "A71_GetModuleInfo().\n");
#if 0
    err = A71_GetModuleInfo(&selectResponse, &debugOn, &restrictedKpIdx, &transportLockState, scpState, &injectLockState, &gpStorageSize);
#else
    // Note : currently in the Generic API it is not possible to fetch all module info in one call
    // rather you have to use multiple calls to HLSE_GetObjectAttribute() with one of the following attributes types:
    // HLSE_ATTR_MODULE_TOTAL_GP_SIZE, HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE, HLSE_ATTR_MODULE_SCP_LOCK_STATE,
    // HLSE_ATTR_MODULE_INJECTION_LOCK_STATE, HLSE_ATTR_MODULE_APPLET_INFO
    // see hlse_GetGPDataSize as an exmaple:
    err = hlse_GetGPDataSize(&gpStorageSize);
    result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve GP Data size.");

    err = A71_GetModuleInfo(&selectResponse, &debugOn, &restrictedKpIdx, &transportLockState, scpState, &injectLockState, &gpStorageSize);
#endif

    result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve module info.");

    if (err == SW_OK)
    {
        PRINTF("A71CH in %s\n", (debugOn == 0) ? "Production Version" : "Debug Mode Version");
        PRINTF("selectResponse:  0x%04X\n", selectResponse);
        if (restrictedKpIdx != A71CH_NO_RESTRICTED_KP)
        {
            PRINTF("restricted keypair index: 0x%02X\n", restrictedKpIdx);
        }
        PRINTF("transportLockState: 0x%02X (%s)\n", transportLockState,
            (transportLockState == A71CH_TRANSPORT_LOCK_STATE_LOCKED) ? "Transport Lock is set" :
            (transportLockState == A71CH_TRANSPORT_LOCK_STATE_UNLOCKED) ? "Open device, Transport Lock can no longer be set" :
            (transportLockState == A71CH_TRANSPORT_LOCK_STATE_ALLOW_LOCK) ? "Transport Lock NOT YET set" : "Undefined Transport Lock state");
        PRINTF("scpState: 0x%02X (%s)\n", *scpState,
            (*scpState == A71CH_SCP_MANDATORY) ? "SCP is mandatory" :
            (*scpState == A71CH_SCP_NOT_SET_UP) ? "SCP is not set up" :
            (*scpState == A71CH_SCP_KEYS_SET) ? "SCP keys set" : "Undefined SCP state");
        PRINTF("injectLockState: 0x%02X (%s)\n", injectLockState,
            (injectLockState == A71CH_INJECT_LOCK_STATE_LOCKED) ? "Locked" :
            (injectLockState == A71CH_INJECT_LOCK_STATE_UNLOCKED) ? "Unlocked" : "Undefined Inject Lock State");
        PRINTF("gpStorageSize:   %d\n", gpStorageSize);
    }
    return result;
}


/*
 * hlse helpers
 */

U16 hlse_GenerateEccKeyPair(SST_Index_t index,
                            HLSE_OBJECT_HANDLE* hObject)
{
    //HLSE_OBJECT_HANDLE handle;
    HLSE_OBJECT_INDEX ind = index;
    HLSE_OBJECT_TYPE objType = HLSE_KEY_PAIR;

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;
    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &ind;
    attr[1].valueLen = sizeof(ind);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = NULL;       // generate
    attr[2].valueLen = 0;

    return HLSE_CreateObject(attr, templateSize, hObject);
}


U16 hlse_GetEccPublicKey(U8 *publicKey, U16 *publicKeyLen,
                         HLSE_OBJECT_HANDLE* hObject)
{
    HLSE_ATTRIBUTE attr;
    U16 err;
    attr.type = HLSE_ATTR_OBJECT_VALUE;
    attr.value = publicKey;
    attr.valueLen = *publicKeyLen;
    err = HLSE_GetObjectAttribute(*hObject, &attr);

    *publicKeyLen = attr.valueLen;
    return err;
}



U16 hlse_SetEccKeyPair(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen, const U8 *privateKey, U16 privateKeyLen,
                       HLSE_OBJECT_HANDLE* hObject)
{
    // to hold wrapped private key + public key
    U8 keyPair[40 + 65];
    HLSE_OBJECT_INDEX ind = index;
    HLSE_OBJECT_TYPE objType = HLSE_KEY_PAIR;

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;
    U16 err;

    memcpy(keyPair, privateKey, privateKeyLen);
    memcpy(keyPair + privateKeyLen, publicKey, 65 /* = publicKeyLen*/);

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &ind;
    attr[1].valueLen = sizeof(ind);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;  // plain
    attr[2].value = keyPair;
    attr[2].valueLen = privateKeyLen + publicKeyLen;

    err = HLSE_CreateObject(attr, templateSize, hObject);

    return err;
}


U16 hlse_SetEccPublicKey(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen,
                         HLSE_OBJECT_HANDLE* hObject)
{
    U16 err;
    HLSE_OBJECT_INDEX ind = index;
    HLSE_OBJECT_TYPE objType = HLSE_PUBLIC_KEY;

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &ind;
    attr[1].valueLen = sizeof(ind);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = (U8 *)publicKey;
    attr[2].valueLen = publicKeyLen;

    err = HLSE_CreateObject(attr, templateSize, hObject);

    return err;
}


U16 hlse_EcdhGetSharedSecret(HLSE_OBJECT_HANDLE hObject, const U8 *pOtherPublicKey, U16 otherPublicKeyLen, U8 *pSharedSecret, U16 *pSharedSecretLen)
{
    HLSE_ECDH_PARAMS params;
    HLSE_MECHANISM_INFO mechInfo;
    memset(&params, 0, sizeof(params));
    params.pPublicKey = (U8 *)pOtherPublicKey;
    params.ulPublicKeyLen = otherPublicKeyLen;

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDH;
    mechInfo.pParameter = &params;
    mechInfo.ulParameterLen = sizeof(params);

    return HLSE_DeriveKey(&mechInfo, hObject, pSharedSecret, pSharedSecretLen);
}


U16 hlse_EccSign(HLSE_OBJECT_HANDLE hObject, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen)
{
    HLSE_MECHANISM_INFO mechInfo;
    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_SIGN;

    return HLSE_Sign(&mechInfo, hObject, (U8*)pHash, hashLen, pSignature, pSignatureLen);
}

U16 hlse_EccNormalizedAsnSign(HLSE_OBJECT_HANDLE hObject, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen)
{
    HLSE_MECHANISM_INFO mechInfo;
    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_NORMALIZE_ASN_SIGN;

    return HLSE_Sign(&mechInfo, hObject, (U8 *)pHash, hashLen, pSignature, pSignatureLen);
}


U16 hlse_EccVerifyWithKey(const U8 *pKeyData, U16 keyDataLen, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult)
{
    HLSE_MECHANISM_INFO mechInfo;
    HLSE_RET_CODE err;
    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_VERIFY;

    err = HLSE_VerifySignatureWithExternalKey(&mechInfo, (U8*)pKeyData, keyDataLen, (U8*)pHash, hashLen, (U8 *)pSignature, signatureLen);

    // check if verification failed
    *pResult = (err == HLSE_ERR_GENERAL_ERROR) ? 0 : 1;

    return err;
}


U16 hlse_GetRandom(U8 *random, U8 randomLen)
{
    U8 result = 1;
    // Get the Module's handle
    HLSE_OBJECT_HANDLE moduleHandle;
    U16 moduleHandleNum = 1;
    HLSE_ATTRIBUTE attr;

    U16 err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    attr.type = HLSE_ATTR_MODULE_RANDOM;
    attr.value = random;
    attr.valueLen = randomLen;

    return HLSE_GetObjectAttribute(moduleHandle, &attr);
}


U16 hlse_GetCredentialInfo(U8 *map, U16 *mapLen)
{
    U8 result = 1;
    // Get the Module's handle
    HLSE_OBJECT_HANDLE moduleHandle;
    U16 moduleHandleNum = 1;
    HLSE_ATTRIBUTE attr;

    U16 err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    attr.type = HLSE_ATTR_MODULE_CREDENTIAL_INFO;
    attr.value = map;
    attr.valueLen = *mapLen;

    return HLSE_GetObjectAttribute(moduleHandle, &attr);
}


U16 hlse_SCP_GP_PutKeys(U8 keyVersion, U8 *keyEnc, U8 *keyMac, U8 *keyDek, U8 *currentKeyDek, U16 keyBytes)
{
    HLSE_OBJECT_HANDLE objHandle;

    HLSE_OBJECT_INDEX index = 0;
    HLSE_OBJECT_TYPE objType = HLSE_SM_KEYS;
    HLSE_A71_SM_KEYS_DATA smVal;
    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    memcpy(&smVal.keyEnc, keyEnc, keyBytes);
    memcpy(&smVal.keyMac, keyMac, keyBytes);
    memcpy(&smVal.keyDek, keyDek, keyBytes);
    smVal.currentKeyDek = currentKeyDek;
    smVal.currentKeyDekLen = 0;
    smVal.keyVersion = keyVersion;

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = &smVal;
    attr[2].valueLen = sizeof(smVal);


    return HLSE_CreateObject(attr, templateSize, &objHandle);
}


U16 hlse_SCP_Authenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes, U8 *sCounter, U16 *sCounterLen)
{
    U16 err;
    HLSE_SECURE_CHANNEL_SCP03_ESTABLISH_PARAMS scp03Params;
    HLSE_SECURE_CHANNEL_ESTABLISH_PARAMS scpParams;
    HLSE_SCP03_CHANNEL_STATE scp03ChannelState;
    HLSE_SECURE_CHANNEL_STATE scpState;

    memcpy(&scp03Params.keyEnc, keyEnc, keyBytes);
    memcpy(&scp03Params.keyMac, keyMac, keyBytes);
    memcpy(&scp03Params.keyDek, keyDek, keyBytes);

    scpParams.type = HLSE_SCP03;
    scpParams.pParameter = &scp03Params;
    scpParams.ulParameterLen = sizeof(scp03Params);

    memset(&scp03ChannelState, 0, sizeof(scp03ChannelState));
    memcpy(&scp03ChannelState.cCounter, sCounter, *sCounterLen);

    scpState.type = HLSE_SCP03;
    scpState.pParameter = &scp03ChannelState;
    scpState.ulParameterLen = sizeof(scp03ChannelState);

    err = HLSE_SMChannelAuthenticate(&scpParams, &scpState);
    *sCounterLen = scpState.ulParameterLen;

    return err;
}

U16 hlse_GetUniqueID(U8 *uid, U16 *uidLen)
{
    U8 result = 1;
    // Get the Module's handle
    HLSE_OBJECT_HANDLE moduleHandle;
    U16 moduleHandleNum = 1;
    HLSE_ATTRIBUTE attr;

    U16 err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    attr.type = HLSE_ATTR_MODULE_UNIQUE_ID;
    attr.value = uid;
    attr.valueLen = *uidLen;

    return HLSE_GetObjectAttribute(moduleHandle, &attr);
}


U16 hlse_InjectLock()
{
    U8 result = 1;
    // Get the Module's handle
    HLSE_OBJECT_HANDLE moduleHandle;
    U16 moduleHandleNum = 1;
    HLSE_ATTRIBUTE attr;
    HLSE_LIFE_CYCLE_STATE lifeCycleState = HLSE_INJECT_LOCKED;

    U16 err = HLSE_EnumerateObjects(HLSE_MODULE, &moduleHandle, &moduleHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");


    attr.type = HLSE_ATTR_MODULE_INJECTION_LOCK_STATE;
    attr.value = &lifeCycleState;
    attr.valueLen = sizeof(lifeCycleState);

    return HLSE_SetObjectAttribute(moduleHandle, &attr);

}


U16 hlse_SetSymKey(SST_Index_t index, const U8 *key, U16 keyLen, HLSE_OBJECT_HANDLE* hObject)
{
    HLSE_OBJECT_INDEX ind = index;
    HLSE_OBJECT_TYPE objType = HLSE_SYMMETRIC_KEY;

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;
    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &ind;
    attr[1].valueLen = sizeof(ind);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = (U8*)key;
    attr[2].valueLen = keyLen;

    return HLSE_CreateObject(attr, templateSize, hObject);
}


U16 hlse_EccVerify(SST_Index_t index, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult)
{
    HLSE_OBJECT_HANDLE handles[A71CH_PUBLIC_KEY_MAX];

    U8 result = 1;
    U16 handleNum = A71CH_PUBLIC_KEY_MAX;

    U16 err = HLSE_EnumerateObjects(HLSE_PUBLIC_KEY, handles, &handleNum);
    HLSE_MECHANISM_INFO mechInfo;
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    if ((err != HLSE_SW_OK) || (handleNum <= index)) {
        return 0;
    }

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_VERIFY;

    err = HLSE_VerifySignature(
        &mechInfo,
        handles[index],
        (U8*)pHash, hashLen,
        (U8*)pSignature, signatureLen);

    // check if verification failed
    *pResult = (err == HLSE_ERR_GENERAL_ERROR) ? 0 : 1;

    return err;
}


U16 hlse_GetGPDataSize(U16* gpSize)
{
    U8 result = 1;

    // Get the Module's handle
    HLSE_OBJECT_HANDLE modHandle = 0;
    U16 modHandleNum = 1;
    HLSE_ATTRIBUTE attr;
    U16 err = HLSE_EnumerateObjects(HLSE_MODULE, &modHandle, &modHandleNum);
    result &= AX_CHECK_SW(err, HLSE_SW_OK, "err");

    *gpSize = 0;
    attr.type = HLSE_ATTR_MODULE_TOTAL_GP_SIZE;
    attr.value = gpSize;
    attr.valueLen = sizeof(U16);
    return HLSE_GetObjectAttribute(modHandle, &attr);
}
