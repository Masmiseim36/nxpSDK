/*! *********************************************************************************
* \addtogroup LowPowerRefDes
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2019-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the header file for the lowpower master reference design application
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
#include "shell.h"
#include "PWR_Interface.h"
#include "PWR_Configuration.h"

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
#include "lowpower_master.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

#if defined(gAppStateNoActivityRamRetention_d) && (gAppStateNoActivityRamRetention_d == 1)
#define LP_MASTER_APP_STATE_NO_ACTIVITY         PWR_APP_STATE_NO_ACTIVITY_RAM_RET
#else
#define LP_MASTER_APP_STATE_NO_ACTIVITY         PWR_APP_STATE_NO_ACTIVITY
#endif

/* Debug Macros - stub if not defined */
#ifndef APP_DBG_LOG
#define APP_DBG_LOG(...)
#endif

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct bacConfig_tag
{
    uint16_t                    hService;
    uint16_t                    hBattery;
    uint16_t                    hBatteryCccd;
    uint16_t                    hBatteryDesc;
    gattDbCharPresFormat_t      batteryFormat;
} bacConfig_t;

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
    mAppConfigureNotification_c,
    mAppRunning_c,
}appState_t;

typedef struct appCustomInfo_tag
{
    tmcConfig_t     tempClientConfig;
    bacConfig_t     batteryClientConfig;
    bool_t          tempDescRead;
    bool_t          tempNotifyEnabled;
    bool_t          battDescRead;
    bool_t          battNotifyEnabled;
    /* Add persistent information here */
}appCustomInfo_t;

typedef struct appPeerInfo_tag
{
    deviceId_t      deviceId;
    appCustomInfo_t customInfo;
    bool_t          isBonded;
    appState_t      appState;
}appPeerInfo_t;

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

#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d)
static bool_t mWaitForControllerPrivacy = FALSE;
#endif

/* Buffer used for Characteristic related procedures */
static gattAttribute_t      *mpCharProcBuffer = NULL;

/* Timers */
static tmrTimerID_t mAppTimerId;
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Host Stack callbacks */
static void BleApp_ScanningCallback(gapScanningEvent_t* pScanningEvent);
static void BleApp_ConnectionCallback(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattClientCallback(deviceId_t serverDeviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error);
static void BleApp_GattNotificationCallback(deviceId_t serverDeviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength);
static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent);

/* Application private prototypes */
static void BleApp_Config(void);
static void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event);
static void BleApp_StoreServiceHandles(gattService_t *pService);
static void BleApp_StoreDescValues(gattAttribute_t *pDesc);
static void BleApp_PrintTemperature(uint16_t temperature);
static void BleApp_PrintBattery(uint8_t battery);

#if defined(gAppExtAdvEnable_d) && (gAppExtAdvEnable_d ==1)
static bool_t BleApp_CheckExtScanEvent(gapExtScannedDevice_t* pData);
#endif
static bool_t BleApp_CheckScanEvent(gapScannedDevice_t* pData);

static bleResult_t BleApp_ConfigureTemperatureNotifications(void);
static bleResult_t BleApp_ConfigureBatteryNotifications(void);

static void BleApp_PrintLePhyEvent(gapPhyEvent_t* pPhyEvent);

