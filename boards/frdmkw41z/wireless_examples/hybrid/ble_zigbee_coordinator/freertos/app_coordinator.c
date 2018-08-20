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
\file       app_coordinator.c
\brief      Base Device Demo: Coordinator application
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "dbg.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"
#include "pwrm.h"
#include "PDM.h"
#include "app_zps_cfg.h"
#include "app_zcl_cfg.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_apl_aib.h"
#include "zps_apl_zdo.h"
#include "zps_apl_zdp.h"
#include "app_coordinator.h"
#include "app_zcl_task.h"
#include "app_buttons.h"
#include "app_leds.h"
#include "PDM_IDs.h"
#include "app_common.h"
#include "app_events.h"
#include "app_main.h"
#include "ZQueue.h"
#include "ZTimer.h"
#include "app_cfg.h"
#include "OnOff.h"
#include "RNG_Interface.h"
#ifdef JN517x
#include "AHI_ModuleConfiguration.h"
#endif
#ifdef CPU_MKW41Z512VHT4
#include "NVM_Interface.h"
#include "app_zb_utils.h"
#endif


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef DEBUG_APP
    #define TRACE_APP   TRUE
#else
    #define TRACE_APP   FALSE
#endif

#ifdef DEBUG_APP_EVENT
    #define TRACE_APP_EVENT   TRUE
#else
    #define TRACE_APP_EVENT   FALSE
#endif

#ifdef DEBUG_APP_BDB
    #define TRACE_APP_BDB     TRUE
#else
    #define TRACE_APP_BDB     FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent);
PRIVATE void vAppHandleZdoEvents( BDB_tsZpsAfEvent *psZpsAfEvent);
PRIVATE void vAppSendOnOff(void);
PRIVATE void vAppSendIdentifyStop( uint16 u16Address, uint8 u8Endpoint);
PRIVATE void vAppSendRemoteBindRequest(uint16 u16DstAddr, uint16 u16ClusterId, uint8 u8DstEp);
PRIVATE void APP_vBdbInit(void);
#ifndef CPU_MKW41Z512VHT4
PRIVATE void vDeletePDMOnButtonPress(uint8 u8ButtonDIO);
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE teNodeState eNodeState;

/* Register to pdm/nvm eNodeState */
NVM_RegisterDataSet(&eNodeState, 1, sizeof(eNodeState), PDM_ID_APP_COORD, gNVM_MirroredInRam_c);
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vInitialiseCoordinator
 *
 * DESCRIPTION:
 * Initialises the Coordinator application
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitialiseCoordinator(void)
{
    /* Restore any application data previously saved to flash
     * All Application records must be loaded before the call to
     * ZPS_eAplAfInit
     */
#ifndef CPU_MKW41Z512VHT4
    APP_bButtonInitialise();
#endif
    eNodeState = E_STARTUP;
    uint16 u16ByteRead;
    PDM_eReadDataFromRecord(PDM_ID_APP_COORD,
                            &eNodeState,
                            sizeof(eNodeState),
                            &u16ByteRead);
#ifdef JN517x
    /* Default module configuration: change E_MODULE_DEFAULT as appropriate */
      vAHI_ModuleConfigure(E_MODULE_DEFAULT);
#endif

#ifdef CPU_MKW41Z512VHT4
    APP_vSetMacAddr();
#endif
    /* Initialise ZBPro stack */
    ZPS_eAplAfInit();
    
    if(eNodeState != E_RUNNING)
    {
      ZPS_vNwkNibSetPanId (ZPS_pvAplZdoGetNwkHandle(), (uint16) RND_u32GetRand ( 1, 0xfff0 ) );
    }

    APP_ZCL_vInitialise();

#ifdef PDM_EEPROM
    vDisplayPDMUsage();
#endif

    /* Initialise other software modules
     * HERE
     */
    APP_vBdbInit();
#ifndef CPU_MKW41Z512VHT4
#ifndef DONGLE
    /* Delete PDM if required */
    vDeletePDMOnButtonPress(APP_BOARD_SW0_PIN);
#endif
#endif
    /* Always initialise any peripherals used by the application
     * HERE
     */
#ifndef CPU_MKW41Z512VHT4
    APP_vLedInitialise();
#endif
    DBG_vPrintf(TRACE_APP, "Recovered Application State %d On Network %d\r\n",
            eNodeState, sBDB.sAttrib.bbdbNodeIsOnANetwork);
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
    static uint8 u8NoQueryCount;

    switch(psBdbEvent->eEventType)
    {
        case BDB_EVENT_NONE:
            break;

        case BDB_EVENT_ZPSAF:                // Use with BDB_tsZpsAfEvent
            vAppHandleAfEvent(&psBdbEvent->uEventData.sZpsAfEvent);
            break;

        case BDB_EVENT_INIT_SUCCESS:
            eNodeState = E_RUNNING;
            break;

        case BDB_EVENT_NWK_FORMATION_SUCCESS:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: NwkFormation Success \r\n");
            PDM_eSaveRecordData(PDM_ID_APP_COORD,
                                &eNodeState,
                                sizeof(eNodeState));
            break;

        case BDB_EVENT_NWK_STEERING_SUCCESS:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: NwkSteering Success\r\n");
            break;

        case BDB_EVENT_NWK_FORMATION_FAILURE:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Network Formation Failure\r\n");
            break;

        case BDB_EVENT_FB_HANDLE_SIMPLE_DESC_RESP_OF_TARGET:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: F&B Simple Desc response From %04x Profle %04x Device %04x Ep %d Version %d\r\n",
                    psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress,
                    psBdbEvent->uEventData.psFindAndBindEvent->u16ProfileId,
                    psBdbEvent->uEventData.psFindAndBindEvent->u16DeviceId,
                    psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp,
                    psBdbEvent->uEventData.psFindAndBindEvent->u8DeviceVersion);
            break;

        case BDB_EVENT_FB_CHECK_BEFORE_BINDING_CLUSTER_FOR_TARGET:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Check For Binding Cluster %04x\r\n",
                        psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId);
            break;
        case BDB_EVENT_FB_CLUSTER_BIND_CREATED_FOR_TARGET:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Bind Created for cluster %04x\r\n",
                    psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId);
            vAppSendRemoteBindRequest( psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress,
                                       psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId,
                                       psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp);
            break;

        case BDB_EVENT_FB_BIND_CREATED_FOR_TARGET:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Bind Created for target EndPt %d\r\n",
                    psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp);

