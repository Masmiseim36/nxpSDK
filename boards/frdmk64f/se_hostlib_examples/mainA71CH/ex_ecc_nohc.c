/**
 * @file ex_ecc_nohc.c
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
 * Demonstrate basic ECC crypto functionality without relying on ECC Host Crypto functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "a71ch_ex.h"
#include "ax_util.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"
#include "tst_a71ch_util.h"
#include "nxLog_hostLib.h"

static U8 exEccNoHostCrypto(U8 initMode);

/**
 * Demonstrate ECC crypto functionality of A71CH without relying on Host Crypto:
 * - ::exEccNoHostCrypto
 *
 */
U8 exEccNohc()
{
    U8 result = 1;
    LOG_I( "-----------Start exEccNohc()------------");

    DEV_ClearChannelState();

    // Use ECC in plain communication mode
    result &= exEccNoHostCrypto(INIT_MODE_RESET);

    // Use ECC with channel encryption on
    result &= exEccNoHostCrypto(INIT_MODE_RESET_DO_SCP03);

    // overall result
    LOG_I( "-----------End exEccNohc(), result = %s------------", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate ECC crypto functionality of A71CH without relying on Host Crypto
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exEccNoHostCrypto(U8 initMode) {
    U8 result = 1;
    U8 pubEccKey0[128];
    U16 pubEccKey0Len = 0;
    U8 pubEccKey1[128];
    U16 pubEccKey1Len = 0;
    U8 pubEccKeyScratch[128];
    U16 pubEccKeyScratchLen = 0;

    const U8 eccPubKeyTlsNist256_0[] = {
        0x04,
        0x82, 0xCB, 0xC1, 0xE5, 0x9A, 0xE2, 0x6E, 0xE8, 0xB2, 0x0C, 0x82, 0x2F, 0xE1,
        0xD4, 0x6E, 0x65, 0xA4, 0xD8, 0xA8, 0xFB, 0x5A, 0xF3, 0x79, 0x98, 0x54, 0x00, 0x5B, 0x01, 0x6D,
        0xC1, 0xA9, 0x9F, 0x40, 0xC4, 0x45, 0x8F, 0x96, 0x8F, 0xBE, 0xA0, 0x75, 0xEF, 0x49, 0x4F, 0xE8,
        0x2A, 0x18, 0xE2, 0xAD, 0xE1, 0x05, 0xD9, 0xE0, 0x95, 0x32, 0xFD, 0xFB, 0xF2, 0x74, 0x12, 0x63,
        0x60, 0xB8, 0x89,
    };

    const U8 eccPrivKeyTlsNist256_0[] = {
        0x23, 0x37, 0x76, 0x3B, 0xBC, 0xC1, 0xA0, 0x29, 0x6C, 0xF6, 0x01, 0x63, 0x56, 0xC1, 0x94, 0x9E,
        0x8D, 0x74, 0x43, 0x1A, 0xDE, 0x35, 0x7A, 0xBE, 0x27, 0x23, 0x97, 0x3E, 0xEE, 0x25, 0x63, 0x98,
    };

    const U8 eccPubKeyTlsNist256_1[] = {
        0x04, 0x9B, 0x97, 0x98, 0x9E, 0x9B, 0x19, 0x15, 0x66, 0xE0, 0xDD, 0x7D, 0x11, 0x01, 0x41, 0xFB,
        0xF3, 0x11, 0x44, 0x5E, 0x40, 0xF5, 0xC6, 0x1E, 0x5C, 0x4B, 0x0F, 0x94, 0x31, 0x14, 0x8D, 0x9F,
        0x01, 0xBA, 0x77, 0x3D, 0xEB, 0xCE, 0xA4, 0x3C, 0xD0, 0x15, 0x10, 0x2D, 0x26, 0x3B, 0xD2, 0x64,
        0x57, 0xAE, 0xD7, 0x82, 0xCA, 0xBC, 0xFE, 0xBD, 0xFB, 0x5C, 0x3C, 0x8F, 0xDA, 0xEC, 0xFD, 0x1C,
        0x1D
    };

    const U8 eccPrivKeyTlsNist256_1[] = {
        0x0D, 0xBE, 0x36, 0xF7, 0x3A, 0x56, 0xC0, 0xF6, 0xB7, 0xB9, 0xCA, 0x70, 0x76, 0x7D, 0xB4, 0x86,
        0xC5, 0xA8, 0xE1, 0x1E, 0xAB, 0x6E, 0x6E, 0x15, 0xDE, 0xE0, 0xE4, 0x9C, 0x61, 0x0D, 0x03, 0xE4
    };

    const U8 eccPubKeyCANist256_0[] = {
        0x04, 0xC7, 0x84, 0x4F, 0x17, 0x11, 0x94, 0x70, 0x33, 0x1C, 0xDD, 0x99, 0x84, 0x3B, 0xA3, 0x27,
        0x60, 0xEA, 0x30, 0x01, 0x75, 0xB6, 0xCC, 0x31, 0x1B, 0xE0, 0x39, 0x2B, 0x99, 0x24, 0xD3, 0x83,
        0x56, 0x22, 0x47, 0x62, 0x33, 0x8C, 0xE5, 0x83, 0x61, 0x14, 0x2C, 0x73, 0x01, 0x97, 0x7A, 0xAD,
        0x1D, 0x38, 0x9A, 0x7E, 0x52, 0x83, 0xBE, 0x8A, 0x0D, 0x88, 0xCC, 0x2D, 0xAB, 0x67, 0x1E, 0xE6,
        0x11
    };

//  U8 eccPrivKeyCANist256_0[] = {
//      0x93, 0x25, 0x07, 0xCE, 0x72, 0x50, 0xD6, 0x2B, 0x70, 0x43, 0xE3, 0x59, 0xDD, 0x31, 0xDB, 0x62,
//      0x11, 0x1D, 0x78, 0x1D, 0x9D, 0x6A, 0xB5, 0x68, 0x32, 0xA9, 0x32, 0x41, 0xEB, 0xAD, 0xED, 0x03
//  };

    const U8 eccPubKeyCANist256_1[] = {
        0x04, 0x03, 0x61, 0x37, 0x33, 0xEC, 0xF7, 0x42, 0x37, 0x96, 0x6F, 0x0E, 0x13, 0x83, 0x91, 0xF7,
        0x60, 0xDD, 0x61, 0x54, 0x3D, 0x68, 0xA9, 0x0F, 0xDE, 0x23, 0x4B, 0xD8, 0xC4, 0x51, 0xE8, 0x06,
        0xD7, 0xF4, 0xDE, 0x4D, 0x2C, 0x4F, 0x9D, 0x08, 0x67, 0x4C, 0xF3, 0x54, 0xAD, 0xB3, 0xCC, 0x4D,
        0xE9, 0x2E, 0xEF, 0xC4, 0xCA, 0xA8, 0x6D, 0x99, 0x71, 0xF7, 0x15, 0xC4, 0x0C, 0x60, 0x3A, 0xB9,
        0xE2
    };

//  U8 eccPrivKeyCANist256_1[] = {
//      0x88, 0xC9, 0xC5, 0x80, 0xBB, 0x5E, 0xD8, 0x61, 0x6C, 0x8E, 0xE9, 0x63, 0x64, 0x42, 0x06, 0xA2,
//      0x47, 0x9E, 0x0D, 0x16, 0xB1, 0xAE, 0xF3, 0x14, 0x98, 0x18, 0x00, 0x17, 0x2E, 0x59, 0xE1, 0xED
//  };

    const U8 eccPubKeySpareNist256_1[] = {
        0x04, 0x28, 0x93, 0x33, 0xBF, 0x94, 0xF3, 0x48, 0x83, 0xCE, 0x8E, 0x91, 0x25, 0x96, 0x04, 0xC7,
        0x66, 0xA6, 0x8F, 0xBB, 0xB5, 0x5C, 0x49, 0x13, 0x2B, 0x3D, 0x12, 0x8E, 0xD6, 0x34, 0x8F, 0x62,
        0x2B, 0x7C, 0x91, 0x39, 0x7E, 0xC6, 0x65, 0x3B, 0xCD, 0xF1, 0xEC, 0x9B, 0xBA, 0xCB, 0x33, 0xAB,
        0x6D, 0x70, 0x72, 0x93, 0x06, 0x7C, 0xCF, 0x0D, 0xFE, 0xE6, 0x95, 0x6D, 0x51, 0xD2, 0x26, 0x10,
        0x02
    };

//  U8 eccPrivKeySpareNist256_1[] = {
//      0x71, 0xEA, 0x34, 0x9B, 0x3C, 0x17, 0x3F, 0x40, 0x4F, 0xFE, 0xC1, 0xE4, 0xB5, 0xF3, 0xFD, 0xCD,
//      0x58, 0x55, 0x27, 0xF8, 0x8B, 0xA1, 0x4E, 0xCD, 0x5D, 0x3C, 0x0D, 0xF0, 0x45, 0xE0, 0xD5, 0x27
//  };

    const U8 hash[] = {
        0xAA, 0xBB, 0x34, 0x9B, 0x3C, 0x17, 0x3F, 0x40, 0x4F, 0xFE, 0xC1, 0xE4, 0xB5, 0xF3, 0xFD, 0x00,
        0xBB, 0xAA, 0x27, 0xF8, 0x8B, 0xA1, 0x4E, 0xCD, 0x5D, 0x3C, 0x0D, 0xF0, 0x45, 0xE0, 0xD5, 0x11
    };
    const U16 hashLen = sizeof(hash);

    U8 signature[128];
    U16 signatureLen = sizeof(signature);

    U16 err;
    U16 expectedPubKeyLen = 0;
    // U16 expectedPrivKeyLen = 0;

    U8 isOk = 0;
    U8 sharedSecret[256];
    U16 sharedSecretLen = 0;

    U8 sharedSecret_0_1[32];
    U16 sharedSecret_0_1_Len = sizeof(sharedSecret_0_1);

    U8 sharedSecret_1_0[32];
    U16 sharedSecret_1_0_Len = sizeof(sharedSecret_1_0);

    SST_Index_t keyIdx = 0x00;

    LOG_I( "-----------Start exEccNoHostCrypto(%s)------------", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    keyIdx = A71CH_KEY_PAIR_0;
    LOG_I( "A71_GenerateEccKeyPair(0x%02x)", keyIdx);
    err = A71_GenerateEccKeyPair(keyIdx);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    keyIdx = A71CH_KEY_PAIR_1;
    LOG_I( "A71_GenerateEccKeyPair(0x%02x)", keyIdx);
    err = A71_GenerateEccKeyPair(keyIdx);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    pubEccKey0Len = sizeof(pubEccKey0);
    expectedPubKeyLen = 65;
    keyIdx = A71CH_KEY_PAIR_0;
    LOG_I( "A71_GetPublicKeyEccKeyPair(0x%02x)", keyIdx);
    err = A71_GetPublicKeyEccKeyPair(keyIdx, pubEccKey0, &pubEccKey0Len);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(pubEccKey0Len, expectedPubKeyLen, "pubEccKey0Len");
    axPrintByteArray("pubEccKey0", pubEccKey0, pubEccKey0Len, AX_COLON_32);

    pubEccKey1Len = sizeof(pubEccKey1);
    expectedPubKeyLen = 65;
    keyIdx = A71CH_KEY_PAIR_1;
    LOG_I( "A71_GetPublicKeyEccKeyPair(0x%02x)", keyIdx);
    err = A71_GetPublicKeyEccKeyPair(keyIdx, pubEccKey1, &pubEccKey1Len);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(pubEccKey1Len, expectedPubKeyLen, "pubEccKey1Len");
    axPrintByteArray("pubEccKey1", pubEccKey1, pubEccKey1Len, AX_COLON_32);

    // Overwrite the ECC keyPair at index 0 & 1
    keyIdx = A71CH_KEY_PAIR_0;
    LOG_I("A71_SetEccKeyPair(0x%02x)", keyIdx);
    err = A71_SetEccKeyPair(keyIdx, eccPubKeyTlsNist256_0, (U16)sizeof(eccPubKeyTlsNist256_0),
        eccPrivKeyTlsNist256_0, (U16)sizeof(eccPrivKeyTlsNist256_0));
    result &= AX_CHECK_SW(err, SW_OK, "err");

    keyIdx = A71CH_KEY_PAIR_1;
    LOG_I("A71_SetEccKeyPair(0x%02x)", keyIdx);
    err = A71_SetEccKeyPair(keyIdx, eccPubKeyTlsNist256_1, (U16)sizeof(eccPubKeyTlsNist256_1),
        eccPrivKeyTlsNist256_1, (U16)sizeof(eccPrivKeyTlsNist256_1));
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Check whether public key (of keypair) was written succesfully.
    pubEccKeyScratchLen = sizeof(pubEccKeyScratch);
    expectedPubKeyLen = 65;
    keyIdx = A71CH_KEY_PAIR_0;
    LOG_I( "SST_GetPublicKeyECCKeyPair(0x%02x)", keyIdx);
    err = A71_GetPublicKeyEccKeyPair(keyIdx, pubEccKeyScratch, &pubEccKeyScratchLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(pubEccKeyScratchLen, expectedPubKeyLen, "pubEccKeyScratchLen");
    result &= AX_COMPARE_BYTE_ARRAY("pubEccKeyScratch", pubEccKeyScratch, pubEccKeyScratchLen,
        "eccPubKeyTlsNist256_0", eccPubKeyTlsNist256_0, (U16)sizeof(eccPubKeyTlsNist256_0), AX_COLON_32);

    pubEccKeyScratchLen = sizeof(pubEccKeyScratch);
    expectedPubKeyLen = 65;
    keyIdx = A71CH_KEY_PAIR_1;
    LOG_I( "SST_GetPublicKeyECCKeyPair(0x%02x)", keyIdx);
    err = A71_GetPublicKeyEccKeyPair(keyIdx, pubEccKeyScratch, &pubEccKeyScratchLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(pubEccKeyScratchLen, expectedPubKeyLen, "pubEccKeyScratchLen");
    result &= AX_COMPARE_BYTE_ARRAY("pubEccKeyScratch", pubEccKeyScratch, pubEccKeyScratchLen,
        "eccPubKeyTlsNist256_1", eccPubKeyTlsNist256_1, (U16)sizeof(eccPubKeyTlsNist256_1), AX_COLON_32);

    // Overwrite the ECC Public Key at index 0 & 1
    keyIdx = A71CH_PUBLIC_KEY_0;
    LOG_I("A71_SetEccPublicKey(0x%02x)", keyIdx);
    err = A71_SetEccPublicKey(keyIdx, eccPubKeyCANist256_0, (U16)sizeof(eccPubKeyCANist256_0));
    result &= AX_CHECK_SW(err, SW_OK, "err");

    keyIdx = A71CH_PUBLIC_KEY_1;
    LOG_I("A71_SetEccPublicKey(0x%02x)", keyIdx);
    err = A71_SetEccPublicKey(keyIdx, eccPubKeyCANist256_1, (U16)sizeof(eccPubKeyCANist256_1));
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Check whether public key at index 1 was written succesfully.
    pubEccKeyScratchLen = sizeof(pubEccKeyScratch);
    expectedPubKeyLen = 65;
    keyIdx = A71CH_PUBLIC_KEY_1;
    LOG_I( "A71_GetEccPublicKey(0x%02x)", keyIdx);
    err = A71_GetEccPublicKey(keyIdx, pubEccKeyScratch, &pubEccKeyScratchLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(pubEccKeyScratchLen, expectedPubKeyLen, "pubEccKeyScratchLen");
    result &= AX_COMPARE_BYTE_ARRAY("pubEccKeyScratch", pubEccKeyScratch, pubEccKeyScratchLen,
        "eccPubKeyCANist256_1", eccPubKeyCANist256_1, sizeof(eccPubKeyCANist256_1), AX_COLON_32);

    // Check whether public key at index 0 was written succesfully.
    pubEccKeyScratchLen = sizeof(pubEccKeyScratch);
    expectedPubKeyLen = 65;
    keyIdx = A71CH_PUBLIC_KEY_0;
    LOG_I("A71_GetEccPublicKey(0x%02x)", keyIdx);
    err = A71_GetEccPublicKey(keyIdx, pubEccKeyScratch, &pubEccKeyScratchLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(pubEccKeyScratchLen, expectedPubKeyLen, "pubEccKeyScratchLen");
    result &= AX_COMPARE_BYTE_ARRAY("pubEccKeyScratch", pubEccKeyScratch, pubEccKeyScratchLen,
        "eccPubKeyCANist256_0", eccPubKeyCANist256_0, sizeof(eccPubKeyCANist256_0), AX_COLON_32);

    // Generate a shared secret with keypair on index 0 and a spare public key
    keyIdx = A71CH_KEY_PAIR_0;
    LOG_I("A71_EcdhGetSharedSecret(0x%02x, SpareKey)", keyIdx);
    sharedSecretLen = sizeof(sharedSecret);
    err = A71_EcdhGetSharedSecret(keyIdx, eccPubKeySpareNist256_1, (U16)sizeof(eccPubKeySpareNist256_1), sharedSecret, &sharedSecretLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("sharedSecret", sharedSecret, sharedSecretLen, AX_COLON_32);

    // Generate a shared secret using the two provisioned key pairs in the two possible modes
    // Check: SharedSecret(Priv_0, Pub_1) == SharedSecret(Priv_1, Pub_0)
    keyIdx = A71CH_KEY_PAIR_0;
    LOG_I("A71_EcdhGetSharedSecret(0x%02x, Pub_1)", keyIdx);
    sharedSecret_0_1_Len = sizeof(sharedSecret_0_1);
    err = A71_EcdhGetSharedSecret(keyIdx, eccPubKeyTlsNist256_1, (U16)sizeof(eccPubKeyTlsNist256_1), sharedSecret_0_1, &sharedSecret_0_1_Len);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("sharedSecret_0_1", sharedSecret_0_1, sharedSecret_0_1_Len, AX_COLON_32);

    keyIdx = A71CH_KEY_PAIR_1;
    LOG_I("A71_EcdhGetSharedSecret(0x%02x, Pub_0)", keyIdx);
    sharedSecret_1_0_Len = sizeof(sharedSecret_1_0);
    err = A71_EcdhGetSharedSecret(keyIdx, eccPubKeyTlsNist256_0, (U16)sizeof(eccPubKeyTlsNist256_0), sharedSecret_1_0, &sharedSecret_1_0_Len);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("sharedSecret_1_0", sharedSecret_1_0, sharedSecret_1_0_Len, AX_COLON_32);

    result &= AX_COMPARE_BYTE_ARRAY("sharedSecret_0_1", sharedSecret_0_1, sharedSecret_0_1_Len,
        "sharedSecret_1_0", sharedSecret_1_0, sharedSecret_1_0_Len, AX_COLON_32);

    // Sign a Hash (retrieving the native A71CH signature format)
    keyIdx = A71CH_KEY_PAIR_0;
    signatureLen = sizeof(signature);
    LOG_I("A71_EccSign(0x%02x)", keyIdx);
    err = A71_EccSign(keyIdx, hash, hashLen, signature, &signatureLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("signature", signature, signatureLen, AX_COLON_32);

    // Now verify the signature on the A71CH
    LOG_I("A71_EccVerifyWithKey(Pub_0)");
    err = A71_EccVerifyWithKey(eccPubKeyTlsNist256_0, (U16)sizeof(eccPubKeyTlsNist256_0), hash, hashLen,
        signature, signatureLen, &isOk);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U8(isOk, 0x01, "Signature did not verify correctly");

    LOG_I( "-----------End exEccNoHostCrypto(%s), result = %s------------", getInitModeAsString(initMode), ((result == 1)? "OK": "FAILED"));
    return result;
}
