/*! *********************************************************************************
* \addtogroup Location and Navigation Sensor
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Location and Navigation Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"

/* Framework / Drivers */
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "SerialManager.h"
#include "Panic.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#if !defined(MULTICORE_APPLICATION_CORE) || (!MULTICORE_APPLICATION_CORE)
#include "gatt_db_handles.h"
#endif

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "location_and_navigation_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"
#include "ble_service_discovery.h"

#include "board.h"
#include "ApplMain.h"
#include "location_and_navigation_client.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1U)
#include "erpc_host.h"
#endif

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
/* appEvent_t */
#define mAppEvt_PeerConnected_c                 0x00U
#define mAppEvt_PairingComplete_c               0x01U
#define mAppEvt_ServiceDiscoveryComplete_c      0x02U
#define mAppEvt_ServiceDiscoveryFailed_c        0x03U
#define mAppEvt_GattProcComplete_c              0x04U
#define mAppEvt_GattProcError_c                 0x05U

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef uint8_t appEvent_t;

typedef enum appState_tag{
    mAppIdle_c,
    mAppExchangeMtu_c,
    mAppServiceDisc_c,
    mAppEnableNotifications_c,
    mAppRunning_c,
}appState_t;

/*! Location and Navigation Client - Configuration */
typedef struct lncConfig_tag
{
    uint16_t    hService;
    uint16_t    hLNReport;
    uint16_t    hLNReportCccd;
} lncConfig_t;

typedef struct appCustomInfo_tag
{
    lncConfig_t     lncConfig;
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

#if gAppUseBonding_d
static bool_t mRestoringBondedLink = FALSE;
static bool_t mAuthRejected = FALSE;
#endif

static bool_t   mScanningOn = FALSE;

/* Buffer used for Characteristic related procedures */
static gattAttribute_t      *mpCharProcBuffer = NULL;
static uint8_t mSerialId = gSerialMgrInvalidIdx_c;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
/* Host Stack callbacks */
static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattClientCallback (deviceId_t serverDeviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error);
static void BleApp_ServiceDiscoveryCallback (deviceId_t peerDeviceId, servDiscEvent_t* pEvent);
static void BleApp_Config (void);
static void BleApp_StateMachineHandler (deviceId_t peerDeviceId, uint8_t event);
static bool_t CheckScanEvent (gapScannedDevice_t* pData);
static void BleApp_StoreServiceHandles(deviceId_t peerDeviceId, gattService_t *pService);
static void BleApp_GattNotificationCallback
(
    deviceId_t          serverDeviceId,
    uint16_t characteristicValueHandle,
    uint8_t* aValue,
    uint16_t valueLength
);
static bleResult_t BleApp_ConfigureNotifications(deviceId_t peerDeviceId);
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
    /* Initializa the serial interface */
#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    Serial_InitManager();
#else
    SerialManager_Init();
#endif
    (void)Serial_InitInterface (&mSerialId,
                                APP_SERIAL_INTERFACE_TYPE,
                                APP_SERIAL_INTERFACE_INSTANCE);
     (void)Serial_SetBaudRate (mSerialId, APP_SERIAL_INTERFACE_SPEED);

     (void)Serial_Print (mSerialId, "\n\r Location and Navigation Client Demo \n\r", gNoBlock_d);
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1U)
    /* Init eRPC host */
    init_erpc_host();
#endif
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    if (!mScanningOn)
    {
        (void)App_StartScanning(&gScanParams, BleApp_ScanningCallback, gGapDuplicateFilteringEnable_c, gGapScanContinuously_d, gGapScanPeriodicDisabled_d);
    }
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
        case gKBD_EventPressPB1_c:
        {
            BleApp_Start();
            break;
        }
        case gKBD_EventLongPB1_c:
        {
            if (mPeerInformation.deviceId != gInvalidDeviceId_c)
            {
                (void)Gap_Disconnect(mPeerInformation.deviceId);
            }
            break;
        }
        case gKBD_EventPressPB2_c:
        case gKBD_EventLongPB2_c:
        default:
            ; /* For MISRA compliance */
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
        default:
            ; /* For MISRA compliance */
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

    mPeerInformation.appState = mAppIdle_c;

    mScanningOn = FALSE;
    (void)Serial_Print (mSerialId, "\r\nPress SCANSW to connect to a Location and Navigation Server!\r\n", gNoBlock_d);
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
            if (CheckScanEvent(&pScanningEvent->eventData.scannedDevice))
            {
                gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                FLib_MemCpy(gConnReqParams.peerAddress,
                            pScanningEvent->eventData.scannedDevice.aAddress,
                            sizeof(bleDeviceAddress_t));

                (void)Gap_StopScanning();
                (void)App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
            }
        }
        break;
        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;
            if (mScanningOn)
            {
                LED_StopFlashingAllLeds();
                Led1Flashing();
                (void)Serial_Print (mSerialId, "\n\rScanning...\n\r", gNoBlock_d);
            }
        }
        break;
    default:
        ; /* For MISRA compliance */
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
            /* UI */
            LED_StopFlashingAllLeds();
            Led1On();
            (void)Serial_Print (mSerialId, "\n\rConnected!\n\r", gNoBlock_d);

            mPeerInformation.deviceId = peerDeviceId;
            mPeerInformation.isBonded = FALSE;

