/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       app_power_on_counter.c
\brief      ZLO Demo : Stack <-> Light-App Interaction (Implementation)
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>
#include "app_main.h"
#include "dbg.h"
#include "PDM.h"
#include "ZTimer.h"
#include "ZQueue.h"
#include "app_events.h"
#include "PDM_IDs.h"

#include "app_power_on_counter.h"


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_POWER_ON_COUNTER
    #define TRACE_POWER_ON_COUNTER   TRUE
#else
    #define TRACE_POWER_ON_COUNTER   FALSE
#endif

#ifdef GP_CERITIFICATION_TEST
#define CONFIG_RESET_GP_TABLES                   2
#else
#define CONFIG_RESET_GP_TABLES                   0
#endif

#define CONFIG_FIND_BIND_POWER_CYCLES           3
#define CONFIG_FCLEAR_BINDING_POWER_CYCLES       5
#define CONFIG_FACTORY_NEW_POWER_CYCLES         7

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

   if (FALSE == bPreInitFunctionCalled)
   {
      ZTIMER_eStart(u8TimerPowerOn, ZTIMER_TIME_MSEC(POWER_ON_PRE_INIT_COUNTER_DB_IN_MSEC));
      DBG_vPrintf(TRUE, "vManagePowerOnCountInit\n");
   }
   else
   {
      ZTIMER_eStop(u8TimerPowerOn);
      sPowerOnCounts.u8PowerOnCounter++;
        PDM_eSaveRecordData(PDM_ID_POWER_ON_COUNTER,
                            &sPowerOnCounts,
                            sizeof(tsPowerOnCounts));
      switch (sPowerOnCounts.u8PowerOnCounter)
      {
         #if ((CONFIG_FIND_BIND_POWER_CYCLES != 0) && (CONFIG_FIND_BIND_BLINK == TRUE))
         {
            case CONFIG_FIND_BIND_POWER_CYCLES:
            {
               vStartEffect(0);
            }
            break;
         }
         #endif

         #if ((CONFIG_FACTORY_RESET_POWER_CYCLES != 0) && (CONFIG_FACTORY_RESET_BLINK == TRUE))
         {
            case CONFIG_FACTORY_RESET_POWER_CYCLES:
            {
               vStartEffect(0);
            }
            break;
         }
         #endif

         #if ((CONFIG_FACTORY_NEW_POWER_CYCLES != 0) && (CONFIG_FACTORY_NEW_BLINK == TRUE))
         {
            case CONFIG_FACTORY_NEW_POWER_CYCLES:
            {
               vStartEffect(0);
            }
            break;
         }
         #endif

         default :
            break;
      }
      ZTIMER_eStart(u8TimerPowerOn, ZTIMER_TIME_MSEC(POWER_ON_COUNTER_DB_IN_MSEC));
   }
}

/****************************************************************************
 *
 * NAME: APP_cbTimerPowerCount
 *
 * DESCRIPTION:
 * OS task for the power on counters for triggering events on a sequence of
 * On/Off power cycle to the bulb
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerPowerCount( void *pvParam)
{
   if (FALSE == bPreInitFunctionCalled)
   {
      /*Pre Init over */
      bPreInitFunctionCalled=TRUE;
      /*Second call to the init will start timer and increment the values also*/
      vManagePowerOnCountInit();
   }
   else
   {
      APP_tsEvent sPowerOnEvent;
      sPowerOnEvent.eType = APP_E_EVENT_NONE;
      ZTIMER_eStop(u8TimerPowerOn);
      switch (sPowerOnCounts.u8PowerOnCounter)
      {
         #if (CONFIG_FIND_BIND_POWER_CYCLES != 0)
         {
            case CONFIG_FIND_BIND_POWER_CYCLES:
            {
               sPowerOnEvent.eType =  APP_EVENT_POR_FIND_BIND;
            }
            break;
         }
         #endif
            #if (CONFIG_RESET_GP_TABLES != 0)
            {
                case CONFIG_RESET_GP_TABLES:
                {
                    sPowerOnEvent.eType =  APP_EVENT_POR_RESET_GP_TABLES;
                }
                break;
            }
            #endif
         #if (CONFIG_FCLEAR_BINDING_POWER_CYCLES != 0)
         {
            case CONFIG_FCLEAR_BINDING_POWER_CYCLES:
            {
                sPowerOnEvent.eType = APP_EVENT_POR_CLEAR_BINDINGS;
            }
            break;
         }
         #endif

         #if (CONFIG_FACTORY_NEW_POWER_CYCLES != 0)
         {
            case CONFIG_FACTORY_NEW_POWER_CYCLES:
            {
                DBG_vPrintf(TRUE, "got here \n");
            	sPowerOnEvent.eType = APP_E_EVENT_LEAVE_AND_RESET;
            }
            break;
         }
         #endif

         default:

         break;
      }

      if (sPowerOnEvent.eType != APP_E_EVENT_NONE)
      {
          DBG_vPrintf(TRUE,"\nPosting Msg = %d\n",sPowerOnEvent.eType);
          ZQ_bQueueSend( &APP_msgAppEvents, &sPowerOnEvent);
      }
      sPowerOnCounts.u8PowerOnCounter = 0;
        PDM_eSaveRecordData(PDM_ID_POWER_ON_COUNTER,
                &sPowerOnCounts,
                sizeof(tsPowerOnCounts));

   }
}

/****************************************************************************
 *
 * NAME: vManagePowerOnCountReset
 *
 * DESCRIPTION:
 * reset the power on counter and save it
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vManagePowerOnCountReset(void)
{
    ZTIMER_eStop(u8TimerPowerOn);
    sPowerOnCounts.u8PowerOnCounter =0;
    PDM_eSaveRecordData(PDM_ID_POWER_ON_COUNTER,
                        &sPowerOnCounts,
                        sizeof(tsPowerOnCounts));
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
