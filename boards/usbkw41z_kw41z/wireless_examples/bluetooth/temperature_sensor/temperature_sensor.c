/*! *********************************************************************************
* \addtogroup Temperature Sensor
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
#include "Panic.h"


#if (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "temperature_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "temperature_sensor.h"

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
typedef struct advState_tag{
    bool_t      advOn;
}advState_t;
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Adv State */
static advState_t  mAdvState;

static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;

/* Service Data*/
static basConfig_t      basServiceConfig = {service_battery, 0};
static disConfig_t      disServiceConfig = {service_device_info};

static tmsConfig_t tmsServiceConfig = {service_temperature, 0};

/* Application specific data*/
static tmrTimerID_t appTimerId;

static bool_t   mSendDataAfterEncStart = FALSE;
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

/* Timer Callbacks */
#if cPWR_UsePowerDownMode
static void AdvertisingTimerCallback (void *);
static void DisconnectTimerCallback(void* );
#endif

static void BleApp_Advertise(void);
static void BleApp_SendTemperature(void);

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
    Led1On();
    
    if (mPeerDeviceId == gInvalidDeviceId_c)
    {
        /* Device is not connected and not advertising*/
        if (!mAdvState.advOn)
        {
            BleApp_Advertise();
        }
        
#if (cPWR_UsePowerDownMode)    
        PWR_ChangeDeepSleepMode(1);
        PWR_AllowDeviceToSleep();    
#endif           
    }
    else
    {
        BleApp_SendTemperature();
    }
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    BleApp_Start();
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

    /* Start services */
    tmsServiceConfig.initialTemperature = 100 * BOARD_GetTemperature();
    Tms_Start(&tmsServiceConfig);
    
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_Start(&basServiceConfig);
    Dis_Start(&disServiceConfig);

    /* Allocate aplication timer */
    appTimerId = TMR_AllocateTimer();
    
#if (cPWR_UsePowerDownMode)    
    PWR_ChangeDeepSleepMode(3);
    PWR_AllowDeviceToSleep();    
#endif    
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will satrt after
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

#if (cPWR_UsePowerDownMode)
            if(!mAdvState.advOn)
            {
                Led1Off();
                PWR_ChangeDeepSleepMode(3);
                PWR_SetDeepSleepTimeInMs(cPWR_DeepSleepDurationMs);
                PWR_AllowDeviceToSleep();    
            }  
            else
            {
                /* Start advertising timer */
                TMR_StartLowPowerTimer(appTimerId, 
                           gTmrLowPowerSecondTimer_c,
                           TmrSeconds(gAdvTime_c),
                           AdvertisingTimerCallback, NULL);

                Led1On();
            }
#else    
            LED_StopFlashingAllLeds();
            Led1Flashing();

            if(!mAdvState.advOn)
            {
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
            }
#endif  
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
            
            mSendDataAfterEncStart = FALSE;
            if (gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded) &&
                TRUE == isBonded) 
            {
                /* Send temperature data after encryption is started */
                mSendDataAfterEncStart = TRUE;
            }   
#endif  
            
            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;
            TMR_StopTimer(appTimerId);         
            
            /* Subscribe client*/
            mPeerDeviceId = peerDeviceId;
            Bas_Subscribe(peerDeviceId);        
            Tms_Subscribe(peerDeviceId);

            /* UI */
            Led1On();
            
#if (cPWR_UsePowerDownMode)             
            PWR_ChangeDeepSleepMode(1);
            PWR_AllowDeviceToSleep();                
#else
            LED_StopFlashingAllLeds();
            
#endif
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            mPeerDeviceId = gInvalidDeviceId_c;
            Bas_Unsubscribe();
            Tms_Unsubscribe();
            
#if (cPWR_UsePowerDownMode)
            /* UI */
            Led1Off();
            
            /* Go to sleep */
            PWR_ChangeDeepSleepMode(3);
            PWR_SetDeepSleepTimeInMs(cPWR_DeepSleepDurationMs);
            PWR_AllowDeviceToSleep();
#else
            /* restart advertising*/
            BleApp_Start();
#endif             
        }
        break;
        case gConnEvtEncryptionChanged_c:
        {
            if (mSendDataAfterEncStart)
            {
                BleApp_SendTemperature();
            }
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
    switch (pServerEvent->eventType)
    {
        case gEvtCharacteristicCccdWritten_c:
        {
            BleApp_SendTemperature();
        }
        break;
    default:
        break;
    }
}

#if cPWR_UsePowerDownMode
/*! *********************************************************************************
* \brief        Handles advertising timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void AdvertisingTimerCallback(void* pParam)
{
    /* Stop advertising */
    if (mAdvState.advOn)
    {
        Gap_StopAdvertising();
    }
}
#endif

#if cPWR_UsePowerDownMode
/*! *********************************************************************************
* \brief        Handles disconnect timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void DisconnectTimerCallback(void* pParam)
{
    if (mPeerDeviceId != gInvalidDeviceId_c)
    {
        Gap_Disconnect(mPeerDeviceId);
    }
}
#endif
/*! *********************************************************************************
* \brief        Sends temperature value over-the-air.
*
********************************************************************************** */
static void BleApp_SendTemperature(void)
{
    TMR_StopTimer(appTimerId);
    
    /* Update with initial temperature */
    Tms_RecordTemperatureMeasurement(service_temperature, 
                                     BOARD_GetTemperature() * 100);  
    
#if (cPWR_UsePowerDownMode)    
        
    /* Start Sleep After Data timer */
    TMR_StartLowPowerTimer(appTimerId, 
                           gTmrLowPowerSecondTimer_c,
                           TmrSeconds(gGoToSleepAfterDataTime_c),
                           DisconnectTimerCallback, NULL);    
    
    /* Go to sleep */
    PWR_SetDeepSleepTimeInMs(TmrSeconds(gGoToSleepAfterDataTime_c));
#endif    
        
}

/*! *********************************************************************************
* @}
********************************************************************************** */
