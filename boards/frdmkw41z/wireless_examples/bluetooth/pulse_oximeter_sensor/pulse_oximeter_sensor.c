/*! *********************************************************************************
* \addtogroup Pulse Oximeter Sensor
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
* This file is the source file for the Pulse Oximeter Sensor application
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

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "current_time_interface.h"
#include "pulse_oximeter_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "pulse_oximeter_sensor.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

#define mSpO2PRReportInterval_c         (1000)          /* SpO2 and PR report interval in miliseconds  */
#define mBatteryLevelReportInterval_c   (10)            /* battery level report interval in seconds  */
#define mReadTimeCharInterval_c         (3600U)         /* interval between time sync in seconds */
#define mCharReadBufferLength_c         (13U)           /* length of the buffer */
#define mInitialTime_c                  (1451606400U)   /* initial timestamp - 01/01/2016 00:00:00 GMT */
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
static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;

static plxUserData_t    mUserData;
static basConfig_t      basServiceConfig = {service_battery, 0};
static disConfig_t      disServiceConfig = {service_device_info};
static plxConfig_t      plxServiceConfig = {service_pulse_oximeter, 
                                            { gPlx_DefaultSupportedFeatures,
                                              gPlx_DefaultMeasStatusSupport,
                                              {
                                                gPlx_DefaultDevAndSensStatusSupport
                                              }
                                            },
                                            &mUserData,
                                            0
                                           };

static uint16_t cpHandles[] = { value_ra_ctrl_point };

/* Application specific data*/
static tmrTimerID_t mAdvTimerId;
static tmrTimerID_t mMeasurementTimerId;
static tmrTimerID_t mBatteryMeasurementTimerId;
static tmrTimerID_t mLocalTimeTickTimerId;
#if gAppUseTimeService_d
static tmrTimerID_t mReadTimeCharTimerId;
#endif

#if gAppUseTimeService_d
static uint8_t mOutCharReadBuffer[mCharReadBufferLength_c];
static uint16_t mOutCharReadByteCount;
static bool_t isTimeSynchronized = FALSE;
#endif

static uint32_t localTime = mInitialTime_c;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
#if gAppUseTimeService_d
static void BleApp_GattClientCallback (deviceId_t serverDeviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error);
#endif
static void BleApp_Config();

/* Timer Callbacks */
static void AdvertisingTimerCallback (void *);
static void TimerMeasurementCallback (void *);
static void BatteryMeasurementTimerCallback (void *);
static void LocalTimeTickTimerCallback (void *);
#if gAppUseTimeService_d
static void ReadTimeCharTimerCallback (void *);
#endif

static void BleApp_TakeMeasurement(void);
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
    if (mPeerDeviceId == gInvalidDeviceId_c)
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
        case gKBD_EventPressPB2_c:
        {
            /* toggle new measurement */
            BleApp_TakeMeasurement();
            break;
        }
        case gKBD_EventLongPB1_c:
        {
            if (mPeerDeviceId != gInvalidDeviceId_c)
                Gap_Disconnect(mPeerDeviceId);
            break;
        }
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
    GattServer_RegisterHandlesForWriteNotifications(sizeof(cpHandles), cpHandles);    
    
#if gAppUseTimeService_d
    App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
#endif
    
    mAdvState.advOn = FALSE;

    /* Start services */
    mUserData.cMeasurements = 0;
    mUserData.pStoredMeasurements = MEM_BufferAlloc(gPlx_MaxNumOfStoredMeasurements_c * sizeof(plxSpotCheckMeasurement_t));
    mUserData.cReportedRecords = 0;
    Plx_Start(&plxServiceConfig);
    
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_Start(&basServiceConfig);
    Dis_Start(&disServiceConfig);
    
    /* Allocate application timers */
    mAdvTimerId = TMR_AllocateTimer();
    mMeasurementTimerId = TMR_AllocateTimer();
    mBatteryMeasurementTimerId = TMR_AllocateTimer();
    mLocalTimeTickTimerId = TMR_AllocateTimer();
    
    TMR_StartLowPowerTimer(mLocalTimeTickTimerId, gTmrLowPowerIntervalMillisTimer_c,
               TmrSeconds(1), LocalTimeTickTimerCallback, NULL);
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will satrt after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    uint32_t timeout = 0;

    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case fastWhiteListAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessWhiteListOnly_c;
            timeout = gFastConnWhiteListAdvTime_c;
        }
        break;
#endif
        case fastAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            timeout = gFastConnAdvTime_c - gFastConnWhiteListAdvTime_c;
        }
        break;

        case slowAdvState_c:
        {
            gAdvParams.minInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.maxInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            timeout = gReducedPowerAdvTime_c;
        }
        break;
    }

    /* Set advertising parameters*/
    Gap_SetAdvertisingParameters(&gAdvParams);

    /* Start advertising timer */
    TMR_StartLowPowerTimer(mAdvTimerId,gTmrLowPowerSecondTimer_c,
               TmrSeconds(timeout), AdvertisingTimerCallback, NULL);
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
            Led1Flashing();

            if(!mAdvState.advOn)
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
    /* Connection Manager to handle Host Stack interactions */
    BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            mPeerDeviceId = peerDeviceId;
