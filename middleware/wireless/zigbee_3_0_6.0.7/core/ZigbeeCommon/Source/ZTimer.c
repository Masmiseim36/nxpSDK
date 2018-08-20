/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*!=============================================================================
\file       ZTimer.c
\brief      Provides the api functions for ZigBee Timer Module
==============================================================================*/

/****************************************************************************/
/*          Include files                                                   */
/****************************************************************************/
#include "FunctionLib.h"
#include <jendefs.h>
#include <string.h>
#include "dbg.h"
#include "ZTimer.h"
#include "pwrm.h"
#include "TimersManager.h"

/****************************************************************************/
/*          Macro Definitions                                               */
/****************************************************************************/

#ifdef DEBUG_ZTIMER
#define TRACE_ZTIMER    TRUE
#else
#define TRACE_ZTIMER    FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                                */
/****************************************************************************/

typedef struct
{
    uint8            u8NumTimers;
    ZTIMER_tsTimer    *psTimers;
} ZTIMER_tsCommon;

/****************************************************************************/
/*          Local Function Prototypes                                       */
/****************************************************************************/

/****************************************************************************/
/*          Exported Variables                                              */
/****************************************************************************/

/****************************************************************************/
/*          Local Variables                                                 */
/****************************************************************************/

PRIVATE ZTIMER_tsCommon ZTIMER_sCommon;

/****************************************************************************/
/*          Exported Functions                                              */
/****************************************************************************/

/****************************************************************************
 *
 * NAME: ZTIMER_eInit
 *
 * DESCRIPTION:
 * Initialises the ZTimer module
 *
 * RETURNS:
 * ZTIMER_teStatus
 *
 ****************************************************************************/
PUBLIC ZTIMER_teStatus ZTIMER_eInit(ZTIMER_tsTimer *psTimers, uint8 u8NumTimers)
{
    DBG_vPrintf(TRACE_ZTIMER, "ZT: Initialising: ");

    if(psTimers == NULL || u8NumTimers == 0)
    {
        DBG_vPrintf(TRACE_ZTIMER, "Failed\n");
        return E_ZTIMER_FAIL;
    }

    TMR_TimeStampInit();

    ZTIMER_sCommon.u8NumTimers = u8NumTimers;
    ZTIMER_sCommon.psTimers = psTimers;
    FLib_MemSet(psTimers, 0, sizeof(ZTIMER_tsTimer) * u8NumTimers);

    DBG_vPrintf(TRACE_ZTIMER, "Success\n");

    return E_ZTIMER_OK;
}

/****************************************************************************
 *
 * NAME: ZTIMER_vSleep
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void ZTIMER_vSleep(void)
{
}

/****************************************************************************
 *
 * NAME: ZTIMER_vWake
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void ZTIMER_vWake(void)
{
}

/****************************************************************************
 *
 * NAME: ZTIMER_vTask
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void ZTIMER_vTask(void)
{
    int n;
    ZTIMER_tsTimer *psTimer;
    volatile uint32 u32CurrentTime = TMR_GetTimestamp()/1000;

    DBG_vPrintf(TRACE_ZTIMER, "ZT: Tick\n");

    /* Process all of the timers */
    for(n = 0; n < ZTIMER_sCommon.u8NumTimers; n++)
    {
        psTimer = &ZTIMER_sCommon.psTimers[n];

        /* If this timer is not opened and running, skip it */
        if(psTimer->eState != E_ZTIMER_STATE_RUNNING)
        {
            continue;
        }

        DBG_vPrintf(TRACE_ZTIMER, "ZT: Processing timer %d - time %d\n", n, psTimer->Time);

        if(psTimer->Time  > u32CurrentTime)
        {
            /* If the timer has not expired, move on to the next one */
            continue;
        }
        /* Mark the timer as expired. We must do this _before_ calling the callback
         * in case the user restarts the timer in the callback */
        psTimer->eState = E_ZTIMER_STATE_EXPIRED;

        DBG_vPrintf(TRACE_ZTIMER, "ZT: Timer %d expired\n", n);

        /* If this timer should prevent sleeping while running, decrement the activity count */
        if(psTimer->u8Flags & ZTIMER_FLAG_PREVENT_SLEEP)
        {
            PWRM_eFinishActivity();        
        }

        /* If the timer has  a valid callback, call it */
        if(psTimer->pfCallback != NULL)
        {
            psTimer->pfCallback(psTimer->pvParameters);
        }
    }
}

/****************************************************************************
 *
 * NAME: ZTIMER_eOpen
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * ZTIMER_teStatus
 *
 ****************************************************************************/
PUBLIC ZTIMER_teStatus ZTIMER_eOpen(uint8 *pu8TimerIndex, ZTIMER_tpfCallback pfCallback, void *pvParams, uint8 u8Flags)
{
    int n;
    ZTIMER_tsTimer *psTimer;

    DBG_vPrintf(TRACE_ZTIMER, "ZT: Open: ");

    /* Find a unused timer */
    for(n = 0; n < ZTIMER_sCommon.u8NumTimers; n++)
    {
        psTimer = &ZTIMER_sCommon.psTimers[n];

        if(psTimer->eState == E_ZTIMER_STATE_CLOSED)
        {
            psTimer->u8Flags             = u8Flags;
            psTimer->pvParameters        = pvParams;
            psTimer->pfCallback          = pfCallback;
            psTimer->Time                = 0;
            psTimer->eState              = E_ZTIMER_STATE_STOPPED;

            /* Return the index of the timer */
            *pu8TimerIndex = n;

            DBG_vPrintf(TRACE_ZTIMER, "Success (%d)\n", n);

            return E_ZTIMER_OK;
        }
    }

    DBG_vPrintf(TRACE_ZTIMER, "Failed\n");

    return E_ZTIMER_FAIL;
}

