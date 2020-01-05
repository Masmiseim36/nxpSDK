/**
 * @file ex_walkthrough.c
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
 * Refer to \ref walkthrough for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// project specific include files
#include "ax_api.h"
#include "a71ch_ex.h"
#include "ax_util.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"
#include "tst_a71ch_util.h"
#include "nxLog_hostLib.h"

#include "axHostCrypto.h"
#include "tstHostCrypto.h"
#include "HostCryptoAPI.h"
static U8 exWtSetConfigKeys(U8 *moduleConfigKey, U8 *privateKeyConfigKey, U8 *publicKeyConfigKey);
static U8 exWtProvision(void);
static U8 exWtPrepareShipment(void);
static U8 exWtUseCrypto(void);

static EC_KEY *eccKeyRootCA_0 = NULL; //!< OpenSSL specific datastructure to contain keypair, whose public key is stored at index 0
static EC_KEY *eccKeyRootCA_1 = NULL; //!< OpenSSL specific datastructure to contain keypair, whose public key is stored at index 1

static eccKeyComponents_t eccKcTls_0; //!< Crypto library independent datastructure to contain keypair to be stored at index 0
static eccKeyComponents_t eccKcTls_1; //!< Crypto library independent datastructure to contain keypair to be stored at index 1
static eccKeyComponents_t eccKcRootCA_0; //!< Crypto library independent datastructure to contain keypair, whose public key is stored at index 0
static eccKeyComponents_t eccKcRootCA_1; //!< Crypto library independent datastructure to contain keypair, whose public key is stored at index 1

static U8 configKeyModuleLock[16] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x11, 0x40, 0x00, 0xAA, 0x40, 0x01, 0xAA};  //!< Configuration Key for Module, acting as transport key (index ::A71CH_CFG_KEY_IDX_MODULE_LOCK)
static U8 configKeyPrivateKey[16] = {0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0x00, 0x11, 0x40, 0x00, 0xAA, 0x40, 0x01, 0xAA};  //!< Configuration Key for Private Key encryption (index ::A71CH_CFG_KEY_IDX_PRIVATE_KEYS)
static U8 configKeyPublicKey[16] = {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x11, 0x40, 0x00, 0xAA, 0x40, 0x01, 0xAA};   //!< Configuration Key for Public Key encryption (index ::A71CH_CFG_KEY_IDX_PUBLIC_KEYS)

/// @cond
static U8 aesBasePattern[] = { 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };
static U16 aesBasePatternLen = sizeof(aesBasePattern);

static U8 aesRef[A71CH_SYM_KEY_MAX][16];

static U16 productDataOffset = 0x00;
static U8 gpProductData[] = {
    0xAA, 0x10, 0x00, 0xAA, 0x20, 0x01, 0xAA, 0x30, 0x00, 0xAA, 0x40, 0x00, 0xAA, 0x40, 0x01, 0xAA,
    0x50, 0x00, 0xAA, 0x60, 0x01, 0xAA, 0x70, 0x00, 0xAA, 0x80, 0x00, 0xAA, 0x90, 0x01, 0x00, 0x00 };
/// @endcond

#define USE_SCP03

/**
* Demonstrate usage of A71CH from a system integrator's perspective
* - An optional binding and setting up of an SCP03 channel between Host and A71CH (::axExCreateAndSetInitialHostScpKeys
*   and ::axExAuthenticate)
* - Provisioning step (::exWtProvision)
* - Prepare for shipment (::exWtPrepareShipment)
* - Demonstrate Crypyto usage (::exWtUseCrypto)
*
* \note The example relies on the availability of the debug reset to bring the A71CH
* into its initial state.
*
*/
U8 exWalkthrough()
{
    U8 result = 1;
    U8 initMode = INIT_MODE_RESET;

#ifdef USE_SCP03
    U8 scpKeyEncBase[SCP_KEY_SIZE];
    U8 scpKeyMacBase[SCP_KEY_SIZE];
    U8 scpKeyDekBase[SCP_KEY_SIZE];
#endif

    LOG_I("-----------Start exWalkthrough(%s)------------", getInitModeAsString(initMode));

    DEV_ClearChannelState();

    LOG_I("Initialize Secure Module");
    result &= a71chInitModule(initMode);
    // STAGE-0:
    // - The A71CH has been forced into the initial state through the DBG Interface
    // NOTE: The function DBG_RESET is not available in production samples

#ifdef USE_SCP03
    result &= axExCreateAndSetInitialHostScpKeys(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);

    result &= axExAuthenticate(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);
    // STAGE-1:
    // - A set of KNOWN (but randon) HOST Static keys has been injected into the SM
    // - The SCP03 session has been established
#endif

    // Set the configuration keys
    result &= exWtSetConfigKeys(configKeyModuleLock, configKeyPrivateKey, configKeyPublicKey);

    // Provision the A71CH
    result &= exWtProvision();

    // Prepare for end-product shipment
    result &= exWtPrepareShipment();

    // Use provisioned credentials
    result &= exWtUseCrypto();

    HOSTCRYPTO_FreeEccKey(&eccKeyRootCA_0);
    HOSTCRYPTO_FreeEccKey(&eccKeyRootCA_1);

    // overall result
    if (result == 1){
        LOG_I("-----------End exWalkthrough(%s), result = %s------------",
            getInitModeAsString(initMode),"OK");
    }
    else {
    	LOG_E("-----------End exWalkthrough(%s), result = %s------------",
    	            getInitModeAsString(initMode), "FAILED");
    }
    return result;
}

