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
\file       app_zlo_sensor_node.c
\brief      ZLO Demo : Stack <-> Light Sensor App Interaction(Implementation)
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>


#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "pwrm.h"
#include "app_zcl_cfg.h"
#include "RNG_Interface.h"
#include "NVM_Interface.h"
#include "app_common.h"
#include "Groups.h"
#include "PDM_IDs.h"
#include "ZTimer.h"
#include "app_zlo_sensor_node.h"
#include "app_zcl_sensor_task.h"
//#include "app_zbp_utilities.h"
#include "app_events.h"
#include "app_buttons.h"
#include "zcl_customcommand.h"
#include "app_main.h"
#include "app_sensor_state_machine.h"
#include "zcl_common.h"
#include "app_reporting.h"
#include "app_sensor_buttons.h"
#ifdef CLD_OTA
    #include "OTA.h"
    #include "app_ota_client.h"
    #include "app_common.h"
#endif
#include "app_power_on_counter.h"
#include "app_sleep_handler.h"
#include "app_event_handler.h"
#include "app_nwk_event_handler.h"
#include "app_blink_led.h"
#include "bdb_api.h"
#include "bdb_fb_api.h"
#include "App_LightTemperatureOccupancySensor.h"
#include "app_zps_cfg.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_SENSOR_NODE
    #define TRACE_SENSOR_NODE   TRUE
#else
    #define TRACE_SENSOR_NODE   FALSE
#endif

#define WAKE_FROM_DEEP_SLEEP    (1<<11)


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void app_vStartNodeFactoryNew(void);
PRIVATE void app_vRestartNode (void);
PRIVATE uint8 app_u8GetSensorEndpoint( void);
PRIVATE void vAPP_InitialiseTask(void);
#ifndef CPU_MKW41Z512VHT4
PRIVATE void vHandleZdoLeaveRequest(uint8 u8Action, uint64 u64TargetAddr, uint8 u8Flags);
#endif
PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent);
PRIVATE void vAppHandleZdoEvents( BDB_tsZpsAfEvent *psZpsAfEvent);

#ifndef CPU_MKW41Z512VHT4
PRIVATE void vDeletePDMOnButtonPress(uint8 u8ButtonDIO);
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

//PUBLIC PDM_tsRecordDescriptor sDevicePDDesc;
PUBLIC tsDeviceDesc sDeviceDesc = {E_STARTUP,E_STARTUP};
PUBLIC bool_t bBDBJoinFailed = TRUE;
NVM_RegisterDataSet(&sDeviceDesc, 1, sizeof(tsDeviceDesc), PDM_ID_APP_SENSOR, gNVM_MirroredInRam_c);
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
uint8 u8NoQueryCount = 0;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#ifdef CLD_OTA
PUBLIC teNODE_STATES eGetNodeState(void)
{
    return sDeviceDesc.eNodeState;
}
#endif
/****************************************************************************
 *
 * NAME: APP_vInitialiseNode
 *
 * DESCRIPTION:
 * Initialises the application related functions
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitialiseNode(void)
{
    PDM_teStatus eStatusReportReload;
    DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP Sensor Node: APP_vInitialiseNode*");

    /*Initialise the application buttons*/
#ifndef CPU_MKW41Z512VHT4
    APP_bButtonInitialise();
#endif

    vManagePowerOnCountLoadRecord();
	
#ifdef CLD_OTA
    vLoadOTAPersistedData();
#endif

    /*In case of a deep sleep device any button wake up would cause a PDM delete , only check for DIO8
     * pressed for deleting the context */
#ifndef CPU_MKW41Z512VHT4
    DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP Sensor Node: u16AHI_PowerStatus() = %d", u16AHI_PowerStatus());
#endif

    /* Restore any report data that is previously saved to flash */
    eStatusReportReload = eRestoreReports();
    uint16 u16ByteRead;
    PDM_eReadDataFromRecord(PDM_ID_APP_SENSOR,
                                               &sDeviceDesc,
                                               sizeof(tsDeviceDesc),
                                               &u16ByteRead);

#ifndef CPU_MKW41Z512VHT4
    /* Set security state */
    ZPS_vDefaultKeyInit();
#endif

    /* Initialize ZBPro stack */
    ZPS_psAplAibGetAib()->bUseInstallCode = BDB_JOIN_USES_INSTALL_CODE_KEY;