#ifndef USE_GROUPS
            vAppSendIdentifyStop( psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress,
                                  psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp);
#endif
            break;

        case BDB_EVENT_FB_GROUP_ADDED_TO_TARGET:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Group Added with ID %04x\r\n",
                    psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16GroupId);
            u8NoQueryCount = 0;
#ifdef USE_GROUPS
                //Example to ask to Stop identification to that group
            vAppSendIdentifyStop(psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress,
                                 psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp);
            vAppSendRemoteBindRequest( psBdbEvent->uEventData.psFindAndBindEvent->u16TargetAddress,
                    				   psBdbEvent->uEventData.psFindAndBindEvent->uEvent.u16ClusterId,
									   psBdbEvent->uEventData.psFindAndBindEvent->u8TargetEp);
#endif
            break;

        case BDB_EVENT_FB_ERR_BINDING_FAILED:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Binding Failed\r\n");
            break;

        case BDB_EVENT_FB_ERR_BINDING_TABLE_FULL:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Binding Table Full\r\n");
            break;

        case BDB_EVENT_FB_ERR_GROUPING_FAILED:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Grouping Failed\r\n");
            break;

        case BDB_EVENT_FB_NO_QUERY_RESPONSE:

            if(u8NoQueryCount >= 2)
            {
                u8NoQueryCount = 0;
                BDB_vFbExitAsInitiator();
                DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: No Identify Query Response Stopping F&B\r\n");
            }
            else
            {
                u8NoQueryCount++;
                DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: No Identify Query Response\r\n");
            }
            break;

        case BDB_EVENT_FB_TIMEOUT:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: F&B Timeout\r\n");
            break;


        default:
            DBG_vPrintf(TRACE_APP_BDB,"APP-BDB: Unhandled %d\r\n", psBdbEvent->eEventType);
            break;
    }
}


