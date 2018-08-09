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

/*!=============================================================================
\file       app_zcl_light_task.c
\brief      Light Bulb application light task
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "NVM_Interface.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "pwrm.h"
#include "app_zcl_cfg.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_apl_aib.h"
#include "zps_apl_zdp.h"

#include "PDM_IDs.h"
#include "app_reporting.h"

#include "ZTimer.h"

#include "zcl.h"
#include "zcl_options.h"
#include "zll_commission.h"

#include "app_zcl_light_task.h"
#include "app_manage_temperature.h"
#include "app_zlo_light_node.h"
#include "app_common.h"
#include "app_main.h"
#include "Basic.h"
#include "Identify.h"
#include "Groups.h"
#include "Groups_internal.h"

#include "app_events.h"
#include "app_light_interpolation.h"
#include "DriverBulb.h"
#include "bdb_api.h"
#include "bdb_fb_api.h"

#include <string.h>

#ifdef CLD_OTA
    #include "app_ota_client.h"
#endif

#ifdef CLD_GREENPOWER
#include "GreenPower.h"
#include "app_green_power.h"
#endif

#ifdef DEBUG_ZCL
#define TRACE_ZCL   TRUE
#else
#define TRACE_ZCL   FALSE
#endif

#ifdef DEBUG_LIGHT_TASK
#define TRACE_LIGHT_TASK  TRUE
#else
#define TRACE_LIGHT_TASK FALSE
#endif

#ifdef DEBUG_BULB_START
#define TRACE_BULB_START TRUE
#else
#define TRACE_BULB_START FALSE
#endif

#define TRACE_RESET                FALSE

#define TRACE_PATH  FALSE

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

#define ZCL_TICK_TIME           ZTIMER_TIME_MSEC(100)

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void APP_ZCL_cbGeneralCallback(tsZCL_CallBackEvent *psEvent);
PRIVATE void APP_ZCL_cbEndpointCallback(tsZCL_CallBackEvent *psEvent);
PRIVATE void vLoadZCLRecord( void);
PRIVATE void vSaveZCLRecord( void);
PRIVATE void vDefaultZCLRecords(void);
PRIVATE void vCopyRecordsToZCL( void);
PRIVATE void vHandleWriteAttibute(tsZCL_CallBackEvent *psEvent);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
tsStartUpAttrib sZCLAttribs;

NVM_RegisterDataSet(&sZCLAttribs, 1, sizeof(sZCLAttribs), PDM_ID_ZCL_ATTRIB, gNVM_MirroredInRam_c);

uint8  u8StateChangeTick = 0;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void* psGetDeviceTable(void) {
    return &sDeviceTable;
}
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


    vLoadZCLRecord();

    /* Initialise ZLL */
    eZCL_Status = eZCL_Initialise(&APP_ZCL_cbGeneralCallback, apduZCL);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_ZCL, "\nErr: eZLO_Initialise:%d", eZCL_Status);
    }

    /* Start the tick timer */
    ZTIMER_eStart(u8TimerTick, ZCL_TICK_TIME);

    int i;
    for (i=0; i<ZLO_NUMBER_DEVICES; i++)
    {
        sDeviceTable.asDeviceRecords[i].u64IEEEAddr = ZPS_u64NwkNibGetExtAddr(ZPS_pvAplZdoGetNwkHandle() );
    }

    /* Register Light EndPoint */
    eZCL_Status = eApp_ZLO_RegisterEndpoint(&APP_ZCL_cbEndpointCallback);
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
            DBG_vPrintf(TRACE_ZCL, "Error: eZLL_RegisterCommissionEndPoint:%d\r\n", eZCL_Status);
    }

#ifdef CLD_COLOUR_CONTROL
    DBG_vPrintf(TRACE_LIGHT_TASK, "Capabilities %04x\n", sLight.sColourControlServerCluster.u16ColourCapabilities);

