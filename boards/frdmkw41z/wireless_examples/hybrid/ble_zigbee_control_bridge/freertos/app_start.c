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
\file       app_start.c
\brief      This is a public source file for the Hybrid BleZigbeeNodeControlBridge demo application.
==================================================================================================*/

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
#include "RNG_Interface.h"
#include "TimersManager.h"
#include "Keyboard.h"
#include "Messaging.h"
#include "SecLib.h"
#include "Panic.h"

/* Bluetooth Low Energy */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "ble_init.h"
#include "ble_config.h"
#include "l2ca_cb_interface.h"
#include "app_zb_utils.h"

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


/* Application Events */
#define gAppEvtMsgFromHostStack_c       (1 << 0)
#define gAppEvtAppCallback_c            (1 << 1)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/* Host to Application Messages Types */
typedef enum {
    gAppGapGenericMsg_c = 0,
    gAppGapConnectionMsg_c,
    gAppGapAdvertisementMsg_c,
    gAppGapScanMsg_c,
    gAppGattServerMsg_c,
    gAppGattClientProcedureMsg_c,
    gAppGattClientNotificationMsg_c,
    gAppGattClientIndicationMsg_c,
    gAppL2caLeDataMsg_c,
    gAppL2caLeControlMsg_c,
}appHostMsgType_tag;

typedef uint8_t appHostMsgType_t;

/* Host to Application Connection Message */
typedef struct connectionMsg_tag{
    deviceId_t              deviceId;
    gapConnectionEvent_t    connEvent;
}connectionMsg_t;

/* Host to Application GATT Server Message */
typedef struct gattServerMsg_tag{
    deviceId_t          deviceId;
    gattServerEvent_t   serverEvent;
}gattServerMsg_t;

/* Host to Application GATT Client Procedure Message */
typedef struct gattClientProcMsg_tag{
    deviceId_t              deviceId;
    gattProcedureType_t     procedureType;
    gattProcedureResult_t   procedureResult;
    bleResult_t             error;
}gattClientProcMsg_t;

/* Host to Application GATT Client Notification/Indication Message */
typedef struct gattClientNotifIndMsg_tag{
    uint8_t*    aValue;
    uint16_t    characteristicValueHandle;
    uint16_t    valueLength;
    deviceId_t  deviceId;
}gattClientNotifIndMsg_t;

/* L2ca to Application Data Message */
typedef struct l2caLeCbDataMsg_tag{
    deviceId_t  deviceId;
    uint16_t    lePsm;
    uint16_t    packetLength;
    uint8_t     aPacket[0];
}l2caLeCbDataMsg_t;

/* L2ca to Application Control Message */
typedef struct l2caLeCbControlMsg_tag{
    l2capControlMessageType_t   messageType;
    uint16_t                    padding;
    uint8_t                     aMessage[0];
}l2caLeCbControlMsg_t;

typedef struct appMsgFromHost_tag{
    appHostMsgType_t    msgType;
    union {
        gapGenericEvent_t       genericMsg;
        gapAdvertisingEvent_t   advMsg;
        connectionMsg_t         connMsg;
        gapScanningEvent_t      scanMsg;
        gattServerMsg_t         gattServerMsg;
        gattClientProcMsg_t     gattClientProcMsg;
        gattClientNotifIndMsg_t gattClientNotifIndMsg;
        l2caLeCbDataMsg_t       l2caLeCbDataMsg;
        l2caLeCbControlMsg_t    l2caLeCbControlMsg;
    } msgData;
}appMsgFromHost_t;

typedef struct appMsgCallback_tag{
    appCallbackHandler_t   handler;
    appCallbackParam_t     param;
} appMsgCallback_t;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void KBD_Callback( uint8_t events);

PUBLIC void app_vFormatAndSendUpdateLists ( void );
PUBLIC void APP_vMainLoop ( void );
PUBLIC void APP_vInitResources ( void );
PRIVATE void APP_vSetUpWdog ( void );

void vfExtendedStatusCallBack ( ZPS_teExtendedStatus    eExtendedStatus );
PRIVATE void vInitialiseApp ( void );

PRIVATE void APP_cbTimerZclTick (void*    pvParam);

PRIVATE void App_HandleHostMessageInput(appMsgFromHost_t* pMsg);
PRIVATE void App_GenericCallback (gapGenericEvent_t* pGenericEvent);
PRIVATE void App_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
PRIVATE void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
PRIVATE void App_ScanningCallback (gapScanningEvent_t* pAdvertisingEvent);
PRIVATE void App_GattServerCallback (deviceId_t peerDeviceId, gattServerEvent_t* pServerEvent);
PRIVATE void App_GattClientProcedureCallback
(
    deviceId_t              deviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);
PRIVATE void App_GattClientNotificationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
);
PRIVATE void App_GattClientIndicationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
);

PRIVATE void App_L2caLeDataCallback
(
    deviceId_t deviceId,
    uint16_t   lePsm,
    uint8_t* pPacket,
    uint16_t packetLength
);

