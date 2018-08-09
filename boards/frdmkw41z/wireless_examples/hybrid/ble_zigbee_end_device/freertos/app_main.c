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
\file       app_main.c
\brief      app main file
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <stdint.h>
#include "jendefs.h"
#include "app_cfg.h"

#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "board.h"
#include "PWR_Interface.h"
#include "NVM_Interface.h"
#include "RNG_Interface.h"
#include "TimersManager.h"
#include "SerialManager.h"
#include "Keyboard.h"
#include "LED.h"
#include "SecLib.h"
#include "MemManager.h"
#include "Messaging.h"
#include "Panic.h"

#include "fsl_gpio.h"
#include "pin_mux.h"
#include "ZQueue.h"
#include "ZTimer.h"
#include "portmacro.h"
#include "zps_apl_af.h"
#include "mac_vs_sap.h"
#include "dbg.h"
#include "app_main.h"
#include "app_buttons.h"
#include "app_events.h"
#include "app_zcl_task.h"
#include "PDM.h"
#include "app_end_device_node.h"
#ifdef APP_NTAG_ICODE
#include "app_ntag_icode.h"
#endif

#ifdef APP_NTAG_AES
#include "app_ntag_aes.h"
#endif
#include "pwrm.h"
#include "app_zb_shell.h"

/* Bluetooth Low Energy */
#include "ble_init.h"
#include "ble_conn_manager.h"
#include "ble_service_discovery.h"
#include "ApplMain.h"

#include "gatt_db_handles.h"
#include "board.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef DEBUG_APP
#define TRACE_APP   FALSE
#else
#define TRACE_APP   TRUE
#endif

#define APP_QUEUE_SIZE               8
#define BDB_QUEUE_SIZE               2
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
#define APP_ZTIMER_STORAGE           5 /* NTAG: Added timer */
#else
#define APP_ZTIMER_STORAGE           3
#endif
#define TIMER_QUEUE_SIZE             8
#define MLME_QUEQUE_SIZE             8
#define MCPS_QUEUE_SIZE             20


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void KBD_Callback( uint8_t events);

