/*! *********************************************************************************
* \addtogroup Wireless Power PTU
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
#include "wpt_interface.h"

#include "ble_conn_manager.h"
#include "ble_service_discovery.h"
#include "ble_utils.h"

#include "ApplMain.h"
#include "wireless_power_ptu.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gPtuStaticPower16Watts_c  (70)
#define gPtuPower16Watts_mW_c     (16000)
#define gPtuStaticPower15Watts_c  (68)
#define gPtuPower15Watss_mW_c     (15000)
#define gTRegistration_c          (500)
#define gTMonitorInterval_c       (100)
#define gPruTemperatureOffset_c   (40)
#define gIdleTimeTimeout_c        (1000)

#ifndef gPtuDefaultPower_mW_c
#define gPtuDefaultPower_mW_c     (gPtuPower16Watts_mW_c)
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef enum appEvent_tag{
    gPtuEventDevRegFoundDev_c = 0,
    gPtuEventDevRegConnected_c,
    gPtuEventDevRegGattProcComp_c,
    gPtuEventDevRegGattProcFailed_c,
    gPtuEventDevRegTimeout_c,
    gPtuEventLoadVariation_c,
    gPtuEventPeerDisconnected_c,
    gPtuEventChargeAllowedForDevice_c,
    gPtuEventChargeDeniedForDevice_c,
    gPtuEventDeviceCharged_c,
    gPtuEventLocalFault_c,
    gPtuEventLocalFaultCleared_c,
    gPtuEventSystemError_c,
    gPtuEventGattProcComplete_c,
    gPtuEventGattProcError_c
}appEvent_t;

typedef enum wptPtuStates_tag{
    gWptPtuStateConfiguration_c,
    gWptPtuStatePowerSave_c,
    gWptPtuStateLowPower_c,
    gWptPtuStatePowerTransfer_c,
    gWptPtuStateLocalFault_c,
    gWptPtuStateLatchingFault_c
}wptPtuStates_t;

typedef enum wptPruRegStates_tag{
    gStateAdvRecv_c,
    gStateConnEstablished_c,
    gStatePruStaticRead_c,
    gStatePruPtuStaticWritten_c,
    gStatePruDynamicRead_c,
    gStatePruAlertCccdWritten_c
}wptPruRegStates_t;

typedef struct wptPtuInfo_tag
{
    wptPtuConfig_t                     wptClientConfig;
    /* pru specific information */
    wptPruDynamicParamCharacteristic_t wptDynamicChar;
    wptPruStaticParamCharacteristic_t  wptStaticChar;
    wptPruControlCharacteristic_t      wptControlChar;
    wptPruAlertCharacteristic_t        wptAlertChar;
}wptPtuInfo_t;

typedef struct powerSharingInfo_tag
{
    uint64_t prevRequestTime;
    bool_t requestInProgress;
    bool_t alreadySharing;
    adjustPowerCommandValues_t commandType;
}powerSharingInfo_t;

typedef struct appPeerInfo_tag
{
    uint32_t           idleTime;
    deviceId_t         deviceId;
    wptPtuInfo_t       deviceInfo;
    bool_t             isBonded;
    bool_t             isRegistered;
    powerSharingInfo_t pwrShareInfo;
}appPeerInfo_t;
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/* Scanning variables */
static bool_t     mScanningOn = FALSE;
static uint16_t   mCurrentServiceHandle = 0xFFFF;
static deviceId_t mCurrentPeerDeviceId  = gInvalidDeviceId_c;
static uint8_t    mConnectedPruCount    = 0;

/* Application specific data*/
static appPeerInfo_t maPeerInformation[gAppMaxConnections_c];
static const wptPtuStaticParamCharacteristic_t mcWptPtuInfo = {
    .optFields = {.rawOptionalFields = 0},
    .ptuPower = gPtuStaticPower16Watts_c,
    .maxSourceImpedance = 0,
    .maxLoadResistance  = 0,
    .RFU_undef = {0, 0},
    .ptuClass = (uint8_t)gPtuClass3_c,
    .hwRev = 0,
    .swRev = 0,
    .protocolRevision = (uint8_t)gProtocolRevision_130_c,
    .numberOfDevices  = gAppMaxConnections_c - 1,
    .RFU = {0, 0, 0, 0, 0, 0}
};
static wptPtuStates_t mAppState = gWptPtuStateConfiguration_c;
static uint32_t       mTotalAvailablePower = 0;
static bool_t mPowerSharingInProgress = FALSE;
/* data structures for Characteristic related procedures */
gattCharacteristic_t mCharacteristic[gAppMaxConnections_c];
gattAttribute_t mAttribute;
/* Timers */
static tmrTimerID_t mAppTimerId = gTmrInvalidTimerID_c;
static tmrTimerID_t mPtuTimeoutTimerId = gTmrInvalidTimerID_c;

/* Console messages */
static char* const maDisplayPtuState         = "\r\n PTU STATE: ";
static char* const maDisplaySeparator        = "\r\n******************************\r\n";
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


static void BleApp_Config();

void BleApp_StateMachineHandler
(
    deviceId_t peerDeviceId,
    appEvent_t event
);

static bool_t CheckScanEvent
(
 gapScannedDevice_t* pData, 
 uint16_t* pOutServiceHandle
);

static void BleApp_RegistrationTimeoutCallback(void* param);

static bool_t BleApp_DeviceRegistrationSm
(
 deviceId_t peerDeviceId, 
 appEvent_t event
);

static void BleApp_HandlePruDynamicReceived(deviceId_t peerDeviceId);

