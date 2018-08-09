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
\file       app_zlo_light_node.c
\brief      ZLO Demo: Light Node NWK Startup - Implementation
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>
#include "NVM_Interface.h"
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
#include "app_buttons.h"
#include "ZTimer.h"
#include "app_events.h"

#include "app_zcl_light_task.h"
#include "app_zlo_light_node.h"
//#include "zll.h"
#include "zps_nwk_nib.h"
#include "app_power_on_counter.h"

#include "PDM_IDs.h"
#include "zcl_options.h"
#include "app_scenes.h"
#include "app_reporting.h"

#ifdef JN517x
#include "AHI_ModuleConfiguration.h"
#endif

#ifdef CLD_OTA
    #include "OTA.h"
    #include "app_ota_client.h"
#endif

#ifdef CPU_MKW41Z512VHT4
#include "NVM_Interface.h"
#include "app_zb_utils.h"
#include "RNG_Interface.h"
#include "FunctionLib.h"
#else
#include <rnd_pub.h>
#endif


#ifdef USE_PCA9634
#include "SMBus.h"
#include "PCA9634.h"
#endif

#include "DriverBulb.h"

#include "zcl.h"
#include "bdb_api.h"
#include "bdb_fb_api.h"
#include "bdb_tl.h"
#include "app_manage_temperature.h"
#ifdef CLD_GREENPOWER
#include "app_green_power.h"
#endif

#ifdef APP_NTAG
#include "ntag_nwk.h"
#include "app_ntag.h"
#endif
#include "app_zps_cfg.h"

#ifndef DEBUG_APP
#define TRACE_APP   FALSE
#else
#define TRACE_APP   TRUE
#endif

#ifndef DEBUG_LIGHT_NODE
#define TRACE_LIGHT_NODE   FALSE
#else
#define TRACE_LIGHT_NODE   TRUE
#endif


#define TRACE_RESET   FALSE


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define DBG_EVENT  FALSE

#define GREEN_POWER_ENDPOINT                   242


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/* ZDO End point for all devices is always 0 */
#define LIGHT_ZDO_ENDPOINT    (0)
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void vAppHandleAfEvent( BDB_tsZpsAfEvent *psZpsAfEvent);
PRIVATE void vAppHandleZdoEvents( BDB_tsZpsAfEvent *psZpsAfEvent);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsZllState sZllState = { FACTORY_NEW, E_STARTUP, 0 };

NVM_RegisterDataSet(&sZllState, 1, sizeof(tsZllState), PDM_ID_APP_TL_ROUTER, gNVM_MirroredInRam_c);
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
#ifdef PDM_EEPROM
    extern uint8 u8PDM_CalculateFileSystemCapacity();
    extern uint8 u8PDM_GetFileSystemOccupancy();
#endif