void BleApp_Init(void);
void BleApp_Start (gapRole_t mGapStartRole);
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent);

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback(gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent);
static void BleApp_ConnectionCallback(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback(deviceId_t deviceId, gattServerEvent_t* pServerEvent);
static void BleApp_GattClientCallback(deviceId_t serverDeviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error);
static void BleApp_ServiceDiscoveryCallback(deviceId_t deviceId, servDiscEvent_t* pEvent);
static void BleApp_Config (void);
static void BleApp_Advertise (void);
void BleApp_StateMachineHandler(deviceId_t peerDeviceId, uint8_t event);
static void BleApp_StoreServiceHandles(deviceId_t peerDeviceId, gattService_t *pService);
static bool_t BleApp_CheckScanEvent (gapScannedDevice_t* pData);

/* Timer Callbacks */
static void ScaningTimerCallback (void *);
static void UartStreamFlushTimerCallback(void *);

static void BleApp_FlushUartStream(void *pParam);
static void BleApp_ReceivedUartStream(uint8_t *pStream, uint16_t streamLength);
static void BleApp_SendUartStream(deviceId_t deviceId, uint8_t *pRecvStream, uint16_t streamSize);
static void BleApp_GenerateAndSetRandomAddress(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
PUBLIC uint8 u8TimerPoll;
PUBLIC uint8 u8TimerId;
PUBLIC uint8 u8TimerZCL;
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
PUBLIC uint8 u8TimerNtag;
#endif

PUBLIC tszQueue APP_msgBdbEvents;
PUBLIC tszQueue APP_msgAppEvents;
extern uint8_t  gShellSerMgrIf;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE ZTIMER_tsTimer asTimers[APP_ZTIMER_STORAGE + BDB_ZTIMER_STORAGE];
extern const uint8_t gUseRtos_c;

static volatile uint8_t wdt_update_count;


gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput, 0,
};

/* Application input queues */
extern anchor_t mHostAppInputQueue;
extern anchor_t mAppCbInputQueue;

static appPeerInfo_t mPeerInformation;
static gapRole_t mGapRole;
#if gAppUseBonding_d
static bool_t mRestoringBondedLink = FALSE;
#endif
/* Adv Parmeters */
static advState_t mAdvState;
static bool_t   mScanningOn = FALSE;

static uint16_t mCharMonitoredHandles[1] = {value_uart_stream};

/* Service Data*/
static wusConfig_t mWuServiceConfig;

static tmrTimerID_t mBleAppTimerId;
static tmrTimerID_t mUartStreamFlushTimerId;

static uint8_t  mpSendOnBleDataBuffer[mAppUartBufferSize_c] = {0};
static uint16_t mOnBleDataBufferIndex = 0;

/* Local Buffer Size */
static uint16_t mAppUartBufferSize = mAppUartBufferSize_c; 

static uint8_t mDevRandomAddress[] = {0x00,0x01,0x02,0x03,0x04,0x05};

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern void vTimerServerInit(void);
extern void zps_taskZPS(void);
extern void vAppMain(void);
PRIVATE void APP_vSetUpWdog(void);

#if !gUseHciTransportDownward_d
extern void (*pfBLE_SignalFromISR)(void);
static void BLE_SignalFromISRCallback(void);
#endif /* gUseHciTransportDownward_d */

/****************************************************************************
 *
 * NAME: main_task
 *
 * DESCRIPTION:
 * Main  execution loop
 *
 * RETURNS:
 * Never
 *
 ****************************************************************************/

void main_task (uint32_t parameter)
{
    static uint8_t initialized = FALSE;

    if(!initialized)
    {
        initialized = TRUE;

        vTimerServerInit();

        /* Init memory blocks manager */
        MEM_Init();
        RNG_Init();
        SecLib_Init();
        
        vAppRegisterPWRMCallbacks();
        
        /* Init  timers module */
        TMR_Init();
        SerialManager_Init();
        /* Init Led module */
        LED_Init();
        /* Initialize Keyboard (Switches) Module */
        KBD_Init(KBD_Callback);

        /* Wdog/Cop Setup */
        APP_vSetUpWdog();
#if (cPWR_UsePowerDownMode == 0)
        /* Initialize ble and peripheral drivers specific to the ble application */
        BleApp_Init();

        app_zb_shell_init();
#endif
        vAppMain();

    }

    while(1)
    {
         /* place event handler code here... */
        DBG_vPrintf(FALSE, "APP: Entering zps_taskZPS\n");
        zps_taskZPS();

        DBG_vPrintf(FALSE, "APP: Entering bdb_taskBDB\n");
        bdb_taskBDB();

        DBG_vPrintf(FALSE, "APP: Entering ZTIMER_vTask\n");
        ZTIMER_vTask();

        DBG_vPrintf(FALSE, "APP: Entering APP_taskEndDevice\n");
        APP_taskEndDevice();
#if (cPWR_UsePowerDownMode == 0)
        BleApp_Thread(0);

        app_zb_shell_task();
#endif
        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed with in the watchdog period
         * otherwise the system will be reset.
         */

         /* Kick the watchdog */
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
        DBG_vPrintf(TRACE_APP, "APP: Watchdog timer has reset device!\r\n");
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
PUBLIC void APP_vInitResources(void)
{
    /* Initialise the Z timer module */
    ZTIMER_eInit(asTimers, sizeof(asTimers) / sizeof(ZTIMER_tsTimer));

    /* Create Z timers */
    ZTIMER_eOpen(&u8TimerPoll,          APP_cbTimerPoll,        NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerId,            APP_cbTimerId,          NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerZCL,           APP_cbTimerZclTick,     NULL, ZTIMER_FLAG_PREVENT_SLEEP);
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    ZTIMER_eOpen(&u8TimerNtag,          APP_cbNtagTimer,        NULL, ZTIMER_FLAG_PREVENT_SLEEP);
#endif

    /* Create all the queues */
#ifdef CPU_MKW41Z512VHT4
    ZQ_vQueueCreate(&APP_msgAppEvents,     APP_QUEUE_SIZE,     sizeof(APP_tsEvent),        NULL );
    ZQ_vQueueCreate(&APP_msgBdbEvents,     BDB_QUEUE_SIZE,     sizeof(BDB_tsZpsAfEvent),   NULL );
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,   MLME_QUEQUE_SIZE,   sizeof(MAC_tsMlmeVsDcfmInd),NULL );
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,   MCPS_QUEUE_SIZE,    sizeof(MAC_tsMcpsVsDcfmInd),NULL );
    ZQ_vQueueCreate(&zps_TimeEvents,       TIMER_QUEUE_SIZE,   sizeof(zps_tsTimeEvent),    NULL );
#else
    ZQ_vQueueCreate(&APP_msgAppEvents,     APP_QUEUE_SIZE,     sizeof(APP_tsEvent),        (uint8*)asAppEvent);
    ZQ_vQueueCreate(&APP_msgBdbEvents,     BDB_QUEUE_SIZE,     sizeof(BDB_tsZpsAfEvent),   (uint8*)asBdbEvent);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,   MLME_QUEQUE_SIZE,   sizeof(MAC_tsMlmeVsDcfmInd),(uint8*)asMacMlmeVsDcfmInd);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,   MCPS_QUEUE_SIZE,    sizeof(MAC_tsMcpsVsDcfmInd),(uint8*)asMacMcpsDcfmInd);
    ZQ_vQueueCreate(&zps_TimeEvents,       TIMER_QUEUE_SIZE,   sizeof(zps_tsTimeEvent),    (uint8*)asTimeEvent);