static void BleApp_EnablePruOutput
(
 deviceId_t peerDeviceId, 
 bool_t enable,
 wptPruControlCharPermValues_t reason,
 adjustPowerCommandValues_t command
);

static void BleApp_MonitorCallback(void* param);

static void BleApp_HandlePowerSharingMode(deviceId_t peerDeviceId);

static void BleApp_PrintPtuState(wptPtuStates_t mState);

static void BleApp_PrintPruStatus
(
 deviceId_t deviceId, 
 wptPruAlertCharDyn_t* pPruAlert
);

static void BleApp_PrintSystemError
(
 deviceId_t peerDeviceId, 
 wptPruAlertCharacteristic_t* pPruAlert
);

static void BleApp_PrintSystemParameters(void);

static uint16_t BleApp_ComputeMaxConsumedPower(void);
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
    shell_init("A4WPT PTU>");
    TMR_TimeStampInit();
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
    static bool_t bFirstPress = FALSE;
    
    if(!bFirstPress)
    {
        bFirstPress = TRUE;
        BleApp_StateMachineHandler(gInvalidDeviceId_c, gPtuEventLoadVariation_c);
        BleApp_Start();
        return;
    }
    
    switch (events)
    {
    case gKBD_EventPressPB1_c:
        {
            if(gWptPtuStatePowerSave_c == mAppState ||
               gWptPtuStateLowPower_c  == mAppState || 
                   gWptPtuStatePowerTransfer_c == mAppState)
            {
                if(gAppMaxConnections_c > mConnectedPruCount)
                {
                    BleApp_Start();
                }
            }
            break;
        }
    case gKBD_EventLongPB1_c:
        {
            if(gWptPtuStatePowerTransfer_c == mAppState)
            {
                BleApp_PrintSystemParameters();
            }
        }
        break;
    case gKBD_EventVeryLongPB1_c:
        {
            for (uint8_t i = 0; i < gAppMaxConnections_c; i++)
            {
                if (maPeerInformation[i].deviceId != gInvalidDeviceId_c)
                    Gap_Disconnect(maPeerInformation[i].deviceId);
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
    uint8_t i = 0;
    /* Configure as GAP Central */
    BleConnManager_GapCentralConfig();

    /* Register for callbacks*/
    App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    App_RegisterGattClientNotificationCallback(BleApp_GattNotificationCallback);
  
    for(i = 0; i < gAppMaxConnections_c; ++i)
    {
        maPeerInformation[i].deviceId = gInvalidDeviceId_c;
        maPeerInformation[i].isRegistered = FALSE;
    }
    /* UI */
    shell_write("\r\nPress any PB to simulate PRU device detected!\r\n");
    
    /* allocate application timer */
    mAppTimerId = TMR_AllocateTimer();
    /* allocate registration timer */
    mPtuTimeoutTimerId = TMR_AllocateTimer();
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
            if (CheckScanEvent(&pScanningEvent->eventData.scannedDevice, &mCurrentServiceHandle))
            {        
                gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                FLib_MemCpy(gConnReqParams.peerAddress, 
                            pScanningEvent->eventData.scannedDevice.aAddress,
                            sizeof(bleDeviceAddress_t));
                
                Gap_StopScanning();
#if gAppUsePrivacy_d
                gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif
                /* connect to device */
                App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                /* update state machine */
                BleApp_StateMachineHandler(gInvalidDeviceId_c, gPtuEventDevRegFoundDev_c);
            }
        }        
        break;
        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;
            if(!mScanningOn)
            {
                LED_StopFlashingAllLeds();
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
                        
            maPeerInformation[peerDeviceId].deviceId = peerDeviceId;
            maPeerInformation[peerDeviceId].isBonded = FALSE;
            maPeerInformation[peerDeviceId].isRegistered = FALSE;
            
            maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hService 
                = mCurrentServiceHandle;
            /* fast compute for all handles according to specification requirements*/
            maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hPruControl   
                = mCurrentServiceHandle + 2;
            maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hPtuStatic    
                = mCurrentServiceHandle + 4;
            maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hPruAlert     
                = mCurrentServiceHandle + 6;
            maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hPruAlertCccd 
                = mCurrentServiceHandle + 7;
            maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hPruStatic    
                = mCurrentServiceHandle + 9;
            maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hPruDynamic   
                = mCurrentServiceHandle + 11;
            
            ++mConnectedPruCount;
            
#if gAppUseBonding_d            
            Gap_CheckIfBonded(peerDeviceId, &maPeerInformation[peerDeviceId].isBonded);
            
            if (maPeerInformation[peerDeviceId].isBonded)
            {
                /* Encrypt link */
                Gap_EncryptLink(peerDeviceId);
            }
            else
            {
                Gap_Pair(peerDeviceId, &gPairingParameters);
            }
#else
            /* start pairing */
            Gap_Pair(peerDeviceId, &gPairingParameters);
#endif
            BleApp_StateMachineHandler(maPeerInformation[peerDeviceId].deviceId, gPtuEventDevRegConnected_c);
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            maPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;
            maPeerInformation[peerDeviceId].isRegistered = FALSE;
            
            --mConnectedPruCount;
            BleApp_StateMachineHandler(peerDeviceId, gPtuEventPeerDisconnected_c);
        }
        break;

#if gAppUsePairing_d		
        case gConnEvtPairingComplete_c:
        {

        }
        break;
        case gConnEvtEncryptionChanged_c:
        {
#if gAppUseBonding_d
#endif
        }
        break;
#endif /* gAppUsePairing_d */

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
        if(!maPeerInformation[serverDeviceId].isRegistered)
        {
            BleApp_StateMachineHandler(serverDeviceId, gPtuEventDevRegGattProcFailed_c);
        }
        else
        {
            
        }
    }
    else if (procedureResult == gGattProcSuccess_c)
    {        
        if(!maPeerInformation[serverDeviceId].isRegistered)
        {
            BleApp_StateMachineHandler(serverDeviceId, gPtuEventDevRegGattProcComp_c);
        }
        else
        {
            /* perform all checks here */
            if(gGattProcReadCharacteristicValue_c == procedureType)
            {
                BleApp_HandlePruDynamicReceived(serverDeviceId);
            }
        }
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
    if (characteristicValueHandle == 
        maPeerInformation[serverDeviceId].deviceInfo.wptClientConfig.hPruAlert)
    {
        wptPruAlertCharacteristic_t* pAlertValue = (wptPruAlertCharacteristic_t*)aValue;
        /* save data */
        maPeerInformation[serverDeviceId].deviceInfo.wptAlertChar.pruAlert.rawPruAlert = 
            pAlertValue->pruAlert.rawPruAlert;
        
        BleApp_StateMachineHandler(serverDeviceId, gPtuEventSystemError_c);
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

static bool_t CheckScanEvent(gapScannedDevice_t* pData, uint16_t* pOutServiceHandle)
{
    const uint16_t serviceUUID = gBleSig_WPTService_d;
    
    uint8_t index = 0;
    bool_t foundMatch = FALSE;
    
    bool_t bIsLimitedDiscovery = FALSE;
    
    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;
        
        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1];
        adElement.aData = &pData->data[index + 2];
        
        /* search if adv is limited discovery mode */
        if((gapAdTypeFlags_t)adElement.aData[0] & gLeLimitedDiscoverableMode_c)
        {
            bIsLimitedDiscovery = TRUE;
        }
        
        /* Search for WPT Service */
        if (adElement.adType == gAdServiceData16bit_c && bIsLimitedDiscovery)
        {
            foundMatch = MatchDataInAdvElementList(&adElement, (void*)&serviceUUID, 2);
            if(foundMatch)
            {
                *pOutServiceHandle = adElement.aData[2] + (adElement.aData[3] << 8);
            }
        }
        
        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
    }
    return foundMatch;
}

void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event)
{
	uint8_t i = 0;
    /* handle device registration */
    if(gPtuEventDevRegTimeout_c >= event)
    {
        /* registration finished succesfully or with error */
        if(TRUE == BleApp_DeviceRegistrationSm(peerDeviceId, event))
        {
            if(maPeerInformation[peerDeviceId].isRegistered)
            {
                maPeerInformation[peerDeviceId].idleTime = 0;
                if(!TMR_IsTimerActive(mAppTimerId))
                {
                    TMR_StartIntervalTimer(mAppTimerId,
                                           TmrMilliseconds(gTMonitorInterval_c),
                                           BleApp_MonitorCallback,
                                           NULL);
                }
            }
        }
    }
    switch (mAppState)
    {
    case gWptPtuStateConfiguration_c:
        if(gPtuEventLoadVariation_c == event)
        {
            mAppState = gWptPtuStatePowerSave_c;
            BleApp_PrintPtuState(mAppState);
            
            shell_write(maDisplaySeparator);
            shell_write("Device configuration complete");
        }
        else if(gPtuEventLocalFault_c == event)
        {
            mAppState = gWptPtuStateLocalFault_c;
            BleApp_PrintPtuState(mAppState);
        }
        break;
    case gWptPtuStatePowerSave_c:
        if(gInvalidDeviceId_c != peerDeviceId)
        {
            if(gInvalidDeviceId_c != maPeerInformation[peerDeviceId].deviceId &&
               maPeerInformation[peerDeviceId].isRegistered)
            {
                mAppState = gWptPtuStateLowPower_c;
                BleApp_PrintPtuState(mAppState);
            }
        }
        break;
    case gWptPtuStateLowPower_c:
        if(gPtuEventChargeDeniedForDevice_c == event)
        {
            BleApp_HandlePowerSharingMode(peerDeviceId);
        }
        else if(gPtuEventChargeAllowedForDevice_c  == event)
        {
            /* enable output charge for device */
            BleApp_EnablePruOutput(peerDeviceId, 
                                   TRUE, 
                                   gPermittedWithoutReason_c, 
                                   gAdjustPowerCommandMaxPower_c);
       
            shell_write(maDisplaySeparator);
            shell_write(" Enabling charge for device ");
            shell_writeDec(maPeerInformation[peerDeviceId].deviceId);
            
            mAppState = gWptPtuStatePowerTransfer_c;
            BleApp_PrintPtuState(mAppState);
        }
        else if(gPtuEventPeerDisconnected_c == event)
        {
            if(mConnectedPruCount == 0)
            {
                Led1Off();
            
                shell_write(maDisplaySeparator);
                shell_write("connection to all PRUs lost\r\n");
                
                mAppState = gWptPtuStatePowerSave_c;
                BleApp_PrintPtuState(mAppState);
            }
        }
        else if(gPtuEventLocalFault_c == event)
        {
            mAppState = gWptPtuStateLocalFault_c;
            BleApp_PrintPtuState(mAppState);
        }
        break;
    case gWptPtuStatePowerTransfer_c:
        switch(event)
        {
        case gPtuEventDeviceCharged_c:
            BleApp_EnablePruOutput(peerDeviceId, 
                                   FALSE, 
                                   gPermittedWithoutReason_c, 
                                   gAdjustPowerCommandMaxPower_c);
            break;
        case gPtuEventPeerDisconnected_c:
            BleApp_PrintPruStatus(peerDeviceId, 
                                  &maPeerInformation[peerDeviceId].deviceInfo.wptDynamicChar.pruAlert);
            if(mConnectedPruCount == 0)
            {
                Led1Off();
                Led1Flashing();
                Led2Flashing();
                Led3Flashing();
                Led4Flashing(); 
                shell_write(maDisplaySeparator);
                shell_write("connection to all PRUs lost\r\n");
                
                mAppState = gWptPtuStatePowerSave_c;
                BleApp_PrintPtuState(mAppState);
            }
            break;
        case gPtuEventChargeAllowedForDevice_c:
            /* enable output charge for device */
            BleApp_EnablePruOutput(peerDeviceId, 
                                   TRUE, 
                                   gPermittedWithoutReason_c, 
                                   gAdjustPowerCommandMaxPower_c);
       
            shell_write(maDisplaySeparator);
            shell_write(" Enabling charge for device ");
            shell_writeDec(maPeerInformation[peerDeviceId].deviceId);
            break;
        case gPtuEventChargeDeniedForDevice_c:
            /* disable output charge and provide reason */
            BleApp_HandlePowerSharingMode(peerDeviceId);
            break;
        case gPtuEventLocalFault_c:
            mAppState = gWptPtuStateLocalFault_c;
            BleApp_PrintPtuState(mAppState);
            break;
        case gPtuEventSystemError_c:
            mAppState = gWptPtuStateLatchingFault_c;
            /* print system error data and state */
            BleApp_PrintSystemError(peerDeviceId, &maPeerInformation[peerDeviceId].deviceInfo.wptAlertChar);
            BleApp_PrintPtuState(mAppState);

            for(i = 0; i < gAppMaxConnections_c; i++)
            {
            	if(maPeerInformation[i].deviceId != gInvalidDeviceId_c &&
            	   maPeerInformation[i].isRegistered)
            	{
                    Gap_Disconnect(i);
            	}
            }

            break;
        default:
            break;
        }
        break;
    case gWptPtuStateLocalFault_c:
    	shell_write(maDisplaySeparator);
    	shell_write("Internal error: reset device to continue");
        break;
    case gWptPtuStateLatchingFault_c:
    	if(mConnectedPruCount == 0)
    	{
            Led1Off();
            Led1Flashing();
            Led2Flashing();
            Led3Flashing();
            Led4Flashing();
            shell_write(maDisplaySeparator);
            shell_write("connection to all PRUs lost\r\n");
            
            mAppState = gWptPtuStatePowerSave_c;
            BleApp_PrintPtuState(mAppState);
    	}
        break;
    }
}