PRIVATE void App_L2caLeControlCallback
(
    l2capControlMessageType_t  messageType,
    void* pMessage
);

#if !gUseHciTransportDownward_d
PRIVATE void BLE_SignalFromISRCallback(void);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

extern gapAdvertisingData_t     gAppAdvertisingData;
extern const gapAdvertisingParameters_t gAppAdvParams;
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
uint8                     u8TickTimer;
uint8                     u8JoinedDevice =  0;
uint8                     au8LinkRxBuffer[256];

ZPS_tsAfFlashInfoSet      sSet;

#if (cPWR_UsePowerDownMode || gAppUseNvm_d)
#if (!mAppIdleHook_c)
OSA_TASK_DEFINE( App_Idle_Task, gAppIdleTaskPriority_c, 1, gAppIdleTaskStackSize_c, FALSE );
osaTaskId_t gAppIdleTaskId = 0;
#endif
#endif  /* cPWR_UsePowerDownMode */

#if gAppUseNvm_d
PRIVATE bleBondIdentityHeaderBlob_t*  aBondingHeader[gMaxBondedDevices_c];
PRIVATE bleBondDataDynamicBlob_t*     aBondingDataDynamic[gMaxBondedDevices_c];
PRIVATE bleBondDataPRIVATEBlob_t*      aBondingDataPRIVATE[gMaxBondedDevices_c];
PRIVATE bleBondDataDeviceInfoBlob_t*  aBondingDataDeviceInfo[gMaxBondedDevices_c];
PRIVATE bleBondDataDescriptorBlob_t* aBondingDataDescriptor[gMaxBondedDevices_c * gcGapMaximumSavedCccds_c];

NVM_RegisterDataSet(aBondingHeader, gMaxBondedDevices_c, gBleBondIdentityHeaderSize_c, nvmId_BondingHeaderId_c, gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataDynamic, gMaxBondedDevices_c, gBleBondDataDynamicSize_c, nvmId_BondingDataDynamicId_c, gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataPRIVATE, gMaxBondedDevices_c, gBleBondDataPRIVATESize_c, nvmId_BondingDataPRIVATEId_c, gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataDeviceInfo, gMaxBondedDevices_c, gBleBondDataDeviceInfoSize_c, nvmId_BondingDataDeviceInfoId_c, gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataDescriptor, gMaxBondedDevices_c * gcGapMaximumSavedCccds_c, gBleBondDataDescriptorSize_c, nvmId_BondingDataDescriptorId_c, gNVM_NotMirroredInRamAutoRestore_c);
#else
PRIVATE bleBondDataBlob_t          maBondDataBlobs[gMaxBondedDevices_c] = {{{{0}}}};
#endif

/* Application input queues */
PRIVATE anchor_t mHostAppInputQueue;
PRIVATE anchor_t mAppCbInputQueue;

PRIVATE gapGenericCallback_t pfGenericCallback = NULL;
PRIVATE gapAdvertisingCallback_t pfAdvCallback = NULL;
PRIVATE gapScanningCallback_t pfScanCallback = NULL;
PRIVATE gapConnectionCallback_t  pfConnCallback = NULL;
PRIVATE gattServerCallback_t pfGattServerCallback = NULL;
PRIVATE gattClientProcedureCallback_t pfGattClientProcCallback = NULL;
PRIVATE gattClientNotificationCallback_t pfGattClientNotifCallback = NULL;
PRIVATE gattClientNotificationCallback_t pfGattClientIndCallback = NULL;
PRIVATE l2caLeCbDataCallback_t           pfL2caLeCbDataCallback = NULL;
PRIVATE l2caLeCbControlCallback_t        pfL2caLeCbControlCallback = NULL;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
#ifdef FULL_FUNC_DEVICE
extern PUBLIC void APP_cbTimerCommission ( void*    pvParam );
#endif

extern void vTimerServerInit(void);

extern void OSA_TimeInit(void);
extern void BleApp_Init(void);
extern void ResetMCU(void);


extern const uint8_t gUseRtos_c;

#if gUseHciTransportUpward_d
#define BleApp_GenericCallback(param)
#else
extern void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent);
#endif

#if !gUseHciTransportDownward_d
extern void (*pfBLE_SignalFromISR)(void);
#endif /* gUseHciTransportDownward_d */

void BleApp_Task (uint32_t param);

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
    
    Serial_InitInterface(&u8SerialId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate  (u8SerialId,  APP_SERIAL_INTERFACE_SPEED);
    APP_SerialId = u8SerialId;

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
    uint8_t pseudoRNGSeed[20] = {0};

    if(!initialized)
    {
        vTimerServerInit();
        
        /* Init memory blocks manager */
        MEM_Init();
        SecLib_Init();
        /* Init  timers module */
        TMR_Init();
        SerialManager_Init();
        RNG_Init();
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[0])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[4])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[8])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[12])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[16])));
        RNG_SetPseudoRandomNoSeed(pseudoRNGSeed);
        
        /* Initialize Keyboard (Switches) Module */
        /* Wdog/Cop Setup */
        APP_vSetUpWdog();
        KBD_Init(KBD_Callback);
        
