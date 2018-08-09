/*! *********************************************************************************
* \addtogroup HID Host
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the source file for the HID Host application
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
#include "shell.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "hid_interface.h"

#include "ble_conn_manager.h"
#include "ble_service_discovery.h"

#include "ApplMain.h"
#include "hid_host.h"

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
    mAppEnableNotifications_c,
    mAppRunning_c,
}appState_t;

typedef struct appCustomInfo_tag
{
    hidcConfig_t     hidClientConfig;
    /* Add persistent information here */
}appCustomInfo_t;

typedef struct appPeerInfo_tag
{
    deviceId_t      deviceId;
    appCustomInfo_t customInfo;
    bool_t          isBonded;
    appState_t      appState;
}appPeerInfo_t;

typedef struct mouseHidReport_tag{
  uint8_t buttonStatus;
  uint8_t xAxis;
  uint8_t yAxis;
}mouseHidReport_t;
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static appPeerInfo_t mPeerInformation[gAppMaxConnections_c];

#if gAppUseBonding_d
static bool_t mRestoringBondedLink = FALSE;
#endif

static bool_t   mScanningOn = FALSE;

/* Buffer used for Characteristic related procedures */
static gattAttribute_t      *mpCharProcBuffer = NULL;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Host Stack callbacks */
static void BleApp_ScanningCallback 
(
    gapScanningEvent_t* pScanningEvent
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

static void BleApp_GattNotificationCallback
(
    deviceId_t          serverDeviceId,
    uint16_t characteristicValueHandle,
    uint8_t* aValue,
    uint16_t valueLength
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

static void BleApp_StoreServiceHandles
(
	deviceId_t 		peerDeviceId,
    gattService_t   *pService
);

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
    /* UI */
    shell_init("BLE HID Host>");
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    if (!mScanningOn)
    {
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
        	for (uint8_t i = 0; i < gAppMaxConnections_c; i++)
        	{
                if (mPeerInformation[i].deviceId != gInvalidDeviceId_c)
                    Gap_Disconnect(mPeerInformation[i].deviceId);
        	}
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
static void BleApp_Config()
{
    /* Configure as GAP Central */
    BleConnManager_GapCentralConfig();

    /* Register for callbacks*/
    App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    App_RegisterGattClientNotificationCallback(BleApp_GattNotificationCallback);
    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);
	
    for (uint8_t i = 0; i < gAppMaxConnections_c; i++)
    {
        mPeerInformation[i].appState = mAppIdle_c;
    }
    
    mScanningOn = FALSE;
    /* UI */
    shell_write("\r\nPress SW4 to connect to a HID Device!\r\n");
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
            mScanningOn = !mScanningOn;
            if (mScanningOn)
            {
                LED_StopFlashingAllLeds();
                Led1Flashing();
                shell_write("\r\nScanning...\r\n");              
            }
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
            shell_write("\r\nConnected!\r\n");
                        
            mPeerInformation[peerDeviceId].deviceId = peerDeviceId;
            mPeerInformation[peerDeviceId].isBonded = FALSE;
			
#if gAppUseBonding_d            
            Gap_CheckIfBonded(peerDeviceId, &mPeerInformation[peerDeviceId].isBonded);
            
            if ((mPeerInformation[peerDeviceId].isBonded) &&
                (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId,
                    (void*) &mPeerInformation[peerDeviceId].customInfo, 0, sizeof (appCustomInfo_t))))
            {
                mRestoringBondedLink = TRUE;
                /* Restored custom connection information. Encrypt link */
                Gap_EncryptLink(peerDeviceId);
            }
#endif
            BleApp_StateMachineHandler(mPeerInformation[peerDeviceId].deviceId, mAppEvt_PeerConnected_c);
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            mPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;
            
            /* Reset Service Discovery to be sure*/
            BleServDisc_Stop(peerDeviceId);
            
            /* UI */
            LED_TurnOffAllLeds();
            LED_StartFlash(LED_ALL);
            shell_write("\r\nDisconnected!\r\n");
        }
        break;

#if gAppUsePairing_d		
        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(mPeerInformation[peerDeviceId].deviceId, mAppEvt_PairingComplete_c);
            }
        }
        break;
        case gConnEvtEncryptionChanged_c:
        {
            if (mRestoringBondedLink)
            {
                mRestoringBondedLink = FALSE;
                uint16_t value = gCccdNotification_c;
                GattClient_WriteCharacteristicDescriptor(mPeerInformation[peerDeviceId].deviceId,
                                                         mpCharProcBuffer, 
                                                         sizeof(value), (void*)&value);
                }
        }
        break;
#endif /* gAppUsePairing_d */

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
        attErrorCode_t attError = (attErrorCode_t) (error & 0xFF);
        if (attError == gAttErrCodeInsufficientEncryption_c     ||
            attError == gAttErrCodeInsufficientAuthorization_c  ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            /* Start Pairing Procedure */    
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
    if (characteristicValueHandle == mPeerInformation[serverDeviceId].customInfo.hidClientConfig.hHidReport)
    {
        mouseHidReport_t *pReport = (mouseHidReport_t*)aValue;
        shell_write("\r\nReceived HID Report from device ");
        shell_writeDec(serverDeviceId);
        shell_write(": X: ");
        shell_writeHex(&pReport->xAxis, sizeof(uint8_t));
        shell_write(" Y: ");
        shell_writeHex(&pReport->yAxis, sizeof(uint8_t));
    }
}