/****************************************************************************
 *
 * NAME: ZTIMER_eClose
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * ZTIMER_teStatus
 *
 ****************************************************************************/
PUBLIC ZTIMER_teStatus ZTIMER_eClose(uint8 u8TimerIndex)
{
    DBG_vPrintf(TRACE_ZTIMER, "ZT: Close (%d): ", u8TimerIndex);

    if(u8TimerIndex > ZTIMER_sCommon.u8NumTimers)
    {
        DBG_vPrintf(TRACE_ZTIMER, "Failed\n");
        return E_ZTIMER_FAIL;
    }

    /* If the timer is currently running, decrease power manager activity count */
    if(ZTIMER_sCommon.psTimers[u8TimerIndex].eState == E_ZTIMER_STATE_RUNNING)
    {
        PWRM_eFinishActivity();
    }

    ZTIMER_sCommon.psTimers[u8TimerIndex].eState = E_ZTIMER_STATE_CLOSED;

    DBG_vPrintf(TRACE_ZTIMER, "Success\n");

    return E_ZTIMER_OK;
}

/****************************************************************************
 *
 * NAME: ZTIMER_eStart
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * ZTIMER_teStatus
 *
 ****************************************************************************/
PUBLIC ZTIMER_teStatus ZTIMER_eStart(uint8 u8TimerIndex, uint32 u32Time)
{
    DBG_vPrintf(TRACE_ZTIMER, "ZT: Start (%d): ", u8TimerIndex);

    /* Check timer index is valid and the timer has previously been opened */
    if(u8TimerIndex > ZTIMER_sCommon.u8NumTimers || ZTIMER_sCommon.psTimers[u8TimerIndex].eState == E_ZTIMER_STATE_CLOSED || u32Time == 0)
    {
        DBG_vPrintf(TRACE_ZTIMER, "Failed\n");
        return E_ZTIMER_FAIL;
    }

    /* If this timer should prevent sleeping while running and the timer is not currently running, increase power manager activity count */
    if((ZTIMER_sCommon.psTimers[u8TimerIndex].u8Flags & ZTIMER_FLAG_PREVENT_SLEEP) && (ZTIMER_sCommon.psTimers[u8TimerIndex].eState != E_ZTIMER_STATE_RUNNING))
    {
        PWRM_eStartActivity();
    }

    /* Load the timer and start it */
    ZTIMER_sCommon.psTimers[u8TimerIndex].Time = TMR_GetTimestamp()/1000 + u32Time;
    ZTIMER_sCommon.psTimers[u8TimerIndex].eState = E_ZTIMER_STATE_RUNNING;

    DBG_vPrintf(TRACE_ZTIMER, "Success\n");

    return E_ZTIMER_OK;
}

/****************************************************************************
 *
 * NAME: ZTIMER_eStop
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * ZTIMER_teStatus
 *
 ****************************************************************************/
PUBLIC ZTIMER_teStatus ZTIMER_eStop(uint8 u8TimerIndex)
{
    DBG_vPrintf(TRACE_ZTIMER, "ZT: Stop (%d): ", u8TimerIndex);

    /* Check timer index is valid and the timer has previously been opened */
    if(u8TimerIndex > ZTIMER_sCommon.u8NumTimers || ZTIMER_sCommon.psTimers[u8TimerIndex].eState == E_ZTIMER_STATE_CLOSED)
    {
        DBG_vPrintf(TRACE_ZTIMER, "Failed\n");
        return E_ZTIMER_FAIL;
    }

    /* If this timer should prevent sleeping while running and the timer is currently running, decrease power manager activity count */
    if((ZTIMER_sCommon.psTimers[u8TimerIndex].u8Flags & ZTIMER_FLAG_PREVENT_SLEEP) && (ZTIMER_sCommon.psTimers[u8TimerIndex].eState == E_ZTIMER_STATE_RUNNING))
    {
        PWRM_eFinishActivity();
    }

    /* Stop the timer */
    ZTIMER_sCommon.psTimers[u8TimerIndex].eState = E_ZTIMER_STATE_STOPPED;

    DBG_vPrintf(TRACE_ZTIMER, "Success\n");

    return E_ZTIMER_OK;
}

/****************************************************************************
 *
 * NAME: ZTIMER_eGetState
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * ZTIMER_teState
 *
 ****************************************************************************/
PUBLIC ZTIMER_teState ZTIMER_eGetState(uint8 u8TimerIndex)
{
     return ZTIMER_sCommon.psTimers[u8TimerIndex].eState;
}

/****************************************************************************/
/***        Local Functions                                                 */
/****************************************************************************/

/****************************************************************************/
/*          END OF FILE                                                     */
/****************************************************************************/
