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
\file       app_zcl_sensor_task.c
\brief      ZLO Light sensor Control Behavior (Implementation)
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "FunctionLib.h"
#include "LED.h"

#include "ZTimer.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "pwrm.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_apl_aib.h"
#include "zps_apl_zdp.h"
#include "RNG_Interface.h"
#include "zcl_options.h"
#include "zcl.h"
#include "app_common.h"
#include "app_zlo_sensor_node.h"
#include "app_events.h"
#include "LightingBoard.h"
#include "app_main.h"
#include "app_zcl_tick_handler.h"
#include "app_zcl_sensor_task.h"
#include "App_LightSensor.h"
#ifdef CLD_OTA
    #include "app_ota_client.h"
#endif

#include "app_reporting.h"
#include "app_light_sensor_buttons.h"
#include "app_blink_led.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_ZCL
    #define TRACE_ZCL   TRUE
#else
    #define TRACE_ZCL   FALSE
#endif

#ifdef DEBUG_SENSOR_TASK
    #define TRACE_SENSOR_TASK   TRUE
#else
    #define TRACE_SENSOR_TASK   FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void APP_ZCL_cbGeneralCallback(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_ZCL_cbEndpointCallback(tsZCL_CallBackEvent *psEvent);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
//extern PDM_tsRecordDescriptor sDevicePDDesc;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_ZCL_vInitialise
 *
 * DESCRIPTION:
 * Initialises ZCL related functions
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_ZCL_vInitialise(void)
{
    teZCL_Status eZCL_Status;

    /* Initialise ZCL */
    eZCL_Status = eZCL_Initialise(&APP_ZCL_cbGeneralCallback, apduZCL);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: Error eZCL_Initialise returned %d", eZCL_Status);
    }

    /* Register ZLO EndPoint */
    eZCL_Status = eApp_ZCL_RegisterEndpoint(&APP_ZCL_cbEndpointCallback);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_SENSOR_TASK, "\nAPP_ZCL: Error: eApp_ZCL_RegisterEndpoint:%d", eZCL_Status);
    }

    DBG_vPrintf(TRACE_SENSOR_TASK, "\nAPP_ZCL: Chan Mask %08x", ZPS_psAplAibGetAib()->apsChannelMask);
    DBG_vPrintf(TRACE_SENSOR_TASK, "\nAPP_ZCL: RxIdle TRUE");

    ZTIMER_eStart(u8TimerTick, ZCL_TICK_TIME);

    vAPP_ZCL_DeviceSpecific_Init();
#ifdef CLD_OTA
    vAppInitOTA();
#endif
}

/****************************************************************************
 *
 * NAME: ZCL_Task
 *
 * DESCRIPTION:
 * ZCL Task for the sensor
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerZclTick( void *pvParam)
{
  
#ifdef CLD_OTA
    static uint32 u32Tick1Sec = 99;  
    tsZCL_CallBackEvent sCallBackEvent;
#endif
    
#ifndef CLD_OTA
    vAPP_ZCL_Tick();
    ZTIMER_eStart(u8TimerTick, ZCL_TICK_TIME);
#else
    ZTIMER_eStart(u8TimerTick, ZTIMER_TIME_MSEC(10));
#endif
    
#ifdef CLD_OTA    
    u32Tick1Sec++;
    if (u32Tick1Sec == 82)   /* offset this from the 1 second roll over */
    {
        vRunAppOTAStateMachine();
    }

        /* Wrap the 1 second  counter and provide 1Hz ticks to cluster */
    if(u32Tick1Sec > 99)
    {
        u32Tick1Sec = 0;
        sCallBackEvent.pZPSevent = NULL;
        sCallBackEvent.eEventType = E_ZCL_CBET_TIMER;
        vZCL_EventHandler(&sCallBackEvent);  
    }
#endif  

}