#ifdef CLD_OTA
PUBLIC teNODE_STATES eGetNodeState(void)
{
    return sZllState.eNodeState;
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
#if (defined OM15081)
    bool_t bDeleteRecords = FALSE;
#endif


    uint16 u16ByteRead;
    PDM_teStatus eStatusReportReload;

    DBG_vPrintf(TRACE_APP, "APP_vInitialiseNode\r\n");

    vManagePowerOnCountLoadRecord();

    /* Stay awake */
    PWRM_eStartActivity();

    sZllState.eNodeState = E_STARTUP;



    #if (defined OM15081)
        bDeleteRecords = APP_bButtonInitialise();
    #endif

    PDM_eReadDataFromRecord(PDM_ID_APP_TL_ROUTER,
            &sZllState,
            sizeof(tsZllState), &u16ByteRead);


#ifdef CLD_OTA
    vLoadOTAPersistedData();
#endif
#ifdef CLD_GREENPOWER
    vAPP_GP_LoadPDMData();
#endif
    vLoadScenesNVM();
    /* Restore any report data that is previously saved to flash */
    eStatusReportReload = eRestoreReports();

#ifdef JN517x  // todo
    vAHI_ModuleConfigure(E_MODULE_DEFAULT);
#endif


    DBG_vPrintf(TRACE_LIGHT_NODE, "\r\nZll recovered state %02x\r\n", sZllState.eState);

    ZPS_eMiniMacSetTxBuffers (4);

#ifdef CPU_MKW41Z512VHT4
    APP_vSetMacAddr();
#endif  

    /* Initialise ZBPro stack */
    ZPS_eAplAfInit();

#if (defined OM15081)
    if (bDeleteRecords) {
        PDM_vDeleteDataRecord(PDM_ID_APP_SCENES_DATA);
        while (APP_bButtonInitialise());
    }
#endif



    APP_ZCL_vInitialise();

#if (defined OM15081)
    /* If required, at this point delete the network context from flash, perhaps upon some condition
     * For example, check if a button is being held down at reset, and if so request the Persistent
     * Data Manager to delete all its records:
     * e.g. bDeleteRecords = vCheckButtons();
     * Alternatively, always call PDM_vDelete() if context saving is not required.
     */
    if (bDeleteRecords) {

        if (sZllState.eNodeState >= E_RUNNING)
        {
            APP_tsEvent sPowerOnEvent;
            sPowerOnEvent.eType = APP_E_EVENT_LEAVE_AND_RESET;
            ZQ_bQueueSend( &APP_msgAppEvents, &sPowerOnEvent);
        }
        else
        {
            APP_vFactoryResetRecords();
            NVIC_SystemReset();
        }
    }
#endif

    /*Load the reports from the PDM or the default ones depending on the PDM load record status*/
    if(eStatusReportReload !=PDM_E_STATUS_OK )
    {
        /*Load Defaults if the data was not correct*/
        vLoadDefaultConfigForReportable();
    }
    /*Make the reportable attributes */
    vMakeSupportedAttributesReportable();

    if (sZllState.eState == 0) {
        DBG_vPrintf(TRACE_LIGHT_NODE|TRUE, "\r\nSet a random pan\r\n");
        ZPS_vNwkNibSetPanId(ZPS_pvAplZdoGetNwkHandle(), (uint16) RND_u32GetRand(1, 0xfff0) );
    }

    if ((sZllState.eNodeState >= E_RUNNING))
    {
        sZllState.eNodeState = E_NFN_START;
        DBG_vPrintf(TRACE_LIGHT_NODE, "\r\nNOT Factory New Start\r\n");
        sBDB.sAttrib.bbdbNodeIsOnANetwork = TRUE;
        sBDB.sAttrib.bTLStealNotAllowed = TRUE;
    } else {
        DBG_vPrintf(TRACE_LIGHT_NODE, "\r\n--------Factory new start up\r\n");
        sZllState.eNodeState = E_STARTUP;
        sBDB.sAttrib.bbdbNodeIsOnANetwork = FALSE;
        sBDB.sAttrib.bTLStealNotAllowed = FALSE;

    }

    BDB_tsInitArgs sArgs;

    sArgs.hBdbEventsMsgQ = &APP_msgBdbEvents;

    BDB_vInit(&sArgs);
    sBDB.sAttrib.u32bdbPrimaryChannelSet = BDB_PRIMARY_CHANNEL_SET;
    sBDB.sAttrib.u32bdbSecondaryChannelSet = BDB_SECONDARY_CHANNEL_SET;

   /* Start up temperature management behaviour */
    APP_vManageTemperatureInit();
    ZTIMER_eStart(u8TimerRadioRecal, ZTIMER_TIME_SEC(10000));

#ifndef CLD_COLOUR_CONTROL
    /* Second call to bulb initialisation.  This is required by the synchronus bulb      */
    /* Driver to enable the anti rebroadcast-flicker mechanism. Ignored by other drivers */
    DriverBulb_vInit();
#endif

#if (defined PDM_EEPROM)
#ifdef TRACE_APP
    /*
     * The functions u8PDM_CalculateFileSystemCapacity and u8PDM_GetFileSystemOccupancy
     * may be called at any time to monitor space available in  the eeprom
     */
    DBG_vPrintf(TRACE_APP, "PDM: Capacity %d\r\n", u8PDM_CalculateFileSystemCapacity() );
    DBG_vPrintf(TRACE_APP, "PDM: Occupancy %d\r\n", u8PDM_GetFileSystemOccupancy() );
#endif
#endif

    vManagePowerOnCountInit();
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

    switch(psBdbEvent->eEventType)
    {
        case BDB_EVENT_NONE:
            break;
        case BDB_EVENT_ZPSAF:                // Use with BDB_tsZpsAfEvent
            DBG_vPrintf(0,"BdbCb event [%d %d] \r\n",psBdbEvent->uEventData.sZpsAfEvent.u8EndPoint,
                    psBdbEvent->uEventData.sZpsAfEvent.sStackEvent.eType);

            vAppHandleAfEvent(&psBdbEvent->uEventData.sZpsAfEvent);
            break;

        case BDB_EVENT_INIT_SUCCESS:
            if (sZllState.eNodeState == E_STARTUP)
            {
                //BDB_eNsStartNwkSteering();
            }
            else
            {
                DBG_vPrintf(TRACE_APP, "BDB Init go Running");
                sZllState.eState = NOT_FACTORY_NEW;
                sZllState.eNodeState = E_RUNNING;
                PDM_eSaveRecordData(PDM_ID_APP_TL_ROUTER,&sZllState,sizeof(tsZllState));
            }
            break;


        case BDB_EVENT_NWK_STEERING_SUCCESS:
            // go to running state
            DBG_vPrintf(TRACE_APP,"APP: BDB_EVENT_NWK_STEERING_SUCCESS - go running\r\n");
            sZllState.eState = NOT_FACTORY_NEW;
            sZllState.eNodeState = E_RUNNING;
            PDM_eSaveRecordData(PDM_ID_APP_TL_ROUTER,&sZllState,sizeof(tsZllState));
            break;

        case BDB_EVENT_NO_NETWORK:
            DBG_vPrintf(TRACE_APP,"APP: BDB No Networks\r\n");

                uint32 u32Channel;
                eAppApiPlmeGet(PHY_PIB_ATTR_CURRENT_CHANNEL, &u32Channel);
                ZPS_vNwkNibSetChannel( ZPS_pvAplZdoGetNwkHandle(), (uint8)u32Channel );
                DBG_vPrintf(TRACE_APP,"APP: BDB No Networks -> Wait On %d\r\n", u32Channel);
            break;

        case BDB_EVENT_NWK_FORMATION_SUCCESS:
            DBG_vPrintf(TRACE_APP,"APP: BDB_EVENT_NWK_FORMATION_SUCCESS - go running --- \r\n");
            break;

        case BDB_EVENT_NWK_FORMATION_FAILURE:
            DBG_vPrintf(TRACE_APP,"APP: BDB_EVENT_NWK_FORMATION_FAILURE\r\n");
            break;

        case BDB_EVENT_FB_OVER_AT_TARGET:
            DBG_vPrintf(TRACE_APP, "Find and Bind as Target completed\r\n");
#if 0
            if ( !BDB_bTlTouchLinkInProgress() )
            {
                sBDB.sAttrib.bTLStealNotAllowed = TRUE;
            }
#endif
            break;

        default:
            break;
    }
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
    //DBG_vPrintf(TRACE_LIGHT_NODE, "\r\nvAppHandleAfEvent  %d\r\n", psZpsAfEvent->sStackEvent.eType);
    if (psZpsAfEvent->u8EndPoint == app_u8GetDeviceEndpoint())
    {
        if ((psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_DATA_INDICATION) ||
            (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_INTERPAN_DATA_INDICATION) ||
            (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_ZGP_DATA_INDICATION))
        {
            APP_ZCL_vEventHandler( &psZpsAfEvent->sStackEvent);
         }

    }
#ifdef CLD_GREENPOWER
    else if ((psZpsAfEvent->u8EndPoint == GREEN_POWER_ENDPOINT) ||  (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_ZGP_DATA_CONFIRM))
    {
        if ((psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_DATA_INDICATION) ||
            (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_ZGP_DATA_INDICATION)||
            (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_ZGP_DATA_CONFIRM))
        {

            APP_ZCL_vEventHandler( &psZpsAfEvent->sStackEvent);
        }
    }
#endif
    else if (psZpsAfEvent->u8EndPoint == LIGHT_ZDO_ENDPOINT)
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
    else if ( psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_APS_ZGP_DATA_INDICATION )
    {
        PDUM_eAPduFreeAPduInstance(psZpsAfEvent->sStackEvent.uEvent.sApsZgpDataIndEvent.hAPduInst);
    }

}

