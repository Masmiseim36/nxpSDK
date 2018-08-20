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
\file       app_zlo_switch_node.c
\brief      ZLO Controller & Switch Demo : Stack <-> Remote Control App Interaction
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
//#include <AppApi.h>
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
//#include "dbg_uart.h"
#include "pwrm.h"
#include "app_zcl_cfg.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_apl_aib.h"
#include "zps_apl_zdp.h"
#include "RNG_Interface.h"
#include "NVM_Interface.h"
#include "LED.h"
#include "Identify.h"
#include "app_common.h"
#include "app_main.h"
#include "Groups.h"

#include "PDM_IDs.h"
#include "app_events.h"

#include "ZTimer.h"
#include "app_zlo_switch_node.h"

#include "app_zcl_switch_task.h"
//#include "app_zbp_utilities.h"

#include "app_events.h"
#include "zcl_customcommand.h"
#include "app_buttons.h"
//#include "GenericBoard.h"
#include "app_switch_state_machine.h"
#include "zcl_common.h"
#ifdef CLD_OTA
    #include "OTA.h"
    #include "app_ota_client.h"
#endif

#ifdef APP_NTAG_ICODE
#include "app_ntag_icode.h"
#include "nfc_nwk.h"
#endif
#ifdef APP_NTAG_AES
#include "app_ntag_aes.h"
#include "nfc_nwk.h"
#endif

/* 802.15.4 */
#include "PhyInterface.h"
#include "MacInterface.h"

#include "appZdpExtraction.h"
#include "app_main.h"
#include "SerialManager.h"
#include "app_zps_cfg.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef gPHY_802_15_4g_d
#define mDefaultValueOfChannel_c (0x0001FFFF)
#else
#define mDefaultValueOfChannel_c (0x07FFF800)
#endif



#ifdef DEBUG_SWITCH_NODE
    #define TRACE_SWITCH_NODE   TRUE
#else
    #define TRACE_SWITCH_NODE   FALSE
#endif

#define bWakeUpFromSleep() bWaitingToSleep()  /* For readability purpose */

#define APP_LONG_SLEEP_DURATION_IN_SEC 6
#define WAKE_FROM_DEEP_SLEEP    (1<<11)

/* ZDO endpoint for all devices is always 0 */
#define SWITCH_ZDO_ENDPOINT    (0)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PUBLIC void vStopTimer(uint8 u8Timer);
#ifdef SLEEP_ENABLE
    PRIVATE void vStopAllTimers(void);
    PRIVATE void vLoadKeepAliveTime(uint8 u8TimeInSec);
    #ifdef DEEP_SLEEP_ENABLE
        PRIVATE void vActionOnButtonActivationAfterDeepSleep(void);
    #endif
#endif

PRIVATE void vSetAddress(tsZCL_Address * psAddress, bool_t bBroadcast, uint16 u16ClusterId);
PRIVATE void vHandleJoinAndRejoin(void);
PRIVATE void app_vRestartNode (void);
PRIVATE void app_vStartNodeFactoryNew(void);
PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent);
PRIVATE void vHandleRunningStackEvent(ZPS_tsAfEvent* psStackEvent);
PRIVATE bool bAddressInTable( uint16 u16AddressToCheck );
PRIVATE void vAppHandleZdoEvents( BDB_tsZpsAfEvent *psZpsAfEvent);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

//PUBLIC PDM_tsRecordDescriptor   sDevicePDDesc;
PUBLIC tsDeviceDesc             sDeviceDesc;
PUBLIC uint16                   u16GroupId;

NVM_RegisterDataSet(&sDeviceDesc, 1, sizeof(tsDeviceDesc), PDM_ID_APP_ZLO_SWITCH, gNVM_MirroredInRam_c);

extern const uint8 u8MyEndpoint;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE tsDeviceInfo sDeviceInfo;

PRIVATE uint16 u16FastPoll;

#ifdef SLEEP_ENABLE
    #ifdef DEEP_SLEEP_ENABLE
        PRIVATE uint8 u8DeepSleepTime= DEEP_SLEEP_TIME;
    #endif
    PRIVATE uint8 u8KeepAliveTime = KEEP_ALIVETIME;
    PRIVATE pwrm_tsWakeTimerEvent    sWake;
    PRIVATE bool bWaitingForLeave = FALSE;
#else
    PRIVATE uint8 u8KeepAliveTime = 1;    
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern instanceId_t mMacInstance;

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
    DBG_vPrintf(TRACE_SWITCH_NODE, "%s - Addr: 0x%016llx, Action: 0x%02x Flags: 0x%02x\r\n", __FUNCTION__, u64TargetAddr, u8Action, u8Flags);

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
    DBG_vPrintf( TRACE_SWITCH_NODE,"Dimmer-Switch APP_vInitialiseNode\r\n");
    //APP_vInitLeds();

#ifdef DEEP_SLEEP_ENABLE
    vReloadSleepTimers();
#endif

#ifdef CLD_OTA
    vLoadOTAPersistedData();
#endif

    /* Restore any application data previously saved to flash */
    uint16 u16ByteRead;
    PDM_eReadDataFromRecord(PDM_ID_APP_ZLO_SWITCH,
                            &sDeviceDesc,
                            sizeof(tsDeviceDesc),
                            &u16ByteRead);

    /* Initialise ZBPro stack */
    ZPS_psAplAibGetAib()->bUseInstallCode = BDB_JOIN_USES_INSTALL_CODE_KEY;
#ifdef CPU_MKW41Z512VHT4
    APP_vSetMacAddr();
#endif
    ZPS_eAplAfInit();

    APP_ZCL_vInitialise();

    /* Set End Device TimeOut */
    ZPS_bAplAfSetEndDeviceTimeout( ZED_TIMEOUT_16384_MIN);

    /* If the device state has been restored from flash, re-start the stack
     * and set the application running again.
     */
    sBDB.sAttrib.u32bdbPrimaryChannelSet   = BDB_PRIMARY_CHANNEL_SET;
    sBDB.sAttrib.u32bdbSecondaryChannelSet = BDB_SECONDARY_CHANNEL_SET;
    BDB_tsInitArgs sInitArgs;
    sInitArgs.hBdbEventsMsgQ = &APP_msgBdbEvents;
    
    BDB_vInit(&sInitArgs);
    
    if (sDeviceDesc.eNodeState == E_RUNNING)
    {
        app_vRestartNode();
        sBDB.sAttrib.bbdbNodeIsOnANetwork = TRUE;
    }
    else
    {
        app_vStartNodeFactoryNew();
        DBG_vPrintf(TRACE_SWITCH_NODE, "Factory New Start\r\n");
        
        sBDB.sAttrib.bbdbNodeIsOnANetwork = FALSE;
    }
#ifndef CPU_MKW41Z512VHT4
    /* Register callback that will handle ZDP (mgmt) leave requests */
    ZPS_vAplZdoRegisterZdoLeaveActionCallback(vHandleZdoLeaveRequest);