#endif
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

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
PRIVATE void KBD_Callback( uint8_t events)
{
    APP_tsEvent sButtonEvent;

    switch(events)
    {
        case gKBD_EventPB1_c:
            /* SW5 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW1;
            break;
        case gKBD_EventPB2_c:
            /* SW4 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW2;
            break;
        case gKBD_EventPB3_c:
            /* SW3 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW3;
            break;
        case gKBD_EventPB4_c:
            /* SW2 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW4;
            break;
        case gKBD_EventLongPB1_c:
        case gKBD_EventLongPB2_c:
        case gKBD_EventLongPB3_c:
        case gKBD_EventLongPB4_c:
            return;
        case gKBD_EventVeryLongPB1_c: /* factory reset */
        case gKBD_EventVeryLongPB2_c:
        case gKBD_EventVeryLongPB3_c:
        case gKBD_EventVeryLongPB4_c:
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_1;
            break;
        default:
            return;
    }

    sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;

    DBG_vPrintf(TRACE_APP, "Button DN=%d\n", sButtonEvent.uEvent.sButton.u8Button);

    if(ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent) == FALSE)
    {
        DBG_vPrintf(TRACE_APP, "Button: Failed to post Event %d \n", sButtonEvent.eType);
    }
}
/*******************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
    uint32_t pseudoRNGSeed[5] = {0};

    RNG_GetRandomNo(&pseudoRNGSeed[0]);
    RNG_GetRandomNo(&pseudoRNGSeed[1]);
    RNG_GetRandomNo(&pseudoRNGSeed[2]);
    RNG_GetRandomNo(&pseudoRNGSeed[3]);
    RNG_GetRandomNo(&pseudoRNGSeed[4]);
    RNG_SetPseudoRandomNoSeed((uint8_t*)pseudoRNGSeed);
   
        
#if !gUseHciTransportDownward_d
    pfBLE_SignalFromISR = BLE_SignalFromISRCallback;        
#endif /* !gUseHciTransportDownward_d */
    
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
}

/*! *********************************************************************************
 * \brief    Starts the BLE application.
 *
 * \param[in]    mGapRole    GAP Start Role (Central or Peripheral).
 ********************************************************************************** */
