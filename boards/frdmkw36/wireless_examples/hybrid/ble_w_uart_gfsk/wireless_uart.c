/*! *********************************************************************************
 * \addtogroup Wireless UART Application
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Wireless UART Application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "Panic.h"
#include "SerialManager.h"
#include "MemManager.h"
#include "board.h"

/* BLE Host Stack */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_database.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"

#if !defined(MULTICORE_APPLICATION_CORE) || (!MULTICORE_APPLICATION_CORE)
#include "gatt_db_handles.h"
#else
#define UUID128(name, ...)\
    extern uint8_t name[16];
#include "gatt_uuid128.h"
#undef UUID128
#endif

/* Profile / Services */
#include "wireless_uart_interface.h"
#include "battery_interface.h"
/* Wrappers */
#include "ble_conn_manager.h"
#include "ble_service_discovery.h"

#include "board.h"
#include "ApplMain.h"
#include "wireless_uart.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
    #include "erpc_host.h"
    #include "dynamic_gatt_database.h"
#endif


/************************************************************************************
 *************************************************************************************
 * Private macros
 *************************************************************************************
 ************************************************************************************/

#define mAppUartBufferSize_c            gAttMaxWriteDataSize_d(gAttMaxMtu_c) /* Local Buffer Size */

#define mAppUartFlushIntervalInMs_c     (7)     /* Flush Timeout in Ms */

#define mBatteryLevelReportInterval_c   (10)    /* battery level report interval in seconds  */
/************************************************************************************
 *************************************************************************************
 * Private type definitions
 *************************************************************************************
 ************************************************************************************/
typedef enum appEvent_tag
{
    mAppEvt_PeerConnected_c,
    mAppEvt_PairingComplete_c,
    mAppEvt_ServiceDiscoveryComplete_c,
    mAppEvt_ServiceDiscoveryNotFound_c,
    mAppEvt_ServiceDiscoveryFailed_c,
    mAppEvt_GattProcComplete_c,
    mAppEvt_GattProcError_c
} appEvent_t;

typedef enum appState_tag
{
    mAppIdle_c,
    mAppExchangeMtu_c,
    mAppServiceDisc_c,
    mAppServiceDiscRetry_c,
    mAppRunning_c
} appState_t;

typedef struct appPeerInfo_tag
{
    deviceId_t  deviceId;
    bool_t      isBonded;
    wucConfig_t clientInfo;
    appState_t  appState;
    gapRole_t   gapRole;
} appPeerInfo_t;

typedef struct advState_tag
{
    bool_t advOn;
} advState_t;
/************************************************************************************
 *************************************************************************************
 * Private memory declarations
 *************************************************************************************
 ************************************************************************************/

static appPeerInfo_t maPeerInformation[gAppMaxConnections_c];
static gapRole_t     mGapRole;

/* Adv Parameters */
static advState_t mAdvState;
static bool_t   mScanningOn = FALSE;

static uint16_t mCharMonitoredHandles[1] = { (uint16_t)value_uart_stream };

/* Service Data*/
static wusConfig_t mWuServiceConfig;
static bool_t      mBasValidClientList[gAppMaxConnections_c] = {FALSE};
static basConfig_t mBasServiceConfig = {(uint16_t)service_battery, 0, mBasValidClientList, gAppMaxConnections_c};

static tmrTimerID_t mAppTimerId = gTmrInvalidTimerID_c;
static tmrTimerID_t mUartStreamFlushTimerId = gTmrInvalidTimerID_c;
static tmrTimerID_t mBatteryMeasurementTimerId = gTmrInvalidTimerID_c;

static uint8_t gAppSerMgrIf;
static uint16_t mAppUartBufferSize = mAppUartBufferSize_c;
static volatile bool_t mAppUartNewLine = FALSE;
static volatile bool_t mAppDapaPending = FALSE;