#endif
    #ifdef PDM_EEPROM
        vDisplayPDMUsage();
    #endif
}


/****************************************************************************
 *
 * NAME: bLightsDiscovered
 *
 * DESCRIPTION:
 * Initializes LED's
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC bool bLightsDiscovered(void)
{
    if(sDeviceInfo.u8Discovered > 0)
    {
        return TRUE;
    }
    return FALSE;
}


/****************************************************************************
 *
 * NAME: vStartFastPolling
 *
 * DESCRIPTION:
 * Set fast poll time
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStartFastPolling(uint8 u8Seconds)
{
    /* Fast poll is every 100ms, so times by 10 */
    u16FastPoll = 10*u8Seconds;
}

/****************************************************************************
 *
 * NAME: APP_vBdbCallback
 *
 * DESCRIPTION:
 * Callbak from the BDB
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
 PUBLIC void APP_vBdbCallback(BDB_tsBdbEvent *psBdbEvent)
{
    static uint8 u8NoQueryCount;
   
    //DBG_vPrintf(TRACE_SWITCH_NODE,"APP_vBdbCallback BdbCb eEventType=%s(%d)\r\n",BDB_teBdbEventType_char[psBdbEvent->eEventType],psBdbEvent->eEventType);
    switch(psBdbEvent->eEventType)
    {
        case BDB_EVENT_NONE:
            break;
        case BDB_EVENT_ZPSAF:                // Use with BDB_tsZpsAfEvent
            if(psBdbEvent->uEventData.sZpsAfEvent.sStackEvent.eType != 15)
                 DBG_vPrintf(TRACE_SWITCH_NODE,"\r\nBdbCb event [%d %d]\r\n",psBdbEvent->uEventData.sZpsAfEvent.u8EndPoint,
                                psBdbEvent->uEventData.sZpsAfEvent.sStackEvent.eType);      

            vAppHandleAfEvent(&psBdbEvent->uEventData.sZpsAfEvent);
            break;

        case BDB_EVENT_INIT_SUCCESS:
            DBG_vPrintf(TRACE_SWITCH_NODE,"APP: BdbInitSuccessful\r\n");
            break;
        
        case BDB_EVENT_FAILURE_RECOVERY_FOR_REJOIN: //Recovery on rejoin failure
            DBG_vPrintf(TRACE_SWITCH_NODE,"BDB EVT Recovery On Rejoin Failure\r\n");
            break;
        case BDB_EVENT_REJOIN_FAILURE: // only for ZED
            DBG_vPrintf(TRACE_SWITCH_NODE, "BDB EVT failed to rejoin\r\n");         
            #if SLEEP_ENABLE
        /* If waiting for leave do not try to go to sleep*/
            if(bWaitingForLeave == FALSE)
            {
                //DBG_vPrintf(TRACE_SWITCH_NODE,"Init Rejoin Failure!\n");
                // Go to deep sleep
                vLoadKeepAliveTime(0);
                #ifdef DEEP_SLEEP_ENABLE
                vLoadDeepSleepTimer(0);
                #endif
                ZTIMER_eStart(u8TimerTick, ZTIMER_TIME_MSEC(10));
            }
            #endif
            break;

        case BDB_EVENT_REJOIN_SUCCESS: // only for ZED
             DBG_vPrintf(TRACE_SWITCH_NODE, "BDB EVT Rejoin success\r\n");            
             vHandleJoinAndRejoin();
            #ifdef DEEP_SLEEP_ENABLE
                /*If it is coming out of deep sleep take action on button press */
                vActionOnButtonActivationAfterDeepSleep();
            #endif
            break;


        case BDB_EVENT_NWK_STEERING_SUCCESS:
            // go to running state
            DBG_vPrintf(TRACE_SWITCH_NODE,"GoRunningState!\r\n");
            //sDeviceDesc.eNodeState = E_RUNNING;
            vHandleJoinAndRejoin();
            break;

        case BDB_EVENT_NO_NETWORK:
            DBG_vPrintf(TRACE_SWITCH_NODE,"No network!\r\n");            
            // Go to deep sleep
            #ifdef SLEEP_ENABLE
                vLoadKeepAliveTime(0);
                #ifdef DEEP_SLEEP_ENABLE
                    vLoadDeepSleepTimer(0);
                    vUpdateKeepAliveTimer();
                #endif
            #endif
            break;

        case BDB_EVENT_APP_START_POLLING:
            DBG_vPrintf(TRACE_SWITCH_NODE,"Start Polling!\r\n");
            /* Start 1 seconds polling */
            DBG_vPrintf(TRACE_SWITCH_NODE, "APP_vBdbCallback: eNodeState = E_WAIT_STARTUP\r\n");
            sDeviceDesc.eNodeState = E_WAIT_STARTUP;
            ZTIMER_eStart(u8TimerPoll, POLL_TIME_FAST);
            vStartFastPolling(2);
            break;

        case BDB_EVENT_FB_HANDLE_SIMPLE_DESC_RESP_OF_TARGET:
            DBG_vPrintf(TRACE_SWITCH_NODE,"Simple descriptor %d %d %04x %04x %d \r\n",psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp,
                    psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress,
                    psBdbEvent->uEventData.psFindAndBindEvent->u16ProfileId,
                    psBdbEvent->uEventData.psFindAndBindEvent->u16DeviceId,
                    psBdbEvent->uEventData.psFindAndBindEvent->u8DeviceVersion);
         
            break;

        case BDB_EVENT_FB_CHECK_BEFORE_BINDING_CLUSTER_FOR_TARGET:
            DBG_vPrintf(TRACE_SWITCH_NODE,"Check For Binding Cluster %d \r\n",psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId);
            
            break;

        case BDB_EVENT_FB_CLUSTER_BIND_CREATED_FOR_TARGET:
            DBG_vPrintf(TRACE_SWITCH_NODE,"Bind Created for cluster %d \r\n",psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId);
            vSetIndividualLightInformation();
            break;

        case BDB_EVENT_FB_BIND_CREATED_FOR_TARGET:
        {
            DBG_vPrintf(TRACE_SWITCH_NODE,"Bind Created for target EndPt %d \r\n",psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp);
            
  
            u8NoQueryCount = 0;
            // Example to ask to stop identification to an end point on completing discovery

                uint8 u8Seq;
                tsZCL_Address sAddress;
                tsCLD_Identify_IdentifyRequestPayload sPayload;

                sPayload.u16IdentifyTime = 0;
                sAddress.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
                sAddress.uAddress.u16DestinationAddress = psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress;

                eCLD_IdentifyCommandIdentifyRequestSend(
                                        psBdbEvent->uEventData.psFindAndBindEvent->u8InitiatorEp,
                                        psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp,
                                        &sAddress,
                                        &u8Seq,
                                        &sPayload);

                break;
        }

        case BDB_EVENT_FB_GROUP_ADDED_TO_TARGET:
        {
            DBG_vPrintf(TRACE_SWITCH_NODE,"Group Added with ID %d \r\n",psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16GroupId);
            u8NoQueryCount = 0;
            //Example to ask to Stop identification to that group
            uint8 u8Seq;
            tsZCL_Address sAddress;
            tsCLD_Identify_IdentifyRequestPayload sPayload;

            sPayload.u16IdentifyTime = 0;
            sAddress.eAddressMode = E_ZCL_AM_GROUP;
            sAddress.uAddress.u16GroupAddress = psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16GroupId;

            eCLD_IdentifyCommandIdentifyRequestSend(
                                    psBdbEvent->uEventData.psFindAndBindEvent->u8InitiatorEp,
                                    psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp,
                                    &sAddress,
                                    &u8Seq,
                                    &sPayload);
            break;
        }

        case BDB_EVENT_FB_ERR_BINDING_TABLE_FULL:
            DBG_vPrintf(TRACE_SWITCH_NODE,"ERR: Bind Table Full\r\n");
            break;

        case BDB_EVENT_FB_ERR_BINDING_FAILED:
            DBG_vPrintf(TRACE_SWITCH_NODE,"ERR: Bind\r\n");
            break;

        case BDB_EVENT_FB_ERR_GROUPING_FAILED:
            DBG_vPrintf(TRACE_SWITCH_NODE,"ERR: Group\r\n");
            break;

        case BDB_EVENT_FB_NO_QUERY_RESPONSE:
            DBG_vPrintf(TRACE_SWITCH_NODE,"ERR: No Query response\r\n");
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
            DBG_vPrintf(TRACE_SWITCH_NODE,"ERR: TimeOut\r\n");
            break;

        default:
            DBG_vPrintf(1, "BDB EVT default evt %d\r\n", psBdbEvent->eEventType);
            break;
    }
}

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
PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent)
{
    if (psZpsAfEvent->u8EndPoint == app_u8GetDeviceEndpoint() )
    {
        if ((psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_DATA_INDICATION) ||
            (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_INTERPAN_DATA_INDICATION))
        {
            DBG_vPrintf(TRACE_SWITCH_NODE, "Pass to ZCL\r\n");
            APP_ZCL_vEventHandler( &psZpsAfEvent->sStackEvent);
         }
    } else if (psZpsAfEvent->u8EndPoint == SWITCH_ZDO_ENDPOINT)
    {
        // events for ep 0
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

/****************************************************************************
 *
 * NAME: vAppHandleZdoEvents
 *
 * DESCRIPTION:
 * Application handler for stack events for end point 0 (ZDO)
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vAppHandleZdoEvents( BDB_tsZpsAfEvent *psZpsAfEvent)
{
   // DBG_vPrintf(TRACE_SWITCH_NODE, "For ep 0, State %s(%d)\r\n", eNodeState_char[sDeviceDesc.eNodeState] ,sDeviceDesc.eNodeState);
    
    /* Handle events depending on node state */
    switch (sDeviceDesc.eNodeState)
    {
    case E_STARTUP:
        break;

    case E_RUNNING:
        //DBG_vPrintf(TRACE_SWITCH_NODE, "E_RUNNING\r\n");
        vHandleRunningStackEvent(&psZpsAfEvent->sStackEvent);
        break;
    default:
        break;
    }
}

/****************************************************************************
 *
 * NAME: vHandleRunningStackEvent
 *
 * DESCRIPTION:
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vHandleRunningStackEvent(ZPS_tsAfEvent* psStackEvent)
{
    
    switch (psStackEvent->eType)
    {
    case ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE:
        DBG_vPrintf(TRACE_SWITCH_NODE, "Joined as EndPoint\r\n");
        vHandleJoinAndRejoin();
        #ifdef APP_NTAG_ICODE
        {
            /* Not a rejoin ? */
            if (FALSE == psStackEvent->uEvent.sNwkJoinedEvent.bRejoin)
            {
                /* Write network data to tag */
                APP_vNtagStart(DIMMERSWITCH_SWITCH_ENDPOINT);
            }
        }
        #endif
        break;

    case ZPS_EVENT_NWK_FAILED_TO_JOIN:
        DBG_vPrintf(TRACE_SWITCH_NODE, "Running Failed to join\r\n");
        if (ZPS_psAplAibGetAib()->u64ApsUseExtendedPanid != 0)
        {
            DBG_vPrintf(TRACE_SWITCH_NODE, "Restore epid %x\r\n", ZPS_psAplAibGetAib()->u64ApsUseExtendedPanid);
            ZPS_vNwkNibSetExtPanId(ZPS_pvAplZdoGetNwkHandle(), ZPS_psAplAibGetAib()->u64ApsUseExtendedPanid);
        }
        break;

    case ZPS_EVENT_NWK_LEAVE_INDICATION:

       DBG_vPrintf(TRACE_SWITCH_NODE, "LEAVE IND Addr %016llx Rejoin %02x\r\n",
               psStackEvent->uEvent.sNwkLeaveIndicationEvent.u64ExtAddr,
               psStackEvent->uEvent.sNwkLeaveIndicationEvent.u8Rejoin);

       if ( (psStackEvent->uEvent.sNwkLeaveIndicationEvent.u64ExtAddr == 0UL) &&
            (psStackEvent->uEvent.sNwkLeaveIndicationEvent.u8Rejoin == 0) )
       {
           /* We sare asked to Leave without rejoin */
           DBG_vPrintf(TRACE_SWITCH_NODE, "LEAVE IND -> For Us No Rejoin\r\n");
           DBG_vPrintf(TRACE_SWITCH_NODE, "Leave -> Reset Data Structures\r\n");
           APP_vFactoryResetRecords();
           DBG_vPrintf(TRACE_SWITCH_NODE, "Leave -> Soft Reset\r\n");
           ResetMCU();
           }
           break;

    case ZPS_EVENT_NWK_LEAVE_CONFIRM:
            /* reset app data and restart */
        DBG_vPrintf(TRACE_SWITCH_NODE, "Leave Confirm\r\n");
        APP_vFactoryResetRecords();
        /* force a restart */
        ResetMCU();
        break;

    case ZPS_EVENT_APS_DATA_INDICATION:
        #ifdef CLD_OTA
        if ((psStackEvent->uEvent.sApsDataIndEvent.eStatus == ZPS_E_SUCCESS) &&
                (psStackEvent->uEvent.sApsDataIndEvent.u8DstEndpoint == 0))
        {
            // Data Ind for ZDp Ep
            if (ZPS_ZDP_MATCH_DESC_RSP_CLUSTER_ID == psStackEvent->uEvent.sApsDataIndEvent.u16ClusterId)
            {
                vHandleMatchDescriptor(psStackEvent);
            } else if (ZPS_ZDP_IEEE_ADDR_RSP_CLUSTER_ID == psStackEvent->uEvent.sApsDataIndEvent.u16ClusterId) {
                vHandleIeeeAddressRsp(psStackEvent);
            }
        }
        #endif
        break;

    case ZPS_EVENT_NWK_POLL_CONFIRM:
        break;

    default:
        //DBG_vPrintf(TRACE_SWITCH_NODE, "Running unhandled %d\r\n", psStackEvent->eType);
        break;
    }
}
/****************************************************************************
 *
 * NAME: APP_ZLO_Switch_Task
 *
 * DESCRIPTION:
 * Task that handles the application related functionality
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_taskSwitch(void)
{

    APP_tsEvent sAppEvent;
    sAppEvent.eType = APP_E_EVENT_NONE;

    if (ZQ_bQueueReceive(&APP_msgAppEvents, &sAppEvent) == TRUE)
    {
        DBG_vPrintf(TRACE_SWITCH_NODE, "ZPR: App event %d, NodeState=%d\r\n", sAppEvent.eType, sDeviceDesc.eNodeState);



#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
        /* Is this a button event on NTAG_FD ? */
        if ( (sAppEvent.eType == APP_E_EVENT_BUTTON_UP || sAppEvent.eType == APP_E_EVENT_BUTTON_UP)
                && (sAppEvent.uEvent.sButton.u8Button == APP_E_BUTTONS_NFC_FD) )
        {
            #if APP_NTAG_ICODE
                APP_vNtagStart(DIMMERSWITCH_SWITCH_ENDPOINT);
            #endif
            #if APP_NTAG_AES
                APP_vNtagStart(NFC_NWK_NSC_DEVICE_ZIGBEE_ROUTER_DEVICE);
            #endif
        }
        /* Other event (handle as normal) ? */
        else
