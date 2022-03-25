/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
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
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && (configUSE_TICKLESS_IDLE != 0)
#include "task.h"
#endif
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
#include "clock_config.h"
#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ))
#include "rco32k_calibration.h"
#endif /* BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ */
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
    gAppGapGenericMsg_c = 0U,
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

/* SecLib to Application Data Message */
typedef struct secLibMsgData_tag{
    computeDhKeyParam_t *pData;
} secLibMsgData_t;

typedef struct appMsgFromHost_tag{
    uint32_t    msgType;
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

#ifdef CPU_QN908X
#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ))
#if (defined(CFG_CALIBRATION_ON_IDLE_TASK) && (CFG_CALIBRATION_ON_IDLE_TASK > 0))
static void App_Rco32KCalibrate(void);
#endif /* CFG_CALIBRATION_ON_IDLE_TASK */
#endif /* BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ */
#endif /* CPU_QN908X */


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

#if (!defined(KW37A4_SERIES) && !defined(KW37Z4_SERIES) && !defined(KW38A4_SERIES) && !defined(KW38Z4_SERIES) && !defined(KW39A4_SERIES))
extern void (*pfBLE_SignalFromISR)(void);
#endif

#endif /* gUseHciTransportDownward_d */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void App_Thread (uint32_t param);

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

#ifdef CPU_QN908X
#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ))
extern volatile bool_t gRco32kCalibrationRequest;
extern void RCO32K_Calibrate(void);
#endif /* BOARD_XTAL1_CLK_HZ  */
#endif /* CPU_QN908X */
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

#ifdef CPU_QN908X
        /* Initialize QN9080 BLE Controller. Requires that MEM_Manager and SecLib to be already initialized */
        BLE_Init(gAppMaxConnections_c);
#endif /* CPU_QN908X */

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

#ifdef CPU_QN908X
#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ))
#if (defined(CFG_CALIBRATION_ON_IDLE_TASK) && (CFG_CALIBRATION_ON_IDLE_TASK > 0))
static void App_Rco32KCalibrate(void)
{
    if(gRco32kCalibrationRequest)
    {
        /* perform RCO32K calibration */
        RCO32K_Calibrate();
        /* clear the calibration request flag */
        gRco32kCalibrationRequest = FALSE;
    }
}
#endif /* CFG_CALIBRATION_ON_IDLE_TASK */
#endif /* BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ */
#endif /* CPU_QN908X */

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && mAppEnterLpFromIdleTask_c

/*! *********************************************************************************
* \brief The idle task is created when applications enable the usage of the Framework
*        Low-Power module
*
* \param[in] none
*
* \return  none
********************************************************************************** */
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

/*! *********************************************************************************
* \brief Is linked by the application in order to hook the idle task
*
* \param[in] none
*
* \return  none
********************************************************************************** */
void vApplicationIdleHook(void)
{
#ifdef CPU_QN908X
#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ))
#if (defined(CFG_CALIBRATION_ON_IDLE_TASK) && (CFG_CALIBRATION_ON_IDLE_TASK > 0))
    App_Rco32KCalibrate();
#endif /* CFG_CALIBRATION_ON_IDLE_TASK */
#endif /* BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ */
#endif /* CPU_QN908X */
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

#ifdef CPU_QN908X
#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ))
#if (defined(CFG_CALIBRATION_ON_IDLE_TASK) && (CFG_CALIBRATION_ON_IDLE_TASK > 0))
        App_Rco32KCalibrate();
#endif /* CFG_CALIBRATION_ON_IDLE_TASK */
#endif /* BOARD_XTAL1_CLK_HZ != CLK_XTAL_32KHZ */
#endif /* CPU_QN908X */

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

/*! *********************************************************************************
* \brief Initializes task
*
* \param[in]  none
*
* \return  osaStatus_Error or osaStatus_Success.
*
********************************************************************************** */
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

/*! *********************************************************************************
* \brief  Application wrapper function for Gap_Connect.
*
* \param[in] pParameters  Connection Request parameters
* \param[in] connCallback Callback used to receive connection events.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t App_Connect(
    gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t             connCallback
)
{
    pfConnCallback = connCallback;

    return Gap_Connect(pParameters, App_ConnectionCallback);
}

/*! *********************************************************************************
* \brief  Application wrapper function for Gap_StartAdvertising.
*
* \param[in] advertisingCallback   Callback used by the application to receive advertising events.
*                                  Can be NULL.
* \param[in] connectionCallback    Callback used by the application to receive connection events.
*                                  Can be NULL.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t App_StartAdvertising(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback
)
{
    pfAdvCallback = advertisingCallback;
    pfConnCallback = connectionCallback;

    return Gap_StartAdvertising(App_AdvertisingCallback, App_ConnectionCallback);
}

/*! *********************************************************************************
* \brief  Application wrapper function for Gap_StartExtAdvertising.
*
* \param[in] advertisingCallback   Callback used by the application to receive advertising events.
*                                  Can be NULL.
* \param[in] connectionCallback    Callback used by the application to receive connection events.
*                                  Can be NULL.
* \param[in] handle                The ID of the advertising set
* \param[in] duration              The duration of the advertising
* \param[in] maxExtAdvEvents       The maximum number of advertising events
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
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

/*! *********************************************************************************
* \brief  Application wrapper function for Gap_StartScanning.
*
* \param[in] pScanningParameters    The scanning parameters; may be NULL.
* \param[in] scanningCallback       The scanning callback.
* \param[in] enableFilterDuplicates Enable duplicate advertising reports filtering
* \param[in] duration               Scan duration
* \param[in] period                 Scan period
*
* \return    gBleSuccess_c or error.
*
********************************************************************************** */
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

/*! *********************************************************************************
* \brief  Application wrapper function for GattServer_RegisterCallback.
*
* \param[in] serverCallback Application-defined callback to be triggered by this module.
*
* \return    gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t App_RegisterGattServerCallback(gattServerCallback_t  serverCallback)
{
    pfGattServerCallback = serverCallback;

    return GattServer_RegisterCallback(App_GattServerCallback);
}

/*! *********************************************************************************
* \brief  Application wrapper function for App_RegisterGattClientProcedureCallback.
*
* \param[in] callback Application-defined callback to be triggered by this module.
*
* \return    gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t App_RegisterGattClientProcedureCallback(gattClientProcedureCallback_t  callback)
{
    pfGattClientProcCallback = callback;

    return GattClient_RegisterProcedureCallback(App_GattClientProcedureCallback);
}

/*! *********************************************************************************
* \brief  Application wrapper function for GattClient_RegisterNotificationCallback.
*
* \param[in] callback   Application defined callback to be triggered by this module.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t App_RegisterGattClientNotificationCallback(gattClientNotificationCallback_t  callback)
{
    pfGattClientNotifCallback = callback;

    return GattClient_RegisterNotificationCallback(App_GattClientNotificationCallback);
}

/*! *********************************************************************************
* \brief  Application wrapper function for GattClient_RegisterIndicationCallback.
*
* \param[in] callback   Application defined callback to be triggered by this module.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t App_RegisterGattClientIndicationCallback(gattClientIndicationCallback_t  callback)
{
    pfGattClientIndCallback = callback;

    return GattClient_RegisterIndicationCallback(App_GattClientIndicationCallback);
}

/*! *********************************************************************************
* \brief  Application wrapper function for L2ca_RegisterLeCbCallbacks.
*
* \param[in] pCallback     L2cap Data Callback
* \param[in] pCtrlCallback L2cap Control Callback
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
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

/*! *********************************************************************************
* \brief  Posts an application event containing a callback handler and parameter.
*
* \param[in] handler Handler function, to be executed when the event is processed.
* \param[in] param   Parameter for the handler function.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
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

/*! *********************************************************************************
* \brief  Performs NVM erase operation
*
* \param[in] mEntryIdx Bonded device index
*
* \return  bleResult_t
*
********************************************************************************** */
bleResult_t App_NvmErase(uint8_t mEntryIdx)
{
    bleResult_t status = gBleSuccess_c;
#if gAppUseNvm_d
    NVM_Status_t nvmStatus = gNVM_OK_c;
    uint32_t mDescIdx = 0U;
#endif
    
    if(mEntryIdx >= (uint8_t)gMaxBondedDevices_c)
    {
          status = gBleInvalidParameter_c;
    }
    else
    {
#if gAppUseNvm_d
#if gUnmirroredFeatureSet_d == TRUE
        nvmStatus = NvErase((void**)&aBondingHeader[mEntryIdx]);
        
        if (nvmStatus == gNVM_OK_c)
        {
            nvmStatus = NvErase((void**)&aBondingDataDynamic[mEntryIdx]);
        }
        
        if (nvmStatus == gNVM_OK_c)
        {
            nvmStatus = NvErase((void**)&aBondingDataStatic[mEntryIdx]);
        }
        
        if (nvmStatus == gNVM_OK_c)
        {
            nvmStatus = NvErase((void**)&aBondingDataDeviceInfo[mEntryIdx]);
        }
        
        for(mDescIdx = ((uint32_t)mEntryIdx * gcGapMaximumSavedCccds_c);
            mDescIdx < ((uint32_t)mEntryIdx + 1U) * gcGapMaximumSavedCccds_c; mDescIdx++)
        {
            nvmStatus = NvErase((void**)&aBondingDataDescriptor[mDescIdx]);
            
            if (nvmStatus != gNVM_OK_c)
            {
                break;
            }
        }
#else // mirrored
        FLib_MemSet(&aBondingHeader[mEntryIdx], 0, gBleBondIdentityHeaderSize_c);
        nvmStatus = NvSaveOnIdle((void*)&aBondingHeader[mEntryIdx], FALSE);
        
        if (nvmStatus == gNVM_OK_c)
        {
            FLib_MemSet(&aBondingDataDynamic[mEntryIdx], 0, gBleBondDataDynamicSize_c);
            nvmStatus = NvSaveOnIdle((void*)&aBondingDataDynamic[mEntryIdx], FALSE);
        }
        
        if (nvmStatus == gNVM_OK_c)
        {
            FLib_MemSet(&aBondingDataStatic[mEntryIdx], 0, gBleBondDataStaticSize_c);
            nvmStatus = NvSaveOnIdle((void*)&aBondingDataStatic[mEntryIdx], FALSE);
        }
        
        if (nvmStatus == gNVM_OK_c)
        {
            FLib_MemSet(&aBondingDataDeviceInfo[mEntryIdx], 0, gBleBondDataDeviceInfoSize_c);
            nvmStatus = NvSaveOnIdle((void*)&aBondingDataDeviceInfo[mEntryIdx], FALSE);
        }
        
        for(mDescIdx = ((uint32_t)mEntryIdx * gcGapMaximumSavedCccds_c);
            mDescIdx < ((uint32_t)mEntryIdx + 1U) * gcGapMaximumSavedCccds_c; mDescIdx++)
        {
            FLib_MemSet(&aBondingDataDescriptor[mDescIdx], 0, gBleBondDataDescriptorSize_c);
            nvmStatus = NvSaveOnIdle((void*)&aBondingDataDescriptor[mDescIdx], FALSE);
            
            if (nvmStatus == gNVM_OK_c)
            {
                break;
            }
        }
#endif
        if (nvmStatus != gNVM_OK_c)
        {
            status = gBleUnexpectedError_c;
        }
#else /* gAppUseNvm_d */
        FLib_MemSet(&maBondDataBlobs[mEntryIdx], 0, sizeof(bleBondDataBlob_t));
#endif
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  Performs NVM write operation
*
* \param[in] mEntryIdx            Bonded device index
* \param[in] pBondHeader          Bond Identity Header
* \param[in] pBondDataDynamic     Bond Data Dynamic
* \param[in] pBondDataStatic      Bond Data Static
* \param[in] pBondDataDeviceInfo  Bond Data Device Info
* \param[in] pBondDataDescriptor  Bond Data Descriptor
* \param[in] mDescriptorIndex     Descriptor Index
*
* \return    bleResult_t
*
********************************************************************************** */
bleResult_t App_NvmWrite
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
    bleResult_t status = gBleSuccess_c;
#if gAppUseNvm_d
    NVM_Status_t nvmStatus = gNVM_OK_c;
#endif
    if(mEntryIdx >= (uint8_t)gMaxBondedDevices_c)
    {
          status = gBleInvalidParameter_c;
    }
    else
    {
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
                    nvmStatus = NvSaveOnIdle(ppNvmData, FALSE);
                }
                else
                {
                    *ppNvmData = pRamData;
                    nvmStatus = NvSyncSave(ppNvmData, FALSE);
                }
            }
            
            if (nvmStatus != gNVM_OK_c)
            {
                /* An error occured, return error status. */
                status = gBleUnexpectedError_c;
                break;
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
                    nvmStatus = NvSaveOnIdle((void*)&aBondingHeader[mEntryIdx], FALSE);
                }
                break;
                case 1:
                if(pBondDataDynamic != NULL)
                {
                    FLib_MemCpy((void*)&aBondingDataDynamic[mEntryIdx], pBondDataDynamic, gBleBondDataDynamicSize_c);
                    nvmStatus = NvSaveOnIdle((void*)&aBondingDataDynamic[mEntryIdx], FALSE);
                }
                break;
                case 2:
                if(pBondDataStatic != NULL)
                {
                    FLib_MemCpy((void*)&aBondingDataStatic[mEntryIdx], pBondDataStatic, gBleBondDataStaticSize_c);
                    nvmStatus = NvSaveOnIdle((void*)&aBondingDataStatic[mEntryIdx], FALSE);
                }
                break;
                case 3:
                if(pBondDataDeviceInfo != NULL)
                {
                    FLib_MemCpy((void*)&aBondingDataDeviceInfo[mEntryIdx], pBondDataDeviceInfo, gBleBondDataDeviceInfoSize_c);
                    nvmStatus = NvSaveOnIdle((void*)&aBondingDataDeviceInfo[mEntryIdx], FALSE);
                }
                break;
                case 4:
                if(pBondDataDescriptor != NULL)
                {
                    if(mDescriptorIndex < gcGapMaximumSavedCccds_c)
                    {
                        FLib_MemCpy((void*)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex], pBondDataDescriptor, gBleBondDataDescriptorSize_c);
                        nvmStatus = NvSaveOnIdle((void*)&aBondingDataDescriptor[mEntryIdx * gcGapMaximumSavedCccds_c + mDescriptorIndex], FALSE);
                    }
                }
                break;
                default:
                ; /* No action required */
                break;
            }
            
            if (nvmStatus != gNVM_OK_c)
            {
                /* An error occured, return error status.*/
                status = gBleUnexpectedError_c;
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
    
    return status;
}

/*! *********************************************************************************
* \brief  Performs NVM read operation
*
* \param[in] mEntryIdx            Bonded device index
* \param[in] pBondHeader          Bond Identity Header
* \param[in] pBondDataDynamic     Bond Data Dynamic
* \param[in] pBondDataStatic      Bond Data Static
* \param[in] pBondDataDeviceInfo  Bond Data Device Info
* \param[in] pBondDataDescriptor  Bond Data Descriptor
* \param[in] mDescriptorIndex     Descriptor Index
*
* \return  bleResult_t
*
********************************************************************************** */
bleResult_t App_NvmRead
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
    bleResult_t status = gBleSuccess_c;

    if(mEntryIdx >= (uint8_t)gMaxBondedDevices_c)
    {
          status = gBleInvalidParameter_c;
    }
    else
    {
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
            
            /* if ppNvmData is not NULL the same holds for pRamData */
            if((NULL != ppNvmData) && (NULL != *ppNvmData))
            {
                FLib_MemCpy(pRamData, *ppNvmData, mSize);
            }
            if((NULL != ppNvmData) && (NULL == *ppNvmData))
            {
                status = gBleUnavailable_c;
                break;
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
                    else
                    {
                        status = gBleUnexpectedError_c;
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
                    else
                    {
                        status = gBleUnexpectedError_c;
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
                    else
                    {
                        status = gBleUnexpectedError_c;
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
                    else
                    {
                        status = gBleUnexpectedError_c;
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
                        else
                        {
                            status = gBleUnexpectedError_c;
                        }
                    }
                }
                break;
                default:
                ; /* No action required */
                break;
            }
            
            if (status != gBleSuccess_c)
            {
                /* An error occured, return error status.*/
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
    
    return status;
}

#if !defined(MULTICORE_CONNECTIVITY_CORE) || (!MULTICORE_CONNECTIVITY_CORE)

/*! *********************************************************************************
* \brief Sends the GAP Generic Event triggered by the Host Stack to the application
*
* \param[in] pGenericEvent Pointer to the generic event
*
* \return  none
*
********************************************************************************** */
void App_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(gapGenericEvent_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGapGenericMsg_c;
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
        case (uint32_t)gAppGapGenericMsg_c:
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
        case (uint32_t)gAppGapAdvertisementMsg_c:
        {
            if (pfAdvCallback != NULL)
            {
                pfAdvCallback(&pMsg->msgData.advMsg);
            }
            break;
        }
        case (uint32_t)gAppGapScanMsg_c:
        {
            if (pfScanCallback != NULL)
            {
                pfScanCallback(&pMsg->msgData.scanMsg);
            }
            break;
        }
        case (uint32_t)gAppGapConnectionMsg_c:
        {
            if (pfConnCallback != NULL)
            {
                pfConnCallback(pMsg->msgData.connMsg.deviceId, &pMsg->msgData.connMsg.connEvent);
            }
            break;
        }
        case (uint32_t)gAppGattServerMsg_c:
        {
            if (pfGattServerCallback != NULL)
            {
                pfGattServerCallback(pMsg->msgData.gattServerMsg.deviceId, &pMsg->msgData.gattServerMsg.serverEvent);
            }
            break;
        }
        case (uint32_t)gAppGattClientProcedureMsg_c:
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
        case (uint32_t)gAppGattClientNotificationMsg_c:
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
        case (uint32_t)gAppGattClientIndicationMsg_c:
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
        case (uint32_t)gAppL2caLeDataMsg_c:
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
        case (uint32_t)gAppL2caLeControlMsg_c:
        {
            if (pfL2caLeCbControlCallback != NULL)
            {
                pfL2caLeCbControlCallback(&pMsg->msgData.l2caLeCbControlMsg);
            }
            break;
        }
#if defined(FSL_FEATURE_SOC_CAU3_COUNT) && (FSL_FEATURE_SOC_CAU3_COUNT > 0)
#else
        case (uint32_t)gAppSecLibMultiplyMsg_c:
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

/*! *********************************************************************************
* \brief Sends the GAP Connection Event triggered by the Host Stack to the application
*
* \param[in] peerDeviceId The id of the peer device
* \param[in] pConnectionEvent Pointer to the connection event
*
* \return  none
*
********************************************************************************** */
MULTICORE_STATIC void App_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) || (gFsciBleTest_d == 1))
    fsciBleGapConnectionEvtMonitor(peerDeviceId, pConnectionEvent);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    uint32_t msgLen = sizeof(uint32_t) + sizeof(connectionMsg_t);

    if(pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        gapSmpKeys_t    *pKeys = pConnectionEvent->eventData.keysReceivedEvent.pKeys;

        /* add room for pMsgIn->msgType */
        msgLen = sizeof(uint32_t);
        /* add room for pMsgIn->msgData.connMsg.deviceId */
        msgLen += sizeof(uint32_t);
        /* add room for pMsgIn->msgData.connMsg.connEvent.eventType */
        msgLen += sizeof(uint32_t);
        /* add room for pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys pointer */
        msgLen += sizeof(void*);
        /* add room for pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys data */
        msgLen += sizeof(gapSmpKeys_t);
        if (pKeys->aLtk != NULL)
        {
            /* add room for LTK and Rand data */
            msgLen += (uint32_t)pKeys->cLtkSize + (uint32_t)pKeys->cRandSize;
        }
        /* add room for IRK data */
        msgLen += (pKeys->aIrk != NULL) ? (gcSmpIrkSize_c + gcBleDeviceAddressSize_c) : 0U;
        /*  add room for CSRK data */
        msgLen += (pKeys->aCsrk != NULL) ? gcSmpCsrkSize_c : 0U;
        /* add room for device address data */
        msgLen += (pKeys->aAddress != NULL) ? (gcBleDeviceAddressSize_c) : 0U;
    }

    pMsgIn = MSG_Alloc(msgLen);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGapConnectionMsg_c;
    pMsgIn->msgData.connMsg.deviceId = peerDeviceId;

    if(pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        union
        {
            uint8_t      *pu8;
            gapSmpKeys_t *pObject;
        } temp = {0}; /* MISRA rule 11.3 */

        gapSmpKeys_t    *pKeys = pConnectionEvent->eventData.keysReceivedEvent.pKeys;

        /* MISRA C:2012 rule 18.1 */
        uint8_t         *pCursor = (uint8_t*)&pMsgIn;
        pCursor += (uint8_t*)&pMsgIn->msgData.connMsg.connEvent.eventData.keysReceivedEvent.pKeys - (uint8_t*)&pMsgIn;

        pMsgIn->msgData.connMsg.connEvent.eventType = gConnEvtKeysReceived_c;
        pCursor += sizeof(void*); /* skip pKeys pointer */

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

/*! *********************************************************************************
* \brief Sends the GAP Advertising Event triggered by the Host Stack to the application
*
* \param[in] pAdvertisingEvent Pointer to the advertising event
*
* \return  none
*
********************************************************************************** */
MULTICORE_STATIC void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) || (gFsciBleTest_d == 1))
    fsciBleGapAdvertisingEvtMonitor(pAdvertisingEvent);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(gapAdvertisingEvent_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGapAdvertisementMsg_c;
    pMsgIn->msgData.advMsg.eventType = pAdvertisingEvent->eventType;
    pMsgIn->msgData.advMsg.eventData = pAdvertisingEvent->eventData;

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

/*! *********************************************************************************
* \brief Sends the GAP Scanning Event triggered by the Host Stack to the application
*
* \param[in] pScanningEvent Pointer to the scanning event
*
* \return  none
*
********************************************************************************** */
MULTICORE_STATIC void App_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1) && ((gFsciBleBBox_d == 1) || (gFsciBleTest_d == 1))
    fsciBleGapScanningEvtMonitor(pScanningEvent);
#else
    appMsgFromHost_t *pMsgIn = NULL;

    uint32_t msgLen = sizeof(uint32_t) + sizeof(gapScanningEvent_t);

    if (pScanningEvent->eventType == gDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.scannedDevice.dataLength;
    }
    else if (pScanningEvent->eventType == gExtDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.extScannedDevice.dataLength;
    }
    else if (pScanningEvent->eventType == gPeriodicDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.periodicScannedDevice.dataLength;
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

    pMsgIn->msgType = (uint32_t)gAppGapScanMsg_c;
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
    else if (pScanningEvent->eventType == gPeriodicDeviceScanned_c)
    {
        FLib_MemCpy(&pMsgIn->msgData.scanMsg.eventData.periodicScannedDevice,
                    &pScanningEvent->eventData.periodicScannedDevice,
                    sizeof(pScanningEvent->eventData.periodicScannedDevice));

        pMsgIn->msgData.scanMsg.eventData.periodicScannedDevice.pData = (uint8_t*)&pMsgIn->msgData + sizeof(gapScanningEvent_t);
        FLib_MemCpy(pMsgIn->msgData.scanMsg.eventData.periodicScannedDevice.pData,
                    pScanningEvent->eventData.periodicScannedDevice.pData,
                    pScanningEvent->eventData.periodicScannedDevice.dataLength);
    }
    else if (pScanningEvent->eventType == gPeriodicAdvSyncEstablished_c)
    {
        FLib_MemCpy(&pMsgIn->msgData.scanMsg.eventData.syncEstb,
                    &pScanningEvent->eventData.syncEstb,
                    sizeof(pScanningEvent->eventData.syncEstb));
    }
    else if (pScanningEvent->eventType == gPeriodicAdvSyncLost_c)
    {
        FLib_MemCpy(&pMsgIn->msgData.scanMsg.eventData.syncLost,
                    &pScanningEvent->eventData.syncLost,
                    sizeof(pScanningEvent->eventData.syncLost));
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

/*! *********************************************************************************
* \brief Handles the server events received from one of the peer devices
*
* \param[in] peerDeviceId The id of the peer device
* \param[in] pServerEvent The Gatt server event received
*
* \return  none
*
********************************************************************************** */
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
    uint32_t msgLen = sizeof(uint32_t) + sizeof(gattServerMsg_t);

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

    pMsgIn->msgType = (uint32_t)gAppGattServerMsg_c;
    pMsgIn->msgData.gattServerMsg.deviceId = peerDeviceId;
    FLib_MemCpy(&pMsgIn->msgData.gattServerMsg.serverEvent, pServerEvent, sizeof(gattServerEvent_t));

    if ((pMsgIn->msgData.gattServerMsg.serverEvent.eventType == gEvtAttributeWritten_c) ||
        (pMsgIn->msgData.gattServerMsg.serverEvent.eventType == gEvtAttributeWrittenWithoutResponse_c))
    {
        /* MISRA C:2012 rule 18.1 */
        uint32_t destAddr = (uint32_t)(uint8_t*)&pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue;
        destAddr += sizeof(uint8_t*);

        /* Copy value after the gattServerEvent_t structure and update the aValue pointer*/
        pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue = (uint8_t*)destAddr;
        FLib_MemCpy((void*)destAddr,
                    pServerEvent->eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.cValueLength);
    }

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

/*! *********************************************************************************
* \brief Handles a client procedure result reveived from a peer device
*
* \param[in] deviceId The id of the peer device
* \param[in] procedureType The received procedure type
* \param[in] procedureResult The received procedure result
* \param[in] error The received error
*
* \return  none
*
********************************************************************************** */
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

    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(gattClientProcMsg_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGattClientProcedureMsg_c;
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

/*! *********************************************************************************
* \brief Handles a notification received from a client
*
* \param[in] deviceId The id of the peer device
* \param[in] characteristicValueHandle The characteristic value handle
* \param[in] aValue The received value
* \param[in] valueLength The length of the value
*
* \return  none
*
********************************************************************************** */
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
    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(gattClientNotifIndMsg_t) + (uint32_t)valueLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGattClientNotificationMsg_c;
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

/*! *********************************************************************************
* \brief Handles a indication received from a client
*
* \param[in] deviceId The id of the peer device
* \param[in] characteristicValueHandle The characteristic value handle
* \param[in] aValue The received value
* \param[in] valueLength The length of the value
*
* \return  none
*
********************************************************************************** */
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
    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(gattClientNotifIndMsg_t)
                        + (uint32_t)valueLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGattClientIndicationMsg_c;
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

/*! *********************************************************************************
* \brief L2cap data callback
*
* \param[in] deviceId     The id of the peer device
* \param[in] channelId    The id of the channel
* \param[in] pPacket      Pointer to the received packet
* \param[in] packetLength The length of the packet
*
* \return  none
*
********************************************************************************** */
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
    pMsgIn = MSG_Alloc(sizeof(uint32_t) + (sizeof(l2caLeCbDataMsg_t) - 1U)
                        + (uint32_t)packetLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppL2caLeDataMsg_c;
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

/*! *********************************************************************************
* \brief L2Cap Control Callback
*
* \param[in] pMessage L2cap Control Message
*
* \return  none
*
********************************************************************************** */
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
        case gL2ca_ChannelStatusNotification_c:
        {
            messageLength = sizeof(l2caLeCbChannelStatusNotification_t);
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
    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(l2capControlMessage_t));

    if (pMsgIn == NULL)
    {
          return;
    }

    pMsgIn->msgType = (uint32_t)gAppL2caLeControlMsg_c;
    pMsgIn->msgData.l2caLeCbControlMsg.messageType = pMessage->messageType;

    FLib_MemCpy(&pMsgIn->msgData.l2caLeCbControlMsg.messageData, &pMessage->messageData, messageLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
#endif /* (MULTICORE_APPLICATION_CORE == 1) && (gFsciBleBBox_d == 1) */
}

/*! *********************************************************************************
* \brief SecLib Callback
*
* \param[in] pData SecLib message data
*
* \return  none
*
********************************************************************************** */
MULTICORE_STATIC void App_SecLibMultCallback
(
    computeDhKeyParam_t *pData
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(secLibMsgData_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppSecLibMultiplyMsg_c;
    pMsgIn->msgData.secLibMsgData.pData = pData;

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)

/*! *********************************************************************************
* \brief Notifies the application to deny sleep
*
* \param[in] none
*
* \return  none
*
* Called by BLE when a connect is received
*
********************************************************************************** */
static void BLE_SignalFromISRCallback(void)
{
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#if (!defined(CPU_MKW37A512VFT4) && !defined(CPU_MKW37Z512VFT4) && !defined(CPU_MKW38A512VFT4) && !defined(CPU_MKW38Z512VFT4) && !defined(CPU_MKW39A512VFT4) && !defined(CPU_MKW39Z512VFT4))
    PWR_DisallowDeviceToSleep();
#endif /* CPU_MKW37xxx, CPU_MKW38xxx and CPU_MKW39xxx*/
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
#ifdef FSL_RTOS_FREE_RTOS
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
        OSA_InterruptDisable();   
        if(eAbortSleep !=  eTaskConfirmSleepModeStatus())
        {
            wakeupReason = PWR_EnterLowPower();
            /* Get actual deep sleep time and convert it to OS ticks */
            xExpectedIdleTime = PWR_GetTotalSleepDurationMS() / portTICK_PERIOD_MS;
            /* Update the OS time ticks. */
            vTaskStepTick( xExpectedIdleTime );
        }
        else
        {
            wakeupReason.AllBits = 0U;
        }
        OSA_InterruptEnable();  
        (void)wakeupReason;
/* On KW37, KW38, KW39 series, key handle is processed directly in the lowpower FWK module */
#if (!defined(KW37A4_SERIES) && !defined(KW37Z4_SERIES) && !defined(KW38A4_SERIES) && !defined(KW38Z4_SERIES) && !defined(KW39A4_SERIES))
#if gKBD_KeysCount_c > 0
        /* Woke up on Keyboard Press */
        if(wakeupReason.Bits.FromKeyBoard != 0U)
        {
#if defined(cPWR_EnableDeepSleepMode_8) && (cPWR_EnableDeepSleepMode_8)
            /* Skip over the key scan timer to improve low power consumption. */
            BleApp_HandleKeys(gKBD_EventPressPB1_c);
#else
            KBD_SwitchPressedOnWakeUp();
#endif
        }
#endif /* gKBD_KeysCount_c > 0 */
#endif /* (!defined(KW37A4_SERIES) && !defined(KW37Z4_SERIES) && !defined(KW38A4_SERIES) && !defined(KW38Z4_SERIES) && !defined(KW39A4_SERIES)) */
        
    }
    else
    {
        /* Enter MCU Sleep */
        PWR_EnterSleep();
    }
}
#endif /* defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) && (configUSE_TICKLESS_IDLE != 0) */
#endif /* FSL_RTOS_FREE_RTOS */