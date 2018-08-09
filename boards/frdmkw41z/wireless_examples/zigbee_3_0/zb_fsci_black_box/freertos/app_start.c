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
\file       app_start.c
\brief      This is a public source file for the ZigbeeNodeControlBridge 
            demo application.
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "pwrm.h"
#include "pdum_apl.h"
#include "pdum_nwk.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "app_zcl_cfg.h"
#include "zps_apl_af.h"
#include "zps_nwk_pub.h"
#include "zps_mac.h"
#include "RNG_Interface.h"
#include <string.h>
#include "SerialLink.h"
#include "app_Znc_cmds.h"
#include "PDM_IDs.h"
#include "app_common.h"
#include "Log.h"
#include "app_events.h"
#include "zcl_common.h"

#ifdef CLD_OTA
#include "app_ota_server.h"
#endif

#include "app_cfg.h"

#include "LED.h"
#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "board.h"
#include "SerialManager.h"
#include "MemManager.h"
#include "NVM_Interface.h"
#include "SecLib.h"
#include "Keyboard.h"
#include "TimersManager.h"
#include "app_zb_utils.h"

#if gFsciIncluded_c
#include "FsciInterface.h"
#include "Messaging.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef TRACE_APPSTART
#define TRACE_APPSTART                                            FALSE
#endif

#ifndef TRACE_EXC
#define TRACE_EXC                                                 TRUE
#endif

#define TIMER_QUEUE_SIZE                                           8
#define MLME_QUEQUE_SIZE                                           8
#define MCPS_QUEUE_SIZE                                            30
#define ZPS_QUEUE_SIZE                                             2
#define APP_QUEUE_SIZE                                             8
#define TX_QUEUE_SIZE                                              150
#define RX_QUEUE_SIZE                                              150
#define BDB_QUEUE_SIZE                                             2
#define APP_ZTIMER_STORAGE                                         4

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PUBLIC void app_vFormatAndSendUpdateLists ( void );
PUBLIC void APP_vMainLoop ( void );
PUBLIC void APP_vInitResources ( void );
PRIVATE void APP_vSetUpWdog ( void );

void vfExtendedStatusCallBack ( ZPS_teExtendedStatus    eExtendedStatus );
PRIVATE void vInitialiseApp ( void );

PRIVATE void APP_cbTimerZclTick (void*    pvParam);
PRIVATE void KBD_Callback( uint8_t events);
#if gFsciIncluded_c
static void APP_ProcessFsciPkt(void* pData, void* param, uint32_t fsciInterface);
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

extern uint8 u8SerialId;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PUBLIC tsLedState         s_sLedState =  { LED1,  ZTIMER_TIME_MSEC(1),  FALSE };



tsZllState sZllState = {
        .eState             = FACTORY_NEW,
        .eNodeState         = E_STARTUP,
        .u8DeviceType       = 0,
        .u8MyChannel        = 11,
        .u16MyAddr          = TL_MIN_ADDR,
#ifdef FULL_FUNC_DEVICE
        .u16FreeAddrLow     = TL_MIN_ADDR,
        .u16FreeAddrHigh    = TL_MAX_ADDR,
        .u16FreeGroupLow    = TL_MIN_GROUP,
        .u16FreeGroupHigh   = TL_MAX_GROUP,
#endif
#ifdef CLD_OTA
        .bValid                 = FALSE,
        .u64IeeeAddrOfServer    = 0,
        .u16NwkAddrOfServer     = 0,
        .u8OTAserverEP          = 0
#endif
};

/* Register to pdm/nvm sZllState */
NVM_RegisterDataSet(&sZllState, 1, sizeof(tsZllState), PDM_ID_APP_ZLL_CMSSION, gNVM_MirroredInRam_c);

PUBLIC tszQueue           APP_msgBdbEvents;
PUBLIC tszQueue           APP_msgAppEvents;
ZTIMER_tsTimer            asTimers[APP_ZTIMER_STORAGE + BDB_ZTIMER_STORAGE];

uint8                     u8IdTimer;
uint8                     u8TmrToggleLED;
uint8                     u8HaModeTimer;
uint8                     u8AppPollTimer;
uint8                     u8TickTimer;
uint8                     u8JoinedDevice =  0;