#if gAppUseBonding_d
            Gap_CheckIfBonded(peerDeviceId, &mPeerInformation.isBonded);

            if ((mPeerInformation.isBonded) &&
                (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId,
                    (void*) &mPeerInformation.customInfo, 0, sizeof (appCustomInfo_t))))
            {
                mRestoringBondedLink = TRUE;
                /* Restored custom connection information. Encrypt link */
                Gap_EncryptLink(peerDeviceId);
            }
            else
            {
                mRestoringBondedLink = FALSE;
                mAuthRejected = FALSE;
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

            /* UI */
            LED_TurnOffAllLeds();
            LED_StartFlash(LED_ALL);
            (void)Serial_Print (mSerialId, "\n\rDisconnected!\n\r", gNoBlock_d);

        }
        break;

#if gAppUsePairing_d
        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PairingComplete_c);
            }
        }
        break;

#if gAppUseBonding_d
        case gConnEvtEncryptionChanged_c:
        {
            if (pConnectionEvent->eventData.encryptionChangedEvent.newEncryptionState)
            {
                if ((TRUE == mRestoringBondedLink) &&
                     (FALSE == mAuthRejected))
                {
                    if (gBleSuccess_c != BleApp_ConfigureNotifications(peerDeviceId))
                    {
                        (void)Gap_Disconnect(peerDeviceId);
                    }
                }
                else
                {
                    mRestoringBondedLink = FALSE;
                }
            }
        }
        break;

        case gConnEvtAuthenticationRejected_c:
        {
            /* Start Pairing Procedure */
            mAuthRejected = TRUE;
            (void)Gap_Pair(peerDeviceId, &gPairingParameters);
        }
        break;
#endif /* gAppUseBonding_d */
#endif /* gAppUsePairing_d */

    default:
        ; /* For MISRA compliance */
        break;
    }
}