void BleApp_Start (gapRole_t mGapRole)
{
    switch (mGapRole)
    {
        case gGapCentral_c:
        {
            gPairingParameters.localIoCapabilities = gIoKeyboardDisplay_c;
            App_StartScanning(&gScanParams, BleApp_ScanningCallback, TRUE);
            break;
        }
        case gGapPeripheral_c:
        {
            gPairingParameters.localIoCapabilities = gIoDisplayOnly_c;
            BleApp_Advertise();
            break;
        }
        default:
            break;
    }
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
* \brief        Configures BLE Stack after initialization. Usually used for
*               configuring advertising, scanning, white list, services, et al.
*
********************************************************************************** */
static void BleApp_Config()
{
    /* Perform common configurations */
    BleConnManager_GapPeripheralConfig();
    
    /* Register for callbacks*/
    App_RegisterGattServerCallback(BleApp_GattServerCallback);
    App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    GattServer_RegisterHandlesForWriteNotifications(NumberOfElements((mCharMonitoredHandles)), mCharMonitoredHandles);
    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);

    /* Setup Advertising and scanning data */
    Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);

    mAdvState.advOn = FALSE;
    mPeerInformation.appState = mAppIdle_c;
    mScanningOn = FALSE;
    
    mGapRole = gGapPeripheral_c;

    /* Start services */
    Wus_Start(&mWuServiceConfig);

    /* Allocate application timer */
    mBleAppTimerId = TMR_AllocateTimer();
    mUartStreamFlushTimerId = TMR_AllocateTimer();
    
    if((gTmrInvalidTimerID_c == mBleAppTimerId)||
       (gTmrInvalidTimerID_c == mUartStreamFlushTimerId))
    {
        panic(0, 0, 0, 0);
    }
}

/*! *********************************************************************************
* \brief        Handles BLE generic callback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    /* Call BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);
    
    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:    
            BleApp_Config();
            BleApp_Start(mGapRole);
            break;  
        case gPublicAddressRead_c:
            /* generate and set random static address */
            BleApp_GenerateAndSetRandomAddress();
            break; 
        case gRandomAddressSet_c:
#if gAppUsePairing_d            
            gSmpKeys.addressType = gBleAddrTypeRandom_c;
            gSmpKeys.aAddress = mDevRandomAddress;
#endif 
            break; 
        case gAdvertisingParametersSetupComplete_c:
            App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
            break;         
        
        case gAdvertisingSetupFailed_c:
        case gInternalError_c:
            //panic(0,0,0,0);
            break;
        default: 
            break;
    }
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will satrt after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    /* Set advertising parameters*/
    Gap_SetAdvertisingParameters(&gAdvParams);
}

/*! *********************************************************************************
 * \brief        Handles BLE Scanning callback from host stack.
 *
 * \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
 ********************************************************************************** */
static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
    switch (pScanningEvent->eventType)
    {
        case gDeviceScanned_c:
        {
            if (BleApp_CheckScanEvent(&pScanningEvent->eventData.scannedDevice))
            {        
                gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                FLib_MemCpy(gConnReqParams.peerAddress, 
                            pScanningEvent->eventData.scannedDevice.aAddress,
                            sizeof(bleDeviceAddress_t));
                
                Gap_StopScanning();
                App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
            }
        }        
        break;
        
        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;
            
            /* Node starts scanning */
            if (mScanningOn)
            { 
                /* Start advertising timer */
                TMR_StartLowPowerTimer(mBleAppTimerId, 
                           gTmrLowPowerSecondTimer_c,
                           TmrSeconds(gScanningTime_c),
                           ScaningTimerCallback, NULL);  
            }
            /* Node is not scanning */
            else
            {                
                TMR_StopTimer(mBleAppTimerId);
            }
        }
        break;
    case gScanCommandFailed_c:
    {
        panic(0, 0, 0, 0);
        break;
    }
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
    case gAdvertisingStateChanged_c:
        {
            mAdvState.advOn = !mAdvState.advOn;
        }
        break;
        
    case gAdvertisingCommandFailed_c:
        {
            panic(0,0,0,0);
        }
        break;
        
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Connection callback from host stack.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
********************************************************************************** */
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            mPeerInformation.deviceId = peerDeviceId;

            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;

            /* Subscribe client*/
            Wus_Subscribe(peerDeviceId);
            /* Stop Advertising Timer*/
            mAdvState.advOn = FALSE;
            TMR_StopTimer(mBleAppTimerId);
            