/* Timer callbacks */
static void ScanningTimeoutTimerCallback(void* pParam);
static void DisconnectTimerCallback(void* pParam);
#if defined(gAppWakeUpTimerAfterNoActivityMs) && (gAppWakeUpTimerAfterNoActivityMs != 0U)
static void WakeUpTimerCallback(void* pParam);
#endif

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
    uint16_t               resetStatus;

    /* shell_init is done in hardware_init, logo display is delayed to now
       to be sure Serial Manager task is ready to be used */
    shell_write_prompt("BLE Lowpower Master>");

    if (PWRLib_MCU_WakeupReason.Bits.FromKeyBoard == 1)
    {
        APP_DBG_LOG("Wakeup from KBD");
    }

    PWR_SetWakeupPins(BOARD_WAKEUP_PIN_KEYBOARD_BITMAP);

    resetStatus = PWR_GetSystemResetStatus();
    if( (resetStatus & gPWRLib_ResetStatus_WAKEUP) && ((resetStatus & gPWRLib_ResetStatus_PIN) != gPWRLib_ResetStatus_PIN) )
    {
        shell_write("\r\nWake up from RAMOFF\r\n");
    }
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    /* Change App mode to SCAN */
    PWR_SetNewAppState(PWR_APP_STATE_SCAN);
    PWR_AllowDeviceToSleep();

    if (!mScanningOn)
    {
        /* Start scanning */
        (void)App_StartScanning(&gScanParams, BleApp_ScanningCallback, gGapDuplicateFilteringEnable_c, gGapScanContinuously_d, gGapScanPeriodicDisabled_d);
    }

    APP_DBG_LOG("");
    DBG_LOG_DUMP(FALSE);
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    APP_DBG_LOG("%d", events);

    /* Start on button press if not scanning */
    switch (events)
    {
    case 1:
        break;
    case 2:
        if (mScanningOn)
        {
            APP_DBG_LOG("Stopping scan");
            (void)Gap_StopScanning();
        }
        else
        {
            APP_DBG_LOG("Starting scan");
            BleApp_Start();
        }
        break;
    }
    DBG_LOG_DUMP(FALSE);
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
        {
            if(pGenericEvent->eventData.phyEvent.phyEventType == gPhyUpdateComplete_c )
            {
                BleApp_PrintLePhyEvent(&pGenericEvent->eventData.phyEvent);
            }
        }
        break;

#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
        case gControllerPrivacyStateChanged_c:
        {
            if(pGenericEvent->eventData.newControllerPrivacyState == TRUE)
            {
                if(mWaitForControllerPrivacy == TRUE)
                {
                    /* Controller Privacy is ready, we can start scan */
                    BleApp_Start();
                    mWaitForControllerPrivacy = FALSE;
                }
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
    shell_write("\r\ntrying to connect to a lp refdes app\r\n");

#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
    if (gcBondedDevices == 0U)
    {
        /* if no devices are bonded, we can start scan immediately */
        BleApp_Start();
    }
    else
    {
        /* will wait event from Controller before starting scan */
        mWaitForControllerPrivacy = TRUE;
        /* allow lowpower while COntroller Privacy gets ready */
        PWR_SetNewAppState(PWR_APP_STATE_SCAN);
        PWR_AllowDeviceToSleep();
    }
#else
    BleApp_Start();
#endif
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
#if defined(gAppExtAdvEnable_d) && (gAppExtAdvEnable_d ==1)
        case gExtDeviceScanned_c:
#endif
        {
            /* Check if the scanned device implements the Temperature Custom Profile */
            if( FALSE == mFoundDeviceToConnect )
            {
#if defined(gAppExtAdvEnable_d) && (gAppExtAdvEnable_d ==1)
                 if (gExtDeviceScanned_c == pScanningEvent->eventType)
                  {
                    mFoundDeviceToConnect = BleApp_CheckExtScanEvent(&pScanningEvent->eventData.extScannedDevice);
                  }
                 else               
#endif
                 {
                    mFoundDeviceToConnect = BleApp_CheckScanEvent(&pScanningEvent->eventData.scannedDevice);
                 }

                if (mFoundDeviceToConnect)
                {
                    /* Set connection parameters and stop scanning. Connect on gScanStateChanged_c. */
                    gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                    FLib_MemCpy(gConnReqParams.peerAddress,
                                pScanningEvent->eventData.scannedDevice.aAddress,
                                sizeof(bleDeviceAddress_t));

                    (void)Gap_StopScanning();
#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d)
                    gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif
                }
            }
        }
        break;

        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;

            /* Node starts scanning */
            if (mScanningOn)
            {
                mFoundDeviceToConnect = FALSE;

                shell_write("\r\nScanning...\r\n");
                APP_DBG_LOG("Scanning");

                /* Start scanning timer */
                (void)TMR_StartLowPowerTimer(mAppTimerId,
                           gTmrLowPowerSecondTimer_c,
                           TmrSeconds(gAppScanningTimeout_c),
                           ScanningTimeoutTimerCallback, NULL);
            }
            /* Node scanning */
            else
            {
                (void)TMR_StopTimer(mAppTimerId);

                shell_write("\r\nScanning stopped!\r\n");
                APP_DBG_LOG("Scanning stopped");

                /* Connect with the previously scanned peer device */
                if (mFoundDeviceToConnect)
                {
                    (void)App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                }
                else
                {
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
#if defined(gAppStateNoActivityRamRetention_d) && (gAppStateNoActivityRamRetention_d == 1)
                    shell_write("\r\nGoing into lowpower (RAMRET)\r\n");
#else
                    shell_write("\r\nGoing into lowpower (RAMOFF)\r\n");
#endif
#if defined(gAppWakeUpTimerAfterNoActivityMs) && (gAppWakeUpTimerAfterNoActivityMs != 0U)
                    (void)TMR_StartLowPowerTimer(mAppTimerId,
                       gTmrLowPowerSingleShotMillisTimer_c,
                       TmrMilliseconds(gAppWakeUpTimerAfterNoActivityMs),
                       WakeUpTimerCallback, NULL);
#endif
                    /* Go to lowpower when no longer RF activity */
                    PWR_SetNewAppState(LP_MASTER_APP_STATE_NO_ACTIVITY);
#endif
                }
            }
        }
        break;

        case gScanCommandFailed_c:
        {
            ; /* No action required */
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
            /* Update UI */
            shell_write("\r\nConnected!\r\n");
            APP_DBG_LOG("Connected");

            mPeerInformation.deviceId = peerDeviceId;
            mPeerInformation.isBonded = FALSE;

            /* Set low power mode */
            PWR_SetNewAppState(PWR_APP_STATE_CONN);

#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
            (void)Gap_CheckIfBonded(peerDeviceId, &mPeerInformation.isBonded, NULL);

            if ((mPeerInformation.isBonded) &&
                (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId,
                    (void*) &mPeerInformation.customInfo, 0, sizeof (appCustomInfo_t))))
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

            (void)TMR_StopTimer(mAppTimerId);

            /* Update UI */
            shell_write("\r\nDisconnected!\r\n");
            APP_DBG_LOG("Disconnected");

#if gAppRestartScanAfterConnect
            BleApp_Start();
#else
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
#if defined(gAppStateNoActivityRamRetention_d) && (gAppStateNoActivityRamRetention_d == 1)
            shell_write("\r\nGoing into lowpower (RAMRET)\r\n");
#else
            shell_write("\r\nGoing into lowpower (RAMOFF)\r\n");
#endif
#if defined(gAppWakeUpTimerAfterNoActivityMs) && (gAppWakeUpTimerAfterNoActivityMs != 0U)
            (void)TMR_StartLowPowerTimer(mAppTimerId,
               gTmrLowPowerSingleShotMillisTimer_c,
               TmrMilliseconds(gAppWakeUpTimerAfterNoActivityMs),
               WakeUpTimerCallback, NULL);
#endif
            PWR_SetNewAppState(LP_MASTER_APP_STATE_NO_ACTIVITY);
#endif
#endif
        }
        break;

