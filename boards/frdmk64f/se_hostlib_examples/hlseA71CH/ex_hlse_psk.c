/**
 * @file ex_hlse_psk.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 * @par Description
 * Demonstrate the pre-shared-key
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

#include "tstHostCrypto.h"

#include "tst_a71ch_util.h"
#include "tst_hlse_a71ch_util.h"
#include "HLSEAPI.h"

/*******************************************************************
* global variables and struct definitions
*******************************************************************/
#define AX_TLS_LABEL_LEN       13 //!< Should this not move to project header file?
#define AX_TLS_SECRET_LEN      32 //!< Should this not move to project header file?

#define PLAIN_PSK 0x00 //!< Select RFC4279 example mode
#define ECDH_PSK  0x01 //!< Select RFC5489 example mode

static U8 exPskTls1_2(U8 initMode, U8 pskMode);

/**
 * Demonstrate plain or ECDH enhanced pre-shared key based master key creation:
 * - ::exPskTls1_2
 *
 */
U8 exHlsePsk()
{
    U8 result = 1;
    printf( "\r\n-----------\r\nStart exPsk()\r\n------------\r\n");

    // Without channel encryption
    result &= exPskTls1_2(INIT_MODE_RESET, PLAIN_PSK);

    result &= exPskTls1_2(INIT_MODE_RESET, ECDH_PSK);

    // With channel encryption
    result &= exPskTls1_2(INIT_MODE_RESET_DO_SCP03, PLAIN_PSK);

    result &= exPskTls1_2(INIT_MODE_RESET_DO_SCP03, ECDH_PSK);

    // overall result
    printf( "\r\n-----------\r\nEnd exPsk(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

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
 * - The pre-master secret is created according to RFC4279 section 2 ( if \p pskMode == ::PLAIN_PSK)
 * - The pre-master secret is created according to RFC5489 (if \p pskMode == ::ECDH_PSK)
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 * @param[in] pskMode Either ::PLAIN_PSK or ::ECDH_PSK
 */


#ifdef __ICCARM__
#pragma optimize=none
#endif

static U8 exPskTls1_2(U8 initMode, U8 pskMode)
{
    U8 result = 1;
    U16 err = 0; /* by default, Failed */

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
    U8 label[AX_TLS_LABEL_LEN];

    U8 clientHelloRandom[AX_TLS_SECRET_LEN];
    U16 clientHelloRandomLen = sizeof(clientHelloRandom);
    U8 serverHelloRandom[AX_TLS_SECRET_LEN];

    U8 labelAndSeed[AX_TLS_LABEL_LEN+2*AX_TLS_SECRET_LEN];
    U16 labelAndSeedLen = sizeof(labelAndSeed);

    U8 serverSeed[AX_TLS_SECRET_LEN];
    U16 serverSeedLen = sizeof(serverSeed);

    U8 hostPsk[A71CH_SYM_KEY_MAX*16];
    U16 hostPskLen;

    U8 premasterSecret[256];
    U16 premasterSecretLen = 0;

    U8 masterSecret[AX_TLS_PSK_MASTER_SECRET_LEN];
    U8 masterSecretHost[AX_TLS_PSK_MASTER_SECRET_LEN];
    U16 masterSecretHostLen = sizeof(masterSecretHost);

    SST_Index_t indexKp;
    eccKeyComponents_t eccKcTls_0;
    eccKeyComponents_t eccKcTls_Host;
    U8 ecdhSS[32];
    U16 ecdhSSLen = sizeof(ecdhSS);

    U8 indexOffset = 0x00;

    U8 nBlock = 1;

    int i;

    HLSE_OBJECT_HANDLE handleEccKeyPair;
    HLSE_OBJECT_HANDLE aesKeyHandles[A71CH_SYM_KEY_MAX];

    printf( "\r\n-----------\r\nStart exPskTls1_2(%s, %s)\r\n------------\r\n", getInitModeAsString(initMode),
        (pskMode == 0x00) ? "PLAIN_PSK" : "ECDH_PSK");

    if ( (pskMode != PLAIN_PSK) && (pskMode != ECDH_PSK) )
    {
        printf("pskMode (0x%02X) does not have a legal value.\r\n", pskMode);
        result = 0;
        return result;
    }

    DEV_ClearChannelState();
    // printf("\r\nDBG_Reset()\r\n");
    // err = DBG_Reset();
    // result &= AX_CHECK_SW(err, SW_OK, "err");

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);

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

        err = HOSTCRYPTO_GetPublicKey(eccKeyTls_Host, eccKcTls_Host.pub, &(eccKcTls_Host.pubLen), (64 << 1)+1);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        err = HOSTCRYPTO_GetPrivateKey(eccKeyTls_Host, eccKcTls_Host.priv, &(eccKcTls_Host.privLen), 64);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        eccKcTls_Host.bits = expectedPrivKeyLen << 3;
        eccKcTls_Host.curve = eccCurve;

        // Set first ECC keyPair with eccKcTls_0 (Key pair created on Host)
        indexKp = A71CH_KEY_PAIR_0;
        printf("\r\nA71_SetEccKeyPair(0x%02x)\r\n", indexKp);
#if 0
        err = A71_SetEccKeyPair(indexKp, eccKcTls_0.pub, eccKcTls_0.pubLen, eccKcTls_0.priv, eccKcTls_0.privLen);
#else
        err = hlse_SetEccKeyPair(indexKp, eccKcTls_0.pub, eccKcTls_0.pubLen, eccKcTls_0.priv, eccKcTls_0.privLen,
            &handleEccKeyPair);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Create and compare a shared secret on A71CH and Host
        //   The Second ECC key pair was already set, now use it in combination with
        // eccKeyTls_0 to create a shared secret
        // index = A71CH_KEY_PAIR_1;
        // printf("\r\nA71_EcdhGetSharedSecret(0x%02x) on A71CH\r\n", index);
        // sharedSecretOnA71CHLen = sizeof(sharedSecretOnA71CH);
        // err = A71_EcdhGetSharedSecret(index, eccKcTls_0.pub, eccKcTls_0.pubLen, sharedSecretOnA71CH, &sharedSecretOnA71CHLen);
        // result &= AX_CHECK_SW(err, SW_OK, "err");
        // axPrintByteArray("sharedSecretOnA71CH", sharedSecretOnA71CH, sharedSecretOnA71CHLen, AX_COLON_32);

        printf("\r\nA71_EcdhGetSharedSecret() on Host\r\n");
        ecdhSSLen = sizeof(ecdhSS);
        err = HOSTCRYPTO_ECC_ComputeSharedSecret(eccKeyTls_Host, eccKcTls_0.pub, eccKcTls_0.pubLen, ecdhSS, &ecdhSSLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("sharedSecretOnHost", ecdhSS, ecdhSSLen, AX_COLON_32);

        HOSTCRYPTO_FreeEccKey(&eccKeyTls_0);
        HOSTCRYPTO_FreeEccKey(&eccKeyTls_Host);
    }

    // The premaster secret is stored in the SYM key store
    for (indexAesKey=0; indexAesKey<A71CH_SYM_KEY_MAX; indexAesKey++)
    {
        // Write the key (unwrapped)
        printf( "\r\nA71_SetSymKey(0x%02x)\r\n", indexAesKey);
#if 0
        err = A71_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]));
