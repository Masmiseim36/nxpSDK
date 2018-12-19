/**
 * @file ex_debug.c
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
#include "sm_printf.h"
#include "a71ch_ex.h"
#include "ax_util.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"
#include "tst_sm_time.h"
#include "tst_a71ch_util.h"

#define MEASURE_POINTS_MAX 16



/**
 * Demonstrate miscelaneous functionality supported by Debug Mode of A71CH
 * - ::exDbgInternalMemory
 * - ::exDbgReflect
 *
 */
U8 exDebugMode(U8 testMode)
{
    U8 result = 1;
    int i;
    long msArray[MEASURE_POINTS_MAX];
    axTimeMeasurement_t mPair;
    int nPoint = 0;

    PRINTF( "\r\n-----------\r\nStart exDebugMode(extended_test=%s, measure_exec_time=%s)\r\n------------\r\n",
        ((testMode & EXTENDED_TEST) == EXTENDED_TEST)? "ON":"OFF",
        ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)? "ON":"OFF");

    DEV_ClearChannelState();

    result &= exDbgInternalMemory(INIT_MODE_RESET);

    if ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)
    {
        // * Measure transfer time for mixed packet sizes
        initMeasurement(&mPair);
    }

    // Test reflect on small packet sizes
    result &= exDbgReflect(5, 7);

    // Test reflect on bigger packet sizes
    result &= exDbgReflect(200, 202);

    // Test reflect on a lot of packet sizes
    result &= exDbgReflect(20, 220);

    // Test reflect on the smallest packet sizes
    result &= exDbgReflect(1, 10);

    // Test reflect on the biggest packet sizes
    result &= exDbgReflect(230, 255);

    if ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)
    {
        // * Measure transfer time for mixed packet sizes
        concludeMeasurement(&mPair);
        msArray[nPoint] = getMeasurement(&mPair);
        PRINTF("exDbgReflect: Exec time for mixed packet sizes = %ld ms\r\n", msArray[nPoint]);
        nPoint++;
    }

    // Duration test
    if ((testMode & EXTENDED_TEST) == EXTENDED_TEST)
    {
        for (i=0; i<20; i++)
        {
            PRINTF("exDbgReflect_duration (iter==%d).\r\n", i);
            result &= exDbgReflect(1, 255);
            if (result == 0)
            {
                PRINTF("exDbgReflect_duration failed (iter==%d).\r\n", i);
                break;
            }
        }
    }

    // Measure Exec time
    if ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)
    {
        // * Measure transfer time for small packets
        PRINTF("exDbgReflect: Measure exec time for packets between 16 & 32 byte.\r\n");
        initMeasurement(&mPair);
        for (i=0; i<3; i++)
        {
            result &= exDbgReflect(16, 32);
            if (result == 0)
            {
                PRINTF("exDbgReflect failed (iter==%d) at line %d.\r\n", i, __LINE__);
                break;
            }
        }
        concludeMeasurement(&mPair);
        msArray[nPoint] = getMeasurement(&mPair);
        PRINTF("exDbgReflect: Exec time for packets between 16 & 32 byte = %ld ms\r\n", msArray[nPoint]);
        nPoint++;

        // * Measure transfer time for big packets
        PRINTF("exDbgReflect: Measure exec time for packets between 230 & 246 byte.\r\n");
        initMeasurement(&mPair);
        for (i=0; i<3; i++)
        {
            result &= exDbgReflect(230, 246);
            if (result == 0)
            {
                PRINTF("exDbgReflect failed (iter==%d) at line %d.\r\n", i, __LINE__);
                break;
            }
        }
        concludeMeasurement(&mPair);
        msArray[nPoint] = getMeasurement(&mPair);
        PRINTF("exDbgReflect: Exec time for packets between 230 & 246 byte = %ld ms\r\n", msArray[nPoint]);
        nPoint++;
    }

    // Report Exec time
    if ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)
    {
        if (result == 1)
        {
            PRINTF("exDbgReflect: Exec time for mixed packet sizes = %ld ms\r\n", msArray[0]);
            PRINTF("exDbgReflect: Exec time for packets between 16 & 32 byte = %ld ms\r\n", msArray[1]);
            PRINTF("exDbgReflect: Exec time for packets between 230 & 246 byte = %ld ms\r\n",  msArray[2]);
        }
    }

    // overall result
    PRINTF( "\r\n-----------\r\nEnd exDebugMode(extended_test=%s, measure_exec_time=%s), result = %s\r\n------------\r\n",
        ((testMode & EXTENDED_TEST) == EXTENDED_TEST)? "ON":"OFF",
        ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)? "ON":"OFF",
        ((result == 1)? "OK": "FAILED"));

    return result;
}

