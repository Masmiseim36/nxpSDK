/*! *********************************************************************************
 * \addtogroup Bluetooth Relay Proxy
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the source file for the Bluetooth Relay Proxy
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

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
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

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

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

static deviceId_t		mPeerDeviceId;
static appPeerInfo_t	mPeerProxyInformation;

#if gAppUseBonding_d
static bool_t mRestoringBondedLink = FALSE;
#endif

/* Scan State */
static bool_t   mScanningOn = FALSE;
static gapScannedDevice_t	*mpScannedDevice = NULL;


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

static void BleApp_Config();

void BleApp_StateMachineHandler
(
    deviceId_t peerDeviceId,
    uint8_t event
);

static bool_t CheckScanEvent(gapScannedDevice_t* pData);

static void BleApp_ConfigureAdv(void);

static bleResult_t BleApp_SaveService(gattService_t*	pService);

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
    	App_StartScanning(&gScanParams, BleApp_ScanningCallback, TRUE);
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
                Gap_Disconnect(mPeerProxyInformation.deviceId);
				
            if (mPeerDeviceId != gInvalidDeviceId_c)
                Gap_Disconnect(mPeerDeviceId);
            break;
        }
        case gKBD_EventPressPB2_c:		
        case gKBD_EventLongPB2_c:
        default:
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

        case gAdvertisingDataSetupComplete_c:
        {
            App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;
        default: 
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
    /* Configure as GAP Dual Role */
    BleConnManager_GapDualRoleConfig();
        
    /* Register for callbacks*/
    App_RegisterGattServerCallback(BleApp_GattServerCallback);
    App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    App_RegisterGattClientIndicationCallback(BleApp_GattIndicationCallback);
    App_RegisterGattClientNotificationCallback(BleApp_GattNotificationCallback);

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
                mpScannedDevice = MEM_BufferAlloc(pScanningEvent->eventData.scannedDevice.dataLength + sizeof(gapScannedDevice_t));
                FLib_MemCpy(mpScannedDevice, &pScanningEvent->eventData.scannedDevice,
                		pScanningEvent->eventData.scannedDevice.dataLength + sizeof(gapScannedDevice_t));
                mpScannedDevice->data = (uint8_t *)mpScannedDevice + sizeof(gapScannedDevice_t);
                
                Gap_StopScanning();
#if gAppUsePrivacy_d
                gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif
                App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
            }
        }        
        break;
        case gScanStateChanged_c:
        {
        }
        break;
        case gScanCommandFailed_c:
        {
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
            
            BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PeerConnected_c);
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
                
                /* Disconnect peer client */
                Gap_Disconnect(mPeerDeviceId);
                
                /* UI */
                LED_TurnOffAllLeds();
                LED_StartFlash(LED_ALL); 
            }
            else if (mPeerDeviceId == peerDeviceId)
            {
                mPeerDeviceId = gInvalidDeviceId_c;
                
                /* Restart advertising on proxy */
                App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
                
                /* UI */
                Led1Flashing();
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

    	if (mCharProcBuffer.value.handle > 0)
    	{
            mCharProcBuffer.value.paValue = MEM_BufferAlloc(23);
            status = GattClient_ReadCharacteristicValue(mPeerProxyInformation.deviceId, &mCharProcBuffer, 23);

            if (status != gBleSuccess_c)
            {
            	MEM_BufferFree(mCharProcBuffer.value.paValue);

            	/* Send Read Attribute Response to the Initiator */
				GattServer_SendAttributeReadStatus(deviceId, pServerEvent->eventData.attributeReadEvent.handle, gAttErrCodeUnlikelyError_c);
            }
    	}
    }
    break;

    case gEvtAttributeWrittenWithoutResponse_c:
    case gEvtAttributeWritten_c:
    {
    	/* Received Write Request from Central on Proxy Server. Forward request to the Peripheral */
    	mCharProcBuffer.value.handle = BleApp_GetTransalatedHandle(pServerEvent->eventData.attributeReadEvent.handle);

    	if (mCharProcBuffer.value.handle > 0)
    	{
            status = GattClient_WriteCharacteristicValue(
            		mPeerProxyInformation.deviceId,
            		&mCharProcBuffer,
					pServerEvent->eventData.attributeWrittenEvent.cValueLength,
					pServerEvent->eventData.attributeWrittenEvent.aValue,
					pServerEvent->eventType & gEvtAttributeWrittenWithoutResponse_c,
					FALSE, FALSE, NULL);

            if ((status != gBleSuccess_c) &&
            		(pServerEvent->eventType & gEvtAttributeWritten_c))
            {
            	/* Send Write Attribute Response to the Initiator */
				GattServer_SendAttributeWrittenStatus(deviceId, pServerEvent->eventData.attributeReadEvent.handle, gAttErrCodeUnlikelyError_c);
            }
    	}
    }
    break;

    case gEvtCharacteristicCccdWritten_c:
    {
    	/* Received Write Request from Central on Proxy Server. Forward request to the Peripheral */
		mCharProcBuffer.value.handle = BleApp_GetTransalatedHandle(pServerEvent->eventData.attributeReadEvent.handle);

		if (mCharProcBuffer.value.handle > 0)
		{
			status = GattClient_WriteCharacteristicDescriptor(
					mPeerProxyInformation.deviceId,
					&mCharProcBuffer.value,
					pServerEvent->eventData.attributeWrittenEvent.cValueLength,
					pServerEvent->eventData.attributeWrittenEvent.aValue);

			if (status != gBleSuccess_c)
			{
				/* Send Write Attribute Response to the Initiator */
				GattServer_SendAttributeWrittenStatus(deviceId, pServerEvent->eventData.attributeReadEvent.handle, gAttErrCodeUnlikelyError_c);
			}
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
	uint16_t handle;

    if (procedureResult == gGattProcError_c)
    {    
        attErrorCode_t attError = (attErrorCode_t) (error & 0xFF);
        if (attError == gAttErrCodeInsufficientEncryption_c     ||
            attError == gAttErrCodeInsufficientAuthorization_c  ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            /* Start Pairing Procedure */  
            gPairingParameters.localIoCapabilities = gIoKeyboardDisplay_c;
            Gap_Pair(serverDeviceId, &gPairingParameters);
        }
        
        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
    }
    else if (procedureResult == gGattProcSuccess_c)
    {        
    	BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
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
			GattDb_WriteAttribute(handle,mCharProcBuffer.value.valueLength, mCharProcBuffer.value.paValue);

			/* Free buffer used for value */
			MEM_BufferFree(mCharProcBuffer.value.paValue);

			/* Send Read Attribute Response to the Initiator */
			GattServer_SendAttributeReadStatus(mPeerDeviceId, handle, error & 0xFF);
		}
		break;
	
        case gGattProcWriteCharacteristicValue_c:
        case gGattProcWriteCharacteristicDescriptor_c:
        {
            /* Get corresponding handle in the local database */
            handle = BleApp_GetProxyHandle(mCharProcBuffer.value.handle);

            /* Send Read Attribute Response to the Initiator */
            GattServer_SendAttributeWrittenStatus(mPeerDeviceId, handle, error & 0xFF);
        }
        break;

        default:
        break;
    }
}