#if gAppUsePairing_d           
#if gAppUseBonding_d            
            Gap_CheckIfBonded(peerDeviceId, &mPeerInformation.isBonded);
            
            if ((mPeerInformation.isBonded) &&
                (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId,
                    (void*) &mPeerInformation.clientInfo, 0, sizeof (wucConfig_t))))
            {
                mRestoringBondedLink = TRUE;
                (void)mRestoringBondedLink;
                /* Restored custom connection information. Encrypt link */
                Gap_EncryptLink(peerDeviceId);
            }
            else
#endif /* gAppUseBonding_d*/ 
            {
                if (mGapRole == gGapCentral_c)
                {
//                    Gap_Pair(peerDeviceId, &gPairingParameters);
                }
            }
#endif /* gAppUsePairing_d */            
            BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PeerConnected_c);
        }
            break;

        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            Wus_Unsubscribe();
            /* Reset Service Discovery to be sure*/
            BleServDisc_Stop(peerDeviceId);

            mPeerInformation.appState = mAppIdle_c;

            BleApp_Start(mGapRole);
        }
        break;

#if gAppUsePairing_d		
        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PairingComplete_c);
            }
        }
        break;
#endif /* gAppUsePairing_d */

    default:
        break;
    }
    
    /* Connection Manager to handle Host Stack interactions */
    if (mGapRole == gGapCentral_c)
    {
        BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);
    }
    else if (mGapRole == gGapPeripheral_c)
    {
        BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);
    }
}

static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent)
{
    switch(pEvent->eventType)
    {
        case gServiceDiscovered_c:
        {
            //BleApp_StoreServiceHandles(peerDeviceId, pEvent->eventData.pService);
            if(pEvent->eventData.pService->uuidType == gBleUuidType128_c)
            {
                if( FLib_MemCmp((void*)&uuid_service_wireless_uart, (void*)&pEvent->eventData.pService->uuid, sizeof(bleUuid_t)))
                { 
                    BleApp_StoreServiceHandles(peerDeviceId, pEvent->eventData.pService); 
                }
            }
        }
        break;

        case gDiscoveryFinished_c:
        {
            if (pEvent->eventData.success)
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryComplete_c);
            }
            else
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryFailed_c);
            }
        }
        break;

        default:
        break;
    }
}

/*! *********************************************************************************
 * \brief        Handles GATT client callback from host stack.
 *
* \param[in]    serverDeviceId      GATT Server device ID.
* \param[in]    procedureType    	Procedure type.
* \param[in]    procedureResult    	Procedure result.
* \param[in]    error    			Callback result.
********************************************************************************** */
static void BleApp_GattClientCallback(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
)
{  
    if (procedureResult == gGattProcError_c)
    {    
        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
    }
    else if (procedureResult == gGattProcSuccess_c)
    {        
    	BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
    }

    /* Signal Service Discovery Module */
    BleServDisc_SignalGattClientEvent(serverDeviceId, procedureType,procedureResult, error);
}



/*! *********************************************************************************
 * \brief        Handles GATT server callback from host stack.
 *
 * \param[in]    deviceId        Client peer device ID.
 * \param[in]    pServerEvent    Pointer to gattServerEvent_t.
 ********************************************************************************** */