#ifdef CLD_COLOURCONTROL_ATTR_STARTUP_COLOUR_TEMPERATURE_MIRED
    /* set up a transotion to restore CCT */
    eCLD_ColourControlHandleColourModeChange( app_u8GetDeviceEndpoint(), E_CLD_COLOURCONTROL_COLOURMODE_COLOUR_TEMPERATURE);
    sLight.sColourControlServerCustomDataStructure.sTransition.u16Time = 0;
    sLight.sColourControlServerCustomDataStructure.sTransition.eCommand = E_CLD_COLOURCONTROL_CMD_MOVE_TO_COLOUR_TEMPERATURE;
    sLight.sColourControlServerCustomDataStructure.sTransition.sStart.iCCT = (int)(sZCLAttribs.u16Mired * 100);
    sLight.sColourControlServerCustomDataStructure.sTransition.sCurrent.iCCT = (int)(sZCLAttribs.u16Mired * 100);
    sLight.sColourControlServerCustomDataStructure.sTransition.sStep.iCCT = 0;
    sLight.sColourControlServerCustomDataStructure.sTransition.sTarget.iCCT = (int)(sZCLAttribs.u16Mired * 100);
#endif

#endif

    vAPP_ZCL_DeviceSpecific_Init();

#ifdef CLD_OTA
    vAppInitOTA();
#endif

#ifdef CLD_GREENPOWER
    vApp_GP_RegisterDevice(&APP_ZCL_cbEndpointCallback);
    ZTIMER_eStart(u8GPTimerTick, GP_ZCL_TICK_TIME);

#endif

    vUpdateBulbFromZCL(TRUE);

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
    sLight.sIdentifyServerCluster.u16IdentifyTime = u16Time;
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

    ZTIMER_eStart(u8TimerTick, ZTIMER_TIME_MSEC(10));

    u32Tick10ms++;
    u32Tick1Sec++;

    /* Provide processor cycles to any drivers that need time behaviour */
    DriverBulb_vTick();

    /* Wrap the Tick10ms counter and provide 100ms ticks to cluster */
    if (u32Tick10ms > 9)
    {
        eZCL_Update100mS();
        u32Tick10ms = 0;
    }
#if ( defined CLD_LEVEL_CONTROL) && !(defined MONO_ON_OFF)  /* add in nine 10ms interpolation points */
    else
    {
        vLI_CreatePoints();
    }
#endif

#ifdef CLD_OTA
    if (u32Tick1Sec == 82)   /* offset this from the 1 second roll over */
    {
        vRunAppOTAStateMachine();
    }
#endif

    if (u32Tick1Sec == 55)   /* offset this from the 1 second roll over */
    {
        if (u8StateChangeTick)
        {
            u8StateChangeTick--;
            if (u8StateChangeTick == 0)
            {
                vSaveZCLRecord();
            }
        }
    }

    /* Wrap the 1 second  counter and provide 1Hz ticks to cluster */
    if(u32Tick1Sec > 99)
    {
        u32Tick1Sec = 0;
        sCallBackEvent.pZPSevent = NULL;
        sCallBackEvent.eEventType = E_ZCL_CBET_TIMER;
        vZCL_EventHandler(&sCallBackEvent);
    }


    /* Pass the tick count into the temperature module for scheduling */
    APP_vManageTemperatureTick(u32Tick1Sec);


}

