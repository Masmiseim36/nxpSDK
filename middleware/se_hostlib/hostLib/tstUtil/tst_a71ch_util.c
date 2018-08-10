/**
 * @file tst_a71ch_util.c
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
 *
 * @par Description
 * This module implements test bench utility functions specific to the a71ch
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
#include "ax_util.h"
#include "a71_debug.h"
#include "tstHostCrypto.h"
#include "global_platf.h"

/**
 * Initializes the A71CH module.
 * All initialization states are only reachable when the A71CH is in Debug Mode
 *
 * \param[in] initMode Either ::INIT_MODE_RESET, ::INIT_MODE_RESET_SELECT, ::INIT_MODE_NO_RESET,
 *    ::INIT_MODE_RESET_DO_SCP03, ::INIT_MODE_RESET_SELECT_DO_SCP03 or ::INIT_MODE_NO_RESET_DO_SCP03
 * \retval 0 failure
 * \retval 1 success
 */
U8 a71chInitModule(U8 initMode)
{
    U8 result = 1;
    U16 err = 0;
    U8 resetMode = 0;
    U8 scp03Mode = 0;

    U8 appletName[] = { 0x61, 0x37, 0x31, 0x63, 0x68 };
    U16 appletNameLen = sizeof(appletName);
    U8 response[256];
    U16 responseLen = sizeof(response);

    printf("\na71chInitModule(%s)\n", getInitModeAsString(initMode));

    resetMode = initMode & INIT_MODE_RESET_MASK;

    switch(resetMode)
    {
    case INIT_MODE_PATTERN_RESET:
        printf("Reset A71CH.\n");
        err = A71_DbgReset();
        result &= AX_CHECK_SW(err, SW_OK, "Failed to reset module");
        break;
    case INIT_MODE_PATTERN_RESET_SELECT:
        printf("Reset A71CH.\n");
        err = A71_DbgReset();
        result &= AX_CHECK_SW(err, SW_OK, "Failed to reset module");
        printf("Select applet.\n");
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
        result = a71chSetupScp03();
        break;
    }

    return result;
}

/**
 * Determine whether the initMode implies setting up an SCP03 channel.
 *
 * \param[in] initMode Either ::INIT_MODE_RESET, ::INIT_MODE_RESET_SELECT, ::INIT_MODE_NO_RESET,
 *    ::INIT_MODE_RESET_DO_SCP03, ::INIT_MODE_RESET_SELECT_DO_SCP03 or ::INIT_MODE_NO_RESET_DO_SCP03
 * \retval 0 No it does not imply an SCP03 channel
 * \retval 1 Yes it implies an SCP03 channel
 */
int a71chScp03Requested(U8 initMode)
{
    int scpRequested = 1;

    U8 scp03Mode = initMode & INIT_MODE_SCP03_MASK;

    switch(scp03Mode)
    {
    case INIT_MODE_PATTERN_PLAIN_COM:
        scpRequested = 0;
        break;
    case INIT_MODE_PATTERN_DO_SCP03:
        scpRequested = 1;
        break;
    }

    return scpRequested;
}

/**
 * Return the string corresponding to the initMode passed as parameter
 *
 * \param[in] initMode Either ::INIT_MODE_RESET, ::INIT_MODE_RESET_SELECT, ::INIT_MODE_NO_RESET,
 *    ::INIT_MODE_RESET_DO_SCP03, ::INIT_MODE_RESET_SELECT_DO_SCP03 or ::INIT_MODE_NO_RESET_DO_SCP03
 * \returns initMode as a string or "not defined"
 */
const char* getInitModeAsString(U8 initMode)
{
    if (initMode == INIT_MODE_NO_RESET)
    {
        return "No init action";
    }
    else if (initMode == INIT_MODE_RESET)
    {
        return "Reset";
    }
    else if (initMode == INIT_MODE_RESET_SELECT)
    {
        return "Reset + Select applet";
    }
    else if (initMode == INIT_MODE_RESET_DO_SCP03)
    {
        return "Reset + Setup SCP03 channel";
    }
    else if (initMode == INIT_MODE_RESET_SELECT_DO_SCP03)
    {
        return "Reset + Select applet + Setup SCP03 channel";
    }
    else if (initMode == INIT_MODE_NO_RESET_DO_SCP03)
    {
        return "Setup SCP03 channel";
    }
    else
    {
        printf("initMode not defined\n");
        assert(0);
        return "not defined";
    }
}

