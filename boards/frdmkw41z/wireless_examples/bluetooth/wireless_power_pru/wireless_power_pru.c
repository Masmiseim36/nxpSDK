/*! *********************************************************************************
* \addtogroup Wireless Power PRU
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
* This file is the source file for the Temperature Sensor application
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
#include "MemManager.h"
#include "RNG_Interface.h"
#include "Panic.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "wpt_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "SerialManager.h"
#include "ApplMain.h"
#include "wireless_power_pru.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define BleApp_CheckOvp()               (mVRect >= (uint16_t)gVRectMax_mV_c)
#define BleApp_CheckPowerRemoved()      (mVRect <= (uint16_t)gVRectUvlo_mV_c)
#define BleApp_GetVOut()                (5000)
#define BleApp_GetIOut()                (mChargeComplete?0:100)

#define gAdvTimeoutS_c                  (10)
#define gPruMonitorTime_c               (50)
#define gPruAlertTime_c                 (200)
#define gPruOvpOutputDisableTimeout_c   (250)
#define gTemperatureLimitForOtp_c       (125)       /* over temperature protection */
#define gPruTemperatureOffset_c         (40)  /* temperature field starts at -40 deg */
#define gMinPowerDraw_mW_c              (1000) /* mW power when output is disabled */

#ifndef gAdjustPowerCommandSupport_d
#define gAdjustPowerCommandSupport_d    (FALSE)
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum vRectLimits_tag
{
    gVRectUvlo_mV_c = 3300,
    gVRectBoot_mV_c = 3800,
    gVRectMin_mV_c  = 6500,
    gVRectSet_mV_c  = 12000,
    gVRectHigh_mV_c = 18000,
    gVRectMax_mV_c  = 20000 /* >= 1.1 * gVrectHigh_mV_c */
}vRectLimits_t;

typedef struct advState_tag{
    bool_t      advOn;
}advState_t;

typedef enum wptPruStates_tag{
    gWptPruStateNull_c = 0,
    gWptPruStateBoot_c,
    gWptPruStateOn_c,
    gWptPruStateSystemError_c,
    gWptPruStateMaxState_c
}wptPruStates_t;

typedef enum pruEvents_tag{
    gPruEventBeaconDetected_c = 0,
    gPruEventPowerDenied_c,
    gPruEventPowerAllowed_c,
    gPruEventPowerRemoved_c,
    gPruEventChargeComplete_c,
    gPruEventDisconnected_c,
    gPruEventErrorOccured_c,
    gPruEventErrorCorrected_c,
    gPruEventDummyEvent_c
}pruEvents_t;
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Adv State */
static advState_t  mAdvState;

static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;

/* Application specific data*/
static uint8_t mAppSer;
static wptPruStaticParamCharacteristic_t wptPruStaticChar = {
    .optFieldsValidity = {.rawOptionalFields = 0},
    .protocolRevision = (uint8_t)gProtocolRevision_130_c,
    .RFU_undef = 0,
    .pruCategory = (uint8_t)gPruCategory4_c,
#if gAdjustPowerCommandSupport_d
    .pruInformation = {.fields.adjustPwrCapability = 1},
#else
    .pruInformation = {.rawPruInfo = 0x00},
#endif
    .hwRevision = 0,
    .swRevision = 0,
#if gAdjustPowerCommandSupport_d
    .PRectMax_100mW = 90,
#else
    .PRectMax_100mW = 80,
#endif
    .vRectMinStatic_mV = (uint16_t)gVRectMin_mV_c,
    .vRectHighStatic_mV = (uint16_t)gVRectHigh_mV_c,
    .vRectSet_mV = (uint16_t)gVRectSet_mV_c,
    .deltaR1_001Ohm = 0,
    .RFU = {0,0,0,0}
};
static wptPtuStaticParamCharacteristic_t  wptPtuStaticChar;
static wptPruAlertCharacteristic_t        wptPruAlertChar;
static wptPruDynamicParamCharacteristic_t wptPruDynamicChar;

static uint16_t     maRegisteredHandles[2] = {0};
static tmrTimerID_t mPruTimeoutTimerId = gTmrInvalidTimerID_c;
static tmrTimerID_t appTimerId         = gTmrInvalidTimerID_c;

static bool_t   mChargeComplete = FALSE;
static wptPruStates_t mAppState = gWptPruStateNull_c;
static uint16_t mVRect = (uint16_t)gVRectUvlo_mV_c;

