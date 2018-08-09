/*! *********************************************************************************
* \addtogroup Alert Notification Server
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
* This file is the source file for the Alert Notification Server application
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

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "current_time_interface.h"
#include "reference_time_update_interface.h"
#include "next_dst_change_interface.h"
#include "alert_notification_interface.h"

#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "alert_notification_server.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mBatteryLevelReportInterval_c   (10)        /* battery level report interval in seconds  */

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct appPeerInfo_tag
{
    deviceId_t      deviceId;
    bool_t          isBonded;
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

/* Service Data*/
static basConfig_t      basServiceConfig = {service_battery, 0};
static ctsConfig_t      ctsServiceConfig = {service_current_time, gCts_InitTime, gCts_InitTime, 0U, gCts_InitLocalTimeInfo, gCts_InitReferenceTimeInfo, FALSE};
static rtusConfig_t     rtusServiceConfig = {service_reference_time, {gRtusIdle_c, gRtusSuccessful_c}};
static ndcsConfig_t     ndcsServiceConfig = {service_next_dst, {{2016,1,1,0,0,0},0U}};
static ansConfig_t      ansServiceConfig = {service_alert};
static uint16_t         cpHandles[] = { value_alert_cp, value_current_time, value_time_update_cp };

/* Application specific data*/
static tmrTimerID_t mBatteryMeasurementTimerId;
static tmrTimerID_t mAppTimerId;
static tmrTimerID_t mCTSTickTimerId;
static tmrTimerID_t mRTUSReferenceUpdateTimerId;

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

static void ScanningTimeoutTimerCallback(void* pParam);

static void BleApp_Config();
static void BleApp_SendUnreadAlertStatus(void);
static void BleApp_SendNewAlert(void);

static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
static bool_t CheckScanEvent(gapScannedDevice_t* pData);
static void BatteryMeasurementTimerCallback (void *);
static void CTSTickTimerCallback (void *);
static void RTUSReferenceUpdateTimerCallback (void *);

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
            if (mPeerInformation.deviceId != gInvalidDeviceId_c)
                Gap_Disconnect(mPeerInformation.deviceId);
            break;
        }
        case gKBD_EventPressPB2_c:		
        {
            ctsServiceConfig.localTime += 3600;
            ctsServiceConfig.adjustReason = gCts_ManualUpdate;
            Cts_RecordCurrentTime(&ctsServiceConfig);
            BleApp_SendNewAlert();
            break;
        }
        case gKBD_EventLongPB2_c:
        {
            BleApp_SendUnreadAlertStatus();          
            break;
        }
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
    App_RegisterGattServerCallback(BleApp_GattServerCallback);
    GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpHandles), cpHandles);    

    /* Initialize private variables */
    mScanningOn = FALSE;
    
    /* Allocate scann timeout timer */
    mAppTimerId = TMR_AllocateTimer();

    /* Start services */
    Ans_Start(&ansServiceConfig);
    
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_Start(&basServiceConfig);
    
    Cts_Start(&ctsServiceConfig);
    Ndcs_Start(&ndcsServiceConfig);
    Rtus_Start(&rtusServiceConfig);
    
    /* Allocate application timers */
    mBatteryMeasurementTimerId = TMR_AllocateTimer();
    mCTSTickTimerId = TMR_AllocateTimer();
    mRTUSReferenceUpdateTimerId = TMR_AllocateTimer();
    
    /* Start local time tick timer */
    TMR_StartLowPowerTimer(mCTSTickTimerId, gTmrLowPowerIntervalMillisTimer_c,
               TmrSeconds(1), CTSTickTimerCallback, NULL);
    
    /* Start reference update timer */
    TMR_StartLowPowerTimer(mRTUSReferenceUpdateTimerId, gTmrLowPowerIntervalMillisTimer_c,
               TmrSeconds(60), RTUSReferenceUpdateTimerCallback, NULL);
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
            
            /* Node starts scanning */
            if (mScanningOn)
            { 
                /* Start scan timeout timer */
                TMR_StartLowPowerTimer(mAppTimerId, 
                           gTmrLowPowerSecondTimer_c,
                           TmrSeconds(gScanningTime_c),
                           ScanningTimeoutTimerCallback, NULL);              
                LED_StopFlashingAllLeds();
                Led1Flashing();
            }
            /* Node is not scanning */
            else
            {                
                TMR_StopTimer(mAppTimerId);

                Led1Flashing();
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
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
                        
            mPeerInformation.deviceId = peerDeviceId;
            mPeerInformation.isBonded = FALSE; 
#if gAppUseBonding_d            
            Gap_CheckIfBonded(peerDeviceId, &mPeerInformation.isBonded);
            
            if (mPeerInformation.isBonded)
            {              
                mRestoringBondedLink = TRUE;
                /* Restored custom connection information. Encrypt link */
                Gap_EncryptLink(peerDeviceId);
                
            }
#endif

            /* Subscribe client*/
            Bas_Subscribe(peerDeviceId);        
            Ans_Subscribe(peerDeviceId);
            Cts_Subscribe(peerDeviceId);
            Ndcs_Subscribe(peerDeviceId);
            Rtus_Subscribe(peerDeviceId);
            
            /* Start battery measurements */
            TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);     
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            mPeerInformation.deviceId = gInvalidDeviceId_c;
            
            /* Stop battery measurements */
            TMR_StopTimer(mBatteryMeasurementTimerId);
            
            /* UI */
            LED_TurnOffAllLeds();
            LED_StartFlash(LED_ALL);
            
            /* Unsubscribe client*/
            Bas_Unsubscribe(peerDeviceId);        
            Ans_Unsubscribe(peerDeviceId);
            Cts_Unsubscribe(peerDeviceId);
        }
        break;
        
        case gConnEvtPairingRequest_c:
        {
#if gAppUsePairing_d           
            gPairingParameters.peripheralKeys = pConnectionEvent->eventData.pairingEvent.peripheralKeys;
            Gap_AcceptPairingRequest(peerDeviceId, &gPairingParameters);
#else
            Gap_RejectPairing(peerDeviceId, gPairingNotSupported_c);
#endif
        }
        break;
		
