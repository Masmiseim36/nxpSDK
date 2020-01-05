/**
 * @file ex_scp.c
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
 * Set up an SCP03 channel between Host and A71CH,
 * under the assumption the HostOS may set - and also has access to - the SCP03 Base Keys.
 * In principle the binding between Host and Secure channel is only done once in the lifetime of
 * the system.
 *
 * @note In a potentially more secure setup only the bootloader has access to the base key set,
 * the bootloader hands over the session keys to the host OS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "a71ch_ex.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"
#include "tst_a71ch_util.h"
#include "sm_printf.h"
#include "axHostCrypto.h"
#include "tstHostCrypto.h"
#include "scp.h"
#include "ax_util.h"
#include "HostCryptoAPI.h"
/*******************************************************************
* global variables and struct definitions
*******************************************************************/

static U8 exAuthenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek);
static U8 exProvision(void);
static U8 exUseCrypto(void);

static EC_KEY *eccKeyTls_0 = NULL; //!< OpenSSL specific datastructure to contain keypair to be stored at index 0
static EC_KEY *eccKeyTls_1 = NULL; //!< OpenSSL specific datastructure to contain keypair to be stored at index 1
static EC_KEY *eccKeyRootCA_0 = NULL; //!< OpenSSL specific datastructure to contain keypair, whose public key is stored at index 0
static EC_KEY *eccKeyRootCA_1 = NULL; //!< OpenSSL specific datastructure to contain keypair, whose public key is stored at index 1

static eccKeyComponents_t eccKcTls_0; //!< Crypto library independent datastructure to contain keypair to be stored at index 0
static eccKeyComponents_t eccKcTls_1; //!< Crypto library independent datastructure to contain keypair to be stored at index 1
static eccKeyComponents_t eccKcRootCA_0; //!< Crypto library independent datastructure to contain keypair, whose public key is stored at index 0
static eccKeyComponents_t eccKcRootCA_1; //!< Crypto library independent datastructure to contain keypair, whose public key is stored at index 1

/// @cond
static U8 aesBasePattern[] = { 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };
static U16 aesBasePatternLen = sizeof(aesBasePattern);

static U8 aesRef[A71CH_SYM_KEY_MAX][16];
/// @endcond

static void signalFunctionCallback(ScpEvent_t event, void *context)
{
    AX_UNUSED_ARG(context);
    PRINTF("scpCallback: ");
    switch (event)
    {
        case SCP_WRONG_PADDING:
            PRINTF("Wrong padding\r\n");
        break;

        case SCP_WRONG_RESPMAC:
            PRINTF("Wrong response mac\r\n");
        break;

        case SCP_GENERIC_FAILURE:
            PRINTF("Non specified failure\r\n");
        break;

        default:
            PRINTF("Unknown event type\r\n");
        break;
    }
    return;
}

/**
* This example illustrates setting up an SCP03 channel between Host and A71CH.
* It is assumed the HostOS may select and set the SCP03 Base Keys.
*
* - Set an initial SCP03 base key set (based on random data retrieved from the SM (Secure Module))
* - Establish an SCP03 session
* - Provision the A71CH with some key material
* - Demonstrate crypto operations using the provisioned key material
*/
U8 exScp()
{
    U8 result = 1;
    U8 initMode = INIT_MODE_RESET;

    U8 scpKeyEncBase[SCP_KEY_SIZE];
    U8 scpKeyMacBase[SCP_KEY_SIZE];
    U8 scpKeyDekBase[SCP_KEY_SIZE];

    PRINTF("\r\n-----------\r\nStart exScp(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Installing Callback (this step is optional)
    SCP_Subscribe(signalFunctionCallback, NULL);

    DEV_ClearChannelState();

    sm_printf(CONSOLE, "Reset Secure Module\r\n");
    result &= a71chInitModule(initMode);

    // STAGE-0:
    // - The A71CH has been forced into the initial state through the DBG Interface
    // NOTE: The function DBG_RESET is not available in production samples
    result &= axExCreateAndSetInitialHostScpKeys(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);

    result &= exAuthenticate(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);

    // STAGE-1:
    // - A set of KNOWN (but randon) HOST Static keys has been injected into the SM
    // - The SCP03 session has been established

    result &= exProvision();

    result &= exUseCrypto();

    assert(result);

    HOSTCRYPTO_FreeEccKey(&eccKeyTls_0);
    HOSTCRYPTO_FreeEccKey(&eccKeyTls_1);
    HOSTCRYPTO_FreeEccKey(&eccKeyRootCA_0);
    HOSTCRYPTO_FreeEccKey(&eccKeyRootCA_1);

    // overall result
    PRINTF("\r\n-----------\r\nEnd exScp(%s), result = %s\r\n------------\r\n",
        getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));

    return result;
}