ZPS_tsAfFlashInfoSet      sSet;

#if gFsciIncluded_c
gFsciSerialConfig_t mFsciConfig = 
{
  APP_SERIAL_INTERFACE_SPEED,
  APP_SERIAL_INTERFACE_TYPE,
  APP_SERIAL_INTERFACE_INSTANCE,
  0
};

msgQueue_t mFsciRxPkts;
#endif

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
#ifdef FULL_FUNC_DEVICE
extern PUBLIC void APP_cbTimerCommission ( void*    pvParam );
#endif

extern void vTimerServerInit(void);

extern void OSA_TimeInit(void);
extern const uint8_t gUseRtos_c;

static volatile uint8_t wdt_update_count;

gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput, 1,
};

/****************************************************************************
 *
 * NAME: vAppMain
 *
 * DESCRIPTION:
 * Entry point for application from a cold start.
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/

PUBLIC void vAppMain(void)
{
    uint8    u8FormJoin;
    uint8    au8LinkTxBuffer[50];

    /* Initialise debugging */
    /* Send debug output through SerialLink to host */
    vSL_LogInit ( );
    
  #if gFsciIncluded_c
    u8SerialId = 0;
    MSG_InitQueue(&mFsciRxPkts);
    FSCI_Init(&mFsciConfig);
    FSCI_RegisterOpGroup(ZB_COMMON_FSCI_OG, gFsciMonitorMode_c, APP_ProcessFsciPkt, NULL, u8SerialId);
    FSCI_RegisterOpGroup(ZB_ZHA_FSCI_OG,    gFsciMonitorMode_c, APP_ProcessFsciPkt, NULL, u8SerialId);
    FSCI_RegisterOpGroup(ZB_PDM_FSCI_OG,    gFsciMonitorMode_c, APP_ProcessFsciPkt, NULL, u8SerialId);
    FSCI_RegisterOpGroup(ZB_MISC_FSCI_OG,   gFsciMonitorMode_c, APP_ProcessFsciPkt, NULL, u8SerialId);
    FSCI_RegisterOpGroup(ZB_IAS_FSCI_OG,    gFsciMonitorMode_c, APP_ProcessFsciPkt, NULL, u8SerialId);
    FSCI_RegisterOpGroup(ZB_OTA_FSCI_OG,    gFsciMonitorMode_c, APP_ProcessFsciPkt, NULL, u8SerialId);
  #else
    Serial_InitInterface(&u8SerialId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate  (u8SerialId,  APP_SERIAL_INTERFACE_SPEED);
    APP_SerialId = u8SerialId;
  #endif
    DBG_vPrintf ( TRACE_APPSTART, "APP: Entering APP_vInitResources()\n" );
    APP_vInitResources ( );

    vLog_Printf ( TRACE_APPSTART,LOG_DEBUG, "\n\nInitialising \n" );


    vLog_Printf ( TRACE_EXC, LOG_INFO, "\n** Control Bridge Reset** " );

    if (RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog)
    {
        vLog_Printf ( TRACE_EXC, LOG_CRIT, "\nWATCHDOG RESET");
        vSL_LogFlush ( );
    }

    vInitialiseApp ();

    app_vFormatAndSendUpdateLists ( );

    
    if (sZllState.eNodeState == E_RUNNING)
    {
        /* Declared within if statement. If it is declared at the top
         * the function, the while loop will cause the data to be on
         * the stack forever.
         */
        if( sZllState.u8DeviceType >=  1 )
        {
            u8FormJoin = 0;
        }
        else
        {
            u8FormJoin = 1;
        }

        APP_vSendJoinedFormEventToHost ( u8FormJoin, au8LinkTxBuffer );
        vSL_WriteMessage ( E_SL_MSG_NODE_NON_FACTORY_NEW_RESTART,
                           1,
                           ( uint8* ) &sZllState.eNodeState ) ;
        BDB_vStart();
    }
    else
    {
        vSL_WriteMessage( E_SL_MSG_NODE_FACTORY_NEW_RESTART,
                          1,
                          ( uint8* ) &sZllState.eNodeState );

    }

    ZTIMER_eStart ( u8TickTimer, ZCL_TICK_TIME );
}