#endif
        {
            if (sAppEvent.eType == APP_E_EVENT_POR_FACTORY_RESET)
            {
                if (ZPS_E_SUCCESS !=  ZPS_eAplZdoLeaveNetwork(0, FALSE,FALSE)) 
                {
                    /* Leave failed,so just reset everything */
                    DBG_vPrintf(TRUE,"Deleting the PDM\n");
                    APP_vFactoryResetRecords();
                    ResetMCU();
                }
            }
            else
            {
                if (sDeviceDesc.eNodeState == E_RUNNING)
                {
                    switch(sAppEvent.eType)
                    {
                    case APP_E_EVENT_BUTTON_DOWN:
                    case APP_E_EVENT_BUTTON_UP:
                        
                        vApp_ProcessKeyCombination(sAppEvent);
#ifdef SLEEP_ENABLE
                        vReloadSleepTimers();
#endif
                        /* TODO:Reset the channel mask to last used so that the
                        * rejoining joining will be attempted
                        * */
                        break;
                        
                    default:
                        break;
                    }
                }
                else if(sDeviceDesc.eNodeState == E_STARTUP)
                {
                    DBG_vPrintf(TRACE_SWITCH_NODE,"Start Steering\r\n");
                    sBDB.sAttrib.u32bdbPrimaryChannelSet = BDB_PRIMARY_CHANNEL_SET;
                    sBDB.sAttrib.u32bdbSecondaryChannelSet = BDB_SECONDARY_CHANNEL_SET;
                    BDB_eNsStartNwkSteering();
                }
            }
        }
    }
    if(u8KeepAliveTime == 0)
    {
        StopLed1Flashing();
        Led1Off();
    }
    else
    {
        if(TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
        {
            Led1On();
        }
        else
        {
            Led1Flashing();
        }
    }
}