#ifdef CPU_MKW41Z512VHT4
    APP_vSetMacAddr();
#endif
    ZPS_eAplAfInit();

    DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP Sensor Node: ZPS_eAplAfInit");

    APP_ZCL_vInitialise();

    /* Set end device age out time to 11 days 9 hours & 4 mins */
    ZPS_bAplAfSetEndDeviceTimeout(ZED_TIMEOUT_16384_MIN);

    /*Load the reports from the PDM or the default ones depending on the PDM load record status*/
    if(eStatusReportReload !=PDM_E_STATUS_OK )
    {
        /*Load Defaults if the data was not correct*/
        vLoadDefaultConfigForReportable();
    }
    /*Make the reportable attributes */
    vMakeSupportedAttributesReportable();

    /* If the device state has been restored from flash, re-start the stack
     * and set the application running again.
     */
    sBDB.sAttrib.u32bdbPrimaryChannelSet = BDB_PRIMARY_CHANNEL_SET;
    sBDB.sAttrib.u32bdbSecondaryChannelSet = BDB_SECONDARY_CHANNEL_SET;
    BDB_tsInitArgs sInitArgs;
    sInitArgs.hBdbEventsMsgQ = &APP_msgBdbEvents;

    BDB_vInit(&sInitArgs);

    if (APP_bNodeIsInRunningState())
    {
        app_vRestartNode();
        sBDB.sAttrib.bbdbNodeIsOnANetwork = TRUE;
    }
    else
    {
        DBG_vPrintf(TRACE_SENSOR_NODE, "\nFactory New Start");
        app_vStartNodeFactoryNew();
        sBDB.sAttrib.bbdbNodeIsOnANetwork = FALSE;
    }

#ifndef CPU_MKW41Z512VHT4
    if (FALSE == (u16AHI_PowerStatus() & WAKE_FROM_DEEP_SLEEP))
    {
        vDeletePDMOnButtonPress(APP_BUTTONS_BUTTON_1);
    }
#endif

#ifndef CPU_MKW41Z512VHT4
    /* Register callback that will handle ZDP (mgmt) leave requests */
    ZPS_vAplZdoRegisterZdoLeaveActionCallback(vHandleZdoLeaveRequest);
#endif

    #ifdef PDM_EEPROM
        vDisplayPDMUsage();
    #endif

#ifndef CPU_MKW41Z512VHT4
    bRGB_LED_Enable();
    bRGB_LED_SetLevel(LED_MAX_LEVEL,LED_MIN_LEVEL,LED_MIN_LEVEL);
    bRGB_LED_Off();
    bRGB_LED_SetGroupLevel(LED_LEVEL);

    vHTSreset();
    DBG_vPrintf(TRACE_SENSOR_NODE,"Reset Temp sensor\n");

    /* Always initialise the light sensor and the RGB Led
     * the iic interface runs slower (100kHz) than that used
     * by the RGB driver (400KHz)
     */
    bool_t bStatus= bTSL2550_Init();
    DBG_vPrintf(TRACE_SENSOR_NODE,"bTSL2550_Init = %d\n",bStatus);
#endif

    vAPP_LightSensorSample();
    vAPP_InitialiseTask();
}

/****************************************************************************
 *
 * NAME: vAPP_InitialiseTask
 *
 * DESCRIPTION:
 * This is the main App Initialise.
 * Task that checks the power status and starts tasks based on those results
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAPP_InitialiseTask(void)
{
#ifndef CPU_MKW41Z512VHT4
    DBG_vPrintf(TRACE_SENSOR_NODE, "APP Initialise Tasks: Power Status = %d", u16AHI_PowerStatus());
#endif

    vManagePowerOnCountInit();

    if (APP_bNodeIsInRunningState())
    {
#ifndef CPU_MKW41Z512VHT4
        vActionOnButtonActivationAfterDeepSleep();
#endif
    }
    else
    {
        /* We are factory new so start the blink timers*/
        vStartBlinkTimer(APP_JOINING_BLINK_TIME);
    }
}

