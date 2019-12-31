/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* \file
*
* This is a source file for the main application.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Drv */
#include "LED.h"

/* Fwk */
#include "fsl_os_abstraction.h"
#include "MemManager.h"
#include "TimersManager.h"
#include "RNG_Interface.h"
#include "Messaging.h"
#include "Flash_Adapter.h"
#include "SecLib.h"
#include "Panic.h"

#if defined(gFsciIncluded_c) && (gFsciIncluded_c == 1)
#include "FsciInterface.h"
#if gFSCI_IncludeLpmCommands_c
#include "FsciCommands.h"
#endif
#endif

/* KSDK */
#include "board.h"
#if defined(MULTICORE_CONNECTIVITY_CORE) && (MULTICORE_CONNECTIVITY_CORE)
#include "fsl_cau3.h"
#endif

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#endif

#ifdef FSL_RTOS_FREE_RTOS
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#endif

#include "ApplMain.h"
#if gAppUseNvm_d
#include "NVM_Interface.h"
#if defined(gFsciIncluded_c) && (gFsciIncluded_c == 1)
#include "NV_FsciCommands.h"
#endif
#endif /* gAppUseNvm_d */

#ifdef CPU_QN908X
#include "controller_interface.h"
#include "fsl_wdt.h"
#endif

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) || (gFsciBleTest_d == 1))
#include "fsci_ble_gap.h"
#include "fsci_ble_gatt.h"
#include "fsci_ble_l2cap_cb.h"
#endif

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/* NVM Dataset identifiers */
#if gAppUseNvm_d
#define nvmId_BondingHeaderId_c          0x4011
#define nvmId_BondingDataDynamicId_c     0x4012
#define nvmId_BondingDataStaticId_c      0x4013
#define nvmId_BondingDataDeviceInfoId_c  0x4014
#define nvmId_BondingDataDescriptorId_c  0x4015
#endif

/* Application Events */
#define gAppEvtMsgFromHostStack_c       (1U << 0U)
#define gAppEvtAppCallback_c            (1U << 1U)

#ifdef FSL_RTOS_FREE_RTOS
    #if (configUSE_IDLE_HOOK)
        #define mAppIdleHook_c 1
    #endif

    #if (configUSE_TICKLESS_IDLE != 0)
        #define mAppEnterLpFromIdleTask_c (0)
        #define mAppTaskWaitTime_c        (1000U) /* milliseconds */
        #define mAppOverheadTicks_c       (1U)    /* Application processing overhead in OS ticks */
    #endif
#endif

#ifndef mAppIdleHook_c
#define mAppIdleHook_c 0
#endif

#ifndef mAppEnterLpFromIdleTask_c
#define mAppEnterLpFromIdleTask_c 1
#endif

#ifndef mAppTaskWaitTime_c
#define mAppTaskWaitTime_c (osaWaitForever_c)
#endif

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
#define MULTICORE_STATIC
#else
#define MULTICORE_STATIC STATIC
#endif


/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
/* Host to Application Messages Types */
typedef enum {
    gAppGapGenericMsg_c = 0,
    gAppGapConnectionMsg_c,
    gAppGapAdvertisementMsg_c,
    gAppGapScanMsg_c,
    gAppGattServerMsg_c,
    gAppGattClientProcedureMsg_c,
    gAppGattClientNotificationMsg_c,
    gAppGattClientIndicationMsg_c,
    gAppL2caLeDataMsg_c,
    gAppL2caLeControlMsg_c,
    gAppSecLibMultiplyMsg_c,
}appHostMsgType_t;

/* Host to Application Connection Message */
typedef struct connectionMsg_tag{
    deviceId_t              deviceId;
    gapConnectionEvent_t    connEvent;
}connectionMsg_t;

/* Host to Application GATT Server Message */
typedef struct gattServerMsg_tag{
    deviceId_t          deviceId;
    gattServerEvent_t   serverEvent;
}gattServerMsg_t;

/* Host to Application GATT Client Procedure Message */
typedef struct gattClientProcMsg_tag{
    deviceId_t              deviceId;
    gattProcedureType_t     procedureType;
    gattProcedureResult_t   procedureResult;
    bleResult_t             error;
}gattClientProcMsg_t;

/* Host to Application GATT Client Notification/Indication Message */
typedef struct gattClientNotifIndMsg_tag{
    uint8_t*    aValue;
    uint16_t    characteristicValueHandle;
    uint16_t    valueLength;
    deviceId_t  deviceId;
}gattClientNotifIndMsg_t;

/* L2ca to Application Data Message */
typedef struct l2caLeCbDataMsg_tag{
    deviceId_t  deviceId;
    uint16_t    channelId;
    uint16_t    packetLength;
    uint8_t     aPacket[1];
}l2caLeCbDataMsg_t;

/* Sec Lib to Application Data Message */
typedef struct secLibMsgData_tag{
    computeDhKeyParam_t *pData;
} secLibMsgData_t;

typedef struct appMsgFromHost_tag{
    appHostMsgType_t    msgType;
    union {
        gapGenericEvent_t       genericMsg;
        gapAdvertisingEvent_t   advMsg;
        connectionMsg_t         connMsg;
        gapScanningEvent_t      scanMsg;
        gattServerMsg_t         gattServerMsg;
        gattClientProcMsg_t     gattClientProcMsg;
        gattClientNotifIndMsg_t gattClientNotifIndMsg;
        l2caLeCbDataMsg_t       l2caLeCbDataMsg;
        l2capControlMessage_t   l2caLeCbControlMsg;
        secLibMsgData_t         secLibMsgData;

    } msgData;
}appMsgFromHost_t;

typedef struct appMsgCallback_tag{
    appCallbackHandler_t   handler;
    appCallbackParam_t     param;
}appMsgCallback_t;
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
#if (defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c) || gAppUseNvm_d
#if (mAppIdleHook_c)
    #define AppIdle_TaskInit()
    #define App_Idle_Task()
#else
    static osaStatus_t AppIdle_TaskInit(void);
    static void App_Idle_Task(osaTaskParam_t argument);
#endif
#endif

#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
static void App_KeyboardCallBack(uint8_t events);
#endif

static void App_HandleHostMessageInput(appMsgFromHost_t* pMsg);


MULTICORE_STATIC void App_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
MULTICORE_STATIC void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
MULTICORE_STATIC void App_ScanningCallback (gapScanningEvent_t* pScanningEvent);
MULTICORE_STATIC void App_GattServerCallback (deviceId_t peerDeviceId, gattServerEvent_t* pServerEvent);
MULTICORE_STATIC void App_GattClientProcedureCallback
(
    deviceId_t              deviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);
MULTICORE_STATIC void App_GattClientNotificationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
);
MULTICORE_STATIC void App_GattClientIndicationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
);

MULTICORE_STATIC void App_L2caLeDataCallback
(
    deviceId_t deviceId,
    uint16_t channelId,
    uint8_t* pPacket,
    uint16_t packetLength
);

MULTICORE_STATIC void App_L2caLeControlCallback
(
    l2capControlMessage_t* pMessage
);

MULTICORE_STATIC void App_SecLibMultCallback
(
    computeDhKeyParam_t *pData
);

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)
static void BLE_SignalFromISRCallback(void);
#endif

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && (configUSE_TICKLESS_IDLE != 0)
extern void vTaskStepTick( const TickType_t xTicksToJump );
#endif

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void App_Thread (uint32_t param);

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)
extern void (*pfBLE_SignalFromISR)(void);
#endif /* gUseHciTransportDownward_d */

