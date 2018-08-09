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
\file       app_zcl_task.c
\brief      This is a template file for the applications zcl task.
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "LED.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "app_zcl_cfg.h"
#include "PDM_IDs.h"
#include "ZTimer.h"
#include "zcl.h"
#include "app_cfg.h"
#include "zcl_options.h"
#include "app_leds.h"
#include "app_zcl_task.h"
#include "app_router_node.h"
#include "app_common.h"
#include "app_main.h"
#include "base_device.h"
#include "app_events.h"
#include <string.h>
#include "app_reporting.h"
#include "bdb_fb_api.h"

#ifdef CLD_OTA
#include "OTA.h"
#include "app_ota_client.h"
#endif

#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef DEBUG_ZCL
#define TRACE_ZCL   TRUE
#else
#define TRACE_ZCL   FALSE
#endif



/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

#define ZCL_TICK_TIME           ZTIMER_TIME_MSEC(100)


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void APP_ZCL_cbGeneralCallback(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_ZCL_cbEndpointCallback(tsZCL_CallBackEvent *psEvent);
PUBLIC void APP_vHandleIdentify(uint16 u16Time);
PRIVATE void APP_vHandleClusterCustomCommands(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_vHandleClusterUpdate(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_vZCL_DeviceSpecific_Init(void);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

tsZHA_BaseDevice sBaseDevice;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE uint8 u8IdentifyCount = 0;
PRIVATE bool_t bIdentifyState = FALSE;


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

    /* Initialise ZLL */
    eZCL_Status = eZCL_Initialise(&APP_ZCL_cbGeneralCallback, apduZCL);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_ZCL, "\r\nErr: eZLO_Initialise:%d", eZCL_Status);
    }

    /* Start the tick timer */
    if(ZTIMER_eStart(u8TimerZCL, ZCL_TICK_TIME) != E_ZTIMER_OK)
    {
        DBG_vPrintf(TRACE_ZCL, "APP: Failed to Start Tick Timer\r\n");
    }

    /* Register Light EndPoint */
   eZCL_Status =  eZHA_RegisterBaseDeviceEndPoint(ROUTER_APPLICATION_ENDPOINT,
                                                   &APP_ZCL_cbEndpointCallback,
                                                   &sBaseDevice);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
            DBG_vPrintf(TRACE_ZCL, "Error: eZHA_RegisterBaseDeviceEndPoint: %02x\r\r\n", eZCL_Status);
    }

    APP_vZCL_DeviceSpecific_Init();
#ifndef CPU_MKW41Z512VHT4
    APP_vSetLed(LED1, sBaseDevice.sOnOffServerCluster.bOnOff);
#else
    if(sBaseDevice.sOnOffServerCluster.bOnOff)
    {
        Led1On();
    }
    else
    {
        Led1Off();
    }
#endif
    
#ifdef CLD_OTA
    vAppInitOTA();
#endif    
}


/****************************************************************************
 *
 * NAME: APP_ZCL_vSetIdentifyTime
 *
 * DESCRIPTION:
 * Sets the remaining time in the identify cluster
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_ZCL_vSetIdentifyTime(uint16 u16Time)
{
    sBaseDevice.sIdentifyServerCluster.u16IdentifyTime = u16Time;
}


/****************************************************************************
 *
 * NAME: APP_cbTimerZclTick
 *
 * DESCRIPTION:
 * Task kicked by the tick timer
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerZclTick(void *pvParam)
{
    static uint32 u32Tick10ms = 9;
    static uint32 u32Tick1Sec = 99;

    tsZCL_CallBackEvent sCallBackEvent;

    if(ZTIMER_eStart(u8TimerZCL, ZTIMER_TIME_MSEC(10)) != E_ZTIMER_OK)
    {
        DBG_vPrintf(TRACE_ZCL, "APP: Failed to Start Tick Timer\r\n");
    }

    u32Tick10ms++;
    u32Tick1Sec++;

    /* Wrap the Tick10ms counter and provide 100ms ticks to cluster */
    if (u32Tick10ms > 9)
    {
        eZCL_Update100mS();
        u32Tick10ms = 0;
    }