/****************************************************************************
 *
 * NAME: APP_vBdbCallback
 *
 * DESCRIPTION:
 * Callback from the BDB
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vBdbCallback(BDB_tsBdbEvent *psBdbEvent)
{
#ifndef CPU_MKW41Z512VHT4
    vAHI_WatchdogRestart(); // JV - Assuming bdb_taskBDB() context might take longer; especially during initializaiton. ToDo - discuss with Richard towards removal from here
#endif

    switch(psBdbEvent->eEventType)
    {

    case BDB_EVENT_NONE:
        break;

    case BDB_EVENT_ZPSAF:                // Use with BDB_tsZpsAfEvent
        vAppHandleAfEvent(&psBdbEvent->uEventData.sZpsAfEvent);
        break;

    case BDB_EVENT_INIT_SUCCESS:
        DBG_vPrintf(TRACE_SENSOR_NODE,"APP: BdbInitSuccessful\n");
        break;

    case BDB_EVENT_FAILURE_RECOVERY_FOR_REJOIN: //Recovery on rejoin failure
        DBG_vPrintf(TRACE_SENSOR_NODE,"BDB EVT Recovery On Rejoin Failure\n");
        vHandleFailedRejoin();
        break;

    case BDB_EVENT_REJOIN_FAILURE: // only for ZED
        DBG_vPrintf(TRACE_SENSOR_NODE, "BDB EVT failed to rejoin\n");
        vHandleFailedToJoin();
        break;

    case BDB_EVENT_REJOIN_SUCCESS: // only for ZED
        DBG_vPrintf(TRACE_SENSOR_NODE, "BDB EVT Rejoin success\n");
        bBDBJoinFailed = FALSE;
        vHandleNetworkJoinEndDevice();
        break;

    case BDB_EVENT_NWK_STEERING_SUCCESS:
        // go to running state
        DBG_vPrintf(TRACE_SENSOR_NODE,"GoRunningState!");
        bBDBJoinFailed = FALSE;
        vHandleNetworkJoinAndRejoin();
        break;

    case BDB_EVENT_NO_NETWORK:
        DBG_vPrintf(TRACE_SENSOR_NODE,"\nNo network!");
        vHandleFailedToJoin();
        break;

    case BDB_EVENT_APP_START_POLLING:
        DBG_vPrintf(TRACE_SENSOR_NODE,"Start Polling!\n");
        /* Start fast polling */
        vStartPollTimer(POLL_TIME_FAST);
        break;

    case BDB_EVENT_FB_HANDLE_SIMPLE_DESC_RESP_OF_TARGET:
        DBG_vPrintf(TRACE_SENSOR_NODE,"Simple descriptor %d %d %04x %04x %d \n",psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp,
                psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress,
                psBdbEvent->uEventData.psFindAndBindEvent->u16ProfileId,
                psBdbEvent->uEventData.psFindAndBindEvent->u16DeviceId,
                psBdbEvent->uEventData.psFindAndBindEvent->u8DeviceVersion);
        break;

    case BDB_EVENT_FB_CHECK_BEFORE_BINDING_CLUSTER_FOR_TARGET:
        DBG_vPrintf(TRACE_SENSOR_NODE,"Check For Binding Cluster %d \n",psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId);
        break;

    case BDB_EVENT_FB_CLUSTER_BIND_CREATED_FOR_TARGET:
        DBG_vPrintf(TRACE_SENSOR_NODE,"Bind Created for cluster %d \n",psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId);
        break;

    case BDB_EVENT_FB_BIND_CREATED_FOR_TARGET:
        {
            DBG_vPrintf(TRACE_SENSOR_NODE,"Bind Created for target EndPt %d \n",psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp);
            u8NoQueryCount = 0;
            uint8 u8Seq;
            tsZCL_Address sAddress;
            tsCLD_Identify_IdentifyRequestPayload sPayload;

            sPayload.u16IdentifyTime = 0;
            sAddress.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
            sAddress.uAddress.u16DestinationAddress = psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress;

            eCLD_IdentifyCommandIdentifyRequestSend(psBdbEvent->uEventData.psFindAndBindEvent->u8InitiatorEp,
                                                    psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp,
                                                    &sAddress,
                                                    &u8Seq,
                                                    &sPayload);
        }
        break;

    case BDB_EVENT_FB_GROUP_ADDED_TO_TARGET:
        DBG_vPrintf(TRACE_SENSOR_NODE,"Group Bind Created\n");
        break;

    case BDB_EVENT_FB_ERR_BINDING_TABLE_FULL:
        DBG_vPrintf(TRACE_SENSOR_NODE,"ERR: Bind Table Full\n");
        break;

    case BDB_EVENT_FB_ERR_BINDING_FAILED:
        DBG_vPrintf(TRACE_SENSOR_NODE,"ERR: Bind\n");
        break;

    case BDB_EVENT_FB_ERR_GROUPING_FAILED:
        DBG_vPrintf(TRACE_SENSOR_NODE,"ERR: Group\n");
        break;

    case BDB_EVENT_FB_NO_QUERY_RESPONSE:
        DBG_vPrintf(TRACE_SENSOR_NODE,"ERR: No Query response\n");
        //Example to stop further query repeating
        if(u8NoQueryCount >= 2)
        {
            u8NoQueryCount = 0;
            BDB_vFbExitAsInitiator();
        }
        else
        {
            u8NoQueryCount++;
        }
        break;

    case BDB_EVENT_FB_TIMEOUT:
        DBG_vPrintf(TRACE_SENSOR_NODE,"ERR: TimeOut\n");
        break;

    default:
        DBG_vPrintf(TRACE_SENSOR_NODE, "BDB EVT default evt %d\n", psBdbEvent->eEventType);
        break;

    }
}