/************************************************************************************
 *************************************************************************************
 * Private functions prototypes
 *************************************************************************************
 ************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback(gapAdvertisingEvent_t *pAdvertisingEvent);
static void BleApp_ScanningCallback(gapScanningEvent_t *pScanningEvent);
static void BleApp_ConnectionCallback
(
    deviceId_t peerDeviceId,
    gapConnectionEvent_t *pConnectionEvent
);
static void BleApp_GattServerCallback
(
    deviceId_t deviceId,
    gattServerEvent_t *pServerEvent
);

static void BleApp_GattClientCallback
(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);

static void BleApp_ServiceDiscoveryCallback
(
    deviceId_t      peerDeviceId,
    servDiscEvent_t *pEvent
);

static void BleApp_Config(void);
static void BleApp_Advertise(void);

void BleApp_StateMachineHandler
(
    deviceId_t peerDeviceId,
    appEvent_t event
);

static void BleApp_StoreServiceHandles
(
    deviceId_t       peerDeviceId,
    gattService_t   *pService
);
static bool_t BleApp_CheckScanEvent(gapScannedDevice_t *pData);

/* Timer Callbacks */
static void ScanningTimerCallback(void *pParam);
static void UartStreamFlushTimerCallback(void *pData);
static void BatteryMeasurementTimerCallback(void *pParam);

/* Uart Tx/Rx Callbacks*/
static void Uart_RxCallBack(void *pData);
static void Uart_TxCallBack(void *pBuffer);

static void BleApp_FlushUartStream(void *pParam);
static void BleApp_ReceivedUartStream(deviceId_t peerDeviceId, uint8_t *pStream, uint16_t streamLength);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
    /* Initialize application support for drivers */
    BOARD_InitAdc();
}

/*! *********************************************************************************
* \brief    Initializes application specific functionality 
*
********************************************************************************** */
void BleApp_InitSerialInterface(uint8_t appSerMgrIf)
{
    gAppSerMgrIf = appSerMgrIf;  

    /* Install Controller Events Callback handler */
    (void)Serial_SetRxCallBack(gAppSerMgrIf, Uart_RxCallBack, NULL);
    
    /* allocate timer for uart */
    mUartStreamFlushTimerId = TMR_AllocateTimer();
}

/*! *********************************************************************************
 * \brief    Starts the BLE application.
 *
 * \param[in]    gapRole    GAP Start Role (Central or Peripheral).
 ********************************************************************************** */
void BleApp_Start(gapRole_t gapRole)
{
    switch (gapRole)
    {
        case gGapCentral_c:
        {
            mGapRole = gGapCentral_c;
            (void)Serial_Print(gAppSerMgrIf, "\n\rBLE: Scanning...\n\r", gAllowToBlock_d);
            mAppUartNewLine = TRUE;
#if gUseControllerNotifications_c
            Gap_ControllerEnhancedNotification(gNotifScanEventOver_c | gNotifScanAdvPktRx_c |
                                               gNotifScanRspRx_c | gNotifScanReqTx_c, 0);
#endif
            gPairingParameters.localIoCapabilities = gIoKeyboardDisplay_c;
            (void)App_StartScanning(&gScanParams, BleApp_ScanningCallback, gGapDuplicateFilteringEnable_c, gGapScanContinuously_d, gGapScanPeriodicDisabled_d);
            break;
        }

        case gGapPeripheral_c:
        {
             mGapRole = gGapPeripheral_c;
            (void)Serial_Print(gAppSerMgrIf, "\n\rBLE: Advertising...\n\r", gAllowToBlock_d);
            mAppUartNewLine = TRUE;
#if gUseControllerNotifications_c
            Gap_ControllerEnhancedNotification(gNotifAdvEventOver_c | gNotifAdvTx_c |
                                               gNotifAdvScanReqRx_c | gNotifAdvConnReqRx_c, 0);
#endif
            gPairingParameters.localIoCapabilities = gIoDisplayOnly_c;
            BleApp_Advertise();
            break;
        }

        default:
        {
            ; /* No action required */
            break;
        }
    }
}

