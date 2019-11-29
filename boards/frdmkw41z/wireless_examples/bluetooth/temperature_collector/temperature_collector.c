/*! *********************************************************************************
* \addtogroup Temperature Collector
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the source file for the Temperature Collector application
*
* SPDX-License-Identifier: BSD-3-Clause
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
#include "PWR_Interface.h"

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
#include "temperature_collector.h"

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

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static appPeerInfo_t mPeerInformation;

#if gAppUseBonding_d
static bool_t mRestoringBondedLink = FALSE;
#endif

static bool_t   mScanningOn = FALSE;
static bool_t   mFoundDeviceToConnect = FALSE;

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
    gattService_t   *pService
);

static void BleApp_StoreDescValues
(
    gattAttribute_t     *pDesc
);

static void BleApp_PrintTemperature
(
    uint16_t temperature
);

static void ScanningTimeoutTimerCallback(void* pParam);
#if (cPWR_UsePowerDownMode) 
static void DisconnectTimerCallback(void* pParam);
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
    /* UI */
    shell_init("BLE Temp Collector>");
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
#if cPWR_UsePowerDownMode  
    BleApp_Start();
#else      
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
                Gap_Disconnect(mPeerInformation.deviceId);
            break;
        }
        case gKBD_EventPressPB2_c:		
        case gKBD_EventLongPB2_c:
        default:
            break;
    }
#endif    
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
	
    /* Initialize private variables */
    mPeerInformation.appState = mAppIdle_c;
    mScanningOn = FALSE;
    mFoundDeviceToConnect = FALSE;
    
    /* Allocate scann timeout timer */
    mAppTimerId = TMR_AllocateTimer();
	
    /* UI */
    shell_write("\r\nPress SW4 to connect to a Temperature Sensor!\r\n");
    
#if (cPWR_UsePowerDownMode)  
    /* Allow entering sleep mode until any user interaction */
    PWR_AllowDeviceToSleep();  
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
        {
            mFoundDeviceToConnect = CheckScanEvent(&pScanningEvent->eventData.scannedDevice);
            if (mFoundDeviceToConnect)
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
            
            /* Node starts scanning */
            if (mScanningOn)
            {  		
                mFoundDeviceToConnect = FALSE;
                
                shell_write("\r\nScanning...\r\n");
                
                /* Start advertising timer */
                TMR_StartLowPowerTimer(mAppTimerId, 
                           gTmrLowPowerSecondTimer_c,
                           TmrSeconds(gScanningTime_c),
                           ScanningTimeoutTimerCallback, NULL);              
#if (cPWR_UsePowerDownMode)
                PWR_ChangeDeepSleepMode(1);
                PWR_AllowDeviceToSleep();                
                Led1On();
#else
                LED_StopFlashingAllLeds();
                Led1Flashing();
#endif              
            }
            /* Node is not scanning */
            else
            {                
                TMR_StopTimer(mAppTimerId);
                
                if (mFoundDeviceToConnect)
                {
                    App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                }
                else
                {
#if (cPWR_UsePowerDownMode)
                    Led1Off();
                    /* Go to sleep */
                    PWR_ChangeDeepSleepMode(3);
                    PWR_SetDeepSleepTimeInMs(cPWR_DeepSleepDurationMs);
                    PWR_AllowDeviceToSleep();
#else
                    LED_StopFlashingAllLeds();
                    Led1Flashing();
                    Led2Flashing();
                    Led3Flashing();
                    Led4Flashing();
#endif             
                }
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
                        
            mPeerInformation.deviceId = peerDeviceId;
            mPeerInformation.isBonded = FALSE; 
            
#if (cPWR_UsePowerDownMode)             
            PWR_ChangeDeepSleepMode(1);
            PWR_AllowDeviceToSleep();                
#endif                 
			
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
#endif
            BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PeerConnected_c);
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            mPeerInformation.deviceId = gInvalidDeviceId_c;
            
            /* Reset Service Discovery to be sure*/
            BleServDisc_Stop(peerDeviceId);
            
            /* UI */
            shell_write("\r\nDisconnected!\r\n");
            
#if (cPWR_UsePowerDownMode)
            /* Go to sleep */
            PWR_ChangeDeepSleepMode(3);
            PWR_SetDeepSleepTimeInMs(cPWR_DeepSleepDurationMs);
            PWR_AllowDeviceToSleep();
            Led1Off();
#else
            LED_TurnOffAllLeds();
            LED_StartFlash(LED_ALL);
#endif            
        }
        break;
		

