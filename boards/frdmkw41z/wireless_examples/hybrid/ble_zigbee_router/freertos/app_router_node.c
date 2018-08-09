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
\file       app_router_node.c
\brief      Base Device Demo: Router application
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <string.h>
#include "dbg.h"
#include "pdum_apl.h"
#include "pdum_nwk.h"
#include "app_pdum_cfg.h"
#include "pwrm.h"
#include "PDM.h"
#include "app_zcl_cfg.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_apl_zdp.h"
#include "zps_apl_aib.h"
#include "zps_apl_aps.h"
#include "app_common.h"
#include "app_main.h"
#include "app_cfg.h"
#include "app_leds.h"
#include "app_buttons.h"
#include "ZTimer.h"
#include "app_events.h"
#include "app_zcl_task.h"
#include "app_router_node.h"
#include "zps_nwk_nib.h"
#include "PDM_IDs.h"
#include "zcl_options.h"
#include "zcl.h"
#include "app_reporting.h"
#ifdef APP_NTAG_ICODE
#include "app_ntag_icode.h"
#include "nfc_nwk.h"
#endif
#ifdef APP_NTAG_AES
#include "app_ntag_aes.h"
#include "nfc_nwk.h"
#endif
#include "app_zps_cfg.h"


#include "NVM_Interface.h"
#include "RNG_Interface.h"
#include "FunctionLib.h"
#include "app_zb_utils.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/


#ifndef DEBUG_APP
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


#define ASSOCIATION_ATTEMPTS   (5)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent);
PRIVATE void vAppHandleZdoEvents( BDB_tsZpsAfEvent *psZpsAfEvent);
PRIVATE void APP_vBdbInit(void);
PRIVATE void vPrintAPSTable(void);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE teNodeState eNodeState;

/* Register to pdm/nvm eNodeState */
NVM_RegisterDataSet(&eNodeState, 1, sizeof(eNodeState), PDM_ID_APP_ROUTER, gNVM_MirroredInRam_c);

uint32 u32OldFrameCtr;
uint8 u8AssociatuionAttempts;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#ifdef PDM_EEPROM
    extern uint8 u8PDM_CalculateFileSystemCapacity();
    extern uint8 u8PDM_GetFileSystemOccupancy();
#endif

extern void zps_vNwkSecClearMatSet(void *psNwk);


/****************************************************************************
 *
 * NAME: APP_vInitialiseRouter
 *
 * DESCRIPTION:
 * Initialises the application related functions
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitialiseRouter(void)
{
    uint16 u16ByteRead;
    PDM_teStatus eStatusReportReload;

    /* Stay awake */
    PWRM_eStartActivity();
#ifndef CPU_MKW41Z512VHT4
    APP_vLedInitialise();

    APP_bButtonInitialise();
#endif
    eNodeState = E_STARTUP;
    PDM_eReadDataFromRecord(PDM_ID_APP_ROUTER,
                            &eNodeState,
                            sizeof(teNodeState),
                            &u16ByteRead);

   /* Restore any report data that is previously saved to flash */
       eStatusReportReload = eRestoreReports();

    ZPS_eMiniMacSetTxBuffers (4);
#ifdef JN517x
    /* Default module configuration: change E_MODULE_DEFAULT as appropriate */
      vAHI_ModuleConfigure(E_MODULE_DEFAULT);
#endif

    ZPS_psAplAibGetAib()->bUseInstallCode = BDB_JOIN_USES_INSTALL_CODE_KEY;
    #ifdef CPU_MKW41Z512VHT4
    APP_vSetMacAddr();
    #endif
    /* Initialise ZBPro stack */
    ZPS_eAplAfInit();
    /* Initialise ZCL */
    APP_ZCL_vInitialise();

    /* Initialise other software modules
     * HERE
     */
    APP_vBdbInit();
#ifndef CPU_MKW41Z512VHT4
    /* Delete PDM if required */
    vDeletePDMOnButtonPress(APP_BOARD_SW0_PIN);
