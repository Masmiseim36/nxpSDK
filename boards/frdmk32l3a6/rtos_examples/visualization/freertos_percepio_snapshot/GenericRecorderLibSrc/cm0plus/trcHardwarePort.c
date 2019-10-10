/*******************************************************************************
 * Tracealyzer / FreeRTOS+Trace - Snapshot Trace Recorder Library v3.0.9
 * Percepio AB, www.percepio.com
 *
 * trcHardwarePort.c
 *
 * Contains together with trcHardwarePort.h all hardware portability issues of
 * the trace recorder library.
 *
 *******************************************************************************
 * Copyright (c) 2016, Percepio AB.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 *
 * Tabs are used for indent in this file (1 tab = 4 spaces)
 ******************************************************************************/

#include "trcHardwarePort.h"
#include "trcKernelPort.h"

#if (USE_TRACEALYZER_RECORDER == 1)

#include <stdint.h>

uint32_t trace_disable_timestamp = 0;
static uint32_t last_timestamp   = 0;

/*******************************************************************************
 * uiTraceTickCount
 *
 * This variable is should be updated by the Kernel tick interrupt. This does
 * not need to be modified when developing a new timer port. It is preferred to
 * keep any timer port changes in the HWTC macro definitions, which typically
 * give sufficient flexibility.
 ******************************************************************************/
uint32_t uiTraceTickCount = 0;

uint32_t DWT_CYCLES_ADDED = 0; /* Used on ARM Cortex-M only */

#if (SELECTED_PORT == PORT_ARM_CortexM) /* ONLY DWT PORT SHOULD BE HERE */

void prvTraceInitCortexM()
{
    /* Make sure DWT is enabled is enabled, if supported */
    REG_DEMCR |= DEMCR_TRCENA;

    do
    {
        /* Verify that DWT is supported */
        if (REG_DEMCR == 0)
        {
            vTraceError("DWT not supported by this chip!");
            break;
        }

        /* Verify that DWT_CYCCNT is supported */
        if (REG_DWT_CTRL & DWT_CTRL_NOCYCCNT)
        {
            vTraceError("DWT_CYCCNT not supported by this chip!");
            break;
        }

        /* Reset the cycle counter */
        REG_DWT_CYCCNT = 0;

        /* Enable the cycle counter */
        REG_DWT_CTRL |= DWT_CTRL_CYCCNTENA;

    } while (0); /* breaks above jump here */
}

#endif

/******************************************************************************
 * vTracePortGetTimeStamp
 *
 * Returns the current time based on the HWTC macros which provide a hardware
 * isolation layer towards the hardware timer/counter.
 *
 * The HWTC macros and vTracePortGetTimeStamp is the main porting issue
 * or the trace recorder library. Typically you should not need to change
 * the code of vTracePortGetTimeStamp if using the HWTC macros.
 *
 ******************************************************************************/
