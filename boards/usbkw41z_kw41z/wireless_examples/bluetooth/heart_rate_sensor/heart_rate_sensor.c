/*! *********************************************************************************
* \addtogroup Heart Rate Sensor
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
* This file is the source file for the Heart Rate Sensor application
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
#include "heart_rate_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "heart_rate_sensor.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mHeartRateLowerLimit_c          (40) /* Heart beat lower limit, 8-bit value */
#define mHeartRateUpperLimit_c          (201) /* Heart beat upper limit, 8-bit value */
#define mHeartRateRange_c               (mHeartRateUpperLimit_c - mHeartRateLowerLimit_c) /* Range = [ADC16_HB_LOWER_LIMIT .. ADC16_HB_LOWER_LIMIT + ADC16_HB_DYNAMIC_RANGE] */
#define mHeartRateReportInterval_c      (1)        /* heart rate report interval in seconds  */
#define mBatteryLevelReportInterval_c   (10)        /* battery level report interval in seconds  */
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
#if gAppUseBonding_d
    fastWhiteListAdvState_c,
#endif
    fastAdvState_c,
    slowAdvState_c
}advType_t;

typedef struct advState_tag{
    bool_t      advOn;
    advType_t   advType;
}advState_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Adv State */
static advState_t  mAdvState;
static bool_t      mRestartAdv;
static uint32_t    mAdvTimeout;
static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;

/* Service Data*/
static basConfig_t      basServiceConfig = {service_battery, 0};
static hrsUserData_t    hrsUserData;
static hrsConfig_t hrsServiceConfig = {service_heart_rate, TRUE, TRUE, TRUE, gHrs_BodySensorLocChest_c, &hrsUserData};
static uint16_t cpHandles[1] = { value_hr_ctrl_point };

/* Application specific data*/
static bool_t mToggle16BitHeartRate = FALSE;
static bool_t mContactStatus = TRUE;
static tmrTimerID_t mAdvTimerId;
static tmrTimerID_t mMeasurementTimerId;
static tmrTimerID_t mBatteryMeasurementTimerId;

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
static void AdvertisingTimerCallback (void *);
static void TimerMeasurementCallback (void *);
static void BatteryMeasurementTimerCallback (void *);

static void BleApp_Advertise(void);

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
    /* Device is not connected and not advertising*/
    if (!mAdvState.advOn)
    {
#if gAppUseBonding_d
        if (gcBondedDevices > 0)
        {
            mAdvState.advType = fastWhiteListAdvState_c;
        }
        else
        {
#endif
            mAdvState.advType = fastAdvState_c;
#if gAppUseBonding_d
        }
#endif
        BleApp_Advertise();
    }
    
#if (cPWR_UsePowerDownMode)    
    PWR_ChangeDeepSleepMode(1); /* MCU=LLS3, LL=DSM, wakeup on GPIO/LL */
    PWR_AllowDeviceToSleep();
#endif       
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
            if (mPeerDeviceId == gInvalidDeviceId_c)
            {
                BleApp_Start();
            }
            break;
        }
        case gKBD_EventPressPB2_c:
        {
            mToggle16BitHeartRate = (mToggle16BitHeartRate)?FALSE:TRUE;
        }
        break;
        case gKBD_EventLongPB1_c:
        {
            if (mPeerDeviceId != gInvalidDeviceId_c)
            {
                Gap_Disconnect(mPeerDeviceId);
            }
            break;
        }
        case gKBD_EventLongPB2_c:
        {
            mContactStatus = mContactStatus?FALSE:TRUE;
            Hrs_SetContactStatus(service_heart_rate, mContactStatus);
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
    GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpHandles), cpHandles);
    App_RegisterGattServerCallback(BleApp_GattServerCallback);

    mAdvState.advOn = FALSE;

    /* Start services */
    hrsServiceConfig.sensorContactDetected = mContactStatus;
#if gHrs_EnableRRIntervalMeasurements_d    
    hrsServiceConfig.pUserData->pStoredRrIntervals = MEM_BufferAlloc(sizeof(uint16_t) * gHrs_NumOfRRIntervalsRecorded_c);
#endif    
    Hrs_Start(&hrsServiceConfig);
    
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_Start(&basServiceConfig);
    
    /* Allocate application timers */
    mAdvTimerId = TMR_AllocateTimer();
    mMeasurementTimerId = TMR_AllocateTimer();
    mBatteryMeasurementTimerId = TMR_AllocateTimer();

#if (cPWR_UsePowerDownMode)    
    PWR_ChangeDeepSleepMode(3); /* MCU=LLS3, LL=IDLE, wakeup on GPIO/LL */
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
    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case fastWhiteListAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessWhiteListOnly_c;
            mAdvTimeout = gFastConnWhiteListAdvTime_c;
        }
        break;