#endif
    /* Always initialise any peripherals used by the application
     * HERE
     */
    u8AssociatuionAttempts = 0;
    //u8AssociatuionAttempts = ASSOCIATION_ATTEMPTS;

    /* The functions u8PDM_CalculateFileSystemCapacity and u8PDM_GetFileSystemOccupancy
     * may be called at any time to monitor space available in  the eeprom  */
  // TODO don't know if they exist DBG_vPrintf(TRACE_APP, "PDM: Capacity %d\r\n", u8PDM_CalculateFileSystemCapacity() );
  // TODO don't know if they exist  DBG_vPrintf(TRACE_APP, "PDM: Occupancy %d\r\n", u8PDM_GetFileSystemOccupancy() );

    DBG_vPrintf(TRACE_APP, "Start Up StaTe %d On Network %d\r\n",
            eNodeState,
            sBDB.sAttrib.bbdbNodeIsOnANetwork);

    /*Load the reports from the PDM or the default ones depending on the PDM load record status*/
    if(eStatusReportReload !=PDM_E_STATUS_OK )
    {
        /*Load Defaults if the data was not correct*/
        vLoadDefaultConfigForReportable();
    }
    /*Make the reportable attributes */
    vMakeSupportedAttributesReportable();

    vPrintAPSTable();


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
    BDB_teStatus eStatus;

    switch(psBdbEvent->eEventType)
    {
        case BDB_EVENT_NONE:
            break;

        case BDB_EVENT_ZPSAF:                // Use with BDB_tsZpsAfEvent
            vAppHandleAfEvent(&psBdbEvent->uEventData.sZpsAfEvent);
            break;

        case BDB_EVENT_INIT_SUCCESS:

            DBG_vPrintf(TRACE_APP,"APP: BDB_EVENT_INIT_SUCCESS\r\n");
            if (eNodeState == E_STARTUP)
            {
  //              eStatus = BDB_eNsStartNwkSteering();
  //              DBG_vPrintf(TRACE_APP, "BDB Try Steering status %d\r\n",eStatus);
            }
            else
            {
                DBG_vPrintf(TRACE_APP, "BDB Init go Running");
                eNodeState = E_RUNNING;
                PDM_eSaveRecordData(PDM_ID_APP_ROUTER,&eNodeState,sizeof(teNodeState));
            }

            break;


        case BDB_EVENT_NWK_FORMATION_SUCCESS:
            DBG_vPrintf(TRACE_APP,"APP: NwkFormation Success \r\n");
            eNodeState = E_RUNNING;
            PDM_eSaveRecordData(PDM_ID_APP_ROUTER,
                                &eNodeState,
                                sizeof(eNodeState));
            break;

        case BDB_EVENT_NO_NETWORK:     
            DBG_vPrintf(TRACE_APP,"APP: BDB No Networks\r\n");
            if (eNodeState == E_STARTUP)
            {
            	if (u8AssociatuionAttempts > 0)
            	{
            		u8AssociatuionAttempts--;
            		eStatus = BDB_eNsStartNwkSteering();
            		DBG_vPrintf(TRACE_APP, "BDB rery Steering status %d\r\n",eStatus);
            	}
            }         
            break;

        case BDB_EVENT_NWK_STEERING_SUCCESS:
            DBG_vPrintf(TRACE_APP,"APP: NwkSteering Success \r\n");
            eNodeState = E_RUNNING;
            PDM_eSaveRecordData(PDM_ID_APP_ROUTER, &eNodeState, sizeof(teNodeState) );
            break;

        default:
            break;
    }
}