/****************************************************************************
 *
 * NAME: vHandleJoinAndRejoin
 *
 * DESCRIPTION:
 * Handles the Join and Rejoin events
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vHandleJoinAndRejoin( void  )
{
    DBG_vPrintf(TRACE_SWITCH_NODE, "DEVICE_IN_NETWORK \r\n");
    uint64 u64MacAddr = *((uint64*)pvAppApiGetMacAddrLocation());
    DBG_vPrintf(TRACE_SWITCH_NODE, "vHandleJoinAndRejoin: eNodeState = E_RUNNING\r\n");
    sDeviceDesc.eNodeState = E_RUNNING;
    u16GroupId = ((uint16)u64MacAddr) ^ ((uint16)(u64MacAddr>>16));
    vSetIndividualLightInformation();
    PDM_eSaveRecordData(PDM_ID_APP_ZLO_SWITCH,
                            &sDeviceDesc,
                            sizeof(tsDeviceDesc));
    ZPS_vSaveAllZpsRecords();
    /* Stop Fast polling */
    vStartFastPolling(0);
    /* Start 1 seconds polling */
    ZTIMER_eStart(u8TimerPoll, POLL_TIME);
    ZTIMER_eStart(u8TimerTick, ZCL_TICK_TIME);
}

#ifdef SLEEP_ENABLE
#ifdef DEEP_SLEEP_ENABLE
/****************************************************************************
 *
 * NAME: vActionOnButtonActivationAfterDeepSleep
 *
 * DESCRIPTION:
 * Takes some action based on the button that activated the wake up from deep
 * sleep
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vActionOnButtonActivationAfterDeepSleep(void)
{

}

/****************************************************************************
 *
 * NAME: vLoadDeepSleepTimer
 *
 * DESCRIPTION:
 * Loads the deep sleep time
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vLoadDeepSleepTimer(uint8 u8SleepTime)
{
    u8DeepSleepTime = u8SleepTime;
}
/****************************************************************************
 *
 * NAME: bGoingDeepSleep
 *
 * DESCRIPTION:
 * Checks if the module is going to deep sleep
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC bool bGoingDeepSleep(void)
{
    if (0==u8DeepSleepTime)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif
/****************************************************************************
 *
 * NAME: vLoadKeepAliveTime
 *
 * DESCRIPTION:
 * Loads the keep alive timer based on the right conditions.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vLoadKeepAliveTime(uint8 u8TimeInSec)
{
    u8KeepAliveTime=u8TimeInSec;
    ZTIMER_eStop( u8TimerPoll);
    ZTIMER_eStart(u8TimerPoll, POLL_TIME);
    ZTIMER_eStop( u8TimerTick);
    ZTIMER_eStart(u8TimerTick, ZCL_TICK_TIME);
}

/****************************************************************************
 *
 * NAME: bWaitingToSleep
 *
 * DESCRIPTION:
 * Gets the status if the module is waiting for sleep.
 *
 * RETURNS:
 * bool
 *
 ****************************************************************************/
PUBLIC bool bWaitingToSleep(void)
{
    if (0 == u8KeepAliveTime)
        return TRUE;
    else
        return FALSE;
}