#if gAppUsePairing_d
        case gConnEvtPairingComplete_c:
        {
            /* Notify state machine handler on pairing complete */
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PairingComplete_c);
            }
        }
        break;

#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
        case gConnEvtEncryptionChanged_c:
        {
            if( pConnectionEvent->eventData.encryptionChangedEvent.newEncryptionState )
            {
                if( mRestoringBondedLink )
                {
                    /* Try to enable temperature notifications, disconnect on failure */
                    if( gBleSuccess_c != BleApp_ConfigureTemperatureNotifications() )
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
* \param[in]    pService    Pointer to gattService_t.
********************************************************************************** */
static void BleApp_StoreServiceHandles(gattService_t *pService)
{
    uint8_t i,j;

    if ((pService->uuidType == gBleUuidType128_c) &&
        FLib_MemCmp(pService->uuid.uuid128, uuid_service_temperature, 16))
    {
        /* Found Primary Temperature Service */
        mPeerInformation.customInfo.tempClientConfig.hService = pService->startHandle;

        for (i = 0; i < pService->cNumCharacteristics; i++)
        {
            if ((pService->aCharacteristics[i].value.uuidType == gBleUuidType16_c) &&
                (pService->aCharacteristics[i].value.uuid.uuid16 == gBleSig_Temperature_d))
            {
                /* Found Temperature Characteristic */
                mPeerInformation.customInfo.tempClientConfig.hTemperature = pService->aCharacteristics[i].value.handle;

                for (j = 0; j < pService->aCharacteristics[i].cNumDescriptors; j++)
                {
                    if (pService->aCharacteristics[i].aDescriptors[j].uuidType == gBleUuidType16_c)
                    {
                        switch (pService->aCharacteristics[i].aDescriptors[j].uuid.uuid16)
                        {
                            /* Found Temperature Characteristic Presentation Format Descriptor */
                            case gBleSig_CharPresFormatDescriptor_d:
                            {
                                mPeerInformation.customInfo.tempClientConfig.hTempDesc = pService->aCharacteristics[i].aDescriptors[j].handle;
                                break;
                            }
                            /* Found Temperature Characteristic CCCD */
                            case gBleSig_CCCD_d:
                            {
                                mPeerInformation.customInfo.tempClientConfig.hTempCccd = pService->aCharacteristics[i].aDescriptors[j].handle;
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
    else if((pService->uuidType == gBleUuidType16_c) &&
             (pService->uuid.uuid16 == (uint16_t)gBleSig_BatteryService_d) )
    {
        /* Found Primary Battery service */
        mPeerInformation.customInfo.batteryClientConfig.hService = pService->startHandle;
        
        for (i = 0; i < pService->cNumCharacteristics; i++)
        {
            if ((pService->aCharacteristics[i].value.uuidType == gBleUuidType16_c) &&
                (pService->aCharacteristics[i].value.uuid.uuid16 == gBleSig_BatteryLevel_d))
            {
                /* Found Battery Level Characteristic */
                mPeerInformation.customInfo.batteryClientConfig.hBattery = pService->aCharacteristics[i].value.handle;

                for (j = 0; j < pService->aCharacteristics[i].cNumDescriptors; j++)
                {
                    if (pService->aCharacteristics[i].aDescriptors[j].uuidType == gBleUuidType16_c)
                    {
                        switch (pService->aCharacteristics[i].aDescriptors[j].uuid.uuid16)
                        {
                            /* Found Battery Characteristic Presentation Format Descriptor */
                            case gBleSig_CharPresFormatDescriptor_d:
                            {
                                mPeerInformation.customInfo.batteryClientConfig.hBatteryDesc = pService->aCharacteristics[i].aDescriptors[j].handle;
                                break;
                            }
                            /* Found Battery Characteristic CCCD */
                            case gBleSig_CCCD_d:
                            {
                                mPeerInformation.customInfo.batteryClientConfig.hBatteryCccd = pService->aCharacteristics[i].aDescriptors[j].handle;
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
    else
    {
        ; /* service is skipped */
    }
}

/*! *********************************************************************************
* \brief        Stores the value of the specified attribute.
*
* \param[in]    pDesc       Pointer to gattAttribute_t.
********************************************************************************** */
static void BleApp_StoreDescValues(gattAttribute_t *pDesc)
{
    if (pDesc->handle == mPeerInformation.customInfo.tempClientConfig.hTempDesc)
    {
        /* Store temperature format*/
        FLib_MemCpy(&mPeerInformation.customInfo.tempClientConfig.tempFormat,
                    pDesc->paValue,
                    pDesc->valueLength);
        mPeerInformation.customInfo.tempDescRead = TRUE;
    }
    else
    {
        /* in current implementation, must be battery service handle */
        assert(pDesc->handle == mPeerInformation.customInfo.batteryClientConfig.hBatteryDesc);

        /* Store battery level format*/
        FLib_MemCpy(&mPeerInformation.customInfo.batteryClientConfig.batteryFormat,
                    pDesc->paValue,
                    pDesc->valueLength);
        mPeerInformation.customInfo.battDescRead = TRUE;
    }
}

/*! *********************************************************************************
* \brief        Displays the temperature value to shell.
*
* \param[in]    temperature     Temperature value
********************************************************************************** */
static void BleApp_PrintTemperature(uint16_t temperature)
{
    APP_DBG_LOG("");

    shell_write("Temperature: ");
    shell_writeDec((uint32_t)temperature / 100UL);

    /* Add 'C' for Celsius degrees - UUID 0x272F.
       www.bluetooth.com/specifications/assigned-numbers/units */
    if (mPeerInformation.customInfo.tempClientConfig.tempFormat.unitUuid16 == 0x272FU)
    {
        shell_write(" C\r\n");
    }
    else
    {
        shell_write("\r\n");
    }
}

/*! *********************************************************************************
* \brief        Displays the battery level to shell.
*
* \param[in]    battery     Battery level
********************************************************************************** */
static void BleApp_PrintBattery(uint8_t battery)
{
    APP_DBG_LOG("");

    shell_write("Battery: ");
    shell_writeDec((uint32_t)battery);

    /* Add '%' for percentage - UUID 0x27AD.
       www.bluetooth.com/specifications/assigned-numbers/units */
    if (mPeerInformation.customInfo.batteryClientConfig.batteryFormat.unitUuid16 == 0x27ADU)
    {
        shell_write(" %\r\n");
    }
    else
    {
        shell_write("\r\n");
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
static void BleApp_GattClientCallback(deviceId_t serverDeviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error)
{
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
                    if( mpCharProcBuffer != NULL )
                    {
                        /* Store the value of the descriptor */
                        BleApp_StoreDescValues(mpCharProcBuffer);
                    }
                }
                break;

                case gGattProcWriteCharacteristicDescriptor_c:
                {
                    if( mpCharProcBuffer != NULL )
                    {
                        if( mPeerInformation.customInfo.tempNotifyEnabled == FALSE )
                        {
                            mPeerInformation.customInfo.tempNotifyEnabled = TRUE;
                        }
                        else
                        {
                            mPeerInformation.customInfo.battNotifyEnabled = TRUE;
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

            BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
        }
    }

    /* Signal Service Discovery Module */
    BleServDisc_SignalGattClientEvent(serverDeviceId, procedureType, procedureResult, error);

}

/*! *********************************************************************************
* \brief        Handles GATT client notification callback from host stack.
*
* \param[in]    serverDeviceId              GATT Server device ID.
* \param[in]    characteristicValueHandle   Handle.
* \param[in]    aValue                      Pointer to value.
* \param[in]    valueLength                 Value length.
********************************************************************************** */
static void BleApp_GattNotificationCallback(deviceId_t serverDeviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength)
{
    APP_DBG_LOG("");
    if (characteristicValueHandle == mPeerInformation.customInfo.tempClientConfig.hTemperature)
    {
        BleApp_PrintTemperature(Utils_ExtractTwoByteValue(aValue));

        /* Restart Wait For Data timer */
        (void)TMR_StartLowPowerTimer(mAppTimerId,
                       gTmrLowPowerSecondTimer_c,
                       TmrSeconds(gAppDisconnectTimeout_c),
                       DisconnectTimerCallback, NULL);
    }
    else if (characteristicValueHandle == mPeerInformation.customInfo.batteryClientConfig.hBattery)
    {
        /* battery value is only 1 byte */
        BleApp_PrintBattery(*aValue);
        /* Restart Wait For Data timer */
        (void)TMR_StartLowPowerTimer(mAppTimerId,
                       gTmrLowPowerSecondTimer_c,
                       TmrSeconds(gAppDisconnectTimeout_c),
                       DisconnectTimerCallback, NULL);
    }
    else
    {
        ; /* nothing to do with this notification */
    }
        
    DBG_LOG_DUMP(FALSE);
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
                In case extended advertisement is scanned.
*               This function is called from the scanning callback.
*
* \param[in]    pData                   Pointer to gapExtScannedDevice_t.
*
* \return       TRUE if the scanned device implements the Temperature Custom Service,
                FALSE otherwise
********************************************************************************** */
#if defined(gAppExtAdvEnable_d) && (gAppExtAdvEnable_d ==1)
static bool_t BleApp_CheckExtScanEvent(gapExtScannedDevice_t* pData)
{
    uint32_t index = 0U;
    uint8_t name[10];
    uint8_t nameLength = 0U;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->pData[index];
        adElement.adType = (gapAdType_t)pData->pData[index + 1U];
        adElement.aData = &pData->pData[index + 2U];

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
        }

        /* Move on to the next AD element type */
        index += (uint32_t)adElement.length + sizeof(uint8_t);
    }

    if (foundMatch == TRUE)
    {
        /* Update UI */
        shell_write("\r\nFound ExtAdv device: \r\n");
        if(nameLength > 0U)
        {
            shell_writeN((char*)name, (uint16_t)nameLength - 1U);
            SHELL_NEWLINE();
        }
        shell_writeHexLe(pData->aAddress, 6);
        SHELL_NEWLINE();
        shell_write("Tx Power: ");
        shell_writeSignedDec(pData->txPower);           /*Prints the txPower*/
        SHELL_NEWLINE();
        shell_write("Primary PHY: ");                   /*Prints the Primary PHY config*/
        shell_writeSignedDec(pData->primaryPHY);        /*1: 1M, 2: 2M, 3: Coded*/
        SHELL_NEWLINE();
        shell_write("Secondary PHY: ");                 /*Prints the Secondary PHY config*/
        shell_writeSignedDec(pData->secondaryPHY);      /*1: 1M, 2: 2M, 3: Coded*/
        SHELL_NEWLINE();
    }
    return foundMatch;
}
#endif

/*! *********************************************************************************
* \brief        Process scanning events to search for the Temperature Custom Service.
*               This function is called from the scanning callback.
*
* \param[in]    pData                   Pointer to gapScannedDevice_t.
*
* \return       TRUE if the scanned device implements the Temperature Custom Service,
                FALSE otherwise
********************************************************************************** */
static bool_t BleApp_CheckScanEvent(gapScannedDevice_t* pData)
{
    uint32_t index = 0U;
    uint8_t name[10];
    uint8_t nameLength = 0U;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1U];
        adElement.aData = &pData->data[index + 2U];

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
        }

        /* Move on to the next AD element type */
        index += (uint32_t)adElement.length + sizeof(uint8_t);
    }

    if (foundMatch == TRUE)
    {
        /* Update UI */
        shell_write("\r\nFound device: \r\n");
        if(nameLength > 0U)
        {
            shell_writeN((char*)name, (uint16_t)nameLength - 1U);
            SHELL_NEWLINE();
        }
        shell_writeHexLe(pData->aAddress, 6);
    }
    return foundMatch;
}

/*! *********************************************************************************
* \brief        State machine handler of the Temperature Collector application.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
********************************************************************************** */
static void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event)
{
    switch (mPeerInformation.appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
                if( (mPeerInformation.customInfo.tempClientConfig.hTemperature != gGattDbInvalidHandle_d) &&\
                    (mPeerInformation.customInfo.batteryClientConfig.hBattery != gGattDbInvalidHandle_d) )
                {
                    /* Moving to Notification Configuration state and wait for Link encryption result */
                    mPeerInformation.appState = mAppConfigureNotification_c;
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
                        assert(0);
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
                if( mPeerInformation.customInfo.tempDescRead == TRUE )
                {
                    if( mPeerInformation.customInfo.battDescRead == FALSE )
                    {
                        /* once Temperature Descriptor has been read, read Battery Descriptor */
                        mpCharProcBuffer->handle = mPeerInformation.customInfo.batteryClientConfig.hBatteryDesc;
                        mpCharProcBuffer->paValue = (uint8_t*)(mpCharProcBuffer + 1);
                        (void)GattClient_ReadCharacteristicDescriptor(mPeerInformation.deviceId, mpCharProcBuffer, gAttDefaultMtu_c);
                    }
                    else
                    {
                        /* Try to enable Temperature notifications, disconnect on failure
                           Battery notifications will be enabled in mAppConfigureNotification_c state */
                        if( (gBleSuccess_c == BleApp_ConfigureTemperatureNotifications()) )
                        {
                            mPeerInformation.appState = mAppConfigureNotification_c;
                        }
                        else
                        {
                            (void)Gap_Disconnect(peerDeviceId);
                        }
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

        case mAppConfigureNotification_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                assert( mPeerInformation.customInfo.batteryClientConfig.hBatteryCccd != 0U );

                /* Try to enable Battery notifications, disconnect on failure */
                if( (gBleSuccess_c == BleApp_ConfigureBatteryNotifications()) )
                {
                    /* notification configuration is complete, move to running state */
                    mPeerInformation.appState = mAppRunning_c;
                }
                else
                {
                    (void)Gap_Disconnect(peerDeviceId);
                }
            }
            else
            {
                if(event == mAppEvt_PairingComplete_c)
                {
                    /* Try to enable Temperature notifications, disconnect on failure
                       Battery notifications will be enabled at next mAppEvt_GattProcComplete_c event */
                    if( gBleSuccess_c != BleApp_ConfigureTemperatureNotifications() )
                    {
                        (void)Gap_Disconnect(peerDeviceId);
                    }
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
                               TmrSeconds(gAppDisconnectTimeout_c),
                               DisconnectTimerCallback, NULL);
            }
            else
            {
                if (event == mAppEvt_PairingComplete_c)
                {
                    /* Pairing should be completed before being in mAppRunning state */
                    assert(0);
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
static bleResult_t BleApp_ConfigureTemperatureNotifications(void)
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
        /* Prepare CCCD write request */
        mpCharProcBuffer->handle = mPeerInformation.customInfo.tempClientConfig.hTempCccd;
        mpCharProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
        mpCharProcBuffer->valueLength = 0;
        /* Write Temperature CCCD */
        (void)GattClient_WriteCharacteristicDescriptor(mPeerInformation.deviceId,
                                                       mpCharProcBuffer,
                                                       (uint16_t)sizeof(value), (void*)&value);
    }
    else
    {
        assert(0);
        result = gBleOutOfMemory_c;
    }

    return result;
}

/*! *********************************************************************************
* \brief        Enable battery notifications by writing peer's CCCD of the
                Battery characteristic.
*
* \return       gBleSuccess_c or gBleOutOfMemory_c
********************************************************************************** */
static bleResult_t BleApp_ConfigureBatteryNotifications(void)
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
        /* Prepare CCCD write request */
        mpCharProcBuffer->handle = mPeerInformation.customInfo.batteryClientConfig.hBatteryCccd;
        mpCharProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
        mpCharProcBuffer->valueLength = 0;
        /* Write Battery CCCD */
        (void)GattClient_WriteCharacteristicDescriptor(mPeerInformation.deviceId,
                                                       mpCharProcBuffer,
                                                       (uint16_t)sizeof(value), (void*)&value);
    }
    else
    {
        assert(0);
        result = gBleOutOfMemory_c;
    }

    return result;
}

/*! *********************************************************************************
* \brief        Stop scanning after a given time (gAppScanningTimeout_c).
*               Called on timer task.
*
* \param[in]    pParam              not used
********************************************************************************** */
static void ScanningTimeoutTimerCallback(void* pParam)
{
    /* Stop scanning */
    if (mScanningOn)
    {
         APP_DBG_LOG("");
        (void)Gap_StopScanning();
    }
    else
    {
         APP_DBG_LOG("error");
         assert(0);
    }
}

/*! *********************************************************************************
* \brief        Disconnect from peer device if no data was received for a given time.
*               (gAppDisconnectTimeout_c). Called on timer task.
*
* \param[in]    pParam              not used
********************************************************************************** */
static void DisconnectTimerCallback(void* pParam)
{
    /* Disconnect from peer device */
    if (mPeerInformation.deviceId != gInvalidDeviceId_c)
    {
         APP_DBG_LOG("");
        (void)Gap_Disconnect(mPeerInformation.deviceId);
         DBG_LOG_DUMP(FALSE);
    }
}

/*! *********************************************************************************
* \brief        Handles wake up after no activity timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
#if defined(gAppWakeUpTimerAfterNoActivityMs) && (gAppWakeUpTimerAfterNoActivityMs != 0U)
static void WakeUpTimerCallback(void* pParam)
{
    APP_DBG_LOG("");
    /* Start scanning again  */
    BleApp_Start();
}
#endif

/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
static void BleApp_PrintLePhyEvent(gapPhyEvent_t* pPhyEvent)
{
    /* String dictionary corresponding to gapLePhyMode_t */
    static const char* mLePhyModeStrings[] =
    {
        "Invalid\n\r",
        "1M\n\r",
        "2M\n\r",
        "Coded\n\r",
    };
    uint8_t txPhy = (pPhyEvent->txPhy <= gLePhyCoded_c) ? pPhyEvent->txPhy : 0;
    uint8_t rxPhy = (pPhyEvent->rxPhy <= gLePhyCoded_c) ? pPhyEvent->rxPhy : 0;
    shell_write("Phy Update Complete.\n\r");
    shell_write("TxPhy ");
    shell_write(mLePhyModeStrings[txPhy]);
    shell_write("RxPhy ");
    shell_write(mLePhyModeStrings[rxPhy]);
}
/*! *********************************************************************************
* @}
********************************************************************************** */