#if defined(gMWS_Enabled_d) && (gMWS_Enabled_d)
extern void App_Init(void);
#endif

#if !defined(MULTICORE_CONNECTIVITY_CORE) || (!MULTICORE_CONNECTIVITY_CORE)
void App_GenericCallback (gapGenericEvent_t* pGenericEvent);
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if (defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c) || gAppUseNvm_d
#if (!mAppIdleHook_c)
static OSA_TASK_DEFINE( App_Idle_Task, gAppIdleTaskPriority_c, 1, gAppIdleTaskStackSize_c, FALSE );
static osaTaskId_t mAppIdleTaskId = NULL;
#endif
#endif  /* cPWR_UsePowerDownMode */

#if gAppUseNvm_d
#if gUnmirroredFeatureSet_d == TRUE
static bleBondIdentityHeaderBlob_t*  aBondingHeader[gMaxBondedDevices_c];
static bleBondDataDynamicBlob_t*     aBondingDataDynamic[gMaxBondedDevices_c];
static bleBondDataStaticBlob_t*      aBondingDataStatic[gMaxBondedDevices_c];
static bleBondDataDeviceInfoBlob_t*  aBondingDataDeviceInfo[gMaxBondedDevices_c];
static bleBondDataDescriptorBlob_t* aBondingDataDescriptor[gMaxBondedDevices_c * gcGapMaximumSavedCccds_c];

NVM_RegisterDataSet(aBondingHeader, gMaxBondedDevices_c, gBleBondIdentityHeaderSize_c, nvmId_BondingHeaderId_c, (uint16_t)gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataDynamic, gMaxBondedDevices_c, gBleBondDataDynamicSize_c, nvmId_BondingDataDynamicId_c, (uint16_t)gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataStatic, gMaxBondedDevices_c, gBleBondDataStaticSize_c, nvmId_BondingDataStaticId_c, (uint16_t)gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataDeviceInfo, gMaxBondedDevices_c, gBleBondDataDeviceInfoSize_c, nvmId_BondingDataDeviceInfoId_c, (uint16_t)gNVM_NotMirroredInRamAutoRestore_c);
NVM_RegisterDataSet(aBondingDataDescriptor, gMaxBondedDevices_c * gcGapMaximumSavedCccds_c, gBleBondDataDescriptorSize_c, nvmId_BondingDataDescriptorId_c, (uint16_t)gNVM_NotMirroredInRamAutoRestore_c);
#else /* mirrored dataset */
static bleBondIdentityHeaderBlob_t  aBondingHeader[gMaxBondedDevices_c];
static bleBondDataDynamicBlob_t     aBondingDataDynamic[gMaxBondedDevices_c];
static bleBondDataStaticBlob_t      aBondingDataStatic[gMaxBondedDevices_c];
static bleBondDataDeviceInfoBlob_t  aBondingDataDeviceInfo[gMaxBondedDevices_c];
static bleBondDataDescriptorBlob_t  aBondingDataDescriptor[gMaxBondedDevices_c * gcGapMaximumSavedCccds_c];
/* register datasets */
NVM_RegisterDataSet(aBondingHeader, gMaxBondedDevices_c, gBleBondIdentityHeaderSize_c, nvmId_BondingHeaderId_c, (uint16_t)gNVM_MirroredInRam_c);
NVM_RegisterDataSet(aBondingDataDynamic, gMaxBondedDevices_c, gBleBondDataDynamicSize_c, nvmId_BondingDataDynamicId_c, (uint16_t)gNVM_MirroredInRam_c);
NVM_RegisterDataSet(aBondingDataStatic, gMaxBondedDevices_c, gBleBondDataStaticSize_c, nvmId_BondingDataStaticId_c, (uint16_t)gNVM_MirroredInRam_c);
NVM_RegisterDataSet(aBondingDataDeviceInfo, gMaxBondedDevices_c, gBleBondDataDeviceInfoSize_c, nvmId_BondingDataDeviceInfoId_c, (uint16_t)gNVM_MirroredInRam_c);
NVM_RegisterDataSet(aBondingDataDescriptor, gMaxBondedDevices_c * gcGapMaximumSavedCccds_c, gBleBondDataDescriptorSize_c, nvmId_BondingDataDescriptorId_c, (uint16_t)gNVM_MirroredInRam_c);
#endif
#else
static bleBondDataBlob_t          maBondDataBlobs[gMaxBondedDevices_c] = {{{{0}}}};
#if gAppUseNvm_d
NVM_RegisterDataSet(NULL, 0U, 0U, 0U, 0U);
#endif /* gAppUseNvm_d */
#endif

static osaEventId_t  mAppEvent;

static gapGenericCallback_t pfGenericCallback = NULL;
static gapAdvertisingCallback_t pfAdvCallback = NULL;
static gapScanningCallback_t pfScanCallback = NULL;
static gapConnectionCallback_t  pfConnCallback = NULL;
static gattServerCallback_t pfGattServerCallback = NULL;
static gattClientProcedureCallback_t pfGattClientProcCallback = NULL;
static gattClientNotificationCallback_t pfGattClientNotifCallback = NULL;
static gattClientNotificationCallback_t pfGattClientIndCallback = NULL;
static l2caLeCbDataCallback_t           pfL2caLeCbDataCallback = NULL;
static l2caLeCbControlCallback_t        pfL2caLeCbControlCallback = NULL;