static void BleApp_RegistrationTimeoutCallback(void* param)
{
    BleApp_StateMachineHandler(mCurrentPeerDeviceId, 
                               gPtuEventDevRegTimeout_c);
}

static void BleApp_MonitorCallback(void* param)
{
    uint8_t i = 0;
    wptPtuInfo_t* pDevInfo;
    bleResult_t result;
    
    for(i = 0; i < gAppMaxConnections_c; i++)
    {
        if(maPeerInformation[i].deviceId != gInvalidDeviceId_c &&
           maPeerInformation[i].isRegistered)
        {
            pDevInfo = &(maPeerInformation[i].deviceInfo);
            /* read dynamic characteristics */
            mCharacteristic[i].value.handle = pDevInfo->wptClientConfig.hPruDynamic;
            mCharacteristic[i].value.paValue = (uint8_t*)(&pDevInfo->wptDynamicChar);
            result = GattClient_ReadCharacteristicValue(i, &mCharacteristic[i], gWpt_PruDynamicLen_c);
            if(gBleSuccess_c != result)
            {
                BleApp_StateMachineHandler(i, gPtuEventLocalFault_c);
            }
            else
            {
                maPeerInformation[i].idleTime += gTMonitorInterval_c;
                /* idle connection */
                if(maPeerInformation[i].idleTime >= gIdleTimeTimeout_c)
                {
                    Gap_Disconnect(i);
                }
            }
        }
    }
}