static void BleApp_GattServerCallback (
                                       deviceId_t deviceId,
                                       gattServerEvent_t* pServerEvent)
{
    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWrittenWithoutResponse_c:
        {
            if (pServerEvent->eventData.attributeWrittenEvent.handle == value_uart_stream)
            {
            	BleApp_ReceivedUartStream(pServerEvent->eventData.attributeWrittenEvent.aValue,
                            pServerEvent->eventData.attributeWrittenEvent.cValueLength);
            }
            break;
        }
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief        
*
* \param[in]    
********************************************************************************** */
static bool_t MatchDataInAdvElementList (gapAdStructure_t *pElement, void *pData, uint8_t iDataLen)
{
    uint8_t i;

    for (i = 0; i < pElement->length; i += iDataLen)
    {
        if (FLib_MemCmp(pData, &pElement->aData[i], iDataLen))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*! *********************************************************************************
 * \brief        Checks Scan data for a device to connect.
 *
 * \param[in]    pData    Pointer to gapScannedDevice_t.
 ********************************************************************************** */
static bool_t BleApp_CheckScanEvent (gapScannedDevice_t* pData)
{
    uint8_t index = 0;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t) pData->data[index + 1];
        adElement.aData = &pData->data[index + 2];

        /* Search for Temperature Custom Service */
        if ((adElement.adType == gAdIncomplete128bitServiceList_c)
            || (adElement.adType == gAdComplete128bitServiceList_c))
        {
            foundMatch = MatchDataInAdvElementList(&adElement,
                &uuid_service_wireless_uart, 16);
        }

        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
    }

    return foundMatch;
}

/*! *********************************************************************************
 * \brief        Stores handles used by the application.
 *
 * \param[in]    pService    Pointer to gattService_t.
 ********************************************************************************** */
static void BleApp_StoreServiceHandles (deviceId_t peerDeviceId, gattService_t *pService)
{
    /* Found Wireless UART Service */
    mPeerInformation.clientInfo.hService = pService->startHandle;

    if (pService->cNumCharacteristics > 0 &&
        pService->aCharacteristics != NULL)
    {
        /* Found Uart Characteristic */
        mPeerInformation.clientInfo.hUartStream =
            pService->aCharacteristics[0].value.handle;
    }
}

/*! *********************************************************************************
 * \brief        Send uart stream over ble
 *
 * \param[in]    pService    Pointer to gattService_t.
 ********************************************************************************** */
static void BleApp_SendUartStream(deviceId_t deviceId, uint8_t *pRecvStream, uint16_t streamSize)
{
    gattCharacteristic_t characteristic;
        
    characteristic.value.handle = mPeerInformation.clientInfo.hUartStream;

    GattClient_WriteCharacteristicValue(deviceId, &characteristic,
                                        streamSize, pRecvStream, TRUE,
                                        FALSE, FALSE, NULL); 
}
/*! *********************************************************************************
* \brief        
*
* \param[in]    deviceId    Device Id.
********************************************************************************** */
void BleApp_StateMachineHandler(deviceId_t peerDeviceId, uint8_t event)
{
    switch (mPeerInformation.appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
                /* Moving to Exchange MTU State */
                mPeerInformation.appState = mAppExchangeMtu_c;
                GattClient_ExchangeMtu(peerDeviceId);
            }
        }
        break;

        case mAppExchangeMtu_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* Moving to Service Discovery State*/
                mPeerInformation.appState = mAppServiceDisc_c;
                              
                Gatt_GetMtu(peerDeviceId, &mAppUartBufferSize);
                
                mAppUartBufferSize = gAttMaxWriteDataSize_d(mAppUartBufferSize);
                /* Start Service Discovery*/
//                BleServDisc_FindService(peerDeviceId, 
//                                        gBleUuidType128_c,
//                                        (bleUuid_t*) &uuid_service_wireless_uart);
                BleServDisc_Start(peerDeviceId);
            }
            else if (event == mAppEvt_GattProcError_c) 
            {
                Gap_Disconnect(peerDeviceId);
            }
        }
        break;

        case mAppServiceDisc_c:
        {
            if (event == mAppEvt_ServiceDiscoveryComplete_c)
            {            	
                /* Moving to Running State*/
                mPeerInformation.appState = mAppRunning_c;
            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                Gap_Disconnect(peerDeviceId);
            }
        }
        break;

        case mAppRunning_c:
                break;
        default:
                break;
    }
}