/*! *********************************************************************************
* \brief        Handler function for the service discovery procedure events
*
* \param[in]    peerDeviceId          The Id of the peer device
* \param[in]    pEvent                pointer to the service discovery eent
*
********************************************************************************** */
static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent)
{
    switch(pEvent->eventType)
    {
    case gServiceDiscovered_c:
        {
            BleApp_StoreServiceHandles(peerDeviceId, pEvent->eventData.pService);
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
        ; /* For MISRA compliance */
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
static void BleApp_GattClientCallback
(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
)
{
    if (procedureResult == gGattProcError_c)
    {
        attErrorCode_t attError = (attErrorCode_t)((uint8_t)error);

        if (attError == gAttErrCodeInsufficientEncryption_c     ||
            attError == gAttErrCodeInsufficientAuthorization_c  ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            /* Start Pairing Procedure */
            (void)Gap_Pair(serverDeviceId, &gPairingParameters);
        }

        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
    }
    else if (procedureResult == gGattProcSuccess_c)
    {
        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
    }
    else
    {
        ; /* For MISRA compliance */
    }

    /* Signal Service Discovery Module */
    BleServDisc_SignalGattClientEvent(serverDeviceId, procedureType,procedureResult, error);

}

/*! *********************************************************************************
* \brief        Stores handles for the specified service.
*
* \param[in]    pService    Pointer to gattService_t.
********************************************************************************** */
static void BleApp_StoreServiceHandles
(
    deviceId_t    peerDeviceId,
    gattService_t *pService
)
{
    uint8_t i, j;

    if ((pService->uuidType == gBleUuidType16_c) &&
        (pService->uuid.uuid16 == gBleSig_LocationAndNavigationService_d))
    {
        /* Found Location and Navigation Service */
        mPeerInformation.customInfo.lncConfig.hService = pService->startHandle;

        for (i = 0; i < pService->cNumCharacteristics; i++)
        {
            if ((pService->aCharacteristics[i].value.uuidType == gBleUuidType16_c) &&
                (pService->aCharacteristics[i].value.uuid.uuid16 == gBleSig_LocationAndSpeed_d))
            {
                /* Found Location and Speed Char */
                mPeerInformation.customInfo.lncConfig.hLNReport = pService->aCharacteristics[i].value.handle;

                for (j = 0; j < pService->aCharacteristics[i].cNumDescriptors; j++)
                {
                    if (( pService->aCharacteristics[i].aDescriptors[j].uuidType == gBleUuidType16_c) &&
                        ( pService->aCharacteristics[i].aDescriptors[j].uuid.uuid16 == gBleSig_CCCD_d))
                    {
                        mPeerInformation.customInfo.lncConfig.hLNReportCccd =  pService->aCharacteristics[i].aDescriptors[j].handle;
                    }
                } /* for(...) */
            }
        } /* for(...) */
    }
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
    union
    {
        uint8_t*            aValueTemp;
        lnsLocAndSpeed_t*   lnsLocAndSpeedTemp;
    }lnsLocAndSpeedVars;

    if (characteristicValueHandle == mPeerInformation.customInfo.lncConfig.hLNReport)
    {
        lnsLocAndSpeedVars.aValueTemp = aValue;
        lnsLocAndSpeed_t *p = lnsLocAndSpeedVars.lnsLocAndSpeedTemp;

        (void)Serial_Print (mSerialId, "\n\r\n\rReceived Report:\n\r================", gNoBlock_d);

        /* If the notification contains the instantaneous speed, print it. */
        if ((p->lnsLocAndSpeedFlags & gLns_InstantaneousSpeedSupported_c) != 0U)
        {
            (void)Serial_Print (mSerialId, "\n\rInstantaneous Speed: ", gNoBlock_d);
            (void)Serial_PrintDec (mSerialId, p->lnsInstantaneousSpeed);
        }

        /* If the notification contains the total distance, print it. */
        if ((p->lnsLocAndSpeedFlags & gLns_TotalDistanceSupported_c) != 0U)
        {
            (void)Serial_Print (mSerialId, "\n\rTotal Distance: ", gNoBlock_d);
            (void)Serial_PrintDec (mSerialId, p->lnsTotalDistance);
        }

        /* If the notification contains the location, print it. */
        if ((p->lnsLocAndSpeedFlags & gLns_LocationSupported_c) != 0U)
        {
            (void)Serial_Print (mSerialId, "\n\rLocation: ", gNoBlock_d);
            (void)Serial_PrintDec (mSerialId, (uint32_t)p->lnsLongitude);
            (void)Serial_Print (mSerialId, ", ", gNoBlock_d);
            (void)Serial_PrintDec (mSerialId, (uint32_t)p->lnsLatitude);
        }

        /* If the notification contains the total distance, print it. */
        if ((p->lnsLocAndSpeedFlags & gLns_ElevationSupported_c) != 0U)
        {
            (void)Serial_Print (mSerialId, "\n\rElevation: ", gNoBlock_d);
            (void)Serial_PrintDec (mSerialId, (uint32_t)p->lnsElevation);
        }

        /* If the notification contains the UTC time, print it. */
        if ((p->lnsLocAndSpeedFlags & gLns_UtcTimeSupported_c) != 0U)
        {
            (void)Serial_Print (mSerialId, "\n\rUTC Time: ", gNoBlock_d);
            (void)Serial_PrintDec (mSerialId, p->lnsUtcTime);
        }
    }
}


/*! *********************************************************************************
* \brief        Check the scan event for specific service.
*
* \param[in]    pElement              pointer to the advertising element structure
* \param[in]    pData                 pointer to the data to match in advertising element
* \param[in]    iDataLen              length of data to match
*
* \return TRUE if a match was found.
*
********************************************************************************** */
static bool_t MatchDataInAdvElementList(gapAdStructure_t *pElement, void *pData, uint8_t iDataLen)
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
* \brief        Check the scan event for specific service.
*
* \param[in]    pData              pointer to the device information structure.
*
* \return TRUE if the required service was found, else FALSE.
*
********************************************************************************** */
static bool_t CheckScanEvent(gapScannedDevice_t* pData)
{
    uint8_t index = 0;
    uint8_t name[10];
    uint8_t nameLength = 0U;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1U];
        adElement.aData = &pData->data[index + 2U];

         /* Search for Location and Navigation Service */
        if ((adElement.adType == gAdIncomplete16bitServiceList_c) ||
            (adElement.adType == gAdComplete16bitServiceList_c))
        {
            uint16_t uuid = gBleSig_LocationAndNavigationService_d;
            foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));
        }

        /* Search for device name */
        if ((adElement.adType == gAdShortenedLocalName_c) ||
            (adElement.adType == gAdCompleteLocalName_c))
        {
            nameLength = MIN(adElement.length, 10U);
            FLib_MemCpy(name, adElement.aData, nameLength);
        }

        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
    }

    if (foundMatch && (nameLength > 0U))
    {
        /* UI: print device name */
        (void)Serial_Print (mSerialId, "\n\rFound device:\n\r", gNoBlock_d);
        (void)Serial_SyncWrite (mSerialId, name, (uint16_t)nameLength-1U);
        /* UI: print device address */
        (void)Serial_Print (mSerialId, "\n\r", gNoBlock_d);
        (void)Serial_PrintHex (mSerialId, pData->aAddress,6, gPrtHexSpaces_c);
    }
    return foundMatch;
}