/****************************************************************************
 *
 * NAME: APP_cbTimerZclMsTick
 *
 * DESCRIPTION:
 * Task kicked by the millisecond tick timer
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerZclMsTick(void *pvParam)
{
    tsZCL_CallBackEvent sZCL_CallBackEvent;
    DBG_vPrintf(TRACE_ZCL, "APP_cbTimerZclMsTick \n");
    sZCL_CallBackEvent.eEventType = E_ZCL_CBET_TIMER_MS ;
    vZCL_EventHandler(&sZCL_CallBackEvent);
}


/****************************************************************************
 *
 * NAME: APP_cbTimerGPZclTick
 *
 * DESCRIPTION:
 * Task kicked by the tick timer
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
#ifdef CLD_GREENPOWER

PUBLIC void APP_cbTimerGPZclTick(void *pvParam)
{
    ZTIMER_eStart(u8GPTimerTick, GP_ZCL_TICK_TIME);
    u8GPZCLTimerEvent = E_ZCL_TIMER_CLICK_MS;
    ZQ_bQueueSend(&APP_msgGPZCLTimerEvents, &u8GPZCLTimerEvent);
}
#endif
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

    DBG_vPrintf(TRACE_ZCL, "\nZCL_Task endpoint event:%d", psStackEvent->eType);
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
//        DBG_vPrintf(TRACE_ZCL, "\nEVT: Lock Mutex\r\n");
        break;

    case E_ZCL_CBET_UNLOCK_MUTEX:
//        DBG_vPrintf(TRACE_ZCL, "\nEVT: Unlock Mutex\r\n");
        break;

    case E_ZCL_CBET_UNHANDLED_EVENT:
        DBG_vPrintf(TRACE_ZCL, "\nEVT: Unhandled Event\r\n");
        break;

    case E_ZCL_CBET_READ_ATTRIBUTES_RESPONSE:
        DBG_vPrintf(TRACE_ZCL, "\nEVT: Read attributes response");
        break;

    case E_ZCL_CBET_READ_REQUEST:
        DBG_vPrintf(TRACE_ZCL, "\nEVT: Read request");
        break;

    case E_ZCL_CBET_DEFAULT_RESPONSE:
        DBG_vPrintf(TRACE_ZCL, "\nEVT: Default response");
        break;

    case E_ZCL_CBET_ERROR:
        DBG_vPrintf(TRACE_ZCL, "\nEVT: Error");
        break;

    case E_ZCL_CBET_TIMER:
        DBG_vPrintf(TRACE_ZCL, "\nEVT: Timer");
        break;

    case E_ZCL_CBET_ZIGBEE_EVENT:
        DBG_vPrintf(TRACE_ZCL, "\nEVT: ZigBee");
        break;

    case E_ZCL_CBET_CLUSTER_CUSTOM:
        DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Custom");
        break;

    default:
        DBG_vPrintf(TRACE_ZCL, "\nInvalid event type");
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

    bool_t bUpdateBulb = FALSE;

    switch (psEvent->eEventType)
    {

    case E_ZCL_CBET_LOCK_MUTEX:
        break;

    case E_ZCL_CBET_UNLOCK_MUTEX:
        break;

    case E_ZCL_CBET_UNHANDLED_EVENT:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Unhandled event");
        break;

    case E_ZCL_CBET_READ_INDIVIDUAL_ATTRIBUTE_RESPONSE:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Rd Attr %04x RS %d AS %d", psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum, psEvent->uMessage.sIndividualAttributeResponse.psAttributeStatus->eRequestStatus, psEvent->uMessage.sIndividualAttributeResponse.psAttributeStatus->eAttributeStatus);
        break;

    case E_ZCL_CBET_READ_ATTRIBUTES_RESPONSE:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Read attributes response");
        break;

    case E_ZCL_CBET_READ_REQUEST:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Read request");
        break;

    case E_ZCL_CBET_DEFAULT_RESPONSE:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Default response");
        break;

    case E_ZCL_CBET_ERROR:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Error");
        break;

    case E_ZCL_CBET_TIMER:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Timer");
        break;

    case E_ZCL_CBET_ENABLE_MS_TIMER:
        #ifdef OTA_CLD_ATTR_REQUEST_DELAY
            ZTIMER_eStart(u8TimerZclMsTick, ZTIMER_TIME_MSEC(psEvent->uMessage.u32TimerPeriodMs));
        #endif
        break;

    case E_ZCL_CBET_DISABLE_MS_TIMER:
        #ifdef OTA_CLD_ATTR_REQUEST_DELAY
            ZTIMER_eStop(u8TimerZclMsTick);
        #endif
        break;

    case E_ZCL_CBET_ZIGBEE_EVENT:
        //DBG_vPrintf(TRACE_ZCL, "\nEP EVT: ZigBee");
        break;

    case E_ZCL_CBET_CLUSTER_CUSTOM:
        DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Custom Cl %04x\n", psEvent->uMessage.sClusterCustomMessage.u16ClusterId);

        switch(psEvent->uMessage.sClusterCustomMessage.u16ClusterId)
        {
            case GENERAL_CLUSTER_ID_ONOFF:
            {

                tsCLD_OnOffCallBackMessage *psCallBackMessage = (tsCLD_OnOffCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;

                DBG_vPrintf(TRACE_ZCL, " CmdId=%d", psCallBackMessage->u8CommandId);

                switch(psCallBackMessage->u8CommandId)
                {

                    case E_CLD_ONOFF_CMD_OFF_EFFECT:
                        DBG_vPrintf(TRACE_ZCL, "\nOff with effect %d:%d", psCallBackMessage->uMessage.psOffWithEffectRequestPayload->u8EffectId,
                                                                      psCallBackMessage->uMessage.psOffWithEffectRequestPayload->u8EffectVariant);
                        break;

                }
                if (sLight.sIdentifyServerCluster.u16IdentifyTime == 0)
                {
                    /* only update bulb if not identifying */
                    bUpdateBulb = TRUE;
                }


            }
            break;
            case GENERAL_CLUSTER_ID_IDENTIFY:
            {
                tsCLD_IdentifyCallBackMessage *psCallBackMessage = (tsCLD_IdentifyCallBackMessage*)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
                if (psCallBackMessage->u8CommandId == E_CLD_IDENTIFY_CMD_TRIGGER_EFFECT)
                {
                    DBG_vPrintf(TRACE_LIGHT_TASK, "Identify Cust CB %d\n", psCallBackMessage->uMessage.psTriggerEffectRequestPayload->eEffectId);
                    vStartEffect(psCallBackMessage->uMessage.psTriggerEffectRequestPayload->eEffectId);
                }
                else if (psCallBackMessage->u8CommandId == E_CLD_IDENTIFY_CMD_IDENTIFY)
                {
                    DBG_vPrintf(TRACE_PATH, "\nJP E_CLD_IDENTIFY_CMD_IDENTIFY");
                    APP_vHandleIdentify(sLight.sIdentifyServerCluster.u16IdentifyTime);
#if (defined BDB_SUPPORT_FIND_AND_BIND_TARGET)
                    if(sLight.sIdentifyServerCluster.u16IdentifyTime == 0)
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
                    DBG_vPrintf(TRACE_RESET, "Basic Factory Reset Received\n");
                    vApp_ZCL_ResetDeviceStructure();
                    vResetZCLRecords();
                    /* Register Light EndPoint */
                    eApp_ZLO_RegisterEndpoint(&APP_ZCL_cbEndpointCallback);
                    vAPP_ZCL_DeviceSpecific_Init();
                    #ifdef CLD_OTA
                        vAppInitOTA();
                    #endif
                    #ifdef CLD_GREENPOWER
                        vApp_GP_RegisterDevice(&APP_ZCL_cbEndpointCallback);
                    #endif
                    bUpdateBulb = TRUE;
                }
            }
            break;

            case GENERAL_CLUSTER_ID_SCENES:
            {
                if (sLight.sIdentifyServerCluster.u16IdentifyTime == 0)
                {
                    /* only update bulb if not identifying */
                    bUpdateBulb = TRUE;
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
            case COMMISSIONING_CLUSTER_ID_TL_COMMISSIONING:
            {
                tsBDB_ZCLEvent sEvent;
                sEvent.eType = BDB_E_ZCL_EVENT_TL_IPAN_MSG;
                sEvent.psCallBackEvent = psEvent;
                BDB_vZclEventHandler(&sEvent);
            }
            break;
#ifdef CLD_GREENPOWER
            case GREENPOWER_CLUSTER_ID:
            {
                tsGP_GreenPowerCallBackMessage *psCallBackMessage = (tsGP_GreenPowerCallBackMessage *)psEvent->uMessage.sClusterCustomMessage.pvCustomData;
                vHandleGreenPowerEvent(psCallBackMessage);
            }
            break;
#endif


        }
        break;

    case E_ZCL_CBET_WRITE_INDIVIDUAL_ATTRIBUTE:
        DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Write Individual Attribute Status %02x\n", psEvent->eZCL_Status);
        if (psEvent->eZCL_Status == E_ZCL_SUCCESS)
        {
            vHandleWriteAttibute(psEvent);
        }
        break;

    case E_ZCL_CBET_REPORT_INDIVIDUAL_ATTRIBUTE:
        {
            tsZCL_IndividualAttributesResponse    *psIndividualAttributeResponse = &psEvent->uMessage.sIndividualAttributeResponse;
            DBG_vPrintf(TRACE_ZCL,"Individual Report attribute for Cluster = %d\n", psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum);
            DBG_vPrintf(TRACE_ZCL,"eAttributeDataType = %d\n", psIndividualAttributeResponse->eAttributeDataType);
            DBG_vPrintf(TRACE_ZCL,"u16AttributeEnum = %d\n", psIndividualAttributeResponse->u16AttributeEnum );
            DBG_vPrintf(TRACE_ZCL,"eAttributeStatus = %d\n", psIndividualAttributeResponse->eAttributeStatus );
        }
        break;

    case E_ZCL_CBET_REPORT_INDIVIDUAL_ATTRIBUTES_CONFIGURE:
        {
            tsZCL_AttributeReportingConfigurationRecord    *psAttributeReportingRecord = &psEvent->uMessage.sAttributeReportingConfigurationRecord;
            DBG_vPrintf(TRACE_ZCL | 1,"Individual Configure Report Cluster %d Attrib %d Type %d Min %d Max %d IntV %d Direcct %d Change %d\n",
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

        if (psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum == GENERAL_CLUSTER_ID_SCENES)
        {

        }
        else if (psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum == GENERAL_CLUSTER_ID_IDENTIFY)
        {
            APP_vHandleIdentify(sLight.sIdentifyServerCluster.u16IdentifyTime);
            if(sLight.sIdentifyServerCluster.u16IdentifyTime == 0)
            {
                tsBDB_ZCLEvent     sBDBZCLEvent;
                /* provide callback to BDB handler for; identify on Target */
                sBDBZCLEvent.eType = BDB_E_ZCL_EVENT_IDENTIFY;
                sBDBZCLEvent.psCallBackEvent = psEvent;
                BDB_vZclEventHandler(&sBDBZCLEvent);
            }
        }
        else
        {
            if (sLight.sIdentifyServerCluster.u16IdentifyTime == 0)
            {
              bUpdateBulb = TRUE;
            }
        }
        break;

    default:
        DBG_vPrintf(TRACE_ZCL, "\nEP EVT: Invalid evt type 0x%x", (uint8)psEvent->eEventType);
        break;

    }

    if (bUpdateBulb)
    {
        vUpdateBulbFromZCL(FALSE);
    }
}

/****************************************************************************
 *
 * NAME: vLoadZCLRecord
 *
 * DESCRIPTION:
 * Recover pwersited  attributes and copy to ZCL
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vLoadZCLRecord( void)
{
    uint16 u16ByteRead;

    /* default the records */
    vDefaultZCLRecords();

    /* load saved attribute data from NVM */
    /* over writes the defaults with the latest values */
    PDM_eReadDataFromRecord(PDM_ID_ZCL_ATTRIB,
                &sZCLAttribs,
                sizeof(tsStartUpAttrib), &u16ByteRead);

    /* copy recovered records to the real ZCL attributes  */
    vCopyRecordsToZCL();

}