#if gAdjustPowerCommandSupport_d
static bool_t mPowerSharingInProgress                   = FALSE;
static adjustPowerCommandValues_t mPwrAdjustCommandType = gAdjustPowerCommandMaxPower_c;
static uint64_t mPowerSharingTimestamp                  = 0;
#endif

/* Console messages */
static char* const maDisplayStaticParams     = "Static Parameters \r\n";
static char* const maDisplayPruState         = "\r\n PRU STATE: ";
static char* const maDisplaySeparator        = "\r\n******************************\r\n";
static char* const maDisplayDynamicParams    = "Dynamic Parameters\r\n";
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
static void BleApp_Config();

static void BleApp_Advertise(void);
static void BleApp_WptPruStateMachineHandler(pruEvents_t event);
static void BleApp_UpdatePruAlert(wptPruAlertCharacteristic_t* pPruAlertChar);
static void BleApp_MonitorPRUDeviceCallback(void* param);
static void BleApp_PrintPruState(wptPruStates_t mState);
static void BleApp_PrintStaticParams(wptPruStaticParamCharacteristic_t* pPruStatic);
static void BleApp_PrintDynamicParams(wptPruDynamicParamCharacteristic_t* pPruDynamic);
static void BleApp_TimeoutCallback(void* param);
static void BleApp_PrintPruStatus(wptPruAlertCharDyn_t* pPruAlert);
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
    /* Initialize serial manager */
    SerialManager_Init();
    /* Initialize lpuart interface */
    Serial_InitInterface(&mAppSer, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    /* Set baud-rate to 115200 */
    Serial_SetBaudRate(mAppSer, APP_SERIAL_INTERFACE_SPEED);
#if gAdjustPowerCommandSupport_d
    TMR_TimeStampInit();
#endif
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    Led1On();
    
    if (mPeerDeviceId == gInvalidDeviceId_c)
    {
        /* Device is not connected and not advertising*/
        if (!mAdvState.advOn)
        {
            BleApp_Advertise();
            TMR_StartSingleShotTimer(mPruTimeoutTimerId, 
                                     TmrSeconds(gAdvTimeoutS_c), 
                                     BleApp_TimeoutCallback, 
                                     NULL);
        }         
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
    /* simulate boot power */    
    case gKBD_EventPB1_c:
        if(BleApp_CheckPowerRemoved())
        {
            BleApp_WptPruStateMachineHandler(gPruEventBeaconDetected_c);
        }
        /* power is applied so use PB1 just to print Dynamic Parameters*/
        else
        {
            BleApp_PrintDynamicParams(&wptPruDynamicChar);
        }
        break;
    /* simulate charge complete */
    case gKBD_EventLongPB1_c:
        mChargeComplete = TRUE;
        break;
    /* simulate power removed */
    case gKBD_EventVeryLongPB1_c:
        if(!BleApp_CheckPowerRemoved())
        {
            BleApp_WptPruStateMachineHandler(gPruEventPowerRemoved_c);
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
        
        case gAdvertisingParametersSetupComplete_c:
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
static void BleApp_Config()
{
    /* Configure as GAP peripheral */
    BleConnManager_GapPeripheralConfig();

    /* Register for callbacks*/
    App_RegisterGattServerCallback(BleApp_GattServerCallback);

    mAdvState.advOn = FALSE;
 
    /* Start service */
    Wpt_Start(service_wpt, &wptPruStaticChar);

    /* register write handles */
    maRegisteredHandles[0] = value_ptu_static;
    maRegisteredHandles[1] = value_pru_control;
    GattServer_RegisterHandlesForWriteNotifications(2, maRegisteredHandles);
    
    /* Allocate aplication timer */
    appTimerId = TMR_AllocateTimer();
    /* Allocate timeout timer */
    mPruTimeoutTimerId = TMR_AllocateTimer();
 
    BleApp_PrintPruState(gWptPruStateNull_c);
}

/*! *********************************************************************************
* \brief      WPT PRU State Machine
*
* \param[in]  event    Event that triggers state change.
*
********************************************************************************** */
static void BleApp_WptPruStateMachineHandler(pruEvents_t event)
{
    static uint32_t mPruAlertCount = gPruAlertTime_c / gPruMonitorTime_c;
    switch(mAppState)
    {
    case gWptPruStateNull_c:
        /* power beacon, sufficient energy to start adv */
        if(gPruEventBeaconDetected_c == event)
        {
            uint16_t len = 0;
            
            mVRect = (uint16_t)gVRectBoot_mV_c;
            /* get default values from profile */
            Wpt_ReadPruDynamic((uint8_t*)&wptPruDynamicChar, &len);
            Wpt_ReadPtuStatic((uint8_t*)&wptPtuStaticChar, &len);
            Wpt_ReadPruAlert((uint8_t*)&wptPruAlertChar, &len);
            
            BleApp_PrintStaticParams(&wptPruStaticChar);
            BleApp_PrintPruState(gWptPruStateBoot_c);
 
            mChargeComplete = FALSE;
#if gAdjustPowerCommandSupport_d
            mPwrAdjustCommandType   = gAdjustPowerCommandMaxPower_c;
            mPowerSharingTimestamp  = 0;
            mPowerSharingInProgress = FALSE;
#endif
            mAppState = gWptPruStateBoot_c;
            
            /* start advertising */
            BleApp_Start();
        }
        break;
    case gWptPruStateBoot_c:
        switch(event)
        {
            /* dummy event means advertising timeout */
        case gPruEventDummyEvent_c:
            /* switch to V UVLO */
            mVRect = (uint16_t)gVRectUvlo_mV_c;
            
            mAppState = gWptPruStateNull_c;
            BleApp_PrintPruState(mAppState);
            break;
        /* PRU Control was written */
        case gPruEventPowerAllowed_c:
            mAppState = gWptPruStateOn_c;
            
            Led2On();
            mVRect = (uint16_t)gVRectSet_mV_c;
            
            /* update the charge port information */
            wptPruDynamicChar.pruAlert.fields.pruChargePort = TRUE;
            
            BleApp_PrintPruState(mAppState);
            BleApp_PrintPruStatus(&wptPruDynamicChar.pruAlert);
            break;
        case gPruEventDisconnected_c:
            /* link loss, start advertising */
            if(!mChargeComplete)
            {
                BleApp_Start();
            }
            else
            {          
                BleApp_PrintDynamicParams(&wptPruDynamicChar);
            }
            break;
        case gPruEventPowerRemoved_c:
            mVRect = (uint16_t)gVRectUvlo_mV_c;
            StopLed2Flashing();
            Led2Off();
            
            mAppState = gWptPruStateNull_c;
            BleApp_PrintPruState(mAppState);
            /* power removed. Initiate disconnect */
            if(gInvalidDeviceId_c != mPeerDeviceId)
            {
                Gap_Disconnect(mPeerDeviceId);
            }
            break;
        default:
            mAppState = gWptPruStateBoot_c;
            break;
        }   
        break;
    case gWptPruStateOn_c:
        switch(event)
        {
        /* power denied, flash led2 sub-state Low Voltage*/
        case gPruEventPowerDenied_c:
            Led2Flashing();
            mVRect = (uint16_t)gVRectBoot_mV_c;
            wptPruDynamicChar.pruAlert.fields.pruChargePort = FALSE;
            
            BleApp_PrintPruStatus(&wptPruDynamicChar.pruAlert);
            break;
        case gPruEventPowerAllowed_c:
            StopLed2Flashing();
            Led2On();
            mVRect = (uint16_t)gVRectSet_mV_c;
            wptPruDynamicChar.pruAlert.fields.pruChargePort = TRUE;
            
            BleApp_PrintPruStatus(&wptPruDynamicChar.pruAlert);
            break;
        /* charge complete, device disconnected or power removed
        *   move to boot state 
        */
        case gPruEventChargeComplete_c:
        case gPruEventDisconnected_c:
            StopLed2Flashing();
            Led2Off();
            wptPruDynamicChar.pruAlert.fields.pruChargePort = FALSE;
            if(!mChargeComplete)
            {
                BleApp_Start();
            }
            else
            {
                BleApp_PrintPruStatus(&wptPruDynamicChar.pruAlert);
            }
            /* switch to V Boot */
            mVRect = (uint16_t)gVRectBoot_mV_c;
            
            mAppState = gWptPruStateBoot_c;
            BleApp_PrintPruState(mAppState);
            break;
        case gPruEventPowerRemoved_c:
            mVRect = (uint16_t)gVRectUvlo_mV_c;
            StopLed2Flashing();
            Led2Off();
            
            mAppState = gWptPruStateNull_c;
            BleApp_PrintPruState(mAppState);
            /* power removed. Initiate disconnect */
            if(gInvalidDeviceId_c != mPeerDeviceId)
            {
                Gap_Disconnect(mPeerDeviceId);
            }
            break;
        /* move to system error to prevent any damage */
        case gPruEventErrorOccured_c:
            if(!BleApp_CheckOvp())
            {
                Led2Off();
            }
            else
            {
                TMR_StartSingleShotTimer(mPruTimeoutTimerId,
                                         TmrMilliseconds(gPruOvpOutputDisableTimeout_c),
                                         BleApp_TimeoutCallback,
                                         NULL);
            }
            mAppState = gWptPruStateSystemError_c;
            mPruAlertCount = gPruAlertTime_c / gPruMonitorTime_c;
            
            BleApp_PrintPruState(gWptPruStateSystemError_c);
            break;
        default:
            mAppState = gWptPruStateOn_c;
            break;
        }
        break;
    case gWptPruStateSystemError_c:
        if(gPruEventErrorOccured_c == event)
        {
            /* if in system error start sending alerts each gPruAlertTime_c ms*/
            if(0 == --mPruAlertCount)
            {
                mPruAlertCount = gPruAlertTime_c / gPruMonitorTime_c;
                Wpt_RecordPruAlertAndNotifyIfCccdWritten(&wptPruAlertChar);
            }
        }
        if(gPruEventErrorCorrected_c == event)
        {
            mVRect = (uint16_t)gVRectBoot_mV_c;
            wptPruDynamicChar.pruAlert.fields.pruChargePort = FALSE;
            if(BleApp_CheckOvp())
            {
                TMR_StopTimer(mPruTimeoutTimerId);
            }
            mAppState = gWptPruStateBoot_c;
            BleApp_PrintPruState(mAppState);
        }
        else if(gPruEventPowerRemoved_c == event)
        {
            mVRect = (uint16_t)gVRectUvlo_mV_c;
            StopLed2Flashing();
            Led2Off();
            
            mAppState = gWptPruStateNull_c;
            BleApp_PrintPruState(mAppState);
        }
        else if(gPruEventPowerDenied_c == event) /* self protection */
        {
            Led2Off();
            mVRect = (uint16_t)gVRectBoot_mV_c;
            wptPruDynamicChar.pruAlert.fields.pruChargePort = FALSE;
            
            BleApp_PrintPruStatus(&wptPruDynamicChar.pruAlert);
        }
        if(gPruEventErrorCorrected_c == event ||
           gPruEventPowerRemoved_c   == event)
        {
            BleApp_PrintPruState(mAppState);
        }
        break;
    case gWptPruStateMaxState_c:
    default:
        mAppState = gWptPruStateBoot_c;
        break;
    }
}

/*! *********************************************************************************
* \brief      Timeout if no connection from PTU
*
********************************************************************************** */
static void BleApp_TimeoutCallback(void* param)
{
    /* this means advertising timed out */
    if(mAdvState.advOn)
    {
        Gap_StopAdvertising();
        BleApp_WptPruStateMachineHandler(gPruEventDummyEvent_c);
    }
    else
    {
        /* over voltage protection */
        BleApp_WptPruStateMachineHandler(gPruEventPowerDenied_c);
    }
}
/*! *********************************************************************************
* \brief      Simulates Dynamic Parameters Updates and PRU Alert Checks
*
********************************************************************************** */
static void BleApp_MonitorPRUDeviceCallback(void* param)
{ 
    uint32_t randomNo = 0;
    uint16_t pRect_mW = 0;
    
    /* update alert fields*/
    BleApp_UpdatePruAlert(&wptPruAlertChar);
    
    /* signal error occured */
    if(wptPruAlertChar.pruAlert.fields.pruOverCurrent ||
       wptPruAlertChar.pruAlert.fields.pruOverTemperature ||
           wptPruAlertChar.pruAlert.fields.pruOverVoltage ||
               wptPruAlertChar.pruAlert.fields.pruSelfProtection)
    {
        BleApp_WptPruStateMachineHandler(gPruEventErrorOccured_c);
    }
    /* optional fields include only temperature, Iout, Vout */
    wptPruDynamicChar.optFieldsValidity.rawOptionalFields = 0;
    wptPruDynamicChar.optFieldsValidity.fields.temperature = 1;
    wptPruDynamicChar.optFieldsValidity.fields.iOut = 1;
    wptPruDynamicChar.optFieldsValidity.fields.vOut = 1;
    /* copy latest alert parameters without last two bits which have different meaning*/
    wptPruDynamicChar.pruAlert.rawPruAlert |= (wptPruAlertChar.pruAlert.rawPruAlert & 0xFC);
    wptPruDynamicChar.temperature_degC = (uint8_t)(BOARD_GetTemperature() + 
                                                   gPruTemperatureOffset_c);
#if gAdjustPowerCommandSupport_d
    if(mPowerSharingInProgress)
    {
        if(TMR_GetTimestamp() - mPowerSharingTimestamp >= 2000000)
        {
            Led3Off();
            mPowerSharingInProgress = FALSE;
            wptPruDynamicChar.pruAlert.fields.adjPowerResponse = 0;
        }
    }
#endif
    
    if(mVRect >= (uint16_t)gVRectMin_mV_c &&
       mVRect <= (uint16_t)gVRectMax_mV_c)
    {
        RNG_GetRandomNo(&randomNo);
#if gAdjustPowerCommandSupport_d
        if(gAdjustPowerCommandMaxPower_c == mPwrAdjustCommandType)
        {
        
#endif
        /* randomize VRECT */
        mVRect = (uint16_t)gVRectSet_mV_c + 
            ((int8_t)(randomNo & 0xFF));
        /* random Prect in range PRECT_MAX - 1W -> PRECT_MAX */
        pRect_mW = (wptPruStaticChar.PRectMax_100mW * 100) - 
            (uint16_t)((randomNo >> 8) % 1000);
        
        wptPruDynamicChar.vRect = mVRect;
#if gAdjustPowerCommandSupport_d
        }
        else
        {
            /* randomize VRECT */
            mVRect = (uint16_t)gVRectMin_mV_c + 
                ((uint8_t)(randomNo & 0xFF));
            switch (mPwrAdjustCommandType)
            {
            case gAdjustPowerCommand66Percent_c:
                pRect_mW = ((wptPruStaticChar.PRectMax_100mW * 100)*2/3) - 
                    (uint16_t)((randomNo >> 8) % 1000);
                break;
            case gAdjustPowerCommand33Percent_c:
                pRect_mW = ((wptPruStaticChar.PRectMax_100mW * 100)/3) - 
                    (uint16_t)((randomNo >> 8) % 500);
                break;
            case gAdjustPowerCommand2_5Watts_c:
                pRect_mW = 2500 - (uint8_t)((randomNo >> 8) % 100);
                break;
            }
        }
#endif
        wptPruDynamicChar.vRect = mVRect;
        /* IRECT is PRECT / VRECT */
        wptPruDynamicChar.iRect = pRect_mW * 1000 / mVRect;
        wptPruDynamicChar.vOut = BleApp_GetVOut();
        wptPruDynamicChar.iOut = BleApp_GetIOut();
    }
    else
    {
        wptPruDynamicChar.vRect = mVRect;
        wptPruDynamicChar.iRect = (gMinPowerDraw_mW_c * 1000) / mVRect;
        wptPruDynamicChar.vOut = 0;
        wptPruDynamicChar.iOut = 0;
    }
    Wpt_RecordPruDynamic(&wptPruDynamicChar);
    
}
/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    /* Set advertising parameters*/
    Gap_SetAdvertisingParameters(&gAdvParams);
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
            mAdvState.advOn = !mAdvState.advOn; 
            LED_StopFlashingAllLeds();

            if(mAdvState.advOn)
            {
                Led1Flashing();                
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            Led2On();
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
    /* Connection Manager to handle Host Stack interactions */
    BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            bool_t isBonded = FALSE ;
            
#if gAppUseBonding_d    
            if (gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded) &&
                TRUE == isBonded) 
            {

            }   
#endif            
            /* stop adv timeout timer */
            TMR_StopTimer(mPruTimeoutTimerId);
            /* Subscribe client*/
            mPeerDeviceId = peerDeviceId;
            Wpt_Subscribe(peerDeviceId);

            TMR_StartIntervalTimer(appTimerId, 
                                   TmrMilliseconds(gPruMonitorTime_c),
                                   BleApp_MonitorPRUDeviceCallback, 
                                   NULL);
            mAdvState.advOn = FALSE;
            LED_StopFlashingAllLeds();
            /* UI */
            Led1On();
        }
        break;
        /* if PTU disconnected because charge is complete
           press PB1 for more than 5 seconds to simulate power removed 
        *  else it means the link was lost so advertising will restart
        */
        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            mPeerDeviceId = gInvalidDeviceId_c;
            Wpt_Unsubscribe();
            TMR_StopTimer(appTimerId);
            /* PRU disconnected means power removed or idle connection */
            if(gHciConnectionTerminatedByLocalHost_c ==
               pConnectionEvent->eventData.disconnectedEvent.reason)
            {
                BleApp_WptPruStateMachineHandler(gPruEventDummyEvent_c);
            }
            else
            {
                BleApp_WptPruStateMachineHandler(gPruEventDisconnected_c);
            }

            /* UI */
            Led1Off();         
        }
        break;
        case gConnEvtEncryptionChanged_c:
        {

        }
        break;        
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
    uint16_t handle = 0xFF;
    wptPruControlCharacteristic_t* pPruControl;
    bool_t alreadyCharging = wptPruDynamicChar.pruAlert.fields.pruChargePort;
    switch (pServerEvent->eventType)
    {
    case gEvtAttributeWritten_c:
    case gEvtAttributeWrittenWithoutResponse_c:
        handle = pServerEvent->eventData.attributeWrittenEvent.handle;
        if(value_ptu_static == handle)
        {        
            Wpt_PruRecordPtuStaticParamCharacteristic(
                (wptPtuStaticParamCharacteristic_t*)(pServerEvent->eventData.attributeWrittenEvent.aValue));
        }
        else if(value_pru_control == handle)
        {
            pPruControl = (wptPruControlCharacteristic_t*)pServerEvent->eventData.attributeWrittenEvent.aValue;
            /* record characteristic value */
            Wpt_PruRecordPruControlCharacteristic(pPruControl);
            /* verify if Charging status should be set */
            if(pPruControl->enables.fields.enablePruOutput)
            {
                /* if device is already charging do not send event to sm */
                if(!alreadyCharging)
                {
                    BleApp_WptPruStateMachineHandler(gPruEventPowerAllowed_c);
                }
            }
            else if(gPermittedWithWaitingTime_c == 
                    (wptPruControlCharPermValues_t)pPruControl->permission)
            {
                BleApp_WptPruStateMachineHandler(gPruEventPowerDenied_c);
            }
            else
            {
                if(gWptPruStateSystemError_c == mAppState)
                {
                    /* simulate error corrected */
                    wptPruAlertChar.pruAlert.fields.pruOverCurrent = 0;
                    wptPruAlertChar.pruAlert.fields.pruOverVoltage = 0;
                    wptPruAlertChar.pruAlert.fields.pruOverTemperature = 0;
                    BleApp_WptPruStateMachineHandler(gPruEventErrorCorrected_c);
                }
                else if(TRUE == mChargeComplete)
                {
                    BleApp_WptPruStateMachineHandler(gPruEventChargeComplete_c);
                }
                else
                {
                    BleApp_WptPruStateMachineHandler(gPruEventPowerDenied_c);
                }
            }
#if gAdjustPowerCommandSupport_d
            if(wptPruStaticChar.pruInformation.fields.adjustPwrCapability)
            {
                if(gAdjustPowerCommandMaxPower_c != pPruControl->enables.fields.adjustPowerCommand &&
                   !mPowerSharingInProgress)
                {          
                    wptPruDynamicChar.pruAlert.fields.adjPowerResponse = 1;
                    mPwrAdjustCommandType = 
                        (adjustPowerCommandValues_t)pPruControl->enables.fields.adjustPowerCommand;
                    mPowerSharingTimestamp = TMR_GetTimestamp();
                    Led3On();
                    mPowerSharingInProgress = TRUE;
                }
            }
#endif
        }
        if(gEvtAttributeWritten_c == pServerEvent->eventType)
        {
            GattServer_SendAttributeWrittenStatus(mPeerDeviceId, handle, gAttErrCodeNoError_c);
        }
        break;
    default:
        panic(0,0,0,0);
        break;
    }
}