/**
 * Set config keys to enable full walkthrough
 */
static U8 exWtSetConfigKeys(U8 *moduleConfigKey, U8 *privateKeyConfigKey, U8 *publicKeyConfigKey)
{
    U8 result = 1;
    U16 err;

    U8 indexCfgKey = A71CH_CFG_KEY_IDX_MODULE_LOCK;

    LOG_I("-----------Start exWtSetConfigKeys()------------");
    // Store all configure keys (value is stored in variables with module scope)
    indexCfgKey = A71CH_CFG_KEY_IDX_MODULE_LOCK;
    LOG_I("A71_SetConfigKey(0x%02x)", indexCfgKey);
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, moduleConfigKey, 16);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyModuleLock", moduleConfigKey, 16, AX_COLON_32);

    indexCfgKey = A71CH_CFG_KEY_IDX_PRIVATE_KEYS;
    LOG_I("A71_SetConfigKey(0x%02x)", indexCfgKey);
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, privateKeyConfigKey, 16);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyPrivateKey", privateKeyConfigKey, 16, AX_COLON_32);

    indexCfgKey = A71CH_CFG_KEY_IDX_PUBLIC_KEYS;
    LOG_I("A71_SetConfigKey(0x%02x)", indexCfgKey);
    err = A71_SetConfigKey((SST_Index_t)indexCfgKey, publicKeyConfigKey, 16);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("configKeyPublicKey", publicKeyConfigKey, 16, AX_COLON_32);

    LOG_I( "-----------End exWtSetConfigKeys(), result = %s------------", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
* Demonstrate crypto operations using the provisioned key material
*/
static U8 exWtUseCrypto()
{
    U8 result = 1;
    U16 err;
    SST_Index_t index;
    HLSE_RET_CODE retcode;
    int nRet;
    U8 isOk = 0;
    int i;
    HLSE_MECHANISM_INFO mechInfo;

    U8 symSecret[16];

    U8 preCookedSha256[] = {
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,
        0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1
    };

    U8 dataToSign[] = {
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

    ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    EC_KEY *eccKeyTls_Host = NULL;
    eccKeyComponents_t eccKcTls_Host;
    EC_KEY *eccKeyTls_0 = NULL;

    // const U16 expectedPubKeyLen = 65;
    const U16 expectedPrivKeyLen = 32;

    U8 nBlock;
    U8 salt[] = {0xAE, 0xBE, 0x11, 0x22};
    U16 saltLen = sizeof(salt);
    U8 info[256];
    U16 infoLen = sizeof(info);
    U8 derivedData[128];
    U16 derivedDataLen = sizeof(derivedData);
    U8 derivedDataOnHost[128];

    U8 uid[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
    U16 uidLen = sizeof(uid);

    LOG_I( "-----------Start exWtUseCrypto()------------");

    // Sign a precooked hash on the A71CH with the first key pair, do the subsequent verification on the Host
    // ------------------------------------------------------------------------------------------------------
    index = A71CH_KEY_PAIR_0;
    LOG_I("A71_EccSign(0x%02x) on A71CH.", index);
    signatureLen = sizeof(signature);
    err = A71_EccSign(index, preCookedSha256, sizeof(preCookedSha256), signature, &signatureLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("signature", signature, signatureLen, AX_COLON_32);

    // .. now verify on host
    // First construct an OpenSSL ECC key containing the public key; we
    // We've already built the eccKcTls_0 data structure while provisioning the A71CH.
    err = HOSTCRYPTO_EccCreateOpenSslEccFromComponents(&eccKeyTls_0, &eccKcTls_0);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    LOG_I("(Host)ECDSA_verify API with eccKeyTls_0 (verify signature created on A71CH).");
    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_VERIFY;
    retcode = HLCRYPT_Verify(&mechInfo,(U8 *)eccKeyTls_0,0,preCookedSha256,sizeof(preCookedSha256),signature,signatureLen);
    if (retcode == HLSE_SW_OK)
    {
        LOG_I("Verification OK for eccKeyTls_0.");
    }
    else
    {
        LOG_E("Return value: %d, Verification Not OK for eccKeyTls_0. Test Failed!", retcode);
        result &= 0;
    }

    // Sign data on the host, do the subsequent verification on the A71CH
    // ------------------------------------------------------------------
    // On the Host calculate SHA256 on data, sign the hash using eccKcRootCA_0.
    hashSha256Len = sizeof(hashSha256);
    nRet = HOST_SHA256_Get(dataToSign, dataToSignLen, hashSha256);
    assert(nRet == HOST_CRYPTO_OK);
    if (nRet != HOST_CRYPTO_OK) {
        LOG_E("HOST_SHA256_Get operation failed.");
        result &= 0;
    }
    LOG_I("(Host)ECDSA_sign API with eccKeyRootCA_0.");
    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_ECDSA_SIGN;
    retcode = HLCRYPT_Sign(&mechInfo, (U8 *)eccKeyRootCA_0, 0,hashSha256, hashSha256Len, signatureOnHost, &nSigLen);
    LOG_I("ECDSA_sign returned: 0x%02X, Siglen is %ld", retcode, nSigLen);
    if (retcode != HLSE_SW_OK)
    {
        LOG_E("ECDSA_sign operation failed.");
        result &= 0;
    }
    else
    {
        axPrintByteArray("signatureOnHost", signatureOnHost, (U16)nSigLen, AX_COLON_32);
    }

    // ... do the subsequent verification on the A71CH with the matching public key object.
    index = A71CH_PUBLIC_KEY_0;
    LOG_I("A71_EccVerify(0x%02x) on A71CH.", index);
    signatureOnHostLen = (U16)nSigLen;
    isOk = 0x00;
    err = A71_EccVerify(index, hashSha256, sizeof(hashSha256), signatureOnHost, signatureOnHostLen, &isOk);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U8(isOk, 1, "Signature verification failed");
    if (isOk == 1)
    {
        LOG_I("Verification on A71CH is OK, Test Passed");
    }

    // NEGATIVE TEST: verifying the signature with the wrong public key must fail
    index = A71CH_PUBLIC_KEY_1;
    LOG_I("A71_EccVerify(0x%02x) on A71CH.", index);
    signatureOnHostLen = (U16)nSigLen;
    isOk = 0x00;
    err = A71_EccVerify(index, hashSha256, sizeof(hashSha256), signatureOnHost, signatureOnHostLen, &isOk);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U8(isOk, 0, "Negative test: Signature verification must fail");
    if (isOk == 0)
    {
        LOG_I("Negative Test Passed");
    }

    // Create and compare a shared secret on A71CH and Host
    // ----------------------------------------------------
    //   The Second ECC key pair was generated during provisioning, now use it in combination with
    // eccKeyTls_Host to create a shared secret

    // First create a new keypair on the host.
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyTls_Host);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    // Break down the ECC key generated in OpenSSL into eccKeyComponents
    err = HOSTCRYPTO_GetPublicKey(eccKeyTls_Host, eccKcTls_Host.pub, &(eccKcTls_Host.pubLen), (64 << 1)+1);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GetPrivateKey(eccKeyTls_Host, eccKcTls_Host.priv, &(eccKcTls_Host.privLen), 64);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    eccKcTls_Host.bits = expectedPrivKeyLen << 3;
    eccKcTls_Host.curve = eccCurve;

    index = A71CH_KEY_PAIR_1;
    LOG_I("A71_EcdhGetSharedSecret(0x%02x) on A71CH", index);
    sharedSecretOnA71CHLen = sizeof(sharedSecretOnA71CH);
    err = A71_EcdhGetSharedSecret(index, eccKcTls_Host.pub, eccKcTls_Host.pubLen, sharedSecretOnA71CH, &sharedSecretOnA71CHLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("sharedSecretOnA71CH", sharedSecretOnA71CH, sharedSecretOnA71CHLen, AX_COLON_32);

    LOG_I("A71_EcdhGetSharedSecret() on Host");
    sharedSecretOnHostLen = sizeof(sharedSecretOnHost);
    err = HOSTCRYPTO_ECC_ComputeSharedSecret(eccKeyTls_Host, eccKcTls_1.pub, eccKcTls_1.pubLen, sharedSecretOnHost, &sharedSecretOnHostLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("sharedSecretOnHost", sharedSecretOnHost, sharedSecretOnHostLen, AX_COLON_32);

    result &= AX_COMPARE_BYTE_ARRAY( "sharedSecretOnA71CH", sharedSecretOnA71CH, expectedSharedSecretLen,
        "sharedSecretOnHost", sharedSecretOnHost, expectedSharedSecretLen, AX_COLON_32);

    // Use the stored symmetric secret on index A71CH_SYM_KEY_0 to derive a secret from
    // --------------------------------------------------------------------------------
    // - salt = 0xAEBE1122
    // - info = uid | 0x00 0x01 0x02 0x03 0x04

    // Fetch UID from A71CH
    err = A71_GetUniqueID(uid, &uidLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    memcpy(info, uid, uidLen);
    for (i=0; i<5; i++) { info[uidLen+i] = (U8)i; }
    infoLen = uidLen + 5;
    nBlock = 1;
    derivedDataLen = 16;

    LOG_I("A71_HkdfSymKey() on Host");
    err = A71_HkdfSymKey(A71CH_SYM_KEY_0, nBlock,
        salt, saltLen,
        info,  infoLen,
        derivedData, derivedDataLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("derivedData from A71CH", derivedData, derivedDataLen, AX_COLON_32);

    // Assuming e.g. the backend knows the shared secret (the stored symmetric secret) it can
    // calculate the same derived data and use this
    memcpy(symSecret, aesBasePattern, 16);
    symSecret[0] = 0x00;
    LOG_I("HOSTCRYPTO_HkdfFullSha256(KeyLen=%d, infoLen=%d, derivedDataLen=%d)", 16, infoLen, derivedDataLen);
    err = HOSTCRYPTO_HkdfFullSha256(salt, saltLen, symSecret, 16, info, infoLen, derivedDataOnHost, derivedDataLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    result &= AX_COMPARE_BYTE_ARRAY( "derivedData from A71CH", derivedData, derivedDataLen,
        "derivedData calculated outside secure module", derivedDataOnHost, derivedDataLen, AX_COLON_32);

    HOSTCRYPTO_FreeEccKey(&eccKeyTls_Host);
    HOSTCRYPTO_FreeEccKey(&eccKeyTls_0);

    LOG_I( "-----------End exWtUseCrypto(), result = %s------------",
        ((result == 1)? "OK": "FAILED"));

    return result;
}


/**
* Provision the A71CH with key material
*/
static U8 exWtProvision()
{
    U8 result = 1;
    U16 err = 0;

    ECCCurve_t eccCurve = ECCCurve_NIST_P256;

    int indexAesKey = 0;

    const U16 expectedPubKeyLen = 65;
    const U16 expectedPrivKeyLen = 32;

    SST_Index_t index;

    U8 uid[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
    U16 uidLen = sizeof(uid);

    LOG_I( "-----------Start exWtProvision()------------");

    // In a typical provisioning scenario the keypair matching the public key
    // to be inserted would be created and stored securely on a back-end system.
    // It's also possible to create the keypairs to be contained in the A71CH externally
    // and to insert both private and public key during the provisioning phase. In this
    // example the keypairs are generated on the A71CH itself and the private key never
    // leaves the A71CH.
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyRootCA_0);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyRootCA_1);
    result &= AX_CHECK_SW(err, SW_OK, "err");

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

    // Generate the first ECC keyPair (Extract the public key)
    index = A71CH_KEY_PAIR_0;
    LOG_I("A71_GenerateEccKeyPair(0x%02x)", index);
    err = A71_GenerateEccKeyPair(index);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Generate the second ECC keyPair (Extract the public key)
    index = A71CH_KEY_PAIR_1;
    LOG_I("A71_GenerateEccKeyPair(0x%02x)", index);
    err = A71_GenerateEccKeyPair(index);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Both ECC Key pairs have been created, now extract the value of the public key
    index = A71CH_KEY_PAIR_0;
    LOG_I( "SST_GetPublicKeyECCKeyPair(0x%02x)", index);
    eccKcTls_0.pubLen = sizeof(eccKcTls_0.pub);
    err = A71_GetPublicKeyEccKeyPair(index, eccKcTls_0.pub, &(eccKcTls_0.pubLen));
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(eccKcTls_0.pubLen, expectedPubKeyLen, "Wrong public key length");
    // Complete eccKcTls_0 datastructure
    if (err == SW_OK)
    {
        eccKcTls_0.bits = 256;
        eccKcTls_0.curve = eccCurve;
        eccKcTls_0.privLen = 0;
    }

    index = A71CH_KEY_PAIR_1;
    LOG_I( "SST_GetPublicKeyECCKeyPair(0x%02x)", index);
    eccKcTls_1.pubLen = sizeof(eccKcTls_1.pub);
    err = A71_GetPublicKeyEccKeyPair(index, eccKcTls_1.pub, &(eccKcTls_1.pubLen));
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(eccKcTls_1.pubLen, expectedPubKeyLen, "Wrong public key length");
    // Complete eccKcTls_1 datastructure
    if (err == SW_OK)
    {
        eccKcTls_1.bits = 256;
        eccKcTls_1.curve = eccCurve;
        eccKcTls_1.privLen = 0;
    }

    // Set the public key of eccKcRootCA_0 on the A71CH (index=0)
    index = A71CH_PUBLIC_KEY_0;
    LOG_I("A71_SetEccPublicKey(0x%02x)", index);
    err = A71_SetEccPublicKey(index, eccKcRootCA_0.pub, eccKcRootCA_0.pubLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Set the public key of eccKcRootCA_1 on the A71CH (index=1)
    index = A71CH_PUBLIC_KEY_1;
    LOG_I("A71_SetEccPublicKey(0x%02x)", index);
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
        LOG_I( "A71_SetSymKey(0x%02x)", indexAesKey);
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    // Write end-product specific data into GP storage.
    // This data can be end-product trim data, configuration data, ...
    // The host or the backend can encrypt with a key derived from a a key in the SYM key store before
    // storing the data in GP storage.
    err = A71_SetGpData(productDataOffset, gpProductData, (U16) sizeof(gpProductData));
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Fetch UID from A71CH
    err = A71_GetUniqueID(uid, &uidLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Possible additional steps
    // - Create Certificate Signing Request
    // - Store a certificate (or the hash of a certificate) in GP storage
    // - Store public keys in GP storage

    LOG_I( "-----------End exWtProvision(), result = %s------------",
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
* Before shipping the end-product partially disable provisioning capabilities
*/
static U8 exWtPrepareShipment()
{
    U8 result = 1;
    U16 sw = 0;

    SST_Index_t index;

    LOG_I( "-----------Start exWtPrepareShipment()------------");

    sw = A71_InjectLock();
    result &= AX_CHECK_SW(sw, SW_OK, "Unable to lock");

    // In this specific instance, and as an example only, we also:
    // - Freeze the first keypair
    // - Freeze the first public key
    // - Freeze the first symmetric secret (Only for applet < 0x0130)
    index = A71CH_KEY_PAIR_0;
    sw = A71_FreezeEccKeyPairWithChallenge(index, configKeyPrivateKey, sizeof(configKeyPrivateKey));
    result &= AX_CHECK_SW(sw, SW_OK, "Unable to freeze key pair");

    index = A71CH_PUBLIC_KEY_0;
    sw = A71_FreezeEccPublicKeyWithChallenge(index, configKeyPublicKey, sizeof(configKeyPublicKey));
    result &= AX_CHECK_SW(sw, SW_OK, "Unable to freeze public key");

    LOG_I( "-----------End exWtPrepareShipment(), result = %s------------",
        ((result == 1)? "OK": "FAILED"));

    return result;
}