/****************************************************************************
 *
 * NAME: APP_taskLight
 *
 * DESCRIPTION:
 * Task that handles application related functions
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_taskLight(void)
{
    APP_tsLightEvent sAppEvent;
    sAppEvent.eType = APP_E_EVENT_NONE;

    if (ZQ_bQueueReceive(&APP_msgAppEvents, &sAppEvent) == TRUE)
    {
        DBG_vPrintf(TRACE_APP, "ZPR: App event %d, NodeState=%d\r\n", sAppEvent.eType, sZllState.eNodeState);
        switch (sAppEvent.eType)
        {
            case  APP_E_EVENT_BUTTON_DOWN:
            {   
                switch(sAppEvent.uEvent.sButton.u8Button)
                {
                    case APP_E_BUTTONS_BUTTON_1: 
                        if (ZPS_E_SUCCESS !=  ZPS_eAplZdoLeaveNetwork(0, FALSE,FALSE)) 
                        {
                            /* Leave failed,so just reset everything */
                            DBG_vPrintf(TRUE,"Deleting the PDM\n");
                            APP_vFactoryResetRecords();
                            ResetMCU();
                        }
                        break;
                    case APP_E_BUTTONS_BUTTON_SW2:
                        /* Permit Join */
                        if (TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                        {
                           BDB_eNsStartNwkSteering();
                        }                                
                        break;    
                    case APP_E_BUTTONS_BUTTON_SW3:
                        /* Join Network */
                        if (FALSE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                        {
                            BDB_eNsStartNwkSteering();
                            sBDB.sAttrib.bTLStealNotAllowed = FALSE;
                        }
                        break;
                    case APP_E_BUTTONS_BUTTON_SW4:  //FindAndBind
                        DBG_vPrintf(TRACE_APP, "APP: Switch 4\r\n");
                        if(TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork)
                        {
                            BDB_eFbTriggerAsTarget(app_u8GetDeviceEndpoint());
#ifdef CLD_GREENPOWER
                            vApp_GP_EnterCommissioningMode();
#endif
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
         case APP_E_EVENT_BUTTON_UP:
            switch(sAppEvent.uEvent.sButton.u8Button)
            {
#ifdef APP_NTAG
            case APP_E_BUTTONS_NFC_FD:
                DBG_vPrintf(TRACE_LIGHT_NODE, "APP-EVT: NFC_FD UP\r\n");
                APP_vNtagStart(NFC_NWK_LIGHT);
                break;
#endif
            }
            break;
#if 0            
        case APP_E_EVENT_BUTTON_DOWN:
            switch(sAppEvent.uEvent.sButton.u8Button)
            {
#if (defined OM15081)
            case APP_E_BUTTONS_BUTTON_1:
                if (sZllState.eNodeState == E_RUNNING)
                {
                    DBG_vPrintf(TRACE_LIGHT_NODE, "Button down %d\r\n", sAppEvent.uEvent.sButton.u8Button);
                    BDB_eNsStartNwkSteering();
                    BDB_eFbTriggerAsTarget(app_u8GetDeviceEndpoint());
                    sBDB.sAttrib.bTLStealNotAllowed = FALSE;

                    #ifdef CLD_GREENPOWER
                    vApp_GP_EnterCommissioningMode();
                    #endif
                }
                break;
#endif
#endif
#ifdef APP_NTAG
            case APP_E_BUTTONS_NFC_FD:
                DBG_vPrintf(TRACE_LIGHT_NODE, "APP-EVT:  NFC_FD DOWN\r\n");
                APP_vNtagStart(NFC_NWK_LIGHT);
                break;

            }
            break;
#endif
        case APP_EVENT_POR_FIND_BIND:
        if (sZllState.eNodeState == E_RUNNING)
        {
            //DBG_vPrintf(TRACE_LIGHT_NODE, "Button down %d\r\n", sAppEvent.uEvent.sButton.u8Button);
            //BDB_eNsStartNwkSteering();
            //BDB_eFbTriggerAsTarget(app_u8GetDeviceEndpoint());
            //sBDB.sAttrib.bTLStealNotAllowed = FALSE;

            #ifdef CLD_GREENPOWER
            vApp_GP_EnterCommissioningMode();
            #endif
        }
        break;
        case APP_E_EVENT_LEAVE_AND_RESET:
            if (sZllState.eNodeState == E_RUNNING)
            {
                DBG_vPrintf(TRUE, "GOT HERE LEAVE \r\n");
                if (ZPS_eAplZdoLeaveNetwork( 0UL, FALSE, FALSE) != ZPS_E_SUCCESS )
                {
                    DBG_vPrintf(TRUE, "GOT HERE LEAVE 1\r\n");
                    APP_vFactoryResetRecords();
                    DBG_vPrintf(TRUE, "GOT HERE LEAVE 2\r\n");
                    NVIC_SystemReset();
                    DBG_vPrintf(TRUE, "GOT HERE LEAVE 3\r\n");
                }
                else
                {
                    DBG_vPrintf(TRUE, "GOT HERE LEAVE 4\r\n");
                }
            }
            else
            {
                DBG_vPrintf(TRUE, "GOT HERE LEAVE 5 \r\n");
                APP_vFactoryResetRecords();
                DBG_vPrintf(TRUE, "GOT HERE LEAVE 6\r\n");
                NVIC_SystemReset();
            }
            break;

        case APP_EVENT_POR_CLEAR_BINDINGS:
            break;
#ifdef CLD_GREENPOWER
        case APP_EVENT_POR_RESET_GP_TABLES:
            vAPP_GP_ResetData();
            break;
#endif
        default:
            break;

        }
    }
#ifdef CLD_GREENPOWER
    else if (ZQ_bQueueReceive(&APP_msgGPZCLTimerEvents, &u8GPZCLTimerEvent) == TRUE)
    {
        tsZCL_CallBackEvent sZCL_CallBackEvent;
        sZCL_CallBackEvent.eEventType =E_ZCL_CBET_TIMER_MS;
        vZCL_EventHandler(&sZCL_CallBackEvent);
    }
#endif
    APP_vSetLedState(TRUE == sBDB.sAttrib.bbdbNodeIsOnANetwork);
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
    tsBDB_ZCLEvent sBdbEvent;
    tsZCL_CallBackEvent sCallBackEvent;
    sBdbEvent.psCallBackEvent = &sCallBackEvent;

    if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_NWK_NEW_NODE_HAS_JOINED)
    {
        DBG_vPrintf(TRACE_LIGHT_NODE, "\r\nNode joined %04x", psZpsAfEvent->sStackEvent.uEvent.sNwkJoinIndicationEvent.u16NwkAddr);
    }

    if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_NWK_STATUS_INDICATION)
    {
        DBG_vPrintf(TRACE_LIGHT_NODE, "Nwk Status %d from %04x\r\n", psZpsAfEvent->sStackEvent.uEvent.sNwkStatusIndicationEvent.u8Status,
                psZpsAfEvent->sStackEvent.uEvent.sNwkStatusIndicationEvent.u16NwkAddr);
    }
    else if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_NWK_DISCOVERY_COMPLETE)
    {
        sBdbEvent.eType = BDB_E_ZCL_EVENT_DISCOVERY_DONE;
        sBdbEvent.psCallBackEvent->pZPSevent = &psZpsAfEvent->sStackEvent;
        BDB_vZclEventHandler(&sBdbEvent);
    }
    else if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_NWK_LEAVE_INDICATION)
    {
        DBG_vPrintf(TRACE_RESET, "LEAVE IND Addr %016llx Rejoin %02x\r\n",
                psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveIndicationEvent.u64ExtAddr,
                psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveIndicationEvent.u8Rejoin);
        if ( (psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveIndicationEvent.u64ExtAddr == 0UL) &&
             (psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveIndicationEvent.u8Rejoin == 0) )
        {
            /* We sare asked to Leave without rejoin */
            DBG_vPrintf(TRACE_RESET, "LEAVE IND -> For Us No Rejoin\r\n");
            APP_vFactoryResetRecords();
            NVIC_SystemReset();
        }
    }
    else if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_NWK_LEAVE_CONFIRM)
    {
        DBG_vPrintf(TRACE_RESET, "LEAVE CFM Status %02x Addr %016llx\r\n",
                            psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveConfirmEvent.eStatus,
                            psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveConfirmEvent.u64ExtAddr);

        if ((psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveConfirmEvent.eStatus == ZPS_E_SUCCESS) &&
            (psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveConfirmEvent.u64ExtAddr == 0UL))
        {
            /* It was a self leave success */
            if (sBDB.sAttrib.bLeaveRequested)
            {
                /* originated in touch link, pass it over */
                sBDB.sAttrib.bLeaveRequested = FALSE;
                sBdbEvent.eType = BDB_E_ZCL_EVENT_LEAVE_CFM;
                DBG_vPrintf(TRACE_LIGHT_NODE, "Send Leave cfm\r\n");
                sBdbEvent.psCallBackEvent->pZPSevent = &psZpsAfEvent->sStackEvent;
                BDB_vZclEventHandler(&sBdbEvent);
            }
            else if(psZpsAfEvent->sStackEvent.uEvent.sNwkLeaveConfirmEvent.bRejoin == FALSE)
            {
                DBG_vPrintf(TRACE_RESET, "Leave -> Reset Data Structures\r\n");
                APP_vFactoryResetRecords();
                NVIC_SystemReset();
            }
        }
    }
    else if (psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_NWK_JOINED_AS_ROUTER)
    {
        sZllState.eNodeState = E_RUNNING;
        /* set the aps use pan id */
        ZPS_eAplAibSetApsUseExtendedPanId( ZPS_u64NwkNibGetEpid(ZPS_pvAplZdoGetNwkHandle()) );

        sZllState.eState = NOT_FACTORY_NEW;
        sZllState.u16MyAddr = psZpsAfEvent->sStackEvent.uEvent.sNwkJoinedEvent.u16Addr;

        PDM_eSaveRecordData(PDM_ID_APP_TL_ROUTER,&sZllState,sizeof(tsZllState));
        DBG_vPrintf(TRACE_LIGHT_NODE, "Joined as Router\r\n");
        /* identify to signal the join */
        APP_ZCL_vSetIdentifyTime( 10);
        sBDB.sAttrib.bTLStealNotAllowed = TRUE;
    }
    else if (ZPS_EVENT_APS_DATA_INDICATION == psZpsAfEvent->sStackEvent.eType)
    {
        DBG_vPrintf(TRACE_APP, "\r\nData Ind: Profile :%x Cluster :%x EP:%x\r\n",
                psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.u16ProfileId,
                psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.u16ClusterId,
                psZpsAfEvent->sStackEvent.uEvent.sApsDataIndEvent.u8DstEndpoint);
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
 
    DBG_vPrintf(TRACE_RESET, "RESET ALL APP DATA STRUCTS\r\n");
    APP_vRemoveAllGroupsAndScenes();

    sZllState.eNodeState = E_STARTUP;
    sZllState.eState = FACTORY_NEW;
    sBDB.sAttrib.bbdbNodeIsOnANetwork = FALSE;
    sBDB.sAttrib.bTLStealNotAllowed = FALSE;

    sZllState.u16MyAddr = 0;
#ifdef CLD_OTA
    sZllState.bValid = FALSE;
    sZllState.u64IeeeAddrOfServer = 0;
    sZllState.u16NwkAddrOfServer = 0xffff;
    sZllState.u8OTAserverEP = 0xff;
    vOTAResetPersist();
#endif

#ifdef CLD_GREENPOWER
     vAPP_GP_ResetData();
#endif

    /* clear out the stack */
    ZPS_vDefaultStack();
    (void)ZPS_eAplAibSetApsUseExtendedPanId(ZPS_APS_AIB_INIT_USE_EXTENDED_PANID);
    ZPS_vSetKeys();

    /* reset and save zcl persisted attributes */
    vResetZCLRecords();

    vLoadDefaultConfigForReportable();

    /* save everything */
    PDM_eSaveRecordData(PDM_ID_APP_TL_ROUTER,&sZllState,sizeof(tsZllState));
    ZPS_vSaveAllZpsRecords();
    
#ifdef CPU_MKW41Z512VHT4
    NvCompletePendingOperations();
#endif      
}

/****************************************************************************
 *
 * NAME: APP_vSetRunning
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
    sZllState.eState = NOT_FACTORY_NEW;
    sZllState.eNodeState = E_RUNNING;
    PDM_eSaveRecordData(PDM_ID_APP_TL_ROUTER,&sZllState,sizeof(tsZllState));
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
    return sLight.sCLD_OTA_CustomDataStruct.sOTACallBackMessage.sPersistedData;
}
#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