/*! *********************************************************************************
* \brief      Performs Alert update
*
********************************************************************************** */
static void BleApp_UpdatePruAlert(wptPruAlertCharacteristic_t* pPruAlertChar)
{

    wptPruAlertChar.pruAlert.fields.chargeComplete = mChargeComplete;

    /*check temperature*/
    wptPruAlertChar.pruAlert.fields.pruOverTemperature = 
        (uint8_t)(BOARD_GetTemperature() + gPruTemperatureOffset_c) > gTemperatureLimitForOtp_c;
    /* check over-voltage, over-current, self protection */
    wptPruAlertChar.pruAlert.fields.pruOverVoltage = BleApp_CheckOvp();
    wptPruAlertChar.pruAlert.fields.pruOverCurrent = 0;
    wptPruAlertChar.pruAlert.fields.pruSelfProtection = 0;
}

static void BleApp_PrintPruState(wptPruStates_t mState)
{
    Serial_Print(mAppSer, maDisplaySeparator, gAllowToBlock_d);
    Serial_Print(mAppSer, maDisplayPruState, gAllowToBlock_d);
    switch(mState)
    {
    case gWptPruStateNull_c:
        Serial_Print(mAppSer, "Null", gAllowToBlock_d);
        break;
    case gWptPruStateBoot_c:
        Serial_Print(mAppSer, "Boot", gAllowToBlock_d);
        break;
    case gWptPruStateOn_c:
        Serial_Print(mAppSer, "On", gAllowToBlock_d);
        break;
    case gWptPruStateSystemError_c:
        Serial_Print(mAppSer, "System Error", gAllowToBlock_d);
        break;
    default:
        Serial_Print(mAppSer, "undefined", gAllowToBlock_d);
        break;
    }
}
static void BleApp_PrintStaticParams(wptPruStaticParamCharacteristic_t* pPruStatic)
{
    Serial_Print(mAppSer, maDisplaySeparator, gAllowToBlock_d);
    Serial_Print(mAppSer, maDisplayStaticParams, gAllowToBlock_d);
    Serial_Print(mAppSer, "Vrmin: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruStatic->vRectMinStatic_mV);
    Serial_Print(mAppSer, ", Vrset: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruStatic->vRectSet_mV);
    Serial_Print(mAppSer, ", Vrmax: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruStatic->vRectHighStatic_mV);
    Serial_Print(mAppSer, ", Pmax: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruStatic->PRectMax_100mW*100);
    Serial_Print(mAppSer, ", Tmax: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, gTemperatureLimitForOtp_c);
}
static void BleApp_PrintDynamicParams(wptPruDynamicParamCharacteristic_t* pPruDynamic)
{
    Serial_Print(mAppSer, maDisplaySeparator, gAllowToBlock_d);
    Serial_Print(mAppSer, maDisplayDynamicParams, gAllowToBlock_d);
    Serial_Print(mAppSer, "Vrect: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruDynamic->vRect);
    Serial_Print(mAppSer, ", Irect: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruDynamic->iRect);
    Serial_Print(mAppSer, ", Vout: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruDynamic->vOut);
    Serial_Print(mAppSer, ", iOut: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)pPruDynamic->iOut);
    Serial_Print(mAppSer, ", Temp: ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)(pPruDynamic->temperature_degC - gPruTemperatureOffset_c));
}
static void BleApp_PrintPruStatus
(
 wptPruAlertCharDyn_t* pPruAlert
)
{
    char* const pTrue = "TRUE";
    char* const pFalse = "FALSE";
    
    Serial_Print(mAppSer, maDisplaySeparator, gAllowToBlock_d);
    Serial_Print(mAppSer, "Out Ena: ", gAllowToBlock_d);
    if(pPruAlert->fields.pruChargePort)
    {
        Serial_Print(mAppSer, pTrue, gAllowToBlock_d);
    }
    else
    {
        Serial_Print(mAppSer, pFalse, gAllowToBlock_d);
    }
    
    Serial_Print(mAppSer, ", Chg Complete: ", gAllowToBlock_d);
    if(pPruAlert->fields.chargeComplete)
    {
        Serial_Print(mAppSer, pTrue, gAllowToBlock_d);
    }
    else
    {
        Serial_Print(mAppSer, pFalse, gAllowToBlock_d);
    }
}
/*! *********************************************************************************
* @}
********************************************************************************** */
