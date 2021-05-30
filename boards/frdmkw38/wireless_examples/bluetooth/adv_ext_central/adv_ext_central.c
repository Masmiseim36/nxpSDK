/*! *********************************************************************************
* \addtogroup Extended Advertising Central
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2020 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the extended advertising central application
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
#include "Panic.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "SerialManager.h"
#include "MemManager.h"
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "temperature_interface.h"

#include "ble_conn_manager.h"
#include "ble_service_discovery.h"

#include "ApplMain.h"
#include "adv_ext_central.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mAppExtAdvListSize_c  (10U)
#define mBlePeriodicAdvInvalidSyncHandle_c  (0x1FFFU)
#define mPeriodicExtAdvInvalidIndex_c  (0xFFU)
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef enum appEvent_tag{
    mAppEvt_PeerConnected_c,
    mAppEvt_PairingComplete_c,
    mAppEvt_ServiceDiscoveryComplete_c,
    mAppEvt_ServiceDiscoveryFailed_c,
    mAppEvt_GattProcComplete_c,
    mAppEvt_GattProcError_c
}appEvent_t;

typedef enum appState_tag{
    mAppIdle_c,
    mAppExchangeMtu_c,
    mAppServiceDisc_c,
    mAppReadDescriptor_c,
    mAppRunning_c,
}appState_t;

typedef struct appCustomInfo_tag
{
    tmcConfig_t     tempClientConfig;
    /* Add persistent information here */
}appCustomInfo_t;

typedef struct appPeerInfo_tag
{
    deviceId_t      deviceId;
    appCustomInfo_t customInfo;
    bool_t          isBonded;
    appState_t      appState;
}appPeerInfo_t;

typedef struct extAdvListElement_tag
{
    bleDeviceAddress_t aAddress;
    uint8_t SID;
    uint32_t dataCRC;
    uint32_t perDataCRC;
    uint16_t periodicAdvInterval;
    uint16_t syncHandle;
}extAdvListElement_t;

typedef struct perAdvListElement_tag
{
    uint16_t syncHandle;
    uint32_t dataCRC;
}perAdvListElement_t;
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static appPeerInfo_t mPeerInformation;

#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
static bool_t mRestoringBondedLink = FALSE;
#endif

static bool_t   mScanningOn = FALSE;
static bool_t   mFoundDeviceToConnect = FALSE;

/* Buffer used for Characteristic related procedures */
static gattAttribute_t      *mpCharProcBuffer = NULL;

/* Timers */
static tmrTimerID_t mAppTimerId;
static uint8_t gAppSerMgrIf;
static const char* maAdvEvtConnStrings[] = { "\n\rNon Connectable", "\n\rConnectable" };
static const char* maAdvEvtScannStrings[] = { "\n\rNon Scannable", "\n\rScannable" };
static const char* maAdvEvtDirStrings[] = { "\n\rUndirected", "\n\rDirected" };
static const char* maAdvEvtDataTypeStrings[] = { "\n\rAdv Data", "\n\rScan Req Data" };
static const char* maAdvEvtAdvTypeStrings[] = { "\n\rExtended Advertising", "Legacy Advertising" };
static const char** maAdvPropStrings[] = {maAdvEvtConnStrings, maAdvEvtScannStrings, maAdvEvtDirStrings, maAdvEvtDataTypeStrings, maAdvEvtAdvTypeStrings};
static const char* maLePhyStrings[] = { "", "gLePhy1M_c", "gLePhy2M_c", "gLePhyCoded_c" };
static const char* maScanStrings[] = {"Passive", "Active"};
#if mAE_CentralDebug_c
static const char* maScanEventStrings[] = {\
    "gScanStateChanged_c",\
    "gScanCommandFailed_c",\
    "gDeviceScanned_c",\
    "gExtDeviceScanned_c",\
    "gPeriodicDeviceScanned_c",\
    "gPeriodicAdvSyncEstablished_c",\
    "gPeriodicAdvSyncLost_c",\
    "gPeriodicAdvSyncTerminated_c"\
};
#endif
static extAdvListElement_t maAppExtAdvList[mAppExtAdvListSize_c];
static uint8_t mAppExtAdvListIndex;
static uint8_t mPerExtAdvIndexPending = mPeriodicExtAdvInvalidIndex_c;
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Host Stack callbacks */
static void BleApp_ScanningCallback( gapScanningEvent_t* pScanningEvent );
static void BleApp_ConnectionCallback( deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent );
static void BleApp_GattClientCallback( deviceId_t serverDeviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error );
static void BleApp_GattNotificationCallback( deviceId_t serverDeviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength );
static void BleApp_ServiceDiscoveryCallback( deviceId_t peerDeviceId, servDiscEvent_t* pEvent );
static void BleApp_Config(void);
void BleApp_StateMachineHandler( deviceId_t peerDeviceId, appEvent_t event );
static bool_t CheckScanEvent(gapScanningEvent_t* pScanningEvent);
static void BleApp_StoreServiceHandles( gattService_t *pGattService );
static void BleApp_StoreDescValues( gattAttribute_t *pDesc );
static void BleApp_PrintTemperature( uint16_t temperature );
static bleResult_t BleApp_ConfigureNotifications(void);
static void ScanningTimeoutTimerCallback(void* pParam);
static void DisconnectTimerCallback(void* pParam);
static void AppPrintLePhyEvent(gapPhyEvent_t* pPhyEvent);
static void AppPrintString(const char* pBuff);
static void AppPrintDec(uint32_t nb);
static void AppPrintHexLe( uint8_t *pHex, uint8_t len);
static bool_t CheckForAEPeripheralDevice(uint8_t* pData, uint16_t dataLength);
static void AppHandleExtAdvEvent( gapExtScannedDevice_t* pExtScannedDevice);
static void AppHandlePeriodicDeviceScanEvent( gapPeriodicScannedDevice_t* pGapPeriodicScannedDevice);
void AppTerminatePeriodicAdvSync(void);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    For Hard Fault Debug Purposes.
*
********************************************************************************** */
#if mAE_CentralDebug_c
void HardFault_Handler(void)
{
    while(1);
}
#endif
/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
    /* Init shell and set prompt */
