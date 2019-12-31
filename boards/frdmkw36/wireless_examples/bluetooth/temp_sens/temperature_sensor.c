/*! *********************************************************************************
* \addtogroup Temperature Sensor
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Temperature Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "Panic.h"
#include "SerialManager.h"

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#if !defined(MULTICORE_APPLICATION_CORE) || (!MULTICORE_APPLICATION_CORE)
#include "gatt_db_handles.h"
#endif

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "temperature_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "temperature_sensor.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
#include "erpc_host.h"
#include "dynamic_gatt_database.h"
#endif

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
static bool_t           basValidClientList[gAppMaxConnections_c] = {FALSE};
static basConfig_t      basServiceConfig = {(uint16_t)service_battery, 0, basValidClientList, gAppMaxConnections_c};
static disConfig_t      disServiceConfig = {(uint16_t)service_device_info};

static tmsConfig_t tmsServiceConfig = {(uint16_t)service_temperature, 0};

/* Application specific data*/
static tmrTimerID_t appTimerId;

static uint8_t gAppSerMgrIf;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
static void BleApp_Config(void);

/* Timer Callbacks */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static void AdvertisingTimerCallback(void *pParam);
static void DisconnectTimerCallback(void *pParam);
#endif

static void BleApp_Advertise(void);
static void BleApp_SendTemperature(void);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static void SleepTimeoutSequence(void);
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
    /* Initialize application support for drivers */
    BOARD_InitAdc();

#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    Serial_InitManager();
#else
    SerialManager_Init();