/****************************************************************************
 *
 * NAME: APP_ZCL_vEventHandler
 *
 * DESCRIPTION:
 * Sends the stack event to the ZCL event handler.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_ZCL_vEventHandler(ZPS_tsAfEvent* psStackEvent)
{
    tsZCL_CallBackEvent sCallBackEvent;

    sCallBackEvent.pZPSevent = psStackEvent;
    sCallBackEvent.eEventType = E_ZCL_CBET_ZIGBEE_EVENT;
    vZCL_EventHandler(&sCallBackEvent);
}

PUBLIC void vAPP_ZCL_Tick(void)
{
    ZPS_tsAfEvent sStackEvent;
    tsZCL_CallBackEvent sCallBackEvent;
    sCallBackEvent.pZPSevent = &sStackEvent;

    vDecrementTickCount();
    sCallBackEvent.eEventType = E_ZCL_CBET_TIMER;
    vZCL_EventHandler(&sCallBackEvent);
}
/****************************************************************************
 *
 * NAME: APP_ZCL_cbGeneralCallback
 *
 * DESCRIPTION:
 * General callback for ZCL events
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_ZCL_cbGeneralCallback(tsZCL_CallBackEvent *psEvent)
{

    switch (psEvent->eEventType)
    {

    case E_ZCL_CBET_UNHANDLED_EVENT:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EVT Unhandled Event");
        break;

    case E_ZCL_CBET_READ_ATTRIBUTES_RESPONSE:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EVT Read attributes response");
        break;

    case E_ZCL_CBET_READ_REQUEST:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EVT Read request");
        break;

    case E_ZCL_CBET_DEFAULT_RESPONSE:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EVT Default response");
        break;

    case E_ZCL_CBET_ERROR:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EVT Error");
        break;

    case E_ZCL_CBET_TIMER:
        break;

    case E_ZCL_CBET_ZIGBEE_EVENT:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EVT ZigBee");
        break;

    case E_ZCL_CBET_CLUSTER_CUSTOM:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EP EVT Custom");
        break;

    default:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: Invalid event type");
        break;

    }
}

/****************************************************************************
 *
 * NAME: APP_ZCL_cbEndpointCallback
 *
 * DESCRIPTION:
 * Endpoint specific callback for ZCL events
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_ZCL_cbEndpointCallback(tsZCL_CallBackEvent *psEvent)
{

    switch (psEvent->eEventType)
    {

    case E_ZCL_CBET_REPORT_INDIVIDUAL_ATTRIBUTES_CONFIGURE:
        {
            tsZCL_AttributeReportingConfigurationRecord    *psAttributeReportingRecord= &psEvent->uMessage.sAttributeReportingConfigurationRecord;
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: Individual Configure attribute for Cluster = %d",psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum);
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: eAttributeDataType = %d",psAttributeReportingRecord->eAttributeDataType);
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: u16AttributeEnum = %d",psAttributeReportingRecord->u16AttributeEnum );
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: u16MaximumReportingInterval = %d",psAttributeReportingRecord->u16MaximumReportingInterval );
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: u16MinimumReportingInterval = %d",psAttributeReportingRecord->u16MinimumReportingInterval );
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: u16TimeoutPeriodField = %d",psAttributeReportingRecord->u16TimeoutPeriodField );
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: u8DirectionIsReceived = %d",psAttributeReportingRecord->u8DirectionIsReceived );
            DBG_vPrintf(TRACE_ZCL,"\nAPP_ZCL: uAttributeReportableChange = %d",psAttributeReportingRecord->uAttributeReportableChange );
            if (E_ZCL_SUCCESS == psEvent->eZCL_Status)
            {
                if(MEASUREMENT_AND_SENSING_CLUSTER_ID_ILLUMINANCE_MEASUREMENT == psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum)
                {
                    vSaveReportableRecord(MEASUREMENT_AND_SENSING_CLUSTER_ID_ILLUMINANCE_MEASUREMENT,psAttributeReportingRecord);
                }
            }
        }
        break;

    case E_ZCL_CBET_UNHANDLED_EVENT:
    case E_ZCL_CBET_READ_ATTRIBUTES_RESPONSE:
    case E_ZCL_CBET_READ_REQUEST:
    case E_ZCL_CBET_DEFAULT_RESPONSE:
    case E_ZCL_CBET_ERROR:
    case E_ZCL_CBET_TIMER:
    case E_ZCL_CBET_ZIGBEE_EVENT:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EP EVT No action");
        break;

    case E_ZCL_CBET_READ_INDIVIDUAL_ATTRIBUTE_RESPONSE:
        DBG_vPrintf(TRACE_SENSOR_TASK, "\nAPP_ZCL: Read Attrib Rsp %d %02x", psEvent->uMessage.sIndividualAttributeResponse.eAttributeStatus,
            *((uint8*)psEvent->uMessage.sIndividualAttributeResponse.pvAttributeData));
        break;

    case E_ZCL_CBET_CLUSTER_CUSTOM:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EP EVT: Custom %04x", psEvent->uMessage.sClusterCustomMessage.u16ClusterId);

        switch (psEvent->uMessage.sClusterCustomMessage.u16ClusterId)
        {

        case GENERAL_CLUSTER_ID_BASIC:
        {
            tsCLD_BasicCallBackMessage *psCallBackMessage = (tsCLD_BasicCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
            if (psCallBackMessage->u8CommandId == E_CLD_BASIC_CMD_RESET_TO_FACTORY_DEFAULTS )
            {
                DBG_vPrintf(TRACE_ZCL, "Basic Factory Reset Received\n");
                /* resetting the sensor structure back to zero*/
                FLib_MemSet(&sSensor,0,sizeof(tsZLO_LightSensorDevice));
                vAPP_ZCL_DeviceSpecific_Init();
				#ifdef CLD_OTA
                	vAppInitOTA();
                #endif
                eApp_ZCL_RegisterEndpoint(&APP_ZCL_cbEndpointCallback);
            }
        }
        break;
			
