/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_power_on_counter.c
\brief      ZLO Demo : Power On Counter(Implementation)
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>
#include "dbg.h"
#include "ZTimer.h"
#include "app_main.h"
#include "PDM.h"

#include "app_events.h"
//#include "app_zbp_utilities.h"
#include "PDM_IDs.h"

#include "app_power_on_counter.h"
#include "App_LightTemperatureOccupancySensor.h"
#include "app_blink_led.h"
#include "zcl_common.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_POWER_ON_COUNTER
    #define TRACE_POWER_ON_COUNTER   TRUE
#else
    #define TRACE_POWER_ON_COUNTER   FALSE
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
PRIVATE tsPowerOnCounts sPowerOnCounts;
PRIVATE bool_t bPreInitFunctionCalled;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vManagePowerOnCountLoadRecord
 *
 * DESCRIPTION:
 * Loads record from PDM before the AfInit.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vManagePowerOnCountLoadRecord(void)
{
    PDM_teStatus eStatus;
    uint16 u16ByteRead;
    eStatus = PDM_eReadDataFromRecord(PDM_ID_POWER_ON_COUNTER,
                                      &sPowerOnCounts,
                                      sizeof(tsPowerOnCounts),
                                      &u16ByteRead);

    DBG_vPrintf(TRACE_POWER_ON_COUNTER, "\nAPP: Status = %d, u8PowerOnCounter =%d\n", eStatus, sPowerOnCounts.u8PowerOnCounter);

}
/****************************************************************************
 *
 * NAME: vManagePowerOnCountInit
 *
 * DESCRIPTION:
 * Initialises the power on counters for triggering events on a sequence of
 * On/Off power cycle to the bulb
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vManagePowerOnCountInit(void)
{

    DBG_vPrintf(TRACE_POWER_ON_COUNTER, "\nManage Power: %s", __FUNCTION__);

    if (FALSE == bPreInitFunctionCalled)
    {
        ZTIMER_eStart(u8TimerPowerOnCount, ZTIMER_TIME_MSEC(POWER_ON_PRE_INIT_COUNTER_DB_IN_MSEC));
    }
    else
    {
        ZTIMER_eStop(u8TimerPowerOnCount);
        sPowerOnCounts.u8PowerOnCounter++;
        PDM_eSaveRecordData(PDM_ID_POWER_ON_COUNTER,
                            &sPowerOnCounts,
                            sizeof(tsPowerOnCounts));

        switch (sPowerOnCounts.u8PowerOnCounter)
        {

        case 3:
                vStartBlinkTimer(APP_KEEP_AWAKE_TIME);
            break;

        case 5:
                vStartBlinkTimer(APP_KEEP_AWAKE_TIME);
            break;

        case 7:
            break;

        default :
            break;

        }

        ZTIMER_eStart(u8TimerPowerOnCount, ZTIMER_TIME_MSEC(POWER_ON_COUNTER_DB_IN_MSEC));
    }
}

/****************************************************************************
 *
 * NAME: APP_vManagePowerOnCount
 *
 * DESCRIPTION:
 * OS task for the power on counters for triggering events on a sequence of
 * On/Off power cycle to the bulb
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAPP_cbTimerPowerOnCount( void * pvParam)
{
    DBG_vPrintf(TRACE_POWER_ON_COUNTER, "\nManage Power: %s", __FUNCTION__);
    if (FALSE == bPreInitFunctionCalled)
    {
        DBG_vPrintf(TRACE_POWER_ON_COUNTER, "\nManage Power: Pre Init");
        /*Pre Init over */
        bPreInitFunctionCalled=TRUE;
        /*Second call to the init will start timer and increment the vales also*/
        vManagePowerOnCountInit();
    }
    else
    {
        APP_tsEvent sPowerOnEvent;

        sPowerOnEvent.eType = APP_E_EVENT_NONE;
        DBG_vPrintf(TRACE_POWER_ON_COUNTER, "\nManage Power: Stopping Timer");
        ZTIMER_eStop(u8TimerPowerOnCount);
        switch (sPowerOnCounts.u8PowerOnCounter)
        {
            case 3:
                DBG_vPrintf(TRACE_POWER_ON_COUNTER, "\nManage Power: APP_E_EVENT_KEEPALIVE_START");
                sPowerOnEvent.eType=APP_E_EVENT_KEEPALIVE_START;
                break;
            case 5:
                DBG_vPrintf(TRACE_POWER_ON_COUNTER, "\nManage Power: APP_E_EVENT_FIND_BIND_START");
                sPowerOnEvent.eType=APP_E_EVENT_FIND_BIND_START;
                break;
            default :
                break;

        }
        DBG_vPrintf(TRACE_POWER_ON_COUNTER,"\nPosting Msg = %d\n",sPowerOnEvent.eType);
        ZQ_bQueueSend(&APP_msgAppEvents, &sPowerOnEvent);
        sPowerOnCounts.u8PowerOnCounter=0;
        PDM_eSaveRecordData(PDM_ID_POWER_ON_COUNTER,
                           &sPowerOnCounts,
                           sizeof(tsPowerOnCounts));
    }
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