void vTracePortGetTimeStamp(uint32_t *pTimestamp)
{
    static uint32_t last_hwtc_count = 0;
    uint32_t hwtc_count             = 0;

#if HWTC_TYPE == HWTC_TYPE_SYSTICK
    /* systick based timer */
    static uint32_t last_traceTickCount = 0;
    uint32_t traceTickCount             = 0;
#else  /*HWTC_TYPE == HWTC_TYPE_SYSTICK*/
    /* Free running timer */
    static uint32_t last_hwtc_rest = 0;
    uint32_t diff                  = 0;
    uint32_t diff_scaled           = 0;
#endif /*HWTC_TYPE == HWTC_TYPE_SYSTICK*/

    if (trace_disable_timestamp == 1)
    {
        if (pTimestamp)
            *pTimestamp = last_timestamp;
        return;
    }

    /* Retrieve HWTC_COUNT only once since the same value should be used all throughout this function. */
#if (HWTC_COUNT_DIRECTION == DIRECTION_INCREMENTING)
    /* Get the increasing tick count */
    hwtc_count = HWTC_COUNT;
#elif (HWTC_COUNT_DIRECTION == DIRECTION_DECREMENTING)
    /* Convert decreasing tick count into increasing tick count */
    hwtc_count = HWTC_PERIOD - HWTC_COUNT;
#else
    Junk text to cause compiler error - HWTC_COUNT_DIRECTION is not set correctly !Should be DIRECTION_INCREMENTING or
        DIRECTION_DECREMENTING
#endif

#if (SELECTED_PORT == PORT_Win32)
    /* The Win32 port uses ulGetRunTimeCounterValue for timestamping, which in turn
    uses QueryPerformanceCounter. That function is not always reliable when used over
    multiple threads. We must therefore handle rare cases where the timestamp is less
    than the previous. In practice, the Win32 should "never" roll over since the
    performance counter is 64 bit wide. */

    if (last_hwtc_count > hwtc_count)
    {
        hwtc_count = last_hwtc_count;
    }
#endif

#if HWTC_TYPE == HWTC_TYPE_SYSTICK
    /* Timestamping is based on a timer that wraps at HWTC_PERIOD */
    if (last_traceTickCount - uiTraceTickCount - 1 < 0x80000000)
    {
        /* This means last_traceTickCount is higher than uiTraceTickCount,
        so we have previously compensated for a missed tick.
        Therefore we use the last stored value because that is more accurate. */
        traceTickCount = last_traceTickCount;
    }
    else
    {
        /* Business as usual */
        traceTickCount = uiTraceTickCount;
    }

    /* Check for overflow. May occur if the update of uiTraceTickCount has been
    delayed due to disabled interrupts. */
    if (traceTickCount == last_traceTickCount && hwtc_count < last_hwtc_count)
    {
        /* A trace tick has occurred but not been executed by the kernel, so we compensate manually. */
        traceTickCount++;
    }

    /* Check if the return address is OK, then we perform the calculation. */
    if (pTimestamp)
    {
        /* Get timestamp from trace ticks. Scale down the period to avoid unwanted overflows. */
        last_timestamp = traceTickCount * (HWTC_PERIOD / HWTC_DIVISOR);
        /* Increase timestamp by (hwtc_count + "lost hardware ticks from scaling down period") / HWTC_DIVISOR. */
        last_timestamp += (hwtc_count + traceTickCount * (HWTC_PERIOD % HWTC_DIVISOR)) / HWTC_DIVISOR;
    }
    /* Store the previous value */
    last_traceTickCount = traceTickCount;
#else  /*HWTC_TYPE == HWTC_TYPE_SYSTICK*/
    /* Timestamping is based on a free running timer */
    /* This seemingly unnecessarily complicated code will handle free running clocks that can be scaled down to avoid
    too large DTS values. Without this, the scaled timestamp will wrap at (2^32 / HWTC_DIVISOR) ticks. The scaled
    timestamp returned from this function is supposed to go from 0 -> 2^32, which in real time would represent (0 ->
    2^32 * HWTC_DIVISOR) ticks. */

    /* First we see how long time has passed since the last timestamp call, and we also add the ticks that went missing
     * when we scaled down the last time. */
    diff = (hwtc_count - last_hwtc_count) + last_hwtc_rest;

    /* Scale down the diff */
    diff_scaled = diff / HWTC_DIVISOR;

    /* Find out how many ticks were lost when scaling down, so we can add them the next time */
    last_hwtc_rest = diff % HWTC_DIVISOR;

    /* We increase the scaled timestamp by the scaled amount */
    last_timestamp += diff_scaled;
#endif /*HWTC_TYPE == HWTC_TYPE_SYSTICK*/

    /* Is anyone interested in the results? */
    if (pTimestamp)
        *pTimestamp = last_timestamp;

    /* Store the previous value */
    last_hwtc_count = hwtc_count;
}

#endif