#ifdef CLD_OTA
            case OTA_CLUSTER_ID:
            {
                tsOTA_CallBackMessage *psCallBackMessage = (tsOTA_CallBackMessage *)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
                vHandleAppOtaClient(psCallBackMessage);
            }
            break;
#endif
        case GENERAL_CLUSTER_ID_IDENTIFY:
            {
                DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: for identify cluster");
                tsCLD_IdentifyCallBackMessage *psCallBackMessage = (tsCLD_IdentifyCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
                if (psCallBackMessage->u8CommandId == E_CLD_IDENTIFY_CMD_IDENTIFY)
                {
                    DBG_vPrintf(TRACE_ZCL, "\nEP E_CLD_IDENTIFY_CMD_IDENTIFY");
                    /* provide callback to BDB handler for identify query response on initiator*/
                    if(psEvent->psClusterInstance->bIsServer == FALSE)
                    {
                        tsBDB_ZCLEvent  sBDBZCLEvent;
                        DBG_vPrintf(TRACE_ZCL, "\nCallBackBDB");
                        sBDBZCLEvent.eType = BDB_E_ZCL_EVENT_IDENTIFY_QUERY;
                        sBDBZCLEvent.psCallBackEvent = psEvent;
                        BDB_vZclEventHandler(&sBDBZCLEvent);
                    }
                    else
                    {
                        if(sSensor.sIdentifyServerCluster.u16IdentifyTime == 0)
                        {
                            vStopBlinkTimer();
                        }
                    }
                }
                else if((psCallBackMessage->u8CommandId == E_CLD_IDENTIFY_CMD_TRIGGER_EFFECT) &&
                        psEvent->psClusterInstance->bIsServer)
                {
                    DBG_vPrintf(TRACE_ZCL, "Trigger Effect ID %d Vr %d\r\n",
                            psCallBackMessage->uMessage.psTriggerEffectRequestPayload->eEffectId,
                            psCallBackMessage->uMessage.psTriggerEffectRequestPayload->u8EffectVarient);
                }
            }
            break;


        case GENERAL_CLUSTER_ID_GROUPS:
            DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: for groups cluster");
            break;

        case 0x1000:
            DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: - for 0x1000");
            break;

        default:
            DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: for unknown cluster %d", psEvent->uMessage.sClusterCustomMessage.u16ClusterId);
            break;
        }
        break;

    case E_ZCL_CBET_CLUSTER_UPDATE:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: Update Id %04x", psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum);
        break;

    default:
        DBG_vPrintf(TRACE_ZCL, "\nAPP_ZCL: EP EVT Invalid event type");
        break;
    }

}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