/* Application input queues */
static anchor_t mHostAppInputQueue;
static anchor_t mAppCbInputQueue;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#if !defined(gHybridApp_d) || (!gHybridApp_d)
/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize
*         the system
*
* \param[in]  param
*
********************************************************************************** */
void main_task(uint32_t param)
{
    static bool_t platformInitialized = FALSE;

    if (FALSE == platformInitialized)
    {
        platformInitialized = TRUE;

#if !defined(CPU_QN908X) && defined(gDCDC_Enabled_d) && (gDCDC_Enabled_d)
        /* Init DCDC module */
        BOARD_DCDCInit();
#endif

        (void)MEM_Init();

#ifdef CPU_QN908X
        /* Initialize QN9080 Controller */
        BLE_Init(gAppMaxConnections_c);
#endif /* CPU_QN908X */

        /* Framework init */
#if defined(gRngSeedStorageAddr_d) || defined(gXcvrDacTrimValueSorageAddr_d)
        NV_Init();
#endif
        TMR_Init();

        /* Cryptographic and RNG hardware initialization */
#if defined(MULTICORE_CONNECTIVITY_CORE) && (MULTICORE_CONNECTIVITY_CORE)
        /* Make sure the clock is provided by M0+ in order to avoid issues after exiting low power mode from M0+ */
        CLOCK_EnableClock(kCLOCK_Cau3);
#if defined(FSL_CAU3_USE_HW_SEMA) && (FSL_CAU3_USE_HW_SEMA > 0)
        CLOCK_EnableClock(FSL_CAU3_SEMA42_CLOCK_NAME);
#endif /* FSL_CAU3_USE_HW_SEMA */
#else
        SecLib_Init();
#endif

        /* Set external multiplication callback if we don't have support for hardware elliptic curve
         * multiplication */
#if defined(FSL_FEATURE_SOC_CAU3_COUNT) && (FSL_FEATURE_SOC_CAU3_COUNT > 0)
#else
        SecLib_SetExternalMultiplicationCb(App_SecLibMultCallback);
#endif

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
        Board_StartSecondaryCoreApp();
        CLOCK_DisableClock(kCLOCK_Cau3);
#else
        /* RNG software initialization and PRNG initial seeding (from hardware) */
        (void)RNG_Init();
        RNG_SetPseudoRandomNoSeed(NULL);
#endif /* MULTICORE_APPLICATION_CORE */
        LED_Init();
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
        KBD_Init(App_KeyboardCallBack);
#endif

#if gAppUseNvm_d
        /* Initialize NV module */
        (void)NvModuleInit();
#endif

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)
        pfBLE_SignalFromISR = BLE_SignalFromISRCallback;
#endif /* !gUseHciTransportDownward_d */

#if (defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c) || gAppUseNvm_d
#if (!mAppIdleHook_c)
        (void)AppIdle_TaskInit();
#endif
#endif
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
        PWR_Init();
        PWR_DisallowDeviceToSleep();
#else
        Led1Flashing();
        Led2Flashing();
        Led3Flashing();
        Led4Flashing();
#endif

        /* Initialize peripheral drivers specific to the application */
        BleApp_Init();

        /* Create application event */
        mAppEvent = OSA_EventCreate(TRUE);
        if( NULL == mAppEvent )
        {
            panic(0,0,0,0);
            return;
        }

        /* Prepare application input queue.*/
        MSG_InitQueue(&mHostAppInputQueue);

        /* Prepare callback input queue.*/
        MSG_InitQueue(&mAppCbInputQueue);

#if !defined(MULTICORE_CONNECTIVITY_CORE) || (!MULTICORE_CONNECTIVITY_CORE)
        /* BLE Host Stack Init */
        if (Ble_Initialize(App_GenericCallback) != gBleSuccess_c)
        {
            panic(0,0,0,0);
            return;
        }
#endif /* MULTICORE_CONNECTIVITY_CORE */

#if (gAppUseNvm_d && defined(gFsciIncluded_c) && (gFsciIncluded_c))
#if gNvmEnableFSCIMonitoring_c
        NV_SetFSCIMonitoringState(TRUE);
#endif
#if gNvmEnableFSCIRequests_c
        NV_RegisterToFSCI();
#endif
#endif

#if defined(gMWS_Enabled_d) && (gMWS_Enabled_d)
        App_Init();
#endif
    }

    /* Call application task */
    App_Thread( param );
}
#endif /* gHybridApp_d */

/*! *********************************************************************************
* \brief  This function represents the Application task.
*         This task reuses the stack allocated for the MainThread.
*         This function is called to process all events for the task. Events
*         include timers, messages and any other user defined events.
* \param[in]  argument
*
* \remarks  For bare-metal, process only one type of message at a time,
*           to allow other higher priority task to run.
*
********************************************************************************** */
void App_Thread (uint32_t param)
{
#if !defined(gHybridApp_d) || (!gHybridApp_d)
    osaEventFlags_t event = 0U;

    for (;;)
    {
        (void)OSA_EventWait(mAppEvent, osaEventFlagsAll_c, FALSE, mAppTaskWaitTime_c , &event);
#else
    {
#endif /* gHybridApp_d */
        /* Check for existing messages in queue */
        if (MSG_Pending(&mHostAppInputQueue))
        {
            /* Pointer for storing the messages from host. */
            appMsgFromHost_t *pMsgIn = MSG_DeQueue(&mHostAppInputQueue);

            if (pMsgIn != NULL)
            {
                /* Process it */
                App_HandleHostMessageInput(pMsgIn);

                /* Messages must always be freed. */
                (void)MSG_Free(pMsgIn);
            }
        }

        /* Check for existing messages in queue */
        if (MSG_Pending(&mAppCbInputQueue))
        {
            /* Pointer for storing the callback messages. */
            appMsgCallback_t *pMsgIn = MSG_DeQueue(&mAppCbInputQueue);

            if (pMsgIn != NULL)
            {
                /* Execute callback handler */
                if (pMsgIn->handler != NULL)
                {
                    pMsgIn->handler(pMsgIn->param);
                }

                /* Messages must always be freed. */
                (void)MSG_Free(pMsgIn);
            }
        }
#if !defined(gHybridApp_d) || (!gHybridApp_d)
        /* Signal the App_Thread again if there are more messages pending */
        event = MSG_Pending(&mHostAppInputQueue) ? gAppEvtMsgFromHostStack_c : 0U;
        event |= MSG_Pending(&mAppCbInputQueue) ? gAppEvtAppCallback_c : 0U;

        if (event != 0U)
        {
            (void)OSA_EventSet(mAppEvent, gAppEvtAppCallback_c);
        }

        /* For BareMetal break the while(1) after 1 run */
        if( gUseRtos_c == 0U )
        {
            break;
        }
#endif /* gHybridApp_d */
    }
}

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c
static void App_Idle(void)
{
    if( PWR_CheckIfDeviceCanGoToSleep() )
    {
        /* Enter Low Power */
        (void)PWR_EnterLowPower();

#if gFSCI_IncludeLpmCommands_c
        /* Send Wake Up indication to FSCI */
        FSCI_SendWakeUpIndication();
#endif

#if gKBD_KeysCount_c > 0
        /* Woke up on Keyboard Press */
        if(PWRLib_MCU_WakeupReason.Bits.FromKeyBoard != 0U)
        {
#if defined(cPWR_EnableDeepSleepMode_8) && (cPWR_EnableDeepSleepMode_8)
            /* Skip over the key scan timer to improve low power consumption. */
            BleApp_HandleKeys(gKBD_EventPressPB1_c);
#else
            KBD_SwitchPressedOnWakeUp();
#endif
        }
#endif
    }
    else
    {
        /* Enter MCU Sleep */
        PWR_EnterSleep();
    }
}
#endif /* cPWR_UsePowerDownMode */

#if (mAppIdleHook_c)

void vApplicationIdleHook(void)
{
#if (gAppUseNvm_d)
    NvIdle();
#endif
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c
    App_Idle();
#endif
}

#else /* mAppIdleHook_c */

#if (defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c) || gAppUseNvm_d
static void App_Idle_Task(osaTaskParam_t argument)
{
    for (;;)
    {
#if gAppUseNvm_d
        NvIdle();
#endif

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c
        App_Idle();
#endif

        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0U)
        {
            break;
        }
    }
}

static osaStatus_t AppIdle_TaskInit(void)
{
    if(NULL != mAppIdleTaskId)
    {
        return osaStatus_Error;
    }

    /* Task creation */
    mAppIdleTaskId = OSA_TaskCreate(OSA_TASK(App_Idle_Task), NULL);

    if( NULL == mAppIdleTaskId )
    {
        panic(0,0,0,0);
        return osaStatus_Error;
    }

    return osaStatus_Success;
}
#endif /* cPWR_UsePowerDownMode */

#endif /* mAppIdleHook_c */

bleResult_t App_Connect(
    gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t             connCallback
)
{
    pfConnCallback = connCallback;

    return Gap_Connect(pParameters, App_ConnectionCallback);
}

bleResult_t App_StartAdvertising(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback
)
{
    pfAdvCallback = advertisingCallback;
    pfConnCallback = connectionCallback;

    return Gap_StartAdvertising(App_AdvertisingCallback, App_ConnectionCallback);
}

