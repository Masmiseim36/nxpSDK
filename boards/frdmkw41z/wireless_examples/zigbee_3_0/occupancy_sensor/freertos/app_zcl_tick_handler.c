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

/*!=================================================================================================
\file       app_zcl_tick_handler.c
\brief      ZLO Demo: Ticks the ZCL and handles ticks while asleep
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "dbg.h"
#include "App_OccupancySensor.h"
#include "app_zcl_tick_handler.h"
#include "ZTimer.h"
#include "app_main.h"
#include "PWR_Interface.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_ZCL_PROCESSING
    #define TRACE_ZCL_PROCESSING   TRUE
#else
    #define TRACE_ZCL_PROCESSING   FALSE
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE uint32 u32NumberOfZCLTicksSinceLastReport = 0;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:        vDecrementTickCount
 *
 * DESCRIPTION: Increments the count since last report or resets it if
 * it reaches the max report interval
 *
 ****************************************************************************/
PUBLIC void vDecrementTickCount(void)
{
    if (u32NumberOfZCLTicksSinceLastReport == ZLO_MAX_REPORT_INTERVAL)
    {
        u32NumberOfZCLTicksSinceLastReport = 0;
    }
    else
    {
        u32NumberOfZCLTicksSinceLastReport++;
    }
}

/****************************************************************************
 *
 * NAME:        u32GetNumberOfZCLTicksSinceReport
 *
 * DESCRIPTION: Gets the number of ticks since the last report
 *
 ****************************************************************************/
PUBLIC uint32 u32GetNumberOfZCLTicksSinceReport(void)
{
    return u32NumberOfZCLTicksSinceLastReport;
}

/****************************************************************************
 *
 * NAME:        vUpdateZCLTickSinceSleep
 *
 * DESCRIPTION: Is called when woken up and ticks the ZCL the amount of seconds
 * it was asleep for.
 *
 ****************************************************************************/
PUBLIC void vUpdateZCLTickSinceSleep(void)
{
    uint32_t lpSleepTimeSeconds;
    ZPS_tsAfEvent sStackEvent;
    tsZCL_CallBackEvent sCallBackEvent;
    sCallBackEvent.pZPSevent = &sStackEvent;
    lpSleepTimeSeconds = PWR_GetTotalSleepDurationMS() / 1000;
    PWR_ResetTotalSleepDuration();
    
    while(lpSleepTimeSeconds)
    {
        sCallBackEvent.eEventType = E_ZCL_CBET_TIMER;
        vZCL_EventHandler(&sCallBackEvent);
        lpSleepTimeSeconds--;
    }

    u32NumberOfZCLTicksSinceLastReport = 0;
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
