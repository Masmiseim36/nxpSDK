/**
 * @file ex_psk.c
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
 * Demonstrate the pre-shared-key
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

/*******************************************************************
* global variables and struct definitions
*******************************************************************/
#define AX_TLS_LABEL_LEN       13 //!< Should this not move to project header file?
#define AX_TLS_SECRET_LEN      32 //!< Should this not move to project header file?



/**
 * Demonstrate plain or ECDH enhanced pre-shared key based master key creation:
 * - ::exPskTls1_2
 *
 * @param[in]  appletVersion  The applet version
 *
 * @return     1 if successful. Else 0.
 */
U8 exPsk()
{
    U8 result = 1;
    PRINTF( "\r\n-----------\r\nStart exPsk()\r\n------------\r\n");

    // Without channel encryption
    result &= exPskTls1_2(INIT_MODE_RESET, PLAIN_PSK);

    result &= exPskTls1_2(INIT_MODE_RESET, ECDH_PSK);

    // With channel encryption
    result &= exPskTls1_2(INIT_MODE_RESET_DO_SCP03, PLAIN_PSK);

    result &= exPskTls1_2(INIT_MODE_RESET_DO_SCP03, ECDH_PSK);

    // overall result
    PRINTF( "\r\n-----------\r\nEnd exPsk(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * This example function creates a master secret based upon
 * - all supported PSK sizes (16, 32, 48 and 64 byte)
 * - all supported offsets in the SYM keystore (from 0 upto ::A71CH_SYM_KEY_MAX-1)
 *
 * The master secret is created according to RFC5246 (TLS1.2)
 *
 * Two test modes are supported:
 * - The pre-master secret is created according to RFC4279 section 2 ( if @p pskMode == ::PLAIN_PSK)
 * - The pre-master secret is created according to RFC5489 (if @p pskMode == ::ECDH_PSK)
 *
 * @param[in]  initMode       Visit the documentation of ::a71chInitModule for
 *                            more information on this parameter
 * @param[in]  pskMode        Either ::PLAIN_PSK or ::ECDH_PSK
 * @param[in]  appletVersion  The applet version
 *
 * @return     1 if successful, 0 if failed.
 */

#ifdef __ICCARM__
#pragma optimize=none
#endif

U8 exPskTls1_2(U8 initMode, U8 pskMode)
{
    U8 result = 1;
    U16 err = SW_OK;

    const U8 aesRef[A71CH_SYM_KEY_MAX_B][16] = {
        {0x01, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56},
        {0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0x03, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0xF2, 0x87, 0x28, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0x04, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4},
        {0x05, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56},
        {0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
        {0x07, 0x79, 0xEF, 0x82, 0xCD, 0xF7, 0x12, 0xF2, 0x87, 0x28, 0xFD, 0x18, 0xED, 0xD7, 0xF2, 0xE4},
        {0x08, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4},
    };

    U8 indexAesKey = 0;

    char labelString[] = "master secret";
    U8 label[AX_TLS_LABEL_LEN] = {0};

    U8 clientHelloRandom[AX_TLS_SECRET_LEN] = {0};
    U16 clientHelloRandomLen = sizeof(clientHelloRandom);
    U8 serverHelloRandom[AX_TLS_SECRET_LEN] = {0};

    U8 labelAndSeed[AX_TLS_LABEL_LEN+2*AX_TLS_SECRET_LEN] = {0};
    U16 labelAndSeedLen = sizeof(labelAndSeed);

    U8 serverSeed[AX_TLS_SECRET_LEN] = {0};
    U16 serverSeedLen = sizeof(serverSeed);

    U8 hostPsk[A71CH_SYM_KEY_MAX*16];
    U16 hostPskLen;

    U8 premasterSecret[256];
    U16 premasterSecretLen = 0;

    U8 masterSecret[AX_TLS_PSK_MASTER_SECRET_LEN] = {0};
    U8 masterSecretHost[AX_TLS_PSK_MASTER_SECRET_LEN] = {0};
    U16 masterSecretHostLen = sizeof(masterSecretHost);

    SST_Index_t indexKp;
    eccKeyComponents_t eccKcTls_0;
    eccKeyComponents_t eccKcTls_Host;
    U8 ecdhSS[32];
    U16 ecdhSSLen = sizeof(ecdhSS);

    U8 indexOffset = 0x00;

    U8 nBlock = 1;

    int i;

    PRINTF( "\r\n-----------\r\nStart exPskTls1_2(%s, %s)\r\n------------\r\n", getInitModeAsString(initMode),
        (pskMode == 0x00) ? "PLAIN_PSK" : "ECDH_PSK");

    if ( (pskMode != PLAIN_PSK) && (pskMode != ECDH_PSK) )
    {
        PRINTF("pskMode (0x%02X) does not have a legal value.\r\n", pskMode);
        result = 0;
        return result;
    }

    DEV_ClearChannelState();
    // PRINTF("\r\nDBG_Reset()\r\n");
    // err = DBG_Reset();
    // result &= AX_CHECK_SW(err, SW_OK, "err");

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);

    // Conditionaly create and inject an ECC keypair (pskMode == ECDH_PSK)
    if (pskMode == ECDH_PSK)
    {
        ECCCurve_t eccCurve = ECCCurve_NIST_P256;
        EC_KEY *eccKeyTls_0 = NULL;
        EC_KEY *eccKeyTls_Host = NULL;

        // const U16 expectedPubKeyLen = 65;
        const U16 expectedPrivKeyLen = 32;

        err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyTls_0);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        err = HOSTCRYPTO_GenerateEccKey(eccCurve, &eccKeyTls_Host);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Break down the ECC keys generated in OpenSSL into eccKeyComponents
        err = HOSTCRYPTO_GetPublicKey(eccKeyTls_0, eccKcTls_0.pub, &(eccKcTls_0.pubLen), (64 << 1)+1);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        err = HOSTCRYPTO_GetPrivateKey(eccKeyTls_0, eccKcTls_0.priv, &(eccKcTls_0.privLen), 64);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        eccKcTls_0.bits = expectedPrivKeyLen << 3;
        eccKcTls_0.curve = eccCurve;
        HOSTCRYPTO_FreeEccKey(&eccKeyTls_0);

        err = HOSTCRYPTO_GetPublicKey(eccKeyTls_Host, eccKcTls_Host.pub, &(eccKcTls_Host.pubLen), (64 << 1)+1);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        err = HOSTCRYPTO_GetPrivateKey(eccKeyTls_Host, eccKcTls_Host.priv, &(eccKcTls_Host.privLen), 64);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        eccKcTls_Host.bits = expectedPrivKeyLen << 3;
        eccKcTls_Host.curve = eccCurve;

        // Set first ECC keyPair with eccKcTls_0 (Key pair created on Host)
        indexKp = A71CH_KEY_PAIR_0;
        PRINTF("\r\nA71_SetEccKeyPair(0x%02x)\r\n", indexKp);
        err = A71_SetEccKeyPair(indexKp, eccKcTls_0.pub, eccKcTls_0.pubLen, eccKcTls_0.priv, eccKcTls_0.privLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Create and compare a shared secret on A71CH and Host
        //   The Second ECC key pair was already set, now use it in combination with
        // eccKeyTls_0 to create a shared secret
        // index = A71CH_KEY_PAIR_1;
        // PRINTF("\r\nA71_EcdhGetSharedSecret(0x%02x) on A71CH\r\n", index);
        // sharedSecretOnA71CHLen = sizeof(sharedSecretOnA71CH);
        // err = A71_EcdhGetSharedSecret(index, eccKcTls_0.pub, eccKcTls_0.pubLen, sharedSecretOnA71CH, &sharedSecretOnA71CHLen);
        // result &= AX_CHECK_SW(err, SW_OK, "err");
        // axPrintByteArray("sharedSecretOnA71CH", sharedSecretOnA71CH, sharedSecretOnA71CHLen, AX_COLON_32);

        PRINTF("\r\nA71_EcdhGetSharedSecret() on Host\r\n");
        ecdhSSLen = sizeof(ecdhSS);
        err = HOSTCRYPTO_ECC_ComputeSharedSecret(eccKeyTls_Host, eccKcTls_0.pub, eccKcTls_0.pubLen, ecdhSS, &ecdhSSLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("sharedSecretOnHost", ecdhSS, ecdhSSLen, AX_COLON_32);
        HOSTCRYPTO_FreeEccKey(&eccKeyTls_Host);
    }

    // The premaster secret is stored in the SYM key store
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        // Write the key (unwrapped)
        PRINTF( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    // Convert ascii string to equivalent byte array - don't convert closing 0x00
    for (i=0; i<sizeof(labelString)-1; i++)
    {
        if (i >= AX_TLS_LABEL_LEN)
        {
            PRINTF("Insufficient storage for label (i=%d) totalsize=%d\r\n", i, sizeof(labelString));
            result = 0;
            break;
        }
        label[i] = (U8) labelString[i];
        PRINTF("0x%02X - %c\r\n", label[i], labelString[i]);
    }

    for (i=0; i<AX_TLS_SECRET_LEN; i++)
    {
        clientHelloRandom[i] = (U8)(0xC0 + i);
        serverHelloRandom[i] = (U8)(0x30 + i);
    }

    // REMARK: Backwards compatibility with 1.2 applet is not implemented (some elements to prepare for this are present)
    for (indexOffset = 0x00; indexOffset < A71CH_SYM_KEY_MAX; indexOffset++)
    {
        U8 nBlockMax = ( (A71CH_SYM_KEY_MAX-indexOffset) > A71CH_SYM_KEY_COMBINED_MAX ) ? A71CH_SYM_KEY_COMBINED_MAX : (A71CH_SYM_KEY_MAX-indexOffset);
        for (nBlock=1; nBlock<=nBlockMax; nBlock++)
        {
            int j;

            {
                clientHelloRandomLen = sizeof(clientHelloRandom);
                err = A71_CreateClientHelloRandom(clientHelloRandom, (U8)clientHelloRandomLen);
                result &= AX_CHECK_SW(err, SW_OK, "A71_CreateClientHelloRandom failed.");

                // Please ensure the label - as set in the A71CH - matches the value of 'label'

                memcpy(serverSeed, serverHelloRandom, AX_TLS_SECRET_LEN);
                serverSeedLen = AX_TLS_SECRET_LEN;
            }

            memcpy(labelAndSeed, label, AX_TLS_LABEL_LEN);
            memcpy(&labelAndSeed[AX_TLS_LABEL_LEN], clientHelloRandom, AX_TLS_SECRET_LEN);
            memcpy(&labelAndSeed[AX_TLS_LABEL_LEN+AX_TLS_SECRET_LEN], serverHelloRandom, AX_TLS_SECRET_LEN);
            labelAndSeedLen = sizeof(labelAndSeed);

            // Call A71_PskDeriveMasterSecret on Secure Element and compare created master secret with value returned by KDF on Host
            // ** Shared secret is nBlock * 16 byte long and located in (starts at) AES-STORE @ [A71CH_SYM_KEY_0 + indexOffset]
            indexAesKey = A71CH_SYM_KEY_0 + indexOffset;
            if (pskMode == PLAIN_PSK)
            {
                PRINTF("\r\nA71_PskDeriveMasterSecret(0x%02x, nBlock=%d, labelAndSeedLen=%d)\r\n", indexAesKey, nBlock, serverSeedLen);
                err = A71_PskDeriveMasterSecret(indexAesKey, nBlock, serverSeed, serverSeedLen, masterSecret);
                result &= AX_CHECK_SW(err, SW_OK, "A71_PskDeriveMasterSecret failed.");
            }
            else if (pskMode == ECDH_PSK)
            {
                PRINTF("\r\nA71_EcdhPskDeriveMasterSecret(0x%02x, 0x%02x, nBlock=%d, labelAndSeedLen=%d)\r\n", indexKp, indexAesKey, nBlock, serverSeedLen);
                err = A71_EcdhPskDeriveMasterSecret(indexKp, eccKcTls_Host.pub, eccKcTls_Host.pubLen, indexAesKey, nBlock,
                    serverSeed, serverSeedLen, masterSecret);
                result &= AX_CHECK_SW(err, SW_OK, "A71_PskDeriveMasterSecret failed.");
            }
            if (err == SW_OK)
            {
                axPrintByteArray("masterSecret", masterSecret, AX_TLS_PSK_MASTER_SECRET_LEN, AX_COLON_32);
            }

            // ** Calculate master secret on Host
            // *** First create pre-master secret based upon PSK
            for (j=0; j<nBlock; j++)
            {
                memcpy(&hostPsk[j*16], &aesRef[indexAesKey+j], sizeof(aesRef[indexAesKey+j]));
            }
            hostPskLen = nBlock*sizeof(aesRef[indexAesKey]);
            axPrintByteArray("hostPsk", hostPsk, nBlock*sizeof(aesRef[indexAesKey]), AX_COLON_32);
            premasterSecretLen = sizeof(premasterSecret);
            if (pskMode == PLAIN_PSK)
            {
                err = HOSTCRYPTO_TlsPskCreatePremasterSecret(hostPsk, hostPskLen, premasterSecret, &premasterSecretLen);
                result &= AX_CHECK_SW(err, SW_OK, "Failed to create premasterSecret");
            }
            else if (pskMode == ECDH_PSK)
            {
                err = HOSTCRYPTO_TlsEcdhPskCreatePremasterSecret(ecdhSS, ecdhSSLen, hostPsk, hostPskLen, premasterSecret, &premasterSecretLen);
                result &= AX_CHECK_SW(err, SW_OK, "Failed to create premasterSecret");
            }
            if (err == SW_OK) { axPrintByteArray("premasterSecret", premasterSecret, premasterSecretLen, AX_COLON_32); }
            // *** Now invoke P_SHA256
            PRINTF("\r\nHOSTCRYPTO_Tls1_2_P_Sha256(secret,secretLen=%d, ...)\r\n", premasterSecretLen);
            err = HOSTCRYPTO_Tls1_2_P_Sha256(premasterSecret, premasterSecretLen,
                labelAndSeed, labelAndSeedLen, masterSecretHost, masterSecretHostLen);
            result &= AX_CHECK_SW(err, SW_OK, "HOSTCRYPTO_Tls1_2_P_Sha256 failed");
            if (err == SW_OK)
            {
                axPrintByteArray("masterSecretHost", masterSecretHost, AX_TLS_PSK_MASTER_SECRET_LEN, AX_COLON_32);
            }

            // ** Compare both master secrets
            if (memcmp(masterSecret, masterSecretHost, AX_TLS_PSK_MASTER_SECRET_LEN) != 0 )
            {
                result = 0;

                PRINTF("\r\n***** ERROR Master secrets differ for PSKLen = %d:\r\n", nBlock*16);
                axPrintByteArray("masterSecret", masterSecret, AX_TLS_PSK_MASTER_SECRET_LEN, AX_COLON_32);
                axPrintByteArray("masterSecretHost", masterSecretHost, AX_TLS_PSK_MASTER_SECRET_LEN, AX_COLON_32);
            }
            if (result == 0)
                break;
        }
        if (result == 0)
            break;
    }

    PRINTF("\r\n-----------\r\nEnd exPskTls1_2(%s, %s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        (pskMode == 0x00) ? "PLAIN_PSK" : "ECDH_PSK",
        ((result == 1)? "OK": "FAILED") );

    return result;
}