/****************************************************************************
 *
 * NAME: vStopAllTimers
 *
 * DESCRIPTION:
 * Stops all the timers
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStopAllTimers(void)
{
    vStopTimer(u8TimerPoll);
    vStopTimer(u8TimerButtonScan);
    vStopTimer(u8TimerTick);
    vStopTimer(u8TimerButtonDelay);
    vStopTimer(u8TimerChangeMode);
}

/****************************************************************************
 *
 * NAME: vUpdateKeepAliveTimer
 *
 * DESCRIPTION:
 * Updates the Keep Alive time at 1 sec call from the tick timer that served ZCL as well.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vUpdateKeepAliveTimer(void)
{
    te_SwitchState eSwitchState = eGetSwitchState();

    if( (eSwitchState == LIGHT_CONTROL_MODE ) || (eSwitchState == INDIVIDUAL_CONTROL_MODE ) )
    {
        if( u8KeepAliveTime > 0 )
        {
                                     // TODO  u8KeepAliveTime--;
            #ifdef CPU_MKW41Z512VHT4
                #if cPWR_UsePowerDownMode
                u8KeepAliveTime--;
                #endif
            #endif
            DBG_vPrintf(TRACE_SWITCH_NODE," KeepAliveTime = %d \r\n",u8KeepAliveTime);
            
        }
        else
        {
            vStopAllTimers();
            DBG_vPrintf(TRACE_SWITCH_NODE,"\n Activity %d, KeepAliveTime = %d \r\n",PWRM_u16GetActivityCount(),u8KeepAliveTime);

            #ifdef DEEP_SLEEP_ENABLE
                if(u8DeepSleepTime > 0 )
                {
                    u8DeepSleepTime--;
                    /* The activity counter seems to be still greater than 0 - Go back to be awake, Try again in the next sleep time*/
                    if(PWRM_u16GetActivityCount())
                    {
                        DBG_vPrintf(TRACE_SWITCH_NODE,"\n Abort Sleep - Reload timers\r\n");
                        vReloadSleepTimers();
                    }
                    else
                    {
                        PWRM_teStatus eStatus = PWRM_eScheduleActivity(&sWake, APP_LONG_SLEEP_DURATION_IN_SEC*1000 , vWakeCallBack);
                        DBG_vPrintf(TRACE_SWITCH_NODE,"\nSleep Status = %d, u8DeepSleepTime = %d \r\n",eStatus,u8DeepSleepTime);
                    }
                }
                else
                {
                    /*It is OK to force a deep sleep by finishing activities when we intent to enter the deep sleep*/
                    while (PWRM_u16GetActivityCount())
                    {
                        PWRM_eFinishActivity();
                    }
                    PWRM_vInit(4);
                }
            #else
                /* The activity counter seems to be still greater than 0 - Go back to be awake, Try again in the next sleep time*/
                if(PWRM_u16GetActivityCount())
                {
                    vReloadSleepTimers();
                }
                else
                {
                    /* The activity counter is 0 so a sleep with ram on can be scheduled*/
                    PWRM_teStatus eStatus = PWRM_eScheduleActivity(&sWake, APP_LONG_SLEEP_DURATION_IN_SEC*1000 , vWakeCallBack);
                    DBG_vPrintf(TRACE_SWITCH_NODE,"\nSleep Status = %d\r\n",eStatus);
                }
            #endif
        }
    }
    else
    {
        vReloadSleepTimers();
    }
}
#endif

/****************************************************************************
 *
 * NAME: vAppOnOff
 *
 * DESCRIPTION:
 *    Send out ON or OFF command, the address mode(group/unicast/bound etc)
 *    is taken from the selected light index set by the caller
 *
 * RETURNS:
 * void
 *
****************************************************************************/
PUBLIC void vAppOnOff(teCLD_OnOff_Command eCmd) {
    
    uint8 u8Seq;
    tsZCL_Address sAddress;
    
    vSetAddress(&sAddress, FALSE, GENERAL_CLUSTER_ID_ONOFF);
    
    if ((eCmd == E_CLD_ONOFF_CMD_ON) || (eCmd == E_CLD_ONOFF_CMD_OFF) || (eCmd == E_CLD_ONOFF_CMD_TOGGLE)) 
    {
        eCLD_OnOffCommandSend(u8MyEndpoint,
                              sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                              &sAddress, &u8Seq, eCmd);
    }
}


/****************************************************************************
 *
 * NAME: vAppIdentify
 *
 * DESCRIPTION:
 *    Send out Identify command, the address mode(group/unicast/bound etc)
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppIdentify( uint16 u16Time) {
    uint8 u8Seq;
    tsZCL_Address sAddress;
    tsCLD_Identify_IdentifyRequestPayload sPayload;

    sPayload.u16IdentifyTime = u16Time;

    vSetAddress(&sAddress,FALSE,GENERAL_CLUSTER_ID_IDENTIFY);

    eCLD_IdentifyCommandIdentifyRequestSend(
                            u8MyEndpoint,
                            sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                            &sAddress,
                            &u8Seq,
                            &sPayload);
}

/****************************************************************************
 *
 * NAME: vAppLevelMove
 *
 * DESCRIPTION:
 *    Send out Level Up or Down command, the address mode(group/unicast/bound etc)
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppLevelMove(teCLD_LevelControl_MoveMode eMode, uint8 u8Rate, bool_t bWithOnOff)
{
    tsCLD_LevelControl_MoveCommandPayload sPayload = {0};
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, FALSE,GENERAL_CLUSTER_ID_LEVEL_CONTROL);

    sPayload.u8Rate = u8Rate;
    sPayload.u8MoveMode = eMode;

    eCLD_LevelControlCommandMoveCommandSend(
                                    u8MyEndpoint,
                                    sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                                    &sAddress,
                                    &u8Seq,
                                    bWithOnOff, /* with on off */
                                    &sPayload);
}


/****************************************************************************
 *
 * NAME: vAppLevelStop
 *
 * DESCRIPTION:
 *    Send out Level Stop command, the address mode(group/unicast/bound etc)
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppLevelStop(void)
{
    tsCLD_LevelControl_StopCommandPayload sPayload = {0};
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, FALSE,GENERAL_CLUSTER_ID_LEVEL_CONTROL);
    eCLD_LevelControlCommandStopCommandSend(
                        u8MyEndpoint,
                        sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                        &sAddress,
                        &u8Seq,
                        FALSE, /* without on off */
                        &sPayload);
}


/****************************************************************************
 *
 * NAME: vAppLevelStepMove
 *
 * DESCRIPTION:
 *    Send out Step Move command, the address mode(group/unicast/bound etc)
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppLevelStepMove(teCLD_LevelControl_MoveMode eMode)
{
    tsCLD_LevelControl_StepCommandPayload sPayload = {0};
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, FALSE,GENERAL_CLUSTER_ID_LEVEL_CONTROL);

    sPayload.u16TransitionTime = 0x000a;
    sPayload.u8StepMode = eMode;
    sPayload.u8StepSize = 0x20;
    eCLD_LevelControlCommandStepCommandSend(
                        u8MyEndpoint,
                        sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                        &sAddress,
                        &u8Seq,
                        FALSE,               /* with on off */
                        &sPayload);
}