bleResult_t App_StartExtAdvertising(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback,
    uint8_t                     handle,
    uint16_t                    duration,
    uint8_t                     maxExtAdvEvents
)
{
    pfAdvCallback = advertisingCallback;
    pfConnCallback = connectionCallback;

    return Gap_StartExtAdvertising(App_AdvertisingCallback, App_ConnectionCallback, handle, duration, maxExtAdvEvents);
}

bleResult_t App_StartScanning(
    gapScanningParameters_t*    pScanningParameters,
    gapScanningCallback_t       scanningCallback,
    gapFilterDuplicates_t       enableFilterDuplicates,
    uint16_t                    duration,
    uint16_t                    period
)
{
    pfScanCallback = scanningCallback;

    return Gap_StartScanning(pScanningParameters, App_ScanningCallback,  enableFilterDuplicates, duration, period);
}

bleResult_t App_RegisterGattServerCallback(gattServerCallback_t  serverCallback)
{
    pfGattServerCallback = serverCallback;

    return GattServer_RegisterCallback(App_GattServerCallback);
}

bleResult_t App_RegisterGattClientProcedureCallback(gattClientProcedureCallback_t  callback)
{
    pfGattClientProcCallback = callback;

    return GattClient_RegisterProcedureCallback(App_GattClientProcedureCallback);
}

bleResult_t App_RegisterGattClientNotificationCallback(gattClientNotificationCallback_t  callback)
{
    pfGattClientNotifCallback = callback;

    return GattClient_RegisterNotificationCallback(App_GattClientNotificationCallback);
}

bleResult_t App_RegisterGattClientIndicationCallback(gattClientIndicationCallback_t  callback)
{
    pfGattClientIndCallback = callback;

    return GattClient_RegisterIndicationCallback(App_GattClientIndicationCallback);
}

bleResult_t App_RegisterLeCbCallbacks
(
    l2caLeCbDataCallback_t      pCallback,
    l2caLeCbControlCallback_t   pCtrlCallback
)
{
    pfL2caLeCbDataCallback = pCallback;
    pfL2caLeCbControlCallback = pCtrlCallback;

    return L2ca_RegisterLeCbCallbacks(App_L2caLeDataCallback, App_L2caLeControlCallback);
}

bleResult_t App_PostCallbackMessage
(
    appCallbackHandler_t   handler,
    appCallbackParam_t     param
)
{
    appMsgCallback_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc(sizeof (appMsgCallback_t));

    if (pMsgIn == NULL)
    {
        return gBleOutOfMemory_c;
    }

    pMsgIn->handler = handler;
    pMsgIn->param = param;

    /* Put message in the Cb App queue */
    (void)MSG_Queue(&mAppCbInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtAppCallback_c);

    return gBleSuccess_c;
}

void App_NvmErase(uint8_t mEntryIdx)
{
    if(mEntryIdx >= (uint8_t)gMaxBondedDevices_c)
    {
          return;
    }
#if gAppUseNvm_d
#if gUnmirroredFeatureSet_d == TRUE
    (void)NvErase((void**)&aBondingHeader[mEntryIdx]);
    (void)NvErase((void**)&aBondingDataDynamic[mEntryIdx]);
    (void)NvErase((void**)&aBondingDataStatic[mEntryIdx]);
    (void)NvErase((void**)&aBondingDataDeviceInfo[mEntryIdx]);

    uint32_t mDescIdx;

    for(mDescIdx = (mEntryIdx * gcGapMaximumSavedCccds_c);
        mDescIdx < (mEntryIdx + 1U) * gcGapMaximumSavedCccds_c; mDescIdx++)
    {
        (void)NvErase((void**)&aBondingDataDescriptor[mDescIdx]);
    }
#else // mirrored
    FLib_MemSet(&aBondingHeader[mEntryIdx], 0, gBleBondIdentityHeaderSize_c);
    NvSaveOnIdle((void*)&aBondingHeader[mEntryIdx], FALSE);
    FLib_MemSet(&aBondingDataDynamic[mEntryIdx], 0, gBleBondDataDynamicSize_c);
    NvSaveOnIdle((void*)&aBondingDataDynamic[mEntryIdx], FALSE);
    FLib_MemSet(&aBondingDataStatic[mEntryIdx], 0, gBleBondDataStaticSize_c);
    NvSaveOnIdle((void*)&aBondingDataStatic[mEntryIdx], FALSE);
    FLib_MemSet(&aBondingDataDeviceInfo[mEntryIdx], 0, gBleBondDataDeviceInfoSize_c);
    NvSaveOnIdle((void*)&aBondingDataDeviceInfo[mEntryIdx], FALSE);

    uint32_t mDescIdx;

    for(mDescIdx = (mEntryIdx * gcGapMaximumSavedCccds_c);
        mDescIdx < (mEntryIdx + 1) * gcGapMaximumSavedCccds_c; mDescIdx++)
    {
        FLib_MemSet(&aBondingDataDescriptor[mDescIdx], 0, gBleBondDataDescriptorSize_c);
        NvSaveOnIdle((void*)&aBondingDataDescriptor[mDescIdx], FALSE);
    }
#endif
#else
    FLib_MemSet(&maBondDataBlobs[mEntryIdx], 0, sizeof(bleBondDataBlob_t));
#endif
}

void App_NvmWrite
(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
)
{
    if(mEntryIdx >= (uint8_t)gMaxBondedDevices_c)
    {
          return;
    }
#if gAppUseNvm_d
    uint8_t  idx   = 0;

#if gUnmirroredFeatureSet_d == TRUE
    uint32_t mSize = 0;
    void**   ppNvmData = NULL;
    void*    pRamData = NULL;
#endif

#if gUnmirroredFeatureSet_d == TRUE

    for(idx = 0; idx < 5U; idx++)
    {
        ppNvmData = NULL;
        switch(*(uint8_t*)&idx)
        {
        case 0:
            if(pBondHeader != NULL)
            {
                ppNvmData = (void**)&aBondingHeader[mEntryIdx];
                pRamData  = pBondHeader;
                mSize     = gBleBondIdentityHeaderSize_c;
            }
            break;
        case 1:
            if(pBondDataDynamic != NULL)
            {
                ppNvmData = (void**)&aBondingDataDynamic[mEntryIdx];
                pRamData  = pBondDataDynamic;
                mSize     = gBleBondDataDynamicSize_c;
            }
            break;
        case 2:
            if(pBondDataStatic != NULL)
            {
                ppNvmData = (void**)&aBondingDataStatic[mEntryIdx];
                pRamData  = pBondDataStatic;
                mSize     = gBleBondDataStaticSize_c;
            }
            break;
        case 3:
            if(pBondDataDeviceInfo != NULL)
            {
                ppNvmData = (void**)&aBondingDataDeviceInfo[mEntryIdx];
                pRamData  = pBondDataDeviceInfo;
                mSize     = gBleBondDataDeviceInfoSize_c;
            }
            break;
        case 4:
            if(pBondDataDescriptor != NULL)
            {
                if(mDescriptorIndex < gcGapMaximumSavedCccds_c)
                {
                    ppNvmData = (void**)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex];
                    pRamData  = pBondDataDescriptor;
                    mSize     = gBleBondDataDescriptorSize_c;
                }
            }
            break;
        default:
            ; /* No action required */
            break;
        }

        if(ppNvmData != NULL)
        {
            if(gNVM_OK_c == NvMoveToRam(ppNvmData))
            {
                FLib_MemCpy(*ppNvmData, pRamData, mSize);
                (void)NvSaveOnIdle(ppNvmData, FALSE);
            }
            else
            {
                *ppNvmData = pRamData;
                (void)NvSyncSave(ppNvmData, FALSE);
            }
        }
    }