/**
 * Establish an SCP03 channel with random keys.
 * Best called implicitly via ::a71chInitModule
 *
 * \retval 0 failure
 * \retval 1 success
 */
U8 a71chSetupScp03()
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

    printf( "\n-----------\nStart a71chSetupScp03()\n------------\n");

    DEV_ClearChannelState();

    // Security module generates random data for initial SCP03 keys
    sm_printf(CONSOLE, "\nA71_GetRandom(randomLen=%d)\n", randomLen);
    err = A71_GetRandom(random, randomLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    if (result != 1) { goto SCP03_EXIT; }

    // Storing Static Keys
    memcpy(keyEnc, random, SCP_KEY_SIZE);
    memcpy(keyMac, random + SCP_KEY_SIZE, SCP_KEY_SIZE);
    memcpy(keyDek, random + (2*SCP_KEY_SIZE), SCP_KEY_SIZE);

    keyVersion = (U8) (SST_HOST_SCP_KEYSET >> 8);
    sm_printf(CONSOLE, "\nSCP_GP_PutKeys(keyVersion=0x%02X)\n", keyVersion);
    err = SCP_GP_PutKeys(keyVersion, keyEnc, keyMac, keyDek, currentKeyDek, AES_KEY_LEN_nBYTE);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    if (result != 1) { goto SCP03_EXIT; }

    // Authenticate Channel
    sm_printf(CONSOLE, "\nSCP_Authenticate()\n");
    err = SCP_Authenticate(keyEnc, keyMac, keyDek, SCP_KEY_SIZE, sCounter, &sCounterLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(sCounterLen, 0, "Only expected when SCP03 is configured for pseudo-random challenge");

SCP03_EXIT:
    printf( "\n-----------\nEnd a71chSetupScp03(), result = %s\n------------\n",
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
U8 a71chShowModuleInfo(U8 *scpState)
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
    err = A71_GetModuleInfo(&selectResponse, &debugOn, &restrictedKpIdx, &transportLockState, scpState, &injectLockState, &gpStorageSize);
    result &= AX_CHECK_SW(err, SW_OK, "Failed to retrieve module info.");

    if (err == SW_OK)
    {
        printf("A71CH in %s\n", (debugOn == 0) ? "Production Version" : "Debug Mode Version");
        printf("selectResponse:  0x%04X\n", selectResponse);
        if (restrictedKpIdx != A71CH_NO_RESTRICTED_KP)
        {
            printf("restricted keypair index: 0x%02X\n", restrictedKpIdx);
        }
        printf("transportLockState: 0x%02X (%s)\n", transportLockState,
            (transportLockState == A71CH_TRANSPORT_LOCK_STATE_LOCKED) ? "Transport Lock is set" :
            (transportLockState == A71CH_TRANSPORT_LOCK_STATE_UNLOCKED) ? "Open device, Transport Lock can no longer be set" :
            (transportLockState == A71CH_TRANSPORT_LOCK_STATE_ALLOW_LOCK) ? "Transport Lock NOT YET set" : "Undefined Transport Lock state");
        printf("scpState: 0x%02X (%s)\n", *scpState,
            (*scpState == A71CH_SCP_MANDATORY) ? "SCP is mandatory" :
            (*scpState == A71CH_SCP_NOT_SET_UP) ? "SCP is not set up" :
            (*scpState == A71CH_SCP_KEYS_SET) ? "SCP keys set" : "Undefined SCP state");
        printf("injectLockState: 0x%02X (%s)\n", injectLockState,
            (injectLockState == A71CH_INJECT_LOCK_STATE_LOCKED) ? "Locked" :
            (injectLockState == A71CH_INJECT_LOCK_STATE_UNLOCKED) ? "Unlocked" : "Undefined Inject Lock State");
        printf("gpStorageSize:   %d\n", gpStorageSize);
    }
    return result;
}