#ifdef CLD_SCENES
/****************************************************************************
 *
 * NAME: vAppRecallSceneById
 *
 * DESCRIPTION:
 *    Send out Recall Scene command, the address is group addressing
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppRecallSceneById( uint8 u8SceneId, uint16 u16GroupId)
{

    tsCLD_ScenesRecallSceneRequestPayload sPayload;
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, FALSE,GENERAL_CLUSTER_ID_SCENES);

    DBG_vPrintf(TRACE_SWITCH_NODE, "\nRecall Scene %d\r\n", u8SceneId);

    sPayload.u16GroupId = u16GroupId;
    sPayload.u8SceneId = u8SceneId;

    eCLD_ScenesCommandRecallSceneRequestSend(
                            u8MyEndpoint,
                            sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                            &sAddress,
                            &u8Seq,
                            &sPayload);

}
/****************************************************************************
 *
 * NAME: vAppStoreSceneById
 *
 * DESCRIPTION:
 *    Send out Store Scene command, the address mode is group addressing
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppStoreSceneById(uint8 u8SceneId, uint16 u16GroupId)
{
    tsCLD_ScenesStoreSceneRequestPayload sPayload;

    uint8 u8Seq;
    tsZCL_Address sAddress;

    sAddress.eAddressMode = E_ZCL_AM_GROUP;
    sAddress.uAddress.u16DestinationAddress = u16GroupId;

    sPayload.u16GroupId = u16GroupId;
    sPayload.u8SceneId = u8SceneId;


    eCLD_ScenesCommandStoreSceneRequestSend(
                            u8MyEndpoint,
                            sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,   /* dst ep */
                            &sAddress,
                            &u8Seq,
                            &sPayload);

}

#endif

/****************************************************************************
 *
 * NAME: vAppAddGroup
 *
 * DESCRIPTION:
 *    Send out Add Group command, the address mode is unicast addressing and
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppAddGroup( uint16 u16GroupId, bool_t bBroadcast)
{

    tsCLD_Groups_AddGroupRequestPayload sPayload;
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, bBroadcast,GENERAL_CLUSTER_ID_GROUPS);

    sPayload.sGroupName.pu8Data = (uint8*)"";
    sPayload.sGroupName.u8Length = 0;
    sPayload.sGroupName.u8MaxLength = 0;
    sPayload.u16GroupId = u16GroupId;

    eCLD_GroupsCommandAddGroupRequestSend(
                            u8MyEndpoint,
                            sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                            &sAddress,
                            &u8Seq,
                            &sPayload);

}

/****************************************************************************
 *
 * NAME: vAppRemoveGroup
 *
 * DESCRIPTION:
 *    Send out remove group command, the address mode (group/unicast/bound etc)
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppRemoveGroup( uint16 u16GroupId, bool_t bBroadcast)
{

    tsCLD_Groups_RemoveGroupRequestPayload sPayload;
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, bBroadcast,GENERAL_CLUSTER_ID_GROUPS);

    sPayload.u16GroupId = u16GroupId;

    eCLD_GroupsCommandRemoveGroupRequestSend(
                            u8MyEndpoint,
                            sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                            &sAddress,
                            &u8Seq,
                            &sPayload);

}

/****************************************************************************
 *
 * NAME: vAppRemoveAllGroups
 *
 * DESCRIPTION:
 *    Send out Remove All group command, the address mode(group/unicast/bound etc)
 *    is taken from the selected light index set by the caller
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppRemoveAllGroups(bool_t bBroadcast)
{

    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, bBroadcast,GENERAL_CLUSTER_ID_GROUPS);

    eCLD_GroupsCommandRemoveAllGroupsRequestSend(
                            u8MyEndpoint,
                            sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u8Ep,
                            &sAddress,
                            &u8Seq);

}

/****************************************************************************
 *
 * NAME: vStopTimer
 *
 * DESCRIPTION:
 * Stops the timer
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStopTimer(uint8 u8Timer)
{
    ZTIMER_eStop(u8Timer);
}


/****************************************************************************
 *
 * NAME: vManageWakeUponSysControlISR
 *
 * DESCRIPTION:
 * Called from SysControl ISR to process the wake up conditions
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vManageWakeUponSysControlISR(teInterruptType eInterruptType)
{
    #ifdef SLEEP_ENABLE
        /*In any case this could be a wake up from timer interrupt or from buttons
         * press
         * */
        if(TRUE == bWakeUpFromSleep())
        {
            /*Only called if the module is coming out of sleep */
            #ifdef CLD_OTA
                if(eInterruptType == E_INTERRUPT_WAKE_TIMER_EXPIRY)
                {
                    /* Increment time out value by sleep duration in seconds */
                    vIncrementTimeOut(APP_LONG_SLEEP_DURATION_IN_SEC);
                }
            #endif
            /*Only called if the module is coming out of sleep */
            DBG_vPrintf(TRACE_SWITCH_NODE,"vISR_SystemController on WakeUP\r\n");
            vLoadKeepAliveTime(KEEP_ALIVETIME);
            vWakeCallBack();
        }
    #endif
}
#ifdef SLEEP_ENABLE
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
    DBG_vPrintf(TRACE_SWITCH_NODE, "vWakeCallBack\r\n");

    /*Start Polling*/
    ZTIMER_eStart(u8TimerPoll, POLL_TIME);

    /*Start the APP_TickTimer to continue the ZCL tasks */
    ZTIMER_eStart(u8TimerTick, ZCL_TICK_TIME);
}

/****************************************************************************
 *
 * NAME: App_SedWakeUpFromKeyBoard
 *
 * DESCRIPTION:
 * Stop sleeping for KEEP_ALIVETIME.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void App_SedWakeUpFromKeyBoard(void)
{
     /* stop sleeping for KEEP_ALIVETIME */
     u8KeepAliveTime = KEEP_ALIVETIME;
     APP_cbTimerPoll(NULL);
}
#endif
/****************************************************************************
 *
 * NAME: APP_cbTimerPoll
 *
 * DESCRIPTION:
 * Poll Task for the polling as well it triggers the rejoin in case of pool failure
 * It also manages sleep timing.
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerPoll(void *pvParam)
{
    uint32 u32PollPeriod = POLL_TIME;


    if(
    #ifdef SLEEP_ENABLE
      !bWaitingToSleep() &&
    #endif
       /* Do fast polling when the device is running */
      ((sDeviceDesc.eNodeState == E_RUNNING) || (sDeviceDesc.eNodeState == E_WAIT_STARTUP)))

    {

        if( u16FastPoll )
        {
            u16FastPoll--;
            u32PollPeriod = POLL_TIME_FAST;
            /*Reload the Sleep timer during fast poll*/
            #ifdef SLEEP_ENABLE
                vReloadSleepTimers();
            #endif
        }
        ZTIMER_eStop(u8TimerPoll);
        ZTIMER_eStart(u8TimerPoll, u32PollPeriod);

        ZPS_teStatus u8PStatus;
        u8PStatus = ZPS_eAplZdoPoll();
        if( u8PStatus )
        {
            DBG_vPrintf(TRACE_SWITCH_NODE, "\nPoll Failed %d\r\n", u8PStatus );
        }
    }
}

