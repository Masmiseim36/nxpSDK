/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
#include "App_LightTemperatureOccupancySensor.h"
#include "app_zcl_tick_handler.h"
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
