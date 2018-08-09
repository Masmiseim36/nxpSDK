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
\file       app_PIR_OpenCollector_events.c
\brief      ZLO Demo: Driver behaviour of the Open Collector PIR Sensor
==================================================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "dbg.h"
#include "app_sleep_handler.h"
#include "app_PIR_events.h"
#include "app_sensor_state_machine.h"
#include "app_blink_led.h"

#include "LED.h"

#include "app_zlo_sensor_node.h"
#include "app_reporting.h"
#include "App_LightTemperatureOccupancySensor.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_PIR_EVENTS
    #define TRACE_PIR_EVENTS   TRUE
#else
    #define TRACE_PIR_EVENTS  FALSE
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vHandlePIROpenCollectorOccupiedToUnoccupiedTimerExpired(void);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE teRunningTimer u8RunningTimerType = E_APP_TIMER_NONE;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vHandlePIROpenCollectorFallingEdgeEvent
 *
 * DESCRIPTION:
 * We have received a falling edge event which means we are now occupied.
 * Change the occupancy to occupied, turn the LED on, send the report (if we
 * were previously unoccupied. If we were already occupied, stop the occupied
 * to unoccupied timer.
 *
 ****************************************************************************/
PUBLIC void vHandleFallingEdgeEvent(void)
{

    DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP PIR Event: Falling Edge Event");
    if (sSensor.sOccupancySensingServerCluster.u8Occupancy == 0x00)
    {
        /* We were previously unoccupied*/
        DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP PIR Event: Occupied");
        sSensor.sOccupancySensingServerCluster.u8Occupancy = E_CLD_OS_OCCUPIED;
        APP_vSetLED(LED1, sSensor.sOccupancySensingServerCluster.u8Occupancy);
    }
    else
    {
#ifndef CPU_MKW41Z512VHT4
        /* We are already occupied*/
        vAHI_WakeTimerStop(E_AHI_WAKE_TIMER_0);
        DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP PIR Event: Stopped Timer");
#endif
    }
}

/****************************************************************************
 *
 * NAME: vHandlePIROpenCollectorRisingEdgeEvent
 *
 * DESCRIPTION:
 * We have received a rising edge event which means we are now unoccupied.
 * Start the occupied to unoccupied timer and attempt to go to sleep.
 *
 ****************************************************************************/
PUBLIC void vHandleRisingEdgeEvent(void)
{
    DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP PIR Event: Rising Edge Event");
    u8RunningTimerType = E_APP_TIMER_OCCUPIED_TO_UNOCCUPIED;
    if (0 == sSensor.sOccupancySensingServerCluster.u16PIROccupiedToUnoccupiedDelay)
    {
        vHandlePIROpenCollectorOccupiedToUnoccupiedTimerExpired();
    }
    else
    {
        vStartWakeTimer(sSensor.sOccupancySensingServerCluster.u16PIROccupiedToUnoccupiedDelay);
    }
    DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP Occupancy Event: Occupied Timer Start");
}

/****************************************************************************
 *
 * NAME: vHandlePIROpenCollectorNewJoinEvent
 *
 * DESCRIPTION:
 * This method is called when we have just joined a new network, rejoined
 * a network or we have just come out of find and bind. If we are occupied,
 * send a report out, if we are unoccupied, just start the occupied to unoccupied
 * delay timer so we aren't potentially turning lights off prematurely.
 *
 ****************************************************************************/
PUBLIC void vHandleNewJoinEvent(void)
{

    DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP PIR Event: New join (RunningTimerType=%d) (Occupancy=%02x)", u8RunningTimerType, sSensor.sOccupancySensingServerCluster.u8Occupancy);
    switch (u8RunningTimerType)
    {

    case E_APP_TIMER_NONE:
    case E_APP_TIMER_OCCUPIED_TO_UNOCCUPIED:
        DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP PIR Event: Starting Occupied to Unoccupied timer");
        u8RunningTimerType = E_APP_TIMER_OCCUPIED_TO_UNOCCUPIED;
        if (0 == sSensor.sOccupancySensingServerCluster.u16PIROccupiedToUnoccupiedDelay)
        {
            vHandlePIROpenCollectorOccupiedToUnoccupiedTimerExpired();
        }
        else
        {
            vStartWakeTimer(sSensor.sOccupancySensingServerCluster.u16PIROccupiedToUnoccupiedDelay);
        }
        break;

    case E_APP_TIMER_UNOCCUPIED_TO_OCCUPIED:
    default:
        break;

    }
}

/****************************************************************************
 *
 * NAME: vHandlePIROpenCollectorWakeTimerExpired
 *
 * DESCRIPTION:
 * Handles a wake timer expired event. We check to see what type of timer
 * event we had running and processes it accordingly.
 *
 ****************************************************************************/
PUBLIC void vHandleWakeTimeoutEvent(void)
{
    switch (u8RunningTimerType)
    {

    case E_APP_TIMER_OCCUPIED_TO_UNOCCUPIED:
        vHandlePIROpenCollectorOccupiedToUnoccupiedTimerExpired();
        break;

    case E_APP_TIMER_UNOCCUPIED_TO_OCCUPIED:
    default:
        break;

    }

}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vHandlePIROpenCollectorOccupiedToUnoccupiedTimerExpired
 *
 * DESCRIPTION:
 * When a timer goes from occupied to unoccupied this method changes it to
 * unoccupied, turns the LED on and sends out a report.
 *
 ****************************************************************************/
PRIVATE void vHandlePIROpenCollectorOccupiedToUnoccupiedTimerExpired(void)
{
    DBG_vPrintf(TRACE_PIR_EVENTS,"\nAPP PIR Event: Unoccupied = %d", sSensor.sOccupancySensingServerCluster.u8Occupancy);
    u8RunningTimerType = E_APP_TIMER_NONE;
    sSensor.sOccupancySensingServerCluster.u8Occupancy = 0x0;
    APP_vSetLED(LED1, sSensor.sOccupancySensingServerCluster.u8Occupancy);
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