#ifdef CLD_OTA
    if (u32Tick1Sec == 82)   /* offset this from the 1 second roll over */
    {
        vRunAppOTAStateMachine();
    }
#endif

    /* Wrap the 1 second  counter and provide 1Hz ticks to cluster */
    if(u32Tick1Sec > 99)
    {
        u32Tick1Sec = 0;
        sCallBackEvent.pZPSevent = NULL;
        sCallBackEvent.eEventType = E_ZCL_CBET_TIMER;
        vZCL_EventHandler(&sCallBackEvent);
    }
}

/****************************************************************************
 *
 * NAME: APP_ZCL_vEventHandler
 *
 * DESCRIPTION:
 * Main ZCL processing task
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_ZCL_vEventHandler(ZPS_tsAfEvent *psStackEvent)
{
    tsZCL_CallBackEvent sCallBackEvent;
    sCallBackEvent.pZPSevent = psStackEvent;

    DBG_vPrintf(TRACE_ZCL, "ZCL_Task endpoint event:%d \r\n", psStackEvent->eType);
    sCallBackEvent.eEventType = E_ZCL_CBET_ZIGBEE_EVENT;
    vZCL_EventHandler(&sCallBackEvent);

}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
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

    case E_ZCL_CBET_LOCK_MUTEX:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Lock Mutex\r\r\n");
        break;

    case E_ZCL_CBET_UNLOCK_MUTEX:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Unlock Mutex\r\r\n");
        break;

    case E_ZCL_CBET_UNHANDLED_EVENT:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Unhandled Event\r\r\n");
        break;

    case E_ZCL_CBET_READ_ATTRIBUTES_RESPONSE:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Read attributes response");
        break;

    case E_ZCL_CBET_READ_REQUEST:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Read request");
        break;

    case E_ZCL_CBET_DEFAULT_RESPONSE:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Default response");
        break;

    case E_ZCL_CBET_ERROR:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Error");
        break;

    case E_ZCL_CBET_TIMER:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: Timer");
        break;

    case E_ZCL_CBET_ZIGBEE_EVENT:
        DBG_vPrintf(TRACE_ZCL, "\r\nEVT: ZigBee");
        break;

    case E_ZCL_CBET_CLUSTER_CUSTOM:
        DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Custom");
        break;

    default:
        DBG_vPrintf(TRACE_ZCL, "\r\nInvalid event type");
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
        case E_ZCL_CBET_LOCK_MUTEX:
            break;

        case E_ZCL_CBET_UNLOCK_MUTEX:
            break;

        case E_ZCL_CBET_UNHANDLED_EVENT:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Unhandled event");
            break;

        case E_ZCL_CBET_READ_INDIVIDUAL_ATTRIBUTE_RESPONSE:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Rd Attr Rsp %04x AS %d", psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum,
                psEvent->uMessage.sIndividualAttributeResponse.eAttributeStatus);
            break;

        case E_ZCL_CBET_READ_ATTRIBUTES_RESPONSE:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Read attributes response");
            break;

        case E_ZCL_CBET_READ_REQUEST:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Read request");
            break;

        case E_ZCL_CBET_DEFAULT_RESPONSE:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Default response");
            break;

        case E_ZCL_CBET_ERROR:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Error");
            break;

        case E_ZCL_CBET_TIMER:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Timer");
            break;

        case E_ZCL_CBET_ZIGBEE_EVENT:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: ZigBee");
            break;

        case E_ZCL_CBET_CLUSTER_CUSTOM:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Custom Cl %04x\r\n", psEvent->uMessage.sClusterCustomMessage.u16ClusterId);
            APP_vHandleClusterCustomCommands(psEvent);
        break;

        case E_ZCL_CBET_WRITE_INDIVIDUAL_ATTRIBUTE:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Write Individual Attribute Status %02x\r\n", psEvent->eZCL_Status);
        break;

        case E_ZCL_CBET_REPORT_INDIVIDUAL_ATTRIBUTE:
        {
            tsZCL_IndividualAttributesResponse    *psIndividualAttributeResponse = &psEvent->uMessage.sIndividualAttributeResponse;
            DBG_vPrintf(TRACE_ZCL,"Individual Report attribute for Cluster = %d\r\n", psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum);
            DBG_vPrintf(TRACE_ZCL,"eAttributeDataType = %d\r\n", psIndividualAttributeResponse->eAttributeDataType);
            DBG_vPrintf(TRACE_ZCL,"u16AttributeEnum = %d\r\n", psIndividualAttributeResponse->u16AttributeEnum );
            DBG_vPrintf(TRACE_ZCL,"eAttributeStatus = %d\r\n", psIndividualAttributeResponse->eAttributeStatus );
        }
            break;

        case E_ZCL_CBET_REPORT_INDIVIDUAL_ATTRIBUTES_CONFIGURE:
        {
            tsZCL_AttributeReportingConfigurationRecord    *psAttributeReportingRecord = &psEvent->uMessage.sAttributeReportingConfigurationRecord;
            DBG_vPrintf(TRACE_ZCL,"Individual Configure Report Cluster %d Attrib %d Type %d Min %d Max %d IntV %d Direcct %d Change %d\r\n",
                    psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum,
                    psAttributeReportingRecord->u16AttributeEnum,
                    psAttributeReportingRecord->eAttributeDataType,
                    psAttributeReportingRecord->u16MinimumReportingInterval,
                    psAttributeReportingRecord->u16MaximumReportingInterval,
                    psAttributeReportingRecord->u16TimeoutPeriodField,
                    psAttributeReportingRecord->u8DirectionIsReceived,
                    psAttributeReportingRecord->uAttributeReportableChange);

            if (E_ZCL_SUCCESS == psEvent->eZCL_Status)
            {

                vSaveReportableRecord( psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum,
                                       psAttributeReportingRecord);

            }
        }
        break;

        case E_ZCL_CBET_CLUSTER_UPDATE:
            DBG_vPrintf(TRACE_ZCL, "Update Id %04x\r\n", psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum);
            APP_vHandleClusterUpdate(psEvent);
            break;
        case E_ZCL_CBET_REPORT_REQUEST:
            break;

        default:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Invalid evt type 0x%x", (uint8)psEvent->eEventType);
            break;
    }
}

/****************************************************************************
 *
 * NAME: APP_vHandleIdentify
 *
 * DESCRIPTION:
 * Application identify handler
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void APP_vHandleIdentify(uint16 u16Time)
{
    static bool bActive = FALSE;
    if (u16Time == 0)
    {
            /*
             * Restore to off/off state
             */