/**
 * The host locally stores the SCP03 base keys passed as parameters and establishes
 * the authenticated and encrypted SCP03 channel with the A71CH.
 *
 * @param[in] keyEnc IN: SCP03 base key
 * @param[in] keyMac IN: SCP03 base key
 * @param[in] keyDek IN: SCP03 base key
 */
static U8 exAuthenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek)
{
    U8 sCounter[3];
    U16 sCounterLen = sizeof(sCounter);

    U8 result = 1;
    U16 err = 0;

    PRINTF( "\r\n-----------\r\nStart exAuthenticate()\r\n------------\r\n");

    // Authenticate Channel
    sm_printf(CONSOLE, "\r\nSCP_Authenticate()\r\n");
    err = SCP_Authenticate(keyEnc, keyMac, keyDek, SCP_KEY_SIZE, sCounter, &sCounterLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(sCounterLen, 0, "Only expected when SCP03 is configured for pseudo-random challenge");

    PRINTF( "\r\n-----------\r\nEnd exAuthenticate(), result = %s\r\n------------\r\n",
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
* Demonstrate crypto operations using the provisioned key material
*/
static U8 exUseCrypto()
{
    U8 result = 1;
    U16 err;
    SST_Index_t index;
    HLSE_RET_CODE retcode;
    int nRet;
    U8 isOk = 0;
    HLSE_MECHANISM_INFO mechInfo;

    const U8 preCookedSha256[] = {
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,
        0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1
    };

    const U8 dataToSign[] = {
        0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
        0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
        0x33, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
        0x44, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
        0x55, 0x11, 0x11, 0x11
    };
    U8 dataToSignLen = 68;

    U8 hashSha256[32];
    U16 hashSha256Len = sizeof(hashSha256);

    U8 signature[256];
    U16 signatureLen = sizeof(signature);
    U8 signatureOnHost[256];
    U16 signatureOnHostLen = sizeof(signatureOnHost);
    U32 nSigLen = 0;

    U8 sharedSecretOnA71CH[32];
    U16 sharedSecretOnA71CHLen = 0;
    U8 sharedSecretOnHost[32];
    U16 sharedSecretOnHostLen = 0;
    U16 expectedSharedSecretLen = sizeof(sharedSecretOnHost);

    PRINTF( "\r\n-----------\r\nStart exUseCrypto()\r\n------------\r\n");

    // Sign a precooked hash on the A71CH with the first key pair, do the subsequent verification on the Host
    index = A71CH_KEY_PAIR_0;
    signatureLen = sizeof(signature);
    PRINTF("\r\nA71_EccSign(0x%02x) on A71CH.\r\n", index);
    err = A71_EccSign(index, preCookedSha256, sizeof(preCookedSha256), signature, &signatureLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("signature", signature, signatureLen, AX_COLON_32);

    // .. Verify on host
    PRINTF("\r\n(Host)ECDSA_verify API with eccKeyTls_0 (verify signature created on A71CH).\r\n");
    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_VERIFY;
    retcode = HLCRYPT_Verify(&mechInfo,(U8 *)eccKeyTls_0,0,(U8 *)preCookedSha256,sizeof(preCookedSha256),signature,signatureLen);
    if (retcode == HLSE_SW_OK)
    {
        PRINTF("Verification OK for eccKeyTls_0.\r\n");
    }
    else
    {
        PRINTF("Return value: %d, Verification Not OK for eccKeyTls_0. Test Failed!\r\n", retcode);
        result &= 0;
    }

    // On the Host calculate SHA256 on data, sign the hash using eccKcRootCA_0.
    hashSha256Len = sizeof(hashSha256);
    nRet = HOST_SHA256_Get(dataToSign, dataToSignLen, hashSha256);
    assert(nRet == HOST_CRYPTO_OK);
    if (nRet != HOST_CRYPTO_OK)
    {
        PRINTF("HOST_SHA256_Get operation failed.\r\n");
        result &= 0;
    }

    PRINTF("\r\n(Host)ECDSA_sign API with eccKeyRootCA_0.\r\n");
    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_SIGN;
    retcode = HLCRYPT_Sign(&mechInfo, (U8 *)eccKeyRootCA_0, 0,hashSha256, hashSha256Len, signatureOnHost, &nSigLen);
    PRINTF("ECDSA_sign returned: 0x%02X, Siglen is %ld\r\n", retcode, nSigLen);
    if (retcode != HLSE_SW_OK)
    {
        PRINTF("ECDSA_sign operation failed.\r\n");
        result &= 0;
    }
    else
    {
        axPrintByteArray("signatureOnHost", signatureOnHost, (U16)nSigLen, AX_COLON_32);
    }

    // ... do the subsequent verification on the A71CH with the matching public key object.
    index = A71CH_PUBLIC_KEY_0;
    PRINTF("\r\nA71_EccVerify(0x%02x) on A71CH.\r\n", index);
    signatureOnHostLen = (U16)nSigLen;
    isOk = 0x00;
    err = A71_EccVerify(index, hashSha256, sizeof(hashSha256), signatureOnHost, signatureOnHostLen, &isOk);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U8(isOk, 1, "Signature verification failed");
    if (isOk == 1)
    {
        PRINTF("Verification on A71CH is OK, Test Passed\r\n");
    }

    // NEGATIVE TEST: verifying the signature with the wrong public key must fail
    index = A71CH_PUBLIC_KEY_1;
    PRINTF("\r\nA71_EccVerify(0x%02x) on A71CH.\r\n", index);
    signatureOnHostLen = (U16)nSigLen;
    isOk = 0x00;
    err = A71_EccVerify(index, hashSha256, sizeof(hashSha256), signatureOnHost, signatureOnHostLen, &isOk);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U8(isOk, 0, "Negative test: Signature verification must fail");
    if (isOk == 0)
    {
        PRINTF("Negative Test Passed\r\n");
    }

    // Create and compare a shared secret on A71CH and Host
    //   The Second ECC key pair was already set, now use it in combination with
    // eccKeyTls_0 to create a shared secret
    index = A71CH_KEY_PAIR_1;
    PRINTF("\r\nA71_EcdhGetSharedSecret(0x%02x) on A71CH\r\n", index);
    sharedSecretOnA71CHLen = sizeof(sharedSecretOnA71CH);
    err = A71_EcdhGetSharedSecret(index, eccKcTls_0.pub, eccKcTls_0.pubLen, sharedSecretOnA71CH, &sharedSecretOnA71CHLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("sharedSecretOnA71CH", sharedSecretOnA71CH, sharedSecretOnA71CHLen, AX_COLON_32);

    PRINTF("\r\nA71_EcdhGetSharedSecret() on Host\r\n");
    sharedSecretOnHostLen = sizeof(sharedSecretOnHost);
    err = HOSTCRYPTO_ECC_ComputeSharedSecret(eccKeyTls_0, eccKcTls_1.pub, eccKcTls_1.pubLen, sharedSecretOnHost, &sharedSecretOnHostLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("sharedSecretOnHost", sharedSecretOnHost, sharedSecretOnHostLen, AX_COLON_32);

    if (memcmp(sharedSecretOnA71CH, sharedSecretOnHost, expectedSharedSecretLen) != 0)
    {
        PRINTF("Shared secret calculated on A71CH does not match with shared secret calculated on Host.\r\n");
        result &= 0;
    }



    PRINTF( "\r\n-----------\r\nEnd exUseCrypto(), result = %s\r\n------------\r\n",
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
* Provision the A71CH with some key material
*/
static U8 exProvision()
{
    U8 result = 1;
    U16 err = 0;

    ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    U8 pubTlsKey[256];
    U16 pubTlsKeyLen = 0;

    int indexAesKey = 0;

    const U16 expectedPubKeyLen = 65;
    const U16 expectedPrivKeyLen = 32;

    SST_Index_t index;

    PRINTF( "\r\n-----------\r\nStart exProvision()\r\n------------\r\n");

    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyTls_0);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyTls_1);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyRootCA_0);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyRootCA_1);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Break down the ECC keys generated in OpenSSL into eccKeyComponents
    err = HOSTCRYPTO_GetPublicKey(eccKeyTls_0, eccKcTls_0.pub, &(eccKcTls_0.pubLen), (64 << 1)+1);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GetPrivateKey(eccKeyTls_0, eccKcTls_0.priv, &(eccKcTls_0.privLen), 64);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    eccKcTls_0.bits = expectedPrivKeyLen << 3;
    eccKcTls_0.curve = eccCurve;

    err = HOSTCRYPTO_GetPublicKey(eccKeyTls_1, eccKcTls_1.pub, &(eccKcTls_1.pubLen), (64 << 1)+1);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GetPrivateKey(eccKeyTls_1, eccKcTls_1.priv, &(eccKcTls_1.privLen), 64);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    eccKcTls_1.bits = expectedPrivKeyLen << 3;
    eccKcTls_1.curve = eccCurve;

    err = HOSTCRYPTO_GetPublicKey(eccKeyRootCA_0, eccKcRootCA_0.pub, &(eccKcRootCA_0.pubLen), (64 << 1)+1);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GetPrivateKey(eccKeyRootCA_0, eccKcRootCA_0.priv, &(eccKcRootCA_0.privLen), 64);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    eccKcRootCA_0.bits = expectedPrivKeyLen << 3;
    eccKcRootCA_0.curve = eccCurve;

    err = HOSTCRYPTO_GetPublicKey(eccKeyRootCA_1, eccKcRootCA_1.pub, &(eccKcRootCA_1.pubLen), (64 << 1)+1);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GetPrivateKey(eccKeyRootCA_1, eccKcRootCA_1.priv, &(eccKcRootCA_1.privLen), 64);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    eccKcRootCA_1.bits = expectedPrivKeyLen << 3;
    eccKcRootCA_1.curve = eccCurve;

    // Set first ECC keyPair with eccKcTls_0 (Key pair created on Host)
    index = A71CH_KEY_PAIR_0;
    PRINTF("\r\nA71_SetEccKeyPair(0x%02x)\r\n", index);
    err = A71_SetEccKeyPair(index, eccKcTls_0.pub, eccKcTls_0.pubLen, eccKcTls_0.priv, eccKcTls_0.privLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Set second ECC keyPair with eccKcTls_1 (Key pair created on Host)
    index = A71CH_KEY_PAIR_1;
    PRINTF("\r\nA71_SetEccKeyPair(0x%02x)\r\n", index);
    err = A71_SetEccKeyPair(index, eccKcTls_1.pub, eccKcTls_1.pubLen, eccKcTls_1.priv, eccKcTls_1.privLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Both ECC Key pairs have been set, compare the public key of the first key pair with reference value
    index = A71CH_KEY_PAIR_0;
    PRINTF( "\r\nSST_GetPublicKeyECCKeyPair(0x%02x)\r\n", index);
    pubTlsKeyLen = sizeof(pubTlsKey);
    err = A71_GetPublicKeyEccKeyPair(index, pubTlsKey, &pubTlsKeyLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(pubTlsKeyLen, expectedPubKeyLen, "pubTlsKeyLength");
    // Compare retrieved value with reference value
    if (memcmp(pubTlsKey, eccKcTls_0.pub, expectedPubKeyLen) != 0)
    {
        PRINTF("Retrieved Public key does not match reference value.\r\n");
        result &= 0;
    }

    // First set the matching public key on the A71CH (index=0)
    index = A71CH_PUBLIC_KEY_0;
    PRINTF("\r\nA71_SetEccPublicKey(0x%02x)\r\n", index);
    err = A71_SetEccPublicKey(index, eccKcRootCA_0.pub, eccKcRootCA_0.pubLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Set the public key part of eccKcRootCA_1 on the A71CH (index=1)
    index = A71CH_PUBLIC_KEY_1;
    PRINTF("\r\nA71_SetEccPublicKey(0x%02x)\r\n", index);
    err = A71_SetEccPublicKey(index, eccKcRootCA_1.pub, eccKcRootCA_1.pubLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Fill the SYM key store with A71CH_SYM_KEY_MAX keys (128 bits long)
    //    The keys being written are based upon a Base pattern, with the first key written having a 0x00 byte
    // in position 0, the second key written a 0x00 byte in position 1 and so on
    // Base Pattern: 0xF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF
    // First Key  :  0x00F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF
    // Second Key :  0xF000F2F3F4F5F6F7F8F9FAFBFCFDFEFF
    // ..
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        memcpy(aesRef[indexAesKey], aesBasePattern, aesBasePatternLen);
        // Put the 0x00 marker
        aesRef[indexAesKey][indexAesKey] = (U8)0x00;
        // Write the key (unwrapped)
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    // NOTE: Reading out symmetric keys is not supported

    PRINTF( "\r\n-----------\r\nEnd exProvision(), result = %s\r\n------------\r\n",
        ((result == 1)? "OK": "FAILED"));

    return result;
}