#else // gMirroredFeatureSet_d

    for(idx = 0; idx < 5; idx++)
    {
        switch(idx)
        {
        case 0:
            if(pBondHeader != NULL)
            {
                FLib_MemCpy((void*)&aBondingHeader[mEntryIdx], pBondHeader, gBleBondIdentityHeaderSize_c);
                (void)NvSaveOnIdle((void*)&aBondingHeader[mEntryIdx], FALSE);
            }
            break;
        case 1:
            if(pBondDataDynamic != NULL)
            {
                FLib_MemCpy((void*)&aBondingDataDynamic[mEntryIdx], pBondDataDynamic, gBleBondDataDynamicSize_c);
                (void)NvSaveOnIdle((void*)&aBondingDataDynamic[mEntryIdx], FALSE);
            }
            break;
        case 2:
            if(pBondDataStatic != NULL)
            {
                FLib_MemCpy((void*)&aBondingDataStatic[mEntryIdx], pBondDataStatic, gBleBondDataStaticSize_c);
                (void)NvSaveOnIdle((void*)&aBondingDataStatic[mEntryIdx], FALSE);
            }
            break;
        case 3:
            if(pBondDataDeviceInfo != NULL)
            {
                FLib_MemCpy((void*)&aBondingDataDeviceInfo[mEntryIdx], pBondDataDeviceInfo, gBleBondDataDeviceInfoSize_c);
                (void)NvSaveOnIdle((void*)&aBondingDataDeviceInfo[mEntryIdx], FALSE);
            }
            break;
        case 4:
            if(pBondDataDescriptor != NULL)
            {
                if(mDescriptorIndex < gcGapMaximumSavedCccds_c)
                {
                    FLib_MemCpy((void*)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex], pBondDataDescriptor, gBleBondDataDescriptorSize_c);
                    (void)NvSaveOnIdle((void*)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex], FALSE);
                }
            }
            break;
        default:
            ; /* No action required */
            break;
        }
    }

#endif //gUnmirroredFeatureSet_d

#else

    if(pBondHeader != NULL)
    {
        FLib_MemCpy(&maBondDataBlobs[mEntryIdx].bondHeader, pBondHeader, gBleBondIdentityHeaderSize_c);
    }

    if(pBondDataDynamic != NULL)
    {
        FLib_MemCpy((uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDynamic,
                    pBondDataDynamic,
                    gBleBondDataDynamicSize_c
                        );
    }

    if(pBondDataStatic != NULL)
    {
        FLib_MemCpy((uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobStatic,
                    pBondDataStatic,
                    gBleBondDataStaticSize_c
                        );
    }

    if(pBondDataDeviceInfo != NULL)
    {
        FLib_MemCpy((uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDeviceInfo,
                    pBondDataDeviceInfo,
                    gBleBondDataDeviceInfoSize_c
                        );
    }

    if(pBondDataDescriptor != NULL && mDescriptorIndex != gcGapMaximumSavedCccds_c)
    {
        FLib_MemCpy((uint8_t*)&(maBondDataBlobs[mEntryIdx].bondDataDescriptors[mDescriptorIndex]),
                    pBondDataDescriptor,
                    gBleBondDataDescriptorSize_c
                        );
    }

#endif
}

void App_NvmRead
(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
)
{
    if(mEntryIdx >= (uint8_t)gMaxBondedDevices_c)
    {
          return;
    }
#if gAppUseNvm_d
    uint8_t  idx = 0;
#if gUnmirroredFeatureSet_d == TRUE
    uint32_t mSize = 0;
    void**   ppNvmData = NULL;
    void*    pRamData = NULL;
#endif

#if gUnmirroredFeatureSet_d == TRUE
    for(idx = 0; idx < 5U; idx++)
    {
        ppNvmData = NULL;
        switch(*(uint8_t*)&idx)
        {
        case 0:
            if(pBondHeader != NULL)
            {
                ppNvmData = (void**)&aBondingHeader[mEntryIdx];
                pRamData  = pBondHeader;
                mSize     = gBleBondIdentityHeaderSize_c;
            }
            break;
        case 1:
            if(pBondDataDynamic != NULL)
            {
                ppNvmData = (void**)&aBondingDataDynamic[mEntryIdx];
                pRamData  = pBondDataDynamic;
                mSize     = gBleBondDataDynamicSize_c;
            }
            break;
        case 2:
            if(pBondDataStatic != NULL)
            {
                ppNvmData = (void**)&aBondingDataStatic[mEntryIdx];
                pRamData  = pBondDataStatic;
                mSize     = gBleBondDataStaticSize_c;
            }
            break;
        case 3:
            if(pBondDataDeviceInfo != NULL)
            {
                ppNvmData = (void**)&aBondingDataDeviceInfo[mEntryIdx];
                pRamData  = pBondDataDeviceInfo;
                mSize     = gBleBondDataDeviceInfoSize_c;
            }
            break;
        case 4:
            if(pBondDataDescriptor != NULL)
            {
                if(mDescriptorIndex < gcGapMaximumSavedCccds_c)
                {
                    ppNvmData = (void**)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex];
                    pRamData  = pBondDataDescriptor;
                    mSize     = gBleBondDataDescriptorSize_c;
                }
            }
            break;
        default:
            ; /* No action required */
            break;
        }

        if(ppNvmData != NULL && pRamData != NULL)
        {
            if(*ppNvmData != NULL)
            {
                FLib_MemCpy(pRamData, *ppNvmData, mSize);
            }
        }
    }
#else // gMirroredFeatureSet_d
    for(idx = 0; idx < 5; idx++)
    {
        switch(idx)
        {
        case 0:
            if(pBondHeader != NULL)
            {
                if(gNVM_OK_c == NvRestoreDataSet((void*)&aBondingHeader[mEntryIdx], FALSE))
                {
                  FLib_MemCpy(pBondHeader, (void*)&aBondingHeader[mEntryIdx], gBleBondIdentityHeaderSize_c);
                }
            }
            break;
        case 1:
            if(pBondDataDynamic != NULL)
            {
                if(gNVM_OK_c == NvRestoreDataSet((void*)&aBondingDataDynamic[mEntryIdx], FALSE))
                {
                  FLib_MemCpy(pBondDataDynamic, (void*)&aBondingDataDynamic[mEntryIdx], gBleBondDataDynamicSize_c);
                }
            }
            break;
        case 2:
            if(pBondDataStatic != NULL)
            {
                if(gNVM_OK_c == NvRestoreDataSet((void*)&aBondingDataStatic[mEntryIdx], FALSE))
                {
                  FLib_MemCpy(pBondDataStatic, (void*)&aBondingDataStatic[mEntryIdx], gBleBondDataStaticSize_c);
                }
            }
            break;
        case 3:
            if(pBondDataDeviceInfo != NULL)
            {
                if(gNVM_OK_c == NvRestoreDataSet((void*)&aBondingDataDeviceInfo[mEntryIdx], FALSE))
                {
                  FLib_MemCpy(pBondDataDeviceInfo, (void*)&aBondingDataDeviceInfo[mEntryIdx], gBleBondDataDeviceInfoSize_c);
                }
            }
            break;
        case 4:
            if(pBondDataDescriptor != NULL)
            {
                if(mDescriptorIndex < gcGapMaximumSavedCccds_c)
                {
                    if(gNVM_OK_c == NvRestoreDataSet((void*)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex], FALSE))
                    {
                      FLib_MemCpy(pBondDataDescriptor, (void*)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex], gBleBondDataDescriptorSize_c);
                    }
                }
            }
            break;
        default:
            ; /* No action required */
            break;
        }
    }
