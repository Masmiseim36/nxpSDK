/**
 * @file ex_boot.c
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
 * Handover of SCP03 session keys from bootloader to OS. Refer to ::exBoot for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

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
#include "app_boot.h"

#if defined(SCI2C)
#include "sci2c.h"
#endif

#if defined(SCI2C)
static U8 writeStateToFile(char *szFilename, SmCommState_t *commState, Scp03SessionState_t *scp03State);
static U8 readStateFromFile(char *szFilename, SmCommState_t *commState, Scp03SessionState_t *scp03State);
#endif

char* axExBootGetBootModeAsString(U8 bootMode)
{
    if (bootMode == BOOT_SKIP_EXAMPLE)
    {
        return "Skip boot example";
    }
    else if (bootMode == BOOT_SIMULATED_CYCLE)
    {
        return "Do full boot cycle (Bootloader/OS combined)";
    }
    else if (bootMode == BOOT_BOOTLOADER_ROLE)
    {
        return "Mimick behaviour of bootloader only (store session state on filesystem)";
    }
    else if (bootMode == BOOT_HOST_OS_RESUME)
    {
        return "Mimick behaviour of Host OS only (retrieve session state from filesystem)";
    }
    else
    {
        PRINTF("bootMode not defined\r\n");
        assert(0);
        return "not defined";
    }
}

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
/// @endcond

/**
* This example illustrates that the HostOS does NOT need to know the SCP03 Base Keys
* to establish an SCP03 session (provided the boot loader has established the
* SCP03 session and saved the SCP03 session state)
*
* When using SCI2C the bootloader must also save the SCI2C state.
*
* - The boot loader
*       - Sets an initial SCP03 base key set (based on random data retrieved from
*         the SM (Secure Module))
*       - Establishes an SCP03 session
*       - Stores the SCP03 session (optionally also SCI2C state)
*
* - The OS
*       - Retrieves the SCP03 session and communication state
*       - Resumes the SCP03 session
*
* \note When the OS takes over from the boot loader the communication link with
*    the Secure Element may not be broken.
* \note Storing the session and communication state on the filesystem is done for convenience
*    in this example. Use an appropriate and secure sharing mechanism in a product implementation.
* \note Full functionality of this example is only available when connecting to the A71CH over SCI2C.
* @param[in] bootMode One of the following values
*   - ::BOOT_SIMULATED_CYCLE Can be used to simulate handover of session keys (no dependency on link protocol)
*   - ::BOOT_BOOTLOADER_ROLE Play the role of initial bootloader and store session and communication state to filesystem
*   - ::BOOT_HOST_OS_RESUME  Play the role of OS and retrieve session and communication state from the filesystem
*/
U8 exBoot(U8 bootMode)
{
    U8 result = 1;
    U8 initMode = INIT_MODE_NO_RESET;

    U8 scpKeyEncBase[SCP_KEY_SIZE];
    U8 scpKeyMacBase[SCP_KEY_SIZE];
    U8 scpKeyDekBase[SCP_KEY_SIZE];

    U8 dataRef[] = {0xBE, 0xFA, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    U16 dataRefLen = sizeof(dataRef);
    U8 dataFetch[8];
    U16 packetSize;

    U16 connectStatus;
    U8 Atr[64] = {0};
    U16 AtrLen = sizeof(Atr);

    SmCommState_t commState = {0};
    Scp03SessionState_t sessionState;

    U16 sw = SW_OK;
#if defined(SCI2C)
    char szFilename[] = "sessionState";
#endif

    PRINTF("\r\n-----------\r\nStart exBoot(%s)\r\n------------\r\n", axExBootGetBootModeAsString(bootMode));

    if (bootMode == BOOT_SKIP_EXAMPLE)
    {
        PRINTF("***** WARNING: exBoot example is skipped *****\r\n");
    }
    else if (bootMode == BOOT_SIMULATED_CYCLE)
    {
        initMode = INIT_MODE_RESET;

        // Installing Callback (this step is optional)
        SCP_Subscribe(signalFunctionCallback, NULL);
        DEV_ClearChannelState();
#if defined(SCI2C)|| defined(T1oI2C)
        SM_Close(SMCOM_CLOSE_MODE_TERMINATE);
#endif
        connectStatus = SM_Connect(&commState, Atr, &AtrLen);
        if (connectStatus != SW_OK)
        {
            sm_printf(CONSOLE, "Failed to establish connection to Secure Module: 0x%04" PRIX32 "\r\n", connectStatus);
            result = 0;
            return result;
        }
        else
        {
#ifndef SMCOM_JRCP_V1
            int i=0;
            sm_printf(CONSOLE, "ATR=0x");
            for (i=0; i<AtrLen; i++) sm_printf(CONSOLE, "%02" PRIX16 ".", Atr[i]);
            sm_printf(CONSOLE, "\r\n");
#endif
        }

        // STAGE-0:
        // - Force the A71CH into the initial state (only possible when in Debug Mode)
        sm_printf(CONSOLE, "Initialize Module\r\n");
        result &= a71chInitModule(initMode);

        // STAGE-1:
        // - Fetch random data from the A71CH
        // - Set the SCP03 keys based on this random data
        // - This binding between Host and A71CH must only be done once
        result &= axExCreateAndSetInitialHostScpKeys(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);

        // STAGE-2:
        // - Locally store the SCP03 base keys on the Host (for re-use in the next boot cycle)
        // - Set up the authenticated and encrypted SCP03 channel with the A71CH.
        result &= axExAuthenticate(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);

        // STAGE-3:
        // - Write an 8 byte pattern to GPStorage (just using some standard A71CH functionality)
        packetSize = dataRefLen;
        PRINTF( "\r\nA71_SetGpData(0, %d, ...)\r\n", packetSize);
        sw = A71_SetGpData(0, dataRef, packetSize);
        result &= AX_CHECK_SW(sw, SW_OK, "A71_SetGpData fails");

        // Before Handing over to the Host OS:
        // - Retrieve and store the SCP03 session state
        // - Retrieve and store SCI2C communication state
        SCP_GetScpSessionState(&sessionState);
        // At this stage the System Integrator must store Session State in Mailbox for HostOS
#if defined(SCI2C)
        commState.param1 = sci2c_GetSequenceCounter();
        // At this stage the System Integrator must store communication state in Mailbox for HostOS
        SM_Close(SMCOM_CLOSE_MODE_STD);
#endif
        sm_printf(DBGOUT, "\r\n\r\n");
        sm_printf(DBGOUT, "******************************************************************\r\n");
        sm_printf(DBGOUT, "** System Integrator must store Communication and Session State **\r\n");
        sm_printf(DBGOUT, "******************************************************************\r\n");

        DEV_ClearChannelState();

        // +++ Handover from Boot to HostOS +++

        // ** Host OS phase **
        SM_ResumeConnection(&commState, &sessionState);

        // - Retrieve and compare the 8 byte reference pattern from GPStorage (just using some standard A71CH functionality)
        packetSize = dataRefLen;
        PRINTF( "\r\nA71_GetGpData(0, %d, ...)\r\n", packetSize);
        sw = A71_GetGpData(0, dataFetch, packetSize);
        result &= AX_CHECK_SW(sw, SW_OK, "A71_GetGpData fails");
        axPrintByteArray ("dataFetch", dataFetch, packetSize, AX_COLON_32);
        result &= AX_COMPARE_BYTE_ARRAY("dataRef", dataRef, dataRefLen,
            "dataFetch", dataFetch, dataRefLen, AX_COLON_32);
    }
#if defined(SCI2C)
    else if (bootMode == BOOT_BOOTLOADER_ROLE)
    {
        initMode = INIT_MODE_RESET;

        // Installing Callback (this step is optional)
        SCP_Subscribe(signalFunctionCallback, NULL);
        DEV_ClearChannelState();
        SM_Close(SMCOM_CLOSE_MODE_TERMINATE);
        connectStatus = SM_Connect(&commState, Atr, &AtrLen);
        if (connectStatus != SW_OK)
        {
            sm_printf(CONSOLE, "Failed to establish connection to Secure Module: 0x%04" PRIX32 "\r\n", connectStatus);
            result = 0;
            return result;
        }
        else
        {
            int i=0;
            sm_printf(CONSOLE, "ATR=0x");
            for (i=0; i<AtrLen; i++) sm_printf(CONSOLE, "%02" PRIX16 ".", Atr[i]);
            sm_printf(CONSOLE, "\r\n");
        }

        // STAGE-0:
        // - Force the A71CH into the initial state (only possible when in Debug Mode)
        sm_printf(CONSOLE, "Initialize Module\r\n");
        result &= a71chInitModule(initMode);

        // STAGE-1:
        // - Fetch random data from the A71CH
        // - Set the SCP03 keys based on this random data
        result &= axExCreateAndSetInitialHostScpKeys(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);

        // STAGE-2:
        // - Locally store the SCP03 base keys
        // - Set up the authenticated and encrypted SCP03 channel with the A71CH.
        result &= axExAuthenticate(scpKeyEncBase, scpKeyMacBase, scpKeyDekBase);

        // STAGE-3:
        // - Write an 8 byte pattern to GPStorage (just using some standard A71CH functionality)
        packetSize = dataRefLen;
        PRINTF( "\r\nA71_SetGpData(0, %d, ...)\r\n", packetSize);
        sw = A71_SetGpData(0, dataRef, packetSize);
        result &= AX_CHECK_SW(sw, SW_OK, "A71_SetGpData fails");

        // Before Handing over to the Host OS:
        // - Retrieve and store the SCP03 session state
        // - Retrieve and store SCI2C communication state
        SCP_GetScpSessionState(&sessionState);
        commState.param1 = sci2c_GetSequenceCounter();

        SM_Close(SMCOM_CLOSE_MODE_STD);

        sm_printf(DBGOUT, "\r\n\r\n");
        sm_printf(DBGOUT, "******************************************************************\r\n");
        sm_printf(DBGOUT, "** System Integrator must store Communication and Session State **\r\n");
        sm_printf(DBGOUT, "******************************************************************\r\n");
        sm_printf(DBGOUT, "..\r\n..\r\n");

        // WARNING: Simply writing the SessionState to the file system is not a secure implementation.
        // it must not be used in products.
        writeStateToFile(szFilename, &commState, &sessionState);

        DEV_ClearChannelState();

    }
    else if (bootMode == BOOT_HOST_OS_RESUME)
    {
        SmCommState_t retrCommState;
        Scp03SessionState_t retrScp03State;

        readStateFromFile(szFilename, &retrCommState, &retrScp03State);
        PRINTF("retrCommState.param1         : 0x%02" PRIX16 "\r\n", retrCommState.param1);
        PRINTF("retrCommState.hostLibVersion : 0x%02" PRIX16 "\r\n", retrCommState.hostLibVersion);
        PRINTF("retrCommState.appletVersion  : 0x%04" PRIX32 "\r\n", retrCommState.appletVersion);

        DEV_ClearChannelState();

        // +++ Handover from Boot to HostOS +++

        sm_printf(DBGOUT, "\r\n\r\n");
        sm_printf(DBGOUT, "*******************************************************\r\n");
        sm_printf(DBGOUT, "** Host OS resumes SCP03 session with Secure Element **\r\n");
        sm_printf(DBGOUT, "*******************************************************\r\n");
        // ** Host OS phase **
        SM_ResumeConnection(&retrCommState, &retrScp03State);

        // - Retrieve and compare the 8 byte reference pattern from GPStorage (just using some standard A71CH functionality)
        packetSize = dataRefLen;
        PRINTF( "\r\nA71_GetGpData(0, %d, ...)\r\n", packetSize);
        sw = A71_GetGpData(0, dataFetch, packetSize);
        result &= AX_CHECK_SW(sw, SW_OK, "A71_GetGpData fails");
        axPrintByteArray ("dataFetch", dataFetch, packetSize, AX_COLON_32);

        result &= AX_COMPARE_BYTE_ARRAY("dataRef", dataRef, dataRefLen,
            "dataFetch", dataFetch, dataRefLen, AX_COLON_32);
    }
#endif // defined(SCI2C)
    else
    {
        PRINTF("bootMode (0x%02" PRIX16 ") is not defined.\r\n", (uint16_t)bootMode);
        result = 0;
    }

    // overall result
    PRINTF("\r\n-----------\r\nEnd exBoot(%s), result = %s\r\n------------\r\n",
        axExBootGetBootModeAsString(bootMode), ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * The host fetches random data from the A71CH and sets these as SCP03 base keys in the
 * A71CH.
 *
 * @param[in,out] keyEnc IN: Buffer to contain key; OUT: Key created and inserted into A71CH
 * @param[in,out] keyMac IN: Buffer to contain key; OUT: Key created and inserted into A71CH
 * @param[in,out] keyDek IN: Buffer to contain key; OUT: Key created and inserted into A71CH
 */
U8 axExCreateAndSetInitialHostScpKeys(U8 *keyEnc, U8 *keyMac, U8 *keyDek)
{
    U8 result = 1;
    U16 err = 0;
    U8 random[3*SCP_KEY_SIZE];
    U8 randomLen = (U8)sizeof(random);
    U8 *currentKeyDek = NULL;
    U8 keyVersion = 1;

    assert(keyEnc != NULL);
    assert(keyMac != NULL);
    assert(keyDek != NULL);

    PRINTF( "\r\n-----------\r\nStart axExCreateAndSetInitialHostScpKeys()\r\n------------\r\n");

    // Security module generates random data for initial SCP03 keys
    sm_printf(CONSOLE, "\r\nA71_GetRandom(randomLen=%d)\r\n", randomLen);
    err = A71_GetRandom(random, randomLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    // Storing Static Keys
    memcpy(keyEnc, random, SCP_KEY_SIZE);
    memcpy(keyMac, random + SCP_KEY_SIZE, SCP_KEY_SIZE);
    memcpy(keyDek, random + (2*SCP_KEY_SIZE), SCP_KEY_SIZE);

    keyVersion = (U8) (SST_HOST_SCP_KEYSET >> 8);
    sm_printf(CONSOLE, "\r\nSCP_GP_PutKeys(keyVersion=0x%02" PRIX16 ")\r\n", keyVersion);
    err = SCP_GP_PutKeys(keyVersion, keyEnc, keyMac, keyDek, currentKeyDek, AES_KEY_LEN_nBYTE);
    result &= AX_CHECK_SW(err, SW_OK, "err");

    PRINTF( "\r\n-----------\r\nEnd axExCreateAndSetInitialHostScpKeys(), result = %s\r\n------------\r\n",
        ((result == 1)? "OK": "FAILED"));

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
U8 axExAuthenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek)
{
    U8 sCounter[3];
    U16 sCounterLen = sizeof(sCounter);

    U8 result = 1;
    U16 err = 0;

    PRINTF( "\r\n-----------\r\nStart axExAuthenticate()\r\n------------\r\n");

    // Authenticate Channel
    sm_printf(CONSOLE, "\r\nSCP_Authenticate()\r\n");
    err = SCP_Authenticate(keyEnc, keyMac, keyDek, SCP_KEY_SIZE, sCounter, &sCounterLen);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    result &= AX_CHECK_U16(sCounterLen, 0, "Only expected when SCP03 is configured for pseudo-random challenge");

    PRINTF( "\r\n-----------\r\nEnd axExAuthenticate(), result = %s\r\n------------\r\n",
        ((result == 1)? "OK": "FAILED"));

    return result;
}

#if defined(SCI2C)
static U8 writeStateToFile(char *szFilename, SmCommState_t *commState, Scp03SessionState_t *scp03State)
{
    U8 result = 1;
#ifdef FTR_FILE_SYSTEM
    FILE *fHandle = NULL;

    fHandle = fopen(szFilename, "w");

    if (fHandle == NULL)
    {
        sm_printf(DBGOUT, "Failed to open file %s for writing\r\n", szFilename);
        return 0;
    }

    fwrite((const void *) commState, sizeof(SmCommState_t), 1, fHandle);
    fwrite((const void *) scp03State, sizeof(Scp03SessionState_t), 1, fHandle);

    fclose(fHandle);
#endif
    return result;
}

static U8 readStateFromFile(char *szFilename, SmCommState_t *commState, Scp03SessionState_t *scp03State)
{
    U8 result = 1;
#ifdef FTR_FILE_SYSTEM
    FILE *fHandle = NULL;

    fHandle = fopen(szFilename, "r");

    if (fHandle == NULL)
    {
        sm_printf(DBGOUT, "Failed to open file %s for reading", szFilename);
        return 0;
    }

    fread((void *) commState, sizeof(SmCommState_t), 1, fHandle);
    fread((void *) scp03State, sizeof(Scp03SessionState_t), 1, fHandle);

    fclose(fHandle);
#endif
    return result;
}
#endif