#if gAppUsePairing_d		
        case gConnEvtPasskeyRequest_c:
            Gap_EnterPasskey(peerDeviceId, gPasskeyValue_c);
            break;
            
        case gConnEvtPasskeyDisplay_c:
            /* Display on a screen or simply ignore */
            break;
        
        case gConnEvtPairingResponse_c:
        {
            if (pConnectionEvent->eventData.pairingEvent.localIoCapabilities == gIoKeyboardDisplay_c ||
                pConnectionEvent->eventData.pairingEvent.localIoCapabilities == gIoKeyboardOnly_c)
            {
                Gap_SetLocalPasskey(gPasskeyValue_c);
            }
            break;
        }
        
        case gConnEvtKeyExchangeRequest_c:
        {
            gapSmpKeys_t sentSmpKeys = gSmpKeys;
            
            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gLtk_c))
            {
                sentSmpKeys.aLtk = NULL;
                /* When the LTK is NULL EDIV and Rand are not sent and will be ignored. */
            }
            
            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gIrk_c))
            {
                sentSmpKeys.aIrk = NULL;
                /* When the IRK is NULL the Address and Address Type are not sent and will be ignored. */
            }
            
            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gCsrk_c))
            {
                sentSmpKeys.aCsrk = NULL;
            }
            
            Gap_SendSmpKeys(peerDeviceId, &sentSmpKeys);
            break;
        }
            
        case gConnEvtLongTermKeyRequest_c:
            if (pConnectionEvent->eventData.longTermKeyRequestEvent.ediv == gSmpKeys.ediv &&
                pConnectionEvent->eventData.longTermKeyRequestEvent.randSize == gSmpKeys.cRandSize)
            {
                Gap_LoadEncryptionInformation(peerDeviceId, gSmpKeys.aLtk, &gSmpKeys.cLtkSize);
                /* EDIV and RAND both matched */
                Gap_ProvideLongTermKey(peerDeviceId, gSmpKeys.aLtk, gSmpKeys.cLtkSize);
            }
            else
            /* EDIV or RAND size did not match */
            {
                Gap_DenyLongTermKey(peerDeviceId);
            }
            break;
        case gConnEvtSlaveSecurityRequest_c:
        {
            Gap_Pair(peerDeviceId, &gPairingParameters);
            break;
        }
        break;