#endif

#else

    if(pBondHeader != NULL)
    {
        FLib_MemCpy(pBondHeader, &maBondDataBlobs[mEntryIdx].bondHeader, gBleBondIdentityHeaderSize_c);
    }

    if(pBondDataDynamic != NULL)
    {
        FLib_MemCpy(pBondDataDynamic,
                    (uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDynamic,
                    gBleBondDataDynamicSize_c
                        );
    }

    if(pBondDataStatic != NULL)
    {
        FLib_MemCpy(pBondDataStatic,
                    (uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobStatic,
                    gBleBondDataStaticSize_c
                        );
    }

    if(pBondDataDeviceInfo != NULL)
    {
        FLib_MemCpy(pBondDataDeviceInfo,
                    (uint8_t*)&maBondDataBlobs[mEntryIdx].bondDataBlobDeviceInfo,
                    gBleBondDataDeviceInfoSize_c
                        );
    }

    if(pBondDataDescriptor != NULL && mDescriptorIndex < gcGapMaximumSavedCccds_c)
    {
        FLib_MemCpy(pBondDataDescriptor,
                    (uint8_t*)&(maBondDataBlobs[mEntryIdx].bondDataDescriptors[mDescriptorIndex]),
                    gBleBondDataDescriptorSize_c
                        );
    }

#endif
}

#if !defined(MULTICORE_CONNECTIVITY_CORE) || (!MULTICORE_CONNECTIVITY_CORE)
void App_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(gapGenericEvent_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGapGenericMsg_c;
    FLib_MemCpy(&pMsgIn->msgData.genericMsg, pGenericEvent, sizeof(gapGenericEvent_t));

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}
#endif /* MULTICORE_CONNECTIVITY_CORE */

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*****************************************************************************
* Handles all key events for this device.
* Interface assumptions: None
* Return value: None
*****************************************************************************/
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
static void App_KeyboardCallBack
  (
  uint8_t events  /*IN: Events from keyboard module  */
  )
{
    BleApp_HandleKeys(events);
}
#endif

/*****************************************************************************
* Handles all messages received from the host task.
* Interface assumptions: None
* Return value: None
*****************************************************************************/
static void App_HandleHostMessageInput(appMsgFromHost_t* pMsg)
{
    switch ( pMsg->msgType )
    {
        case gAppGapGenericMsg_c:
        {
            if (pfGenericCallback != NULL)
            {
                pfGenericCallback(&pMsg->msgData.genericMsg);
            }
            else
            {
                BleApp_GenericCallback(&pMsg->msgData.genericMsg);
            }
            break;
        }
        case gAppGapAdvertisementMsg_c:
        {
            if (pfAdvCallback != NULL)
            {
                pfAdvCallback(&pMsg->msgData.advMsg);
            }
            break;
        }
        case gAppGapScanMsg_c:
        {
            if (pfScanCallback != NULL)
            {
                pfScanCallback(&pMsg->msgData.scanMsg);
            }
            break;
        }
        case gAppGapConnectionMsg_c:
        {
            if (pfConnCallback != NULL)
            {
                pfConnCallback(pMsg->msgData.connMsg.deviceId, &pMsg->msgData.connMsg.connEvent);
            }
            break;
        }
        case gAppGattServerMsg_c:
        {
            if (pfGattServerCallback != NULL)
            {
                pfGattServerCallback(pMsg->msgData.gattServerMsg.deviceId, &pMsg->msgData.gattServerMsg.serverEvent);
            }
            break;
        }
        case gAppGattClientProcedureMsg_c:
        {
            if (pfGattClientProcCallback != NULL)
            {
                pfGattClientProcCallback(
                    pMsg->msgData.gattClientProcMsg.deviceId,
                    pMsg->msgData.gattClientProcMsg.procedureType,
                    pMsg->msgData.gattClientProcMsg.procedureResult,
                    pMsg->msgData.gattClientProcMsg.error);
            }
            break;
        }
        case gAppGattClientNotificationMsg_c:
        {
            if (pfGattClientNotifCallback != NULL)
            {
                pfGattClientNotifCallback(
                    pMsg->msgData.gattClientNotifIndMsg.deviceId,
                    pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle,
                    pMsg->msgData.gattClientNotifIndMsg.aValue,
                    pMsg->msgData.gattClientNotifIndMsg.valueLength);
            }
            break;
        }
        case gAppGattClientIndicationMsg_c:
        {
            if (pfGattClientIndCallback != NULL)
            {
                pfGattClientIndCallback(
                    pMsg->msgData.gattClientNotifIndMsg.deviceId,
                    pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle,
                    pMsg->msgData.gattClientNotifIndMsg.aValue,
                    pMsg->msgData.gattClientNotifIndMsg.valueLength);
            }
            break;
        }
        case gAppL2caLeDataMsg_c:
        {
            if (pfL2caLeCbDataCallback != NULL)
            {
                pfL2caLeCbDataCallback(
                    pMsg->msgData.l2caLeCbDataMsg.deviceId,
                    pMsg->msgData.l2caLeCbDataMsg.channelId,
                    pMsg->msgData.l2caLeCbDataMsg.aPacket,
                    pMsg->msgData.l2caLeCbDataMsg.packetLength);
            }
            break;
        }
        case gAppL2caLeControlMsg_c:
        {
            if (pfL2caLeCbControlCallback != NULL)
            {
                pfL2caLeCbControlCallback(&pMsg->msgData.l2caLeCbControlMsg);
            }
            break;
        }
#if defined(FSL_FEATURE_SOC_CAU3_COUNT) && (FSL_FEATURE_SOC_CAU3_COUNT > 0)
#else
        case gAppSecLibMultiplyMsg_c:
        {
            computeDhKeyParam_t *pData = pMsg->msgData.secLibMsgData.pData;
            bool_t ready = SecLib_HandleMultiplyStep(pData);

            if(ready == FALSE)
            {
                SecLib_ExecMultiplicationCb(pData);
            }
            else
            {
                bleResult_t status;

                status = Gap_ResumeLeScStateMachine(pData);
                if (status != gBleSuccess_c)
                {
                    /* Not enough memory to resume LE SC operations */
                    panic(0, (uint32_t)Gap_ResumeLeScStateMachine, 0, 0);
                }
            }

            break;
        }
#endif
        default:
        {
            ; /* No action required */
            break;
        }
    }
}



MULTICORE_STATIC void App_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) || (gFsciBleTest_d == 1))
    fsciBleGapConnectionEvtMonitor(peerDeviceId, pConnectionEvent);