/****************************************************************************
 *
 * NAME: APP_taskCoordinator
 *
 * DESCRIPTION:
 * Main state machine
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_taskCoordinator(void)
{
    BDB_teStatus eStatus;
    APP_tsEvent sAppEvent;
    sAppEvent.eType = APP_E_EVENT_NONE;

    if (ZQ_bQueueReceive(&APP_msgAppEvents, &sAppEvent) == TRUE)
    {
        DBG_vPrintf(TRACE_APP_EVENT, "APP-EVT: Event %d, NodeState=%d\r\n", sAppEvent.eType, eNodeState);

        if (sAppEvent.eType == APP_E_EVENT_BUTTON_DOWN)
        {
            switch(sAppEvent.uEvent.sButton.u8Button)
            {
            case APP_E_BUTTONS_BUTTON_SW5:
                    /* On.Off Toggle */
                    DBG_vPrintf(TRACE_APP_EVENT, "APP-EVT: Send Toggle Cmd\r\n");
                    vAppSendOnOff();
                    break;

                case APP_E_BUTTONS_BUTTON_SW4:
                    /* Permit Join */
                    if (TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                    {
                        eStatus = BDB_eNsStartNwkSteering();
                        DBG_vPrintf(TRACE_APP_EVENT, "APP-EVT: Request Nwk Steering %02x\r\n", eStatus);
                    }
                    break;

                case APP_E_BUTTONS_BUTTON_SW3:
                    /* Create Network */
                    if (FALSE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                    {
                        eStatus = BDB_eNfStartNwkFormation();
                        DBG_vPrintf(TRACE_APP_EVENT, "APP-EVT: Request Nwk Formation %02x\r\n", eStatus);
                    }
                    break;

                case APP_E_BUTTONS_BUTTON_SW2:
                    /* Find & Bind */
#ifdef USE_GROUPS
                    sBDB.sAttrib.u16bdbCommissioningGroupID = GROUP_ID;
#endif
                    eStatus = BDB_eFbTriggerAsInitiator(COORDINATOR_APPLICATION_ENDPOINT);
                    DBG_vPrintf(TRACE_APP_EVENT, "APP-EVT: Find and Bind initiate %02x\r\n", eStatus);
                    break;

                case APP_E_BUTTONS_BUTTON_1:
                    /* Factory Reset */
                    if (ZPS_E_SUCCESS !=  ZPS_eAplZdoLeaveNetwork(0, FALSE,FALSE))
                    {
                        /* Leave failed,so just reset everything */
                        DBG_vPrintf(TRUE,"Deleting the PDM\n");
                        APP_vFactoryResetRecords();
                        ResetMCU();
                    }
                    break;

                default:
                    break;
            }
        }
        else if (sAppEvent.eType == APP_E_EVENT_LEAVE_AND_RESET)
        {
            DBG_vPrintf(TRACE_APP_EVENT, "APP-EVT: Leave nd Reset\r\n");
        }

    }

    APP_vSetLedState(TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork);
}