/*! *********************************************************************************
* \brief        Send through Bluetooth data from serial manager.
*
* \param[in]    pData    Pointer to data buffer.
* \param[in]    size     size in bytes of the data buffer.
********************************************************************************** */
uint32_t Serial_CustomSendData(uint8_t *pData, uint32_t size)
{
    TMR_StopTimer(mUartStreamFlushTimerId);
    
    if (mPeerInformation.appState == mAppRunning_c)
    {
        /* Cache the data before sending through ble */
        /* This reduces the ble traffic in dual mode*/
        while(size)
        {
            uint16_t tempSize = 0;
            if(mOnBleDataBufferIndex + size < mAppUartBufferSize)
            {
                TMR_StartLowPowerTimer(mUartStreamFlushTimerId,
                                       gTmrLowPowerSingleShotMillisTimer_c,
                                       mAppUartFlushIntervalInMs_c,
                                       UartStreamFlushTimerCallback, NULL);
                FLib_MemCpy(&mpSendOnBleDataBuffer[mOnBleDataBufferIndex],pData,size);
                mOnBleDataBufferIndex += size;
                size = 0;
            }
            else
            {
                tempSize = mAppUartBufferSize - mOnBleDataBufferIndex;
                FLib_MemCpy(&mpSendOnBleDataBuffer[mOnBleDataBufferIndex],pData,tempSize);
                BleApp_SendUartStream(mPeerInformation.deviceId, mpSendOnBleDataBuffer, (uint16_t)mAppUartBufferSize);
                mOnBleDataBufferIndex = 0;
                size -= tempSize;
                pData += tempSize;
            }
        }
    }
    else
    {
        /* Device was disconnected */
        mOnBleDataBufferIndex = 0;
    }
    Serial_CustomSendCompleted(gShellSerMgrIf);
    return 0;
}

/*! *********************************************************************************
 * \brief        Handles scanning timer callback.
 *
 * \param[in]    pParam        Calback parameters.
 ********************************************************************************** */
static void ScaningTimerCallback (void * pParam)
{
    /* Stop scanning and start advertising */
    Gap_StopScanning();
    
    mGapRole = gGapPeripheral_c;
    gPairingParameters.localIoCapabilities = gIoDisplayOnly_c;
    BleApp_Advertise();
}
/*! *********************************************************************************
* \brief       Handle ble cache buffer when mUartStreamFlushTimerId expired
*
* \param[in]    pParam   NULL
********************************************************************************** */
static void BleApp_FlushUartStream(void *pParam)
{
    OSA_InterruptDisable();
    if((mPeerInformation.appState == mAppRunning_c)&&(0  != mOnBleDataBufferIndex))
    {
        BleApp_SendUartStream(mPeerInformation.deviceId, mpSendOnBleDataBuffer, (uint16_t)mOnBleDataBufferIndex);
    }
    mOnBleDataBufferIndex = 0;
    OSA_InterruptEnable();
}
/*! *********************************************************************************
* \brief        
*
* \param[in]    deviceId    Device Id.
********************************************************************************** */
void BleApp_SerialSyncWrite(uint8_t *pBuf, uint16_t bufLen)
{
    if (mPeerInformation.appState != mAppRunning_c)
    {
        return;
    } 
    BleApp_SendUartStream(mPeerInformation.deviceId, pBuf, bufLen);
}
/*! *********************************************************************************
* \brief        
*
* \param[in]    deviceId    Device Id.
********************************************************************************** */
static void BleApp_ReceivedUartStream(uint8_t *pStream, uint16_t streamLength)
{
    Serial_CustomReceiveData(gShellSerMgrIf, pStream, streamLength);
}

/*! *********************************************************************************
* \brief        
*
* \param[in]    deviceId    Device Id.
********************************************************************************** */
static void UartStreamFlushTimerCallback(void *pData)
{
    App_PostCallbackMessage(BleApp_FlushUartStream, NULL);
}

/*!*************************************************************************************************
\fn     BleApp_GenerateAndSetRandomAddress
\brief  Generate a rnd. address
\param  [in]
\return         None
***************************************************************************************************/

static void BleApp_GenerateAndSetRandomAddress(void)
{
    /* dev addr from UID here */
    uint32_t gaUniqueId[4];
    uint8_t len;
    
    BOARD_GetMCUUid((uint8_t *)gaUniqueId, &len);
    sha1Context_t mCtx;
    SHA1_Hash(&mCtx, (uint8_t*)gaUniqueId, sizeof(gaUniqueId));
    FLib_MemCpy(mDevRandomAddress, (uint8_t*)(&mCtx.hash), sizeof(mDevRandomAddress));
    
    /* Set random static bits */
    mDevRandomAddress[5] |= 0xC0;
    bleResult_t res = Gap_SetRandomAddress(mDevRandomAddress);
    
    if(res != gBleSuccess_c)
    {
        panic(0,0,0,0);
    }
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