static bool_t BleApp_DeviceRegistrationSm
(
 deviceId_t peerDeviceId, 
 appEvent_t event
)
{
    static wptPruRegStates_t appState = gStateAdvRecv_c;
    uint8_t cccdValue[2];
    bleResult_t result                = gBleSuccess_c;
    wptPtuInfo_t* pDevInfo            = NULL;
    bool_t moveToNextState            = FALSE;
    bool_t registerFailed             = FALSE;
    bool_t registerFinished           = FALSE;
      
    if(gPtuEventDevRegTimeout_c == event ||
       gPtuEventDevRegGattProcFailed_c == event)
    {
        registerFailed = TRUE;
    }
    else if(gPtuEventDevRegFoundDev_c == event && 
            gStateAdvRecv_c == appState)
    {
        moveToNextState = TRUE;
    }
    else if(gPtuEventDevRegConnected_c == event && 
            gStateConnEstablished_c == appState)
    {
        moveToNextState = TRUE;
    }
    else if(gPtuEventDevRegGattProcComp_c == event &&
            (gStatePruDynamicRead_c == appState || 
             gStatePruStaticRead_c  == appState ||
             gStatePruPtuStaticWritten_c == appState ||
             gStatePruAlertCccdWritten_c == appState))
    {
        moveToNextState = TRUE;
    }
    switch(appState)
    {
    case gStateAdvRecv_c:
        if(moveToNextState)
        {
            /* start registration timer */
            TMR_StartSingleShotTimer(mPtuTimeoutTimerId,
                                     TmrMilliseconds(gTRegistration_c),
                                     BleApp_RegistrationTimeoutCallback,
                                     NULL);
            appState = gStateConnEstablished_c;
        }
        break;
    case gStateConnEstablished_c:
        if(moveToNextState)
        {
            mCurrentPeerDeviceId = peerDeviceId;
            maPeerInformation[peerDeviceId].isRegistered = FALSE;
            
            pDevInfo = &maPeerInformation[peerDeviceId].deviceInfo;
            
            mCharacteristic[peerDeviceId].value.handle = pDevInfo->wptClientConfig.hPruStatic;
            mCharacteristic[peerDeviceId].value.paValue = (uint8_t*)(&pDevInfo->wptStaticChar);
            /* read pru static */
            result = GattClient_ReadCharacteristicValue(peerDeviceId, 
                                                        &mCharacteristic[peerDeviceId], 
                                                        gWpt_PruStaticLen_c);
            if(gBleSuccess_c != result)
            {
                registerFailed = TRUE;
            }
            else
            {
                appState = gStatePruStaticRead_c;
            }
        }
        break;
    case gStatePruStaticRead_c:
        if(moveToNextState)
        {
            /* write PTU static characteristic value */
            pDevInfo = &maPeerInformation[peerDeviceId].deviceInfo;
            
            mCharacteristic[peerDeviceId].value.handle = pDevInfo->wptClientConfig.hPtuStatic;
            
            result = GattClient_CharacteristicWriteWithoutResponse(peerDeviceId,
                                                                   &mCharacteristic[peerDeviceId],
                                                                   gWpt_PtuStaticLen_c,
                                                                   (uint8_t*)&mcWptPtuInfo);
            if(gBleSuccess_c != result)
            {
                registerFailed = TRUE;
            }
            else
            {
                appState = gStatePruPtuStaticWritten_c;
            }    
        }
        break;
    case gStatePruPtuStaticWritten_c:
        if(moveToNextState)
        {
            pDevInfo = &maPeerInformation[peerDeviceId].deviceInfo;
            mCharacteristic[peerDeviceId].value.handle = pDevInfo->wptClientConfig.hPruDynamic;
            mCharacteristic[peerDeviceId].value.paValue = (uint8_t*)(&pDevInfo->wptDynamicChar);
            /* dynamic characteristic read */
            result = GattClient_ReadCharacteristicValue(peerDeviceId,
                                                        &mCharacteristic[peerDeviceId],
                                                        gWpt_PruDynamicLen_c);
            if(gBleSuccess_c != result)
            {
                registerFailed = TRUE;
            }
            else
            {
                appState = gStatePruDynamicRead_c;
            }
        }
        break;
    case gStatePruDynamicRead_c:
        if(moveToNextState)
        {
            TMR_StopTimer(mPtuTimeoutTimerId);

            Utils_PackTwoByteValue(gCccdNotification_c, cccdValue);
            /* enable alert notification */
            mAttribute.handle = maPeerInformation[peerDeviceId].deviceInfo.wptClientConfig.hPruAlertCccd;
            mAttribute.paValue = cccdValue;
            result = GattClient_WriteCharacteristicDescriptor(maPeerInformation[peerDeviceId].deviceId,
                                                              &mAttribute, 
                                                              sizeof(cccdValue), 
                                                              cccdValue);
            if(gBleSuccess_c != result)
            {
                registerFailed = TRUE;
            }
            else
            {
                appState = gStatePruAlertCccdWritten_c;
            }
        }
        break;
    case gStatePruAlertCccdWritten_c:
        if(moveToNextState)
        {
            registerFinished = TRUE;
            maPeerInformation[peerDeviceId].isRegistered = TRUE;
            
            maPeerInformation[peerDeviceId].pwrShareInfo.alreadySharing = FALSE;
            maPeerInformation[peerDeviceId].pwrShareInfo.requestInProgress = FALSE;
            maPeerInformation[peerDeviceId].pwrShareInfo.commandType = gAdjustPowerCommandMaxPower_c;
            
            maPeerInformation[peerDeviceId].deviceInfo.wptControlChar.enables.rawEnables = 0;
            maPeerInformation[peerDeviceId].deviceInfo.wptControlChar.permission = gPermittedWithoutReason_c;
            
            mCurrentPeerDeviceId = gInvalidDeviceId_c;
            
            appState = gStateAdvRecv_c;
            
            shell_write(maDisplaySeparator);
            
            /* UI */
            shell_write("\r\nRegistered WPT PRU device ");
            shell_writeDec(peerDeviceId);
            shell_write("\r\nDevice address: ");
            shell_writeHex(gConnReqParams.peerAddress, gcBleDeviceAddressSize_c);
            SHELL_NEWLINE();
        }
        break;
    default:
        registerFailed = TRUE;
        break;
    }
    if(registerFailed)
    {
        if(gInvalidDeviceId_c != peerDeviceId)
        {
            /* disconnect if procedure failes */
            if(gInvalidDeviceId_c != maPeerInformation[peerDeviceId].deviceId)
            {
                maPeerInformation[peerDeviceId].isRegistered = FALSE;
                Gap_Disconnect(peerDeviceId);
            }
        }
        else
        {
            /* stop scanning if procedure fails before connection */
            Gap_StopScanning();
        }
        TMR_StopTimer(mPtuTimeoutTimerId);
        registerFinished = TRUE;
        appState = gStateAdvRecv_c;
    }
    return registerFinished;
}

