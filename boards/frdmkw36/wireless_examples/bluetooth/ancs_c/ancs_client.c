/*! *********************************************************************************
* \addtogroup ANCS Client
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2018-2019 NXP
*
* \file ancs_client.c
*
* This file is the source file for the ANCS Client application
*
* SPDX-License-Identifier: BSD-3-Clause
*/

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
#include "shell.h"
#include "Panic.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1U)
    #include "dynamic_gatt_database.h"
    #define UUID128(name, ...) extern uint8_t name[];
    #include "gatt_uuid128.h"
    #undef UUID128
#else
    #include "gatt_db_handles.h"
#endif

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "current_time_interface.h"
#include "reference_time_update_interface.h"
#include "next_dst_change_interface.h"
#include "ancs_interface.h"

#include "ble_conn_manager.h"
#include "ble_service_discovery.h"
#include "board.h"
#include "ApplMain.h"
#include "ancs_client.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1U)
    #include "erpc_host.h"
    #include "dynamic_gatt_database.h"
#endif


/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mBatteryLevelReportInterval_c           (10)            /* battery level report interval in seconds  */
#define mReadTimeCharInterval_c                 (3600U)         /* interval between time sync in seconds */
#define mCharReadBufferLength_c                 (13U)           /* length of the buffer */
#define mInitialTime_c                          (1451606400U)   /* initial timestamp - 01/01/2016 00:00:00 GMT */

#define mMaxDisplayNotifications_c              (16U)           /* The maximum number of notifications the application is
                                                                 * capable of displaying and managing. This will impact memory usage. */
#define mMaxNotifAppNameDisplayLength_c         (40U)           /* The maximum number of characters to use to display the name of the associated application obtained from the server. */
#define mMaxNotifCatDisplayLength_c             (20U)           /* The maximum number of characters to use to display a notification category. */

#define mAppEvt_PeerConnected_c                 (0U)
#define mAppEvt_PairingComplete_c               (1U)
#define mAppEvt_ServiceDiscoveryComplete_c      (2U)
#define mAppEvt_ServiceDiscoveryFailed_c        (3U)
#define mAppEvt_GattProcComplete_c              (4U)
#define mAppEvt_GattProcError_c                 (5U)
#define mAppEvt_AncsNsNotificationReceived_c    (6U)
#define mAppEvt_AncsDsNotificationReceived_c    (7U)

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef union
{
    uint8_t        *pNsDataTemp;
    ancsNsPacket_t *pNsPacketTemp;
} pPayloadPacketTemp_t;

typedef enum appState_tag
{
    mAppIdle_c,
    mAppExchangeMtu_c,
    mAppPrimaryServiceDisc_c,
    mAppCharServiceDisc_c,
    mAppNsDescriptorSetup_c,
    mAppDsDescriptorSetup_c,
    mAppRunning_c,
} appState_t;

typedef enum
{
#if gAppUseBonding_d
    fastWhiteListAdvState_c,
#endif
    fastAdvState_c,
    slowAdvState_c
} advType_t;

typedef struct advState_tag
{
    bool_t      advOn;
    advType_t   advType;
} advState_t;

typedef struct appCustomInfo_tag
{
    ancsClientConfig_t   ancsClientConfig;
    /* Add persistent information here */
} appCustomInfo_t;

typedef struct appPeerInfo_tag
{
    deviceId_t      deviceId;
    appCustomInfo_t customInfo;
    bool_t          isBonded;
    appState_t      appState;
} appPeerInfo_t;

/*! Structure type holding information about the notifications displayed on the ANCS Client. */
typedef struct notifInfo_tag
{
    bool_t          slotUsed;       /*!< Boolean signaling if the slot is in use or not. */
    ancsEventFlag_t notifFlags;     /*!< 0x01 = Silent = S
                                     *   0x02 = Important = I
                                     *   0x04 = Pre Existing = E
                                     *   0x08 = Positive Action = P
                                     *   0x10 = Negative Action = N
                                     *   Only the set flags are displayed. */
    ancsCatId_t     notifCat;       /*!< Notification category. Should be displayed as text. */
    uint32_t        notifUid;       /*!< Notification unique identifier. Should be displayed as is. */
    bool_t          appNameValid;   /*!< Application name has been obtained from the ANCS Server and is valid. */
    uint8_t         appName[mMaxNotifAppNameDisplayLength_c];   /*!< Application displayed name. */
} notifInfo_t;

/*! Structure containing the OTAP Server functional data. */
typedef struct ancsClientAppData_tag
{
    notifInfo_t     notifications[mMaxDisplayNotifications_c];
    ancsComdId_t    lastCommandSentToAncsProvider;
    bool_t          notificationDataChanged;
} ancsClientAppData_t;


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static appPeerInfo_t mPeerInformation =
{
    .deviceId = gInvalidDeviceId_c,
    .customInfo =
    {
        .ancsClientConfig =
        {
            .hService = gGattDbInvalidHandle_d,
            .hNotificationSource = gGattDbInvalidHandle_d,
            .hNotificationSourceCccd = gGattDbInvalidHandle_d,
            .hControlPoint = gGattDbInvalidHandle_d,
            .hDataSource = gGattDbInvalidHandle_d,
            .hDataSourceCccd = gGattDbInvalidHandle_d,
        },
    },
    .isBonded = FALSE,
    .appState = mAppIdle_c,
};

static const char ancsFlagsToLetterTable[] =
{
    'S', /*!< 0 - Silent */
    'I', /*!< 1 - Important */
    'E', /*!< 2 - pre Existing */
    'P', /*!< 3 - Positive action */
    'N', /*!< 4 - Negative action */
};

static uint8_t ancsNotifCatToStringTable[][mMaxNotifCatDisplayLength_c + 1] = /* Extra byte for the 0 terminated string */
{
    "Other               ", /*  0 */
    "IncomingCall        ", /*  1 */
    "MissedCall          ", /*  2 */
    "Voicemail           ", /*  3 */
    "Social              ", /*  4 */
    "Schedule            ", /*  5 */
    "Email               ", /*  6 */
    "News                ", /*  7 */
    "Health And Fitness  ", /*  8 */
    "Business And Finance", /*  9 */
    "Location            ", /* 10 */
    "Entertainment       ", /* 11 */
    "Invalid             ", /* 12 */
};

static const char mAncsFalgNotSetSymbol = '_';
static const char mAncsAppNameCharPlaceholder = '_';
static const char mAncsNotifCatCharPlaceholder = '_';

#if gAppUseBonding_d
    static bool_t mSendDataAfterEncStart = FALSE;
#endif

/* Adv State */
static advState_t       mAdvState;

/* Service Data */
static bool_t           basValidClientList[gAppMaxConnections_c] = { FALSE };
static basConfig_t      basServiceConfig = {(uint16_t)service_battery, 0, basValidClientList, gAppMaxConnections_c};
static disConfig_t      disServiceConfig = {(uint16_t)service_device_info};
static ctsConfig_t      ctsServiceConfig = {(uint16_t)service_current_time, gCts_InitTime, gCts_InitTime, 0U, gCts_InitLocalTimeInfo, gCts_InitReferenceTimeInfo, FALSE};
static rtusConfig_t     rtusServiceConfig = {(uint16_t)service_reference_time, {gRtusIdle_c, gRtusSuccessful_c}};
static ndcsConfig_t     ndcsServiceConfig = {(uint16_t)service_next_dst, {{2016, 1, 1, 0, 0, 0}, 0U}};
static uint16_t         cpHandles[] = { (uint16_t)value_current_time, (uint16_t)value_time_update_cp };

/* Application specific data*/
static tmrTimerID_t     mAdvTimerId;
static tmrTimerID_t     mCTSTickTimerId;
static tmrTimerID_t     mRTUSReferenceUpdateTimerId;
static tmrTimerID_t     mBatteryMeasurementTimerId;

/* Buffer used for Service Discovery */
static gattService_t            *mpServiceDiscoveryBuffer = NULL;
static uint8_t                  mcPrimaryServices = 0;

/* Buffer used for Characteristic Discovery */
static gattCharacteristic_t     *mpCharDiscoveryBuffer = NULL;
static uint8_t                  mCurrentServiceInDiscoveryIndex;
static uint8_t                  mCurrentCharInDiscoveryIndex;

/* Buffer used for Characteristic Descriptor Discovery */
static gattAttribute_t          *mpCharDescriptorBuffer = NULL;

/* Buffer used for Characteristic related procedures */
static gattAttribute_t          *mpDescProcBuffer = NULL;

/* Application Data */

/*! ANCS Client data structure.
 *  Contains functional information about the notifications displayed
 *  and manipulated by the ANCS Client */
static ancsClientAppData_t      ancsClientData;

#if gAppUseTimeService_d
    static uint8_t mOutCharReadBuffer[mCharReadBufferLength_c];
    static uint16_t mOutCharReadByteCount;
    static bool_t isTimeSynchronized = FALSE;

    static uint32_t localTime = mInitialTime_c;
#endif


/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Host Stack callbacks */
static void BleApp_AdvertisingCallback
(
    gapAdvertisingEvent_t *pAdvertisingEvent
);

static void BleApp_ConnectionCallback
(
    deviceId_t peerDeviceId,
    gapConnectionEvent_t *pConnectionEvent
);

static void BleApp_GattServerCallback
(
    deviceId_t deviceId,
    gattServerEvent_t *pServerEvent
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
    deviceId_t  serverDeviceId,
    uint16_t    characteristicValueHandle,
    uint8_t    *aValue,
    uint16_t    valueLength
);

