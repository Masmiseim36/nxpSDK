/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_zcl_task.c
\brief      Base Device Application Template:
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#ifndef CPU_MKW41Z512VHT4
#include <AppApi.h>
#else
#include "LED.h"
#endif
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "app_zcl_cfg.h"
#include "PDM_IDs.h"
#include "ZTimer.h"
#include "zcl.h"
#include "zcl_options.h"
#include "app_zcl_task.h"
#include "app_cfg.h"
#include "app_leds.h"


#include "base_device.h"
#include "app_common.h"
#include "app_main.h"
#include "app_events.h"
#include "bdb_api.h"
#ifdef DR1199
#include "GenericBoard.h"
#endif

#include <string.h>
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef DEBUG_ZCL
#define TRACE_ZCL   TRUE
#else
#define TRACE_ZCL   FALSE
#endif

#define TRACE_RESET FALSE


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

#define ZCL_TICK_TIME           ZTIMER_TIME_MSEC(100)


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void APP_ZCL_cbGeneralCallback(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_ZCL_cbEndpointCallback(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_vHandleClusterCustomCommands(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_vHandleClusterUpdate(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_vZCL_DeviceSpecific_Init(void);
PRIVATE void vStartEffect(uint8 u8Effect);
PRIVATE void vHandleIdentifyRequest(uint16 u16Duration);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

tsZHA_BaseDevice sBaseDevice;

/****************************************************************************/
/***        Local Variables                                               ***/
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

    /* Initialise ZLL */
    eZCL_Status = eZCL_Initialise(&APP_ZCL_cbGeneralCallback, apduZCL);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_ZCL, "\r\nErr: eZLO_Initialise:%d", eZCL_Status);
    }

#ifndef APP_ALLOW_ZPS_SUSPEND
    /* Start the tick timer */
    if(ZTIMER_eStart(u8TimerZCL, ZCL_TICK_TIME) != E_ZTIMER_OK)
    {
        DBG_vPrintf(TRACE_ZCL, "APP: Failed to Start Tick Timer\r\n");
    }
#endif
    /* Register Base Device EndPoint */
    eZCL_Status =  eZHA_RegisterBaseDeviceEndPoint(COORDINATOR_APPLICATION_ENDPOINT,
                                                   &APP_ZCL_cbEndpointCallback,
                                                   &sBaseDevice);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
            DBG_vPrintf(TRACE_ZCL, "Error: eZHA_RegisterBaseDeviceEndPoint:%02x\r\r\n", eZCL_Status);
    }

    APP_vZCL_DeviceSpecific_Init();
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
#if TRUE == TRACE_ZCL
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
#endif

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
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Write Individual Attribute");
            break;

        case E_ZCL_CBET_CLUSTER_UPDATE:
            DBG_vPrintf(TRACE_ZCL, "Update Id %04x\r\n", psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum);
            APP_vHandleClusterUpdate(psEvent);
            break;

        case E_ZCL_CBET_REPORT_INDIVIDUAL_ATTRIBUTE:
            DBG_vPrintf(TRACE_ZCL, "ZCL Attribute Report: Cluster %04x Attribute %04x Value %d\r\n",
                            psEvent->pZPSevent->uEvent.sApsDataIndEvent.u16ClusterId,
                            psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum,
                            *((uint8*)(psEvent->uMessage.sIndividualAttributeResponse.pvAttributeData)) );
        break;

        case E_ZCL_CBET_REPORT_ATTRIBUTES:
            break;

        default:
            DBG_vPrintf(TRACE_ZCL, "\r\nEP EVT: Invalid evt type 0x%x", (uint8)psEvent->eEventType);
            break;
    }
}

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
        case GENERAL_CLUSTER_ID_BASIC:
        {
            tsCLD_BasicCallBackMessage *psCallBackMessage = (tsCLD_BasicCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
            if (psCallBackMessage->u8CommandId == E_CLD_BASIC_CMD_RESET_TO_FACTORY_DEFAULTS )
            {
                DBG_vPrintf(TRACE_RESET, "Basic Factory Reset Received\r\n");
                FLib_MemSet(&sBaseDevice,0,sizeof(tsZHA_BaseDevice));
                APP_vZCL_DeviceSpecific_Init();
                /* Register Base Device EndPoint */
                eZHA_RegisterBaseDeviceEndPoint(COORDINATOR_APPLICATION_ENDPOINT,
                                                               &APP_ZCL_cbEndpointCallback,
                                                               &sBaseDevice);

            }
        }
        break;

        case GENERAL_CLUSTER_ID_IDENTIFY:
        {
            tsCLD_IdentifyCallBackMessage *psCallBackMessage = (tsCLD_IdentifyCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
            DBG_vPrintf(TRACE_ZCL, "- for identify cluster\r\r\n");
            /* provide callback to BDB handler for identify query response on initiator*/
            if(psEvent->psClusterInstance->bIsServer == FALSE)
            {
                tsBDB_ZCLEvent  sBDBZCLEvent;
                DBG_vPrintf(TRACE_ZCL, "\r\nCallBackBDB");
                sBDBZCLEvent.eType = BDB_E_ZCL_EVENT_IDENTIFY_QUERY;
                sBDBZCLEvent.psCallBackEvent = psEvent;
                BDB_vZclEventHandler(&sBDBZCLEvent);
            }
            else
            {
                // Server Side
                if (psCallBackMessage->u8CommandId == E_CLD_IDENTIFY_CMD_IDENTIFY)
                {
                    vHandleIdentifyRequest(sBaseDevice.sIdentifyServerCluster.u16IdentifyTime);
                }
                else if (psCallBackMessage->u8CommandId == E_CLD_IDENTIFY_CMD_TRIGGER_EFFECT)
                {
                    vStartEffect( psCallBackMessage->uMessage.psTriggerEffectRequestPayload->eEffectId);
                }
            }
        }
        break;

        case GENERAL_CLUSTER_ID_GROUPS:
            break;
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
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8ManufacturerName, "NXP", CLD_BAS_MANUF_NAME_SIZE);
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8ModelIdentifier, "BDB-Coordinator", CLD_BAS_MODEL_ID_SIZE);
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8DateCode, "20150212", CLD_BAS_DATE_SIZE);
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8SWBuildID, "1000-0001", CLD_BAS_SW_BUILD_SIZE);
    #ifdef CLD_BAS_ATTR_MANUFACTURER_VERSION_DETAILS
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8ManufacturerVersionDetails, "Zigbee_Version_3.0", CLD_BAS_MANUFACTURER_VERSION_SIZE);
    #endif
    #ifdef CLD_BAS_ATTR_SERIAL_NUMBER
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8SerialNumber, "1234", CLD_BAS_SERIAL_NUMBER_SIZE);
    #endif
    #ifdef CLD_BAS_ATTR_PRODUCT_LABEL
    FLib_MemCpy(sBaseDevice.sBasicServerCluster.au8ProductLabel, "Kinetis_KW41Z", CLD_BAS_PRODUCT_LABEL_SIZE);
    #endif
    
}

