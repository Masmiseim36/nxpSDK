/**
 * @file ex_sst_kp.c
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
 * Example invocation of ECC key pair secure storage specific functionality of the A71CH
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
#include "tstHostCrypto.h"
#include "HostCryptoAPI.h"


/**
 * Demonstrate storage of key pairs:
 * - ::exSstKeyPair
 *
 */
U8 exSstKp()
{
    U8 result = 1;
    PRINTF( "\r\n-----------\r\nStart exSstKp()\r\n------------\r\n");

    DEV_ClearChannelState();

    // No channel encryption
    result &= exSstKeyPair(INIT_MODE_RESET);

    // With channel encryption
    result &= exSstKeyPair(INIT_MODE_RESET_DO_SCP03);

    // overall result
    PRINTF( "\r\n-----------\r\nEnd exSstKp(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}


/**
 * Demonstrate
 * - setting/getting/erasing/freezing of key pairs
 * - demonstrate key can be used through sign operations
 *
 * @param[in]  initMode       Visit the documentation of ::a71chInitModule for
 *                            more information on this parameter
 * @param[in]  appletVersion  The applet version
 *
 * @return     1 if passed.
 */
U8 exSstKeyPair(U8 initMode)
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

    SST_Index_t kpIndex;

    PRINTF("\r\n-----------\r\nStart exSstKeyPair(%s)\r\n------------\r\n", getInitModeAsString(initMode));

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

        PRINTF( "\r\nA71_SetEccKeyPair(0x%02X)\r\n", (SST_Index_t)i);
        err = A71_SetEccKeyPair((SST_Index_t) i, eccKcTls[i].pub, eccKcTls[i].pubLen,
            eccKcTls[i].priv, eccKcTls[i].privLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Read out and verify public key
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        PRINTF( "\r\nA71_GetPublicKeyEccKeyPair (0x%02X)\r\n", (SST_Index_t)i);
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

    // Fill up the hash with some reference data
    hashSha256Len = sizeof(hashSha256);
    for (i=0; i<hashSha256Len; i++) { hashSha256[i] = (U8)i; }

    // Sign a hash of correct length on the host and do the verification on the A71CH
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        signatureLen = sizeof(signature);
        PRINTF("\r\nA71_EccSign(0x%02X)\r\n", (SST_Index_t)i);
        err = A71_EccSign((SST_Index_t)i, hashSha256, hashSha256Len, signature, &signatureLen);
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
    err = A71_EraseEccKeyPair(kpIndex);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // ** Check whether erase was effective **
    PRINTF("\r\nA71_GetPublicKeyEccKeyPair (index=0x%02X)\r\n", kpIndex);
    fetchedPubKeyLen = sizeof(fetchedPubKey);
    err = A71_GetPublicKeyEccKeyPair ((SST_Index_t)kpIndex, fetchedPubKey, &fetchedPubKeyLen);
    result &= AX_CHECK_SW(err, SW_CONDITIONS_NOT_SATISFIED, "Get Public Ecc Key was supposed to fail");

    // Fill in the original keypair again
    kpIndex = A71CH_KEY_PAIR_0;
    PRINTF( "\r\nA71_SetEccKeyPair(0x%02X)\r\n", kpIndex);
    err = A71_SetEccKeyPair((SST_Index_t)kpIndex, eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
        eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");


    // Now Lock the first half of the slots for update
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX>>1; kpIndex++)
    {
        PRINTF( "\r\nA71_FreezeEccKeyPair(0x%02x)\r\n", kpIndex);
        err = A71_FreezeEccKeyPair((SST_Index_t)kpIndex);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Now fetch and compare the values with the reference values
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_GetPublicKeyEccKeyPair (0x%02x)\r\n", kpIndex);
        fetchedPubKeyLen = sizeof(fetchedPubKey);
        err = A71_GetPublicKeyEccKeyPair ((SST_Index_t)kpIndex, fetchedPubKey, &fetchedPubKeyLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        result &= AX_COMPARE_BYTE_ARRAY("eccKcTls[kpIndex]", eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
            "fetchedKey", fetchedPubKey, fetchedPubKeyLen, AX_COLON_32);
    }

    // Check whether the locked half (i.e. first half) is truly 'frozen' ....
    for (kpIndex=0; kpIndex<A71CH_KEY_PAIR_MAX>>1; kpIndex++)
    {
        PRINTF( "\r\nA71_SetEccKeyPair(0x%02x)\r\n", kpIndex);
        err = A71_SetEccKeyPair((SST_Index_t)kpIndex, eccKcAlt.pub, eccKcAlt.pubLen, eccKcAlt.priv, eccKcAlt.privLen);
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "Expected to fail, frozen credential cannot be overwritten");
    }

    // Overwrite the second half
    for (kpIndex=A71CH_KEY_PAIR_MAX>>1; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_SetEccKeyPair(0x%02x)\r\n", kpIndex);
        err = A71_SetEccKeyPair((SST_Index_t)kpIndex, eccKcAlt.pub, eccKcAlt.pubLen, eccKcAlt.priv, eccKcAlt.privLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check that the second half was overwritten, by signing a hash of correct length on the A71CH and do the verification on the Host
    for (kpIndex=A71CH_KEY_PAIR_MAX>>1; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        signatureLen = sizeof(signature);
        PRINTF("\r\nA71_EccSign(0x%02X)\r\n", (SST_Index_t)kpIndex);
        err = A71_EccSign((SST_Index_t)kpIndex, hashSha256, hashSha256Len, signature, &signatureLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Verify ... on opposite platform
        PRINTF("\r\n(Host)ECDSA_verify API with eccKeyAlt (verify signature created on A71CH).\r\n");
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_VERIFY;
        retcode = HLCRYPT_Verify(&mechInfo,(U8 *)eccKeyAlt,0,hashSha256,hashSha256Len,signature,signatureLen);
        if (retcode == HLSE_SW_OK)
        {
            PRINTF("Verification OK for eccKeyAlt.\r\n");
        }
        else
        {
            PRINTF("Return value: %d, Verification Not OK for eccKeyAlt. Test Failed!\r\n", retcode);
            result &= 0;
            break;
        }
    }

    // Put back the original (reference) values in the second half
    for (kpIndex=A71CH_KEY_PAIR_MAX>>1; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_SetEccKeyPair(0x%02X)\r\n", (SST_Index_t)kpIndex);
        err = A71_SetEccKeyPair((SST_Index_t) kpIndex, eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
            eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
    }

    // Check all keys are still fine by doing a sign/verify operation
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        signatureLen = sizeof(signature);
        PRINTF("\r\nA71_EccSign(0x%02X)\r\n", (SST_Index_t)i);
        err = A71_EccSign((SST_Index_t)i, hashSha256, hashSha256Len, signature, &signatureLen);
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

    // Now disable the plain insertion/reading out of Symmetric keys & Keypairs
    PRINTF("\r\nA71_InjectLock()\r\n");
    err = A71_InjectLock();
    result &= AX_CHECK_SW(err, SW_OK, "err");
    assert(result);

    for (kpIndex=A71CH_KEY_PAIR_MAX>>1; kpIndex<A71CH_KEY_PAIR_MAX; kpIndex++)
    {
        PRINTF( "\r\nA71_SetEccKeyPair(0x%02X)\r\n", (SST_Index_t)kpIndex);
        err = A71_SetEccKeyPair((SST_Index_t) kpIndex, eccKcTls[kpIndex].pub, eccKcTls[kpIndex].pubLen,
            eccKcTls[kpIndex].priv, eccKcTls[kpIndex].privLen);
        result &= AX_CHECK_SW(err, SW_COMMAND_NOT_ALLOWED, "err");
    }

    HOSTCRYPTO_FreeEccKey(&eccKeyAlt);
    for (i=0; i<A71CH_KEY_PAIR_MAX; i++)
    {
        HOSTCRYPTO_FreeEccKey(&eccKeyTls[i]);
    }

    PRINTF( "\r\n-----------\r\nEnd exSstKeyPair(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    return result;
}