/****************************************************************************
 *
 * NAME: vSetAddress
 *
 * DESCRIPTION:
 *     Set Address Mode for the outgoing commands
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vSetAddress(tsZCL_Address * psAddress, bool_t bBroadcast, uint16 u16ClusterId)
{
    if (bBroadcast)
    {
        DBG_vPrintf(TRACE_SWITCH_NODE, "\r\nBcast u16ClusterId=%d\r\n",u16ClusterId);

        psAddress->eAddressMode = E_ZCL_AM_BROADCAST;
        psAddress->uAddress.eBroadcastMode = ZPS_E_APL_AF_BROADCAST_RX_ON;
    }
    else
    {
        /*Get The switch states to decide the address mode to be taken up.*/
        switch (eGetSwitchState())
        {
            case LIGHT_CONTROL_MODE:
                /*By Default chose Group Addressing*/
                DBG_vPrintf(TRACE_SWITCH_NODE, "\nGroup u16ClusterId=%d\r\n",u16ClusterId);

                psAddress->eAddressMode = E_ZCL_AM_GROUP;
                psAddress->uAddress.u16DestinationAddress = u16GroupId;
                break;

            case COMMISSIONING_MODE:
            case INDIVIDUAL_CONTROL_MODE:
                DBG_vPrintf(TRACE_SWITCH_NODE, "\nUcastMatch u16ClusterId=%d\r\n",u16ClusterId);

                if(sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u16Address <= 0xFFF8 )
                {
                    psAddress->eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
                    psAddress->uAddress.u16DestinationAddress = sDeviceInfo.sLightInfo[sDeviceInfo.u8Index].u16Address;
                    DBG_vPrintf(TRACE_SWITCH_NODE, "\nAddress %d \r\n",psAddress->uAddress.u16DestinationAddress);
                }
                else
                {
                    psAddress->eAddressMode = E_ZCL_AM_ENUM_END;
                    psAddress->uAddress.u16DestinationAddress = 0xFFFF;
                }
                break;

            default:
                break;
        }
    }
}

/****************************************************************************
 *
 * NAME: bAddressInTable
 *
 * DESCRIPTION:
 * Checks if an address is already present in the last discovery
 *
 * PARAMETERS:
 * uint16 u16AddressToCheck
 *
 * RETURNS:
 * bool
 *
 ****************************************************************************/
PRIVATE bool bAddressInTable( uint16 u16AddressToCheck )
{
    uint8 i;

    for( i=0; i < NUMBER_DEVICE_TO_BE_DISCOVERED; i++ )
    {
        /* Commented out due to excessive calls */
        if(sDeviceInfo.sLightInfo[i].u16Address == u16AddressToCheck )
        {
            DBG_vPrintf(TRACE_SWITCH_NODE, "\n bAddressInTable dup!\r\n");
            return TRUE;
        }
    }

    return FALSE;

}

/****************************************************************************
 *
 * NAME: vSelectLight
 *
 * DESCRIPTION:
 * Set the next node to identify
 *
 * PARAMETERS:
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vSelectLight(void)
{
    if(bLightsDiscovered())
    {
        /* Select next node from the list for commissioning */
        if(sDeviceInfo.u8Index > 0)
            sDeviceInfo.u8Index--;
        else
            sDeviceInfo.u8Index = sDeviceInfo.u8Discovered-1;
        /* Send identify to the selected node */
        vAppIdentify(APP_IDENTIFY_TIME_IN_SECS);
    }
}