/****************************************************************************
 *
 * NAME: APP_taskSensor
 *
 * DESCRIPTION:
 * Checks to see what event has triggered the task to start and calls the
 * appropriate function. This handles App events, Stack events, timer activations
 * and manual activations.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_taskSensor(void)
{
    APP_tsEvent sAppEvent;
    sAppEvent.eType = APP_E_EVENT_NONE;

    /*Collect the application events*/
    if (ZQ_bQueueReceive(&APP_msgAppEvents, &sAppEvent) == TRUE)
    {
        DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP ZLO Sensor Task: App Event %d", sAppEvent.eType);
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
        /* Is this a button event on SW3 ? */
        if ( ( sAppEvent.eType == APP_E_EVENT_BUTTON_DOWN || sAppEvent.eType == APP_E_EVENT_BUTTON_UP )
                && (sAppEvent.uEvent.sButton.u8Button == APP_E_BUTTONS_NFC_FD) )
        {
            /* Always pass this on for processing */
            vAppHandleAppEvent(sAppEvent);
        }
        /* Other event (handle as normal) ? */
        else
#endif
        {
            // Reset if required
            if ((sAppEvent.eType == APP_E_EVENT_BUTTON_DOWN) && (sAppEvent.uEvent.sButton.u8Button == APP_E_BUTTONS_BUTTON_1))
            {
                if (ZPS_E_SUCCESS !=  ZPS_eAplZdoLeaveNetwork(0, FALSE,FALSE)) 
                {
                    /* Leave failed,so just reset everything */
                    DBG_vPrintf(TRUE,"Deleting the PDM\n");
                    APP_vFactoryResetRecords();
                    ResetMCU();
                }
            }

            if(bBDBJoinFailed)
            {
                vStartBlinkTimer(APP_JOINING_BLINK_TIME);
                if(APP_bNodeIsInRunningState())
                {
                    // TODO kick BDB for rejoin
                    DBG_vPrintf(TRACE_SENSOR_NODE, "Call BDB vStart\n");
                    sBDB.sAttrib.bbdbNodeIsOnANetwork = TRUE;
                    BDB_vStart();
                }
                else
                {
                    //Retrigger the network steering as sensor is not part of a network
                    vAppHandleStartup();
                }
            }
            else
            {
                vAppHandleAppEvent(sAppEvent);
            }
        }
    }
}

/****************************************************************************
 *
 * NAME: vAppHandleAfEvent
 *
 * DESCRIPTION:
 * Handles AF Events.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent)
{
    if (psZpsAfEvent->u8EndPoint == app_u8GetSensorEndpoint())
    {
        DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP ZLO Sensor Task: ZCL Event");
        APP_ZCL_vEventHandler( &psZpsAfEvent->sStackEvent);
    }
    else if (psZpsAfEvent->u8EndPoint == LIGHTTEMPERATUREOCCUPANCYSENSOR_ZDO_ENDPOINT)
    {
        DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP ZLO Sensor Task: Main APP Handle");
        vAppHandleZdoEvents( psZpsAfEvent);
    }

    /* Ensure Freeing of Apdus */
    if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_DATA_INDICATION)
    {
        PDUM_eAPduFreeAPduInstance(psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.hAPduInst);
    }
    else if ( psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_INTERPAN_DATA_INDICATION )
    {
        PDUM_eAPduFreeAPduInstance(psZpsAfEvent->sStackEvent.uEvent.sApsInterPanDataIndEvent.hAPduInst);
    }

}
#ifndef CPU_MKW41Z512VHT4
/****************************************************************************
 *
 * NAME: vHandleZdoLeaveRequest
 *
 * DESCRIPTION:
 * Callback that will handle ZDP (mgmt) leave requests
 *
 * RETURNS:
 * None
 *
 ****************************************************************************/