#if gAppUseTimeService_d
            bool_t isBonded = FALSE;

            Gap_CheckIfBonded(mPeerDeviceId, &isBonded);
            if(isBonded == FALSE)
            {
                if(isTimeSynchronized == FALSE)
                {
                    bleUuid_t uuid = { .uuid16 = gBleSig_CurrentTime_d };
                    
                  /* Read CTS Characteristic. If the device doesn't have time services
                   gAttErrCodeAttributeNotFound_c will be received. */
                    GattClient_ReadUsingCharacteristicUuid
                    (
                        peerDeviceId,
                        gBleUuidType16_c,
                        &uuid,
                        NULL,
                        mOutCharReadBuffer,
                        13,
                        &mOutCharReadByteCount
                    );
                }
            }
#endif
            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;            
        
            /* Subscribe client*/
            Bas_Subscribe(peerDeviceId);        
            Plx_Subscribe(peerDeviceId);

            /* UI */
            LED_StopFlashingAllLeds();
            Led1On();
            
            /* Stop Advertising Timer*/
            TMR_StopTimer(mAdvTimerId);

            
            /* Start measurements */
            TMR_StartLowPowerTimer(mMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(1), TimerMeasurementCallback, NULL);

            /* Start battery measurements */
            TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);
#if gAppUseTimeService_d
            /* Start time readings */
            TMR_StartLowPowerTimer(mReadTimeCharTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mReadTimeCharInterval_c), ReadTimeCharTimerCallback, NULL);
#endif
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            Bas_Unsubscribe();
            Plx_Unsubscribe(&plxServiceConfig);

            mPeerDeviceId = gInvalidDeviceId_c;

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
#if gAppUseTimeService_d
            TMR_StopTimer(mReadTimeCharTimerId);
            isTimeSynchronized = FALSE;
#endif
        }
        break;
        
        case gConnEvtEncryptionChanged_c:
        {
#if gAppUseTimeService_d
            if(isTimeSynchronized == FALSE)
              {
                  bleUuid_t uuid = { .uuid16 = gBleSig_CurrentTime_d };
                  
                  /* Read CTS Characteristic. If the device doesn't have time services
                   gAttErrCodeAttributeNotFound_c will be received. */
                  GattClient_ReadUsingCharacteristicUuid
                  (
                      peerDeviceId,
                      gBleUuidType16_c,
                      &uuid,
                      NULL,
                      mOutCharReadBuffer,
                      13,
                      &mOutCharReadByteCount
                  );
              }
#endif
        }
        break;
        
        case gConnEvtPairingComplete_c:
        {
#if gAppUseTimeService_d
            if(isTimeSynchronized == FALSE)
            {
                bleUuid_t uuid = { .uuid16 = gBleSig_CurrentTime_d };

                /* Read CTS Characteristic. If the device doesn't have time services
                 gAttErrCodeAttributeNotFound_c will be received. */
                GattClient_ReadUsingCharacteristicUuid
                (
                    peerDeviceId,
                    gBleUuidType16_c,
                    &uuid,
                    NULL,
                    mOutCharReadBuffer,
                    13,
                    &mOutCharReadByteCount
                );
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
            
            if (handle == value_ra_ctrl_point)
            {
                Plx_ControlPointHandler(&plxServiceConfig, &pServerEvent->eventData.attributeWrittenEvent);
            }
            else
            {
                GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
            }
            break;
        }
        case gEvtHandleValueConfirmation_c:
        {
            if (plxServiceConfig.procInProgress == gProcSendingRecords)
            {
                if(mUserData.cReportedRecords == mUserData.cMeasurements)
                {
                    plxServiceConfig.procInProgress = gProcWaitingForConfirm;
                }
            }
            else
            {
                plxServiceConfig.procInProgress = gProcNotInProgress;
            }
            break;
        }
        case gEvtError_c:
        {
            if (pServerEvent->eventData.procedureError.error == gGattClientConfirmationTimeout_c)
            {
                /* Procedure timeout at ATT */
                plxServiceConfig.procInProgress = gProcNotInProgress;
            }
            break;
        }
        default:
            break;
    }
}