/****************************************************************************
 *
 * NAME: vAppRegisterPWRMCallbacks
 *
 * DESCRIPTION:
 * 
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/

void vAppRegisterPWRMCallbacks(void)
{

}

/****************************************************************************
 *
 * NAME: APP_cbToggleLED
 *
 * DESCRIPTION:
 * Toggle LED callback
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void APP_cbToggleLED ( void* pvParam )
{
    tsLedState*    psLedState =  ( tsLedState* ) pvParam;

    Led1Toggle();

    if( u8JoinedDevice == 10 )
    {
        if(  !ZPS_vNwkGetPermitJoiningStatus ( ZPS_pvAplZdoGetNwkHandle ( ) ) )
        {
            psLedState->u32LedToggleTime =  ZTIMER_TIME_MSEC ( 10 );
        }

        if( ZPS_vNwkGetPermitJoiningStatus ( ZPS_pvAplZdoGetNwkHandle ( ) ) )
        {
            psLedState->u32LedToggleTime =  ZTIMER_TIME_MSEC ( 500 );
        }
        u8JoinedDevice =  0;
    }
    u8JoinedDevice++;

    ZTIMER_eStart( u8TmrToggleLED, psLedState->u32LedToggleTime );
}

#ifdef FULL_FUNC_DEVICE

/****************************************************************************
 *
 * NAME: psGetEndpointRecordTable
 *
 * DESCRIPTION:
 * return the address of the endpoinTable
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC tsZllEndpointInfoTable * psGetEndpointRecordTable(void)
{
    return &sEndpointTable;
}


/****************************************************************************
 *
 * NAME: psGetGroupRecordTable
 *
 * DESCRIPTION:
 * return the address of the group record table
 *
 * RETURNS:
 * pointer to the group record table
 *
 ****************************************************************************/
PUBLIC tsZllGroupInfoTable * psGetGroupRecordTable(void)
{
    return &sGroupTable;
}

#endif
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vInitialiseApp
 *
 * DESCRIPTION:
 * Initialises Zigbee stack and application.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vInitialiseApp ( void )
{
    uint16            u16DataBytesRead;
    BDB_tsInitArgs    sArgs;
    uint8             u8DeviceType;


    PDM_eInitialise(700, 63);

    PDUM_vInit ( );

    PWRM_vInit ( 3 );

    sZllState.eNodeState =  E_STARTUP;
    /* Restore any application data previously saved to flash */
    PDM_eReadDataFromRecord ( PDM_ID_APP_ZLL_CMSSION,
                              &sZllState,
                              sizeof ( tsZllState ),
                              &u16DataBytesRead );
#ifdef FULL_FUNC_DEVICE
    PDM_eReadDataFromRecord ( PDM_ID_APP_END_P_TABLE,
                              &sEndpointTable,
                              sizeof ( tsZllEndpointInfoTable ),
                              &u16DataBytesRead );
    PDM_eReadDataFromRecord ( PDM_ID_APP_GROUP_TABLE,
                              &sGroupTable,
                              sizeof ( tsZllGroupInfoTable ),
                              &u16DataBytesRead );
#endif
    ZPS_eMiniMacSetTxBuffers ( 5 );

    if ( sZllState.eNodeState == E_RUNNING )
    {
        u8DeviceType = ( sZllState.u8DeviceType >=  2 ) ? 1 : sZllState.u8DeviceType;
        APP_vConfigureDevice ( u8DeviceType );
        ZPS_eAplAfInit ( );
    }
    else
    {

        ZPS_eAplAfInit ( );
        sZllState.u8DeviceType =  0;
        ZPS_vNwkNibSetChannel ( ZPS_pvAplZdoGetNwkHandle(), DEFAULT_CHANNEL);
        ZPS_vNwkNibSetPanId (ZPS_pvAplZdoGetNwkHandle(), (uint16) RND_u32GetRand ( 1, 0xfff0 ) );

    }

    APP_vSetMacAddr();

    /* If the device state has been restored from flash, re-start the stack
     * and set the application running again.
     */
    sBDB.sAttrib.bbdbNodeIsOnANetwork      =  ( ( sZllState.eNodeState >= E_RUNNING ) ? ( TRUE ) : ( FALSE ) );
    sBDB.sAttrib.bTLStealNotAllowed        =  !sBDB.sAttrib.bbdbNodeIsOnANetwork;
    sArgs.hBdbEventsMsgQ                   =  &APP_msgBdbEvents;
    BDB_vInit ( &sArgs );

    ZPS_vExtendedStatusSetCallback(vfExtendedStatusCallBack);
    APP_ZCL_vInitialise();
    /* Needs to be after we initialise the ZCL and only if we are already
     * running. If we are not running we will send the notify after we
     * have a network formed notification.
     */
    if (sZllState.eNodeState == E_RUNNING)
    {
#ifdef CLD_OTA
        vAppInitOTA();
#endif
    }
}