#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    gAppSerMgrIf = BOARD_GetSerialManagerInterface();;
#else
    SerialManager_Init();
    (void)Serial_InitInterface(&gAppSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    (void)Serial_SetBaudRate(gAppSerMgrIf, APP_SERIAL_INTERFACE_SPEED);
#endif
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);
#endif
    /* Start scanning */
    mAppExtAdvListIndex = 0;
    mPerExtAdvIndexPending = mPeriodicExtAdvInvalidIndex_c;
    (void)App_StartScanning(&gScanParams, BleApp_ScanningCallback, gGapDuplicateFilteringDisable_c, gGapScanContinuously_d, gGapScanPeriodicDisabled_d);
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    switch (events)
    {
    case gKBD_EventPressPB1_c:  /* fall-through */
    case gKBD_EventLongPB1_c:   /* fall-through */
        break;

        /* Start the application */
    case gKBD_EventPressPB2_c:
        {
            if (!mScanningOn)
            {
                gScanParams.type = gScanTypeActive_c;
                BleApp_Start();
            }
        }
        break;
        /* Start the application */
    case gKBD_EventLongPB2_c:
        {
            if (!mScanningOn)
            {
                gScanParams.type = gScanTypePassive_c;
                BleApp_Start();
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
        {
            BleApp_Config();
        }
        break;

    case gLePhyEvent_c:
        if(pGenericEvent->eventData.phyEvent.phyEventType == gPhyUpdateComplete_c )
        {
            AppPrintLePhyEvent(&pGenericEvent->eventData.phyEvent);
        }
        break;
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
    /* Configure as GAP Central */
    BleConnManager_GapCommonConfig();

    /* Register for callbacks*/
    (void)App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    (void)App_RegisterGattClientNotificationCallback(BleApp_GattNotificationCallback);
    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);

    /* Initialize private variables */
    mPeerInformation.appState = mAppIdle_c;
    mScanningOn = FALSE;
    mFoundDeviceToConnect = FALSE;

    /* Allocate scan timeout timer */
    mAppTimerId = TMR_AllocateTimer();

    /* Update UI */
    AppPrintString("\n\rExtended Advertising Application - Central");
    AppPrintString("\r\nPress WAKESW to Start Active Scanning!");
    AppPrintString("\r\nPress WAKESW Long to Start Passive Scanning!\r\n");

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    /* Allow entering sleep mode until any user interaction */
    (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
    PWR_AllowDeviceToSleep();
#else
    LED_StopFlash(LED_ALL);
    Led1On();
    Led2On();
#endif
}

/*! *********************************************************************************
* \brief        Handles BLE Scanning callback from host stack.
*
* \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
********************************************************************************** */

static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
#if mAE_CentralDebug_c
    AppPrintString("\n\rScan Callback - ");
    AppPrintString(maScanEventStrings[pScanningEvent->eventType]);
#endif
    switch (pScanningEvent->eventType)
    {
    case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;
            /* Node starts scanning */
            if (mScanningOn)
            {
                mFoundDeviceToConnect = FALSE;
                AppPrintString("\r\n");
                AppPrintString(maScanStrings[gScanParams.type]);
                AppPrintString(" Scanning Started\r\n");
                /* Start scanning timer */
                (void)TMR_StartLowPowerTimer(mAppTimerId,
                                             gTmrLowPowerSecondTimer_c,
                                             TmrSeconds(gScanningTime_c),
                                             ScanningTimeoutTimerCallback, NULL);
#if !defined(cPWR_UsePowerDownMode) || (cPWR_UsePowerDownMode == 0)
                 Led1Flashing();
#endif
            }
            /* Node is not scanning */
            else
            {
                (void)TMR_StopTimer(mAppTimerId);
#if !defined(cPWR_UsePowerDownMode) || (cPWR_UsePowerDownMode == 0)
                StopLed1Flashing();
                Led1On();
#endif
                /* Connect with the previously scanned peer device */
                if (mFoundDeviceToConnect)
                {
                    (void)App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                }
                else
                {
                    AppPrintString("\r\nStop Scanning\r\n");
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
                    /* Go to sleep */
                    (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
#endif
                }
                AppTerminatePeriodicAdvSync();
            }
        }
        break;

    case gScanCommandFailed_c:
        {
            ; /* No action required */
        }
        break;
    case gDeviceScanned_c:
        {
            bool_t AE_peripheral = CheckForAEPeripheralDevice(pScanningEvent->eventData.scannedDevice.data, pScanningEvent->eventData.scannedDevice.dataLength);
            if(AE_peripheral)
            {
                AppPrintString("\r\nFound AE peripheral device");
                AppPrintHexLe(pScanningEvent->eventData.scannedDevice.aAddress, 6);
                if( FALSE == mFoundDeviceToConnect )
                {
                    /* Check if the scanned device implements the Temperature Custom Profile */
                    mFoundDeviceToConnect = CheckScanEvent(pScanningEvent);
                    if (mFoundDeviceToConnect)
                    {
                        /* Set connection parameters and stop scanning. Connect on gScanStateChanged_c. */
                        gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                        FLib_MemCpy(gConnReqParams.peerAddress, pScanningEvent->eventData.scannedDevice.aAddress, sizeof(bleDeviceAddress_t));
                        gConnReqParams.initiatingPHYs = (uint8_t)gLePhy1MFlag_c;
                        (void)Gap_StopScanning();
#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d)
                        gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif
                    }
                }
            }
        }
        break;
    case gExtDeviceScanned_c:
        {
            bool_t AE_peripheral = CheckForAEPeripheralDevice(pScanningEvent->eventData.extScannedDevice.pData, pScanningEvent->eventData.extScannedDevice.dataLength);
            if(AE_peripheral)
            {
#if mAE_CentralDebug_c
                AppPrintString("\r\nExtended Advertising Found");
#endif
                AppHandleExtAdvEvent(&pScanningEvent->eventData.extScannedDevice);
                if( FALSE == mFoundDeviceToConnect )
                {
                    if((pScanningEvent->eventData.extScannedDevice.advEventProperties & (bleAdvReportEventProperties_t)gAdvEventConnectable_c) != 0U )
                    {
                        /* Check if the scanned device implements the Temperature Custom Profile */
                        mFoundDeviceToConnect = CheckScanEvent(pScanningEvent);
                        if (mFoundDeviceToConnect)
                        {
                            /* Set connection parameters and stop scanning. Connect on gScanStateChanged_c. */
                            gConnReqParams.peerAddressType = pScanningEvent->eventData.extScannedDevice.addressType;
                            FLib_MemCpy(gConnReqParams.peerAddress, pScanningEvent->eventData.extScannedDevice.aAddress, sizeof(bleDeviceAddress_t));
                            gConnReqParams.initiatingPHYs = ((1U)<<(pScanningEvent->eventData.extScannedDevice.secondaryPHY - 1U)) | ((1U)<<(pScanningEvent->eventData.extScannedDevice.primaryPHY - 1U));
                            (void)Gap_StopScanning();
#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d)
                            gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.extScannedDevice.advertisingAddressResolved;
#endif
                        }
                    }
                }
            }
        }
        break;
    case gPeriodicDeviceScanned_c:
        {
#if mAE_CentralDebug_c
            AppPrintString("\r\nPeriodic Advertising Found");
#endif
            AppHandlePeriodicDeviceScanEvent(&pScanningEvent->eventData.periodicScannedDevice);
        }
        break;
    case gPeriodicAdvSyncEstablished_c:
        {
            AppPrintString("\r\nPeriodic Adv Sync Established");
            if((mPerExtAdvIndexPending != mPeriodicExtAdvInvalidIndex_c) && (mPerExtAdvIndexPending < mAppExtAdvListIndex))
            {
                maAppExtAdvList[mPerExtAdvIndexPending].syncHandle = pScanningEvent->eventData.syncEstb.syncHandle;
                maAppExtAdvList[mPerExtAdvIndexPending].perDataCRC = 0;
            }
            mPerExtAdvIndexPending = mPeriodicExtAdvInvalidIndex_c;
        }
        break;
    case gPeriodicAdvSyncLost_c:
        {
            AppPrintString("\r\nPeriodic Adv Sync Lost");
            uint8_t i;
            for( i=0 ; i<mAppExtAdvListIndex ; i++)
            {
                if(maAppExtAdvList[i].syncHandle == pScanningEvent->eventData.syncLost.syncHandle)
                {
                    maAppExtAdvList[i].syncHandle = mBlePeriodicAdvInvalidSyncHandle_c;
                    maAppExtAdvList[i].periodicAdvInterval = 0;
                    break;
                }
            }
        }
        break;
    case gPeriodicAdvSyncTerminated_c:
        AppPrintString("\r\nPeriodic Adv Sync Terminated");
        if((mPerExtAdvIndexPending != mPeriodicExtAdvInvalidIndex_c) && (mPerExtAdvIndexPending < mAppExtAdvListIndex))
        {
            maAppExtAdvList[mPerExtAdvIndexPending].syncHandle = mBlePeriodicAdvInvalidSyncHandle_c;

        }
        mPerExtAdvIndexPending = mPeriodicExtAdvInvalidIndex_c;
        if (!mScanningOn)
        {
            AppTerminatePeriodicAdvSync();
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
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    /* Connection Manager to handle Host Stack interactions */
    BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
    case gConnEvtConnected_c:
        {
            AppPrintString("\r\nConnected!\r\n");
            mPeerInformation.deviceId = peerDeviceId;
            mPeerInformation.isBonded = FALSE;
            /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
            (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);

#if (!defined(CPU_MKW37A512VFT4) && !defined(CPU_MKW37Z512VFT4) && !defined(CPU_MKW38A512VFT4) && !defined(CPU_MKW38Z512VFT4) && !defined(CPU_MKW39A512VFT4) && !defined(CPU_MKW39Z512VFT4))
            PWR_AllowDeviceToSleep();
#endif /* CPU_MKW37xxx, CPU_MKW38xxx and CPU_MKW39xxx*/
#else
            /* Update UI */
            Led2Flashing();
#endif

#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
            bool_t isBonded = FALSE;
            (void)Gap_CheckIfBonded(peerDeviceId, &isBonded, NULL);

            if ((isBonded) &&
                (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId, (void*) &mPeerInformation.customInfo, 0, sizeof (appCustomInfo_t))))
            {
                mRestoringBondedLink = TRUE;
                /* Restored custom connection information. Encrypt link */
                (void)Gap_EncryptLink(peerDeviceId);
            }
#endif
            BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PeerConnected_c);
        }
        break;

    case gConnEvtDisconnected_c:
        {
            mPeerInformation.deviceId = gInvalidDeviceId_c;
            mPeerInformation.appState = mAppIdle_c;
            /* Reset Service Discovery to be sure*/
            BleServDisc_Stop(peerDeviceId);
            /* Update UI */
            AppPrintString("\r\nDisconnected!\r\n");

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
            /* Go to sleep */
            (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
#else
            /* UI */
            StopLed2Flashing();
            Led2On();
#endif
        }
        break;

#if gAppUsePairing_d
    case gConnEvtPairingComplete_c:
        {
            /* Notify state machine handler on pairing complete */
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
                mPeerInformation.isBonded = TRUE;
#endif
                BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PairingComplete_c);
            }
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
            else
            {
                mPeerInformation.isBonded = FALSE;
            }
#endif
        }
        break;

#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
    case gConnEvtEncryptionChanged_c:
        {
            if( pConnectionEvent->eventData.encryptionChangedEvent.newEncryptionState )
            {
                mPeerInformation.isBonded = TRUE;
                if( mRestoringBondedLink )
                {
                    /* Try to enable temperature notifications, disconnect on failure */
                    if( gBleSuccess_c != BleApp_ConfigureNotifications() )
                    {
                        (void)Gap_Disconnect(peerDeviceId);
                    }
                    else
                    {
                        mRestoringBondedLink = FALSE;
                    }
                }
            }
        }
        break;

    case gConnEvtAuthenticationRejected_c:
        {
            /* Start Pairing Procedure */
            (void)Gap_Pair(peerDeviceId, &gPairingParameters);
        }
        break;
#endif /* gAppUseBonding_d */
#endif /* gAppUsePairing_d */

    default:
        ; /* No action required */
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles discovered services.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pEvent              Pointer to servDiscEvent_t.
********************************************************************************** */
static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent)
{
    switch(pEvent->eventType)
    {
        /* Store the discovered handles for later use. */
    case gServiceDiscovered_c:
        {
            BleApp_StoreServiceHandles(pEvent->eventData.pService);
        }
        break;

        /* Service discovery has finished, run the state machine. */
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
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Stores handles for the specified service.
*
* \param[in]    pGattService    Pointer to gattService_t.
********************************************************************************** */
static void BleApp_StoreServiceHandles
(
gattService_t   *pGattService
)
{
    uint8_t i,j;

    if ((pGattService->uuidType == gBleUuidType128_c) &&
        FLib_MemCmp(pGattService->uuid.uuid128, uuid_service_temperature, 16))
    {
        /* Found Temperature Service */
        mPeerInformation.customInfo.tempClientConfig.hService = pGattService->startHandle;

        for (i = 0; i < pGattService->cNumCharacteristics; i++)
        {
            if ((pGattService->aCharacteristics[i].value.uuidType == gBleUuidType16_c) &&
                (pGattService->aCharacteristics[i].value.uuid.uuid16 == gBleSig_Temperature_d))
            {
                /* Found Temperature Char */
                mPeerInformation.customInfo.tempClientConfig.hTemperature = pGattService->aCharacteristics[i].value.handle;

                for (j = 0; j < pGattService->aCharacteristics[i].cNumDescriptors; j++)
                {
                    if (pGattService->aCharacteristics[i].aDescriptors[j].uuidType == gBleUuidType16_c)
                    {
                        switch (pGattService->aCharacteristics[i].aDescriptors[j].uuid.uuid16)
                        {
                            /* Found Temperature Char Presentation Format Descriptor */
                        case gBleSig_CharPresFormatDescriptor_d:
                            {
                                mPeerInformation.customInfo.tempClientConfig.hTempDesc = pGattService->aCharacteristics[i].aDescriptors[j].handle;
                                break;
                            }
                            /* Found Temperature Char CCCD */
                        case gBleSig_CCCD_d:
                            {
                                mPeerInformation.customInfo.tempClientConfig.hTempCccd = pGattService->aCharacteristics[i].aDescriptors[j].handle;
                                break;
                            }
                        default:
                            ; /* No action required */
                            break;
                        }
                    }
                }
            }
        }
    }
}

/*! *********************************************************************************
* \brief        Stores the value of the specified attribute.
*
* \param[in]    pDesc       Pointer to gattAttribute_t.
********************************************************************************** */
static void BleApp_StoreDescValues
(
gattAttribute_t     *pDesc
)
{
    if (pDesc->handle == mPeerInformation.customInfo.tempClientConfig.hTempDesc)
    {
        /* Store temperature format*/
        FLib_MemCpy(&mPeerInformation.customInfo.tempClientConfig.tempFormat, pDesc->paValue, pDesc->valueLength);
    }
}

/*! *********************************************************************************
* \brief        Displays the temperature value to shell.
*
* \param[in]    temperature     Temperature value
********************************************************************************** */
static void BleApp_PrintTemperature
(
uint16_t temperature
)
{
    AppPrintString("Temperature: ");
    AppPrintDec((uint32_t)temperature / 100UL);

    /* Add 'C' for Celsius degrees - UUID 0x272F.
    www.bluetooth.com/specifications/assigned-numbers/units */
    if (mPeerInformation.customInfo.tempClientConfig.tempFormat.unitUuid16 == 0x272FU)
    {
        AppPrintString(" C\r\n");
    }
    else
    {
        AppPrintString("\r\n");
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
static void BleApp_GattClientCallback
(
deviceId_t              serverDeviceId,
gattProcedureType_t     procedureType,
gattProcedureResult_t   procedureResult,
bleResult_t             error
)
{
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
    if ((mPeerInformation.isBonded) || (mPeerInformation.appState != mAppRunning_c))
    {
#endif
        if (procedureResult == gGattProcError_c)
        {
            attErrorCode_t attError = (attErrorCode_t)(uint8_t)(error);

            if (attError == gAttErrCodeInsufficientEncryption_c     ||
                attError == gAttErrCodeInsufficientAuthorization_c  ||
                    attError == gAttErrCodeInsufficientAuthentication_c)
            {
                /* Start Pairing Procedure */
                (void)Gap_Pair(serverDeviceId, &gPairingParameters);
            }

            BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
        }
        else
        {
            if (procedureResult == gGattProcSuccess_c)
            {
                switch(procedureType)
                {
                case gGattProcReadCharacteristicDescriptor_c:
                    {
                        if (mpCharProcBuffer != NULL)
                        {
                            /* Store the value of the descriptor */
                            BleApp_StoreDescValues(mpCharProcBuffer);
                        }
                        break;
                    }

                default:
                    {
                        ; /* No action required */
                        break;
                    }
                }
                BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
            }
        }
        /* Signal Service Discovery Module */
        BleServDisc_SignalGattClientEvent(serverDeviceId, procedureType, procedureResult, error);
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
    }
#endif
}

/*! *********************************************************************************
* \brief        Handles GATT client notification callback from host stack.
*
* \param[in]    serverDeviceId              GATT Server device ID.
* \param[in]    characteristicValueHandle   Handle.
* \param[in]    aValue                      Pointer to value.
* \param[in]    valueLength                 Value length.
********************************************************************************** */
static void BleApp_GattNotificationCallback
(
deviceId_t  serverDeviceId,
uint16_t    characteristicValueHandle,
uint8_t*    aValue,
uint16_t    valueLength
)
{
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
    if (mPeerInformation.isBonded)
    {
#endif
        if (characteristicValueHandle == mPeerInformation.customInfo.tempClientConfig.hTemperature)
        {
            BleApp_PrintTemperature(Utils_ExtractTwoByteValue(aValue));
            /* Restart Wait For Data timer */
            (void)TMR_StartLowPowerTimer(mAppTimerId,
                                         gTmrLowPowerSecondTimer_c,
                                         TmrSeconds(gWaitForDataTime_c),
                                         DisconnectTimerCallback, NULL);
        }
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
    }
#endif
}

/*! *********************************************************************************
* \brief        Detect whether the provided data is found in the advertising data.
*
* \param[in]    pElement                    Pointer a to AD structure.
* \param[in]    pData                       Data to look for.
* \param[in]    iDataLen                    Size of data to look for.
*
* \return       TRUE if data matches, FALSE if not
********************************************************************************** */
static bool_t MatchDataInAdvElementList(gapAdStructure_t *pElement, void *pData, uint8_t iDataLen)
{
    uint32_t i;
    bool_t status = FALSE;

    for (i = 0; i < (uint32_t)pElement->length - 1UL; i += iDataLen)
    {
        /* Compare input data with advertising data. */
        if (FLib_MemCmp(pData, &pElement->aData[i], iDataLen))
        {
            status = TRUE;
            break;
        }
    }
    return status;
}


/*! *********************************************************************************
* \brief        Process scanning events to search for the Temperature Custom Service.
*               This function is called from the scanning callback.
*
* \param[in]    pScanningEvent                   Pointer to the gapScanningEvent_t structure provided by the scanning callback.
*
* \return       TRUE if the scanned device implements the Temperature Custom Service,
                FALSE otherwise
********************************************************************************** */
static bool_t CheckScanEvent(gapScanningEvent_t* pScanningEvent)
{
    uint32_t index = 0;
    uint8_t name[13];
    uint8_t lineEnd[] = "\n\r";
    uint8_t nameLength = 0;
    bool_t foundMatch = FALSE;
    uint8_t* pData;
    uint16_t dataLength;
    uint8_t* pAddress;
    if(pScanningEvent->eventType == gDeviceScanned_c)
    {
        pData = pScanningEvent->eventData.scannedDevice.data;
        dataLength = pScanningEvent->eventData.scannedDevice.dataLength;
        pAddress = pScanningEvent->eventData.scannedDevice.aAddress;
    }
    else
    {
        pData = pScanningEvent->eventData.extScannedDevice.pData;
        dataLength = pScanningEvent->eventData.extScannedDevice.dataLength;
        pAddress = pScanningEvent->eventData.extScannedDevice.aAddress;
    }

    while (index < dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData[index];
        adElement.adType = (gapAdType_t)pData[index + 1U];
        adElement.aData = &pData[index + 2U];

        /* Search for Temperature Custom Service */
        if ((adElement.adType == gAdIncomplete128bitServiceList_c) ||
            (adElement.adType == gAdComplete128bitServiceList_c))
        {
            foundMatch = MatchDataInAdvElementList(&adElement, &uuid_service_temperature, 16);
        }

        if ((adElement.adType == gAdShortenedLocalName_c) ||
            (adElement.adType == gAdCompleteLocalName_c))
        {
            nameLength = MIN(adElement.length, 10U);
            FLib_MemCpy(name, adElement.aData, nameLength);
            FLib_MemCpy(name + nameLength, lineEnd, sizeof(lineEnd));
        }

        /* Move on to the next AD element type */
        index += (uint32_t)adElement.length + sizeof(uint8_t);
    }

    if (foundMatch && (nameLength > 0U))
    {
        /* Update UI */
        AppPrintString("\r\nFound device: \r\n");
        AppPrintString((const char*)name);
        AppPrintHexLe(pAddress, 6);
    }
    return foundMatch;
}

/*! *********************************************************************************
* \brief        Process scanning events to search for the "EA*PRPH" name
*               This function is called from the scanning callback.
*
* \param[in]    pData                   Pointer to advertising data.
*
* \return       TRUE if the scanned device name is "EA*PRPH",
                FALSE otherwise
********************************************************************************** */
static bool_t CheckForAEPeripheralDevice(uint8_t* pData, uint16_t dataLength)
{
    uint32_t index = 0;
    uint8_t nameLength = 0;
    bool_t foundMatch = FALSE;
    uint8_t eaString[] = "EA*PRPH";
    while (index < dataLength)
    {
        gapAdStructure_t adElement;
        adElement.length = pData[index];
        adElement.adType = (gapAdType_t)pData[index + 1U];
        adElement.aData = &pData[index + 2U];
        if ((adElement.adType == gAdShortenedLocalName_c) ||
            (adElement.adType == gAdCompleteLocalName_c))
        {
            nameLength = MIN(adElement.length - 1U, 10U);
            foundMatch =  FLib_MemCmp (adElement.aData, eaString, nameLength);
            if(foundMatch)
            {
                break;
            }
        }
        /* Move on to the next AD element type */
        index += (uint32_t)adElement.length + sizeof(uint8_t);
    }
    return foundMatch;
}

/*! *********************************************************************************
* \brief        State machine handler of the Temperature Collector application.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
********************************************************************************** */
void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event)
{
    switch (mPeerInformation.appState)
    {
    case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
                if (mPeerInformation.customInfo.tempClientConfig.hTemperature != gGattDbInvalidHandle_d)
                {
                    /* Moving to Running State and wait for Link encryption result */
                    mPeerInformation.appState = mAppRunning_c;
                }
                else
#endif
                {
                    /* Moving to Exchange MTU State */
                    mPeerInformation.appState = mAppExchangeMtu_c;
                    (void)GattClient_ExchangeMtu(peerDeviceId, gAttMaxMtu_c);
                }
            }
        }
        break;

    case mAppExchangeMtu_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* Moving to Service Discovery State*/
                mPeerInformation.appState = mAppServiceDisc_c;

                /* Start Service Discovery*/
                (void)BleServDisc_Start(peerDeviceId);
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
                /* Moving to Primary Service Discovery State*/
                mPeerInformation.appState = mAppReadDescriptor_c;

                if (mPeerInformation.customInfo.tempClientConfig.hTempDesc != 0U)
                {
                    mpCharProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + gAttDefaultMtu_c);
                    if (mpCharProcBuffer == NULL)
                    {
                        (void)Gap_Disconnect(peerDeviceId);
                    }
                    else
                    {
                        mpCharProcBuffer->handle = mPeerInformation.customInfo.tempClientConfig.hTempDesc;
                        mpCharProcBuffer->paValue = (uint8_t*)(mpCharProcBuffer + 1);
                        (void)GattClient_ReadCharacteristicDescriptor(mPeerInformation.deviceId, mpCharProcBuffer, gAttDefaultMtu_c);
                    }
                }
            }
            else
            {
                if (event == mAppEvt_ServiceDiscoveryFailed_c)
                {
                    (void)Gap_Disconnect(peerDeviceId);
                }
            }
        }
        break;

    case mAppReadDescriptor_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mPeerInformation.customInfo.tempClientConfig.hTempCccd != 0U)
                {
                    /* Try to enable temperature notifications, disconnect on failure */
                    if( gBleSuccess_c != BleApp_ConfigureNotifications() )
                    {
                        (void)Gap_Disconnect(peerDeviceId);
                    }
                    else
                    {
                        /* Moving to Running State*/
                        mPeerInformation.appState = mAppRunning_c;
                    }
                }
            }
            else
            {
                if (event == mAppEvt_PairingComplete_c)
                {
                    /* Continue after pairing is complete */
                    (void)GattClient_ReadCharacteristicDescriptor(peerDeviceId, mpCharProcBuffer, gAttDefaultMtu_c);
                }
            }
        }
        break;

    case mAppRunning_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mpCharProcBuffer != NULL)
                {
                    (void)MEM_BufferFree(mpCharProcBuffer);
                    mpCharProcBuffer = NULL;
                }