/****************************************************************************
 *
 * NAME: vStartEffect
 *
 * DESCRIPTION:
 * ZLO Device Specific identify effect set up
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PRIVATE void vStartEffect(uint8 u8Effect)
{
    switch (u8Effect) {
        case E_CLD_IDENTIFY_EFFECT_BLINK:
            sBaseDevice.sIdentifyServerCluster.u16IdentifyTime = 2;
            break;

        case E_CLD_IDENTIFY_EFFECT_BREATHE:
            sBaseDevice.sIdentifyServerCluster.u16IdentifyTime = 17;
            break;

        case E_CLD_IDENTIFY_EFFECT_OKAY:
            sBaseDevice.sIdentifyServerCluster.u16IdentifyTime = 3;
            break;

        case E_CLD_IDENTIFY_EFFECT_CHANNEL_CHANGE:
            sBaseDevice.sIdentifyServerCluster.u16IdentifyTime = 9;
            break;

        case E_CLD_IDENTIFY_EFFECT_FINISH_EFFECT:
        case E_CLD_IDENTIFY_EFFECT_STOP_EFFECT:
            sBaseDevice.sIdentifyServerCluster.u16IdentifyTime = 1;
            break;
    }
    vHandleIdentifyRequest( sBaseDevice.sIdentifyServerCluster.u16IdentifyTime);
}

/****************************************************************************
 *
 * NAME: vHandleIdentifyRequest
 *
 * DESCRIPTION: handle identify request command received by the remote
 * causes the identify blink for the required time
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vHandleIdentifyRequest(uint16 u16Duration)
{
    ZTIMER_eStop(u8TimerId);
    if (u16Duration == 0)
    {
#ifndef CPU_MKW41Z512VHT4
        APP_vSetLed(LED1, OFF);
#else
        Led2Off();
        Led3Off();
        Led4Off();
#endif
    }
    else
    {
        ZTIMER_eStart(u8TimerId, ZTIMER_TIME_MSEC(500));
#ifndef CPU_MKW41Z512VHT4
        APP_vSetLed(LED1, ON);
#else
        Led1On();
#endif
    }
}

/****************************************************************************
 *
 * NAME: APP_cbTimerId
 *
 * DESCRIPTION: Tasks that handles the flashing leds during identfy operation
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerId(void *pvParam)
{
    if (sBaseDevice.sIdentifyServerCluster.u16IdentifyTime == 0)
    {
        ZTIMER_eStop(u8TimerId);
#ifndef CPU_MKW41Z512VHT4
        APP_vSetLed(LED1, OFF);
#else
        Led2Off();
        Led3Off();
        Led4Off();
#endif
    }
    else
    {
#ifndef CPU_MKW41Z512VHT4
    	APP_vSetLed(LED1, (sBaseDevice.sIdentifyServerCluster.u16IdentifyTime%2));
#else
        if(sBaseDevice.sIdentifyServerCluster.u16IdentifyTime%2)
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
        ZTIMER_eStop(u8TimerId);
        ZTIMER_eStart(u8TimerId, ZTIMER_TIME_MSEC(500));
    }
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