/*! *********************************************************************************
* \brief        Stores handles for the specified service.
*
* \param[in]    pService    Pointer to gattService_t.
********************************************************************************** */
static void BleApp_StoreServiceHandles
(
	deviceId_t 		peerDeviceId,
    gattService_t   *pService
)
{
    uint8_t i, j;

    if ((pService->uuidType == gBleUuidType16_c) &&
        (pService->uuid.uuid16 == gBleSig_HidService_d))
    {
        /* Found HID Service */
        mPeerInformation[peerDeviceId].customInfo.hidClientConfig.hService = pService->startHandle;

        for (i = 0; i < pService->cNumCharacteristics; i++)
        {
            if ((pService->aCharacteristics[i].value.uuidType == gBleUuidType16_c) &&
                (pService->aCharacteristics[i].value.uuid.uuid16 == gBleSig_Report_d))
            {
                /* Found HID Report Char */
                mPeerInformation[peerDeviceId].customInfo.hidClientConfig.hHidReport = pService->aCharacteristics[i].value.handle;

                for (j = 0; j < pService->aCharacteristics[i].cNumDescriptors; j++)
				{
					if (( pService->aCharacteristics[i].aDescriptors[j].uuidType == gBleUuidType16_c) &&
					( pService->aCharacteristics[i].aDescriptors[j].uuid.uuid16 == gBleSig_CCCD_d))
					{
						mPeerInformation[peerDeviceId].customInfo.hidClientConfig.hHidReportCccd =  pService->aCharacteristics[i].aDescriptors[j].handle;
					}
				}
            }
        }
    }
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
    uint8_t name[10];
    uint8_t nameLength;
    bool_t foundMatch = FALSE;
    
    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;
        
        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1];
        adElement.aData = &pData->data[index + 2];
      
         /* Search for Temperature Custom Service */
        if ((adElement.adType == gAdIncomplete16bitServiceList_c) ||
          (adElement.adType == gAdComplete16bitServiceList_c))
        {
            uint16_t uuid = gBleSig_HidService_d;
            foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));
        }
        
        if ((adElement.adType == gAdShortenedLocalName_c) ||
          (adElement.adType == gAdCompleteLocalName_c))
        {
            nameLength = MIN(adElement.length, 10);
            FLib_MemCpy(name, adElement.aData, nameLength);
        }
        
        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
    }
    
    if (foundMatch)
    {
        /* UI */
        shell_write("\r\nFound device: \r\n");
        shell_writeN((char*)name, nameLength-1);
        SHELL_NEWLINE();
        shell_writeHex(pData->aAddress, 6);
    }
    return foundMatch;
}

void BleApp_StateMachineHandler(deviceId_t peerDeviceId, uint8_t event)
{
    switch (mPeerInformation[peerDeviceId].appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
                /* Moving to Exchange MTU State */
                mPeerInformation[peerDeviceId].appState = mAppExchangeMtu_c;
                GattClient_ExchangeMtu(peerDeviceId);
            }
        }
        break;

        case mAppExchangeMtu_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* Moving to Primary Service Discovery State*/
                mPeerInformation[peerDeviceId].appState = mAppServiceDisc_c;

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
				mPeerInformation[peerDeviceId].appState = mAppEnableNotifications_c;
                
                if (mPeerInformation[peerDeviceId].customInfo.hidClientConfig.hHidReportCccd)
                {
                    mpCharProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + 23);
                                                    
                    if (!mpCharProcBuffer)
                      return;
                    
                    mpCharProcBuffer->handle = mPeerInformation[peerDeviceId].customInfo.hidClientConfig.hHidReportCccd;
                    mpCharProcBuffer->paValue = (uint8_t*)(mpCharProcBuffer + 1);
                    GattClient_ReadCharacteristicDescriptor(mPeerInformation[peerDeviceId].deviceId, mpCharProcBuffer ,23);
                }
            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                Gap_Disconnect(peerDeviceId);
            }
        }
        break;

        case mAppEnableNotifications_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mpCharProcBuffer && (!(mpCharProcBuffer->paValue[0] & gCccdNotification_c)) &&
                    (mpCharProcBuffer->handle == mPeerInformation[peerDeviceId].customInfo.hidClientConfig.hHidReportCccd))
                {              
                    uint16_t value = gCccdNotification_c;

                	/* Moving to Running State*/
					mPeerInformation[peerDeviceId].appState = mAppRunning_c;

                    GattClient_WriteCharacteristicDescriptor(peerDeviceId,
                                                             mpCharProcBuffer,
                                                             sizeof(value), (void*)&value);
                }
            }
            else if (event == mAppEvt_PairingComplete_c)
            {
                /* Continue after pairing is complete */
                GattClient_ReadCharacteristicDescriptor(peerDeviceId, mpCharProcBuffer ,23);
            }
        }
        break;

        case mAppRunning_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mpCharProcBuffer)
				{
                	MEM_BufferFree(mpCharProcBuffer);
                	mpCharProcBuffer = NULL;
				}

                /* Write data in NVM */
                Gap_SaveCustomPeerInformation(peerDeviceId,
                                              (void*) &mPeerInformation[peerDeviceId].customInfo, 0,
                                              sizeof (appCustomInfo_t));
            }

        }
        break;
    }
}


/*! *********************************************************************************
* @}
********************************************************************************** */
