/*! *********************************************************************************
* \addtogroup Temperature Sensor
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
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
#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
#include "GPIO_Adapter.h"
#include "gpio_pins.h"
#endif
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
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#if ( gAppStartAfterReset_d != 0) && (gAppStartAfterResetToLowPowerDelayMs_d != 0)
static tmrTimerID_t mAllowDeviceToSleepTimerId;
#endif
#endif
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

static void AppPrintString(const char* pBuff);
#if defined(gAppPrintLePhyEvent_c) && (gAppPrintLePhyEvent_c)
static void AppPrintLePhyEvent(gapPhyEvent_t* pPhyEvent);
#endif

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#if ( gAppStartAfterReset_d != 0) && (gAppStartAfterResetToLowPowerDelayMs_d != 0)
static void AllowDeviceToSleepTimerCallback(void *);
#endif
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
    /* Get the serial interface in use */
    gAppSerMgrIf = BOARD_GetSerialManagerInterface();
#else
    SerialManager_Init();
    Serial_InitInterface(&gAppSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate(gAppSerMgrIf, APP_SERIAL_INTERFACE_SPEED);
#endif

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
#if (gAppStartAfterReset_d)            
            BleApp_Start();
#endif
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
#if defined(gAppPrintLePhyEvent_c) && (gAppPrintLePhyEvent_c)
    case gLePhyEvent_c:
        if(pGenericEvent->eventData.phyEvent.phyEventType == gPhyUpdateComplete_c )
        {
            AppPrintLePhyEvent(&pGenericEvent->eventData.phyEvent);
        }
#endif

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
#if (gAppStartAfterReset_d == 0)
    AppPrintString("\r\nTemperature sensor -> Press switch to start advertising.\r\n");
#endif
    /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    #if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
        #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
             #if (gAppStartAfterReset_d == 0)
                 (void)PWR_ChangeBlackBoxDeepSleepMode(cPWR_DeepSleepMode);
             #endif

             #if (gAppStartAfterReset_d == 0) || (gAppStartAfterResetToLowPowerDelayMs_d == 0)
                  PWR_AllowBlackBoxToSleep();
             #endif
        #endif
        (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
        PWR_AllowDeviceToSleep();
    #else
        #if (gAppStartAfterReset_d == 0)
            (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
        #endif
        #if (gAppStartAfterReset_d == 0) || (gAppStartAfterResetToLowPowerDelayMs_d == 0)    
            PWR_AllowDeviceToSleep();
        #endif
    #endif /* defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE) */
    #if ( gAppStartAfterReset_d != 0) && (gAppStartAfterResetToLowPowerDelayMs_d != 0)
        mAllowDeviceToSleepTimerId = TMR_AllocateTimer();
        TMR_StartLowPowerTimer( mAllowDeviceToSleepTimerId, gTmrSingleShotTimer_c, gAppStartAfterResetToLowPowerDelayMs_d, AllowDeviceToSleepTimerCallback, NULL);
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
                AppPrintString("Advertising stopped.\r\n");

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
                AppPrintString("Advertising started.\r\n");

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

            AppPrintString("Connected!\r\n");
            /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
             #if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
                #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
                    (void)PWR_ChangeBlackBoxDeepSleepMode(gAppDeepSleepMode_c);
                    PWR_AllowBlackBoxToSleep();
                #endif
             #else
                (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);
#if (!defined(CPU_MKW37A512VFT4) && !defined(CPU_MKW37Z512VFT4) && !defined(CPU_MKW38A512VFT4) && !defined(CPU_MKW38Z512VFT4) && !defined(CPU_MKW39A512VFT4) && !defined(CPU_MKW39Z512VFT4))
                PWR_AllowDeviceToSleep();
#endif /* CPU_MKW37xxx, CPU_MKW38xxx and CPU_MKW39xxx*/
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

            AppPrintString("Disconnected!\r\n");

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

        case gConnEvtEncryptionChanged_c:   /* Fall-through */
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
* \brief        Prints a string.
*
********************************************************************************** */
static void AppPrintString(const char* pBuff)
{
    (void)Serial_Print(gAppSerMgrIf, pBuff, gAllowToBlock_d);
}
/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
#if defined(gAppPrintLePhyEvent_c) && (gAppPrintLePhyEvent_c)
static void PrintLePhyEvent(void(*pfPrint)(const char *),gapPhyEvent_t* pPhyEvent)
{
    /* String dictionary corresponding to gapLePhyMode_t */
    static const char* mLePhyModeStrings[] =
    {
        "Invalid\r\n",
        "1M\r\n",
        "2M\r\n",
        "Coded\r\n",
    };
    uint8_t txPhy = (pPhyEvent->txPhy <= gLePhyCoded_c) ? pPhyEvent->txPhy : 0;
    uint8_t rxPhy = (pPhyEvent->rxPhy <= gLePhyCoded_c) ? pPhyEvent->rxPhy : 0;
    pfPrint("Phy Update Complete.\r\n");
    pfPrint("TxPhy ");
    pfPrint(mLePhyModeStrings[txPhy]);
    pfPrint("RxPhy ");
    pfPrint(mLePhyModeStrings[rxPhy]);
}
#endif

/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
#if defined(gAppPrintLePhyEvent_c) && (gAppPrintLePhyEvent_c)
static void AppPrintLePhyEvent(gapPhyEvent_t* pPhyEvent)
{
    PrintLePhyEvent(AppPrintString, pPhyEvent);
}
#endif

/*! *********************************************************************************
* \brief        Handles mAllowDeviceToSleepTimerId timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#if ( gAppStartAfterReset_d != 0) && (gAppStartAfterResetToLowPowerDelayMs_d != 0)
static void AllowDeviceToSleepTimerCallback(void * pParam)
{
  (void)pParam;
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
#if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
  PWR_AllowBlackBoxToSleep();
#endif
#else
  PWR_AllowDeviceToSleep();
#endif
}
#endif /*defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)*/
#endif /* ( gAppStartAfterReset_d != 0) && (gAppStartAfterResetToLowPowerDelayMs_d != 0) */
/*! *********************************************************************************
* @}
********************************************************************************** */