/****************************************************************************
 *
 * NAME: main_task
 *
 * DESCRIPTION:
 * application task.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void main_task (uint32_t parameter)
{
    
    static uint8_t initialized = FALSE;

    if(!initialized)
    {
        vTimerServerInit();
        
        /* Init memory blocks manager */
        MEM_Init();
        RNG_Init();
        SecLib_Init();
        LED_Init();
        /* Init  timers module */
        TMR_Init();
        SerialManager_Init();
        /* Initialize Keyboard (Switches) Module */
        KBD_Init(KBD_Callback);
        
        /* Wdog/Cop Setup */
        APP_vSetUpWdog();
        
        initialized = TRUE;
        vAppMain();

        bSetPermitJoinForever = TRUE;
    }

    while(1)
    {
         /* place event handler code here... */

        zps_taskZPS ( );
        bdb_taskBDB ( );

        APP_vHandleAppEvents ( );
        APP_vProcessRxData ( );
        ZTIMER_vTask ( );

#ifdef DBG_ENABLE
        vSL_LogFlush ( ); /* flush buffers */
#endif
        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed within the watchdog period
         * otherwise the system will be reset.
         */
        COP_Refresh(SIM);

#if gNvStorageIncluded_d
        NvIdle();
#endif
        /*
         * suspends CPU operation when the system is idle or puts the device to
         * sleep if there are no activities in progress
         */
        PWRM_vManagePower();

        if(!gUseRtos_c)
        {
            break;
        }
    }
}

/****************************************************************************
 *
 * NAME: APP_vSetUpWdog
 *
 * DESCRIPTION:
 * Set up Wdog 
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_vSetUpWdog(void)
{

    if (RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog)
    {
        DBG_vPrintf(TRUE, "APP: Watchdog timer has reset device!\r\n");
        COP_Disable(SIM);
        while(1);
    }
    else
    {
        cop_config_t configCop;
        COP_GetDefaultConfig(&configCop);
        /* timeout cycle - kCOP_2Power5CyclesOr2Power13Cycles gives approx 8 sec timeout and
         * kCOP_2Power8CyclesOr2Power16Cycles gives approx 64 sec timeout */
        /* Set watch dog feed time constant to approximately 8s */
        configCop.timeoutCycles = kCOP_2Power5CyclesOr2Power13Cycles;
        COP_Init(SIM, &configCop);
    }

}