/****************************************************************************
 *
 * NAME: vSaveZCLRecord
 *
 * DESCRIPTION:
 * Check if any of the persisted attributes have changed
 * and save if they have
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vSaveZCLRecord( void)
{

    bool_t bSave =FALSE;
    if (sLight.sOnOffServerCluster.eStartUpOnOff != sZCLAttribs.u8OnOffStartAttrib)
    {
        bSave = TRUE;
        /* start up attrib has changed */
        sZCLAttribs.u8OnOffStartAttrib = sLight.sOnOffServerCluster.eStartUpOnOff;
        DBG_vPrintf(TRACE_BULB_START, "Save OnOffStartUp %02x\n", sZCLAttribs.u8OnOffStartAttrib );
    }
    if ( (sLight.sOnOffServerCluster.bOnOff != (bool_t)sZCLAttribs.u8OnOff) &&
            ( (sLight.sOnOffServerCluster.eStartUpOnOff == E_CLD_ONOFF_STARTUPONOFF_TOGGLE) ||
              (sLight.sOnOffServerCluster.eStartUpOnOff == E_CLD_ONOFF_STARTUPONOFF_PREVIOUS_VALUE))
       )
    {
        /* On state has changed
         * and restore state is dependent on previous state, so save it
         */
        bSave = TRUE;

        sZCLAttribs.u8OnOff = (uint8)sLight.sOnOffServerCluster.bOnOff;
        DBG_vPrintf(TRACE_BULB_START, "Save On State %d\n", sZCLAttribs.u8OnOff);
    }