static void BleApp_HandlePruDynamicReceived(deviceId_t peerDeviceId)
{
    wptPtuInfo_t* pDevInfo = &(maPeerInformation[peerDeviceId].deviceInfo);
    /* check if device is charged */
    if(pDevInfo->wptDynamicChar.pruAlert.fields.chargeComplete)
    {
        /* Disable charge port */
        if(pDevInfo->wptDynamicChar.pruAlert.fields.pruChargePort)
        {
            BleApp_StateMachineHandler(peerDeviceId, gPtuEventDeviceCharged_c);
        }
        /* charge complete, disconnected mode */
        else
        {
            Gap_Disconnect(peerDeviceId);
        }
    } 
    /* check if device is not charging */
    else if(!pDevInfo->wptDynamicChar.pruAlert.fields.pruChargePort)
    {
        mTotalAvailablePower = (uint16_t)gPtuDefaultPower_mW_c - 
            BleApp_ComputeMaxConsumedPower();
        
        uint16_t mPwrNeeded = 0;
        
        if(maPeerInformation[peerDeviceId].pwrShareInfo.alreadySharing)
        {
            switch(maPeerInformation[peerDeviceId].pwrShareInfo.commandType)
            {
            case gAdjustPowerCommand33Percent_c:
                mPwrNeeded = pDevInfo->wptStaticChar.PRectMax_100mW * 100 / 3;
                break;
            case gAdjustPowerCommand66Percent_c:
                mPwrNeeded = pDevInfo->wptStaticChar.PRectMax_100mW * 100 * 2 / 3;
                break;
            case gAdjustPowerCommand2_5Watts_c:
                mPwrNeeded = 2500;
                break;
            default:
                mPwrNeeded = pDevInfo->wptStaticChar.PRectMax_100mW * 100;
                break;
            }
        }
        else
        {
            mPwrNeeded = pDevInfo->wptStaticChar.PRectMax_100mW * 100;
        }
        if(mTotalAvailablePower >= mPwrNeeded)
        {
            BleApp_StateMachineHandler(peerDeviceId, gPtuEventChargeAllowedForDevice_c);
        }
        else
        {
            BleApp_StateMachineHandler(peerDeviceId, gPtuEventChargeDeniedForDevice_c);
        }
    }
    /* handle power sharing */
    if(mPowerSharingInProgress)
    {
        if(maPeerInformation[peerDeviceId].pwrShareInfo.requestInProgress)
        {
            if(pDevInfo->wptDynamicChar.pruAlert.fields.adjPowerResponse)
            {
                maPeerInformation[peerDeviceId].pwrShareInfo.requestInProgress = FALSE;
                maPeerInformation[peerDeviceId].pwrShareInfo.alreadySharing = TRUE;
                maPeerInformation[peerDeviceId].pwrShareInfo.commandType = 
                    (adjustPowerCommandValues_t)pDevInfo->wptControlChar.enables.fields.adjustPowerCommand;
            }
            else if(TMR_GetTimestamp() - 
                    maPeerInformation[peerDeviceId].pwrShareInfo.prevRequestTime >= 2000000)
            {
                maPeerInformation[peerDeviceId].pwrShareInfo.requestInProgress = FALSE;
            }
        }
        BleApp_HandlePowerSharingMode(peerDeviceId);
    }
    maPeerInformation[peerDeviceId].idleTime = 0;
}