static void BleApp_HandleValueWriteConfirmations
(
    deviceId_t  deviceId
);

static void BleApp_AttributeNotified
(
    deviceId_t  deviceId,
    uint16_t    handle,
    uint8_t    *pValue,
    uint16_t    length
);

static void BleApp_Config(void);

static void BleApp_StateMachineHandlerExchangeMtu(deviceId_t peerDeviceId, uint8_t event);
static void BleApp_StateMachineHandlerPrimaryServiceDisc(deviceId_t peerDeviceId, uint8_t event);
static void BleApp_StateMachineHandlerCharServiceDisc(deviceId_t peerDeviceId, uint8_t event);
static void BleApp_StateMachineHandlerNsDescriptorSetup(deviceId_t peerDeviceId, uint8_t event);

void BleApp_StateMachineHandler
(
    deviceId_t peerDeviceId,
    uint8_t event
);


static void BleApp_HandleAttMtuChange
(
    deviceId_t peerDeviceId
);

static void BleApp_StoreServiceHandles
(
    gattService_t   *pService
);

static void BleApp_StoreCharHandles
(
    gattCharacteristic_t   *pChar
);

static void BleApp_StoreDescValues
(
    gattAttribute_t     *pDesc
);

static void BleApp_ServiceDiscoveryErrorHandler(void);

/* Timer Callbacks */
static void AdvertisingTimerCallback(void *pParam);
static void BatteryMeasurementTimerCallback(void *pParam);
static void CTSTickTimerCallback(void *pParam);
static void RTUSReferenceUpdateTimerCallback(void *pParam);

static void BleApp_Advertise(void);

/* Application Functions */
static void AncsClient_HandleDisconnectionEvent(deviceId_t deviceId);
static void AncsClient_ResetNotificationData(void);
static void AncsClient_ProcessNsNotification(deviceId_t deviceId, uint8_t *pNsData, uint16_t nsDataLength);
static void AncsClient_ProcessDsNotifAttributes(deviceId_t deviceId, uint8_t *pDsData, uint16_t dsDataLength);
static void AncsClient_ProcessDsNotifAppInfo(uint8_t *pDsData, uint16_t dsDataLength);
static void AncsClient_ProcessDsNotification(deviceId_t deviceId, uint8_t *pDsData, uint16_t dsDataLength);
static void AncsClient_DisplayNotifications(void);
static void AncsClient_SendCommandToAncsServer(deviceId_t ancsServerDevId, void *pCommand, uint16_t cmdLength);

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

    /* UI */
    shell_init("BLE ANCS Client>");

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1U)
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

    if (mPeerInformation.deviceId == gInvalidDeviceId_c)
    {
        /* Device is not connected and not advertising */
        if (!mAdvState.advOn)
        {
#if gAppUseBonding_d

            if (gcBondedDevices > 0U)
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
            {
                (void)Gap_Disconnect(mPeerInformation.deviceId);
            }

            break;
        }

        case gKBD_EventPressPB2_c:
        {
            ctsServiceConfig.localTime += 3600U;
            ctsServiceConfig.adjustReason = gCts_ManualUpdate;
            (void)Cts_RecordCurrentTime(&ctsServiceConfig);
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
void BleApp_GenericCallback(gapGenericEvent_t *pGenericEvent)
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
            panic(0, 0, 0, 0);
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
        panic(0, 0, 0, 0);
        return;
    }

#endif /* MULTICORE_APPLICATION_CORE */

    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();

    /* Register for callbacks*/
    (void)App_RegisterGattServerCallback(BleApp_GattServerCallback);
    (void)App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    (void)GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpHandles), cpHandles);
    (void)App_RegisterGattClientNotificationCallback(BleApp_GattNotificationCallback);

    mAdvState.advOn = FALSE;

    /* Start services */
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_Start(&basServiceConfig);
    (void)Dis_Start(&disServiceConfig);
    (void)Cts_Start(&ctsServiceConfig);
    (void)Ndcs_Start(&ndcsServiceConfig);
    (void)Rtus_Start(&rtusServiceConfig);

    /* Allocate application timers */
    mAdvTimerId = TMR_AllocateTimer();
    mBatteryMeasurementTimerId = TMR_AllocateTimer();
    mCTSTickTimerId = TMR_AllocateTimer();
    mRTUSReferenceUpdateTimerId = TMR_AllocateTimer();

    /* Start local time tick timer */
    (void)TMR_StartLowPowerTimer(mCTSTickTimerId, gTmrLowPowerIntervalMillisTimer_c,
                                 TmrSeconds(1), CTSTickTimerCallback, NULL);

    /* Start reference update timer */
    (void)TMR_StartLowPowerTimer(mRTUSReferenceUpdateTimerId, gTmrLowPowerIntervalMillisTimer_c,
                                 TmrSeconds(60), RTUSReferenceUpdateTimerCallback, NULL);

    /* Reset application data. */
    AncsClient_ResetNotificationData();

    /* UI */
    shell_write("\r\nPress ADVSW to start advertising to find a device with the ANCS Service!\r\n");
}


/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
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
    (void)TMR_StartLowPowerTimer(mAdvTimerId, gTmrLowPowerSecondTimer_c,
                                 TmrSeconds(timeout), AdvertisingTimerCallback, NULL);
}


/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback(gapAdvertisingEvent_t *pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
        {
            mAdvState.advOn = !mAdvState.advOn;
            LED_StopFlashingAllLeds();
            Led1Flashing();

            if (!mAdvState.advOn)
            {
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();

                shell_write("\r\nStopped advertising...\r\n");
            }
            else
            {
                shell_write("\r\nAdvertising...\r\n");
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            panic(0, 0, 0, 0);
        }
        break;

        default:
        {
            shell_write("\r\nWarning: Unhandled Advertising Event:");
            shell_write(" 0x");
            shell_writeHex((uint8_t *) & (pAdvertisingEvent->eventType), 1);
            shell_write("\r\n");
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
static void BleApp_ConnectionCallback(deviceId_t peerDeviceId, gapConnectionEvent_t *pConnectionEvent)
{
    /* Connection Manager to handle Host Stack interactions */
    BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);

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

#if gAppUseBonding_d
            mSendDataAfterEncStart = FALSE;

            if ((gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &mPeerInformation.isBonded)) &&
                (TRUE == mPeerInformation.isBonded))
            {
                mSendDataAfterEncStart = TRUE;
            }
#endif

            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;

            BleApp_StateMachineHandler(mPeerInformation.deviceId, mAppEvt_PeerConnected_c);

            /* Subscribe client*/
            (void)Bas_Subscribe(&basServiceConfig, peerDeviceId);
            (void)Cts_Subscribe(peerDeviceId);
            (void)Ndcs_Subscribe(peerDeviceId);
            (void)Rtus_Subscribe(peerDeviceId);

            /* Stop Advertising Timer*/
            (void)TMR_StopTimer(mAdvTimerId);

            /* Start battery measurements */
            (void)TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                                         TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);
        }
        break;

        case gConnEvtDisconnected_c:
        {
            mPeerInformation.deviceId = gInvalidDeviceId_c;
            mPeerInformation.appState = mAppIdle_c;

            /* Reset Service Discovery to be sure*/
            BleServDisc_Stop(peerDeviceId);

            /* Stop battery measurements */
            (void)TMR_StopTimer(mBatteryMeasurementTimerId);

            /* Notify application */
            AncsClient_HandleDisconnectionEvent(peerDeviceId);

            /* UI */
            LED_TurnOffAllLeds();
            LED_StartFlash(LED_ALL);
            shell_write("\r\nDisconnected!\r\n");

            /* Unsubscribe client*/
            (void)Bas_Unsubscribe(&basServiceConfig, peerDeviceId);
            (void)Cts_Unsubscribe();
            (void)Ndcs_Unsubscribe();
            (void)Rtus_Unsubscribe();
        }
        break;

        case gConnEvtPairingRequest_c:
        {
#if gAppUsePairing_d
            shell_write("\r\nReceived Pairing Request. Response sent by Connection Manager.\r\n");
#else
            shell_write("\r\nReceived Pairing Request. Response sent by Connection Manager. Pairing not supported.\r\n");
#endif
        }
        break;