#ifdef CLD_LEVEL_CONTROL
    if ( (sZCLAttribs.u8LevelOptionsAttrib != sLight.sLevelControlServerCluster.u8Options) ||
         (sZCLAttribs.u8LevelStartattrib != sLight.sLevelControlServerCluster.u8StartUpCurrentLevel ))
    {
        bSave = TRUE;
        sZCLAttribs.u8LevelOptionsAttrib = sLight.sLevelControlServerCluster.u8Options;
        sZCLAttribs.u8LevelStartattrib = sLight.sLevelControlServerCluster.u8StartUpCurrentLevel;
        DBG_vPrintf(TRACE_BULB_START, "Save LevelStartUp %02x Options %02x\n", sZCLAttribs.u8LevelStartattrib,
                sZCLAttribs.u8LevelOptionsAttrib);

    }
     if ( (sZCLAttribs.u8Level != sLight.sLevelControlServerCluster.u8CurrentLevel) &&
          (sLight.sLevelControlServerCluster.u8StartUpCurrentLevel == 0xff))
    {
        /* Current Level has changed
         * and restore value depends on previous vsaalue, so save it
         */
         bSave = TRUE;
        sZCLAttribs.u8Level = sLight.sLevelControlServerCluster.u8CurrentLevel;
    }
#endif