#if gAppUseNvm_d        
        /* Initialize NV module */
        NvModuleInit();
#endif      
        
#if !gUseHciTransportDownward_d
        pfBLE_SignalFromISR = BLE_SignalFromISRCallback;        
#endif /* !gUseHciTransportDownward_d */

#if (cPWR_UsePowerDownMode)
        PWR_Init();
        PWR_DisallowDeviceToSleep();
#endif            
        /* Initialize ble and peripheral drivers specific to the ble application */
        BleApp_Init();        

        /* Prepare application input queue.*/
        MSG_InitQueue(&mHostAppInputQueue);
        
        /* Prepare callback input queue.*/
        MSG_InitQueue(&mAppCbInputQueue);
        
        /* BLE Host Stack Init */
        if (Ble_Initialize(App_GenericCallback) != gBleSuccess_c)
        {
            panic(0,0,0,0);
            return;
        }
        
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
        
        /* Ble app */
        BleApp_Task(0);      

#ifdef DBG_ENABLE
        vSL_LogFlush ( ); /* flush buffers */
#endif
        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed within the watchdog period
         * otherwise the system will be reset.
         */
#ifdef CPU_MKW41Z512VHT4
        COP_Refresh(SIM);
#else
        /* Kick the watchdog */
        WWDT_Refresh(WWDT);
#endif

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
    ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],            ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT,    u16Length );
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
    ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],         ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT,    u16Length );
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
        ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],            ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT,     u16Length );
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
        ZNC_BUF_U8_UPD  ( &au8LinkTxBuffer [ 0 ],            ZIGBEENODECONTROLBRIDGE_ZLO_ENDPOINT,     u16Length );
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


/* BLE related */

bleResult_t App_Connect(
    gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t             connCallback
)
{
    pfConnCallback = connCallback;
    
    return Gap_Connect(pParameters, App_ConnectionCallback);
}

bleResult_t App_StartAdvertising(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback
)
{
    pfAdvCallback = advertisingCallback;
    pfConnCallback = connectionCallback;
    
    return Gap_StartAdvertising(App_AdvertisingCallback, App_ConnectionCallback);
}

bleResult_t App_StartScanning(
    gapScanningParameters_t*    pScanningParameters,
    gapScanningCallback_t       scanningCallback,
    bool_t                      enableFilterDuplicates
)
{
    pfScanCallback = scanningCallback;
    
    return Gap_StartScanning(pScanningParameters, App_ScanningCallback, enableFilterDuplicates);
}

bleResult_t App_RegisterGattServerCallback(gattServerCallback_t  serverCallback)
{
    pfGattServerCallback = serverCallback;
    
    return GattServer_RegisterCallback(App_GattServerCallback);
}

bleResult_t App_RegisterGattClientProcedureCallback(gattClientProcedureCallback_t  callback)
{
    pfGattClientProcCallback = callback;

    return GattClient_RegisterProcedureCallback(App_GattClientProcedureCallback);
}

bleResult_t App_RegisterGattClientNotificationCallback(gattClientNotificationCallback_t  callback)
{
    pfGattClientNotifCallback = callback;

    return GattClient_RegisterNotificationCallback(App_GattClientNotificationCallback);
}

bleResult_t App_RegisterGattClientIndicationCallback(gattClientIndicationCallback_t  callback)
{
    pfGattClientIndCallback = callback;

    return GattClient_RegisterIndicationCallback(App_GattClientIndicationCallback);
}

bleResult_t App_RegisterLeCbCallbacks
(
    l2caLeCbDataCallback_t      pCallback,
    l2caLeCbControlCallback_t   pCtrlCallback
)
{
    pfL2caLeCbDataCallback = pCallback;
    pfL2caLeCbControlCallback = pCtrlCallback;

    return L2ca_RegisterLeCbCallbacks(App_L2caLeDataCallback, App_L2caLeControlCallback);
}

bleResult_t App_PostCallbackMessage
(
    appCallbackHandler_t   handler,
    appCallbackParam_t     param
)
{
    appMsgCallback_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc(sizeof (appMsgCallback_t));

    if (!pMsgIn)
    {
        return gBleOutOfMemory_c;
    }

    pMsgIn->handler = handler;
    pMsgIn->param = param;

    /* Put message in the Cb App queue */
    MSG_Queue(&mAppCbInputQueue, pMsgIn);

    return gBleSuccess_c;
}

