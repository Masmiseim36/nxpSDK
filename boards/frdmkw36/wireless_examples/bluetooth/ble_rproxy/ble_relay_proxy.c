/*! *********************************************************************************
 * \addtogroup Bluetooth Relay Proxy
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Bluetooth Relay Proxy
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
#include "Panic.h"
#include "MemManager.h"
#include "board.h"

/* BLE Host Stack */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_database.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_dynamic.h"

#include "ble_conn_manager.h"
#include "ble_service_discovery.h"

#include "ApplMain.h"
#include "ble_relay_proxy.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
#include "erpc_host.h"
#endif

/************************************************************************************
*************************************************************************************
* Private macros
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
    mAppRunning_c,
}appState_t;

typedef struct appGattDbHandles_tag
{
    uint16_t handle;
    uint16_t proxyHandle;
}appGattDbHandles_t;

typedef struct appCustomInfo_tag
{
    uint8_t dummy;
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

static appPeerInfo_t    mPeerProxyInformation;

#if gAppUseBonding_d
static bool_t mRestoringBondedLink = FALSE;
#endif

/* Scan State */
static bool_t   mScanningOn = FALSE;
static gapScannedDevice_t   *mpScannedDevice = NULL;


/* Adv State */
static bool_t      mAdvOn = FALSE;
static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;

/* Buffer used for Characteristic related procedures */
static gattCharacteristic_t     mCharProcBuffer;

/* Table with the GATT Database handle correspondence */
static appGattDbHandles_t maGattDbHandles[gMaxReadNotificationHandles_c];
static uint8_t mcNextHandle = 0;

static uint16_t maGattDbReadNotifyHandles[gMaxReadNotificationHandles_c];
static uint8_t mcNextReadNotifyHandle = 0;

static uint16_t maGattDbWriteNotifyHandles[gMaxWriteNotificationHandles_c];
static uint8_t mcNextWriteNotifyHandle = 0;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Host Stack callback */
static void BleApp_ScanningCallback
(
    gapScanningEvent_t* pScanningEvent
);

static void BleApp_AdvertisingCallback
(
    gapAdvertisingEvent_t* pAdvertisingEvent
);

static void BleApp_ConnectionCallback
(
    deviceId_t peerDeviceId,
    gapConnectionEvent_t* pConnectionEvent
);

static void BleApp_GattClientCallback
(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);

static void BleApp_GattServerCallback
(
    deviceId_t          deviceId,
    gattServerEvent_t*  pServerEvent
);

static void BleApp_GattNotificationCallback
(
    deviceId_t  serverDeviceId,
    uint16_t    characteristicValueHandle,
    uint8_t*    aValue,
    uint16_t    valueLength
);

static void BleApp_GattIndicationCallback
(
    deviceId_t  serverDeviceId,
    uint16_t    characteristicValueHandle,
    uint8_t*    aValue,
    uint16_t    valueLength
);

static void BleApp_ServiceDiscoveryCallback
(
    deviceId_t deviceId,
    servDiscEvent_t* pEvent
);

static void BleApp_Config(void);

void BleApp_StateMachineHandler
(
    deviceId_t peerDeviceId,
    uint8_t event
);

static bool_t CheckScanEvent(gapScannedDevice_t* pData);

static void BleApp_ConfigureAdv(void);

static bleResult_t BleApp_SaveService(gattService_t*    pService);

uint16_t BleApp_GetTransalatedHandle(uint16_t proxyHandle);

