/***************************************************************************
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 * **************************************************************************
 *
 *  Name: mainA71CH_hlse.c
 *
 *  This file contains the main entry for the host library example application
 *      using the Generic APi
 *
 *  Define '#define WALKTHROUGH_ONLY' to limit the number of example functions
 *  invoked
 ****************************************************************************/

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "tst_sm_time.h"

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include "a71ch_ex_hlse.h"
#include "ax_api.h"

#include "axHostCrypto.h"
#include "sm_timer.h"
#include "sm_printf.h"

#include "global_platf.h"
#include "app_boot.h"

// -----------------------------------
// Start of END-USER relevant defines
// #define WALKTHROUGH_ONLY
// End of END-USER relevant defines
// -----------------------------------

// ---------------------------------------------------------------------
//           Please do not change the following defines
// ---------------------------------------------------------------------

#define EX_APP_VERSION "1.30:1.31"  // App-version:Preferred-applet-version

#if defined(LINUX) || defined(_WIN32)
#   define SET_TIME(now)         now = time(NULL)
#   define TIME_TO_STRING(pNow)  ctime((pNow))
#else
#   define SET_TIME(now)
#   define TIME_TO_STRING(time)  ("-")
#endif
/*******************************************************************************
 **   Main Function  main()
 *******************************************************************************/
int main(int argc, char ** argv)
{
    U8 result = 1;
    int connectStatus = 0;
    SmCommState_t commState;
#if defined(LINUX) || defined(_WIN32)
    time_t now;
#endif
    U8 dbgTstMode = 0x00;
    // U8 dbgTstMode = EXTENDED_TEST | MEASURE_EXEC_TIME;
    U8 gpStorageTstMode = 0x00;
    // U8 gpStorageTstMode = EXTENDED_TEST;
    axTimeMeasurement_t execTime;
#ifdef TDA8029_UART
    Scp03SessionState_t sessionState;
#endif

    app_boot_Init();

    sm_printf(DBGOUT, "a71ch HLSE example application (Rev %s)\r\n", EX_APP_VERSION);
    sm_printf(DBGOUT, "**********************************************\r\n");
    sm_printf(DBGOUT, "Connect to A71CH-SM. Chunksize at link layer = %d.\r\n", MAX_CHUNK_LENGTH_LINK);

    APP_BOOT_RESUME_HANDLING(exHlseBoot);

#if AX_EMBEDDED
    connectStatus = app_boot_Connect(&commState, NULL);
#else
    connectStatus = app_boot_Connect(&commState, argv[1]);
#endif

    if (connectStatus != 0) {
        sm_printf(CONSOLE, "Connection failed. SW = %d\r\n", connectStatus);
        #if defined(USE_RTOS) && USE_RTOS == 1
            assert(0);
        #else
            return connectStatus;
        #endif
    }

    initMeasurement(&execTime);

#ifdef WALKTHROUGH_ONLY
    result &= exHlseWalkthrough();
#else
    /*
    * New tests
    */
    result &= exHlseCert();
    //goto LBL_REPORT_STATUS;

    /*
     * Previous legacy tests - converted to use HLSE Generic API
     */

    result &= exHlseAes();
    result &= exHlseConfig();
    result &= exHlseDebugMode(dbgTstMode);
    result &= exHlseEccNohc();
    result &= exHlseWalkthrough();

    // exHlseGPStorage : partially converted - as
    // using gp storage "offset" parameter removed from the Generic API to use a higher level abstraction
    result &= exHlseGPStorage(gpStorageTstMode);

    result &= exHlseMisc();
    result &= exHlsePsk();
    result &= exHlseScp();
    result &= exHlseSst(commState.appletVersion);
    result &= exHlseSstKp();
#endif

#if defined(TDA8029_UART) || defined(I2C) || defined(PCSC) || defined(SPI) || defined(IPC)
LBL_REPORT_STATUS:
#endif
    concludeMeasurement(&execTime);
    SET_TIME(now);
    app_test_status(result);

    sm_printf(CONSOLE, "\r\nCompiled for Device Type %c\r\n", A71CH_DEVICE_TYPE);
    sm_printf(CONSOLE, "# Key Pairs = %d.\r\n", A71CH_KEY_PAIR_MAX);
    sm_printf(CONSOLE, "# Pub Key   = %d.\r\n", A71CH_PUBLIC_KEY_MAX);
    sm_printf(CONSOLE, "# Sym Key   = %d.\r\n", A71CH_SYM_KEY_MAX);
    if (commState.appletVersion < 0x0130)
    {
        sm_printf(CONSOLE, "Warning: Please switch to latest A71CH version, attached version is obsolete (attached version=0x%04X)\r\n", commState.appletVersion);
    }
    sm_printf(CONSOLE, "\r\n-----------\r\nExample Set HLSE-A71CH finished (Rev %s) on 0x%04X, overall result = %s\r\n%sExec time: %ld ms\r\n------------\r\n",
            EX_APP_VERSION,
            commState.appletVersion,
            ((result == 1) ? "OK" : "FAILED"),
            TIME_TO_STRING(&now),
            getMeasurement(&execTime));

#ifdef _WIN32
    if (IsDebuggerPresent()) {
        PRINTF("Enter/Return to close window.");
        getchar();
    }
#endif

    return 0;
}