#else
    appMsgFromHost_t *pMsgIn = NULL;
    connectionMsg_t *pConnMsg = NULL;        /* used to get relative address of connEvent member */
    gapConnectionEvent_t *pConnEvent = NULL; /* used to get relative address of eventData member */

    uint32_t msgLen = (uint32_t)&(pMsgIn->msgData) + sizeof(connectionMsg_t);

    if(pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        gapSmpKeys_t    *pKeys = pConnectionEvent->eventData.keysReceivedEvent.pKeys;

        /* Take into account alignment */
        msgLen = (uint32_t)&(pMsgIn->msgData) + (uint32_t)&(pConnMsg->connEvent) +
                 (uint32_t)&(pConnEvent->eventData) + sizeof(gapKeysReceivedEvent_t) + sizeof(gapSmpKeys_t);

        if (pKeys->aLtk != NULL)
        {
            msgLen += 2U * sizeof(uint8_t) + (uint32_t)pKeys->cLtkSize + (uint32_t)pKeys->cRandSize;
        }

        msgLen += (pKeys->aIrk != NULL) ? (gcSmpIrkSize_c + gcBleDeviceAddressSize_c) : 0U;
        msgLen += (pKeys->aCsrk != NULL) ? gcSmpCsrkSize_c : 0U;
    }

    pMsgIn = MSG_Alloc(msgLen);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGapConnectionMsg_c;
    pMsgIn->msgData.connMsg.deviceId = peerDeviceId;

    if(pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        union
        {
            uint8_t      *pu8;
            gapSmpKeys_t *pObject;
        } temp; /* MISRA rule 11.3 */

        gapSmpKeys_t    *pKeys = pConnectionEvent->eventData.keysReceivedEvent.pKeys;
        uint8_t         *pCursor = (uint8_t*)&pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys;

        pMsgIn->msgData.connMsg.connEvent.eventType = gConnEvtKeysReceived_c;
        pCursor += sizeof(void*);

        temp.pu8 = pCursor;
        pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys = temp.pObject;

        /* Copy SMP Keys structure */
        FLib_MemCpy(pCursor, pConnectionEvent->eventData.keysReceivedEvent.pKeys, sizeof(gapSmpKeys_t));
        pCursor += sizeof(gapSmpKeys_t);

        if (pKeys->aLtk != NULL)
        {
            /* Copy LTK */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->cLtkSize = pKeys->cLtkSize;
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aLtk = pCursor;
            FLib_MemCpy(pCursor, pKeys->aLtk, pKeys->cLtkSize);
            pCursor += pKeys->cLtkSize;

            /* Copy RAND */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->cRandSize = pKeys->cRandSize;
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aRand = pCursor;
            FLib_MemCpy(pCursor, pKeys->aRand, pKeys->cRandSize);
            pCursor += pKeys->cRandSize;
        }

        if (pKeys->aIrk != NULL)
        {
            /* Copy IRK */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aIrk = pCursor;
            FLib_MemCpy(pCursor, pKeys->aIrk, gcSmpIrkSize_c);
            pCursor += gcSmpIrkSize_c;

            /* Copy Address*/
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->addressType = pKeys->addressType;
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aAddress = pCursor;
            FLib_MemCpy(pCursor, pKeys->aAddress, gcBleDeviceAddressSize_c);
            pCursor += gcBleDeviceAddressSize_c;
        }

        if (pKeys->aCsrk != NULL)
        {
            /* Copy CSRK */
            pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys->aCsrk = pCursor;
            FLib_MemCpy(pCursor, pKeys->aCsrk, gcSmpCsrkSize_c);
        }
    }
    else
    {
        FLib_MemCpy(&pMsgIn->msgData.connMsg.connEvent, pConnectionEvent, sizeof(gapConnectionEvent_t));
    }

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}


MULTICORE_STATIC void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) || (gFsciBleTest_d == 1))
    fsciBleGapAdvertisingEvtMonitor(pAdvertisingEvent);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(gapAdvertisingEvent_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGapAdvertisementMsg_c;
    pMsgIn->msgData.advMsg.eventType = pAdvertisingEvent->eventType;
    pMsgIn->msgData.advMsg.eventData = pAdvertisingEvent->eventData;

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) || (gFsciBleTest_d == 1))
    fsciBleGapScanningEvtMonitor(pScanningEvent);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    uint32_t msgLen = (uint32_t)&(pMsgIn->msgData) + sizeof(gapScanningEvent_t);

    if (pScanningEvent->eventType == gDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.scannedDevice.dataLength;
    }
    else if (pScanningEvent->eventType == gExtDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.extScannedDevice.dataLength;
    }
    else
    {
        /* msgLen does not modify for all other event types */
    }

    pMsgIn = MSG_Alloc(msgLen);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGapScanMsg_c;
    pMsgIn->msgData.scanMsg.eventType = pScanningEvent->eventType;

    if (pScanningEvent->eventType == gScanCommandFailed_c)
    {
        pMsgIn->msgData.scanMsg.eventData.failReason = pScanningEvent->eventData.failReason;
    }
    else if (pScanningEvent->eventType == gDeviceScanned_c)
    {
        FLib_MemCpy(&pMsgIn->msgData.scanMsg.eventData.scannedDevice,
                    &pScanningEvent->eventData.scannedDevice,
                    sizeof(pScanningEvent->eventData.scannedDevice));

        /* Copy data after the gapScanningEvent_t structure and update the data pointer*/
        pMsgIn->msgData.scanMsg.eventData.scannedDevice.data = (uint8_t*)&pMsgIn->msgData + sizeof(gapScanningEvent_t);
        FLib_MemCpy(pMsgIn->msgData.scanMsg.eventData.scannedDevice.data,
                    pScanningEvent->eventData.scannedDevice.data,
                    pScanningEvent->eventData.scannedDevice.dataLength);
    }
    else if (pScanningEvent->eventType == gExtDeviceScanned_c)
    {
        FLib_MemCpy(&pMsgIn->msgData.scanMsg.eventData.extScannedDevice,
                    &pScanningEvent->eventData.extScannedDevice,
                    sizeof(pScanningEvent->eventData.extScannedDevice));

        /* Copy data after the gapScanningEvent_t structure and update the data pointer*/
        pMsgIn->msgData.scanMsg.eventData.extScannedDevice.pData = (uint8_t*)&pMsgIn->msgData + sizeof(gapScanningEvent_t);
        FLib_MemCpy(pMsgIn->msgData.scanMsg.eventData.extScannedDevice.pData,
                    pScanningEvent->eventData.extScannedDevice.pData,
                    pScanningEvent->eventData.extScannedDevice.dataLength);
    }
    else
    {
        /* no action for all other event types */
    }

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_GattServerCallback
(
    deviceId_t          peerDeviceId,
    gattServerEvent_t*  pServerEvent
)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) )
    fsciBleGattServerEvtMonitor(peerDeviceId, pServerEvent);