/****************************************************************************
 *
 * NAME: APP_taskRouter
 *
 * DESCRIPTION:
 * Task that handles application related functions
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_taskRouter(void)
{
    BDB_teStatus eStatus;
    APP_tsEvent sAppEvent;
    sAppEvent.eType = APP_E_EVENT_NONE;

    if (ZQ_bQueueReceive(&APP_msgAppEvents, &sAppEvent) == TRUE)
    {
        DBG_vPrintf(TRACE_APP, "ZPR: App event %d, NodeState=%d\r\n", sAppEvent.eType, eNodeState);

        if(sAppEvent.eType == APP_E_EVENT_BUTTON_DOWN)
        {
            switch(sAppEvent.uEvent.sButton.u8Button)
            {
                case APP_E_BUTTONS_BUTTON_SW1:
                    /* Create Network */
                    if (FALSE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                    {
                        /* Attempt formation of a Distributed Security network */
                        DBG_vPrintf(TRACE_APP_EVENT, "APP_EVENT: Try Network Formation\r\n");
                        eStatus = BDB_eNfStartNwkFormation();
                        if (eStatus != 0 && eStatus != 7)
                        {
                            DBG_vPrintf(TRACE_APP_EVENT, "APP_EVENT: Formation Failed %02x\r\n", eStatus);
                        }
                        else
                        {
                            DBG_vPrintf(TRACE_APP_EVENT, "APP-ZDO: Network started Channel = %d\r\n", ZPS_u8AplZdoGetRadioChannel());
                        }
                    }
                    break;

                case APP_E_BUTTONS_BUTTON_SW2:
                    /* Permit Join */
                    if (TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                    {
                        eStatus = BDB_eNsStartNwkSteering();
                        DBG_vPrintf(TRACE_APP_EVENT, "APP-EVT: Request Nwk Steering %02x\r\n", eStatus);
                    }
                    break;

                case APP_E_BUTTONS_BUTTON_SW3: 
                    /* Join Network */
                    if (FALSE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                    {
                        eStatus = BDB_eNsStartNwkSteering();
                        DBG_vPrintf(TRACE_APP, "BDB Network Steering status %d\r\n",eStatus);
                    }
                    break;

                case APP_E_BUTTONS_BUTTON_SW4:
                    /* Find & Bind */
#ifdef USE_GROUPS
                    sBDB.sAttrib.u16bdbCommissioningGroupID = GROUP_ID;
#endif
                    eStatus = BDB_eFbTriggerAsTarget(ROUTER_APPLICATION_ENDPOINT);
                    if (eStatus != 0 && eStatus != 9)
                    {
                        DBG_vPrintf(TRACE_APP_EVENT, "APP_EVENT: Fiind and Bind Failed %02x\r\n", eStatus);

                    }
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
    }
    
    APP_vSetLedState(TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork);
}

/****************************************************************************
 *
 * NAME: APP_vOobcSetRunning
 *
 * DESCRIPTION:
 * Set running state for OOBC
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vOobcSetRunning(void)
{
    eNodeState = E_RUNNING;
    PDM_eSaveRecordData(PDM_ID_APP_ROUTER,&eNodeState,sizeof(teNodeState));
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
    if (psZpsAfEvent->u8EndPoint == ROUTER_APPLICATION_ENDPOINT)
    {
        DBG_vPrintf(TRACE_APP, "Pass to ZCL\r\n");
        if ((psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_DATA_INDICATION) ||
            (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_INTERPAN_DATA_INDICATION))
        {
            APP_ZCL_vEventHandler( &psZpsAfEvent->sStackEvent);
         }
    }
    else if (psZpsAfEvent->u8EndPoint == 0)
    {
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
    ZPS_tsAfEvent *psAfEvent = &(psZpsAfEvent->sStackEvent);

    switch(psAfEvent->eType)
    {
        case ZPS_EVENT_APS_DATA_INDICATION:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Data Indication Status %02x from %04x Src Ep Dst %d Ep %d Profile %04x Cluster %04x\r\n",
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
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Network started\r\n");
            break;

        case ZPS_EVENT_NWK_JOINED_AS_ROUTER:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Joined Network Addr %04x Rejoin %d\r\n",
                    psAfEvent->uEvent.sNwkJoinedEvent.u16Addr,
                    psAfEvent->uEvent.sNwkJoinedEvent.bRejoin);
            #ifdef APP_NTAG_ICODE
            {
                /* Not a rejoin ? */
                if (FALSE == psAfEvent->uEvent.sNwkJoinedEvent.bRejoin)
                {
                    /* Write network data to tag */
                    APP_vNtagStart(ROUTER_APPLICATION_ENDPOINT);
                }
            }
            #endif
            break;
        case ZPS_EVENT_NWK_FAILED_TO_START:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Network Failed To start\r\n");
            break;

        case ZPS_EVENT_NWK_FAILED_TO_JOIN:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Failed To Join %02x Rejoin %d\r\n",
                    psAfEvent->uEvent.sNwkJoinFailedEvent.u8Status,
                    psAfEvent->uEvent.sNwkJoinFailedEvent.bRejoin);
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
            if ( (psAfEvent->uEvent.sNwkLeaveIndicationEvent.u64ExtAddr == 0UL) &&
                 (psAfEvent->uEvent.sNwkLeaveIndicationEvent.u8Rejoin == 0) )
            {
                /* We sare asked to Leave without rejoin */
                DBG_vPrintf(TRACE_APP, "LEAVE IND -> For Us No Rejoin\r\n");
                APP_vFactoryResetRecords();
                NVIC_SystemReset();
            }
            break;

        case ZPS_EVENT_NWK_LEAVE_CONFIRM:
            DBG_vPrintf(TRACE_APP, "APP-ZDO: Leave Confirm status %02x Addr %016llx\r\n",
                    psAfEvent->uEvent.sNwkLeaveConfirmEvent.eStatus,
                    psAfEvent->uEvent.sNwkLeaveConfirmEvent.u64ExtAddr);          
            if ((psAfEvent->uEvent.sNwkLeaveConfirmEvent.eStatus == ZPS_E_SUCCESS) &&
                (psAfEvent->uEvent.sNwkLeaveConfirmEvent.u64ExtAddr == 0UL) &&
                (psAfEvent->uEvent.sNwkLeaveConfirmEvent.bRejoin == FALSE))
            {
                DBG_vPrintf(TRACE_APP, "Leave -> Reset Data Structures\r\n");
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


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


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
}
#ifndef CPU_MKW41Z512VHT4
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
PRIVATE void vDeletePDMOnButtonPress(uint8 u8ButtonDIO)
{
    bool_t bDeleteRecords = FALSE;
    uint8 u8Status;

    uint32 u32Buttons = APP_u32GetSwitchIOState() & (1 << u8ButtonDIO);

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
     * Alternatively, always call PDM_vDeleteAllDataRecords() if context saving is not required.
     */
    if(bDeleteRecords)
    {
    	/* wait for button release */
    	while (u32Buttons == 0)
		{
			u32Buttons = APP_u32GetSwitchIOState() & (1 << u8ButtonDIO);
		}
    	u8Status = ZPS_eAplZdoLeaveNetwork(0, FALSE,FALSE);
    	if (ZPS_E_SUCCESS !=  u8Status )
    	{
            /* Leave failed,so just reset everything */
            DBG_vPrintf(TRACE_APP,"Leave failed status %x Deleting the PDM\r\n", u8Status);
            APP_vFactoryResetRecords();
            NVIC_SystemReset();
        } else { DBG_vPrintf(TRACE_APP, "RESET: Sent Leave\r\n"); }
    }
}
#endif
/****************************************************************************
 *
 * NAME: vPrintAPSTable
 *
 * DESCRIPTION:
 * Prints the content of APS table
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vPrintAPSTable(void)
{
    uint8 i;
    uint8 j;

    ZPS_tsAplAib * psAplAib;
    uint64 u64Addr;

    psAplAib = ZPS_psAplAibGetAib();

    for ( i = 0 ; i < (psAplAib->psAplDeviceKeyPairTable->u16SizeOfKeyDescriptorTable + 3) ; i++ )
    {
        u64Addr = ZPS_u64NwkNibGetMappedIeeeAddr(ZPS_pvAplZdoGetNwkHandle(), psAplAib->psAplDeviceKeyPairTable->psAplApsKeyDescriptorEntry[i].u16ExtAddrLkup);
    	DBG_vPrintf(TRUE, "%d MAC: 0x%x %x Key: ", i, (uint32)(u64Addr>>32), (uint32)(u64Addr&0xffffffff) );
        for(j=0; j<16;j++)
        {
            DBG_vPrintf(TRUE, "%02x ", psAplAib->psAplDeviceKeyPairTable->psAplApsKeyDescriptorEntry[i].au8LinkKey[j]);
        }
        DBG_vPrintf(TRUE, "\r\n");
    }
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