uint16_t BleApp_GetProxyHandle(uint16_t serviceHandle);

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
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
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
        LED_StopFlashingAllLeds();
        Led1Flashing();
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
            if (mPeerProxyInformation.deviceId != gInvalidDeviceId_c)
            {
                (void)Gap_Disconnect(mPeerProxyInformation.deviceId);
            }

            if (mPeerDeviceId != gInvalidDeviceId_c)
            {
                (void)Gap_Disconnect(mPeerDeviceId);
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

        case gAdvertisingParametersSetupComplete_c:
        {
            (void)Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
        }
        break;

        case gAdvertisingDataSetupComplete_c:
        {
            if (mPeerProxyInformation.deviceId != gInvalidDeviceId_c)
            {
                (void)App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
            }
        }
        break;

        case gAdvertisingSetupFailed_c:
        {
            panic(0,0,0,0);
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
static void BleApp_Config (void)
{
    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();

    /* Set advertising parameters*/
    (void)Gap_SetAdvertisingParameters(&gAdvParams);

    /* Register for callbacks*/
    (void)App_RegisterGattServerCallback(BleApp_GattServerCallback);
    (void)App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    (void)App_RegisterGattClientIndicationCallback(BleApp_GattIndicationCallback);
    (void)App_RegisterGattClientNotificationCallback(BleApp_GattNotificationCallback);

    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);

    mScanningOn = FALSE;
    mAdvOn = FALSE;

    mPeerDeviceId = gInvalidDeviceId_c;
    mPeerProxyInformation.deviceId = gInvalidDeviceId_c;
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

                /* Store AD Event if match is found */
                mpScannedDevice = MEM_BufferAlloc((uint32_t)pScanningEvent->eventData.scannedDevice.dataLength + sizeof(gapScannedDevice_t));
                if (NULL != mpScannedDevice)
                {
                    FLib_MemCpy(mpScannedDevice, &pScanningEvent->eventData.scannedDevice, sizeof(gapScannedDevice_t));
                    mpScannedDevice->data = (uint8_t *)mpScannedDevice + sizeof(gapScannedDevice_t);
                    FLib_MemCpy(mpScannedDevice->data, pScanningEvent->eventData.scannedDevice.data, (uint32_t)mpScannedDevice->dataLength);
                    (void)Gap_StopScanning();
#if gAppUsePrivacy_d
                    gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif
                    (void)App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                }
                else
                {
                    panic(0,0,0,0);
                }
            }
        }
        break;
        case gScanStateChanged_c:
        {
        }
        break;
        case gScanCommandFailed_c:
        {
            break;
        }
    default:
        ; /* For MISRA compliance */
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
            mAdvOn = !mAdvOn;
            LED_StopFlashingAllLeds();
            Led1Flashing();

            if(!mAdvOn)
            {
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            panic(0,0,0,0);
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
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            /* UI */
            LED_StopFlashingAllLeds();
            Led1On();

            mAdvOn = FALSE;

            BleApp_StateMachineHandler(peerDeviceId, (uint8_t)mAppEvt_PeerConnected_c);
        }
        break;

        case gConnEvtDisconnected_c:
        {
            /* Proxied device has disconnected */
            if (mPeerProxyInformation.deviceId == peerDeviceId)
            {
                mPeerProxyInformation.deviceId = gInvalidDeviceId_c;

                /* Reset Service Discovery to be sure*/
                BleServDisc_Stop(peerDeviceId);

                /* If the phone is conncted */
                if (mPeerDeviceId != gInvalidDeviceId_c)
                {
                    /* Disconnect */
                    (void)Gap_Disconnect(mPeerDeviceId);
                }

                /* If advertising is on, stop it */
                if (mAdvOn)
                {
                    (void)Gap_StopAdvertising();
                }

                /* Move application state back to idle */
                mPeerProxyInformation.appState = mAppIdle_c;

                /* Free the scan device data */
                if (mpScannedDevice != NULL)
                {
                    (void)MEM_BufferFree(mpScannedDevice);
                    mpScannedDevice = NULL;
                }

                /* Clear and init the device dynamic database */
                if (gBleSuccess_c == GattDbDynamic_ReleaseDatabase())
                {
                    (void)GattDbDynamic_Init();
                }

                if(mCharProcBuffer.value.paValue != NULL)
                {
                    (void)MEM_BufferFree(mCharProcBuffer.value.paValue);
                    mCharProcBuffer.value.paValue = NULL;
                }

                /* UI */
                LED_TurnOffAllLeds();
                LED_StartFlash(LED_ALL);
            }
            else if (mPeerDeviceId == peerDeviceId)
            {
                mPeerDeviceId = gInvalidDeviceId_c;

                /* If the phone has disconnected but the cloned device is still connected */
                if (mPeerProxyInformation.deviceId != gInvalidDeviceId_c)
                {
                    /* Restart advertising on proxy */
                    (void)App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
                }
                /* UI */
                Led1Flashing();
            }
            else
            {
                ; /* For MISRA compliance */
            }
        }
        break;