/****************************************************************************
 *
 * NAME: vSetIndividualLightInformation
 *
 * DESCRIPTION:
 * Discovery Individual Lights From Find And Bind Table
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vSetIndividualLightInformation(void)
{
    ZPS_tsAplAib * psAplAib;
    uint8 u8BindingTableSize;
    uint16 u16NwkAddressOfInterest = 0;
    psAplAib  = ZPS_psAplAibGetAib();

    if(psAplAib->psAplApsmeAibBindingTable == NULL)
    {
        DBG_vPrintf(TRACE_SWITCH_NODE,"\n No Binding Table\r\n");
        return;
    }
    else
    {
        u8BindingTableSize = psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable[0].u32SizeOfBindingTable;
        DBG_vPrintf(TRACE_SWITCH_NODE, "\nBind Size %d\r\n",  u8BindingTableSize );
        if( 0 == u8BindingTableSize)
        {
            DBG_vPrintf(TRACE_SWITCH_NODE,"\n Binding Table WithOut Any Entry \r\n");
            return;
        }
        else
        {
            uint32 j;
            for( j = 0 ; j < psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable[0].u32SizeOfBindingTable ; j++ )
            {
                //DBG_vPrintf(TRACE_SWITCH_NODE, "\n Looping Binding Table = %d \r\n",j );
                if ( ((GENERAL_CLUSTER_ID_ONOFF == psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable[0].pvAplApsmeBindingTableEntryForSpSrcAddr[j].u16ClusterId) ||
                        (GENERAL_CLUSTER_ID_LEVEL_CONTROL == psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable[0].pvAplApsmeBindingTableEntryForSpSrcAddr[j].u16ClusterId)) &&
                         (psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable[0].pvAplApsmeBindingTableEntryForSpSrcAddr[j].u8DstAddrMode == E_ZCL_AM_IEEE))
                {
                    //DBG_vPrintf(TRACE_SWITCH_NODE,"\n Binding Table Entry for Address = %x \r\n",psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable[0].pvAplApsmeBindingTableEntryForSpSrcAddr[j].uDstAddress.u64Addr);
                    u16NwkAddressOfInterest = ZPS_u16AplZdoLookupAddr( ZPS_u64NwkNibGetMappedIeeeAddr( ZPS_pvAplZdoGetNwkHandle(), psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable->pvAplApsmeBindingTableEntryForSpSrcAddr[j].u16AddrOrLkUp) );
                    if(bAddressInTable(u16NwkAddressOfInterest) == FALSE)
                    {
                        DBG_vPrintf(TRACE_SWITCH_NODE,"\n Binding Table Entry j=%d u8Ep=%d sDeviceInfo.u8Discovered=%d for u16NwkAddressOfInterest = %x \r\n",
                              j,sDeviceInfo.u8Discovered,sDeviceInfo.sLightInfo[sDeviceInfo.u8Discovered].u8Ep,
                              u16NwkAddressOfInterest);
                    
                        sDeviceInfo.sLightInfo[sDeviceInfo.u8Discovered].u16Address = u16NwkAddressOfInterest;
                        sDeviceInfo.sLightInfo[sDeviceInfo.u8Discovered].u8Ep = psAplAib->psAplApsmeAibBindingTable->psAplApsmeBindingTable[0].pvAplApsmeBindingTableEntryForSpSrcAddr[j].u8DestinationEndPoint;
                        sDeviceInfo.u8Discovered++;
                    }
                }
            }
        }
    }
}

/****************************************************************************
*
* NAME: vAppChangeChannel
*
* DESCRIPTION: This function change the channel randomly to one of the other
* primaries
*
* RETURNS:
* void
*
****************************************************************************/
PUBLIC void vAppChangeChannel( void)
{
    /*Primary channel Set */
    uint8 au8ZHAChannelSet[]={11,14,15,19,20,24,25};

    ZPS_tsAplZdpMgmtNwkUpdateReq sZdpMgmtNwkUpdateReq;
    PDUM_thAPduInstance hAPduInst;
    ZPS_tuAddress uDstAddr;
    uint8 u8Seq;
    uint8 u8Min=0, u8Max=6;
    uint8 u8CurrentChannel, u8RandomNum;

    hAPduInst = PDUM_hAPduAllocateAPduInstance(apduZDP);
    if (hAPduInst != NULL)
    {
        sZdpMgmtNwkUpdateReq.u8ScanDuration = 0xfe;

        u8CurrentChannel = ZPS_u8AplZdoGetRadioChannel();
        u8RandomNum = RND_u32GetRand(u8Min,u8Max);
        if(u8CurrentChannel != au8ZHAChannelSet[u8RandomNum])
        {
            sZdpMgmtNwkUpdateReq.u32ScanChannels = (1<<au8ZHAChannelSet[u8RandomNum]);
        }
        else /* Increment the channel by one rather than spending in RND_u32GetRand */
        {
            /*  For roll over situation */
            if(u8RandomNum == u8Max)
            {
                sZdpMgmtNwkUpdateReq.u32ScanChannels = (1<<au8ZHAChannelSet[u8Min]);
            }
            else
            {
                sZdpMgmtNwkUpdateReq.u32ScanChannels = (1<<au8ZHAChannelSet[u8RandomNum+1]);
            }
        }

        sZdpMgmtNwkUpdateReq.u8NwkUpdateId = ZPS_psAplZdoGetNib()->sPersist.u8UpdateId + 1;
        uDstAddr.u16Addr = 0xfffd;

        if ( 0 == ZPS_eAplZdpMgmtNwkUpdateRequest( hAPduInst,
                                         uDstAddr,
                                         FALSE,
                                         &u8Seq,
                                         &sZdpMgmtNwkUpdateReq))
        {
            DBG_vPrintf(TRACE_SWITCH_NODE, "update Id\r\n");
            /* should really be in stack?? */
            ZPS_psAplZdoGetNib()->sPersist.u8UpdateId++;
        }
    }
}
#ifdef SLEEP_ENABLE
/****************************************************************************
*
* NAME: vReloadSleepTimers
*
* DESCRIPTION:
* reloads boththe timers on identify
*
* RETURNS:
* void
*
****************************************************************************/
PUBLIC void vReloadSleepTimers(void)
{
    vLoadKeepAliveTime(KEEP_ALIVETIME);
    #ifdef DEEP_SLEEP_ENABLE
        vLoadDeepSleepTimer(DEEP_SLEEP_TIME);
    #endif
}
#endif

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
    uint64 u64MacAddr = *((uint64*)pvAppApiGetMacAddrLocation());

    DBG_vPrintf(TRACE_SWITCH_NODE, "\nNon Factory New Start\r\n");
    ZPS_vSaveAllZpsRecords();
    u16GroupId = ((uint16)u64MacAddr) ^ ((uint16)(u64MacAddr>>16));
    /* Start 1 seconds polling */
    ZTIMER_eStart(u8TimerPoll, POLL_TIME);
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
    // BDB_eNsStartNwkSteering();  done later on  only if no OOB commissionning ????

    sDeviceDesc.eNodeState = E_STARTUP;  //E_JOINING_NETWORK;
    /* Stay awake for joining */
    DBG_vPrintf(TRACE_SWITCH_NODE, "Factory New Start\r\n");
    //DBG_vPrintf(TRACE_SWITCH_NODE, "app_vStartNodeFactoryNew: eNodeState = E_STARTUP\n");
    
    //sDeviceDesc.eNodeState = E_STARTUP;
}

#ifdef CLD_OTA
/****************************************************************************
 *
 * NAME: eGetNodeState
 *
 * DESCRIPTION:
 * returns the device state
 *
 * RETURNS:
 * teNODE_STATES
 *
 ****************************************************************************/
PUBLIC teNODE_STATES eGetNodeState(void)
{
    return sDeviceDesc.eNodeState;
}

/****************************************************************************
 *
 * NAME: sGetOTACallBackPersistdata
 *
 * DESCRIPTION:
 * returns a copy to the OTA persistent data from the device structure.
 *
 * RETURNS:
 * tsOTA_PersistedData
 *
 ****************************************************************************/
PUBLIC tsOTA_PersistedData sGetOTACallBackPersistdata(void)
{
    return sSwitch.sCLD_OTA_CustomDataStruct.sOTACallBackMessage.sPersistedData;
}
#endif

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
PUBLIC void APP_vFactoryResetRecords( void)
{
    /* clear out the stack */
    ZPS_vDefaultStack();
    (void)ZPS_eAplAibSetApsUseExtendedPanId(ZPS_APS_AIB_INIT_USE_EXTENDED_PANID);
    /* clear out the application */
    DBG_vPrintf(TRACE_SWITCH_NODE, "vFactoryResetRecords: eNodeState = E_STARTUP\r\n");
    sDeviceDesc.eNodeState = E_STARTUP;
    FLib_MemSet(&sDeviceInfo, 0 , sizeof(tsDeviceInfo));

#ifdef CLD_OTA
    vOTAResetPersist();
#endif

    /* save everything */
    PDM_eSaveRecordData(PDM_ID_APP_ZLO_SWITCH,
                            &sDeviceDesc,
                            sizeof(tsDeviceDesc));
    ZPS_vSaveAllZpsRecords();
#ifdef CPU_MKW41Z512VHT4
    NvCompletePendingOperations();
#endif
}

PUBLIC bool_t APP_bNodeIsInRunningState(void)
{
    DBG_vPrintf(TRACE_SWITCH_NODE, "\nAPP Sensor Node: NodeState=%d\r\n", sDeviceDesc.eNodeState);
    return (sDeviceDesc.eNodeState == E_RUNNING) ? TRUE:FALSE;
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