#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
                /* Write data in NVM */
                (void)Gap_SaveCustomPeerInformation(mPeerInformation.deviceId,
                                                    (void *)&mPeerInformation.customInfo, 0,
                                                    sizeof(appCustomInfo_t));
#endif
                /* Start Wait For Data timer */
                (void)TMR_StartLowPowerTimer(mAppTimerId,
                                             gTmrLowPowerSecondTimer_c,
                                             TmrSeconds(gWaitForDataTime_c),
                                             DisconnectTimerCallback, NULL);
            }
            else
            {
                if (event == mAppEvt_PairingComplete_c)
                {
                    /* Try to enable temperature notifications, disconnect on failure */
                    if( gBleSuccess_c != BleApp_ConfigureNotifications() )
                    {
                        (void)Gap_Disconnect(peerDeviceId);
                    }
                }
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
* \brief        Enable temperature notifications by writing peer's CCCD of the
                Temperature characteristic.
*
* \return       gBleSuccess_c or gBleOutOfMemory_c
********************************************************************************** */
static bleResult_t BleApp_ConfigureNotifications(void)
{
    bleResult_t result = gBleSuccess_c;
    uint16_t value = (uint16_t)gCccdNotification_c;

    /* Allocate buffer for the write operation */
    if( mpCharProcBuffer == NULL )
    {
        mpCharProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + gAttDefaultMtu_c);
    }

    if( mpCharProcBuffer != NULL )
    {
        /* Populate the write request */
        mpCharProcBuffer->handle = mPeerInformation.customInfo.tempClientConfig.hTempCccd;
        mpCharProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
        mpCharProcBuffer->valueLength = 0;
        (void)GattClient_WriteCharacteristicDescriptor(mPeerInformation.deviceId,
                                                       mpCharProcBuffer,
                                                       (uint16_t)sizeof(value), (void*)&value);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

/*! *********************************************************************************
* \brief        Stop scanning after a given time (gScanningTime_c).
                Called on timer task.
*
* \param[in]    pParam              not used
********************************************************************************** */
static void ScanningTimeoutTimerCallback(void* pParam)
{
    /* Stop scanning */
    if (mScanningOn)
    {
        (void)Gap_StopScanning();
    }
}

/*! *********************************************************************************
* \brief        Disconnect from peer device if no data was received for a given time.
                (gWaitForDataTime_c). Called on timer task.
*
* \param[in]    pParam              not used
********************************************************************************** */
static void DisconnectTimerCallback(void* pParam)
{
    /* Disconnect from peer device */
    if (mPeerInformation.deviceId != gInvalidDeviceId_c)
    {
        (void)Gap_Disconnect(mPeerInformation.deviceId);
    }
}

/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
static void PrintLePhyEvent(void(*pfPrint)(const char* pString),gapPhyEvent_t* pPhyEvent)
{
    /* String dictionary corresponding to gapLePhyMode_t */
    static const char* mLePhyModeStrings[] =
    {
        "Invalid\n\r",
        "1M\n\r",
        "2M\n\r",
        "Coded\n\r",
    };
    uint8_t txPhy = (pPhyEvent->txPhy <= (uint8_t)gLePhyCoded_c) ? pPhyEvent->txPhy : (0U);
    uint8_t rxPhy = (pPhyEvent->rxPhy <= (uint8_t)gLePhyCoded_c) ? pPhyEvent->rxPhy : (0U);
    pfPrint("Phy Update Complete.\n\r");
    pfPrint("TxPhy ");
    pfPrint(mLePhyModeStrings[txPhy]);
    pfPrint("RxPhy ");
    pfPrint(mLePhyModeStrings[rxPhy]);
}

/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
static void AppPrintLePhyEvent(gapPhyEvent_t* pPhyEvent)
{
    PrintLePhyEvent(AppPrintString, pPhyEvent);
}

/*! *********************************************************************************
* \brief        Prints a string.
*
********************************************************************************** */
static void AppPrintString(const char* pBuff)
{
    (void)Serial_Print(gAppSerMgrIf, pBuff, gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief        Prints a uint32_t value in decimal.
*
********************************************************************************** */
static void AppPrintDec(uint32_t nb)
{
    (void)Serial_PrintDec(gAppSerMgrIf, nb);
}

/*! *********************************************************************************
* \brief        Prints a len octets number in hexadecimal.
*
* \param[in]    pHex                Pointer to the number.
* \param[in]    len                 Number length.
********************************************************************************** */
static void AppPrintHexLe( uint8_t *pHex, uint8_t len)
{
    (void)Serial_PrintHex(gAppSerMgrIf, pHex, len, gPrtHexNoFormat_c);
}

/*! *********************************************************************************
* \brief        Computes Extended Adv Data CRC to decide whether adv data changed or not
*
* \param[in]    pData                Pointer to advertising data.
* \param[in]    dataLength           data length.
********************************************************************************** */
static uint32_t AppCalculateAdvDataCRC(const void* pData, uint16_t dataLength)
{
    uint32_t CRC = 0;
    const uint32_t* p32 = (const uint32_t*)pData;
    const uint8_t* p8;
    while(dataLength >= sizeof(uint32_t))
    {
        CRC += *p32++;
        dataLength -=  sizeof(uint32_t);
    }
    p8 = (const uint8_t*)p32;
    while(dataLength != 0U)
    {
        CRC += *p8++;
        dataLength --;
    }
    return CRC;
}
/*! *********************************************************************************
* \brief        Prints Extended Adv Event
*
* \param[in]    pExtScannedDevice                pointer to gapExtScannedDevice_t structure provided by the scanning callback.
********************************************************************************** */
static void AppPrintExtAdvEvent( gapExtScannedDevice_t* pExtScannedDevice)
{
    uint16_t  dataLength;
    uint8_t i;
    uint8_t* pData;
    AppPrintString("\r\nExtended Advertising Found");
    AppPrintString("\n\rAdv Properties:");
    for(i = 0U ; i< 5U ; i++)
    {
        uint8_t j;
        j = ((pExtScannedDevice->advEventProperties & ((bleAdvReportEventProperties_t)(1U)<<i)) != 0U)? (1U):(0U);
        AppPrintString(maAdvPropStrings[i][j]);
    }
    AppPrintString("\n\rAdv Address ");
    if((pExtScannedDevice->advEventProperties & (((bleAdvReportEventProperties_t)1U)<<i)) != 0U)
    {
        AppPrintString("Anonymous");
    }
    else
    {
        AppPrintHexLe(pExtScannedDevice->aAddress, 6);
    }
    AppPrintString("\n\rData Set Id = ");AppPrintDec((uint32_t)pExtScannedDevice->SID);
    AppPrintString("\n\rPrimaryPHY = ");AppPrintString(maLePhyStrings[pExtScannedDevice->primaryPHY]);
    AppPrintString("\n\rSecondaryPHY = ");AppPrintString(maLePhyStrings[pExtScannedDevice->secondaryPHY]);

    AppPrintString("\n\rperiodicAdvInterval = ");AppPrintDec((uint32_t)pExtScannedDevice->periodicAdvInterval);

    AppPrintString(maAdvEvtDataTypeStrings[((pExtScannedDevice->advEventProperties & ((1U)<<3))>>3)]);
    dataLength = 0;
    pData = pExtScannedDevice->pData;
    while( dataLength < pExtScannedDevice->dataLength )
    {
        dataLength += ((uint16_t)*pData + 1U);
        if(*(pData+1) == (uint8_t)gAdManufacturerSpecificData_c)
        {
            AppPrintString((char const*)pData+2);
        }
        pData =  pExtScannedDevice->pData + dataLength;
    }
}
/*! *********************************************************************************
* \brief        Handles Extended Advertising Event Management
*
* \param[in]    pExtScannedDevice                pointer to gapExtScannedDevice_t structure provided by the scanning callback.
********************************************************************************** */
static void AppHandleExtAdvEvent( gapExtScannedDevice_t* pExtScannedDevice)
{
    uint8_t advIndex;
    bool_t advPresent = FALSE;
    bool_t advDataChanged = FALSE;
    bool_t handlePriodicAdv = FALSE;
    uint32_t advCRC = AppCalculateAdvDataCRC(pExtScannedDevice->pData, pExtScannedDevice->dataLength);
    if(mAppExtAdvListIndex != 0U)
    {
        uint8_t i;

        for( i=0 ; i<mAppExtAdvListIndex ; i++)
        {
            if(FLib_MemCmp (pExtScannedDevice->aAddress, maAppExtAdvList[i].aAddress, sizeof(bleDeviceAddress_t)))
            {
                if(pExtScannedDevice->SID == maAppExtAdvList[i].SID)
                {
                    advPresent = TRUE;
                    advIndex = i;
                    if(advCRC != maAppExtAdvList[i].dataCRC )
                    {
                        advDataChanged = TRUE;
                    }
                    if(maAppExtAdvList[i].periodicAdvInterval != pExtScannedDevice->periodicAdvInterval)
                    {
                        handlePriodicAdv = TRUE;
                    }
                    break;

                }
                else
                {
                    continue;
                }

            }
            else
            {
                continue;
            }

        }
    }
    if(advPresent == FALSE)
    {

        if(mAppExtAdvListIndex < mAppExtAdvListSize_c)
        {
            FLib_MemCpy (maAppExtAdvList[mAppExtAdvListIndex].aAddress, pExtScannedDevice->aAddress, sizeof(bleDeviceAddress_t));
            maAppExtAdvList[mAppExtAdvListIndex].SID = pExtScannedDevice->SID;
            maAppExtAdvList[mAppExtAdvListIndex].dataCRC = advCRC;
            maAppExtAdvList[mAppExtAdvListIndex].periodicAdvInterval = 0;
            maAppExtAdvList[mAppExtAdvListIndex].syncHandle = mBlePeriodicAdvInvalidSyncHandle_c;
            advIndex = mAppExtAdvListIndex++ ;
            if(pExtScannedDevice->periodicAdvInterval != 0U)
            {
                handlePriodicAdv = TRUE;
            }
        }
        else
        {
            advIndex =   mAppExtAdvListSize_c;
        }
    }
    if( (advPresent == FALSE) || (advDataChanged == TRUE))
    {

        if(advIndex < mAppExtAdvListSize_c)
        {
            maAppExtAdvList[advIndex].dataCRC = advCRC;
        }
        AppPrintExtAdvEvent(pExtScannedDevice);

    }

    if((handlePriodicAdv == TRUE) && (advIndex < mAppExtAdvListSize_c))
    {
        if(pExtScannedDevice->periodicAdvInterval != 0U)
        {
            if( mPerExtAdvIndexPending == mPeriodicExtAdvInvalidIndex_c)
            {
                gapPeriodicAdvSyncReq_t gapPeriodicAdvSyncReq;
                gapPeriodicAdvSyncReq.filterPolicy = (gapCreateSyncReqFilterPolicy_t)gUseCommandParameters_c;
                gapPeriodicAdvSyncReq.SID = pExtScannedDevice->SID;
                gapPeriodicAdvSyncReq.peerAddressType = pExtScannedDevice->addressType;
                FLib_MemCpy (gapPeriodicAdvSyncReq.peerAddress, pExtScannedDevice->aAddress, sizeof(bleDeviceAddress_t));
                gapPeriodicAdvSyncReq.skipCount = 0;
                uint32_t timeout = pExtScannedDevice->periodicAdvInterval;
                timeout = (timeout * 5U)>>2U;/* timeout in ms N*1.25*/
                timeout = timeout*5U;/* at 5 adv missed timeout should expire*/
                timeout = timeout/10U;/* timeout unit is 10ms*/
                gapPeriodicAdvSyncReq.timeout = (uint16_t)timeout;
                AppPrintString("\n\rGap_PeriodicAdvCreateSync ");
                if(gBleSuccess_c == Gap_PeriodicAdvCreateSync( &gapPeriodicAdvSyncReq))
                {
                    maAppExtAdvList[advIndex].syncHandle = gBlePeriodicAdvOngoingSyncCancelHandle;
                    maAppExtAdvList[advIndex].periodicAdvInterval = pExtScannedDevice->periodicAdvInterval;
                    mPerExtAdvIndexPending = advIndex;
                    AppPrintString("Succeded");

                }
                else
                {
                    AppPrintString("Failed");
                }
            }

        }
        else
        {
            if( mPerExtAdvIndexPending == mPeriodicExtAdvInvalidIndex_c)
            {
                if(mBlePeriodicAdvInvalidSyncHandle_c != maAppExtAdvList[advIndex].syncHandle )
                {
                    AppPrintString("\n\rGap_PeriodicAdvTerminateSync ");
                    if(gBleSuccess_c == Gap_PeriodicAdvTerminateSync(maAppExtAdvList[advIndex].syncHandle))
                    {
                        maAppExtAdvList[advIndex].periodicAdvInterval = pExtScannedDevice->periodicAdvInterval;
                        mPerExtAdvIndexPending = advIndex;
                        AppPrintString("Succeded");

                    }
                    else
                    {
                        AppPrintString("Failed");
                    }
                }
            }
        }
    }
}

/*! *********************************************************************************
* \brief        Handles gPeriodicDeviceScanned_c event in the scanning callback
*
* \param[in]    pGapPeriodicScannedDevice                pointer to the gapPeriodicScannedDevice_t structure provided by the scanning callback.
********************************************************************************** */
static void AppHandlePeriodicDeviceScanEvent( gapPeriodicScannedDevice_t* pGapPeriodicScannedDevice)
{

    uint16_t  dataLength;
    uint8_t* pData;
    bool_t perAdvPresent = FALSE;
    bool_t perAdvDataChanged = FALSE;
    uint8_t advIndex;
    uint32_t advCRC = AppCalculateAdvDataCRC(pGapPeriodicScannedDevice->pData, pGapPeriodicScannedDevice->dataLength);
    if(mAppExtAdvListIndex != 0U)
    {
        for( advIndex=0 ; advIndex < mAppExtAdvListIndex ; advIndex++)
        {
            if(maAppExtAdvList[advIndex].syncHandle == pGapPeriodicScannedDevice->syncHandle)
            {
                perAdvPresent = TRUE;
                if(maAppExtAdvList[advIndex].perDataCRC != advCRC)
                {
                    perAdvDataChanged = TRUE;
                    maAppExtAdvList[advIndex].perDataCRC = advCRC;
                }
                break;
            }
        }
    }

    if((perAdvPresent == TRUE) && (perAdvDataChanged == TRUE))
    {
        AppPrintString("\r\nPeriodic Adv Found");
        AppPrintString("\n\rsyncHandle = ");AppPrintDec((uint32_t)pGapPeriodicScannedDevice->syncHandle);
        AppPrintString("\n\rPeriodic Data");
        dataLength = 0;
        pData = pGapPeriodicScannedDevice->pData;
        while ( dataLength < pGapPeriodicScannedDevice->dataLength )
        {
            dataLength += ((uint16_t)*pData + 1U);
            if(*(pData+1) == (uint8_t)gAdManufacturerSpecificData_c)
            {
                AppPrintString((char const*)pData+2);
            }
            pData =  pGapPeriodicScannedDevice->pData + dataLength;
        }
    }
}
/*! *********************************************************************************
* \brief        Stops all Periodic Adv Syncs created during the scanning callback.If there are more than one started, this function
*               is called again on gPeriodicAdvSyncTerminated_c event in scanning callback until all syncs are terminated.
*
* \param[in]    none.
********************************************************************************** */
void AppTerminatePeriodicAdvSync(void)
{
    uint8_t i;
    if( mPerExtAdvIndexPending == mPeriodicExtAdvInvalidIndex_c)
    {
        for( i=0 ; i<mAppExtAdvListIndex ; i++)
        {
            if(maAppExtAdvList[i].syncHandle != mBlePeriodicAdvInvalidSyncHandle_c)
            {
                if(gBleSuccess_c == Gap_PeriodicAdvTerminateSync(maAppExtAdvList[i].syncHandle))
                {
                    mPerExtAdvIndexPending = i;
                }
            }
        }
    }
}
/*! *********************************************************************************
* @}
********************************************************************************** */