#if gAppUsePairing_d

        case gConnEvtPasskeyRequest_c:
            (void)Gap_EnterPasskey(peerDeviceId, gPasskeyValue_c);
            shell_write("\r\nReceived Passkey Request. Please enter passkey...\r\n");
            break;

        case gConnEvtPasskeyDisplay_c:
            shell_write("\r\nReceived Passkey Display Request. The passkey is: ");
            shell_writeDec(pConnectionEvent->eventData.passkeyForDisplay);
            shell_write("\r\n");
            break;

        case gConnEvtLeScDisplayNumericValue_c:
            shell_write("\r\nReceived Numeric Comparison Value: ");
            shell_writeDec(pConnectionEvent->eventData.numericValueForDisplay);
            shell_write("\r\n");
            break;

        case gConnEvtPairingResponse_c:
        {
            shell_write("\r\nReceived Pairing Response.\r\n");
            break;
        }

        case gConnEvtKeyExchangeRequest_c:
        {
            /* For GAP Peripherals the keys are automatically sent by the Connection Manager */
            shell_write("\r\nReceived Key Exchange Request. Sending keys...\r\n");
            break;
        }

        case gConnEvtLongTermKeyRequest_c:
        {
            shell_write("\r\nReceived LTK Request. Handled in Connection Manager.\r\n");
        }
        break;

        case gConnEvtEncryptionChanged_c:
        {
#if gAppUseTimeService_d

            if (isTimeSynchronized == FALSE)
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

#if gAppUseBonding_d
            if (mSendDataAfterEncStart)
            {
                /* Application handles encryption changes here. */
            }
#endif

            shell_write("\r\nReceived encryption changed event with link encryption status: ");
            shell_writeBool(pConnectionEvent->eventData.encryptionChangedEvent.newEncryptionState);
            shell_write("\r\n");
        }
        break;

        case gConnEvtPairingComplete_c:
        {
            shell_write("\r\nPairing completed!\r\n");

#if gAppUseTimeService_d

            if (isTimeSynchronized == FALSE)
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

            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                shell_write("\r\nPairing was successful!\r\n");
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PairingComplete_c);
            }
            else
            {
                shell_write("\r\nPairing was NOT successful with status:");
                shell_write(" 0x");
                shell_writeHex((uint8_t *)(&(pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.failReason)) + 1, 1);
                shell_writeHex((uint8_t *)(&(pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.failReason)), 1);
                shell_write("\r\n");
            }
        }
        break;

        case gConnEvtAuthenticationRejected_c:
        {
            shell_write("\r\nReceived authentication rejected event with status:");
            shell_write(" 0x");
            shell_writeHex((uint8_t *) & (pConnectionEvent->eventData.authenticationRejectedEvent.rejectReason), 1);
            shell_write("\r\n");
        }
        break;

        case gConnEvtKeysReceived_c:
        {
            shell_write("\r\nPairing Info: Received keys from peer device.\r\n");
        }
        break;
#endif /* gAppUsePairing_d */

        default:
        {
            shell_write("\r\nWarning: Unhandled GAP Connection Event:");
            shell_write(" 0x");
            shell_writeHex((uint8_t *) & (pConnectionEvent->eventType), 1);
            shell_write("\r\n");
        }
        break;
    }
}


static void BleApp_HandleAttMtuChange
(
    deviceId_t peerDeviceId
)
{
    uint16_t negotiatedAttMtu;

    /* Get the new negotiated ATT MTU */
    (void)Gatt_GetMtu(peerDeviceId, &negotiatedAttMtu);
    shell_write("\r\nNegotiated MTU: ");
    shell_writeDec(negotiatedAttMtu);
    shell_write("\r\n");
}


static void BleApp_StoreServiceHandles
(
    gattService_t   *pService
)
{
    uint8_t i;

    if ((pService->uuidType == gBleUuidType128_c) &&
        FLib_MemCmp(pService->uuid.uuid128, uuid128_ancs_service, 16))
    {
        /* Found ANCS Service */
        mPeerInformation.customInfo.ancsClientConfig.hService = pService->startHandle;
        shell_write("\r\nFound and stored ANCS Service handle!\r\n");

        /* Look for the ANCS Characteristics */
        for (i = 0; i < pService->cNumCharacteristics; i++)
        {
            if (pService->aCharacteristics[i].value.uuidType == gBleUuidType128_c)
            {
                if (TRUE == FLib_MemCmp(pService->aCharacteristics[i].value.uuid.uuid128,
                                        uuid128_ancs_ns_char,
                                        sizeof(pService->aCharacteristics[i].value.uuid.uuid128)))
                {
                    /* Found ANCS Notification Source Char */
                    mPeerInformation.customInfo.ancsClientConfig.hNotificationSource = pService->aCharacteristics[i].value.handle;
                    shell_write("\r\nFound and stored ANCS Notification Source Characteristic handle!\r\n");
                }
                else if (TRUE == FLib_MemCmp(pService->aCharacteristics[i].value.uuid.uuid128,
                                             uuid128_ancs_cp_char,
                                             sizeof(pService->aCharacteristics[i].value.uuid.uuid128)))
                {
                    /* Found ANCS Control Point Char */
                    mPeerInformation.customInfo.ancsClientConfig.hControlPoint = pService->aCharacteristics[i].value.handle;
                    shell_write("\r\nFound and stored ANCS Control Point Characteristic handle!\r\n");
                }
                else if (TRUE == FLib_MemCmp(pService->aCharacteristics[i].value.uuid.uuid128,
                                             uuid128_ancs_ds_char,
                                             sizeof(pService->aCharacteristics[i].value.uuid.uuid128)))
                {
                    /* Found ANCS Data Source Char */
                    mPeerInformation.customInfo.ancsClientConfig.hDataSource = pService->aCharacteristics[i].value.handle;
                    shell_write("\r\nFound and stored ANCS Data Source Characteristic handle!\r\n");
                }
                else
                {
                    ; /* For MISRA compliance */
                }
            }
        }
    }
}


static void BleApp_StoreCharHandles
(
    gattCharacteristic_t   *pChar
)
{
    uint8_t i;

    if (pChar->value.uuidType == gBleUuidType128_c)
    {
        if (TRUE == FLib_MemCmp(pChar->value.uuid.uuid128,
                                uuid128_ancs_ns_char,
                                sizeof(pChar->value.uuid.uuid128)))
        {
            for (i = 0; i < pChar->cNumDescriptors; i++)
            {
                if (pChar->aDescriptors[i].uuidType == gBleUuidType16_c)
                {
                    switch (pChar->aDescriptors[i].uuid.uuid16)
                    {
                        case gBleSig_CCCD_d:
                        {
                            mPeerInformation.customInfo.ancsClientConfig.hNotificationSourceCccd = pChar->aDescriptors[i].handle;
                            shell_write("\r\nFound and stored ANCS Notification Source CCCD handle!\r\n");
                            break;
                        }

                        default:
                            ; /* For MISRA compliance */
                            break;
                    }
                }
            }
        }
        else if (TRUE == FLib_MemCmp(pChar->value.uuid.uuid128,
                                     uuid128_ancs_ds_char,
                                     sizeof(pChar->value.uuid.uuid128)))
        {
            for (i = 0; i < pChar->cNumDescriptors; i++)
            {
                if (pChar->aDescriptors[i].uuidType == gBleUuidType16_c)
                {
                    switch (pChar->aDescriptors[i].uuid.uuid16)
                    {
                        case gBleSig_CCCD_d:
                        {
                            mPeerInformation.customInfo.ancsClientConfig.hDataSourceCccd = pChar->aDescriptors[i].handle;
                            shell_write("\r\nFound and stored ANCS Data Source CCCD handle!\r\n");
                            break;
                        }

                        default:
                            ; /* For MISRA compliance */
                            break;
                    }
                }
            }
        }
        else
        {
            ; /* For MISRA compliance */
        }
    }
}


/*! *********************************************************************************
* \brief        Handles GATT server callback from host stack.
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    pServerEvent    Pointer to gattServerEvent_t.
********************************************************************************** */
static void BleApp_GattServerCallback(deviceId_t deviceId, gattServerEvent_t *pServerEvent)
{
    uint16_t handle;
    uint8_t status;

    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWritten_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            status = (uint8_t)gAttErrCodeNoError_c;

            if (handle == (uint8_t)value_current_time)
            {
                (void)Cts_CurrentTimeWrittenHandler(&ctsServiceConfig, &pServerEvent->eventData.attributeWrittenEvent);
            }
            else
            {
                (void)GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
            }

            break;
        }

        case gEvtAttributeWrittenWithoutResponse_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;

            if (handle == (uint16_t)value_time_update_cp)
            {
                Rtus_ControlPointHandler(&rtusServiceConfig, &pServerEvent->eventData.attributeWrittenEvent);
            }

            break;
        }

        default:
            ; /* For MISRA compliance */
            break;
    }
}


static void BleApp_StoreDescValues
(
    gattAttribute_t     *pDesc
)
{
    /* No descriptor values are stored in this application. */
}


/*! *********************************************************************************
* \brief        Handles GATT client callback from host stack.
*
* \param[in]    serverDeviceId      GATT Server device ID.
* \param[in]    procedureType       Procedure type.
* \param[in]    procedureResult     Procedure result.
* \param[in]    error               Callback result.
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
        attErrorCode_t attError = (attErrorCode_t)((uint8_t)error);

        if (attError == gAttErrCodeInsufficientEncryption_c         ||
            attError == gAttErrCodeInsufficientEncryptionKeySize_c  ||
            attError == gAttErrCodeInsufficientAuthorization_c      ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
#if gAppUsePairing_d && gAppUseBonding_d
            bool_t isBonded = FALSE;
#endif /* gAppUsePairing_d && gAppUseBonding_d */

            shell_write("\r\nGATT Procedure Security Error:");
            shell_write(" 0x");
            shell_writeHex((uint8_t *) & (attError), 1);
            shell_write("\r\n");

#if gAppUsePairing_d
#if gAppUseBonding_d

            /* Check if the devices are bonded and if this is true than the bond may have
             * been lost on the peer device or the security properties may not be sufficient.
             * In this case try to restart pairing and bonding. */
            if ((gBleSuccess_c == Gap_CheckIfBonded(serverDeviceId, &isBonded)) &&
                (TRUE == isBonded))
#endif /* gAppUseBonding_d */
            {
                (void)Gap_SendSlaveSecurityRequest(serverDeviceId, &gPairingParameters);
            }

