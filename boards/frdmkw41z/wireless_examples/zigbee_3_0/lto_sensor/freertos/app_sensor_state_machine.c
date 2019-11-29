/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_sensor_state_machine.c
\brief      ZLO Demo: Processes all stack events depending on it's state
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "dbg.h"
#include "app_sensor_state_machine.h"
#include "app_common.h"
#include "pwrm.h"
#include "App_LightTemperatureOccupancySensor.h"

#include "app_nwk_event_handler.h"
//#include "app_zbp_utilities.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "bdb_api.h"
#include "app_zlo_sensor_node.h"
#include "app_zcl_cfg.h"
#ifdef APP_NTAG_ICODE
#include "app_ntag_icode.h"
#endif
#include "TimersManager.h"
#include "app_events.h"
#include "app_main.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_SENSOR_STATE
    #define TRACE_SENSOR_STATE   TRUE
#else
    #define TRACE_SENSOR_STATE   FALSE
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static void vWakeTimerCallback(void * p);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vAppHandleRunning
 *
 * DESCRIPTION:
 * Is called to handle stack events when we are currently in the network
 *
 ****************************************************************************/
PUBLIC void vAppHandleRunning(ZPS_tsAfEvent* psStackEvent)
{

    DBG_vPrintf(TRACE_SENSOR_STATE, "\nAPP State Event: Running %02x\n", psStackEvent->eType);

    switch(psStackEvent->eType)
    {

    case ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE:
        vHandleNetworkJoinEndDevice();
        #ifdef APP_NTAG_ICODE
        {
            /* Not a rejoin ? */
            if (FALSE == psStackEvent->uEvent.sNwkJoinedEvent.bRejoin)
            {
                /* Write network data to tag */
                APP_vNtagStart(LIGHTTEMPERATUREOCCUPANCYSENSOR_SENSOR_ENDPOINT);
            }
        }
        #endif
        break;

    case ZPS_EVENT_NWK_LEAVE_INDICATION:
        vHandleNetworkLeave(psStackEvent);
        break;

    case ZPS_EVENT_NWK_POLL_CONFIRM:
        vHandlePollResponse(psStackEvent);
        break;

    case ZPS_EVENT_NWK_LEAVE_CONFIRM:
        vHandleNetworkLeaveConfirm(psStackEvent);
        break;

    default:
        break;

    }

}

/****************************************************************************
 *
 * NAME: vAppHandleStartup
 *
 * DESCRIPTION:
 * Is called to handle stack events when we are currently not in the network
 *
 ****************************************************************************/
PUBLIC void vAppHandleStartup(void)
{
    DBG_vPrintf(TRACE_SENSOR_STATE, "\nAPP State Event: StartUp");
    BDB_eNsStartNwkSteering();
    sDeviceDesc.eNodeState = E_JOINING_NETWORK;
}

/****************************************************************************
 *
 * NAME: vStartWakeTimer
 *
 * DESCRIPTION:
 * Starts wake timer 0
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStartWakeTimer(uint16 u16Tick)
{
    static tmrTimerID_t tmr = gTmrInvalidTimerID_c;
    
    if (tmr != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(tmr);
        tmr = gTmrInvalidTimerID_c;
    }
    
    if (tmr == gTmrInvalidTimerID_c)
    {
        tmr = TMR_AllocateTimer();
        TMR_StartLowPowerTimer(tmr,
                               gTmrSingleShotTimer_c,
                               TmrSeconds(u16Tick),
                               vWakeTimerCallback,
                               ((void *)(uint32_t)tmr));
    }
}

/****************************************************************************
 *
 * NAME: vWakeTimerCallback
 *
 * DESCRIPTION:
 * Callback for wakeup timer expire
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
static void vWakeTimerCallback(void * p)
{
    APP_tsEvent event;

    event.eType = APP_E_EVENT_WAKE_TIMER;

    if(ZQ_bQueueSend(&APP_msgAppEvents, &event) == FALSE)
    {
        DBG_vPrintf(TRUE, "Failed to post Event %d \n", event.eType);
    }
    
    TMR_FreeTimer((tmrTimerID_t)((uint32_t)p));
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