void App_NvmWrite
(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
)
{
    if(mEntryIdx >= gMaxBondedDevices_c)
    {
        return;
    }
#if gAppUseNvm_d  
    uint8_t  idx   = 0;
    uint32_t mSize = 0;
    void**   ppNvmData = NULL;;
    void*    pRamData = NULL;
    

    for(idx = 0; idx < 5; idx++)
    {
        ppNvmData = NULL;
        switch(idx)
        {
        case 0:
            if(pBondHeader != NULL)
            {
                ppNvmData = (void**)&aBondingHeader[mEntryIdx];
                pRamData  = pBondHeader;
                mSize     = gBleBondIdentityHeaderSize_c;
            }
            break;
        case 1:
            if(pBondDataDynamic != NULL)
            {
                ppNvmData = (void**)&aBondingDataDynamic[mEntryIdx];
                pRamData  = pBondDataDynamic;;
                mSize     = gBleBondDataDynamicSize_c;
            }
            break;
        case 2:
            if(pBondDataStatic != NULL)
            {
                ppNvmData = (void**)&aBondingDataStatic[mEntryIdx];
                pRamData  = pBondDataStatic;
                mSize     = gBleBondDataStaticSize_c;
            }
            break;
        case 3:
            if(pBondDataDeviceInfo != NULL)
            {
                ppNvmData = (void**)&aBondingDataDeviceInfo[mEntryIdx];
                pRamData  = pBondDataDeviceInfo;
                mSize     = gBleBondDataDeviceInfoSize_c;
            }
            break;
        case 4:
            if(pBondDataDescriptor != NULL)
            {
                if(mDescriptorIndex < gcGapMaximumSavedCccds_c)
                {
                    ppNvmData = (void**)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex];
                    pRamData  = pBondDataDescriptor;
                    mSize     = gBleBondDataDescriptorSize_c;
                }
            }
            break;
        default:
            break;
        }
        if(ppNvmData != NULL)
        {
            if(gNVM_OK_c == NvMoveToRam(ppNvmData))
            {
                FLib_MemCpy(*ppNvmData, pRamData, mSize);
                NvSaveOnIdle(ppNvmData, FALSE);
            }
        }
    }