#endif /* gAppUsePairing_d */
        }

        else
        {
            shell_write("\r\nGATT Procedure Error:");
            shell_write(" 0x");
            shell_writeHex((uint8_t *) & (attError), 1);
            shell_write("\r\n");
        }

        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
    }
    else if (procedureResult == gGattProcSuccess_c)
    {
        switch (procedureType)
        {
            case gGattProcExchangeMtu_c:
            {
                BleApp_HandleAttMtuChange(serverDeviceId);
            }
            break;

            case gGattProcDiscoverAllPrimaryServices_c:         /* Fallthrough */
            case gGattProcWriteCharacteristicDescriptor_c:
                break;


            case gGattProcDiscoverAllCharacteristics_c:
            {
                BleApp_StoreServiceHandles(mpServiceDiscoveryBuffer + mCurrentServiceInDiscoveryIndex);
            }
            break;

            case gGattProcDiscoverAllCharacteristicDescriptors_c:
            {
                BleApp_StoreCharHandles(mpCharDiscoveryBuffer + mCurrentCharInDiscoveryIndex);

                /* Move on to the next characteristic */
                mCurrentCharInDiscoveryIndex++;
            }
            break;

            case gGattProcReadCharacteristicDescriptor_c:
            {
                if (mpDescProcBuffer != NULL)
                {
                    BleApp_StoreDescValues(mpDescProcBuffer);
                }
            }
            break;

            case gGattProcWriteCharacteristicValue_c:
            {
                BleApp_HandleValueWriteConfirmations(serverDeviceId);
            }
            break;

            case gGattProcReadUsingCharacteristicUuid_c:
            {
#if gAppUseTimeService_d

                if (mOutCharReadByteCount > 2)
                {
                    ctsDayDateTime_t time;
                    uint8_t *pValue = &mOutCharReadBuffer[3];

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

#endif /* gAppUseTimeService_d */
            }
            break;

            default:
            {
                shell_write("\r\nWarning: Unhandled GATT Client Procedure:");
                shell_write(" 0x");
                shell_writeHex((uint8_t *) & (procedureType), 1);
                shell_write("\r\n");
            }
            break;
        }

        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcComplete_c);
    }
    else
    {
        ; /* For MISRA compliance */
    }
}


static void BleApp_HandleValueWriteConfirmations(deviceId_t  deviceId)
{
    /* Handle all command confirmations here - only for commands written
     * to the ANCS Control Point Characteristic. */
    switch (ancsClientData.lastCommandSentToAncsProvider)
    {
        case gAncsCmdIdNoCommand_c:
            /* MISRA compliance */
            break;

        default:
            ; /* Do nothing here. */
            break;
    };
}


static void BleApp_GattNotificationCallback
(
    deviceId_t  serverDeviceId,
    uint16_t    characteristicValueHandle,
    uint8_t    *aValue,
    uint16_t    valueLength
)
{
    BleApp_AttributeNotified(serverDeviceId,
                             characteristicValueHandle,
                             aValue,
                             valueLength);
}


static void BleApp_AttributeNotified
(
    deviceId_t  deviceId,
    uint16_t    handle,
    uint8_t    *pValue,
    uint16_t    length
)
{
    if (mPeerInformation.appState == mAppRunning_c)
    {
        if (handle == mPeerInformation.customInfo.ancsClientConfig.hNotificationSource)
        {
            AncsClient_ProcessNsNotification(deviceId, pValue, length);
            BleApp_StateMachineHandler(deviceId, mAppEvt_AncsNsNotificationReceived_c);
        }
        else if (handle == mPeerInformation.customInfo.ancsClientConfig.hDataSource)
        {
            AncsClient_ProcessDsNotification(deviceId, pValue, length);
            BleApp_StateMachineHandler(deviceId, mAppEvt_AncsDsNotificationReceived_c);
        }
        else
        {
            ; /* For MISRA compliance */
        }
    }
}


static void BleApp_ServiceDiscoveryReset(void)
{
    if (mpServiceDiscoveryBuffer != NULL)
    {
        (void)MEM_BufferFree(mpServiceDiscoveryBuffer);
        mpServiceDiscoveryBuffer = NULL;
    }

    if (mpCharDiscoveryBuffer != NULL)
    {
        (void)MEM_BufferFree(mpCharDiscoveryBuffer);
        mpCharDiscoveryBuffer = NULL;
    }

    if (mpCharDescriptorBuffer != NULL)
    {
        (void)MEM_BufferFree(mpCharDescriptorBuffer);
        mpCharDescriptorBuffer = NULL;
    }
}

static void BleApp_ServiceDiscoveryErrorHandler(void)
{
    mPeerInformation.appState = mAppIdle_c;
    BleApp_ServiceDiscoveryReset();
}


static void BleApp_ServiceDiscoveryCompleted(void)
{
    BleApp_ServiceDiscoveryReset();

    if (0U == mPeerInformation.customInfo.ancsClientConfig.hService)
    {
        shell_write("\r\nWarning: ANCS Service not found!\r\n");
    }

    if (0U == mPeerInformation.customInfo.ancsClientConfig.hNotificationSource)
    {
        shell_write("\r\nWarning: ANCS Notification Source not found!\r\n");
    }

    if (0U != mPeerInformation.customInfo.ancsClientConfig.hNotificationSourceCccd)
    {
        mpDescProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + 23U);
        /* Set notification bit for a CCCD descriptor. */
        uint16_t value = gCccdNotification_c;

        if (NULL == mpDescProcBuffer)
        {
            panic(0, 0, 0, 0);
        }
        else
        {
            /* Moving to the ANCS Notification Source Descriptor Setup State */
            mPeerInformation.appState = mAppNsDescriptorSetup_c;
            /* Enable notifications for the ANCS Notification Source characteristic. */
            mpDescProcBuffer->handle = mPeerInformation.customInfo.ancsClientConfig.hNotificationSourceCccd;
            mpDescProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
            mpDescProcBuffer->valueLength = sizeof(uint16_t);
            mpDescProcBuffer->paValue = (uint8_t *)&value;
            (void)GattClient_WriteCharacteristicDescriptor(mPeerInformation.deviceId, mpDescProcBuffer, sizeof(uint16_t),
                    (uint8_t *)&value);
            shell_write("\r\nWriting ANCS Notification Source CCCD...\r\n");
        }
    }
    else
    {
        shell_write("\r\nWarning: ANCS Notification Source CCCD not found!\r\n");
    }

    if (0U == mPeerInformation.customInfo.ancsClientConfig.hControlPoint)
    {
        shell_write("\r\nWarning: ANCS Control Point not found!\r\n");
    }

    if (0U == mPeerInformation.customInfo.ancsClientConfig.hDataSource)
    {
        shell_write("\r\nWarning: ANCS Data Source not found!\r\n");
    }

    if (0U == mPeerInformation.customInfo.ancsClientConfig.hDataSourceCccd)
    {
        shell_write("\r\nWarning: ANCS Data Source CCCD not found!\r\n");
    }
}

static void BleApp_StateMachineHandlerExchangeMtu(deviceId_t peerDeviceId, uint8_t event)
{
    if (event == mAppEvt_GattProcComplete_c)
    {
        /* Check if required service characteristic discoveries by the client app have been done
         * and change the client application state accordingly. */
        if ((mPeerInformation.customInfo.ancsClientConfig.hNotificationSource == gGattDbInvalidHandle_d)        ||
            (mPeerInformation.customInfo.ancsClientConfig.hNotificationSourceCccd == gGattDbInvalidHandle_d)    ||
            (mPeerInformation.customInfo.ancsClientConfig.hControlPoint == gGattDbInvalidHandle_d)              ||
            (mPeerInformation.customInfo.ancsClientConfig.hDataSource == gGattDbInvalidHandle_d)                ||
            (mPeerInformation.customInfo.ancsClientConfig.hDataSourceCccd == gGattDbInvalidHandle_d)
           )
        {
            /* Allocate memory for Service Discovery */
            mpServiceDiscoveryBuffer = MEM_BufferAlloc(sizeof(gattService_t) * gMaxServicesCount_d);
            mpCharDiscoveryBuffer = MEM_BufferAlloc(sizeof(gattCharacteristic_t) * gMaxServiceCharCount_d);
            mpCharDescriptorBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) * gMaxCharDescriptorsCount_d);

            if ((NULL == mpServiceDiscoveryBuffer) || (NULL == mpCharDiscoveryBuffer) || (NULL == mpCharDescriptorBuffer))
            {
                panic(0, 0, 0, 0);
            }
            else
            {
                /* Moving to Primary Service Discovery State */
                mPeerInformation.appState = mAppPrimaryServiceDisc_c;

                /* Start Service Discovery*/
                (void)GattClient_DiscoverAllPrimaryServices(
                    peerDeviceId,
                    mpServiceDiscoveryBuffer,
                    gMaxServicesCount_d,
                    &mcPrimaryServices);
            }
        }
        else
        {
            /* Set notification bit for a CCCD descriptor. */
            uint16_t value = gCccdNotification_c;

            if (NULL == mpDescProcBuffer)
            {
                panic(0, 0, 0, 0);
            }
            else
            {
                /* Moving to the ANCS Notification Source Descriptor Setup State */
                mPeerInformation.appState = mAppNsDescriptorSetup_c;
                /* Enable notifications for the ANCS Notification Source characteristic. */
                mpDescProcBuffer->handle = mPeerInformation.customInfo.ancsClientConfig.hNotificationSourceCccd;
                mpDescProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
                (void)GattClient_WriteCharacteristicDescriptor(peerDeviceId, mpDescProcBuffer, sizeof(uint16_t),
                        (uint8_t *)&value);
                shell_write("\r\nWriting ANCS Notification Source CCCD...\r\n");
            }
        }
    }
    else if (event == mAppEvt_GattProcError_c)
    {
        shell_write("\r\nWarning: ATT MTU Exchange failed!\r\n");
        BleApp_ServiceDiscoveryErrorHandler();
    }
    else
    {
        ; /* Other events are not handled in this state */
    }
}