static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent)
{
	switch(pEvent->eventType)
	{
		case gServiceDiscovered_c:
		{
			if (BleApp_SaveService(pEvent->eventData.pService) != gBleSuccess_c)
			{
				Gap_Disconnect(peerDeviceId);
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
* \brief        Handles GATT client notification callback from host stack.
*
* \param[in]    serverDeviceId      		GATT Server device ID.
* \param[in]    characteristicValueHandle   Handle.
* \param[in]    aValue    					Pointer to value.
* \param[in]    valueLength    				Value length.
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
	GattServer_SendInstantValueNotification(mPeerDeviceId, handle, valueLength, aValue);
}

/*! *********************************************************************************
* \brief        Handles GATT client indication callback from host stack.
*
* \param[in]    serverDeviceId      		GATT Server device ID.
* \param[in]    characteristicValueHandle   Handle.
* \param[in]    aValue    					Pointer to value.
* \param[in]    valueLength    				Value length.
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
	GattServer_SendInstantValueIndication(mPeerDeviceId, handle, valueLength, aValue);
}

static bool_t MatchDataInAdvElementList(gapAdStructure_t *pElement, void *pData, uint8_t iDataLen)
{ 
    uint8_t i;
    
    for (i=0; i < pElement->length; i+=iDataLen)
    {
        if (FLib_MemCmp(pData, &pElement->aData[i], iDataLen))
        {
            return TRUE;
        } 
    }    
    return FALSE;
}

static bool_t CheckScanEvent(gapScannedDevice_t* pData)
{
    uint8_t index = 0;
    bool_t foundMatch = FALSE;
    
    while (index < pData->dataLength && !foundMatch)
    {
        gapAdStructure_t adElement;
        
        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1];
        adElement.aData = &pData->data[index + 2];
      
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
			(gAppAdvertisingData.cNumAdStructures < 10))
	{
		gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].length = mpScannedDevice->data[index];
		gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].adType = (gapAdType_t)mpScannedDevice->data[index + 1];
		gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].aData = &mpScannedDevice->data[index + 2];

		/* Move on to the next AD element type */
		index += gAppAdvertisingData.aAdStructures[gAppAdvertisingData.cNumAdStructures].length + sizeof(uint8_t);

		gAppAdvertisingData.cNumAdStructures++;
	}

    Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
}

static bleResult_t BleApp_SaveService(gattService_t*	pService)
{
	uint16_t	handle;
	bleResult_t status;

	/* Add primary service */
	status = GattDbDynamic_AddPrimaryServiceDeclaration(pService->uuidType, &pService->uuid, &handle);

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
			maGattDbHandles[mcNextHandle++].proxyHandle = handle + 1;

			/* Store readable char value handle */
			if(pService->aCharacteristics[i].properties & gGattCharPropRead_c)
			{
				maGattDbReadNotifyHandles[mcNextReadNotifyHandle++] = handle + 1;
			}

			/* Store writable char value handle */
			if((pService->aCharacteristics[i].properties & gGattCharPropWrite_c) ||
				(pService->aCharacteristics[i].properties & gGattCharPropWriteWithoutRsp_c))
			{
				maGattDbWriteNotifyHandles[mcNextWriteNotifyHandle++] = handle + 1;
			}
		}

		/* Add all the descriptors in the characteristic */
		for (uint8_t j = 0; j < pService->aCharacteristics[i].cNumDescriptors; j++)
		{
			/* We only treat CCCDs*/
			if (pService->aCharacteristics[i].aDescriptors[j].uuid.uuid16 == gBleSig_CCCD_d)
			{

				status = GattDbDynamic_AddCharacteristicDescriptor(
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
                GattClient_ExchangeMtu(peerDeviceId);
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
            	/* Moving to Primary Service Discovery State*/
				mPeerProxyInformation.appState = mAppRunning_c;

				BleApp_ConfigureAdv();

				/* Register handles from the mirror database for notifications */
				GattServer_RegisterHandlesForReadNotifications((mcNextReadNotifyHandle), maGattDbReadNotifyHandles);
				GattServer_RegisterHandlesForWriteNotifications((mcNextWriteNotifyHandle), maGattDbWriteNotifyHandles);

            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                Gap_Disconnect(peerDeviceId);
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
    }
}


/*! *********************************************************************************
* @}
********************************************************************************** */