#else
        err = hlse_SetSymKey((SST_Index_t)indexAesKey, aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), &aesKeyHandles[indexAesKey]);
#endif
        result &= AX_CHECK_SW(err, SW_OK, "err");
        axPrintByteArray("aesRef[indexAesKey]", aesRef[indexAesKey], sizeof(aesRef[indexAesKey]), AX_COLON_32);
    }

    // Convert ascii string to equivalent byte array - don't convert closing 0x00
    for (i=0; i<sizeof(labelString)-1; i++)
    {
        if (i >= AX_TLS_LABEL_LEN)
        {
            printf("Insufficient storage for label (i=%d) totalsize=%d\r\n", i, sizeof(labelString));
            result = 0;
            break;
        }
        label[i] = (U8) labelString[i];
        printf("0x%02X - %c\r\n", label[i], labelString[i]);
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

            clientHelloRandomLen = sizeof(clientHelloRandom);
            err = A71_CreateClientHelloRandom(clientHelloRandom, (U8)clientHelloRandomLen);
            result &= AX_CHECK_SW(err, SW_OK, "A71_CreateClientHelloRandom failed.");

            // Please ensure the label - as set in the A71CH - matches the value of 'label'

            memcpy(serverSeed, serverHelloRandom, AX_TLS_SECRET_LEN);
            serverSeedLen = AX_TLS_SECRET_LEN;

            memcpy(labelAndSeed, label, AX_TLS_LABEL_LEN);
            memcpy(&labelAndSeed[AX_TLS_LABEL_LEN], clientHelloRandom, AX_TLS_SECRET_LEN);
            memcpy(&labelAndSeed[AX_TLS_LABEL_LEN+AX_TLS_SECRET_LEN], serverHelloRandom, AX_TLS_SECRET_LEN);
            labelAndSeedLen = sizeof(labelAndSeed);

            // Call A71_PskDeriveMasterSecret on Secure Element and compare created master secret with value returned by KDF on Host
            // ** Shared secret is nBlock * 16 byte long and located in (starts at) AES-STORE @ [A71CH_SYM_KEY_0 + indexOffset]
            indexAesKey = A71CH_SYM_KEY_0 + indexOffset;
            if (pskMode == PLAIN_PSK)
            {
                printf("\r\nA71_PskDeriveMasterSecret(0x%02x, nBlock=%d, serverSeedLen=%d)\r\n", indexAesKey, nBlock, serverSeedLen);
#if 0
                err = A71_PskDeriveMasterSecret(indexAesKey, nBlock, serverSeed, serverSeedLen, masterSecret);
#else
                {
                    HLSE_TLS_PSK_MASTER_KEY_DERIVE_PARAMS hlseParams;
                    HLSE_MECHANISM_INFO mechInfo;
                    U16 outMasterSecretLen;

                    memset(&hlseParams, 0, sizeof(hlseParams));
                    hlseParams.nBlock = nBlock;
                    hlseParams.pSeed = serverSeed;
                    hlseParams.ulSeedLen = serverSeedLen;

                    memset(&mechInfo, 0, sizeof(mechInfo));
                    mechInfo.mechanism = HLSE_TLS_PSK_MASTER_KEY_DERIVE;
                    mechInfo.pParameter = &hlseParams;
                    mechInfo.ulParameterLen = sizeof(hlseParams);

                    outMasterSecretLen = sizeof(masterSecret);
                    err = HLSE_DeriveKey(&mechInfo, aesKeyHandles[indexAesKey], masterSecret, &outMasterSecretLen);
                }
#endif
                result &= AX_CHECK_SW(err, SW_OK, "A71_PskDeriveMasterSecret failed.");
            }
            else if (pskMode == ECDH_PSK)
            {
                printf("\r\nA71_EcdhPskDeriveMasterSecret(0x%02x, 0x%02x, nBlock=%d, serverSeedLen=%d)\r\n", indexKp, indexAesKey, nBlock, serverSeedLen);
#if 0
                err = A71_EcdhPskDeriveMasterSecret(indexKp, eccKcTls_Host.pub, eccKcTls_Host.pubLen, indexAesKey, nBlock, labelAndSeed, labelAndSeedLen, masterSecret);
#else
                {
                    HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC_PARAMS hlseParams;
                    HLSE_MECHANISM_INFO mechInfo;
                    U16 outMasterSecretLen;

                    memset(&hlseParams, 0, sizeof(hlseParams));
                    hlseParams.numBlocks = nBlock;
                    hlseParams.pPublicKey = eccKcTls_Host.pub;
                    hlseParams.ulPublicKeyLen = eccKcTls_Host.pubLen;
                    hlseParams.pSeed = serverSeed;
                    hlseParams.ulSeedLen = serverSeedLen;
                    hlseParams.symKeyHandle = aesKeyHandles[indexAesKey];


                    memset(&mechInfo, 0, sizeof(mechInfo));
                    mechInfo.mechanism = HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC;
                    mechInfo.pParameter = &hlseParams;
                    mechInfo.ulParameterLen = sizeof(hlseParams);

                    outMasterSecretLen = sizeof(masterSecret);
                    err = HLSE_DeriveKey(&mechInfo, handleEccKeyPair /*aesKeyHandles[indexAesKey]*/, masterSecret, &outMasterSecretLen);
                }
#endif
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
            printf("\r\nHOSTCRYPTO_Tls1_2_P_Sha256(secret,secretLen=%d, ...)\r\n", premasterSecretLen);
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

                printf("\r\n***** ERROR Master secrets differ for PSKLen = %d:\r\n", nBlock*16);
                axPrintByteArray("masterSecret", masterSecret, AX_TLS_PSK_MASTER_SECRET_LEN, AX_COLON_32);
                axPrintByteArray("masterSecretHost", masterSecretHost, AX_TLS_PSK_MASTER_SECRET_LEN, AX_COLON_32);
            }
        }
    }

    printf("\r\n-----------\r\nEnd exPskTls1_2(%s, %s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        (pskMode == 0x00) ? "PLAIN_PSK" : "ECDH_PSK",
        ((result == 1)? "OK": "FAILED") );

    return result;
}