static void BleApp_StateMachineHandlerCharServiceDisc(deviceId_t peerDeviceId, uint8_t event)
{
    if (event == mAppEvt_GattProcComplete_c)
    {
        bool_t               earlyReturn = FALSE;
        gattService_t        *pCurrentService = mpServiceDiscoveryBuffer + mCurrentServiceInDiscoveryIndex;
        gattCharacteristic_t *pCurrentChar = pCurrentService->aCharacteristics + mCurrentCharInDiscoveryIndex;

        if (mCurrentCharInDiscoveryIndex < pCurrentService->cNumCharacteristics)
        {
            /* Find next characteristic with descriptors*/
            while (mCurrentCharInDiscoveryIndex < pCurrentService->cNumCharacteristics - 1U)
            {
                /* Check if we have handles available between adjacent characteristics */
                if (pCurrentChar->value.handle + 2U < (pCurrentChar + 1)->value.handle)
                {
                    FLib_MemSet(mpCharDescriptorBuffer, 0, sizeof(gattAttribute_t));
                    pCurrentChar->aDescriptors = mpCharDescriptorBuffer;
                    (void)GattClient_DiscoverAllCharacteristicDescriptors(peerDeviceId,
                            pCurrentChar,
                            (pCurrentChar + 1)->value.handle,
                            gMaxCharDescriptorsCount_d);
                    earlyReturn = TRUE;
                    break;
                }
                else
                {
                    mCurrentCharInDiscoveryIndex++;
                    pCurrentChar = pCurrentService->aCharacteristics + mCurrentCharInDiscoveryIndex;
                }
            }

            /* Made it to the last characteristic. Check against service end handle. */
            if (!earlyReturn && (pCurrentChar->value.handle < pCurrentService->endHandle))
            {
                FLib_MemSet(mpCharDescriptorBuffer, 0, sizeof(gattAttribute_t));
                pCurrentChar->aDescriptors = mpCharDescriptorBuffer;
                (void)GattClient_DiscoverAllCharacteristicDescriptors(peerDeviceId,
                        pCurrentChar,
                        pCurrentService->endHandle,
                        gMaxCharDescriptorsCount_d);
                earlyReturn = TRUE;
            }
        }

        if (!earlyReturn)
        {
            /* Move on to the next service */
            mCurrentServiceInDiscoveryIndex++;

            /* Reset characteristic discovery */
            mCurrentCharInDiscoveryIndex = 0;

            if (mCurrentServiceInDiscoveryIndex < mcPrimaryServices)
            {
                /* Allocate memory for Char Discovery */
                (mpServiceDiscoveryBuffer + mCurrentServiceInDiscoveryIndex)->aCharacteristics = mpCharDiscoveryBuffer;

                /* Start Characteristic Discovery for current service */
                (void)GattClient_DiscoverAllCharacteristicsOfService(peerDeviceId,
                        mpServiceDiscoveryBuffer + mCurrentServiceInDiscoveryIndex,
                        gMaxServiceCharCount_d);
            }
            else
            {
                shell_write("\r\nService discovery completed.\r\n");
                BleApp_ServiceDiscoveryCompleted();
            }
        }
    }
    else if (event == mAppEvt_GattProcError_c)
    {
        BleApp_ServiceDiscoveryErrorHandler();
        shell_write("\r\nService discovery error!!!\r\n");
    }
    else
    {
        ; /* Other events are not handled in this state */
    }
}

static void BleApp_StateMachineHandlerPrimaryServiceDisc(deviceId_t peerDeviceId, uint8_t event)
{
    if (event == mAppEvt_GattProcComplete_c)
    {
        /* We found at least one service */
        if (mcPrimaryServices > 0U)
        {
            /* Moving to Characteristic Discovery State*/
            mPeerInformation.appState = mAppCharServiceDisc_c;

            /* Start characteristic discovery with first service*/
            mCurrentServiceInDiscoveryIndex = 0;
            mCurrentCharInDiscoveryIndex = 0;

            mpServiceDiscoveryBuffer->aCharacteristics = mpCharDiscoveryBuffer;

            /* Start Characteristic Discovery for current service */
            (void)GattClient_DiscoverAllCharacteristicsOfService(
                peerDeviceId,
                mpServiceDiscoveryBuffer,
                gMaxServiceCharCount_d);
        }
    }
    else if (event == mAppEvt_GattProcError_c)
    {
        BleApp_ServiceDiscoveryErrorHandler();
    }
    else
    {
        ; /* Other events are not handled in this state */
    }
}

static void BleApp_StateMachineHandlerNsDescriptorSetup(deviceId_t peerDeviceId, uint8_t event)
{
    if (event == mAppEvt_GattProcComplete_c)
    {
        if (mPeerInformation.customInfo.ancsClientConfig.hNotificationSourceCccd > 0U)
        {
            /* Set notification bit for a CCCD descriptor.  */
            uint16_t value = gCccdNotification_c;

            shell_write("\r\nANCS Notification Source CCCD written successfully.\r\n");

            if (NULL == mpDescProcBuffer)
            {
                panic(0, 0, 0, 0);
            }
            else
            {
                /* Moving to the ANCS Data Source Descriptor Setup State */
                mPeerInformation.appState = mAppDsDescriptorSetup_c;
                /* Enable notifications for the ANCS Data Source characteristic. */
                mpDescProcBuffer->handle = mPeerInformation.customInfo.ancsClientConfig.hDataSourceCccd;
                mpDescProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
                (void)GattClient_WriteCharacteristicDescriptor(peerDeviceId, mpDescProcBuffer, sizeof(uint16_t),
                        (uint8_t *)&value);
                shell_write("\r\nWriting ANCS Data Source CCCD...\r\n");
            }
        }
    }
    else if (event == mAppEvt_GattProcError_c)
    {
        shell_write("\r\nWarning: Could not write ANCS Notification Source CCCD.\r\n");
    }
    else if (event == mAppEvt_PairingComplete_c)
    {
        /* Continue after pairing is complete */

        /* Set notification bit for a CCCD descriptor.  */
        uint16_t value = gCccdNotification_c;

        if (NULL == mpDescProcBuffer)
        {
            panic(0, 0, 0, 0);
        }
        else
        {
            /* Enable notifications for the ANCS Notification Source characteristic. */
            mpDescProcBuffer->handle = mPeerInformation.customInfo.ancsClientConfig.hNotificationSourceCccd;
            mpDescProcBuffer->uuid.uuid16 = gBleSig_CCCD_d;
            (void)GattClient_WriteCharacteristicDescriptor(peerDeviceId, mpDescProcBuffer, sizeof(uint16_t),
                    (uint8_t *)&value);
            shell_write("\r\nWriting ANCS Notification Source CCCD...\r\n");
        }
    }
    else
    {
        ; /* Other events are not handled in this state */
    }
}

void BleApp_StateMachineHandler(deviceId_t peerDeviceId, uint8_t event)
{
    switch (mPeerInformation.appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
                /* Moving to Exchange MTU State */
                mPeerInformation.appState = mAppExchangeMtu_c;
                (void)GattClient_ExchangeMtu(peerDeviceId);
            }
        }
        break;

        case mAppExchangeMtu_c:
        {
            BleApp_StateMachineHandlerExchangeMtu(peerDeviceId, event);
        }
        break;

        case mAppPrimaryServiceDisc_c:
        {
            BleApp_StateMachineHandlerPrimaryServiceDisc(peerDeviceId, event);
        }
        break;

        case mAppCharServiceDisc_c:
        {
            BleApp_StateMachineHandlerCharServiceDisc(peerDeviceId, event);
        }
        break;

        case mAppNsDescriptorSetup_c:
        {
            BleApp_StateMachineHandlerNsDescriptorSetup(peerDeviceId, event);
        }
        break;

        case mAppDsDescriptorSetup_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                if (mPeerInformation.customInfo.ancsClientConfig.hDataSourceCccd > 0U)
                {
                    /* Moving to Running State*/
                    mPeerInformation.appState = mAppRunning_c;

                    shell_write("\r\nANCS Data Source CCCD written successfully.\r\n");
                }
            }
            else if (event == mAppEvt_GattProcError_c)
            {
                shell_write("\r\nWarning: Could not write ANCS Data Source CCCD.\r\n");
            }
            else
            {
                ; /* Other events are not handled in this state */
            }

            break;
        }

        case mAppRunning_c:
        {
            if (event == mAppEvt_GattProcComplete_c)
            {
                /* Write data in NVM */
                (void)Gap_SaveCustomPeerInformation(mPeerInformation.deviceId,
                                                    (void *) &mPeerInformation.customInfo, 0,
                                                    sizeof(appCustomInfo_t));
            }
            else if (event == mAppEvt_AncsNsNotificationReceived_c)
            {
                AncsClient_DisplayNotifications();
            }
            else if (event == mAppEvt_AncsDsNotificationReceived_c)
            {
                AncsClient_DisplayNotifications();
            }
            else
            {
                ; /* Other events are not handled in this state */
            }
        }
        break;

        default:
            ; /* For MISRA compliance */
            break;
    }
}