static void BleApp_EnablePruOutput
(
 deviceId_t peerDeviceId, 
 bool_t enable,
 wptPruControlCharPermValues_t reason,
 adjustPowerCommandValues_t command
)
{
    bleResult_t result;
    wptPtuInfo_t* pDevInfo = &(maPeerInformation[peerDeviceId].deviceInfo);
    
    pDevInfo->wptControlChar.enables.fields.enablePruOutput = enable;
    pDevInfo->wptControlChar.enables.fields.enablePruChargeIndicator = enable;
    pDevInfo->wptControlChar.permission = (uint8_t) reason;
    pDevInfo->wptControlChar.enables.fields.adjustPowerCommand = command;
    
    mCharacteristic[peerDeviceId].value.handle = pDevInfo->wptClientConfig.hPruControl;
    
    result = GattClient_CharacteristicWriteWithoutResponse(peerDeviceId,
                                                           &mCharacteristic[peerDeviceId],
                                                           gWpt_PruControlLen_c,
                                                           (uint8_t*)(&pDevInfo->wptControlChar));
    if(gBleSuccess_c != result)
    {
        BleApp_StateMachineHandler(peerDeviceId, gPtuEventLocalFault_c);
        return;
    }
}

static void BleApp_HandlePowerSharingMode(deviceId_t peerDeviceId)
{   
    uint8_t i = 0;
    bool_t procedureEnded = TRUE;
    wptPtuInfo_t* pDevInfo = NULL;
    adjustPowerCommandValues_t commandType = gAdjustPowerCommandMaxPower_c;
    uint16_t mTotalRecoveredWithPowerSharing33 = 0;
    uint16_t mTotalRecoveredWithPowerSharing66 = 0;
    uint16_t mTotalRecoveredWithPowerSharing2_5 = 0;
 
    if(!mPowerSharingInProgress)
    {
        /* add remaining power */
        mTotalRecoveredWithPowerSharing33 = (uint16_t)gPtuDefaultPower_mW_c - 
            BleApp_ComputeMaxConsumedPower();
        mTotalRecoveredWithPowerSharing66 = mTotalRecoveredWithPowerSharing33;
        mTotalRecoveredWithPowerSharing2_5 = mTotalRecoveredWithPowerSharing33;
        /* calculate how much power can be recovered */
       for(i = 0; i < gAppMaxConnections_c; i++)
       {
           if(gInvalidDeviceId_c != maPeerInformation[i].deviceId &&
              maPeerInformation[i].isRegistered)
           {
               pDevInfo = &maPeerInformation[i].deviceInfo;
               /* device supports adjust power capability */
               if(pDevInfo->wptStaticChar.pruInformation.fields.adjustPwrCapability)
               {
                   if(!maPeerInformation[i].pwrShareInfo.alreadySharing)
                   {
                       mTotalRecoveredWithPowerSharing33 += 
                           (uint16_t)pDevInfo->wptStaticChar.PRectMax_100mW *200/3;
                       mTotalRecoveredWithPowerSharing66 += 
                           (uint16_t)pDevInfo->wptStaticChar.PRectMax_100mW *100/3;
                       mTotalRecoveredWithPowerSharing2_5 +=
                           (uint16_t)pDevInfo->wptStaticChar.PRectMax_100mW *100 - 2500;
                   }
                   else
                   {
                       if(gAdjustPowerCommand66Percent_c == 
                          maPeerInformation[i].pwrShareInfo.commandType)
                       {
                           mTotalRecoveredWithPowerSharing33 += 
                               (uint16_t)pDevInfo->wptStaticChar.PRectMax_100mW *100/3;
                           mTotalRecoveredWithPowerSharing66 += 0; 
                           mTotalRecoveredWithPowerSharing2_5 += 
                               ((uint16_t)pDevInfo->wptStaticChar.PRectMax_100mW *200/3) - 2500;
                       }
                       else if(gAdjustPowerCommand33Percent_c == 
                               maPeerInformation[i].pwrShareInfo.commandType)
                       {
                           mTotalRecoveredWithPowerSharing33 += 0;
                           mTotalRecoveredWithPowerSharing66 += 0; 
                           mTotalRecoveredWithPowerSharing2_5 += 
                               ((uint16_t)pDevInfo->wptStaticChar.PRectMax_100mW *100/3) - 2500;
                       }
                   }
               }
           }
           if(mTotalRecoveredWithPowerSharing66 >= 
              maPeerInformation[peerDeviceId].deviceInfo.wptStaticChar.PRectMax_100mW * 100)
           {
               commandType = gAdjustPowerCommand66Percent_c;
               break;
           }
           else if(mTotalRecoveredWithPowerSharing33 >= 
                   maPeerInformation[peerDeviceId].deviceInfo.wptStaticChar.PRectMax_100mW * 100)
           {
               commandType = gAdjustPowerCommand33Percent_c;
               break;
           }
           else if(mTotalRecoveredWithPowerSharing2_5 >= 
                   maPeerInformation[peerDeviceId].deviceInfo.wptStaticChar.PRectMax_100mW * 100)
           {
               commandType = gAdjustPowerCommand2_5Watts_c;
               break;
           }
       }
       /* if recoverable power is sufficient start procedure */
       if(i < gAppMaxConnections_c)
       {
           mPowerSharingInProgress = TRUE;
           do
           {
               if(gInvalidDeviceId_c != maPeerInformation[i].deviceId &&
                  maPeerInformation[i].isRegistered)
               {
                   pDevInfo = &maPeerInformation[i].deviceInfo;
                   
                   if(pDevInfo->wptStaticChar.pruInformation.fields.adjustPwrCapability
                      && ((uint8_t)commandType > 
                          (uint8_t)maPeerInformation[i].pwrShareInfo.commandType))
                   {                  
                       maPeerInformation[i].pwrShareInfo.requestInProgress = TRUE;
                       maPeerInformation[i].pwrShareInfo.prevRequestTime = 
                           TMR_GetTimestamp();
                       if(i != peerDeviceId)
                       {
                           BleApp_EnablePruOutput(i, 
                                                  TRUE, 
                                                  gPermittedWithoutReason_c, 
                                                  commandType);
                       }
                       else
                       {
                           BleApp_EnablePruOutput(i, 
                                                  FALSE, 
                                                  gPermittedWithWaitingTime_c, 
                                                  commandType);
                       }
                   }
               }
           }
           while(i--);
       }
       else
       {
           /* avoid printing multiple times */
           if(gDeniedDueToLtdAvailablePower_c != 
              maPeerInformation[peerDeviceId].deviceInfo.wptControlChar.permission)
           {
               shell_write(maDisplaySeparator);
               shell_write(" Insufficient power for device ");
               shell_writeDec(maPeerInformation[peerDeviceId].deviceId);
           }
           /* disable output charge and provide reason */
           BleApp_EnablePruOutput(peerDeviceId, 
                                  FALSE, 
                                  gDeniedDueToLtdAvailablePower_c,
                                  gAdjustPowerCommandMaxPower_c);
       } 
    }
    else
    {
        for(i = 0; i < gAppMaxConnections_c; i++)
        {
            if(gInvalidDeviceId_c != maPeerInformation[i].deviceId &&
               maPeerInformation[i].isRegistered &&
               maPeerInformation[i].pwrShareInfo.requestInProgress)
            {
                procedureEnded = FALSE;
                break;
            }
        }
        if(procedureEnded)
        {
            mPowerSharingInProgress = FALSE;
        }
    }
}

