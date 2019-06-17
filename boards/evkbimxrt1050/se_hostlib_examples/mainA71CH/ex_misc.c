/**
 * @file ex_misc.c
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
#include "axHostCrypto.h"

static U8 exGetModuleInfo(U8 initMode);
static U8 exGetRandom(U8 initMode);
static U8 exGetSha256(void);
static U8 exSendApdu(void);
static U8 exGetUniqueID(U8 initMode);
static U8 exCredentialInfo(U8 initMode);

// #define WORKAROUND_SHA256_BUG

/**
 * Demonstrate miscelaneous module functionality:
 * - ::exGetModuleInfo
 * - ::exGetRandom
 * - ::exGetUniqueID
 * - ::exGetSha256
 * - ::exSendApdu
 * - ::exCredentialInfo
 *
 */
U8 exMisc()
{
    U8 result = 1;

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exMisc()\r\n------------\r\n");

    DEV_ClearChannelState();

    // Without channel encryption
    result &= exGetRandom(INIT_MODE_RESET);

    result &= exGetUniqueID(INIT_MODE_RESET);

    result &= exGetModuleInfo(INIT_MODE_NO_RESET);

    result &= exGetSha256();

    result &= exSendApdu();

    result &= exCredentialInfo(INIT_MODE_NO_RESET);

    // With channel encryption
    result &= exGetRandom(INIT_MODE_NO_RESET_DO_SCP03);

    result &= exGetUniqueID(INIT_MODE_NO_RESET);

    result &= exGetModuleInfo(INIT_MODE_NO_RESET);

    result &= exCredentialInfo(INIT_MODE_NO_RESET);

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exMisc(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}

/// @cond
#define BLOCKSIZE_RND_ARRAY    64
#define BLOCKS_RND_ARRAY        4
#define MAX_RND_ARRAY        (BLOCKSIZE_RND_ARRAY * BLOCKS_RND_ARRAY)
/// @endcond

/**
 * Demonstrate requesting a byte array with random data from the A71CH
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exGetRandom(U8 initMode)
{
    U8 result = 1;
    U16 err;
    U8 random[MAX_RND_ARRAY];
    int j = 0;

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exGetRandom(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    for (j=1; j<=BLOCKS_RND_ARRAY; j++)
    {
        U16 swExpected = SW_OK;
        U16 maxPayload = ( a71chScp03Requested(initMode) == 1 ) ? A71CH_SCP03_MAX_PAYLOAD_SIZE : 255;
        U8 randomLen = (U8)(j*BLOCKSIZE_RND_ARRAY - 1);
        sm_printf(CONSOLE, "A71_GetRandom(%d)\r\n", randomLen);
        err = A71_GetRandom(random, randomLen);
        swExpected = (randomLen <= maxPayload) ? SW_OK : SW_WRONG_DATA;
        result &= AX_CHECK_SW(err, swExpected, "err");
        if (err == SW_OK) { axPrintByteArray("random", random, randomLen, AX_COLON_32); }
    }

    // Run border cases
    for (j=(A71CH_SCP03_MAX_PAYLOAD_SIZE-2); j<(A71CH_SCP03_MAX_PAYLOAD_SIZE+2); j++)
    {
        U16 swExpected = SW_OK;
        U16 maxPayload = ( a71chScp03Requested(initMode) == 1 ) ? A71CH_SCP03_MAX_PAYLOAD_SIZE : 255;
        U8 randomLen = (U8)(j);
        sm_printf(CONSOLE, "A71_GetRandom(%d)\r\n", randomLen);
        err = A71_GetRandom(random, randomLen);
        swExpected = (randomLen <= maxPayload) ? SW_OK : SW_WRONG_DATA;
        result &= AX_CHECK_SW(err, swExpected, "err");
        if (err == SW_OK) { axPrintByteArray("random", random, randomLen, AX_COLON_32); }
    }

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exGetRandom(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    assert(result);
    return result;
}

/**
 * Demonstrate requesting the A71CH's unique identifier and the cert uid (which is derived from the A71CH unique identifier)
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exGetUniqueID(U8 initMode)
{
    U8 result = 1;
    U16 err;
    U8 uid[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
    U16 uidLen = sizeof(uid);
    U8 certUid[A71CH_MODULE_CERT_UID_LEN] = {0};
    U16 certUidLen = sizeof(certUid);

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exGetUniqueID(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    sm_printf(CONSOLE, "A71_GetUniqueID().\r\n");
    err = A71_GetUniqueID(uid, &uidLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("uid", uid, uidLen, AX_COLON_32);

    sm_printf(CONSOLE, "A71_GetCertUid().\r\n");
    err = A71_GetCertUid(certUid, &certUidLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray("certUid", certUid, certUidLen, AX_COLON_32);

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exGetUniqueID(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    assert(result);
    return result;
}

/**
 * Demonstrate requesting the A71CH's module info
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exGetModuleInfo(U8 initMode)
{
    U8 result = 1;
    U8 scpChannelStateDummy = 0;

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exGetModuleInfo(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= a71chInitModule(initMode);
    assert(result);

    result &= a71chShowModuleInfo(&scpChannelStateDummy);

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exGetModuleInfo(%s), result = %s\r\n------------\r\n", getInitModeAsString(initMode),
        ((result == 1)? "OK": "FAILED"));

    assert(result);
    return result;
}

/// @cond
#define BLOCKSIZE_DATA_ARRAY    64
#ifdef WORKAROUND_SHA256_BUG
#define BLOCKS_DATA_ARRAY        3
#else
#define BLOCKS_DATA_ARRAY       10
#endif
#define MAX_DATA_ARRAY          (BLOCKSIZE_DATA_ARRAY * BLOCKS_DATA_ARRAY)
/// @endcond
/**
 * Demonstrate calculating a SHA256 on the A71CH
 *
 */
static U8 exGetSha256()
{
    U8 result = 1;
    U16 err;
    U8 data[MAX_DATA_ARRAY];
    U8 shaBuf[32];
    U16 shaBufLen = sizeof(shaBuf);
    U8 shaHostBuf[32];
    U16 shaHostBufLen = sizeof(shaHostBuf);
    int j = 0;
    int i = 0;
    int nRet;

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exGetSha256()\r\n------------\r\n");

    for (j=1; j<=BLOCKS_DATA_ARRAY; j++)
    {
        U16 dataLen = (U16)(j*BLOCKSIZE_DATA_ARRAY - 1);
        for (i=0; i<dataLen; i++) { data[i] = (U8)i; }
        sm_printf(CONSOLE, "A71_GetSha256(%d)\r\n", dataLen);
        shaBufLen = sizeof(shaBuf);
        err = A71_GetSha256(data, dataLen, shaBuf, &shaBufLen);
        result &= AX_CHECK_SW(err, SW_OK, "err");

        // Also calculate SHA256 on data on host and compare both values
        shaHostBufLen = sizeof(shaHostBuf);
        nRet = HOST_SHA256_Get(data, dataLen, shaHostBuf);
        assert(nRet == HOST_CRYPTO_OK);
        if (nRet != HOST_CRYPTO_OK) {
          result &= 0;
        }

        result &= AX_COMPARE_BYTE_ARRAY("shaBuf", shaBuf, shaBufLen, "shaHostBuf", shaHostBuf, shaHostBufLen, AX_COLON_32);
    }

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exGetSha256(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    // assert(result);
    return result;

}

#define RSP_BUF_SIZE 256
#define RND_DATA_AMOUNT 0x10
/**
 * Demonstrate sending a raw APDU command
 *
 */
static U8 exSendApdu()
{
    U8 result = 1;
    U16 err;
    U8 cmdRawGetRandom[] = {0x80, 0x95, 0x00, RND_DATA_AMOUNT, 0x00};
    U16 cmdLen = 0;
    U8 rsp[RSP_BUF_SIZE];
    U16 rspLen = RSP_BUF_SIZE;

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exSendApdu()\r\n------------\r\n");

    // Assume lifecycle state of module allows command.
    memset(rsp, 0, sizeof(rsp));
    sm_printf(CONSOLE, "Fetch %d random byte.\r\n", RND_DATA_AMOUNT);
    cmdLen = sizeof(cmdRawGetRandom);
    err = SM_SendAPDU(cmdRawGetRandom, cmdLen, rsp, &rspLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray ("Response on getRandom", rsp, rspLen, AX_COLON_32);

    // Evaluate raw response
    // Length must be RND_DATA_AMOUNT + 2
    if (rspLen != (RND_DATA_AMOUNT + 2))
    {
        sm_printf(CONSOLE, "Did not receive expected amount of data (0x%02x) but (0x%02x)\r\n", RND_DATA_AMOUNT, rspLen);
        result &= 0;
        if ( (rsp[rspLen-2] != 0x90) && (rsp[rspLen-1] != 0x00) )
        {
            sm_printf(CONSOLE, "Did not receive status word 0x9000\r\n");
            result &= 0;
        }
    }

    sm_printf(CONSOLE, "\r\n-----------\r\n exSendApdu, result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * A71_GetCredentialInfo reports on the status of the credentials stored inside the A71CH.
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exCredentialInfo(U8 initMode)
{
    U8 map[256];
    U16 mapLen = sizeof(map);
    U8 result = 1;
    U16 err;

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exCredentialInfo(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH
    result &= a71chInitModule(initMode);
    assert(result);

    // Show initial credential map
    sm_printf(CONSOLE, "A71_GetCredentialInfo\r\n");
    err = A71_GetCredentialInfo(map, &mapLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    axPrintByteArray ("map", map, mapLen, AX_COLON_32);

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exCredentialInfo(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    assert(result);
    return result;
}