/*! *********************************************************************************
* \brief    Handles disconnections from the ANCS Server.
*
* \param[in]    deviceId    Device which was disconnected.
********************************************************************************** */
static void AncsClient_HandleDisconnectionEvent(deviceId_t deviceId)
{
    (void) deviceId;

    AncsClient_ResetNotificationData();
}

/*! *********************************************************************************
* \brief    Resets the application notification data.
*
********************************************************************************** */
static void AncsClient_ResetNotificationData(void)
{
    uint32_t i;

    ancsClientData.lastCommandSentToAncsProvider = gAncsCmdIdNoCommand_c;
    ancsClientData.notificationDataChanged = FALSE;

    for (i = 0; i < mMaxDisplayNotifications_c; i++)
    {
        /* Invalidate all notification entries and the associated application names.
         * If the information is sensitive all other fields need to be cleared. */
        ancsClientData.notifications[i].slotUsed = FALSE;
        ancsClientData.notifications[i].appNameValid = FALSE;
        ancsClientData.notifications[i].appName[0] = 0; /* Invalidate app name information */
    }
}


/*! *********************************************************************************
* \brief    Handles BLE notifications from the ANCS Notification Source Characteristic
*           on the ANCS Server.
*
* \param[in]    deviceId        Id of the device from which the notification was received
* \param[in]    pNsData         Pointer to the ANCS Notification Source BLE notification payload
* \param[in]    nsDataLength    Length of the ANCS Notification Source BLE notification payload
********************************************************************************** */
static void AncsClient_ProcessNsNotification
(
    deviceId_t   deviceId,
    uint8_t     *pNsData,
    uint16_t     nsDataLength
)
{
    pPayloadPacketTemp_t pPayloadPacketTemp;
    pPayloadPacketTemp.pNsDataTemp = pNsData;
    ancsNsPacket_t     *pNsPacket = pPayloadPacketTemp.pNsPacketTemp;
    ancsNsEvent_t       nsEvent;

    if (nsDataLength <= gAncsNsPacketMaxLength)
    {
        uint32_t    i;
        uint32_t     notifUidIndex               = mMaxDisplayNotifications_c;
        uint32_t     firstFreeSlotIndex          = mMaxDisplayNotifications_c;
        uint32_t     lowestNotifUidSlotIndex     = mMaxDisplayNotifications_c;

        bool_t      getNotificationAttributes   = FALSE;

        nsEvent.eventId = (ancsEventId_t)(pNsPacket->eventId);
        nsEvent.eventFlags = pNsPacket->eventFlags;
        nsEvent.categoryId = (ancsCatId_t)(pNsPacket->categoryId);
        nsEvent.categoryCount = pNsPacket->categoryCount;
        (void)nsEvent.categoryCount;
        FLib_MemCpy((uint8_t *)(&(nsEvent.notifUid)), (uint8_t *)(&(pNsPacket->notifUid)), sizeof(nsEvent.notifUid));

        for (i = 0; i < mMaxDisplayNotifications_c; i++)
        {
            /* Walk through the existing notifications table and determine the index of the first free slot if available,
             * the index of the oldest notification and the index of a notification matching the received UID. */
            if (ancsClientData.notifications[i].slotUsed == TRUE)
            {
                if (nsEvent.notifUid == ancsClientData.notifications[i].notifUid)
                {
                    notifUidIndex = i;
                }

                if (lowestNotifUidSlotIndex < mMaxDisplayNotifications_c)
                {
                    if (ancsClientData.notifications[i].notifUid < ancsClientData.notifications[lowestNotifUidSlotIndex].notifUid)
                    {
                        lowestNotifUidSlotIndex = i;
                    }
                }
                else
                {
                    lowestNotifUidSlotIndex = i;
                }
            }
            else
            {
                if (firstFreeSlotIndex == mMaxDisplayNotifications_c)
                {
                    firstFreeSlotIndex = i;
                }
            }
        }

        /* Check if this is and existing notification which needs to be updated or it is a
         * new notification going to a free slot or overwriting an old notification
         * and act accordingly. */
        if (notifUidIndex < mMaxDisplayNotifications_c)
        {
            /* Update existing notification */
            if ((nsEvent.eventId == gAncsEventIdNotificationAdded_c) || (nsEvent.eventId == gAncsEventIdNotificationModified_c))
            {
                ancsClientData.notifications[notifUidIndex].notifCat = nsEvent.categoryId;
                ancsClientData.notifications[notifUidIndex].notifFlags = (ancsEventFlag_t)nsEvent.eventFlags;
                ancsClientData.notifications[notifUidIndex].appNameValid = FALSE;
                ancsClientData.notifications[notifUidIndex].appName[0] = 0; /* Invalidate the application display name */

                ancsClientData.notificationDataChanged = TRUE;

                /* Read more notification data from the server for this Uid. */
                getNotificationAttributes = TRUE;
            }
            else if (nsEvent.eventId == gAncsEventIdNotificationRemoved_c)
            {
                ancsClientData.notifications[notifUidIndex].slotUsed = FALSE;
                ancsClientData.notifications[notifUidIndex].appNameValid = FALSE;
                ancsClientData.notifications[notifUidIndex].appName[0] = 0; /* Invalidate the application display name */

                ancsClientData.notificationDataChanged = TRUE;
            }
            else
            {
                ; /* For MISRA compliance */
            }
        }
        else
        {
            /* Only update the notifications table if this notification exists on the ANCS Client
             * and it is not an old notification being removed from the ANCS Server. */
            if (nsEvent.eventId != gAncsEventIdNotificationRemoved_c)
            {
                if (firstFreeSlotIndex < mMaxDisplayNotifications_c)
                {
                    /* Fill the free notification slot. */
                    notifUidIndex = firstFreeSlotIndex;
                }
                else
                {
                    /* Put the in the place of the oldest notification. */
                    notifUidIndex = lowestNotifUidSlotIndex;
                }

                ancsClientData.notifications[notifUidIndex].slotUsed = TRUE;
                ancsClientData.notifications[notifUidIndex].notifCat = nsEvent.categoryId;
                ancsClientData.notifications[notifUidIndex].notifFlags = (ancsEventFlag_t)nsEvent.eventFlags;
                ancsClientData.notifications[notifUidIndex].notifUid = nsEvent.notifUid;
                ancsClientData.notifications[notifUidIndex].appNameValid = FALSE;
                ancsClientData.notifications[notifUidIndex].appName[0] = 0; /* Invalidate the application display name */

                ancsClientData.notificationDataChanged = TRUE;

                /* Read more notification data from the server for this Uid. */
                getNotificationAttributes = TRUE;
            }
        }

        if (getNotificationAttributes == TRUE)
        {
            /* Prepare and send a Get Notification Attributes command requesting the Application ID */
            uint8_t ancsGetNotifAttrCmd[gAncsCmdIdFieldLength_c + gAncsNotifUidFieldLength_c + gAncsAttrIdFieldLength_c] = {0};

            ancsGetNotifAttrCmd[0] = (uint8_t)gAncsCmdIdGetNotificationAttributes_c;
            FLib_MemCpy(&(ancsGetNotifAttrCmd[1]),
                        (uint8_t *)(&(nsEvent.notifUid)),
                        gAncsNotifUidFieldLength_c);
            ancsGetNotifAttrCmd[5] = (uint8_t)gAncsNotifAttrIdAppIdentifier_c;
            AncsClient_SendCommandToAncsServer(deviceId, ancsGetNotifAttrCmd, (uint16_t)sizeof(ancsGetNotifAttrCmd));
        }
    }
    else
    {
        shell_write("\r\nWarning: Received ANCS NS notification with unexpected length!\r\n");
    }
}


/*! *********************************************************************************
* \brief    Handles disconnections from the ANCS Server.
*
* \param[in]    ancsServerDevId     ANCS Server device to send the command to.
* \param[in]    pCommand            Pointer to the command payload.
* \param[in]    cmdLength           Command payload length.
********************************************************************************** */
static void AncsClient_SendCommandToAncsServer
(
    deviceId_t  ancsServerDevId,
    void       *pCommand,
    uint16_t    cmdLength
)
{
    /* GATT Characteristic to be written - ANCS Control Point */
    gattCharacteristic_t    ancsCtrlPointChar;
    bleResult_t             bleResult;

    /* Only the value handle element of this structure is relevant for this operation. */
    ancsCtrlPointChar.value.handle = mPeerInformation.customInfo.ancsClientConfig.hControlPoint;
    ancsCtrlPointChar.value.valueLength = 0;
    ancsCtrlPointChar.cNumDescriptors = 0;
    ancsCtrlPointChar.aDescriptors = NULL;

    bleResult = GattClient_SimpleCharacteristicWrite(ancsServerDevId,
                &ancsCtrlPointChar,
                cmdLength,
                pCommand);

    if (gBleSuccess_c == bleResult)
    {
        ancsClientData.lastCommandSentToAncsProvider = (ancsComdId_t)(*((uint8_t *)pCommand));
    }
    else
    {
        /*! A BLE error has occurred - Disconnect */
        (void)Gap_Disconnect(ancsServerDevId);
    }
}