static uint16_t BleApp_ComputeMaxConsumedPower(void)
{
    uint16_t mTotalPower_mW = 0;
    uint8_t i = 0;
    wptPtuInfo_t* pDevInfo;
    
    for(i = 0; i < gAppMaxConnections_c; i++)
    {
        if(gInvalidDeviceId_c != maPeerInformation[i].deviceId &&
           TRUE == maPeerInformation[i].isRegistered)
        {
            pDevInfo = &maPeerInformation[i].deviceInfo;
            /* if charge port is enabled this means pru consumes power */
            if(pDevInfo->wptDynamicChar.pruAlert.fields.pruChargePort &&
               pDevInfo->wptControlChar.enables.fields.enablePruOutput)
            {
                switch(maPeerInformation[i].pwrShareInfo.commandType)
                {
                case gAdjustPowerCommand33Percent_c:
                    mTotalPower_mW += pDevInfo->wptStaticChar.PRectMax_100mW * 100 / 3;
                    break;
                case gAdjustPowerCommand66Percent_c:
                    mTotalPower_mW += pDevInfo->wptStaticChar.PRectMax_100mW * 100 * 2 / 3;
                    break;
                case gAdjustPowerCommand2_5Watts_c:
                    mTotalPower_mW += 2500;
                    break;
                default:
                    mTotalPower_mW += pDevInfo->wptStaticChar.PRectMax_100mW * 100;
                    break;
                }
            }
        }
    }
    return mTotalPower_mW;
}