/*! *********************************************************************************
* \brief        Handles BLE generic callback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
void BleApp_GenericCallback(gapGenericEvent_t *pGenericEvent)
{
    /* Call BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);

    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:
        {
            BleApp_Config();
        }
        break;

        case gAdvertisingParametersSetupComplete_c:
        {
            (void)Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
        }
        break;

        case gAdvertisingDataSetupComplete_c:
        {
            (void)App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;

        case gAdvertisingSetupFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

#if gUseControllerNotifications_c
        case gControllerNotificationEvent_c:
        {
            switch(pGenericEvent->eventData.notifEvent.eventType)
            {
                case gNotifEventNone_c:
                    Serial_Print(gAppSerMgrIf, "\n\rConfigured notification status ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.status);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifConnEventOver_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] CONN Event Over device ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.deviceId);
                    Serial_Print(gAppSerMgrIf, " on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, " with RSSI ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint8_t)pGenericEvent->eventData.notifEvent.rssi);
                    Serial_Print(gAppSerMgrIf, " and event counter ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint16_t)pGenericEvent->eventData.notifEvent.ce_counter);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifConnRxPdu_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] CONN Rx PDU from device ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.deviceId);
                    Serial_Print(gAppSerMgrIf, " on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, " with RSSI ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint8_t)pGenericEvent->eventData.notifEvent.rssi);
                    Serial_Print(gAppSerMgrIf, " and event counter ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint16_t)pGenericEvent->eventData.notifEvent.ce_counter);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifAdvEventOver_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] ADV Event Over.\n\r", gAllowToBlock_d);
                    break;

                case gNotifAdvTx_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] ADV Tx on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifAdvScanReqRx_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] ADV Rx Scan Req on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, " with RSSI ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint8_t)pGenericEvent->eventData.notifEvent.rssi);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifAdvConnReqRx_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] ADV Rx Conn Req on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, " with RSSI ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint8_t)pGenericEvent->eventData.notifEvent.rssi);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifScanEventOver_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] SCAN Event Over on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifScanAdvPktRx_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] SCAN Rx Adv Pkt on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, " with RSSI ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint8_t)pGenericEvent->eventData.notifEvent.rssi);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifScanRspRx_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] SCAN Rx Scan Rsp on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, " with RSSI ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, (uint8_t)pGenericEvent->eventData.notifEvent.rssi);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;

                case gNotifScanReqTx_c:
                    Serial_Print(gAppSerMgrIf, "\n\r[EN] SCAN Tx Scan Req on channel ", gAllowToBlock_d);
                    Serial_PrintDec(gAppSerMgrIf, pGenericEvent->eventData.notifEvent.channel);
                    Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);
                    break;
            }
        }
        break;
#endif

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief        Configures BLE Stack after initialization. Usually used for
*               configuring advertising, scanning, white list, services, et al.
*
********************************************************************************** */
static void BleApp_Config(void)
{
    uint8_t mPeerId = 0;

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)

    if (GattDbDynamic_CreateDatabase() != gBleSuccess_c)
    {
        panic(0, 0, 0, 0);
        return;
    }

#endif /* MULTICORE_APPLICATION_CORE */

    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();

    /* Register for callbacks */
    (void)App_RegisterGattServerCallback(BleApp_GattServerCallback);
    (void)App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    (void)GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(mCharMonitoredHandles), mCharMonitoredHandles);
    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);

    for (mPeerId = 0; mPeerId < (uint8_t)gAppMaxConnections_c; mPeerId++)
    {
        maPeerInformation[mPeerId].appState = mAppIdle_c;
        maPeerInformation[mPeerId].deviceId = gInvalidDeviceId_c;
        maPeerInformation[mPeerId].clientInfo.hService = gGattDbInvalidHandleIndex_d;
        maPeerInformation[mPeerId].clientInfo.hUartStream = gGattDbInvalidHandleIndex_d;
    }

    mAdvState.advOn = FALSE;
    mScanningOn = FALSE;

    /* Start services */
    (void)Wus_Start(&mWuServiceConfig);

    mBasServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_Start(&mBasServiceConfig);

    /* Allocate application timer */
    mAppTimerId = TMR_AllocateTimer();
    mBatteryMeasurementTimerId = TMR_AllocateTimer();
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    /* Set advertising parameters*/
    (void)Gap_SetAdvertisingParameters(&gAdvParams);
}