/****************************************************************************
 *
 * NAME: APP_vInitResources
 *
 * DESCRIPTION:
 * Initialise resources (timers, queue's etc)
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitResources ( void )
{

    vLog_Printf ( TRACE_APPSTART,LOG_DEBUG, "APP: Initialising resources...\n");
    vLog_Printf ( TRACE_APPSTART,LOG_DEBUG, "APP: ZPS_tsAfEvent = %d bytes\n",    sizeof ( ZPS_tsAfEvent ) );
    vLog_Printf ( TRACE_APPSTART,LOG_DEBUG, "APP: zps_tsTimeEvent = %d bytes\n",  sizeof ( zps_tsTimeEvent ) );

    /* Initialise the Z timer module */
    ZTIMER_eInit ( asTimers, sizeof(asTimers) / sizeof(ZTIMER_tsTimer));

    /* Create Z timers */
    ZTIMER_eOpen ( &u8TickTimer,       APP_cbTimerZclTick,          NULL,                      ZTIMER_FLAG_PREVENT_SLEEP );
    ZTIMER_eOpen ( &u8IdTimer,         APP_vIdentifyEffectEnd,      NULL,                      ZTIMER_FLAG_PREVENT_SLEEP );
    ZTIMER_eOpen ( &u8TmrToggleLED,    APP_cbToggleLED,             &s_sLedState,              ZTIMER_FLAG_PREVENT_SLEEP );
    ZTIMER_eOpen ( &u8HaModeTimer,     App_TransportKeyCallback,    &u64CallbackMacAddress,    ZTIMER_FLAG_PREVENT_SLEEP );
    ZTIMER_eOpen ( &u8AppPollTimer,    App_vPollTimerCallback,      &u32TimerSeconds,    ZTIMER_FLAG_PREVENT_SLEEP );

    /* Create all the queues */
    ZQ_vQueueCreate ( &APP_msgBdbEvents,      BDB_QUEUE_SIZE,         sizeof ( BDB_tsZpsAfEvent ),       NULL );
    ZQ_vQueueCreate ( &zps_msgMlmeDcfmInd,    MLME_QUEQUE_SIZE,       sizeof ( MAC_tsMlmeVsDcfmInd ),    NULL );
    ZQ_vQueueCreate ( &zps_msgMcpsDcfmInd,    MCPS_QUEUE_SIZE,        sizeof ( MAC_tsMcpsVsDcfmInd ),    NULL );
    ZQ_vQueueCreate ( &zps_TimeEvents,        TIMER_QUEUE_SIZE,       sizeof ( zps_tsTimeEvent ),        NULL);
    ZQ_vQueueCreate ( &APP_msgAppEvents,      APP_QUEUE_SIZE,         sizeof ( APP_tsEvent ),            NULL );

    vZCL_RegisterHandleGeneralCmdCallBack ((void*)APP_vProfileWideCommandSupportedForCluster );

    DBG_vPrintf(TRACE_APPSTART, "APP: Initialising resources complete\n");

}

