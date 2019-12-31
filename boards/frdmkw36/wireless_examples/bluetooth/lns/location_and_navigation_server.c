/*! *********************************************************************************
* \addtogroup Location and Navigation Server
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Bocation and Navigation Server application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
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
#if !defined(MULTICORE_APPLICATION_CORE) || (!MULTICORE_APPLICATION_CORE)
#include "gatt_db_handles.h"
#endif

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "location_and_navigation_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "location_and_navigation_server.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
#include "erpc_host.h"
#include "dynamic_gatt_database.h"
#endif

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

#define mBatteryLevelReportInterval_c   (10U)            /* battery level report interval in seconds  */
#define mLocAndSpeedReportInterval_c    (1U)
#define mNavigationReportInterval_c     (1U)
#define mPositionQualityReadInterval_c  (10U)
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

/* Service Data*/
static lnsUserData_t    mUserData =
{
    .lnsPositionQuality =
    {
        .lnsPositionQualityFlags = 0x0000,
    },
};
static bool_t           basValidClientList[gAppMaxConnections_c] = { FALSE };
static basConfig_t      basServiceConfig = {(uint16_t)service_battery, 0, basValidClientList, gAppMaxConnections_c};
static disConfig_t      disServiceConfig = {(uint16_t)service_device_info};
static lnsConfig_t      lnsServiceConfig = {(uint16_t)service_location_and_navigation, Lns_AllFeatures, &mUserData};

static uint16_t cpHandles[] = { (uint16_t)value_lncp };

/* Application specific data*/
static tmrTimerID_t mAdvTimerId;
static tmrTimerID_t mBatteryMeasurementTimerId;
static tmrTimerID_t mLocAndSpeedMeasurementTimerId;
static tmrTimerID_t mNavigationMeasurementTimerId;
static tmrTimerID_t mPositionQualityReadTimerId;
static tmrTimerID_t mLocalTimeTickTimerId;

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
static void BleApp_Config(void);

/* Timer Callbacks */
static void AdvertisingTimerCallback (void *pParam);
static void BatteryMeasurementTimerCallback (void *pParam);
static void LocAndSpeedMeasurementTimerCallback (void *pParam);
static void NavigationMeasurementTimerCallback (void *pParam);
static void PositionQualityReadTimerCallback (void *pParam);
static void LocalTimeTickTimerCallback (void *pParam);

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

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
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
            break;
        }
        case gKBD_EventLongPB1_c:
        {
            if (mPeerDeviceId != gInvalidDeviceId_c)
            {
                (void)Gap_Disconnect(mPeerDeviceId);
            }
            break;
        }
        case gKBD_EventLongPB2_c:
        {
            break;
        }
        default:
            ; /* For MISRA compliance */
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
            (void)Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
        }
        break;

        case gAdvertisingDataSetupComplete_c:
        {
            (void)App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;

        case gAdvertisingSetupFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
            ; /* For MISRA compliance */
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
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1U)
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
    (void)GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpHandles), cpHandles);

    mAdvState.advOn = FALSE;

    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_Start(&basServiceConfig);
    (void)Dis_Start(&disServiceConfig);
    (void)Lns_Start(&lnsServiceConfig);

    /* Allocate application timers */
    mAdvTimerId = TMR_AllocateTimer();
    mBatteryMeasurementTimerId = TMR_AllocateTimer();
    mLocAndSpeedMeasurementTimerId = TMR_AllocateTimer();
    mPositionQualityReadTimerId = TMR_AllocateTimer();
    mLocalTimeTickTimerId = TMR_AllocateTimer();

    (void)TMR_StartLowPowerTimer(mLocalTimeTickTimerId, gTmrLowPowerIntervalMillisTimer_c,
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

        default:
            ; /* For MISRA compliance */
        break;
    }

    /* Set advertising parameters*/
    (void)Gap_SetAdvertisingParameters(&gAdvParams);

    /* Start advertising timer */
    (void)TMR_StartLowPowerTimer(mAdvTimerId,gTmrLowPowerSecondTimer_c,
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
            ; /* For MISRA compliance */
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
            (void)Bas_Subscribe(&basServiceConfig, peerDeviceId);
            (void)Lns_Subscribe(peerDeviceId);

            /* UI */
            LED_StopFlashingAllLeds();
            Led1On();

            /* Stop Advertising Timer*/
            (void)TMR_StopTimer(mAdvTimerId);

            /* Start battery measurements */
            (void)TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);
            (void)TMR_StartLowPowerTimer(mLocAndSpeedMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mLocAndSpeedReportInterval_c), LocAndSpeedMeasurementTimerCallback, NULL);
            (void)TMR_StartLowPowerTimer(mNavigationMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mNavigationReportInterval_c), NavigationMeasurementTimerCallback, NULL);
            (void)TMR_StartLowPowerTimer(mPositionQualityReadTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mPositionQualityReadInterval_c), PositionQualityReadTimerCallback, NULL);
        }
        break;

        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            (void)Bas_Unsubscribe(&basServiceConfig, peerDeviceId);
            (void)Lns_Unsubscribe(&lnsServiceConfig);

            mPeerDeviceId = gInvalidDeviceId_c;

            /* Restart advertising*/
            BleApp_Start();
        }
        break;

    default:
        ; /* For MISRA compliance */
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
            status = (uint8_t)gAttErrCodeNoError_c;

            if (handle == (uint16_t)value_lncp)
            {
                Lns_ControlPointHandler(&lnsServiceConfig, &pServerEvent->eventData.attributeWrittenEvent);
            }
            else
            {
                (void)GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
            }
            break;
        }

        case gEvtError_c:
        {
            if (pServerEvent->eventData.procedureError.error == gGattClientConfirmationTimeout_c)
            {

            }
            break;
        }
    default:
        ; /* For MISRA compliance */
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
    (void)Gap_StopAdvertising();

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
            ; /* For MISRA compliance */
        break;
    }
    BleApp_Advertise();
}