static void BleApp_PrintSystemError
(
 deviceId_t peerDeviceId, 
 wptPruAlertCharacteristic_t* pPruAlert
)
{
    shell_write(maDisplaySeparator);
    shell_write("Device ");
    shell_writeDec(peerDeviceId);
    shell_write(" error: ");
    if(pPruAlert->pruAlert.fields.pruSelfProtection)
    {
        shell_write("self protection");
    }
    else if(pPruAlert->pruAlert.fields.pruOverTemperature)
    {
        shell_write("over temperature");
    }
    else if(pPruAlert->pruAlert.fields.pruOverCurrent)
    {
        shell_write("over current");
    }
    else if(pPruAlert->pruAlert.fields.pruOverVoltage)
    {
        shell_write("over voltage");
    }
}

static void BleApp_PrintPtuState(wptPtuStates_t mState)
{
    shell_write(maDisplaySeparator);
    shell_write(maDisplayPtuState);
    switch(mState)
    {
    case gWptPtuStateConfiguration_c:
        shell_write("Configuration");
        break;
    case gWptPtuStatePowerSave_c:
        shell_write("Power Save");
        break;
    case gWptPtuStateLowPower_c:
        shell_write("Low Power");
        break;
    case gWptPtuStatePowerTransfer_c:
        shell_write("Power Transfer");
        break;
    case gWptPtuStateLocalFault_c:
        shell_write("Local fault");
        break;
    case gWptPtuStateLatchingFault_c:
        shell_write("Latching fault");
        break;
    default:
        shell_write("undefined");
        break;
    }
}

static void BleApp_PrintPruStatus
(
 deviceId_t deviceId,
 wptPruAlertCharDyn_t* pPruAlert
)
{
    char* const pTrue = "TRUE";
    char* const pFalse = "FALSE";
    
    shell_write(maDisplaySeparator);
    shell_write("Device ID: ");
    shell_writeDec(deviceId);
    SHELL_NEWLINE();
    shell_write("Out Ena: ");
    if(pPruAlert->fields.pruChargePort)
    {
        shell_write(pTrue);
    }
    else
    {
        shell_write(pFalse);
    }
    
    shell_write(", Chg Complete: ");
    if(pPruAlert->fields.chargeComplete)
    {
        shell_write(pTrue);
    }
    else
    {
        shell_write(pFalse);
    }
}

static void BleApp_PrintSystemParameters(void)
{
    uint8_t i = 0;
    wptPtuInfo_t* pDevInfo;
    
    uint8_t lowVoltageCount = 0;
    uint8_t highVoltageCount = 0;
    
    mTotalAvailablePower = (uint16_t)gPtuDefaultPower_mW_c -
        BleApp_ComputeMaxConsumedPower();
    
    shell_write(maDisplaySeparator);
    shell_write("Maximum remaining power: ");
    shell_writeDec(mTotalAvailablePower);
    for(i = 0; i < gAppMaxConnections_c; i++)
    {
        if(gInvalidDeviceId_c != maPeerInformation[i].deviceId &&
           maPeerInformation[i].isRegistered)
        {
            shell_write("\r\nDevice ID: ");
            shell_writeDec(i);
            pDevInfo = &(maPeerInformation[i].deviceInfo);
            
            shell_write("\r\nVrect Dyn (mV): ");
            shell_writeDec(pDevInfo->wptDynamicChar.vRect);
            shell_write(" ,Irect Dyn (mA): ");
            shell_writeDec(pDevInfo->wptDynamicChar.iRect);
            shell_write(" ,Prect(mW): ");
            shell_writeDec(pDevInfo->wptDynamicChar.vRect * pDevInfo->wptDynamicChar.iRect/1000);
            if(pDevInfo->wptDynamicChar.vRect <=
               pDevInfo->wptStaticChar.vRectMinStatic_mV)
            {
                lowVoltageCount++;
            }
            else if(pDevInfo->wptDynamicChar.vRect >=
                    pDevInfo->wptStaticChar.vRectHighStatic_mV)
            {
                highVoltageCount++;
            }
        }
    }
    shell_write("\r\nPTU sub-state: ");
    if(lowVoltageCount)
    {
        shell_write("low voltage");
    }
    else if(highVoltageCount)
    {
        shell_write("high voltage");
    }
    else
    {
        shell_write("optimal voltage");
    }
}
/*! *********************************************************************************
* @}
********************************************************************************** */