/*! *********************************************************************************
* \brief        Handles application events.
*
* \param[in]    peerDeviceId              The device ID.
* \param[in]    event                     application event
*
********************************************************************************** */
static void BleApp_StateMachineHandler(deviceId_t peerDeviceId, uint8_t event)
{
    switch (mPeerInformation.appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
#if gAppUseBonding_d
                if (mRestoringBondedLink &&
                      (mPeerInformation.customInfo.lncConfig.hLNReportCccd != gGattDbInvalidHandle_d))
                {
                    /* Moving to Running State and wait for Link encryption result */
                    mPeerInformation.appState = mAppRunning_c;
                }
                else
#endif
                {
                    /* Moving to Exchange MTU State */
                    mPeerInformation.appState = mAppExchangeMtu_c;
                    (void)GattClient_ExchangeMtu(peerDeviceId);
                }
            }
        }
        break;

        case mAppExchangeMtu_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* Moving to Primary Service Discovery State*/
                mPeerInformation.appState = mAppServiceDisc_c;

                /* Start Service Discovery*/
                if (gBleSuccess_c != BleServDisc_Start(peerDeviceId))
                {
                    (void)Gap_Disconnect(peerDeviceId);
                }
            }
            else if (event == mAppEvt_GattProcError_c)
            {
                (void)Gap_Disconnect(peerDeviceId);
            }
            else
            {
                ; /* For MISRA compliance */
            }
        }
        break;

        case mAppServiceDisc_c:
        {
            if (event == mAppEvt_ServiceDiscoveryComplete_c)
            {
                /* Moving to Primary Service Discovery State*/
                mPeerInformation.appState = mAppEnableNotifications_c;

                if (mPeerInformation.customInfo.lncConfig.hLNReportCccd != gGattDbInvalidHandle_d)
                {
                    mpCharProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + 23U);

                    if (mpCharProcBuffer != NULL)
                    {
                        mpCharProcBuffer->handle = mPeerInformation.customInfo.lncConfig.hLNReportCccd;
                        mpCharProcBuffer->paValue = (uint8_t*)(mpCharProcBuffer + 1);
                        (void)GattClient_ReadCharacteristicDescriptor(mPeerInformation.deviceId, mpCharProcBuffer ,23);
                    }
                }
            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                (void)Gap_Disconnect(peerDeviceId);
            }
            else
            {
                ; /* For MISRA compliance */
            }
        }
        break;

        case mAppEnableNotifications_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mPeerInformation.customInfo.lncConfig.hLNReportCccd != 0U)
                {
                    /* Try to enable notifications, disconnect on failure */
                    if (gBleSuccess_c != BleApp_ConfigureNotifications(peerDeviceId))
                    {
                        (void)Gap_Disconnect(peerDeviceId);
                    }
                    else
                    {
                        /* Moving to Running State */
                        mPeerInformation.appState = mAppRunning_c;
                    }
                }

            }
            else if (event == mAppEvt_PairingComplete_c)
            {
                /* Continue after pairing is complete */
                (void)GattClient_ReadCharacteristicDescriptor(peerDeviceId, mpCharProcBuffer ,23);
            }
            else
            {
                ; /* For MISRA compliance */
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

#if gAppUseBonding_d
                /* Write data in NVM */
                (void)Gap_SaveCustomPeerInformation(peerDeviceId,
                                              (void*) &mPeerInformation.customInfo, 0,
                                              sizeof (appCustomInfo_t));
#endif
            }
            else if (event == mAppEvt_PairingComplete_c)
            {
                if (gBleSuccess_c != BleApp_ConfigureNotifications(peerDeviceId))
                {
                    (void)Gap_Disconnect(peerDeviceId);
                }
            }
            else
            {
                ; /* For MISRA compliance */
            }
        }
        break;

    default:
        ; /* For MISRA compliance */
        break;
    }
}

static bleResult_t BleApp_ConfigureNotifications(deviceId_t peerDeviceId)
{
    bleResult_t result = gBleSuccess_c;
    uint16_t value = gCccdNotification_c;

    if (mpCharProcBuffer == NULL )
    {
        mpCharProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + gAttDefaultMtu_c);
    }

    if (mpCharProcBuffer != NULL )
    {
        mpCharProcBuffer->handle = mPeerInformation.customInfo.lncConfig.hLNReportCccd;
        mpCharProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
        mpCharProcBuffer->valueLength = 0;
        (void)GattClient_WriteCharacteristicDescriptor(peerDeviceId,
                                                 mpCharProcBuffer,
                                                 sizeof(value), (void*)&value);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