/*! *********************************************************************************
* \brief        Handles location and speed measurement timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void LocAndSpeedMeasurementTimerCallback (void * pParam)
{
    uint32_t    distance = 0U;
    uint32_t    randomNumber = 0U;

    if(0U != (localTime % 2U))
    {
        RNG_GetRandomNo(&randomNumber);

        if((0U != (lnsServiceConfig.lnsFeatures & gLns_InstantaneousSpeedSupported_c)) &&
           (0U == (lnsServiceConfig.pUserData->lnsMask & gLnsInstantaneousSpeedOff_c)))
        {
            mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_InstantaneousSpeedPresent_c;
            mUserData.lnsLocAndSpeed.lnsInstantaneousSpeed = (uint16_t)randomNumber & 0x1FU;
        }

        if((0U != (lnsServiceConfig.lnsFeatures & gLns_TotalDistanceSupported_c)) &&
           (0U == (lnsServiceConfig.pUserData->lnsMask & gLnsTotalDistanceOff_c)))
        {
            mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_TotalDistancePresent_c;
            distance = (randomNumber & 0x1FU) / 10U;
            mUserData.lnsLocAndSpeed.lnsTotalDistance += distance;
        }

        RNG_GetRandomNo(&randomNumber);

        if((0U != (lnsServiceConfig.lnsFeatures & gLns_LocationSupported_c)) &&
           (0U == (lnsServiceConfig.pUserData->lnsMask & gLnsLocationOff_c)))
        {
            mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_LocationPresent_c;
            mUserData.lnsLocAndSpeed.lnsLatitude = 45000000 + (int32_t)((uint32_t)(randomNumber & 0x3FFFFFFU));
            mUserData.lnsLocAndSpeed.lnsLongitude = 35000000 + (int32_t)((uint32_t)(randomNumber & 0x3FFFFFFU));
        }

        RNG_GetRandomNo(&randomNumber);

        if((0U != (lnsServiceConfig.lnsFeatures & gLns_ElevationSupported_c)) &&
           (0U == (lnsServiceConfig.pUserData->lnsMask & gLnsElevationOff_c)))
        {
            mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_ElevationPresent_c;
            mUserData.lnsLocAndSpeed.lnsElevation = (int32_t)((uint32_t)(randomNumber & 0x0FFFU)) * 100;
        }

        if((0U != (lnsServiceConfig.lnsFeatures & gLns_HeadingSupported_c)) &&
           (0U == (lnsServiceConfig.pUserData->lnsMask & gLnsHeadingOff_c)))
        {
            mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_HeadingPresent_c;
            mUserData.lnsLocAndSpeed.lnsHeading = 0U;
        }

        if((0U != (lnsServiceConfig.lnsFeatures & gLns_RollingTimeSupported_c)) &&
           (0U == (lnsServiceConfig.pUserData->lnsMask & gLnsRollingTimeOff_c)))
        {
            mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_RollingTimePresent_c;
            mUserData.lnsLocAndSpeed.lnsRollingTime += 1U;
        }

        if((0U != (lnsServiceConfig.lnsFeatures & gLns_UtcTimeSupported_c)) &&
           (0U == (lnsServiceConfig.pUserData->lnsMask & gLnsUtcTimeOff_c)))
        {
            mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_UtcTimePresent_c;
            mUserData.lnsLocAndSpeed.lnsUtcTime += 1U;
        }

        mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_PositionOk_c;
        mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_SpeedAndDistance3D_c;
        mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_BarometricAirPressure_c;
        mUserData.lnsLocAndSpeed.lnsLocAndSpeedFlags |= gLns_HeadingBasedOnMovement_c;
    }

    /* Send measurement over-the-air */
    (void)Lns_RecordLocationAndSpeedCharacteristic((uint16_t)service_location_and_navigation, &mUserData.lnsLocAndSpeed);
}