PRIVATE void vHandleZdoLeaveRequest(uint8 u8Action, uint64 u64TargetAddr, uint8 u8Flags)
{
    DBG_vPrintf(TRACE_SENSOR_NODE, "\n%s - Addr: 0x%016llx, Action: 0x%02x Flags: 0x%02x\n", __FUNCTION__, u64TargetAddr, u8Action, u8Flags);

    /* Check this request is for us */
    if ((u64TargetAddr == ZPS_u64AplZdoGetIeeeAddr()) || (u64TargetAddr == 0ULL))
    {
        /* We respond to NLME leave requests elsewhere.. */
        if (ZPS_LEAVE_ORIGIN_MGMT_LEAVE == u8Action)
        {
            /*what to do if rejoin is set to true i.e. u8Flags = 1....?*/
            if (0 == u8Flags)
            {
                /* No need to wait for ZPS_EVENT_NWK_LEAVE_CONFIRM event which will
                   The parent will do the needful hence just reset the PDM and go for
                   association again*/
                APP_vFactoryResetRecords();
                ResetMCU();
            }
            else
            {
                /*No need to do anything because the stack will generate one rejoin -
                 * if that is success full its fine
                 * else the rejoin statemachine will take care of rejoining. */
            }
        }
    }
}
#endif
/****************************************************************************
 *
 * NAME: vDeletePDMOnButtonPress
 *
 * DESCRIPTION:
 * PDM context clearing on button press
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
#ifndef CPU_MKW41Z512VHT4
PRIVATE void vDeletePDMOnButtonPress(uint8 u8ButtonDIO)
{
    bool_t bDeleteRecords = FALSE;
    uint32 u32Buttons = u32AHI_DioReadInput() & (1 << u8ButtonDIO);
    (void)u32AHI_DioInterruptStatus();
    if (u32Buttons == 0)
    {
        bDeleteRecords = TRUE;
    }
    else
    {
        bDeleteRecords = FALSE;
    }
    /* If required, at this point delete the network context from flash, perhaps upon some condition
     * For example, check if a button is being held down at reset, and if so request the Persistent
     * Data Manager to delete all its records:
     * e.g. bDeleteRecords = vCheckButtons();
     * Alternatively, always call PDM_vDelete() if context saving is not required.
     */
    if(bDeleteRecords)
    {
        sBDB.sAttrib.bbdbNodeIsOnANetwork = FALSE;
        DBG_vPrintf(TRACE_SENSOR_NODE,"\nAPP Sensor Node: Sending the leave");
        if (ZPS_E_SUCCESS !=  ZPS_eAplZdoLeaveNetwork(0, FALSE,FALSE))
        {
            /* Leave failed, probably lost parent, so just reset everything */
            DBG_vPrintf(TRACE_SENSOR_NODE,"\nAPP Sensor Node: Deleting the PDM");
            APP_vFactoryResetRecords();
            vAHI_SwReset();
        }
    }
}
#endif

/****************************************************************************
 *
 * NAME: vAppHandleZdoEvents
 *
 * DESCRIPTION:
 * This is the main state machine which decides whether to call up the startup
 * or running function. This depends on whether we are in the network on not.
 *
 * PARAMETERS:
 * ZPS_tsAfEvent sAppStackEvent Stack event information.
 *
 ****************************************************************************/