#ifdef CLD_COLOUR_CONTROL
     if (sLight.sColourControlServerCluster.u8Options != sZCLAttribs.u8ColourOptionsAttrib)
     {

         bSave = TRUE;
         sZCLAttribs.u8ColourOptionsAttrib = sLight.sColourControlServerCluster.u8Options;
         DBG_vPrintf(TRACE_BULB_START, "Save Colour Options %02x\n", sZCLAttribs.u8ColourOptionsAttrib );
     }
#ifdef CLD_COLOURCONTROL_ATTR_STARTUP_COLOUR_TEMPERATURE_MIRED
     if (sZCLAttribs.u16ColourStartAttrib != sLight.sColourControlServerCluster.u16StartupColourTemperatureMired )
     {
         bSave = TRUE;
         sZCLAttribs.u16ColourStartAttrib = sLight.sColourControlServerCluster.u16StartupColourTemperatureMired;
         DBG_vPrintf(TRACE_BULB_START, "Save ColourStartUp %04x\n", sZCLAttribs.u16ColourStartAttrib );
     }
    if ( ( sZCLAttribs.u16Mired != sLight.sColourControlServerCluster.u16ColourTemperatureMired ) &&
            (sLight.sColourControlServerCluster.u16StartupColourTemperatureMired == 0xffff))
    {
        /* CT has changed and
         * restore value does depend onp revious value, so save it
         */
        bSave = TRUE;
        sZCLAttribs.u16Mired = sLight.sColourControlServerCluster.u16ColourTemperatureMired;
    }
#endif
#endif

    if (bSave)
    {
        PDM_eSaveRecordData(PDM_ID_ZCL_ATTRIB,&sZCLAttribs,sizeof(sZCLAttribs));
    }


}

/****************************************************************************
 *
 * NAME: vDefaultZCLRecords
 *
 * DESCRIPTION:
 * Copy the default values to the ZCL record structute
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vDefaultZCLRecords(void)
{
/* Default, the start up attributes */

    sZCLAttribs.u8OnOffStartAttrib = CLD_ONOFF_START_MODE;
    sZCLAttribs.u8OnOff = CLD_ONOFF_START_STATE;

#ifdef CLD_LEVEL_CONTROL
    sZCLAttribs.u8LevelStartattrib = CLD_LEVEL_CONTROL_START_MODE;
    sZCLAttribs.u8LevelOptionsAttrib = CLD_LEVELCONTROL_OPTIONS;
    sZCLAttribs.u8Level = CLD_LEVEL_CONTROL_STARTUP_LEVEL;
#endif