#if gAppUsePairing_d		
        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PairingComplete_c);
            }
            else
            {
                /* UI */
                shell_write("\r\nPairing Failed!\r\n");
            }
        }
        break;

        case gConnEvtEncryptionChanged_c:
        {
            if (mRestoringBondedLink)
            {
                mRestoringBondedLink = FALSE;
                uint16_t value = gCccdNotification_c;
                GattClient_WriteCharacteristicDescriptor(mPeerInformation.deviceId,
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
			BleApp_StoreServiceHandles(pEvent->eventData.pService);
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
* \brief        Stores handles for the specified service.
*
* \param[in]    pService    Pointer to gattService_t.
********************************************************************************** */
static void BleApp_StoreServiceHandles
(
    gattService_t   *pService
)
{
    uint8_t i,j;
      
    if ((pService->uuidType == gBleUuidType128_c) &&
        FLib_MemCmp(pService->uuid.uuid128, uuid_service_temperature, 16))
    {
        /* Found Temperature Service */
        mPeerInformation.customInfo.tempClientConfig.hService = pService->startHandle;
        
        for (i = 0; i < pService->cNumCharacteristics; i++)
        {
            if ((pService->aCharacteristics[i].value.uuidType == gBleUuidType16_c) &&
                (pService->aCharacteristics[i].value.uuid.uuid16 == gBleSig_Temperature_d))
            {
                /* Found Temperature Char */
                mPeerInformation.customInfo.tempClientConfig.hTemperature = pService->aCharacteristics[i].value.handle;
                
                for (j = 0; j < pService->aCharacteristics[i].cNumDescriptors; j++)
                {
                    if (pService->aCharacteristics[i].aDescriptors[j].uuidType == gBleUuidType16_c)
                    {
                        switch (pService->aCharacteristics[i].aDescriptors[j].uuid.uuid16)
                        {
                            case gBleSig_CharPresFormatDescriptor_d:
                            {
                                mPeerInformation.customInfo.tempClientConfig.hTempDesc = pService->aCharacteristics[i].aDescriptors[j].handle;
                                break;
                            }
                            case gBleSig_CCCD_d:
                            {
                                mPeerInformation.customInfo.tempClientConfig.hTempCccd = pService->aCharacteristics[i].aDescriptors[j].handle;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
}


static void BleApp_StoreDescValues
(
    gattAttribute_t     *pDesc
)
{
    if (pDesc->handle == mPeerInformation.customInfo.tempClientConfig.hTempDesc)
    {
        /* Store temperature format*/
        FLib_MemCpy(&mPeerInformation.customInfo.tempClientConfig.tempFormat,
                    pDesc->paValue,
                    pDesc->valueLength);
    }
  
}

static void BleApp_PrintTemperature
(
    uint16_t temperature
)
{
    shell_write("Temperature: ");
    shell_writeDec(temperature/100);
    if (mPeerInformation.customInfo.tempClientConfig.tempFormat.unitUuid16 == 0x272F)
    {
        shell_write(" C\r\n");
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
        }
        
        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
    }
    else if (procedureResult == gGattProcSuccess_c)
    {        
        switch(procedureType)
        {
            case gGattProcReadCharacteristicDescriptor_c:
            {
                if (mpCharProcBuffer != NULL)
                {
                    BleApp_StoreDescValues(mpCharProcBuffer);
                }
                break;
            }
            
            default:
                break;
        }  

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
    if (characteristicValueHandle == mPeerInformation.customInfo.tempClientConfig.hTemperature)
    {
        BleApp_PrintTemperature(*(uint16_t*)aValue);
        
#if (cPWR_UsePowerDownMode)               
        /* Restart Wait For Data timer */
        TMR_StartLowPowerTimer(mAppTimerId, 
                       gTmrLowPowerSecondTimer_c,
                       TmrSeconds(gWaitForDataTime_c),
                       DisconnectTimerCallback, NULL);  
#endif 
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
        if ((adElement.adType == gAdIncomplete128bitServiceList_c) ||
          (adElement.adType == gAdComplete128bitServiceList_c))
        {
            foundMatch = MatchDataInAdvElementList(&adElement, &uuid_service_temperature, 16);
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
    switch (mPeerInformation.appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
#if gAppUseBonding_d
                if (mPeerInformation.customInfo.tempClientConfig.hTemperature != gGattDbInvalidHandle_d)
                {
                    /* Moving to Running State */
                    mPeerInformation.appState = mAppRunning_c;         

#if (cPWR_UsePowerDownMode)                    
                    /* Start Wait For Data timer */
                    TMR_StartLowPowerTimer(mAppTimerId, 
                               gTmrLowPowerSecondTimer_c,
                               gConnReqParams.connIntervalMax * 10,
                               DisconnectTimerCallback, NULL);                      
#endif                    
                }
                else
#endif              
                {
                    /* Moving to Exchange MTU State */
                    mPeerInformation.appState = mAppExchangeMtu_c;          
                    GattClient_ExchangeMtu(peerDeviceId);
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
                mPeerInformation.appState = mAppReadDescriptor_c;
                
                if (mPeerInformation.customInfo.tempClientConfig.hTempDesc)
                {
                    mpCharProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + 23);
                                                    
                    if (!mpCharProcBuffer)
                      return;
                    
                    mpCharProcBuffer->handle = mPeerInformation.customInfo.tempClientConfig.hTempDesc;
                    mpCharProcBuffer->paValue = (uint8_t*)(mpCharProcBuffer + 1);
                    GattClient_ReadCharacteristicDescriptor(mPeerInformation.deviceId, mpCharProcBuffer ,23);
                }
            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                Gap_Disconnect(peerDeviceId);
            }
        }
        break;

        case mAppReadDescriptor_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mPeerInformation.customInfo.tempClientConfig.hTempCccd)
                {              
                    uint16_t value = gCccdNotification_c;

                    /* Moving to Running State*/
                    mPeerInformation.appState = mAppRunning_c;

                    mpCharProcBuffer->handle = mPeerInformation.customInfo.tempClientConfig.hTempCccd;
                    mpCharProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
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
							
#if gAppUseBonding_d                   
                /* Write data in NVM */
                Gap_SaveCustomPeerInformation(mPeerInformation.deviceId, 
                                              (void*) &mPeerInformation.customInfo, 0, 
                                              sizeof (appCustomInfo_t));
#endif                
#if (cPWR_UsePowerDownMode)               
                /* Start Wait For Data timer */
                TMR_StartLowPowerTimer(mAppTimerId, 
                               gTmrLowPowerSecondTimer_c,
                               gConnReqParams.connIntervalMax * 20,
                               DisconnectTimerCallback, NULL);  
#endif                
            }
        }
        break;
    }
}

static void ScanningTimeoutTimerCallback(void* pParam)
{
    /* Stop scanning */
    if (mScanningOn)
    {
        Gap_StopScanning();
    }
}
#if (cPWR_UsePowerDownMode) 
static void DisconnectTimerCallback(void* pParam)
{
    if (mPeerInformation.deviceId != gInvalidDeviceId_c)
    {
        Gap_Disconnect(mPeerInformation.deviceId);
    }
}
#endif
/*! *********************************************************************************
* @}
********************************************************************************** */