#else
    appMsgFromHost_t *pMsgIn = NULL;
    uint32_t msgLen = (uint32_t)&(pMsgIn->msgData) + sizeof(gattServerMsg_t);

    if (pServerEvent->eventType == gEvtAttributeWritten_c ||
        pServerEvent->eventType == gEvtAttributeWrittenWithoutResponse_c)
    {
        msgLen += pServerEvent->eventData.attributeWrittenEvent.cValueLength;
    }

    pMsgIn = MSG_Alloc(msgLen);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGattServerMsg_c;
    pMsgIn->msgData.gattServerMsg.deviceId = peerDeviceId;
    FLib_MemCpy(&pMsgIn->msgData.gattServerMsg.serverEvent, pServerEvent, sizeof(gattServerEvent_t));

    if ((pMsgIn->msgData.gattServerMsg.serverEvent.eventType == gEvtAttributeWritten_c) ||
        (pMsgIn->msgData.gattServerMsg.serverEvent.eventType == gEvtAttributeWrittenWithoutResponse_c))
    {
        /* Copy value after the gattServerEvent_t structure and update the aValue pointer*/
        pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue =
          (uint8_t *)&pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue + sizeof(uint8_t*);
        FLib_MemCpy(pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.cValueLength);

    }

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_GattClientProcedureCallback
(
    deviceId_t              deviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) )
    fsciBleGattClientProcedureEvtMonitor(deviceId, procedureType, procedureResult, error);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(gattClientProcMsg_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGattClientProcedureMsg_c;
    pMsgIn->msgData.gattClientProcMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientProcMsg.procedureType = procedureType;
    pMsgIn->msgData.gattClientProcMsg.error = error;
    pMsgIn->msgData.gattClientProcMsg.procedureResult = procedureResult;

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_GattClientNotificationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) )
    fsciBleGattClientNotificationEvtMonitor(deviceId, characteristicValueHandle, aValue, valueLength);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(gattClientNotifIndMsg_t)
                        + (uint32_t)valueLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGattClientNotificationMsg_c;
    pMsgIn->msgData.gattClientNotifIndMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientNotifIndMsg.characteristicValueHandle = characteristicValueHandle;
    pMsgIn->msgData.gattClientNotifIndMsg.valueLength = valueLength;

    /* Copy value after the gattClientNotifIndMsg_t structure and update the aValue pointer*/
    pMsgIn->msgData.gattClientNotifIndMsg.aValue = (uint8_t*)&pMsgIn->msgData + sizeof(gattClientNotifIndMsg_t);
    FLib_MemCpy(pMsgIn->msgData.gattClientNotifIndMsg.aValue, aValue, valueLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_GattClientIndicationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) )
    fsciBleGattClientIndicationEvtMonitor(deviceId, characteristicValueHandle, aValue, valueLength);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(gattClientNotifIndMsg_t)
                        + (uint32_t)valueLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppGattClientIndicationMsg_c;
    pMsgIn->msgData.gattClientNotifIndMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientNotifIndMsg.characteristicValueHandle = characteristicValueHandle;
    pMsgIn->msgData.gattClientNotifIndMsg.valueLength = valueLength;

    /* Copy value after the gattClientIndIndMsg_t structure and update the aValue pointer*/
    pMsgIn->msgData.gattClientNotifIndMsg.aValue = (uint8_t*)&pMsgIn->msgData + sizeof(gattClientNotifIndMsg_t);
    FLib_MemCpy(pMsgIn->msgData.gattClientNotifIndMsg.aValue, aValue, valueLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_L2caLeDataCallback
(
    deviceId_t deviceId,
    uint16_t channelId,
    uint8_t* pPacket,
    uint16_t packetLength
)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) )
    fsciBleL2capCbLeCbDataEvtMonitor(deviceId, channelId, pPacket, packetLength);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + (sizeof(l2caLeCbDataMsg_t) - 1U)
                        + (uint32_t)packetLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppL2caLeDataMsg_c;
    pMsgIn->msgData.l2caLeCbDataMsg.deviceId = deviceId;
    pMsgIn->msgData.l2caLeCbDataMsg.channelId = channelId;
    pMsgIn->msgData.l2caLeCbDataMsg.packetLength = packetLength;

    FLib_MemCpy(pMsgIn->msgData.l2caLeCbDataMsg.aPacket, pPacket, packetLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_L2caLeControlCallback
(
    l2capControlMessage_t* pMessage
)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) )
    fsciBleL2capCbLeCbControlEvtMonitor(pMessage);
#else
    appMsgFromHost_t *pMsgIn = NULL;
    uint8_t messageLength = 0U;

    switch (pMessage->messageType) {
        case gL2ca_LePsmConnectRequest_c:
        {
            messageLength = sizeof(l2caLeCbConnectionRequest_t);
            break;
        }
        case gL2ca_LePsmConnectionComplete_c:
        {
            messageLength = sizeof(l2caLeCbConnectionComplete_t);
            break;
        }
        case gL2ca_LePsmDisconnectNotification_c:
        {
            messageLength = sizeof(l2caLeCbDisconnection_t);
            break;
        }
        case gL2ca_NoPeerCredits_c:
        {
            messageLength = sizeof(l2caLeCbNoPeerCredits_t);
            break;
        }
        case gL2ca_LocalCreditsNotification_c:
        {
            messageLength = sizeof(l2caLeCbLocalCreditsNotification_t);
            break;
        }
        case gL2ca_Error_c:
        {
            messageLength = sizeof(l2caLeCbError_t);
            break;
        }
        default:
        {
            messageLength = 0U;
            break;
        }
    }

    if (messageLength == 0U)
    {
        return;
    }

    /* Allocate a buffer with enough space to store the biggest packet */
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(l2capControlMessage_t));

    if (pMsgIn == NULL)
    {
          return;
    }

    pMsgIn->msgType = gAppL2caLeControlMsg_c;
    pMsgIn->msgData.l2caLeCbControlMsg.messageType = pMessage->messageType;

    FLib_MemCpy(&pMsgIn->msgData.l2caLeCbControlMsg.messageData, &pMessage->messageData, messageLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

MULTICORE_STATIC void App_SecLibMultCallback
(
    computeDhKeyParam_t *pData
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(secLibMsgData_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = gAppSecLibMultiplyMsg_c;
    pMsgIn->msgData.secLibMsgData.pData = pData;

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)
/* Called by BLE when a connect is received */
static void BLE_SignalFromISRCallback(void)
{
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    PWR_DisallowDeviceToSleep();
#endif /* cPWR_UsePowerDownMode */
}
#endif /* !gUseHciTransportDownward_d */


/*! *********************************************************************************
* \brief  This function will try to put the MCU into a deep sleep mode for at most
*         the maximum OS idle time specified. Else the MCU will enter a sleep mode
*         until the first IRQ.
*
* \param[in]  xExpectedIdleTime  The idle time in OS ticks
*
* \remarks  This feature is available only for FreeRTOS.
*           This function will replace the default implementation from
*           fsl_tickless_systick.c which is defined as weak.
*
********************************************************************************** */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && (configUSE_TICKLESS_IDLE != 0)
void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
    PWRLib_WakeupReason_t wakeupReason;

    if( PWR_CheckIfDeviceCanGoToSleep() && (xExpectedIdleTime > mAppOverheadTicks_c))
    {
        /* Set deep-sleep duration. Compensate for the application processing overhead time */
        PWR_SetDeepSleepTimeInMs((xExpectedIdleTime - mAppOverheadTicks_c) * portTICK_PERIOD_MS);
        PWR_ResetTotalSleepDuration();

        /* Enter Low Power */
        wakeupReason = PWR_EnterLowPower();

#if gKBD_KeysCount_c > 0
        /* Woke up on Keyboard Press */
        if(wakeupReason.Bits.FromKeyBoard != 0)
        {
            KBD_SwitchPressedOnWakeUp();
        }
#endif
        /* Get actual deep sleep time, and converted to OS ticks */
        xExpectedIdleTime = PWR_GetTotalSleepDurationMS() / portTICK_PERIOD_MS;

        portENTER_CRITICAL();
        /* Update the OS time ticks. */
        vTaskStepTick( xExpectedIdleTime );
        portEXIT_CRITICAL();
    }
    else
    {
        /* Enter MCU Sleep */
        PWR_EnterSleep();
    }
}
#endif