#ifndef CPU_MKW41Z512VHT4
        APP_vSetLed(LED1, sBaseDevice.sOnOffServerCluster.bOnOff);
#else
        if(sBaseDevice.sOnOffServerCluster.bOnOff)
        {
            Led2On();
            Led3On();
            Led4On();
        }
        else
        {
            Led2Off();
            Led3Off();
            Led4Off();
        }
#endif
        bActive = FALSE;
    }
    else
    {
        /* Set the Identify levels */
        if (!bActive) {
            bActive = TRUE;
            u8IdentifyCount = 5;
            bIdentifyState = TRUE;
#ifndef CPU_MKW41Z512VHT4
            APP_vSetLed(LED1, TRUE );
#else
            Led1On();
#endif
        }
    }
}

#ifdef CLD_IDENTIFY_10HZ_TICK
/****************************************************************************
 *
 * NAME: vIdEffectTick
 *
 * DESCRIPTION:
 * ZLO Device Specific identify tick
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vIdEffectTick(uint8 u8Endpoint)
{

    if ((u8Endpoint == ROUTER_APPLICATION_ENDPOINT) &&
        (sBaseDevice.sIdentifyServerCluster.u16IdentifyTime > 0 ))
    {
        u8IdentifyCount--;
        if (u8IdentifyCount == 0)
        {
            u8IdentifyCount = 5;
            bIdentifyState = (bIdentifyState)? FALSE: TRUE;

#ifndef CPU_MKW41Z512VHT4
            APP_vSetLed(LED1, bIdentifyState);
#else
            if(bIdentifyState)
            {
                Led2On();
                Led3On();
                Led4On();
            }
            else
            {
                Led2Off();
                Led3Off();
                Led4Off();
            }
#endif

        }
    }
}
#endif

/****************************************************************************
 *
 * NAME: APP_vHandleClusterCustomCommands
 *
 * DESCRIPTION:
 * callback for ZCL cluster custom command events
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_vHandleClusterCustomCommands(tsZCL_CallBackEvent *psEvent)
{
      switch(psEvent->uMessage.sClusterCustomMessage.u16ClusterId)
    {
        case GENERAL_CLUSTER_ID_ONOFF:
        {
#ifndef CPU_MKW41Z512VHT4
            APP_vSetLed(LED1, sBaseDevice.sOnOffServerCluster.bOnOff);
#else
            if(sBaseDevice.sOnOffServerCluster.bOnOff)
            {
                Led2On();
                Led3On();
                Led4On();
                
            }
            else
            {
                Led2Off();
                Led3Off();
                Led4Off();
            }
#endif
        }
        break;

        case GENERAL_CLUSTER_ID_IDENTIFY:
        {
            tsCLD_IdentifyCallBackMessage *psCallBackMessage = (tsCLD_IdentifyCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
            if (psCallBackMessage->u8CommandId == E_CLD_IDENTIFY_CMD_IDENTIFY)
            {
                APP_vHandleIdentify(sBaseDevice.sIdentifyServerCluster.u16IdentifyTime);
#if (defined BDB_SUPPORT_FIND_AND_BIND_TARGET)
                if(sBaseDevice.sIdentifyServerCluster.u16IdentifyTime == 0)
                {
                    BDB_vFbHandleStopIdentification(psEvent);
                }
#endif
            }
        }
        break;

        case GENERAL_CLUSTER_ID_BASIC:
        {
            tsCLD_BasicCallBackMessage *psCallBackMessage = (tsCLD_BasicCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
            if (psCallBackMessage->u8CommandId == E_CLD_BASIC_CMD_RESET_TO_FACTORY_DEFAULTS )
            {
                DBG_vPrintf(TRACE_ZCL, "Basic Factory Reset Received\r\n");
                FLib_MemSet(&sBaseDevice,0,sizeof(tsZHA_BaseDevice));
                APP_vZCL_DeviceSpecific_Init();
                eZHA_RegisterBaseDeviceEndPoint(ROUTER_APPLICATION_ENDPOINT,
                                                &APP_ZCL_cbEndpointCallback,
                                                &sBaseDevice);
            }
            #ifdef CLD_OTA
                vAppInitOTA();
            #endif
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
    }
}

/****************************************************************************
 *
 * NAME: APP_vHandleClusterUpdate
 *
 * DESCRIPTION:
 * callback for ZCL cluster update events
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_vHandleClusterUpdate(tsZCL_CallBackEvent *psEvent)
{
    if (psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum == GENERAL_CLUSTER_ID_IDENTIFY)
    {
        APP_vHandleIdentify(sBaseDevice.sIdentifyServerCluster.u16IdentifyTime);
        if(sBaseDevice.sIdentifyServerCluster.u16IdentifyTime == 0)
        {
            tsBDB_ZCLEvent  sBDBZCLEvent;
            /* provide callback to BDB handler for identify on Target */
            sBDBZCLEvent.eType = BDB_E_ZCL_EVENT_IDENTIFY;
            sBDBZCLEvent.psCallBackEvent = psEvent;
            BDB_vZclEventHandler(&sBDBZCLEvent);
        }
    }
}

/****************************************************************************
 *
 * NAME: APP_vZCL_DeviceSpecific_Init
 *
 * DESCRIPTION:
 * ZCL specific initialization
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_vZCL_DeviceSpecific_Init(void)
{
    sBaseDevice.sOnOffServerCluster.bOnOff = FALSE;
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8ManufacturerName, "NXP", CLD_BAS_MANUF_NAME_SIZE);
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8ModelIdentifier, "BDB-Router", CLD_BAS_MODEL_ID_SIZE);
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8DateCode, "20150212", CLD_BAS_DATE_SIZE);
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8SWBuildID, "1000-0001", CLD_BAS_SW_BUILD_SIZE);
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