/****************************************************************************
 *
 * NAME: vAppHandleAfEvent
 *
 * DESCRIPTION:
 * Application handler for stack events
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent)
{
    if (psZpsAfEvent->u8EndPoint == COORDINATOR_APPLICATION_ENDPOINT)
    {
        if ((psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_DATA_INDICATION) ||
            (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_INTERPAN_DATA_INDICATION))
        {
            APP_ZCL_vEventHandler( &psZpsAfEvent->sStackEvent);
         }
    }
    else if (psZpsAfEvent->u8EndPoint == COORDINATOR_ZDO_ENDPOINT)
    {
        vAppHandleZdoEvents( psZpsAfEvent);
    }

    /* free up any Apdus */
    if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_DATA_INDICATION)
    {
        PDUM_eAPduFreeAPduInstance(psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.hAPduInst);
    }
    else if ( psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_INTERPAN_DATA_INDICATION )
    {
        PDUM_eAPduFreeAPduInstance(psZpsAfEvent->sStackEvent.uEvent.sApsInterPanDataIndEvent.hAPduInst);
    }


}
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
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
    ZPS_tsAfEvent *psAfEvent = &(psZpsAfEvent->sStackEvent);

    switch(psAfEvent->eType)
    {
        case ZPS_EVENT_APS_DATA_INDICATION:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Data Indication Status %02x from %04x Src Ep %d Dst Ep %d Profile %04x Cluster %04x\r\n",
                    psAfEvent->uEvent.sApsDataIndEvent.eStatus,
                    psAfEvent->uEvent.sApsDataIndEvent.uSrcAddress.u16Addr,
                    psAfEvent->uEvent.sApsDataIndEvent.u8SrcEndpoint,
                    psAfEvent->uEvent.sApsDataIndEvent.u8DstEndpoint,
                    psAfEvent->uEvent.sApsDataIndEvent.u16ProfileId,
                    psAfEvent->uEvent.sApsDataIndEvent.u16ClusterId);
            break;

        case ZPS_EVENT_APS_DATA_CONFIRM:
            break;

        case ZPS_EVENT_APS_DATA_ACK:
            break;
            break;

        case ZPS_EVENT_NWK_STARTED:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Network started Channel = %d\r\n", ZPS_u8AplZdoGetRadioChannel() );
            break;

        case ZPS_EVENT_NWK_FAILED_TO_START:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Network Failed To start\r\n");
            break;

        case ZPS_EVENT_NWK_NEW_NODE_HAS_JOINED:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: New Node %04x Has Joined\r\n",
                    psAfEvent->uEvent.sNwkJoinIndicationEvent.u16NwkAddr);
            break;

        case ZPS_EVENT_NWK_DISCOVERY_COMPLETE:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Discovery Complete %02x\r\n",
                    psAfEvent->uEvent.sNwkDiscoveryEvent.eStatus);
            break;

        case ZPS_EVENT_NWK_LEAVE_INDICATION:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Leave Indication %016llx Rejoin %d\r\n",
                    psAfEvent->uEvent.sNwkLeaveIndicationEvent.u64ExtAddr,
                    psAfEvent->uEvent.sNwkLeaveIndicationEvent.u8Rejoin);
            break;

        case ZPS_EVENT_NWK_LEAVE_CONFIRM:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Leave Confirm status %02x Addr %016llx\r\n",
                    psAfEvent->uEvent.sNwkLeaveConfirmEvent.eStatus,
                    psAfEvent->uEvent.sNwkLeaveConfirmEvent.u64ExtAddr);
            if ( (psAfEvent->uEvent.sNwkLeaveConfirmEvent.eStatus == ZPS_E_SUCCESS) &&
                    (psAfEvent->uEvent.sNwkLeaveConfirmEvent.u64ExtAddr == 0ULL) &&
                    (psAfEvent->uEvent.sNwkLeaveConfirmEvent.bRejoin == FALSE))
            {
                /*-we left  */
                APP_vFactoryResetRecords();
                NVIC_SystemReset();
            }
            break;

        case ZPS_EVENT_NWK_STATUS_INDICATION:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Network status Indication %02x addr %04x\r\n",
                    psAfEvent->uEvent.sNwkStatusIndicationEvent.u8Status,
                    psAfEvent->uEvent.sNwkStatusIndicationEvent.u16NwkAddr);
            break;

        case ZPS_EVENT_NWK_ROUTE_DISCOVERY_CONFIRM:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Discovery Confirm\r\n");
            break;

        case ZPS_EVENT_NWK_ED_SCAN:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Energy Detect Scan %02x\r\n",
                    psAfEvent->uEvent.sNwkEdScanConfirmEvent.u8Status);
            break;
        case ZPS_EVENT_ZDO_BIND:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Zdo Bind event\r\n");
            break;

        case ZPS_EVENT_ZDO_UNBIND:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Zdo Unbiind Event\r\n");
            break;

        case ZPS_EVENT_ZDO_LINK_KEY:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Zdo Link Key Event Type %d Addr %016llx\r\n",
                        psAfEvent->uEvent.sZdoLinkKeyEvent.u8KeyType,
                        psAfEvent->uEvent.sZdoLinkKeyEvent.u64IeeeLinkAddr);
            break;

        case ZPS_EVENT_BIND_REQUEST_SERVER:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Bind Request Server Event\r\n");
            break;

        case ZPS_EVENT_ERROR:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: AF Error Event %d\r\n", psAfEvent->uEvent.sAfErrorEvent.eError);
            break;

        case ZPS_EVENT_TC_STATUS:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Trust Center Status %02x\r\n", psAfEvent->uEvent.sApsTcEvent.u8Status);
            break;

        default:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Unhandled Event %d\r\n", psAfEvent->eType);
            break;
    }
}