PRIVATE void vAppHandleZdoEvents( BDB_tsZpsAfEvent *psZpsAfEvent)
{
#ifdef DEBUG_SENSOR_NODE
    //vDisplayNWKTransmitTable();
#endif

    /* Handle events depending on node state */
#ifdef CLD_OTA
        if ((psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.eStatus == ZPS_E_SUCCESS) &&
                (psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.u8DstEndpoint == 0))
        {
            // Data Ind for ZDp Ep
            if (ZPS_ZDP_MATCH_DESC_RSP_CLUSTER_ID == psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.u16ClusterId)
            {
                vHandleMatchDescriptor(&psZpsAfEvent->sStackEvent);
            } else if (ZPS_ZDP_IEEE_ADDR_RSP_CLUSTER_ID == psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.u16ClusterId) {
                vHandleIeeeAddressRsp(&psZpsAfEvent->sStackEvent);
            }
        }
#endif
    switch (sDeviceDesc.eNodeState)
    {

    case E_STARTUP:
        vAppHandleStartup();
        break;

    case E_RUNNING:
        vAppHandleRunning( &(psZpsAfEvent->sStackEvent) );
        break;

    case E_JOINING_NETWORK:
        break;

    default:
        break;

    }

}

/****************************************************************************
 *
 * NAME: app_vRestartNode
 *
 * DESCRIPTION:
 * Start the Restart the ZigBee Stack after a context restore from
 * the EEPROM/Flash
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void app_vRestartNode (void)
{

    /* The node is in running state indicates that
     * the EZ Mode state is as E_EZ_DEVICE_IN_NETWORK*/
    DBG_vPrintf(TRACE_SENSOR_NODE, "\nNon Factory New Start");

    ZPS_vSaveAllZpsRecords();
}


/****************************************************************************
 *
 * NAME: app_vStartNodeFactoryNew
 *
 * DESCRIPTION:
 * Start the ZigBee Stack for the first ever Time.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void app_vStartNodeFactoryNew(void)
{
    sBDB.sAttrib.u32bdbPrimaryChannelSet = BDB_PRIMARY_CHANNEL_SET;
    sBDB.sAttrib.u32bdbSecondaryChannelSet = BDB_SECONDARY_CHANNEL_SET;
    // BDB_eNsStartNwkSteering(); done later on  only if no OOB commissionning ????

    sDeviceDesc.eNodeState = E_JOINING_NETWORK;
    /* Stay awake for joining */
    DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP Sensor Node: Factory New Start");
}

/****************************************************************************
 *
 * NAME: app_u8GetSensorEndpoint
 *
 * DESCRIPTION:
 * Return the application endpoint
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PRIVATE uint8 app_u8GetSensorEndpoint( void)
{
 return LIGHTTEMPERATUREOCCUPANCYSENSOR_SENSOR_ENDPOINT;
}

/****************************************************************************
 *
 * NAME: APP_vFactoryResetRecords
 *
 * DESCRIPTION: reset application and stack to factory new state
 *              preserving the outgoing nwk frame counter
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vFactoryResetRecords(void)
{
    sDeviceDesc.eNodeState = E_STARTUP;

#ifdef CLD_OTA
    sDeviceDesc.bValid = FALSE;
    sDeviceDesc.u64IeeeAddrOfServer = 0;
    sDeviceDesc.u16NwkAddrOfServer = 0xffff;
    sDeviceDesc.u8OTAserverEP = 0xff;
    vOTAResetPersist();
#endif
    /* clear out the stack */
    ZPS_vDefaultStack();
    (void)ZPS_eAplAibSetApsUseExtendedPanId(ZPS_APS_AIB_INIT_USE_EXTENDED_PANID);
    ZPS_vSetKeys();
    /* save everything */
    PDM_eSaveRecordData(PDM_ID_APP_SENSOR,
                            &sDeviceDesc,
                            sizeof(tsDeviceDesc));

    ZPS_vSaveAllZpsRecords();

#ifdef CPU_MKW41Z512VHT4
    NvCompletePendingOperations();
#endif  
}

PUBLIC bool_t APP_bNodeIsInRunningState(void)
{
    DBG_vPrintf(TRACE_SENSOR_NODE, "\nAPP Sensor Node: NodeState=%d", sDeviceDesc.eNodeState);
    return (sDeviceDesc.eNodeState == E_RUNNING) ? TRUE:FALSE;
}
/****************************************************************************
 *
 * NAME: sGetOTACallBackPersistdata
 *
 * DESCRIPTION:
 * returns persisted data
 *
 * RETURNS:
 * tsOTA_PersistedData
 *
 ****************************************************************************/
#ifdef CLD_OTA
PUBLIC tsOTA_PersistedData sGetOTACallBackPersistdata(void)
{
    return sSensor.sCLD_OTA_CustomDataStruct.sOTACallBackMessage.sPersistedData;
}
#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