#ifdef CLD_COLOUR_CONTROL
    sZCLAttribs.u8ColourOptionsAttrib = CLD_COLOURCONTROL_OPTIONS;
#ifdef CLD_COLOURCONTROL_ATTR_STARTUP_COLOUR_TEMPERATURE_MIRED
    sZCLAttribs.u16ColourStartAttrib = CLD_COLOURCONTROL_START_MODE;
    sZCLAttribs.u16Mired = CLD_COLOURCONTROL_START_CT;
#endif
#endif
}

/****************************************************************************
 *
 * NAME: vCopyRecordsToZCL
 *
 * DESCRIPTION:
 * Copy the data from the ZCL record structure to the real ZCL attributes
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vCopyRecordsToZCL( void)
{
/* copy the recovered on off state to the attribute */

    sLight.sOnOffServerCluster.bOnOff = (bool_t)sZCLAttribs.u8OnOff;
    sLight.sOnOffServerCluster.eStartUpOnOff = sZCLAttribs.u8OnOffStartAttrib;

#ifdef CLD_LEVEL_CONTROL
    /* copy the saved level options and start up to the attribute*/
    sLight.sLevelControlServerCluster.u8Options = sZCLAttribs.u8LevelOptionsAttrib;
    sLight.sLevelControlServerCluster.u8StartUpCurrentLevel = sZCLAttribs.u8LevelStartattrib;
    /* copy the saved level to the attribute*/
    sLight.sLevelControlServerCluster.u8CurrentLevel = sZCLAttribs.u8Level;
#endif

#ifdef CLD_COLOUR_CONTROL
    sLight.sColourControlServerCluster.u8Options = sZCLAttribs.u8ColourOptionsAttrib;
#ifdef CLD_COLOURCONTROL_ATTR_STARTUP_COLOUR_TEMPERATURE_MIRED
    /* save the saved mired to the attribute */
    sLight.sColourControlServerCluster.u16StartupColourTemperatureMired = sZCLAttribs.u16ColourStartAttrib;
    sLight.sColourControlServerCluster.u16ColourTemperatureMired = sZCLAttribs.u16Mired;
#endif
#endif

}

/****************************************************************************
 *
 * NAME: vResetZCLRecords
 *
 * DESCRIPTION:
 * Reset the ZCL records stucture
 * then copy tro the ZCL
 * and save to nvm
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vResetZCLRecords( void)
{
    vDefaultZCLRecords();
    vCopyRecordsToZCL();
    PDM_eSaveRecordData(PDM_ID_ZCL_ATTRIB,&sZCLAttribs,sizeof(sZCLAttribs));
}

/****************************************************************************
 *
 * NAME: vHandleWriteAttibute
 *
 * DESCRIPTION:
 * Check if any of the persisted attributes have changed
 * and save if they have
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vHandleWriteAttibute(tsZCL_CallBackEvent *psEvent)
{

    if ( ((psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum == GENERAL_CLUSTER_ID_ONOFF) &&
            (psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum == E_CLD_ONOFF_ATTR_ID_STARTUP_ONOFF))
#ifdef CLD_LEVEL_CONTROL
          ||
          ((psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum == GENERAL_CLUSTER_ID_LEVEL_CONTROL) &&
            ((psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum == E_CLD_LEVELCONTROL_ATTR_ID_STARTUP_CURRENT_LEVEL) ||
             (psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum == E_CLD_LEVELCONTROL_ATTR_ID_OPTIONS)))
#endif
#ifdef CLD_COLOUR_CONTROL
         ||
          ((psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum == LIGHTING_CLUSTER_ID_COLOUR_CONTROL) &&
           ((psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum == E_CLD_COLOURCONTROL_ATTR_OPTIONS) ||
           (psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum == E_CLD_COLOURCONTROL_ATTR_STARTUP_COLOUR_TEMPERATURE_MIRED)))
#endif
    )
    {
        DBG_vPrintf(TRACE_BULB_START, "Check Persistance Clustter %04x  Attrib %04x\n",
                psEvent->psClusterInstance->psClusterDefinition->u16ClusterEnum,
                psEvent->uMessage.sIndividualAttributeResponse.u16AttributeEnum);
        vSaveZCLRecord();
    }

}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