/*! *********************************************************************************
* \brief    Handles BLE notification attributes from the ANCS Data Source Characteristic
*           on the ANCS Server.
*
* \param[in]    deviceId        Id of the device from which the notification was received
* \param[in]    pDsData         Pointer to the ANCS Data Source BLE notification payload
* \param[in]    dsDataLength    Length of the ANCS Data Source BLE notification payload
********************************************************************************** */
static void AncsClient_ProcessDsNotifAttributes
(
    deviceId_t   deviceId,
    uint8_t     *pDsData,
    uint16_t     dsDataLength
)
{
    uint32_t    i;
    uint32_t    notifUid            = 0xFFFF; /* Initialize to an unlikely notification Uid */
    uint8_t     notifUidIndex       = mMaxDisplayNotifications_c;
    bool_t      getAppAttributes    = FALSE;

    if (dsDataLength >= gAncsCmdIdFieldLength_c + gAncsNotifUidFieldLength_c)
    {
        /* Get the Notification Uid */
        FLib_MemCpy((uint8_t *)(&(notifUid)), &(pDsData[1]), sizeof(notifUid));

        /* Search through the notifications table to find the entry corresponding to the received notification Uid */
        for (i = 0; i < mMaxDisplayNotifications_c; i++)
        {
            if (ancsClientData.notifications[i].slotUsed == TRUE)
            {
                if (notifUid == ancsClientData.notifications[i].notifUid)
                {
                    notifUidIndex = (uint8_t)i;
                    break;
                }
            }
        }

        /* If a corresponding notification Uid was found in the table, perform required actions.
         * Otherwise just ignore the message, the corresponding notification must have been deleted. */
        if (notifUidIndex < mMaxDisplayNotifications_c)
        {
            uint16_t    appIdLength = 0;
            uint8_t    *pAppId      = NULL;

            uint32_t    dsDataIdx   = gAncsCmdIdFieldLength_c + gAncsNotifUidFieldLength_c; /* Initialize the DS data index at the start of the attributes */

            while (dsDataIdx < dsDataLength)
            {
                ancsNotifAttrId_t   notifAttrId         = gAncsNotifAttrIdInvalid;
                uint16_t            notifAttrLength     = 0;
                uint8_t            *pNotifAttrPayload   = NULL;

                notifAttrId = (ancsNotifAttrId_t)(pDsData[dsDataIdx]); /* At the current index should be an attribute ID */
                dsDataIdx = dsDataIdx + gAncsAttrIdFieldLength_c;

                /* Check if the payload contains the Attribute length field. */
                if (dsDataIdx + gAncsAttrLengthFieldLength_c <= dsDataLength)
                {
                    FLib_MemCpy((uint8_t *)(&(notifAttrLength)), &(pDsData[dsDataIdx]), sizeof(notifAttrLength));
                    dsDataIdx = dsDataIdx + gAncsAttrLengthFieldLength_c;
                }
                else
                {
                    shell_write("\r\nWarning: Received ANCS DS notification (Notif Attr) with unexpected length (Notif Attr Length)!\r\n");
                    break; /* Exit the while loop, the packet is malformed */
                }

                /* Check if the payload contains the Attribute payload field. */
                if (dsDataIdx + notifAttrLength <= dsDataLength)
                {
                    pNotifAttrPayload = &(pDsData[dsDataIdx]);
                    dsDataIdx = dsDataIdx + notifAttrLength;
                }
                else
                {
                    shell_write("\r\nWarning: Received ANCS DS notification (Notif Attr) with unexpected length (Notif Attr Payload)!\r\n");
                    break; /* Exit the while loop, the packet is malformed */
                }

                /* Process the notification attribute if it is expected. */
                if (notifAttrId == gAncsNotifAttrIdAppIdentifier_c) /* Application identifier */
                {
                    /* Save the application identifier into the application name buffer, at least the part that fits, zero terminated. */
                    if (notifAttrLength < mMaxNotifAppNameDisplayLength_c)
                    {
                        FLib_MemCpy((uint8_t *)(ancsClientData.notifications[notifUidIndex].appName),
                                    pNotifAttrPayload,
                                    notifAttrLength);
                        ancsClientData.notifications[notifUidIndex].appName[notifAttrLength] = 0x00;
                    }
                    else
                    {
                        FLib_MemCpy((uint8_t *)(ancsClientData.notifications[notifUidIndex].appName),
                                    pNotifAttrPayload,
                                    mMaxNotifAppNameDisplayLength_c - 1U);
                        ancsClientData.notifications[notifUidIndex].appName[mMaxNotifAppNameDisplayLength_c - 1U] = 0x00;
                    }

                    appIdLength = notifAttrLength;
                    pAppId      = pNotifAttrPayload;

                    /* Read more app data from the server. */
                    getAppAttributes = TRUE;
                }
                else
                {
                    shell_write("\r\nWarning: Unhandled Notification Attribute:");
                    shell_write(" 0x");
                    shell_writeHex((uint8_t *) & (notifAttrId), 1);
                    shell_write("\r\n");
                }
            }

            /* Read more app data from the server. */
            if (getAppAttributes == TRUE)
            {
                /* Prepare and send a Get Application Attributes command requesting the Application ID */
                if (appIdLength > 0U)
                {
                    uint8_t     ancsGetAppAttrCmd[gAncsCmdIdFieldLength_c + gAncsAttrIdFieldLength_c + 40U] = {0}; /* 40 bytes reserved space for the app identifier */
                    uint16_t    cmdLength = (uint16_t)(gAncsCmdIdFieldLength_c + appIdLength + 1U + gAncsAttrIdFieldLength_c); /* +1 for the NULL terminated app ID */

                    if ((cmdLength) <= sizeof(ancsGetAppAttrCmd))
                    {
                        ancsGetAppAttrCmd[0] = (uint8_t)gAncsCmdIdGetAppAttributes_c;
                        FLib_MemCpy(&(ancsGetAppAttrCmd[1]),
                                    pAppId,
                                    appIdLength);
                        ancsGetAppAttrCmd[gAncsCmdIdFieldLength_c + appIdLength] = 0x00; /* NULL Terminate the App Id string */
                        ancsGetAppAttrCmd[gAncsCmdIdFieldLength_c + appIdLength + 1U] = (uint8_t)gAncsAppAttrIdDisplayName_c;
                        AncsClient_SendCommandToAncsServer(deviceId, ancsGetAppAttrCmd, cmdLength);
                    }
                    else
                    {
                        shell_write("\r\nWarning: Could not send a Get App Attributes Command, the command buffer is not large enough!\r\n");
                    }
                }
            }
        }
    }
    else
    {
        shell_write("\r\nWarning: Received ANCS DS notification (Notif Attr) with unexpected length (CmdID + Notif UId)!\r\n");
    }
}