#endif
    Serial_InitInterface(&gAppSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate(gAppSerMgrIf, APP_SERIAL_INTERFACE_SPEED);

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
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
    Led1On();

    if (mPeerDeviceId == gInvalidDeviceId_c)
    {
        /* Device is not connected and not advertising */
        if (!mAdvState.advOn)
        {
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    #if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
        #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
            (void)PWR_ChangeBlackBoxDeepSleepMode(gAppDeepSleepMode_c);
        #endif
    #else
        (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);
    #endif
#endif
            /* Set advertising parameters, advertising to start on gAdvertisingParametersSetupComplete_c */
            BleApp_Advertise();
        }
    }
    else
    {
        /* Device is connected, send temperature value */
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
    /* Start application on any key */
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
            /* Configure application and start services */
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
            /* Start advertising if data and parameters were successfully set */
            (void)App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;

        case gAdvertisingSetupFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
        {
            ; /* No action required */
        }
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
static void BleApp_Config(void)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
    if (GattDbDynamic_CreateDatabase() != gBleSuccess_c)
    {
        panic(0,0,0,0);
        return;
    }
#endif /* MULTICORE_APPLICATION_CORE */

    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();

    /* Register for callbacks*/
    (void)App_RegisterGattServerCallback(BleApp_GattServerCallback);

    mAdvState.advOn = FALSE;

    /* Start services */
    tmsServiceConfig.initialTemperature = (int16_t)(100 * BOARD_GetTemperature());
    (void)Tms_Start(&tmsServiceConfig);

    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_Start(&basServiceConfig);
    (void)Dis_Start(&disServiceConfig);

    /* Allocate application timer */
    appTimerId = TMR_AllocateTimer();

    (void)Serial_Print(gAppSerMgrIf, "\n\rTemperature sensor -> Press switch to start advertising.\n\r", gAllowToBlock_d);

    /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    #if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
        #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
            (void)PWR_ChangeBlackBoxDeepSleepMode(cPWR_DeepSleepMode);
            PWR_AllowBlackBoxToSleep();
        #endif
        (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
        PWR_AllowDeviceToSleep();
    #else
        (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
        PWR_AllowDeviceToSleep();
    #endif
#endif
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    /* Set advertising parameters*/
    (void)Gap_SetAdvertisingParameters(&gAdvParams);
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

            if(!mAdvState.advOn)
            {
                (void)Serial_Print(gAppSerMgrIf, "Advertising stopped.\n\r", gAllowToBlock_d);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
                Led1Off();
                /* Go to sleep */
                SleepTimeoutSequence();
#else
                /* UI */
                LED_StopFlashingAllLeds();
                Led1Flashing();
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
#endif
            }
            else
            {
                (void)Serial_Print(gAppSerMgrIf, "Advertising started.\n\r", gAllowToBlock_d);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
                /* Start advertising timer */
                (void)TMR_StartLowPowerTimer(appTimerId,
                       gTmrLowPowerSecondTimer_c,
                       TmrSeconds(gAdvTime_c),
                       AdvertisingTimerCallback, NULL);
                Led1On();
#else
                /* UI */
                LED_StopFlashingAllLeds();
                Led1Flashing();
#endif
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            /* Panic UI */
            Led2On();
            panic(0,0,0,0);
        }
        break;

        default:
        {
            ; /* No action required */
        }
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
            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;
            (void)TMR_StopTimer(appTimerId);

            /* Subscribe client*/
            mPeerDeviceId = peerDeviceId;
            (void)Bas_Subscribe(&basServiceConfig, peerDeviceId);
            (void)Tms_Subscribe(peerDeviceId);

            (void)Serial_Print(gAppSerMgrIf, "Connected!\n\r", gAllowToBlock_d);
            /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
             #if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
                #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
                    (void)PWR_ChangeBlackBoxDeepSleepMode(gAppDeepSleepMode_c);
                    PWR_AllowBlackBoxToSleep();
                #endif
             #else
                (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);
                PWR_AllowDeviceToSleep();
             #endif
#else
            LED_StopFlashingAllLeds();
#endif
            /* UI */
            Led1On();
        }
        break;

        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            mPeerDeviceId = gInvalidDeviceId_c;
            (void)Bas_Unsubscribe(&basServiceConfig, peerDeviceId);
            (void)Tms_Unsubscribe();

            (void)Serial_Print(gAppSerMgrIf, "Disonnected!\n\r", gAllowToBlock_d);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
            /* UI */
            Led1Off();
            /* Go to sleep */
            SleepTimeoutSequence();
#else
            /* restart advertising*/
            BleApp_Start();
#endif
        }
        break;

        case gConnEvtEncryptionChanged_c:   /* Fallthrough */
        default:
        {
            ; /* No action required */
        }
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
            /* Notify the temperature value when CCCD is written */
            BleApp_SendTemperature();
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
/*! *********************************************************************************
* \brief        Stops advertising when the application timeout has expired.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void AdvertisingTimerCallback(void* pParam)
{
    /* Stop advertising */
    if (mAdvState.advOn)
    {
        (void)Gap_StopAdvertising();
    }
}

/*! *********************************************************************************
* \brief        Handles disconnect timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void DisconnectTimerCallback(void* pParam)
{
    /* Terminate connection */
    if (mPeerDeviceId != gInvalidDeviceId_c)
    {
        (void)Gap_Disconnect(mPeerDeviceId);
    }
}
#endif

/*! *********************************************************************************
* \brief        Sends temperature value over-the-air.
*
********************************************************************************** */
static void BleApp_SendTemperature(void)
{
    (void)TMR_StopTimer(appTimerId);

    /* Update with initial temperature */
    (void)Tms_RecordTemperatureMeasurement((uint16_t)service_temperature,
                                           (int16_t)(BOARD_GetTemperature() * 100));

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    /* Start Sleep After Data timer */
    (void)TMR_StartLowPowerTimer(appTimerId,
                           gTmrLowPowerSecondTimer_c,
                           TmrSeconds(gGoToSleepAfterDataTime_c),
                           DisconnectTimerCallback, NULL);
#endif
}

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static void SleepTimeoutSequence(void)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
    #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
        (void)PWR_ChangeBlackBoxDeepSleepMode(cPWR_DeepSleepMode);
    #endif
#else
    (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
#endif
}
#endif

/*! *********************************************************************************
* @}
********************************************************************************** */