#endif /* gAppUsePairing_d */

    default:
        break;
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
    uint16_t handle;
    uint8_t status;
    
    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWritten_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            status = gAttErrCodeNoError_c;
            
            if (handle == value_alert_cp)
            {
                Ans_ControlPointHandler(&ansServiceConfig, &pServerEvent->eventData.attributeWrittenEvent);
            }
            else if (handle == value_current_time)
            {
                Cts_CurrentTimeWrittenHandler(&ctsServiceConfig, &pServerEvent->eventData.attributeWrittenEvent);
            }
            else
            {
                GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
            }
            break;
        }
        
        case gEvtAttributeWrittenWithoutResponse_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            status = gAttErrCodeNoError_c;
            
            if (handle == value_time_update_cp)
            {
                Rtus_ControlPointHandler(&rtusServiceConfig, &pServerEvent->eventData.attributeWrittenEvent);
            }
            break;
        }
        
        default:
            break;
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
          (adElement.adType == gAdComplete16bitServiceList_c) ||
          (adElement.adType == gAdServiceSolicitationList16bit_c))
        {
            uint16_t uuid = gBleSig_AlertNotificationService_d;
            foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));

            /* Connect to a time client for Time Service Demo */
            if (foundMatch != TRUE)
            {
                uint16_t uuid = gBleSig_CurrentTimeService_d;
                foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));
            }
            if (foundMatch != TRUE)
            {
                uint16_t uuid = gBleSig_ReferenceTimeUpdateService_d;
                foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));
            }
            if (foundMatch != TRUE)
            {
                uint16_t uuid = gBleSig_NextDSTChangeService_d;
                foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));
            }
            /* Connect to Blood Pressure Server to update it's time (for Demo purpose)*/
            if (foundMatch != TRUE)
            {
                uint16_t uuid = gBleSig_BloodPressureService_d;
                foundMatch = MatchDataInAdvElementList(&adElement, &uuid, sizeof(uint16_t));
            }
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

    return foundMatch;
}

static void ScanningTimeoutTimerCallback(void* pParam)
{
    /* Stop scanning */
    if (mScanningOn)
    {
        Gap_StopScanning();
    }
}

static void BleApp_SendNewAlert(void)
{
    ansNewAlert_t alert = {gAns_Email_c, 1,  "New mail"};
    alert.textLength = 8;

    Ans_SendNewAlert(ansServiceConfig.serviceHandle, &alert);
}

static void BleApp_SendUnreadAlertStatus(void)
{
    ansUnreadAlertStatus_t alert = {gAns_MissedCall_c, 3};
    Ans_SendUnreadAlertStatus(ansServiceConfig.serviceHandle, &alert);
}

/*! *********************************************************************************
* \brief        Handles battery measurement timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void BatteryMeasurementTimerCallback(void * pParam)
{
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_RecordBatteryMeasurement(basServiceConfig.serviceHandle, basServiceConfig.batteryLevel);
}

/*! *********************************************************************************
* \brief        Handles current time tick callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void CTSTickTimerCallback(void * pParam)
{
    ctsServiceConfig.localTime++;
    ctsServiceConfig.adjustReason = gCts_UnknownReason;
    Cts_RecordCurrentTime(&ctsServiceConfig);
}

/*! *********************************************************************************
* \brief        Handles time update callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void RTUSReferenceUpdateTimerCallback(void * pParam)
{
    if(rtusServiceConfig.timeUpdateState.currentState == gRtusUpdatePending_c)
    {
        rtusResult_t result = gRtusSuccessful_c;
        
        /* We simulate an update just for demo purposes */
        rtusServiceConfig.timeUpdateState.currentState = gRtusIdle_c;
        rtusServiceConfig.timeUpdateState.result = result;
        Rtus_RecordTimeUpdateState(&rtusServiceConfig);
        ctsServiceConfig.adjustReason = gCts_ExternalRefUpdate;
        Cts_RecordCurrentTime(&ctsServiceConfig);
    }
}

/*! *********************************************************************************
* @}
********************************************************************************** */