/*! *********************************************************************************
* \brief    Handles BLE notification application information from the ANCS Data Source
*           Characteristic on the ANCS Server.
*
* \param[in]    pDsData         Pointer to the ANCS Data Source BLE notification payload
* \param[in]    dsDataLength    Length of the ANCS Data Source BLE notification payload
********************************************************************************** */
static void AncsClient_ProcessDsNotifAppInfo(uint8_t     *pDsData,
        uint16_t     dsDataLength)
{
    uint32_t    i;
    uint16_t    rcvdAppIdLength = 0U;
    uint8_t    *pRcvdAppId      = NULL;

    if (dsDataLength < gAncsCmdIdFieldLength_c)
    {
        shell_write("\r\nWarning: Received ANCS DS notification (App Attr) with unexpected length (CmdID)!\r\n");
    }
    else
    {
        /* Get the App Id NULL terminated string in the received notification if it exists. */
        i = gAncsCmdIdFieldLength_c;

        while (i < dsDataLength)
        {
            if (pDsData[i] == 0x00U)
            {
                rcvdAppIdLength = (uint16_t)(i - gAncsCmdIdFieldLength_c); /* Do not count the just found NULL string terminator. */
                pRcvdAppId = (uint8_t *)(&(pDsData[gAncsCmdIdFieldLength_c]));
                break; /* Break the while loop, the App Id length may have been identified. */
            }

            i = i + 1U;
        }

        if (rcvdAppIdLength == 0U)
        {
            shell_write("\r\nWarning: Received ANCS DS notification (App Attr) with invalid App Id!\r\n");
        }
        else
        {
            /* Look for the App Id in the notifications table - only entries with an invalid application name field */
            for (i = 0; i < mMaxDisplayNotifications_c; i++)
            {
                if ((ancsClientData.notifications[i].slotUsed == TRUE) && (ancsClientData.notifications[i].appNameValid == FALSE))
                {
                    bool_t      cmpMatch    = FALSE;
                    uint32_t    dsDataIdx   = 0;

                    if (rcvdAppIdLength < mMaxNotifAppNameDisplayLength_c)
                    {
                        cmpMatch = FLib_MemCmp((uint8_t *)(ancsClientData.notifications[i].appName),
                                               pRcvdAppId,
                                               rcvdAppIdLength);
                    }
                    else
                    {
                        cmpMatch = FLib_MemCmp((uint8_t *)(ancsClientData.notifications[i].appName),
                                               pRcvdAppId,
                                               mMaxNotifAppNameDisplayLength_c - 1U);
                    }

                    if (cmpMatch == FALSE)
                    {
                        continue; /* Continue the for loop over the notifications table, the entry does not match the received App Id */
                    }

                    /* Initialize the DS data index at the start of the attributes.
                     * +1 for the NULL string terminator which is not counted in the App Id length */
                    dsDataIdx = gAncsCmdIdFieldLength_c + (uint32_t)rcvdAppIdLength + 1U;

                    while (dsDataIdx < dsDataLength)
                    {
                        ancsAppAttrId_t appAttrId       = gAncsAppAttrIdInvalid_c;
                        uint16_t        appAttrLength   = 0;
                        uint8_t        *pAppAttrPayload = NULL;

                        appAttrId = (ancsAppAttrId_t)(pDsData[dsDataIdx]); /* At the current index should be an app ID */
                        dsDataIdx = dsDataIdx + gAncsAttrIdFieldLength_c;

                        /* Check if the payload contains the Attribute length field. */
                        if (dsDataIdx + gAncsAttrLengthFieldLength_c <= dsDataLength)
                        {
                            FLib_MemCpy((uint8_t *)(&(appAttrLength)), &(pDsData[dsDataIdx]), sizeof(appAttrLength));
                            dsDataIdx = dsDataIdx + gAncsAttrLengthFieldLength_c;
                        }
                        else
                        {
                            shell_write("\r\nWarning: Received ANCS DS notification (App Attr) with unexpected length (App Attr Length)!\r\n");
                            break; /* Exit the while loop, the packet is malformed */
                        }

                        /* Check if the payload contains the Attribute payload field. */
                        if (dsDataIdx + appAttrLength <= dsDataLength)
                        {
                            pAppAttrPayload = &(pDsData[dsDataIdx]);
                            dsDataIdx = dsDataIdx + appAttrLength;
                        }
                        else
                        {
                            shell_write("\r\nWarning: Received ANCS DS notification (App Attr) with unexpected length (App Attr Payload)!\r\n");
                            break; /* Exit the while loop, the packet is malformed */
                        }

                        /* Process the notification attribute if it is expected. */
                        if (appAttrId == gAncsAppAttrIdDisplayName_c) /* Application display name */
                        {
                            /* Save the application display name into the application name buffer, at least the part that fits, zero terminated. */
                            if (appAttrLength < mMaxNotifAppNameDisplayLength_c)
                            {
                                FLib_MemCpy((uint8_t *)(ancsClientData.notifications[i].appName),
                                            pAppAttrPayload,
                                            appAttrLength);
                                ancsClientData.notifications[i].appName[appAttrLength] = 0x00;
                            }
                            else
                            {
                                FLib_MemCpy((uint8_t *)(ancsClientData.notifications[i].appName),
                                            pAppAttrPayload,
                                            mMaxNotifAppNameDisplayLength_c - 1U);
                                ancsClientData.notifications[i].appName[mMaxNotifAppNameDisplayLength_c - 1U] = 0x00;
                            }

                            ancsClientData.notifications[i].appNameValid = TRUE;
                            ancsClientData.notificationDataChanged = TRUE;
                        }
                        else
                        {
                            shell_write("\r\nWarning: Unhandled Application Attribute:");
                            shell_write(" 0x");
                            shell_writeHex((uint8_t *) & (appAttrId), 1);
                            shell_write("\r\n");
                        }
                    } /* while the end of the attributes was not reached */

                    if (dsDataIdx < dsDataLength)
                    {
                        /* The parsing of the packet ended prematurely. It must be malformed. Break the for loop. */
                        break;
                    }

                } /* if notification slot used and app name is not valid */
            } /* for i over the notifications table */
        }
    }
}

/*! *********************************************************************************
* \brief    Handles BLE notifications from the ANCS Data Source Characteristic
*           on the ANCS Server.
*
* \param[in]    deviceId        Id of the device from which the notification was received
* \param[in]    pDsData         Pointer to the ANCS Data Source BLE notification payload
* \param[in]    dsDataLength    Length of the ANCS Data Source BLE notification payload
********************************************************************************** */
static void AncsClient_ProcessDsNotification(deviceId_t   deviceId,
        uint8_t     *pDsData,
        uint16_t     dsDataLength)
{
    switch ((ancsComdId_t)pDsData[0])
    {
        case gAncsCmdIdGetNotificationAttributes_c:
        {
            AncsClient_ProcessDsNotifAttributes(deviceId, pDsData, dsDataLength);
        }
        break;

        case gAncsCmdIdGetAppAttributes_c:
        {
            AncsClient_ProcessDsNotifAppInfo(pDsData, dsDataLength);
        }
        break;

        case gAncsCmdIdPerformNotificationAction_c:
        {
        }
        break;

        default:
        {
            shell_write("\r\nWarning: Unhandled ANCS Data Source Command Id:");
            shell_write(" 0x");
            shell_writeHex(&(pDsData[0]), 1);
            shell_write("\r\n");
        }
        break;
    }
}


/*! *********************************************************************************
* \brief        Formats and displays information from the notifications
*               table if the flag signaling changes is set.
*
********************************************************************************** */
static void AncsClient_DisplayNotifications(void)
{
    if (ancsClientData.notificationDataChanged == TRUE)
    {
        uint32_t n_idx;
        uint32_t c_idx;

        shell_write("\r\n");
        shell_write("Notif_UID "); /* Notification UID header - 10 chars reserved for value */
        shell_write("  ");
        shell_write("Flags"); /* Notification flags - 5 chars reserved for value */
        shell_write("  ");
        shell_write("Category            "); /* Notification category - 20 chars reserved for value */
        shell_write("  ");
        shell_write("Application"); /* Notification application - if known - mMaxNotifAppNameDisplayLength_c reserved */
        shell_write("\r\n");

        for (n_idx = 0; n_idx < mMaxDisplayNotifications_c; n_idx++)
        {
            if (ancsClientData.notifications[n_idx].slotUsed == TRUE)
            {
                /* Write Notification UID - 10 chars reserved for value */
                shell_write("0x");
                shell_writeHex((uint8_t *)(&(ancsClientData.notifications[n_idx].notifUid)) + 3, 1);
                shell_writeHex((uint8_t *)(&(ancsClientData.notifications[n_idx].notifUid)) + 2, 1);
                shell_writeHex((uint8_t *)(&(ancsClientData.notifications[n_idx].notifUid)) + 1, 1);
                shell_writeHex((uint8_t *)(&(ancsClientData.notifications[n_idx].notifUid)) + 0, 1);
                shell_write("  ");

                /* Write Notification Flags - 5 chars reserved for value */
                for (c_idx = 0; c_idx < sizeof(ancsFlagsToLetterTable); c_idx++)
                {
                    if (((uint8_t)(ancsClientData.notifications[n_idx].notifFlags) & (uint8_t)(0x01U << c_idx)) > 0U)
                    {
                        shell_writeN((const char *)(&(ancsFlagsToLetterTable[c_idx])), 1);
                    }
                    else
                    {
                        shell_writeN((const char *)(&mAncsFalgNotSetSymbol), 1);
                    }
                }

                shell_write("  ");

                /* Write Notification category - 20 chars reserved for value */
                if (ancsClientData.notifications[n_idx].notifCat < gAncsCatIdInvalid_c)
                {
                    shell_writeN((const char *)(&(ancsNotifCatToStringTable[ancsClientData.notifications[n_idx].notifCat])),
                                 mMaxNotifCatDisplayLength_c);
                }
                else
                {
                    for (c_idx = 0; c_idx < mMaxNotifCatDisplayLength_c; c_idx++)
                    {
                        shell_writeN((const char *)(&mAncsNotifCatCharPlaceholder), 1);
                    }
                }

                shell_write("  ");

                /* Write application name if it has been obtained otherwise write mMaxNotifAppNameDisplayLength_c _ characters */
                if (ancsClientData.notifications[n_idx].appNameValid == TRUE)
                {
                    shell_write((char *)(ancsClientData.notifications[n_idx].appName)); /* NULL terminated string. */
                }
                else
                {
                    for (c_idx = 0; c_idx < mMaxNotifAppNameDisplayLength_c; c_idx++)
                    {
                        shell_writeN((const char *)(&mAncsAppNameCharPlaceholder), 1);
                    }
                }

                shell_write("\r\n");
            }
        }

        shell_refresh();

        ancsClientData.notificationDataChanged = FALSE;
    }
}


/*! *********************************************************************************
* \brief        Handles advertising timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void AdvertisingTimerCallback(void *pParam)
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
* \brief        Handles current time tick callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void CTSTickTimerCallback(void *pParam)
{
    ctsServiceConfig.localTime++;
    ctsServiceConfig.adjustReason = gCts_UnknownReason;
    (void)Cts_RecordCurrentTime(&ctsServiceConfig);
}

/*! *********************************************************************************
* \brief        Handles time update callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void RTUSReferenceUpdateTimerCallback(void *pParam)
{
    if (rtusServiceConfig.timeUpdateState.currentState == gRtusUpdatePending_c)
    {
        rtusResult_t result = gRtusSuccessful_c;

        /* We simulate an update just for demo purposes */
        rtusServiceConfig.timeUpdateState.currentState = gRtusIdle_c;
        rtusServiceConfig.timeUpdateState.result = result;
        (void)Rtus_RecordTimeUpdateState(&rtusServiceConfig);
        ctsServiceConfig.adjustReason = gCts_ExternalRefUpdate;
        (void)Cts_RecordCurrentTime(&ctsServiceConfig);
    }
}

/*! *********************************************************************************
* \brief        Handles battery measurement timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void BatteryMeasurementTimerCallback(void *pParam)
{
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_RecordBatteryMeasurement(&basServiceConfig);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
