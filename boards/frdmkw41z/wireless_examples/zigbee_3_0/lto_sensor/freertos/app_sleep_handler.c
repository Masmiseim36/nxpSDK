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
\file       app_sleep_handler.c
\brief      ZLO Demo : Manages sleep configuration.
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>
#include "dbg.h"
#include "ZTimer.h"
#include "app_main.h"
#include "pwrm.h"
#include "app_common.h"
#include "zcl_options.h"
#include "zcl_common.h"
#include "app_sleep_handler.h"
#include "app_zcl_tick_handler.h"
#include "app_zcl_sensor_task.h"
#include "app_sensor_state_machine.h"
#include "App_LightTemperatureOccupancySensor.h"
#include "app_zlo_sensor_node.h"
#include "app_blink_led.h"
#include "LED.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_SLEEP_HANDLER
    #define TRACE_SLEEP_HANDLER   TRUE
#else
    #define TRACE_SLEEP_HANDLER   FALSE
#endif

#if ZLO_MAX_REPORT_INTERVAL == 0
    #define MAXIMUM_TIME_TO_SLEEP (APP_OCCUPANCY_SENSOR_OCCUPIED_TO_UNOCCUPIED_DELAY + 1)
#else
    #define MAXIMUM_TIME_TO_SLEEP (ZLO_MAX_REPORT_INTERVAL)
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE uint8 u8NumberOfTimersTaskTimers(void);
PRIVATE void vStopNonSleepPreventingTimers(void);
PRIVATE void vStartNonSleepPreventingTimers(void);
PRIVATE uint8 u8NumberOfNonSleepPreventingTimers(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE pwrm_tsWakeTimerEvent    sWake;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME:        vAttemptToSleep
 *
 * DESCRIPTION: Checks to see if any software timers are running that may
 * prevent us from going to sleep. If there is none, if wake timer 0 is
 * running, schedule none deep sleep, if there is schedule deep sleep
 * which is checked if its enabled in vScheduleSleep.
 *
 ****************************************************************************/
PUBLIC void vAttemptToSleep(void)
{
    DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: Activity Count = %d", PWRM_u16GetActivityCount());
    DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: Task Timers = %d", u8NumberOfTimersTaskTimers());

    /* Only enter here if the activity count is equal to the number of non sleep preventing timers (in other words, the activity count
     * will become zero when we stop them) */
    if ((PWRM_u16GetActivityCount() == u8NumberOfNonSleepPreventingTimers()) &&
        (0 == u8NumberOfTimersTaskTimers()))
    {
        /* Stop any background timers that are non sleep preventing*/
        vStopNonSleepPreventingTimers();

        PWRM_eScheduleActivity(&sWake, (MAXIMUM_TIME_TO_SLEEP - u32GetNumberOfZCLTicksSinceReport())*1000 , vWakeCallBack);
        PWRM_vManagePower();
    }
}


/****************************************************************************/
/***        Local Function                                                ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:        u8NumberOfTimersTaskTimers
 *
 * DESCRIPTION: Checks to see if any timers are running that shouldn't be
 * interrupted by sleeping.
 *
 ****************************************************************************/
PRIVATE uint8 u8NumberOfTimersTaskTimers(void)
{
    uint8 u8NumberOfRunningTimers = 0;

    if (ZTIMER_eGetState(u8TimerPoll) == E_ZTIMER_STATE_RUNNING)
    {
        DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: u8TimerPoll");
        u8NumberOfRunningTimers++;
    }

    if (ZTIMER_eGetState(u8TimerBlink) == E_ZTIMER_STATE_RUNNING)
    {
        DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: u8TimerBlink");
        u8NumberOfRunningTimers++;
    }

    if (ZTIMER_eGetState(u8TimerPowerOnCount) == E_ZTIMER_STATE_RUNNING)
    {
        DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: u8TimerPowerOnCount");
        u8NumberOfRunningTimers++;
    }

#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    if (ZTIMER_eGetState(u8TimerNtag) == E_ZTIMER_STATE_RUNNING)
    {
        DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: APP_TimerNtag");
        u8NumberOfRunningTimers++;
    }
#endif

    return u8NumberOfRunningTimers;
}

/****************************************************************************
 *
 * NAME:        vStopNonSleepPreventingTimers
 *
 * DESCRIPTION: The timers in this function should not stop us from sleep.
 * Stop the timers to reduce the activity count which will prevent sleep.
 *
 ****************************************************************************/
PRIVATE void vStopNonSleepPreventingTimers()
{
    if (ZTIMER_eGetState(u8TimerTick) != E_ZTIMER_STATE_STOPPED)
    {
        ZTIMER_eStop(u8TimerTick);
    }
}

/****************************************************************************
 *
 * NAME:        vStartNonSleepPreventingTimers
 *
 * DESCRIPTION: Start the timers that wont stop us in vAttemptToSleep
 *
 ****************************************************************************/
PRIVATE void vStartNonSleepPreventingTimers(void)
{
    if (ZTIMER_eGetState(u8TimerTick) != E_ZTIMER_STATE_RUNNING)
    {
        ZTIMER_eStart(u8TimerTick, ZCL_TICK_TIME);
    }
}

/****************************************************************************
 *
 * NAME:        u8NumberOfNonSleepPreventingTimers
 *
 * DESCRIPTION: Returns the number of timers that are running that we are
 * prepared to stop before going to sleep.
 *
 ****************************************************************************/
PRIVATE uint8 u8NumberOfNonSleepPreventingTimers(void)
{
    uint8 u8NumberOfRunningTimers = 0;

    if (ZTIMER_eGetState(u8TimerTick) == E_ZTIMER_STATE_RUNNING)
    {
        DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: u8TimerTick");
        u8NumberOfRunningTimers++;
    }

    return u8NumberOfRunningTimers;
}

/****************************************************************************
 *
 * NAME: vWakeCallBack
 *
 * DESCRIPTION:
 * Wake up call back called upon wake up by the schedule activity event.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vWakeCallBack(void)
{
    vUpdateZCLTickSinceSleep();

    DBG_vPrintf(TRACE_SLEEP_HANDLER, "\nAPP Sleep Handler: vWakeCallBack\n");

    APP_vSetLED(LED1, sSensor.sOccupancySensingServerCluster.u8Occupancy);

    /*Start the u8TimerTick to continue the ZCL tasks */
    vStartNonSleepPreventingTimers();
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