#if gAppUsePairing_d
        case gConnEvtSlaveSecurityRequest_c:
        {
            /* Start Pairing Procedure */
            gPairingParameters.localIoCapabilities = gIoKeyboardDisplay_c;
            Gap_Pair(peerDeviceId, &gPairingParameters);
        }
        break;

        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PairingComplete_c);
            }
        }
        break;

        case gConnEvtPairingRequest_c:
        {
            if (peerDeviceId == mPeerDeviceId)
            {
                gPairingParameters.localIoCapabilities = gIoDisplayOnly_c;
            }
        }
        break;
#endif /* gAppUsePairing_d */

    default:
        ; /* For MISRA compliance */
        break;
    }

    /* Connection Manager to handle Host Stack interactions */
    if (peerDeviceId == mPeerProxyInformation.deviceId)
    {
        BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);
    }
    else if (peerDeviceId == mPeerDeviceId)
    {
        BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);
    }
    else
    {
        ; /* For MISRA compliance */
    }
}

/*! *********************************************************************************
* \brief        Handles GATT server callback from host stack.
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    pServerEvent    Pointer to gattServerEvent_t.
********************************************************************************** */
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
    bleResult_t status;

    switch (pServerEvent->eventType)
    {
    case gEvtAttributeRead_c:
    {
        /* Received Read Request from Central on Proxy Server. Forward request to the Peripheral */
        mCharProcBuffer.value.handle = BleApp_GetTransalatedHandle(pServerEvent->eventData.attributeReadEvent.handle);

        if (mCharProcBuffer.value.handle > 0U)
        {
            mCharProcBuffer.value.paValue = MEM_BufferAlloc(23);
            status = GattClient_ReadCharacteristicValue(mPeerProxyInformation.deviceId, &mCharProcBuffer, 23);

            if (status != gBleSuccess_c)
            {
                (void)MEM_BufferFree(mCharProcBuffer.value.paValue);

                /* Send Read Attribute Response to the Initiator */
                (void)GattServer_SendAttributeReadStatus(deviceId, pServerEvent->eventData.attributeReadEvent.handle, (uint8_t)gAttErrCodeUnlikelyError_c);
            }
        }
    }
    break;

    case gEvtAttributeWrittenWithoutResponse_c:
    case gEvtAttributeWritten_c:
    {
        /* Received Write Request from Central on Proxy Server. Forward request to the Peripheral */
        mCharProcBuffer.value.handle = BleApp_GetTransalatedHandle(pServerEvent->eventData.attributeReadEvent.handle);

        if (mCharProcBuffer.value.handle > 0U)
        {
            status = GattClient_WriteCharacteristicValue(
                    mPeerProxyInformation.deviceId,
                    &mCharProcBuffer,
                    pServerEvent->eventData.attributeWrittenEvent.cValueLength,
                    pServerEvent->eventData.attributeWrittenEvent.aValue,
                    (0U != ((uint8_t)pServerEvent->eventType & (uint8_t)gEvtAttributeWrittenWithoutResponse_c)),
                    FALSE, FALSE, NULL);

            if ((status != gBleSuccess_c) &&
                    (0U != ((uint8_t)pServerEvent->eventType & (uint8_t)gEvtAttributeWritten_c)))
            {
                /* Send Write Attribute Response to the Initiator */
                (void)GattServer_SendAttributeWrittenStatus(deviceId, pServerEvent->eventData.attributeReadEvent.handle, (uint8_t)gAttErrCodeUnlikelyError_c);
            }
        }
    }
    break;

    case gEvtCharacteristicCccdWritten_c:
    {
        /* Received Write Request from Central on Proxy Server. Forward request to the Peripheral */
        mCharProcBuffer.value.handle = BleApp_GetTransalatedHandle(pServerEvent->eventData.attributeReadEvent.handle);

        if (mCharProcBuffer.value.handle > 0U)
        {
            status = GattClient_WriteCharacteristicDescriptor(
                    mPeerProxyInformation.deviceId,
                    &mCharProcBuffer.value,
                    pServerEvent->eventData.attributeWrittenEvent.cValueLength,
                    pServerEvent->eventData.attributeWrittenEvent.aValue);

            if (status != gBleSuccess_c)
            {
                /* Send Write Attribute Response to the Initiator */
                (void)GattServer_SendAttributeWrittenStatus(deviceId, pServerEvent->eventData.attributeReadEvent.handle, (uint8_t)gAttErrCodeUnlikelyError_c);
            }
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
static void BleApp_GattClientCallback(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
)
{
    union
    {
        uint8_t         errorTemp;
        attErrorCode_t  attErrorTemp;
    }errorVars;
    uint16_t handle;

    if (procedureResult == gGattProcError_c)
    {
        errorVars.errorTemp = (uint8_t)error & 0xFFU;
        attErrorCode_t attError = errorVars.attErrorTemp;
        if (attError == gAttErrCodeInsufficientEncryption_c     ||
            attError == gAttErrCodeInsufficientAuthorization_c  ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            /* Start Pairing Procedure */
            gPairingParameters.localIoCapabilities = gIoKeyboardDisplay_c;
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

    switch (procedureType)
    {
        case gGattProcReadCharacteristicValue_c:
        case gGattProcReadCharacteristicDescriptor_c:
        {
            /* Get corresponding handle in the local database */
            handle = BleApp_GetProxyHandle(mCharProcBuffer.value.handle);

            /* Write the value read to be in sync with the original device */
            (void)GattDb_WriteAttribute(handle,mCharProcBuffer.value.valueLength, mCharProcBuffer.value.paValue);

            /* Free buffer used for value */
            (void)MEM_BufferFree(mCharProcBuffer.value.paValue);
            mCharProcBuffer.value.paValue = NULL;

            /* Send Read Attribute Response to the Initiator */
            (void)GattServer_SendAttributeReadStatus(mPeerDeviceId, handle, (uint8_t)error & 0xFFU);
        }
        break;

        case gGattProcWriteCharacteristicValue_c:
        case gGattProcWriteCharacteristicDescriptor_c:
        {
            /* Get corresponding handle in the local database */
            handle = BleApp_GetProxyHandle(mCharProcBuffer.value.handle);

            /* Send Read Attribute Response to the Initiator */
            (void)GattServer_SendAttributeWrittenStatus(mPeerDeviceId, handle, (uint8_t)error & 0xFFU);
        }
        break;

        default:
            ; /* For MISRA compliance */
        break;
    }
}

static void BleApp_ServiceDiscoveryCallback(deviceId_t deviceId, servDiscEvent_t* pEvent)
{
    switch(pEvent->eventType)
    {
        case gServiceDiscovered_c:
        {
            if (BleApp_SaveService(pEvent->eventData.pService) != gBleSuccess_c)
            {
                (void)Gap_Disconnect(deviceId);
            }
        }
        break;

        case gDiscoveryFinished_c:
        {
            if (pEvent->eventData.success)
            {
                BleApp_StateMachineHandler(deviceId, mAppEvt_ServiceDiscoveryComplete_c);
            }
            else
            {
                BleApp_StateMachineHandler(deviceId, mAppEvt_ServiceDiscoveryFailed_c);
            }
        }
        break;

        default:
            ; /* For MISRA compliance */
        break;
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
    uint16_t handle;

    /* Get corresponding handle in the local database */
    handle = BleApp_GetProxyHandle(characteristicValueHandle);

    /* Forward message */
    (void)GattServer_SendInstantValueNotification(mPeerDeviceId, handle, valueLength, aValue);
}

/*! *********************************************************************************
* \brief        Handles GATT client indication callback from host stack.
*
* \param[in]    serverDeviceId              GATT Server device ID.
* \param[in]    characteristicValueHandle   Handle.
* \param[in]    aValue                      Pointer to value.
* \param[in]    valueLength                 Value length.
***********************************************************************************/
static void BleApp_GattIndicationCallback
(
    deviceId_t  serverDeviceId,
    uint16_t    characteristicValueHandle,
    uint8_t*    aValue,
    uint16_t    valueLength
)
{
    uint16_t handle;

    /* Get corresponding handle in the local database */
    handle = BleApp_GetProxyHandle(characteristicValueHandle);

    /* Forward message */
    (void)GattServer_SendInstantValueIndication(mPeerDeviceId, handle, valueLength, aValue);
}

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

static bool_t CheckScanEvent(gapScannedDevice_t* pData)
{
    uint8_t index = 0;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength && !foundMatch)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1U];
        adElement.aData = &pData->data[index + 2U];

         /* Search for Temperature Custom Service */
        if ((adElement.adType == gAdIncomplete16bitServiceList_c) ||
          (adElement.adType == gAdComplete16bitServiceList_c))
        {
            uint16_t uuid = gAppProxySelectService_d;
            foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));
        }

        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
    }
    return foundMatch;
}

static void BleApp_ConfigureAdv(void)
{
    uint8_t index = 0;

    gAppAdvertisingData.cNumAdStructures = 0;

    while ((index < mpScannedDevice->dataLength) &&
            (gAppAdvertisingData.cNumAdStructures < 10U))
    {
        gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].length = mpScannedDevice->data[index];
        gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].adType = (gapAdType_t)mpScannedDevice->data[index + 1U];
        gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].aData = &mpScannedDevice->data[index + 2U];

        /* Move on to the next AD element type */
        index += gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].length + sizeof(uint8_t);

        gAppAdvertisingData.cNumAdStructures++;
    }

    (void)Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
}