#else
    
    if(pBondHeader != NULL)
    {
        FLib_MemCpy(&maBondDataBlobs[mEntryIdx].bondHeader, pBondHeader, gBleBondIdentityHeaderSize_c);
    }
    
    if(pBondDataDynamic != NULL)
    {
        FLib_MemCpy((uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDynamic,
                    pBondDataDynamic,
                    gBleBondDataDynamicSize_c
                        );
    }
    
    if(pBondDataStatic != NULL)
    {
        FLib_MemCpy((uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobStatic,
                    pBondDataStatic,
                    gBleBondDataStaticSize_c
                        );
    }
    
    if(pBondDataDeviceInfo != NULL)
    {
        FLib_MemCpy((uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDeviceInfo,
                    pBondDataDeviceInfo,
                    gBleBondDataDeviceInfoSize_c
                        );
    }
    
    if(pBondDataDescriptor != NULL && mDescriptorIndex != gcGapMaximumSavedCccds_c)
    {
        FLib_MemCpy((uint8_t*)&(maBondDataBlobs[mEntryIdx].bondDataDescriptors[mDescriptorIndex]),
                    pBondDataDescriptor,
                    gBleBondDataDescriptorSize_c
                        );
    }
    
#endif
}

void App_NvmRead
(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
)
{
    if(mEntryIdx >= gMaxBondedDevices_c)
    {
        return;
    }
#if gAppUseNvm_d  
    uint8_t  idx = 0;
    uint32_t mSize = 0;
    void**   ppNvmData = NULL;;
    void*    pRamData = NULL;
    
    for(idx = 0; idx < 5; idx++)
    {
        ppNvmData = NULL;
        switch(idx)
        {
        case 0:
            if(pBondHeader != NULL)
            {
                ppNvmData = (void**)&aBondingHeader[mEntryIdx];
                pRamData  = pBondHeader;
                mSize     = gBleBondIdentityHeaderSize_c;
            }
            break;
        case 1:
            if(pBondDataDynamic != NULL)
            {
                ppNvmData = (void**)&aBondingDataDynamic[mEntryIdx];
                pRamData  = pBondDataDynamic;;
                mSize     = gBleBondDataDynamicSize_c;
            }
            break;
        case 2:
            if(pBondDataStatic != NULL)
            {
                ppNvmData = (void**)&aBondingDataStatic[mEntryIdx];
                pRamData  = pBondDataStatic;
                mSize     = gBleBondDataStaticSize_c;
            }
            break;
        case 3:
            if(pBondDataDeviceInfo != NULL)
            {
                ppNvmData = (void**)&aBondingDataDeviceInfo[mEntryIdx];
                pRamData  = pBondDataDeviceInfo;
                mSize     = gBleBondDataDeviceInfoSize_c;
            }
            break;
        case 4:
            if(pBondDataDescriptor != NULL)
            {
                if(mDescriptorIndex < gcGapMaximumSavedCccds_c)
                {
                    ppNvmData = (void**)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex];
                    pRamData  = pBondDataDescriptor;
                    mSize     = gBleBondDataDescriptorSize_c;
                }
            }
            break;
        default:
            break;
        }
        if(ppNvmData != NULL)
        {
            if(*ppNvmData != NULL && pRamData != NULL)
            {
                FLib_MemCpy(pRamData, *ppNvmData, mSize);
            }
        }
    }
#else
    
    if(pBondHeader != NULL)
    {
        FLib_MemCpy(pBondHeader, &maBondDataBlobs[mEntryIdx].bondHeader, gBleBondIdentityHeaderSize_c);
    }
    
    if(pBondDataDynamic != NULL)
    {
        FLib_MemCpy(pBondDataDynamic,
                    (uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDynamic,
                    gBleBondDataDynamicSize_c
                        );
    }
   
    if(pBondDataStatic != NULL)
    {
        FLib_MemCpy(pBondDataStatic,
                    (uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobStatic,
                    gBleBondDataStaticSize_c
                        );
    }
    
    if(pBondDataDeviceInfo != NULL)
    {
        FLib_MemCpy(pBondDataDeviceInfo,
                    (uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDeviceInfo,
                    gBleBondDataDeviceInfoSize_c
                        );
    }
    
    if(pBondDataDescriptor != NULL && mDescriptorIndex < gcGapMaximumSavedCccds_c)
    {
        FLib_MemCpy(pBondDataDescriptor,
                    (uint8_t*)&(maBondDataBlobs[mEntryIdx].bondDataDescriptors[mDescriptorIndex]),
                    gBleBondDataDescriptorSize_c
                        );
    }
        
#endif
}

/*****************************************************************************
* Handles all messages received from the host task.
* Interface assumptions: None
* Return value: None
*****************************************************************************/
static void App_HandleHostMessageInput(appMsgFromHost_t* pMsg)
{
    switch ( pMsg->msgType ) 
    { 
        case gAppGapGenericMsg_c:
        {
            if (pfGenericCallback)
                pfGenericCallback(&pMsg->msgData.genericMsg);
            else
                BleApp_GenericCallback(&pMsg->msgData.genericMsg);
            break;
        }
        case gAppGapAdvertisementMsg_c:
        {
            if (pfAdvCallback)
                pfAdvCallback(&pMsg->msgData.advMsg);
            break;
        }
        case gAppGapScanMsg_c:
        {
            if (pfScanCallback)
                pfScanCallback(&pMsg->msgData.scanMsg);
            break;
        }
        case gAppGapConnectionMsg_c:
        {
            if (pfConnCallback)
                pfConnCallback(pMsg->msgData.connMsg.deviceId, &pMsg->msgData.connMsg.connEvent);
            break;
        }
        case gAppGattServerMsg_c:
        {
            if (pfGattServerCallback)
                pfGattServerCallback(pMsg->msgData.gattServerMsg.deviceId, &pMsg->msgData.gattServerMsg.serverEvent);
            break;
        }
        case gAppGattClientProcedureMsg_c:
        {
            if (pfGattClientProcCallback)
                pfGattClientProcCallback(
                    pMsg->msgData.gattClientProcMsg.deviceId,
                    pMsg->msgData.gattClientProcMsg.procedureType,
                    pMsg->msgData.gattClientProcMsg.procedureResult,
                    pMsg->msgData.gattClientProcMsg.error);
            break;
        }
        case gAppGattClientNotificationMsg_c:
        {
            if (pfGattClientNotifCallback)
                pfGattClientNotifCallback(
                    pMsg->msgData.gattClientNotifIndMsg.deviceId,
                    pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle,
                    pMsg->msgData.gattClientNotifIndMsg.aValue,
                    pMsg->msgData.gattClientNotifIndMsg.valueLength);
            break;
        }
        case gAppGattClientIndicationMsg_c:
        {
            if (pfGattClientIndCallback)
                pfGattClientIndCallback(
                    pMsg->msgData.gattClientNotifIndMsg.deviceId,
                    pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle,
                    pMsg->msgData.gattClientNotifIndMsg.aValue,
                    pMsg->msgData.gattClientNotifIndMsg.valueLength);
            break;
        }
        case gAppL2caLeDataMsg_c:
        {
            if (pfL2caLeCbDataCallback)
                pfL2caLeCbDataCallback(
                    pMsg->msgData.l2caLeCbDataMsg.deviceId,
                    pMsg->msgData.l2caLeCbDataMsg.lePsm,                    
                    pMsg->msgData.l2caLeCbDataMsg.aPacket,
                    pMsg->msgData.l2caLeCbDataMsg.packetLength);
            break;
        }
        case gAppL2caLeControlMsg_c:
        {
            if (pfL2caLeCbControlCallback)
                pfL2caLeCbControlCallback(
                    pMsg->msgData.l2caLeCbControlMsg.messageType,
                    pMsg->msgData.l2caLeCbControlMsg.aMessage);
            break;
        }
        default:
        {
            break;
        }
    }   
}

static void App_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;   
    
    pMsgIn = MSG_Alloc(GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gapGenericEvent_t));
          
    if (!pMsgIn)
      return;
    
    pMsgIn->msgType = gAppGapGenericMsg_c;
    FLib_MemCpy(&pMsgIn->msgData.genericMsg, pGenericEvent, sizeof(gapGenericEvent_t));

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;   
    uint8_t msgLen = GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gapConnectionEvent_t);
    
    if(pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        gapSmpKeys_t    *pKeys = pConnectionEvent->eventData.keysReceivedEvent.pKeys;

        /* Take into account alignment */
        msgLen = GetRelAddr(appMsgFromHost_t, msgData) + GetRelAddr(connectionMsg_t, connEvent) + 
                 GetRelAddr(gapConnectionEvent_t, eventData) + sizeof(gapKeysReceivedEvent_t) + sizeof(gapSmpKeys_t);
                 
        if (pKeys->aLtk != NULL)
        {
            msgLen += 2 * sizeof(uint8_t) + pKeys->cLtkSize + pKeys->cRandSize;
        }

        msgLen += (pKeys->aIrk != NULL) ? (gcSmpIrkSize_c + gcBleDeviceAddressSize_c) : 0;
        msgLen += (pKeys->aCsrk != NULL) ? gcSmpCsrkSize_c : 0;
    }

    pMsgIn = MSG_Alloc(msgLen);
          
    if (!pMsgIn)
      return;
    
    pMsgIn->msgType = gAppGapConnectionMsg_c;
    pMsgIn->msgData.connMsg.deviceId = peerDeviceId;

    if(pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        gapSmpKeys_t    *pKeys = pConnectionEvent->eventData.keysReceivedEvent.pKeys;
        uint8_t         *pCursor = (uint8_t*)&pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys;

        pMsgIn->msgData.connMsg.connEvent.eventType = gConnEvtKeysReceived_c;
        pCursor += sizeof(void*);
        pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys = (gapSmpKeys_t* )pCursor;
        
        /* Copy SMP Keys structure */
        FLib_MemCpy(pCursor, pConnectionEvent->eventData.keysReceivedEvent.pKeys, sizeof(gapSmpKeys_t));
        pCursor += sizeof(gapSmpKeys_t);
        
        if (pKeys->aLtk != NULL)
        {
            /* Copy LTK */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->cLtkSize = pKeys->cLtkSize;
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aLtk = pCursor;
            FLib_MemCpy(pCursor, pKeys->aLtk, pKeys->cLtkSize);
            pCursor += pKeys->cLtkSize;

            /* Copy RAND */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->cRandSize = pKeys->cRandSize;
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aRand = pCursor;
            FLib_MemCpy(pCursor, pKeys->aRand, pKeys->cRandSize);
            pCursor += pKeys->cRandSize;
        }
        else
          

        if (pKeys->aIrk != NULL)
        {
            /* Copy IRK */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aIrk = pCursor;
            FLib_MemCpy(pCursor, pKeys->aIrk, gcSmpIrkSize_c);
            pCursor += gcSmpIrkSize_c;

            /* Copy Address*/
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->addressType = pKeys->addressType;
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aAddress = pCursor;
            FLib_MemCpy(pCursor, pKeys->aAddress, gcBleDeviceAddressSize_c);
            pCursor += gcBleDeviceAddressSize_c;
        }

        if (pKeys->aCsrk != NULL)
        {
            /* Copy CSRK */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aCsrk = pCursor;
            FLib_MemCpy(pCursor, pKeys->aCsrk, gcSmpCsrkSize_c);
        }
    }
    else
    {
        FLib_MemCpy(&pMsgIn->msgData.connMsg.connEvent, pConnectionEvent, sizeof(gapConnectionEvent_t));
    }

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;   
    
    pMsgIn = MSG_Alloc(GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gapAdvertisingEvent_t));
          
    if (!pMsgIn)
      return;
    
    pMsgIn->msgType = gAppGapAdvertisementMsg_c;
    pMsgIn->msgData.advMsg.eventType = pAdvertisingEvent->eventType;
    pMsgIn->msgData.advMsg.eventData = pAdvertisingEvent->eventData;

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;  
    
    uint8_t msgLen = GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gapScanningEvent_t);
    
    if (pScanningEvent->eventType == gDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.scannedDevice.dataLength;
    }
    
    pMsgIn = MSG_Alloc(msgLen);
          
    if (!pMsgIn)
      return;
    
    pMsgIn->msgType = gAppGapScanMsg_c;
    pMsgIn->msgData.scanMsg.eventType = pScanningEvent->eventType;
    
    if (pScanningEvent->eventType == gScanCommandFailed_c)
    {
        pMsgIn->msgData.scanMsg.eventData.failReason = pScanningEvent->eventData.failReason;
    }
    
    if (pScanningEvent->eventType == gDeviceScanned_c)
    {
        FLib_MemCpy(&pMsgIn->msgData.scanMsg.eventData.scannedDevice, 
                    &pScanningEvent->eventData.scannedDevice, 
                    sizeof(gapScanningEvent_t));
        
        /* Copy data after the gapScanningEvent_t structure and update the data pointer*/
        pMsgIn->msgData.scanMsg.eventData.scannedDevice.data = (uint8_t*)&pMsgIn->msgData + sizeof(gapScanningEvent_t);
        FLib_MemCpy(pMsgIn->msgData.scanMsg.eventData.scannedDevice.data, 
                    pScanningEvent->eventData.scannedDevice.data, 
                    pScanningEvent->eventData.scannedDevice.dataLength);
    }

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_GattServerCallback
(
    deviceId_t          deviceId,
    gattServerEvent_t*  pServerEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;   
    uint16_t msgLen = GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gattServerMsg_t);
    
    if (pServerEvent->eventType == gEvtAttributeWritten_c ||
        pServerEvent->eventType == gEvtAttributeWrittenWithoutResponse_c)
    {
        msgLen += pServerEvent->eventData.attributeWrittenEvent.cValueLength;
    }

    pMsgIn = MSG_Alloc(msgLen);
          
    if (!pMsgIn)
      return;
    
    pMsgIn->msgType = gAppGattServerMsg_c;
    pMsgIn->msgData.gattServerMsg.deviceId = deviceId;
    FLib_MemCpy(&pMsgIn->msgData.gattServerMsg.serverEvent, pServerEvent, sizeof(gattServerEvent_t));

    if ((pMsgIn->msgData.gattServerMsg.serverEvent.eventType == gEvtAttributeWritten_c) ||
        (pMsgIn->msgData.gattServerMsg.serverEvent.eventType == gEvtAttributeWrittenWithoutResponse_c))
    {
        /* Copy value after the gattServerEvent_t structure and update the aValue pointer*/
        pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue = 
          (uint8_t *)&pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue + sizeof(uint8_t*);
        FLib_MemCpy(pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.cValueLength);

    }
    
    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_GattClientProcedureCallback