/****************************************************************************
 *
 * NAME: vAppSendOnOff
 *
 * DESCRIPTION:
 * Sends an On Of Togle Command to the bound devices
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vAppSendOnOff(void)
{
    tsZCL_Address   sDestinationAddress;
    uint8 u8seqNo;
    teZCL_Status eStatus;

    sDestinationAddress.eAddressMode = E_ZCL_AM_BOUND_NON_BLOCKING;

    eStatus = eCLD_OnOffCommandSend( COORDINATOR_APPLICATION_ENDPOINT,      // Src Endpoint
                             0,                                             // Dest Endpoint (bound so do not care)
                             &sDestinationAddress,
                             &u8seqNo,
                             E_CLD_ONOFF_CMD_TOGGLE);

    if (eStatus != E_ZCL_SUCCESS)
    {
    	DBG_vPrintf(TRACE_APP, "Send Toggle Failed x%02x Last error %02x\r\n",
    	        		eStatus, eZCL_GetLastZpsError());
    }

}

/****************************************************************************
 *
 * NAME: vAppSendIdentifyStop
 *
 * DESCRIPTION:
 * Sends an Identify stop command to the target address
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vAppSendIdentifyStop( uint16 u16Address, uint8 u8Endpoint)
{
    uint8 u8Seq;
    tsZCL_Address sAddress;
    tsCLD_Identify_IdentifyRequestPayload sPayload;

    sPayload.u16IdentifyTime = 0;
    sAddress.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
    sAddress.uAddress.u16DestinationAddress = u16Address;
    eCLD_IdentifyCommandIdentifyRequestSend(
                            COORDINATOR_APPLICATION_ENDPOINT,
                            u8Endpoint,
                            &sAddress,
                            &u8Seq,
                            &sPayload);
}

/****************************************************************************
 *
 * NAME: vAppSendRemoteBindRequest
 *
 * DESCRIPTION:
 * Sends a bind request to a remote node for it to create a binding with this node
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vAppSendRemoteBindRequest(uint16 u16DstAddr, uint16 u16ClusterId, uint8 u8DstEp)
{
    PDUM_thAPduInstance hAPduInst;
    ZPS_tuAddress uDstAddr;
    ZPS_tsAplZdpBindUnbindReq sAplZdpBindUnbindReq;
    ZPS_teStatus eStatus;

    uDstAddr.u16Addr = u16DstAddr;

    hAPduInst = PDUM_hAPduAllocateAPduInstance(apduZDP);
    if (PDUM_INVALID_HANDLE != hAPduInst)
    {
        uint8 u8SeqNumber;
        sAplZdpBindUnbindReq.u64SrcAddress = ZPS_u64AplZdoLookupIeeeAddr(u16DstAddr);
        sAplZdpBindUnbindReq.u8SrcEndpoint = COORDINATOR_APPLICATION_ENDPOINT;
        sAplZdpBindUnbindReq.u16ClusterId = u16ClusterId;


        sAplZdpBindUnbindReq.u8DstAddrMode = E_ZCL_AM_IEEE;
        sAplZdpBindUnbindReq.uAddressField.sExtended.u64DstAddress = ZPS_u64NwkNibGetExtAddr(ZPS_pvAplZdoGetNwkHandle() );
        sAplZdpBindUnbindReq.uAddressField.sExtended.u8DstEndPoint = u8DstEp;

        DBG_vPrintf(TRACE_APP, "Remote Bind Dst addr %04x, Ieee Dst Addr %016llx Ieee Src %016llx\r\n",
                uDstAddr.u16Addr,
                sAplZdpBindUnbindReq.uAddressField.sExtended.u64DstAddress,
                sAplZdpBindUnbindReq.u64SrcAddress);

        eStatus = ZPS_eAplZdpBindUnbindRequest(hAPduInst,
                                               uDstAddr,
                                               FALSE,
                                               &u8SeqNumber,
                                               TRUE,
                                               &sAplZdpBindUnbindReq);
        DBG_vPrintf(TRACE_APP, "Sending a remote bind request Status =%x\r\n", eStatus);
    }
}

/****************************************************************************
 *
 * NAME: APP_vFactoryResetRecords
 *
 * DESCRIPTION:
 * Resets persisted data structures to factory new state
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vFactoryResetRecords(void)
{
    /* clear out the stack */
    DBG_vPrintf(TRACE_APP, "Call clear stack\r\n");
    ZPS_vDefaultStack();
    (void)ZPS_eAplAibSetApsUseExtendedPanId(ZPS_APS_AIB_INIT_USE_EXTENDED_PANID);
    ZPS_vSaveAllZpsRecords();
    DBG_vPrintf(TRACE_APP, "Reset BDB Security\r\n");
    ZPS_vSetKeys();

    /* save everything */
    eNodeState = E_STARTUP;
    DBG_vPrintf(TRACE_APP, "Save App Record\r\n");
    PDM_eSaveRecordData(PDM_ID_APP_COORD,&eNodeState,sizeof(teNodeState));

#ifdef CPU_MKW41Z512VHT4
    NvCompletePendingOperations();
#endif
}

/****************************************************************************
 *
 * NAME: APP_vBdbInit
 *
 * DESCRIPTION:
 * Function to initialize BDB attributes and message queue
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_vBdbInit(void)
{
    BDB_tsInitArgs sInitArgs;

    sBDB.sAttrib.bbdbNodeIsOnANetwork = ((eNodeState >= E_RUNNING)?(TRUE):(FALSE));
    sInitArgs.hBdbEventsMsgQ = &APP_msgBdbEvents;

    BDB_vInit(&sInitArgs);
    sBDB.sAttrib.u32bdbPrimaryChannelSet = BDB_PRIMARY_CHANNEL_SET;
    sBDB.sAttrib.u32bdbSecondaryChannelSet = BDB_SECONDARY_CHANNEL_SET;
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