/**
 * Demonstrate using A71_DbgReflect. Can be used to validate communication link.
 *
 * @param[in] nLower Smallest packetsize to be tested
 * @param[in] nUpper Biggest packetsize to be tested
 */
U8 exDbgReflect(int nLower, int nUpper)
{
    U16 sw = 0;
    U8 sndBuf[1024];
    U16 sndBufLen = 0;
    U8 rcvBuf[1024];
    U16 rcvBufLen = sizeof(rcvBuf);
    int i;
    int j;
    U8 result = 1;

    if (nLower > nUpper)
    {
        PRINTF("exDbgReflect: Invalid arguments.\r\n");
        return 0;
    }

    for (i=nLower; i<=nUpper; i++)
    {
        // Fill sndBuffer with monotonic rising/wrapping data
        for (j=0; j<i; j++) { sndBuf[j] = (U8)j; }
        for (j=0; j<i; j++) { rcvBuf[j] = (U8)0xFF; }
        rcvBufLen = sizeof(rcvBuf);
        sndBufLen = (U16)i;
        sw = A71_DbgReflect(sndBuf, sndBufLen, rcvBuf, &rcvBufLen);
        if (sw != SW_OK)
        {
            PRINTF("Call to A71_DbgReflect() failed: sw = 0x%04X.\r\n", sw);
            result &= 0;
        }
        else
        {
            // Check data payload 'reflected'
            // Is length as expected?
            if (rcvBufLen != sndBufLen)
            {
                PRINTF("Not enough data returned: rcv=%d != snd=%d.\r\n", rcvBufLen, sndBufLen);
                result &= 0;
            }
            else
            {
                result &= AX_COMPARE_BYTE_ARRAY("rcvBuf", rcvBuf, rcvBufLen, "sndBuf", sndBuf, sndBufLen, AX_COLON_32);
            }
        }
        if (result == 0)
        {
            // Drop out of loop if an error has occured.
            PRINTF("exDbgReflect: Failed on packetsize %d\r\n", i);
            break;
        }
    }

    return result;
}

/**
 * Demonstrate debug functions reporting back the available internal memory of the A71CH.
 *
 * Note: The memory in use by the A71CH is of no concern to the System Integrator.
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
U8 exDbgInternalMemory(U8 initMode)
{
    S16 freeMem = 0;
    U8 result = 1;
    U16 err;

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exDbgInternalMemory(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH
    result &= a71chInitModule(initMode);
    assert(result);

    // Get Free Memory
    sm_printf(CONSOLE, "A71_DbgGetFreePersistentMemory\r\n");
    err = A71_DbgGetFreePersistentMemory(&freeMem);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    sm_printf(CONSOLE, "FreePersistentMemory: %d\r\n", freeMem);

    sm_printf(CONSOLE, "A71_DbgGetFreeTransientMemory\r\n");
    err = A71_DbgGetFreeTransientMemory(&freeMem);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    sm_printf(CONSOLE, "FreeTransientMemory: %d\r\n", freeMem);

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exDbgInternalMemory(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    assert(result);
    return result;
}

/**
 * Invoke this function with caution as it will permanently disable the Debug Mode
 * of the A71CH included in the Evaluation Kit.
 * The SW is shipped with this example function disabled. So one needs to define
 * A71CH_ALLOW_DISABLE_DEBUG_MODE to enable it.
 */
U8 exPermanentlyDisableDebugMode()
{
    U8 result = 1;
#ifdef A71CH_ALLOW_DISABLE_DEBUG_MODE
    U16 err;
#endif

    sm_printf(CONSOLE, "\r\n-----------\r\nStart exPermanentlyDisableDebugMode()\r\n------------\r\n");

#ifdef A71CH_ALLOW_DISABLE_DEBUG_MODE
    sm_printf(CONSOLE, "A71_DbgDisableDebug()\r\n");
    err = A71_DbgDisableDebug();
    result &= AX_CHECK_SW(err, SW_OK, "Invocation of A71_DbgDisableDebug() failed.");
#else
    sm_printf(CONSOLE, "********************************************************************************\r\n");
    sm_printf(CONSOLE, "A71_DbgDisableDebug() not invoked.\r\n");
    sm_printf(CONSOLE, "Please define A71CH_ALLOW_DISABLE_DEBUG_MODE in header file a71ch_ex.h to enable\r\n");
    sm_printf(CONSOLE, "the invocation of A71_DbgDisableDebug() through this example function.\r\n");
    sm_printf(CONSOLE, "The effect of A71_DbgDisableDebug() is irreversible!\r\n");
    sm_printf(CONSOLE, "********************************************************************************\r\n");
    result = 0;
#endif

    sm_printf(CONSOLE, "\r\n-----------\r\nEnd exPermanentlyDisableDebugMode(), result = %s\r\n------------\r\n", ((result == 1)? "OK": "FAILED"));

    assert(result);
    return result;
}