/*! *********************************************************************************
* \brief        Handles location and speed measurement timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void NavigationMeasurementTimerCallback (void * pParam)
{
    uint32_t    randomNumber = 0;

    RNG_GetRandomNo(&randomNumber);

    mUserData.lnsNavigation.lnsBearing = (uint16_t)randomNumber & 0xFFFFU;

    RNG_GetRandomNo(&randomNumber);

    mUserData.lnsNavigation.lnsHeading = (uint16_t)randomNumber & 0xFFFFU;

    RNG_GetRandomNo(&randomNumber);

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_RemainingDistanceSupported_c))
    {
        mUserData.lnsNavigation.lnsNavigationFlags |= gLns_RemainingDistancePresent_c;
        mUserData.lnsNavigation.lnsRemainingDistance = randomNumber & 0xFFFFFFU;
    }

    RNG_GetRandomNo(&randomNumber);

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_RemainingVerticalDistanceSupported_c))
    {
        mUserData.lnsNavigation.lnsNavigationFlags |= gLns_RemainingVerticalDistancePresent_c;
        mUserData.lnsNavigation.lnsRemainingDistance = randomNumber & 0xFFFFFFU;
    }

    RNG_GetRandomNo(&randomNumber);

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_EstmatedTimeOfArrivalSupported_c))
    {
        mUserData.lnsNavigation.lnsNavigationFlags |= gLns_EstimatedTimeOfArrivalPresent_c;
        mUserData.lnsNavigation.lnsETA = localTime + (randomNumber & 0xFFFFU);
    }

    /* Send measurement over-the-air */
    (void)Lns_RecordNavigationCharacteristic((uint16_t)service_location_and_navigation, &mUserData.lnsNavigation);
}

static void PositionQualityReadTimerCallback(void * pParam)
{
    uint32_t    randomNumber = 0;

    RNG_GetRandomNo(&randomNumber);

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_NumberOfBeaconsInSolutionSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_NoOfBeaconsInSolutionPresent_c;
        mUserData.lnsPositionQuality.lnsNumberOfBeaconsInSolution = (uint8_t)randomNumber;
    }

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_NumberOfBeaconsInViewSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_NoOfBeaconsInViewPresent_c;
        mUserData.lnsPositionQuality.lnsNumberOfBeaconsInView =     (uint8_t)((randomNumber & 0xFF00U) >> 8U);
    }

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_TimeToFirstFixSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_TimeToFirstFixPresent_c;
        mUserData.lnsPositionQuality.lnsTimeToFirstFix = (uint16_t)(randomNumber & 0xFFFF0000U);
    }

    RNG_GetRandomNo(&randomNumber);

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_EstimatedHorizontalPositionErrorSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_EHPEPresent_c;
        mUserData.lnsPositionQuality.lnsEHPE = (randomNumber & 0xFFFFFFFFU);
    }

    RNG_GetRandomNo(&randomNumber);

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_EstimatedVerticalPositionErrorSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_EVPEPresent_c;
        mUserData.lnsPositionQuality.lnsEVPE = (randomNumber & 0xFFFFFFFFU);
    }

    RNG_GetRandomNo(&randomNumber);

    if(0U != (lnsServiceConfig.lnsFeatures & gLns_HorizontalDilutionOfPrecisionSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_HDOPPresent_c;
        mUserData.lnsPositionQuality.lnsHDOP = (uint8_t)randomNumber;
    }

    if(0u != (lnsServiceConfig.lnsFeatures & gLns_VerticalDilutionOfPrecisionSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_VDOPPresent_c;
        mUserData.lnsPositionQuality.lnsVDOP = (uint8_t)((randomNumber & 0xFF00U) >> 8U);
    }

    if(0u != (lnsServiceConfig.lnsFeatures & gLns_PositionStatusSupported_c))
    {
        mUserData.lnsPositionQuality.lnsPositionQualityFlags |= gLns_LastKnownPosition_c;
    }

    /* Send measurement over-the-air */
    (void)Lns_RecordPositionQualityCharacteristic((uint16_t)service_location_and_navigation, &mUserData.lnsPositionQuality);

}

/*! *********************************************************************************
* \brief        Handles battery measurement timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void BatteryMeasurementTimerCallback(void * pParam)
{
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_RecordBatteryMeasurement(&basServiceConfig);
}

static void LocalTimeTickTimerCallback (void * pParam)
{
   localTime++;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
