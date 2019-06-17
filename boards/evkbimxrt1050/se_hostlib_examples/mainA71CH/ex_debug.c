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
#include "a71ch_ex.h"
#include "ax_util.h"
#include "a71_debug.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"
#include "tst_sm_time.h"
#include "tst_a71ch_util.h"
#include "nxLog_hostLib.h"

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
    long msArray[MEASURE_POINTS_MAX] = {0};
    axTimeMeasurement_t mPair;
    int nPoint = 0;

    LOG_I( "-----------Start exDebugMode(extended_test=%s, measure_exec_time=%s)------------",
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
        LOG_I("exDbgReflect: Exec time for mixed packet sizes = %ld ms", msArray[nPoint]);
        nPoint++;
    }

    // Duration test
    if ((testMode & EXTENDED_TEST) == EXTENDED_TEST)
    {
        for (i=0; i<20; i++)
        {
            LOG_I("exDbgReflect_duration (iter==%d).", i);
            result &= exDbgReflect(1, 255);
            if (result == 0)
            {
                LOG_E("exDbgReflect_duration failed (iter==%d).", i);
                break;
            }
        }
    }

    // Measure Exec time
    if ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)
    {
        // * Measure transfer time for small packets
        LOG_I("exDbgReflect: Measure exec time for packets between 16 & 32 byte.");
        initMeasurement(&mPair);
        for (i=0; i<3; i++)
        {
            result &= exDbgReflect(16, 32);
            if (result == 0)
            {
                LOG_I("exDbgReflect failed (iter==%d) at line %d.", i, __LINE__);
                break;
            }
        }
        concludeMeasurement(&mPair);
        msArray[nPoint] = getMeasurement(&mPair);
        LOG_I("exDbgReflect: Exec time for packets between 16 & 32 byte = %ld ms", msArray[nPoint]);
        nPoint++;

        // * Measure transfer time for big packets
        LOG_I("exDbgReflect: Measure exec time for packets between 230 & 246 byte.");
        initMeasurement(&mPair);
        for (i=0; i<3; i++)
        {
            result &= exDbgReflect(230, 246);
            if (result == 0)
            {
                LOG_E("exDbgReflect failed (iter==%d) at line %d.", i, __LINE__);
                break;
            }
        }
        concludeMeasurement(&mPair);
        msArray[nPoint] = getMeasurement(&mPair);
        LOG_I("exDbgReflect: Exec time for packets between 230 & 246 byte = %ld ms", msArray[nPoint]);
        nPoint++;
    }

    // Report Exec time
    if ((testMode & MEASURE_EXEC_TIME) == MEASURE_EXEC_TIME)
    {
        if (result == 1)
        {
            LOG_I("exDbgReflect: Exec time for mixed packet sizes = %ld ms", msArray[0]);
            LOG_I("exDbgReflect: Exec time for packets between 16 & 32 byte = %ld ms", msArray[1]);
            LOG_I("exDbgReflect: Exec time for packets between 230 & 246 byte = %ld ms",  msArray[2]);
        }
    }

    // overall result
    LOG_I( "-----------End exDebugMode(extended_test=%s, measure_exec_time=%s), result = %s------------",
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
        LOG_E("exDbgReflect: Invalid arguments.");
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
            LOG_E("Call to A71_DbgReflect() failed: sw = 0x%04X.", sw);
            result &= 0;
        }
        else
        {
            // Check data payload 'reflected'
            // Is length as expected?
            if (rcvBufLen != sndBufLen)
            {
                LOG_E("Not enough data returned: rcv=%d != snd=%d.", rcvBufLen, sndBufLen);
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
            LOG_E("exDbgReflect: Failed on packetsize %d", i);
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

    LOG_I("-----------Start exDbgInternalMemory(%s)------------", getInitModeAsString(initMode));

    // Initialize the A71CH
    result &= a71chInitModule(initMode);
    assert(result);

    // Get Free Memory
    LOG_I("A71_DbgGetFreePersistentMemory");
    err = A71_DbgGetFreePersistentMemory(&freeMem);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    LOG_I("FreePersistentMemory: %d", freeMem);

    LOG_I( "A71_DbgGetFreeTransientMemory");
    err = A71_DbgGetFreeTransientMemory(&freeMem);
    result &= AX_CHECK_SW(err, SW_OK, "err");
    LOG_I( "FreeTransientMemory: %d", freeMem);

    LOG_I( "-----------End exDbgInternalMemory(), result = %s------------", ((result == 1)? "OK": "FAILED"));

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

    LOG_I( "-----------Start exPermanentlyDisableDebugMode()------------");

#ifdef A71CH_ALLOW_DISABLE_DEBUG_MODE
    LOG_I( "A71_DbgDisableDebug()");
    err = A71_DbgDisableDebug();
    result &= AX_CHECK_SW(err, SW_OK, "Invocation of A71_DbgDisableDebug() failed.");
#else
    LOG_I("********************************************************************************");
    LOG_I("A71_DbgDisableDebug() not invoked.");
    LOG_I("Please define A71CH_ALLOW_DISABLE_DEBUG_MODE in header file a71ch_ex.h to enable");
    LOG_I("the invocation of A71_DbgDisableDebug() through this example function.");
    LOG_I("The effect of A71_DbgDisableDebug() is irreversible!");
    LOG_I("********************************************************************************");
    result = 0;
#endif

    LOG_I("-----------End exPermanentlyDisableDebugMode(), result = %s------------", ((result == 1)? "OK": "FAILED"));

    assert(result);
    return result;
}