(
    deviceId_t              deviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error)
{
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc(GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gattClientProcMsg_t));

    if (!pMsgIn)
      return;

    pMsgIn->msgType = gAppGattClientProcedureMsg_c;
    pMsgIn->msgData.gattClientProcMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientProcMsg.procedureType = procedureType;
    pMsgIn->msgData.gattClientProcMsg.error = error;
    pMsgIn->msgData.gattClientProcMsg.procedureResult = procedureResult;

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_GattClientNotificationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc(GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gattClientNotifIndMsg_t)
                        + valueLength);

    if (!pMsgIn)
      return;

    pMsgIn->msgType = gAppGattClientNotificationMsg_c;
    pMsgIn->msgData.gattClientNotifIndMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientNotifIndMsg.characteristicValueHandle = characteristicValueHandle;
    pMsgIn->msgData.gattClientNotifIndMsg.valueLength = valueLength;

    /* Copy value after the gattClientNotifIndMsg_t structure and update the aValue pointer*/
    pMsgIn->msgData.gattClientNotifIndMsg.aValue = (uint8_t*)&pMsgIn->msgData + sizeof(gattClientNotifIndMsg_t);
    FLib_MemCpy(pMsgIn->msgData.gattClientNotifIndMsg.aValue, aValue, valueLength);

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_GattClientIndicationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc(GetRelAddr(appMsgFromHost_t, msgData) + sizeof(gattClientNotifIndMsg_t)
                        + valueLength);

    if (!pMsgIn)
      return;

    pMsgIn->msgType = gAppGattClientIndicationMsg_c;
    pMsgIn->msgData.gattClientNotifIndMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientNotifIndMsg.characteristicValueHandle = characteristicValueHandle;
    pMsgIn->msgData.gattClientNotifIndMsg.valueLength = valueLength;

    /* Copy value after the gattClientIndIndMsg_t structure and update the aValue pointer*/
    pMsgIn->msgData.gattClientNotifIndMsg.aValue = (uint8_t*)&pMsgIn->msgData + sizeof(gattClientNotifIndMsg_t);
    FLib_MemCpy(pMsgIn->msgData.gattClientNotifIndMsg.aValue, aValue, valueLength);

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_L2caLeDataCallback
(
    deviceId_t deviceId,
    uint16_t   lePsm,
    uint8_t* pPacket,
    uint16_t packetLength
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc(GetRelAddr(appMsgFromHost_t, msgData) + sizeof(l2caLeCbDataMsg_t) 
                        + packetLength);

    if (!pMsgIn)
    {
        return;
    }

    pMsgIn->msgType = gAppL2caLeDataMsg_c;
    pMsgIn->msgData.l2caLeCbDataMsg.deviceId = deviceId;
    pMsgIn->msgData.l2caLeCbDataMsg.lePsm = lePsm;
    pMsgIn->msgData.l2caLeCbDataMsg.packetLength = packetLength;

    FLib_MemCpy(pMsgIn->msgData.l2caLeCbDataMsg.aPacket, pPacket, packetLength);

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

static void App_L2caLeControlCallback
(
    l2capControlMessageType_t  messageType,
    void* pMessage
)
{
    appMsgFromHost_t *pMsgIn = NULL;
    uint8_t messageLength = 0;

    switch (messageType) {
        case gL2ca_LePsmConnectRequest_c:
        {
            messageLength = sizeof(l2caLeCbConnectionRequest_t);
            break;
        }
        case gL2ca_LePsmConnectionComplete_c:
        {
            messageLength = sizeof(l2caLeCbConnectionComplete_t);
            break;
        }
        case gL2ca_LePsmDisconnectNotification_c:
        {
            messageLength = sizeof(l2caLeCbDisconnection_t);
            break;
        }
        case gL2ca_NoPeerCredits_c:
        {
            messageLength = sizeof(l2caLeCbNoPeerCredits_t);
            break;
        }
        case gL2ca_LocalCreditsNotification_c:
        {
            messageLength = sizeof(l2caLeCbLocalCreditsNotification_t);
            break;
        }
        default:
            return;
    }


    /* Allocate a buffer with enough space to store the biggest packet */
    pMsgIn = MSG_Alloc(GetRelAddr(appMsgFromHost_t, msgData) + sizeof(l2caLeCbConnectionComplete_t));

    if (!pMsgIn)
    {
        return;
    }

    pMsgIn->msgType = gAppL2caLeControlMsg_c;
    pMsgIn->msgData.l2caLeCbControlMsg.messageType = messageType;
    
    FLib_MemCpy(pMsgIn->msgData.l2caLeCbControlMsg.aMessage, pMessage, messageLength);

    /* Put message in the Host Stack to App queue */
    MSG_Queue(&mHostAppInputQueue, pMsgIn);
}

#if !gUseHciTransportDownward_d
/* Called by BLE when a connect is received */
static void BLE_SignalFromISRCallback(void)
{
#if (cPWR_UsePowerDownMode)  
    PWR_DisallowDeviceToSleep();  
#endif    
}
#endif

/*! *********************************************************************************
* \brief  This function represents the Application task. 
*         This task reuses the stack alocated for the MainThread.
*         This function is called to process all events for the task. Events 
*         include timers, messages and any other user defined events.
* \param[in]  argument
*
********************************************************************************** */
void BleApp_Task (uint32_t param)
{
    /* Pointer for storing the messages from host. */
    appMsgFromHost_t *pHostMsgIn = NULL;
    /* Pointer for storing the callback messages. */
    appMsgCallback_t *pCbMsgIn = NULL;  
    
    /* Check for existing messages in queue */
    while(MSG_Pending(&mHostAppInputQueue))
    {
        pHostMsgIn = MSG_DeQueue(&mHostAppInputQueue);
        
        if (pHostMsgIn)
        {
            /* Process it */
            App_HandleHostMessageInput(pHostMsgIn);
            
            /* Messages must always be freed. */
            MSG_Free(pHostMsgIn);
            pHostMsgIn = NULL;
        }
    }
    
    /* Dequeue the callback message */
    /* Check for existing messages in queue */
    while(MSG_Pending(&mAppCbInputQueue))
    {
        pCbMsgIn = MSG_DeQueue(&mAppCbInputQueue);
        
        if (pCbMsgIn)
        {
            /* Execute callback handler */
            if (pCbMsgIn->handler)
            {
                pCbMsgIn->handler (pCbMsgIn->param);
            }
            
            /* Messages must always be freed. */
            MSG_Free(pCbMsgIn);
            pCbMsgIn = NULL;
        }
    }
}
extern void ResetMCU(void);
PRIVATE void KBD_Callback( uint8_t events)
{
    uint32_t buff[4];

    switch(events)
    {
        case gKBD_EventPB1_c:                   
          RNG_GetRandomNo(&buff[0]);          
          RNG_GetRandomNo(&buff[1]);
          RNG_GetRandomNo(&buff[2]);
          RNG_GetRandomNo(&buff[3]);
          /* Updated UUID value from advertising data with the random value */
          FLib_MemCpy(&gAppAdvertisingData.aAdStructures[1].aData[3], buff, 16);
          Gap_StopAdvertising();
          /* Setup Advertising data */
          Gap_SetAdvertisingData(&gAppAdvertisingData, NULL);
          if(pfAdvCallback){
            App_StartAdvertising(pfAdvCallback, NULL);
          }
          break;
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
bool_t APP_GetByteFromSerial(uint8 * byte)
{
    uint16_t len = 0;
    
    Serial_GetByteFromRxBuffer(u8SerialId, byte, &len);
    
    return (bool_t)len;
}

/****************************************************************************
 *
 * NAME: App_NvmErase
 *
 * DESCRIPTION:
 * Erase NVM entry
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void App_NvmErase(uint8_t mEntryIdx)
{
    (void)mEntryIdx;
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