static bleResult_t BleApp_SaveService(gattService_t*    pService)
{
    uint16_t    handle;
    bleResult_t status;

    /* Add primary service */
    status = GattDbDynamic_AddPrimaryServiceDeclaration(0, pService->uuidType, &pService->uuid, &handle);

    if (status != gBleSuccess_c)
    {
        return status;
    }

    /* Add all the characteristics in the service */
    for (uint8_t i = 0; i < pService->cNumCharacteristics; i++)
    {
        status = GattDbDynamic_AddCharacteristicDeclarationAndValue(
                pService->aCharacteristics[i].value.uuidType,
                &pService->aCharacteristics[i].value.uuid,
                pService->aCharacteristics[i].properties,
                23,
                0,
                NULL,
                (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c | gPermissionFlagWritable_c),
                &handle);

        if (status != gBleSuccess_c)
        {
            return status;
        }

        /* Store handle correspondence */
        if (mcNextHandle < NumberOfElements(maGattDbHandles))
        {
            maGattDbHandles[mcNextHandle].handle = pService->aCharacteristics[i].value.handle;
            maGattDbHandles[mcNextHandle++].proxyHandle = handle + 1U;

            /* Store readable char value handle */
            if(0U != ((uint8_t)pService->aCharacteristics[i].properties & (uint8_t)gGattCharPropRead_c))
            {
                maGattDbReadNotifyHandles[mcNextReadNotifyHandle++] = handle + 1U;
            }

            /* Store writable char value handle */
            if((0U != (pService->aCharacteristics[i].properties & (uint8_t)gGattCharPropWrite_c)) ||
                ((pService->aCharacteristics[i].properties & (uint8_t)gGattCharPropWriteWithoutRsp_c) != 0U))
            {
                maGattDbWriteNotifyHandles[mcNextWriteNotifyHandle++] = handle + 1U;
            }
        }

        /* Add all the descriptors in the characteristic */
        for (uint8_t j = 0; j < pService->aCharacteristics[i].cNumDescriptors; j++)
        {
            /* We only treat CCCDs*/
            if (pService->aCharacteristics[i].aDescriptors[j].uuid.uuid16 == gBleSig_CCCD_d)
            {

                status = GattDbDynamic_AddCharDescriptor(
                    pService->aCharacteristics[i].aDescriptors[j].uuidType,
                    &pService->aCharacteristics[i].aDescriptors[j].uuid,
                    2,
                    NULL,
                    (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c | gPermissionFlagWritable_c),
                    &handle);

                if (status != gBleSuccess_c)
                {
                    return status;
                }

                /* Store handle correspondence */
                if (mcNextHandle < NumberOfElements(maGattDbHandles))
                {
                    maGattDbHandles[mcNextHandle].handle = pService->aCharacteristics[i].aDescriptors[j].handle;
                    maGattDbHandles[mcNextHandle++].proxyHandle = handle;

                    /* Store readable descriptor handle */
                    maGattDbReadNotifyHandles[mcNextReadNotifyHandle++] = handle;

                    /* Store writable descriptor handle */
                    maGattDbWriteNotifyHandles[mcNextWriteNotifyHandle++] = handle;
                }
            }
        }
    }
    return status;
}