/*! *********************************************************************************
* \brief        Handles GATT client callback from host stack.
*
* \param[in]    serverDeviceId          GATT Server device ID.
* \param[in]    procedureType    	Procedure type.
* \param[in]    procedureResult    	Procedure result.
* \param[in]    error    		Callback result.
********************************************************************************** */
#if gAppUseTimeService_d
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
            //handle auth. errors
        }
        
        else
        {
            //characteristic not found
        }
    }
    else if (procedureResult == gGattProcSuccess_c)
    {        
        switch(procedureType)
        {
            case gGattProcReadUsingCharacteristicUuid_c:
            {
                if (mOutCharReadByteCount > 2)
                {
                    ctsDayDateTime_t time;
                    uint16_t charValueLength = mOutCharReadByteCount - 3;
                    uint16_t handle = Utils_ExtractTwoByteValue(mOutCharReadBuffer+1);
                    uint8_t* pValue = &mOutCharReadBuffer[3];

                    time.dateTime.year          = Utils_ExtractTwoByteValue(&pValue[0]);
                    time.dateTime.month         = pValue[2];
                    time.dateTime.day           = pValue[3];
                    time.dateTime.hours         = pValue[4];
                    time.dateTime.minutes       = pValue[5];
                    time.dateTime.seconds       = pValue[6];
                    time.dayOfWeek              = pValue[7];
                    
                    localTime = Cts_DayDateTimeToEpochTime(time);
                    
                    isTimeSynchronized = TRUE;
                }
            }
            break;
            
            default:
                break;
            }  
    }
}
#endif

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
        }
        break;
#endif
        case fastAdvState_c:
        {
            mAdvState.advType = slowAdvState_c;
        }
        break;

        default:
        break;
    }
    BleApp_Advertise();
}

/*! *********************************************************************************
* \brief        Handles measurement timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void TimerMeasurementCallback(void * pParam)
{
    uint32_t random = 0;
    plxContinuousMeasurement_t measurement;

    measurement.flags = gPlx_SpO2PRFastPresent_c                |\
                        gPlx_SpO2PRSlowPresent_c                |\
                        gPlx_MeasurementStatusPresent_c         |\
                        gPlx_DeviceAndSensorStatusPresent_c     |\
                        gPlx_PulseAmplitudeIndexPresent_c;
    
    RNG_GetRandomNo(&random);
    measurement.SpO2PRNormal.SpO2       = 93 + (random & 0x07);
    measurement.SpO2PRNormal.PulseRate  = 60 + (random & 0x3F);
    
    RNG_GetRandomNo(&random);
    measurement.SpO2PRFast.SpO2         = 93 + (random & 0x07);
    measurement.SpO2PRFast.PulseRate    = 60 + (random & 0x3F);
    
    RNG_GetRandomNo(&random);
    measurement.SpO2PRSlow.SpO2         = 93 + (random & 0x07);
    measurement.SpO2PRSlow.PulseRate    = 60 + (random & 0x3F);
    
    measurement.deviceAndSensorStatus.flags     = gPlx_PoorSignalDetected;
    measurement.deviceAndSensorStatus.rfu       = 0x00;
    
    measurement.measurementStatus       = gPlx_MeasurementOngoing |\
                                          gPlx_ValidatedData;
    
    RNG_GetRandomNo(&random);
    measurement.pulseAmplitudeIndex     = (random & 0x20);
    
    Plx_RecordContinuousMeasurement(service_pulse_oximeter, &measurement);
}

static void BleApp_TakeMeasurement(void)
{
    uint32_t random = 0;
    plxSpotCheckMeasurement_t measurement;
    ctsDayDateTime_t time;

    measurement.flags = ( gPlx_SCTimestampPresent_c             |\
                        gPlx_SCMeasurementStatusPresent_c       |\
                        gPlx_SCDeviceAndSensorStatusPresent_c   |\
                        gPlx_SCPulseAmplitudeIndexPresent_c     |\
                        gPlx_SCDeviceClockNotSet );
    
    RNG_GetRandomNo(&random);
    measurement.SpO2PRNormal.SpO2       = 93 + (random & 0x07);
    measurement.SpO2PRNormal.PulseRate  = 60 + (random & 0x3F);
    
    time = Cts_EpochToDayDateTime(localTime);
    FLib_MemCpy(&measurement.timeStamp, &time.dateTime, sizeof(ctsDateTime_t));
    
    measurement.measurementStatus       = gPlx_MeasurementOngoing |\
                                          gPlx_ValidatedData;
    
    measurement.deviceAndSensorStatus.flags     = gPlx_PoorSignalDetected;
    measurement.deviceAndSensorStatus.rfu       = 0x00;
    
    RNG_GetRandomNo(&random);
    measurement.pulseAmplitudeIndex     = (random & 0x1F);
    
    measurement.seqNumber = (plxServiceConfig.pUserData->lastSeqNumber)++;
    
    Plx_RecordSpotCheckMeasurement(&plxServiceConfig, &measurement);
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

static void LocalTimeTickTimerCallback (void * pParam)
{
    localTime++;
}

#if gAppUseTimeService_d
static void ReadTimeCharTimerCallback (void * pParam)
{
    bleUuid_t uuid = { .uuid16 = gBleSig_CurrentTime_d };

    /* Read CTS Characteristic. If the device doesn't have time services
     gAttErrCodeAttributeNotFound_c will be received. */
    GattClient_ReadUsingCharacteristicUuid
    (
        mPeerDeviceId,
        gBleUuidType16_c,
        &uuid,
        NULL,
        mOutCharReadBuffer,
        13,
        &mOutCharReadByteCount
    );
}
#endif

/*! *********************************************************************************
* @}
********************************************************************************** */