/****************************************************************************
 *
 * NAME: app_vFormatAndSendUpdateLists
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void app_vFormatAndSendUpdateLists ( void )
{
    typedef struct
    {
        uint16     u16Clusterid;
        uint16*    au16Attibutes;
        uint32     u32Asize;
        uint8*     au8command;
        uint32     u32Csize;
    }    tsAttribCommand;

    uint16                 u16Length =  0;
    uint8                  u8Count = 0 ;
    uint8                  u8BufferLoop =  0;
    uint8                  au8LinkTxBuffer[256];
    uint8                  u8MyEndPoint = ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT;

    /*List of clusters per endpoint */
    uint16    u16ClusterListHA [ ]  =  { 0x0000, 0x0001, 0x0003, 0x0004, 0x0005, 0x0006,
                                         0x0008, 0x0019, 0x0101, 0x1000, 0x0300, 0x0201, 0x0204 };
    uint16    u16ClusterListHA2 [ ] =  { 0x0405, 0x0500, 0x0400, 0x0402, 0x0403, 0x0405, 0x0406,
                                         0x0702, 0x0b03, 0x0b04 , 0x1000 };

    /*list of attributes per cluster */

    uint16    u16AttribBasic [ ] =  { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004,
                                      0x0005, 0x0006, 0x0007, 0x4000 };
    uint16    u16AttribIdentify [ ] =  { 0x000 };
    uint16    u16AttribGroups [ ] =  { 0x000 };
    uint16    u16AttribScenes [ ] =  { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005 };
    uint16    u16AttribOnOff [ ] =  { 0x0000, 0x4000, 0x4001, 0x4002 };
    uint16    u16AttribLevel [ ] =  { 0x0000, 0x0001, 0x0010, 0x0011 };
    uint16    u16AttribColour [ ] =  { 0x0000, 0x0001, 0x0002, 0x0007, 0x0008,
                                       0x0010, 0x0011, 0x0012, 0x0013, 0x0015,
                                       0x0016, 0x0017, 0x0019, 0x001A, 0x0020,
                                       0x0021, 0x0022, 0x0024, 0x0025, 0x0026,
                                       0x0028, 0x0029, 0x002A, 0x4000, 0x4001,
                                       0x4002, 0x4003, 0x4004, 0x4006, 0x400A,
                                       0x400B, 0x400C };
    uint16    u16AttribThermostat [ ] =  { 0x0000, 0x0003, 0x0004, 0x0011, 0x0012,
                                           0x001B, 0x001C };
    uint16    u16AttribHum [ ] =  { 0x0000, 0x0001, 0x0002, 0x0003 };
    uint16    u16AttribPower [ ] =  { 0x0020, 0x0034 };
    uint16    u16AttribIllumM [ ] =  { 0x000, 0x0001, 0x0002, 0x0003, 0x0004 };
    uint16    u16AttribIllumT [ ] =  { 0x000, 0x0001, 0x0002 };
    uint16    u16AttribSM [ ] =  { 0x0000, 0x0300, 0x0301, 0x0302, 0x0306, 0x0400 };
    /*list of commands per cluster */
    uint8     u8CommandBasic [ ] =  { 0 };
    uint8     u8CommandIdentify [ ] =  { 0, 1, 0x40 };
    uint8     u8CommandGroups [ ] =  { 0, 1, 2, 3, 4, 5 };
    uint8     u8CommandScenes [ ] =  { 0, 1, 2, 3, 4, 5, 6,
                                    0x40, 0x41, 0x42 };
    uint8     u8CommandsOnOff [ ] =  { 0, 1, 2, 0x40, 0x41, 0x42 };
    uint8     u8CommandsLevel [ ] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    uint8     u8CommandsColour [ ] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                     0xa, 0x40, 0x41, 0x42, 0x43, 0x44, 0x47,
                                     0x4b, 0x4c, 0xfe, 0xff };
    uint8     u8CommandThermostat [ ] = {0};
    uint8     u8CommandHum [ ] =  { 0 };
    uint8     u8CommandPower [ ] =  { 0 };
    uint8     u8CommandIllumM [ ] =  { 0 };
    uint8     u8CommandIllumT [ ] =  { 0 };
    uint8     u8CommandSM [ ] =  { 0 };

    tsAttribCommand    asAttribCommand[13] =  {  { 0x0000, u16AttribBasic, ( sizeof( u16AttribBasic ) / sizeof ( u16AttribBasic [ 0 ] ) ),
                                                  u8CommandBasic, ( sizeof ( u8CommandBasic ) / sizeof ( u8CommandBasic [ 0 ] )  )},
                                                 { 0x0003, u16AttribIdentify, ( sizeof ( u16AttribIdentify ) / sizeof ( u16AttribIdentify [ 0 ]  ) ),
                                                   u8CommandIdentify, ( sizeof ( u8CommandIdentify ) / sizeof ( u8CommandIdentify [ 0 ]  ) ) },
                                                 { 0x0004, u16AttribGroups, ( sizeof ( u16AttribGroups ) / sizeof ( u16AttribGroups [ 0 ]  ) ),
                                                   u8CommandGroups, ( sizeof ( u8CommandGroups ) / sizeof ( u8CommandGroups [ 0 ]  ) ) },
                                                 { 0x0005, u16AttribScenes, ( sizeof ( u16AttribScenes ) / sizeof ( u16AttribScenes [ 0 ]  ) ),
                                                   u8CommandScenes, ( sizeof( u8CommandScenes )  / sizeof ( u8CommandScenes [ 0 ]  ) ) },
                                                 { 0x0006, u16AttribOnOff,  ( sizeof ( u16AttribOnOff ) / sizeof ( u16AttribOnOff [ 0 ]  ) ),
                                                   u8CommandsOnOff, ( sizeof ( u8CommandsOnOff )/ sizeof ( u8CommandsOnOff [ 0 ]  ) ) },
                                                 { 0x0008, u16AttribLevel,  ( sizeof ( u16AttribLevel ) / sizeof ( u16AttribLevel [ 0 ]  ) ),
                                                   u8CommandsLevel, ( sizeof ( u8CommandsLevel ) / sizeof ( u8CommandsLevel [ 0 ]  ) ) },
                                                 { 0x0300, u16AttribColour, ( sizeof ( u16AttribColour ) / sizeof ( u16AttribColour [ 0 ]  ) ),
                                                   u8CommandsColour, ( sizeof ( u8CommandsColour ) / sizeof ( u8CommandsColour [ 0 ]  ) ) },
                                                 { 0x0201, u16AttribThermostat, ( sizeof ( u16AttribThermostat ) / sizeof ( u16AttribThermostat [ 0 ]  ) ),
                                                   u8CommandThermostat, ( sizeof ( u8CommandThermostat ) / sizeof ( u8CommandThermostat [ 0 ]  ) ) },
                                                 { 0x0405, u16AttribHum, ( sizeof ( u16AttribHum ) / sizeof ( u16AttribHum [ 0 ]  ) ),
                                                   u8CommandHum, ( sizeof ( u8CommandHum ) / sizeof ( u8CommandHum [ 0 ]  ) ) },
                                                 { 0x0001, u16AttribPower, ( sizeof ( u16AttribPower ) /sizeof( u16AttribPower [ 0 ]  ) ),
                                                   u8CommandPower, ( sizeof ( u8CommandPower ) /sizeof( u8CommandPower [ 0 ]  ) ) },
                                                 { 0x0400, u16AttribIllumM, ( sizeof ( u16AttribIllumM ) /sizeof( u16AttribIllumM [ 0 ]  ) ),
                                                   u8CommandIllumM, ( sizeof ( u8CommandIllumM ) /sizeof( u8CommandIllumM [ 0 ]  ) ) },
                                                 { 0x0402, u16AttribIllumT, ( sizeof ( u16AttribIllumT ) /sizeof( u16AttribIllumT [ 0 ]  ) ),
                                                   u8CommandIllumT, ( sizeof ( u8CommandIllumT ) /sizeof( u8CommandIllumT [ 0 ]  ) ) },
                                                 { 0x0702, u16AttribSM, ( sizeof ( u16AttribSM ) / sizeof ( u16AttribSM [ 0 ]  ) ),
                                                   u8CommandSM, ( sizeof ( u8CommandSM ) / sizeof ( u8CommandSM [ 0 ]  ) )} };



    /* Cluster list endpoint HA */
    ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],            u8MyEndPoint,    u16Length );
    ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ],    0x0104,                                  u16Length );
    while ( u8BufferLoop < ( sizeof ( u16ClusterListHA ) /  sizeof( u16ClusterListHA [ 0 ] ) ) )
    {
        ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ],    u16ClusterListHA [ u8BufferLoop ],    u16Length );
        u8BufferLoop++;
    }
    vSL_WriteMessage ( E_SL_MSG_NODE_CLUSTER_LIST,
                       u16Length,
                       au8LinkTxBuffer );

        /* Cluster list endpoint HA */
    u16Length = u8BufferLoop =  0;
    ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],         u8MyEndPoint,    u16Length );
    ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ], 0x0104,                                  u16Length );
    while ( u8BufferLoop <  ( sizeof ( u16ClusterListHA2 ) /  sizeof ( u16ClusterListHA2 [ 0 ] ) )  )
    {

        ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ], u16ClusterListHA2 [ u8BufferLoop ],    u16Length );
        u8BufferLoop++;
    }
    vSL_WriteMessage ( E_SL_MSG_NODE_CLUSTER_LIST,
                       u16Length,
                       au8LinkTxBuffer );

        /* Attribute list basic cluster HA EP*/
    for ( u8Count =  0; u8Count < 13; u8Count++ )
    {
        u16Length =  0;
        u8BufferLoop =  0;
        ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],            u8MyEndPoint,     u16Length );
        ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ],    0x0104,                                   u16Length );
        ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ],    asAttribCommand[u8Count].u16Clusterid,    u16Length );
        while ( u8BufferLoop <   asAttribCommand [ u8Count ].u32Asize   )
        {
            ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ],    asAttribCommand[u8Count].au16Attibutes [ u8BufferLoop ],    u16Length );
            u8BufferLoop++;
        }
        vSL_WriteMessage ( E_SL_MSG_NODE_ATTRIBUTE_LIST,
                           u16Length,
                           au8LinkTxBuffer );
        u16Length =  0;
        u8BufferLoop =  0;
        ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],            u8MyEndPoint,     u16Length );
        ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ],    0x0104,                                   u16Length );
        ZNC_BUF_U16_UPD ( &au8LinkTxBuffer [ u16Length ],    asAttribCommand[u8Count].u16Clusterid,    u16Length );
        while ( u8BufferLoop <   asAttribCommand [ u8Count ].u32Csize  )
        {
            ZNC_BUF_U8_UPD ( &au8LinkTxBuffer [ u16Length ],    asAttribCommand[u8Count].au8command [ u8BufferLoop ],    u16Length );
            u8BufferLoop++;
        }
        vSL_WriteMessage ( E_SL_MSG_NODE_COMMAND_ID_LIST,
                           u16Length,
                           au8LinkTxBuffer );
    }
    
}
/****************************************************************************
 *
 * NAME: app_vFormatAndSendUpdateLists
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void vfExtendedStatusCallBack ( ZPS_teExtendedStatus    eExtendedStatus )
{
    vLog_Printf ( TRACE_EXC,LOG_DEBUG, "ERROR: Extended status %x\n", eExtendedStatus );
}
/****************************************************************************
 *
 * NAME: app_vFormatAndSendUpdateLists
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/

PRIVATE void APP_cbTimerZclTick (void*    pvParam)
{
    static uint8 u8Tick100Ms = 9;

    tsZCL_CallBackEvent sCallBackEvent;

    if(ZTIMER_eStart(u8TickTimer, ZTIMER_TIME_MSEC(100)) != E_ZTIMER_OK)
    {
        vLog_Printf ( TRACE_EXC,LOG_DEBUG,  "APP: Failed to Start Tick Timer\n" );
    }


    /* Provide 100ms tick to cluster */
    eZCL_Update100mS();

    /* Provide 1sec tick to cluster - Wrap 1 second  */
    u8Tick100Ms++;
    if(u8Tick100Ms > 9)
    {
        u8Tick100Ms = 0;
        sCallBackEvent.pZPSevent = NULL;
        sCallBackEvent.eEventType = E_ZCL_CBET_TIMER;
        vZCL_EventHandler(&sCallBackEvent);
    }
}
/****************************************************************************
 *
 * NAME: app_vFormatAndSendUpdateLists
 *
 * DESCRIPTION:
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
bool_t APP_vProfileWideCommandSupportedForCluster ( uint16 u16Clusterid )
{
    if ( u16Clusterid == MEASUREMENT_AND_SENSING_CLUSTER_ID_OCCUPANCY_SENSING)
    {
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************
 *
 * NAME: main
 *
 * DESCRIPTION:
 * Entry point into the application.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
extern void ResetMCU(void);
PRIVATE void KBD_Callback( uint8_t events)
{
    switch(events)
    {
        case gKBD_EventPB1_c:
        case gKBD_EventPB2_c:
        case gKBD_EventPB3_c:
        case gKBD_EventPB4_c:
        case gKBD_EventLongPB1_c:
        case gKBD_EventLongPB2_c:
        case gKBD_EventLongPB3_c:
        case gKBD_EventLongPB4_c:
            return;
        case gKBD_EventVeryLongPB1_c: /* factory reset */
        case gKBD_EventVeryLongPB2_c:
        case gKBD_EventVeryLongPB3_c:
        case gKBD_EventVeryLongPB4_c:
            NvFormat();
            ResetMCU();
            break;
        default:
            return;
    }
}

/****************************************************************************
 *
 * NAME: APP_GetByteFromSerial
 *
 * DESCRIPTION:
 * Return bytes receive in serial buffer
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
#if !gFsciIncluded_c
bool_t APP_GetByteFromSerial(uint8 * byte)
{
    uint16_t len = 0;

    Serial_GetByteFromRxBuffer(u8SerialId, byte, &len);
    
    return (bool_t)len;
}
#endif

/****************************************************************************
 *
 * NAME: APP_ProcessFsciPkt
 *
 * DESCRIPTION:
 * Add the received FSCI packet into a queue
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
#if gFsciIncluded_c
static void APP_ProcessFsciPkt(void* pData, void* param, uint32_t fsciInterface)
{
    MSG_Queue(&mFsciRxPkts, pData);
}
#endif


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