/*! *********************************************************************************
 * \brief        Handles BLE Scanning callback from host stack.
 *
 * \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
 ********************************************************************************** */
static void BleApp_ScanningCallback(gapScanningEvent_t *pScanningEvent)
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

                (void)Gap_StopScanning();
#if gAppUsePrivacy_d
                gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif
                (void)App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
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
                (void)TMR_StartLowPowerTimer(mAppTimerId,
                                             gTmrLowPowerSecondTimer_c,
                                             TmrSeconds(gScanningTime_c),
                                             ScanningTimerCallback, NULL);

                Led1Flashing();
            }
            /* Node is not scanning */
            else
            {
                (void)TMR_StopTimer(mAppTimerId);

                Led1Flashing();
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
            }
        }
        break;

        case gScanCommandFailed_c:
        {
            panic(0, 0, 0, 0);
            break;
        }

        default:
        {
            ; /* No action required */
            break;
        }
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback(gapAdvertisingEvent_t *pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
        {
            mAdvState.advOn = !mAdvState.advOn;
            LED_StopFlashingAllLeds();
            Led1Flashing();

            if (!mAdvState.advOn)
            {
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            panic(0, 0, 0, 0);
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Connection callback from host stack.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
********************************************************************************** */
static void BleApp_ConnectionCallback(deviceId_t peerDeviceId, gapConnectionEvent_t *pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            /* Save peer device ID */
            maPeerInformation[peerDeviceId].deviceId = peerDeviceId;

            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;

            /* Subscribe client*/
            (void)Wus_Subscribe(peerDeviceId);
            (void)Bas_Subscribe(&mBasServiceConfig, peerDeviceId);

            /* UI */
            LED_StopFlashingAllLeds();
            Led1On();

            /* Stop Advertising Timer*/
            (void)TMR_StopTimer(mAppTimerId);

            if (!TMR_IsTimerActive(mBatteryMeasurementTimerId))
            {
                /* Start battery measurements */
                (void)TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                                             TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);
            }

#if gAppUsePairing_d
#if gAppUseBonding_d

            if (mGapRole == gGapCentral_c)
            {
                (void)Gap_CheckIfBonded(peerDeviceId, &maPeerInformation[peerDeviceId].isBonded);

                if ((maPeerInformation[peerDeviceId].isBonded) &&
                    (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId,
                            (void *) &maPeerInformation[peerDeviceId].clientInfo, 0, sizeof(wucConfig_t))))
                {
                    /* Restored custom connection information. Encrypt link */
                    (void)Gap_EncryptLink(peerDeviceId);
                }
            }

#endif /* gAppUseBonding_d*/
#endif /* gAppUsePairing_d */

            (void)Serial_Print(gAppSerMgrIf, "\n\rBLE: Connected to device ", gAllowToBlock_d);
            (void)Serial_PrintDec(gAppSerMgrIf, peerDeviceId);

            if (mGapRole == gGapCentral_c)
            {
                (void)Serial_Print(gAppSerMgrIf, " as master.\n\r", gAllowToBlock_d);
            }
            else
            {
                (void)Serial_Print(gAppSerMgrIf, " as slave.\n\r", gAllowToBlock_d);
            }

            mAppUartNewLine = TRUE;

#if defined(gUseControllerNotifications_c) && (gUseControllerNotifications_c)
            Gap_ControllerEnhancedNotification(gNotifConnRxPdu_c, peerDeviceId);
#endif

            maPeerInformation[peerDeviceId].gapRole = mGapRole;

            /* run the state machine */
            BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PeerConnected_c);
        }
        break;

        case gConnEvtDisconnected_c:
        {
            (void)Serial_Print(gAppSerMgrIf, "\n\rBLE: Disconnected from device ", gAllowToBlock_d);
            (void)Serial_PrintDec(gAppSerMgrIf, peerDeviceId);
            (void)Serial_Print(gAppSerMgrIf, ".\n\r", gAllowToBlock_d);

            maPeerInformation[peerDeviceId].appState = mAppIdle_c;
            maPeerInformation[peerDeviceId].clientInfo.hService = gGattDbInvalidHandleIndex_d;
            maPeerInformation[peerDeviceId].clientInfo.hUartStream = gGattDbInvalidHandleIndex_d;

            /* Unsubscribe client */
            (void)Wus_Unsubscribe();
            (void)Bas_Unsubscribe(&mBasServiceConfig, peerDeviceId);
            (void)TMR_StopTimer(mBatteryMeasurementTimerId);

            /* Reset Service Discovery to be sure*/
            BleServDisc_Stop(peerDeviceId);

            /* UI */
            LED_TurnOffAllLeds();
            LED_StartFlash(LED_ALL);

            /* mark device id as invalid */
            maPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;

            /* recalculate minimum of maximum MTU's of all connected devices */
            mAppUartBufferSize                       = mAppUartBufferSize_c;

            for (uint8_t mPeerId = 0; mPeerId < (uint8_t)gAppMaxConnections_c; mPeerId++)
            {
                if (gInvalidDeviceId_c != maPeerInformation[mPeerId].deviceId)
                {
                    uint16_t tempMtu;

                    (void)Gatt_GetMtu(mPeerId, &tempMtu);
                    tempMtu = gAttMaxWriteDataSize_d(tempMtu);

                    if (tempMtu < mAppUartBufferSize)
                    {
                        mAppUartBufferSize = tempMtu;
                    }
                }
            }

            if (mGapRole == gGapPeripheral_c)
            {
                BleApp_Start(mGapRole);
            }
        }
        break;

