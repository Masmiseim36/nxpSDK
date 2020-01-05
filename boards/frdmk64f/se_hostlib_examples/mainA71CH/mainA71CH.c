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
 *  Name: mainA71CH.c
 *
 *  This file contains the main entry for the host library example application
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

#ifdef MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include "a71ch_ex.h"
#include "ax_api.h"

#include "axHostCrypto.h"
#include "sm_timer.h"
#include "sm_printf.h"

#include "global_platf.h"
#include "app_boot.h"
#include "nxLog_App.h"

// -----------------------------------
// Start of END-USER relevant defines
// #define WALKTHROUGH_ONLY
// End of END-USER relevant defines
// -----------------------------------

// ---------------------------------------------------------------------
//           Please do not change the following defines
// ---------------------------------------------------------------------

#define EX_APP_VERSION "1.30:1.31"  // App-version:Preferred-applet-version


//TODO: to be moved to common Header file later
#define PRINT_HOSTLIB_VER       (sm_printf(CONSOLE, "HostLib Version  : 0x%04X", commState.hostLibVersion))

#define PRINT_APPLET_VER        (sm_printf(CONSOLE, "Applet Version   : 0x%04X", commState.appletVersion))
#define PRINT_SECUREBOX_VER       (sm_printf(CONSOLE, "SecureBox Version: 0x%04X", commState.sbVersion))

#if defined(__gnu_linux__) || defined(_WIN32)
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
    SmCommState_t commState = { 0 };
#if defined(__gnu_linux__) || defined(_WIN32)
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
    // int i = 0;

#ifdef MSC_VER
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF |
        _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

    app_boot_Init();

    LOG_I("a71ch HostLibrary example application (Rev %s)", EX_APP_VERSION);
    LOG_I("**********************************************");
    LOG_I("Connect to A71CH-SM. Chunksize at link layer = %d.", MAX_CHUNK_LENGTH_LINK);

    APP_BOOT_RESUME_HANDLING(exBoot);

#if AX_EMBEDDED
    connectStatus = app_boot_Connect(&commState, NULL);
#else
    connectStatus = app_boot_Connect(&commState, argv[1]);
#endif

    if (connectStatus != 0) {
    	LOG_E("Connection failed. SW = %d", connectStatus);
        #if defined(USE_RTOS) && USE_RTOS == 1
            assert(0);
        #else
            return connectStatus;
        #endif
    }

    initMeasurement(&execTime);
#ifdef WALKTHROUGH_ONLY
    result &= exWalkthrough();
#else
    result &= exAes();
    result &= exConfig();
    result &= exDebugMode(dbgTstMode);
    result &= exEccNohc();
    result &= exWalkthrough();
    result &= exGPStorage(gpStorageTstMode);
    result &= exMisc();
    result &= exPsk();
    result &= exScp();
    result &= exSst();
    result &= exBoot(BOOT_SIMULATED_CYCLE);
    result &= exSstKp();
#endif

#if defined(TDA8029_UART) || defined(SCI2C) || defined(PCSC) || defined(SPI) || defined(IPC)
LBL_REPORT_STATUS:
#endif
    concludeMeasurement(&execTime);
    SET_TIME(now);
    app_test_status(result);

    LOG_I("Compiled for Device Type %c", A71CH_DEVICE_TYPE);
    LOG_I("# Key Pairs = %d.", A71CH_KEY_PAIR_MAX);
    LOG_I("# Pub Key   = %d.", A71CH_PUBLIC_KEY_MAX);
    LOG_I("# Sym Key   = %d.", A71CH_SYM_KEY_MAX);
    LOG_I("Gp Storage  = %d byte", A71CH_GP_STORAGE_SIZE);

    if (result == 1) {
       LOG_I("-----------Example Set A71CH finished (Rev %s) on 0x%04X, overall result = %s\r\n%sExec time: %ld ms------------",
			EX_APP_VERSION,
			commState.appletVersion,
			"OK",
			TIME_TO_STRING(&now),
			getMeasurement(&execTime));
    }
    else {
        LOG_E("-----------Example Set A71CH finished (Rev %s) on 0x%04X, overall result = %s\r\n%sExec time: %ld ms------------",
 			EX_APP_VERSION,
 			commState.appletVersion,
			"FAILED",
 			TIME_TO_STRING(&now),
 			getMeasurement(&execTime));
    }

#if defined(_WIN32) && !defined(__MINGW32__)
    if (IsDebuggerPresent()) {
        PRINTF("Enter/Return to close window.");
        getchar();
    }
#endif

    return 0;
}