uint16_t BleApp_GetTransalatedHandle(uint16_t proxyHandle)
{
    uint16_t handle = 0;

    for (uint8_t i = 0; i < mcNextHandle; i++)
    {
        if (maGattDbHandles[i].proxyHandle == proxyHandle)
        {
            handle = maGattDbHandles[i].handle;
        }
    }

    return handle;
}

uint16_t BleApp_GetProxyHandle(uint16_t serviceHandle)
{
    uint16_t handle = 0;

    for (uint8_t i = 0; i < mcNextHandle; i++)
    {
        if (maGattDbHandles[i].handle == serviceHandle)
        {
            handle = maGattDbHandles[i].proxyHandle;
        }
    }

    return handle;
}

void BleApp_StateMachineHandler(deviceId_t peerDeviceId, uint8_t event)
{
    switch (mPeerProxyInformation.appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
                mPeerProxyInformation.deviceId = peerDeviceId;
                mPeerProxyInformation.isBonded = FALSE;

#if gAppUseBonding_d
                Gap_CheckIfBonded(peerDeviceId, &mPeerProxyInformation.isBonded);

                if ((mPeerProxyInformation.isBonded) &&
                    (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId,
                        (void*) &mPeerProxyInformation.customInfo, 0, sizeof (appCustomInfo_t))))
                {
                    mRestoringBondedLink = TRUE;
                    /* Restored custom connection information. Encrypt link */
                    Gap_EncryptLink(peerDeviceId);
                }
#endif

                /* Moving to Exchange MTU State */
                mPeerProxyInformation.appState = mAppExchangeMtu_c;
                (void)GattClient_ExchangeMtu(peerDeviceId);
            }
        }
        break;

        case mAppExchangeMtu_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* Moving to Primary Service Discovery State*/
                mPeerProxyInformation.appState = mAppServiceDisc_c;

                /* Start Service Discovery*/
                (void)BleServDisc_Start(peerDeviceId);
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
                mPeerProxyInformation.appState = mAppRunning_c;

                BleApp_ConfigureAdv();

                /* Register handles from the mirror database for notifications */
                (void)GattServer_RegisterHandlesForReadNotifications((mcNextReadNotifyHandle), maGattDbReadNotifyHandles);
                (void)GattServer_RegisterHandlesForWriteNotifications((mcNextWriteNotifyHandle), maGattDbWriteNotifyHandles);

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

        case mAppRunning_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
                mPeerDeviceId = peerDeviceId;
            }
        }
        break;
        default:
            ; /* For MISRA compliance */
            break;
    }
}


/*! *********************************************************************************
* @}
********************************************************************************** */