#endif
        case fastAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            mAdvTimeout = gFastConnAdvTime_c - gFastConnWhiteListAdvTime_c;
        }
        break;

        case slowAdvState_c:
        {
            gAdvParams.minInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.maxInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            mAdvTimeout = gReducedPowerAdvTime_c;
        }
        break;
    }

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
            
            if (!mAdvState.advOn && mRestartAdv)
            {
                BleApp_Advertise();
                break;
            }                

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
                PWR_ChangeDeepSleepMode(1);
                /* Start advertising timer */
                TMR_StartLowPowerTimer(mAdvTimerId,gTmrLowPowerSecondTimer_c,
                         TmrSeconds(mAdvTimeout), AdvertisingTimerCallback, NULL);             
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
            else
            {
                TMR_StartLowPowerTimer(mAdvTimerId,gTmrLowPowerSecondTimer_c,
                        TmrSeconds(mAdvTimeout), AdvertisingTimerCallback, NULL);  
            }
#endif 
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
	/* Connection Manager to handle Host Stack interactions */
	BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            mPeerDeviceId = peerDeviceId;

            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;            
        
            /* Subscribe client*/
            Bas_Subscribe(peerDeviceId);        
            Hrs_Subscribe(peerDeviceId);
                                    
#if (!cPWR_UsePowerDownMode)  
            /* UI */            
            LED_StopFlashingAllLeds();
            Led1On();            
#endif            
            
            /* Stop Advertising Timer*/
            mAdvState.advOn = FALSE;
            TMR_StopTimer(mAdvTimerId);
            
            /* Start measurements */
            TMR_StartLowPowerTimer(mMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mHeartRateReportInterval_c), TimerMeasurementCallback, NULL);

            /* Start battery measurements */
            TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);            

#if (cPWR_UsePowerDownMode)
    PWR_SetDeepSleepTimeInMs(900);
    PWR_ChangeDeepSleepMode(1);
    PWR_AllowDeviceToSleep();    
#endif
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            Bas_Unsubscribe();
            Hrs_Unsubscribe();

            mPeerDeviceId = gInvalidDeviceId_c;
            
            TMR_StopTimer(mMeasurementTimerId);
            TMR_StopTimer(mBatteryMeasurementTimerId);            

#if (cPWR_UsePowerDownMode)            
            /* UI */
            Led1Off();
            
            /* Go to sleep */
            PWR_ChangeDeepSleepMode(3); /* MCU=LLS3, LL=IDLE, wakeup on swithes/LL */
            PWR_SetDeepSleepTimeInMs(cPWR_DeepSleepDurationMs);
            PWR_AllowDeviceToSleep();
#else
            if (pConnectionEvent->eventData.disconnectedEvent.reason == gHciConnectionTimeout_c)
            {
                /* Link loss detected*/
                BleApp_Start();
            }
            else
            {
              /* Connection was terminated by peer or application */
                BleApp_Start();
            }
#endif			
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
    uint16_t handle;
    uint8_t status;
    
    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWritten_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            status = gAttErrCodeNoError_c;
            
            if (handle == value_hr_ctrl_point)
            {
                status = Hrs_ControlPointHandler(&hrsUserData, pServerEvent->eventData.attributeWrittenEvent.aValue[0]);
            }
            
            GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
        }
        break;
    default:
        break;
    }
}


/*! *********************************************************************************
* \brief        Handles advertising timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void AdvertisingTimerCallback(void * pParam)
{
    /* Stop and restart advertising with new parameters */
    Gap_StopAdvertising();

    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case fastWhiteListAdvState_c:
        {
            mAdvState.advType = fastAdvState_c;
            mRestartAdv = TRUE;
        }
        break;
#endif
        case fastAdvState_c:
        {
            mAdvState.advType = slowAdvState_c;
            mRestartAdv = TRUE;
        }
        break;

        default:
        {
            mRestartAdv = FALSE;
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles measurement timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void TimerMeasurementCallback(void * pParam)
{
    uint16_t hr = BOARD_GetPotentiometerLevel();
    hr = (hr * mHeartRateRange_c) >> 12;

#if gHrs_EnableRRIntervalMeasurements_d    
    Hrs_RecordRRInterval(&hrsUserData, (hr & 0x0F));
    Hrs_RecordRRInterval(&hrsUserData,(hr & 0xF0));
#endif
    
    if (mToggle16BitHeartRate)
    {
        Hrs_RecordHeartRateMeasurement(service_heart_rate, 0x0100 + (hr & 0xFF), &hrsUserData);
    }
    else
    {
        Hrs_RecordHeartRateMeasurement(service_heart_rate, mHeartRateLowerLimit_c + hr, &hrsUserData); 
    }
        
    Hrs_AddExpendedEnergy(&hrsUserData, 100);
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
* @}
********************************************************************************** */