#if gAppUsePairing_d

        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(peerDeviceId,
                                           mAppEvt_PairingComplete_c);
            }
        }
        break;
#endif /* gAppUsePairing_d */

        default:
        {
            ; /* No action required */
        }
        break;
    }

    /* Connection Manager to handle Host Stack interactions */
    switch (maPeerInformation[peerDeviceId].gapRole)
    {
        case gGapCentral_c:
            BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);
            break;

        case gGapPeripheral_c:
            BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);
            break;

        default:
            ; /* No action required */
            break;
    }
}

static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t *pEvent)
{
    switch (pEvent->eventType)
    {
        case gServiceDiscovered_c:
        {
            if (pEvent->eventData.pService->uuidType == gBleUuidType128_c)
            {
                if (FLib_MemCmp((void *)&uuid_service_wireless_uart, (void *)&pEvent->eventData.pService->uuid, sizeof(bleUuid_t)))
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
                if (gGattDbInvalidHandleIndex_d != maPeerInformation[peerDeviceId].clientInfo.hService)
                {
                    BleApp_StateMachineHandler(peerDeviceId,
                                               mAppEvt_ServiceDiscoveryComplete_c);
                }
                else
                {
                    BleApp_StateMachineHandler(peerDeviceId,
                                               mAppEvt_ServiceDiscoveryNotFound_c);
                }
            }
            else
            {
                BleApp_StateMachineHandler(peerDeviceId,
                                           mAppEvt_ServiceDiscoveryFailed_c);
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles GATT client callback from host stack.
*
* \param[in]    serverDeviceId      GATT Server device ID.
* \param[in]    procedureType       Procedure type.
* \param[in]    procedureResult     Procedure result.
* \param[in]    error               Callback result.
********************************************************************************** */
static void BleApp_GattClientCallback(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
)
{
    switch (procedureResult)
    {
        case gGattProcError_c:
            BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
            break;

        case gGattProcSuccess_c:
            BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
            break;

        default:
            ; /* No action required */
            break;
    }

    /* Signal Service Discovery Module */
    BleServDisc_SignalGattClientEvent(serverDeviceId, procedureType, procedureResult, error);
}

/*! *********************************************************************************
 * \brief        Handles GATT server callback from host stack.
 *
 * \param[in]    deviceId        Client peer device ID.
 * \param[in]    pServerEvent    Pointer to gattServerEvent_t.
 ********************************************************************************** */
static void BleApp_GattServerCallback(
    deviceId_t deviceId,
    gattServerEvent_t *pServerEvent)
{
    uint16_t tempMtu = 0;

    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWrittenWithoutResponse_c:
        {
            if (pServerEvent->eventData.attributeWrittenEvent.handle == (uint16_t)value_uart_stream)
            {
                BleApp_ReceivedUartStream(deviceId, pServerEvent->eventData.attributeWrittenEvent.aValue,
                                          pServerEvent->eventData.attributeWrittenEvent.cValueLength);
            }

            break;
        }

        case gEvtMtuChanged_c:
        {
            /* update stream length with minimum of  new MTU */
            (void)Gatt_GetMtu(deviceId, &tempMtu);
            tempMtu = gAttMaxWriteDataSize_d(tempMtu);

            mAppUartBufferSize = mAppUartBufferSize <= tempMtu ? mAppUartBufferSize : tempMtu;
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}


static bool_t MatchDataInAdvElementList(gapAdStructure_t *pElement,
                                        void *pData,
                                        uint8_t iDataLen)
{
    uint8_t i;
    bool_t status = FALSE;

    for (i = 0; i < (pElement->length - 1U); i += iDataLen)
    {
        if (FLib_MemCmp(pData, &pElement->aData[i], iDataLen))
        {
            status = TRUE;
            break;
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief        Checks Scan data for a device to connect.
 *
 * \param[in]    pData    Pointer to gapScannedDevice_t.
 ********************************************************************************** */
static bool_t BleApp_CheckScanEvent(gapScannedDevice_t *pData)
{
    uint8_t index = 0;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t) pData->data[index + 1U];
        adElement.aData = &pData->data[index + 2U];

        /* Search for Wireless UART Service */
        if ((adElement.adType == gAdIncomplete128bitServiceList_c)
            || (adElement.adType == gAdComplete128bitServiceList_c))
        {
            foundMatch = MatchDataInAdvElementList(&adElement,
                                                   &uuid_service_wireless_uart, 16);
        }

        /* Move on to the next AD element type */
        index += adElement.length + sizeof(uint8_t);
    }

    return foundMatch;
}

/*! *********************************************************************************
 * \brief        Stores handles used by the application.
 *
 * \param[in]    pService    Pointer to gattService_t.
 ********************************************************************************** */
static void BleApp_StoreServiceHandles(deviceId_t peerDeviceId, gattService_t *pService)
{
    /* Found Wireless UART Service */
    maPeerInformation[peerDeviceId].clientInfo.hService = pService->startHandle;

    if (pService->cNumCharacteristics > 0U &&
        pService->aCharacteristics != NULL)
    {
        /* Found Uart Characteristic */
        maPeerInformation[peerDeviceId].clientInfo.hUartStream =
            pService->aCharacteristics[0].value.handle;
    }
}

static void BleApp_SendUartStream(uint8_t *pRecvStream, uint8_t streamSize)
{
    gattCharacteristic_t characteristic = {gGattCharPropNone_c, {0}, 0, 0};
    uint8_t              mPeerId = 0;

    /* send UART stream to all peers */
    for (mPeerId = 0; mPeerId < (uint8_t)gAppMaxConnections_c; mPeerId++)
    {
        if (gInvalidDeviceId_c != maPeerInformation[mPeerId].deviceId &&
            mAppRunning_c == maPeerInformation[mPeerId].appState)
        {
            characteristic.value.handle = maPeerInformation[mPeerId].clientInfo.hUartStream;
            (void)GattClient_WriteCharacteristicValue(mPeerId, &characteristic,
                    streamSize, pRecvStream, TRUE,
                    FALSE, FALSE, NULL);
        }
    }
}

void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event)
{
    uint16_t tempMtu = 0;
    union
    {
        uint8_t     *pUuidArray;
        bleUuid_t   *pUuidObj;
    } temp; /* MISRA rule 11.3 */

    temp.pUuidArray = uuid_service_wireless_uart;

    /* invalid client information */
    if (gInvalidDeviceId_c == maPeerInformation[peerDeviceId].deviceId)
    {
        return;
    }

    switch (maPeerInformation[peerDeviceId].appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
                /* Let the central device initiate the Exchange MTU procedure*/
                if (mGapRole == gGapCentral_c)
                {
                    /* Moving to Exchange MTU State */
                    maPeerInformation[peerDeviceId].appState = mAppExchangeMtu_c;
                    (void)GattClient_ExchangeMtu(peerDeviceId);
                }
                else
                {
                    /* Moving to Service Discovery State*/
                    maPeerInformation[peerDeviceId].appState = mAppServiceDisc_c;

                    /* Start Service Discovery*/
                    (void)BleServDisc_FindService(peerDeviceId,
                                                  gBleUuidType128_c,
                                                  temp.pUuidObj);
                }
            }
        }
        break;

        case mAppExchangeMtu_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* update stream length with minimum of maximum MTU's of connected devices */
                (void)Gatt_GetMtu(peerDeviceId, &tempMtu);
                tempMtu = gAttMaxWriteDataSize_d(tempMtu);

                mAppUartBufferSize = mAppUartBufferSize <= tempMtu ? mAppUartBufferSize : tempMtu;

                /* Moving to Service Discovery State*/
                maPeerInformation[peerDeviceId].appState = mAppServiceDisc_c;

                /* Start Service Discovery*/
                (void)BleServDisc_FindService(peerDeviceId,
                                              gBleUuidType128_c,
                                              temp.pUuidObj);
            }
            else
            {
                if (event == mAppEvt_GattProcError_c)
                {
                    (void)Gap_Disconnect(peerDeviceId);
                }
            }
        }
        break;

        case mAppServiceDisc_c:
        {
            if (event == mAppEvt_ServiceDiscoveryComplete_c)
            {
                /* Moving to Running State*/
                maPeerInformation[peerDeviceId].appState = mAppRunning_c;
#if gAppUseBonding_d
                /* Write data in NVM */
                (void)Gap_SaveCustomPeerInformation(maPeerInformation[peerDeviceId].deviceId,
                                                    (void *) &maPeerInformation[peerDeviceId].clientInfo, 0,
                                                    sizeof(wucConfig_t));
#endif
            }
            else if (event == mAppEvt_ServiceDiscoveryNotFound_c)
            {
                /* Moving to Service discovery Retry State*/
                maPeerInformation[peerDeviceId].appState = mAppServiceDiscRetry_c;
                /* Restart Service Discovery for all services */
                (void)BleServDisc_Start(peerDeviceId);
            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                (void)Gap_Disconnect(peerDeviceId);
            }
            else
            {
                /* ignore other event types */
            }
        }
        break;

        case mAppServiceDiscRetry_c:
        {
            if (event == mAppEvt_ServiceDiscoveryComplete_c)
            {
                /* Moving to Running State*/
                maPeerInformation[peerDeviceId].appState = mAppRunning_c;
            }
            else if ((event == mAppEvt_ServiceDiscoveryNotFound_c) ||
                     (event == mAppEvt_ServiceDiscoveryFailed_c))
            {
                (void)Gap_Disconnect(peerDeviceId);
            }
            else
            {
                /* ignore other event types */
            }
        }
        break;

        case mAppRunning_c:
            break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
 * \brief        Handles scanning timer callback.
 *
 * \param[in]    pParam        Callback parameters.
 ********************************************************************************** */
static void ScanningTimerCallback(void *pParam)
{
    /* Stop scanning */
    (void)Gap_StopScanning();
    (void)Serial_Print(gAppSerMgrIf, "\n\rBLE: Scan timeout.\n\r", gAllowToBlock_d);
}

static void BleApp_FlushUartStream(void *pParam)
{
    uint8_t *pMsg = NULL;
    uint16_t bytesRead = 0;
    uint8_t  mPeerId = 0;
    bool_t   mValidDevices = FALSE;

    /* Valid devices are in Running state */
    for (mPeerId = 0; mPeerId < (uint8_t)gAppMaxConnections_c; mPeerId++)
    {
        if ((gInvalidDeviceId_c != maPeerInformation[mPeerId].deviceId) &&
            (mAppRunning_c == maPeerInformation[mPeerId].appState))
        {
            mValidDevices = TRUE;
            break;
        }
    }

    if (mValidDevices)
    {
        /* Allocate buffer for GATT Write */
        pMsg = MEM_BufferAlloc(mAppUartBufferSize);

        if (pMsg != NULL)
        {
            /* Collect the data from the serial manager buffer */
            if (Serial_Read(gAppSerMgrIf, pMsg, mAppUartBufferSize, &bytesRead) == gSerial_Success_c)
            {
                if (bytesRead != 0U)
                {
                    /* Send data over the air */
                    BleApp_SendUartStream(pMsg, (uint8_t)bytesRead);
                }
            }

            /* Free Buffer */
            (void)MEM_BufferFree(pMsg);
        }
    }

    mAppDapaPending = FALSE;
}

static void BleApp_ReceivedUartStream(deviceId_t peerDeviceId, uint8_t *pStream, uint16_t streamLength)
{
    static deviceId_t previousDeviceId = gInvalidDeviceId_c;

    char additionalInfoBuff[14] = { '\r', '\n', 'B', 'L', 'E', ':','[', '0', '0', '-', 'M', ']', ':', ' '};
    uint8_t *pBuffer = NULL;
    uint32_t messageHeaderSize = 0;

    if (mAppUartNewLine || (previousDeviceId != peerDeviceId))
    {
        streamLength += (uint32_t)sizeof(additionalInfoBuff);
    }

    /* Allocate buffer for asynchronous write */
    pBuffer = MEM_BufferAlloc(streamLength);

    if (pBuffer != NULL)
    {
        /* if this is a message from a previous device, print device ID */
        if (mAppUartNewLine || (previousDeviceId != peerDeviceId))
        {
            messageHeaderSize = sizeof(additionalInfoBuff);

            if (mAppUartNewLine)
            {
                mAppUartNewLine = FALSE;
            }

            additionalInfoBuff[7] = '0' + (peerDeviceId / 10U);
            additionalInfoBuff[8] = '0' + (peerDeviceId % 10U);

            if (gGapCentral_c != maPeerInformation[peerDeviceId].gapRole)
            {
                additionalInfoBuff[6] = 'S';
            }

            FLib_MemCpy(pBuffer, additionalInfoBuff, sizeof(additionalInfoBuff));
        }

        FLib_MemCpy(pBuffer + messageHeaderSize, pStream, (uint32_t)streamLength - messageHeaderSize);
        (void)Serial_AsyncWrite(gAppSerMgrIf, pBuffer, streamLength, Uart_TxCallBack, pBuffer);
    }

    /* update the previous device ID */
    previousDeviceId = peerDeviceId;
}

static void UartStreamFlushTimerCallback(void *pData)
{
    if (!mAppDapaPending)
    {
        mAppDapaPending = TRUE;
        (void)App_PostCallbackMessage(BleApp_FlushUartStream, NULL);
    }
}

/*! *********************************************************************************
* \brief        Handles UART Receive callback.
*
* \param[in]    pData        Parameters.
********************************************************************************** */
static void Uart_RxCallBack(void *pData)
{
    uint16_t byteCount = 0;

    (void)Serial_RxBufferByteCount(gAppSerMgrIf, &byteCount);

    if (byteCount < mAppUartBufferSize)
    {
        /* Restart flush timer */
        (void)TMR_StartLowPowerTimer(mUartStreamFlushTimerId,
                                     gTmrLowPowerSingleShotMillisTimer_c,
                                     mAppUartFlushIntervalInMs_c,
                                     UartStreamFlushTimerCallback, NULL);
    }
    else
    {
        /* Post App Msg only one at a time */
        if (!mAppDapaPending)
        {
            mAppDapaPending = TRUE;
            (void)App_PostCallbackMessage(BleApp_FlushUartStream, NULL);
        }
    }
}

/*! *********************************************************************************
* \brief        Handles UART Transmit callback.
*
* \param[in]    pData        Parameters.
********************************************************************************** */
static void Uart_TxCallBack(void *pBuffer)
{
    (void)MEM_BufferFree(pBuffer);
}


/*! *********************************************************************************
* \brief        Handles battery measurement timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void BatteryMeasurementTimerCallback(void *pParam)
{
    mBasServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_RecordBatteryMeasurement(&mBasServiceConfig);
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
