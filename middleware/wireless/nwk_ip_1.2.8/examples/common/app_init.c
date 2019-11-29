/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
 \file       app_init.c
 \brief      This is a public source file for the initial system startup module. It contains
 the implementation of the interface functions.
 ==================================================================================================*/

/*==================================================================================================
 Include Files
 ==================================================================================================*/
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#if WDOG_ENABLE
#include "fsl_wdog.h"
#endif
#include "PhyInterface.h"
#include "MacInterface.h"

/* FSL Framework */
#include "RNG_Interface.h"
#include "LED.h"
#include "nvm_adapter.h"
#if NVM_NG_ENABLED
#include "NVM_Interface.h"
#endif
#include "TimersManager.h"
#include "Keyboard.h"
#if gLpmIncluded_d
#include "PWR_Interface.h"
#endif
#include "SerialManager.h"
#include "Panic.h"
#include "SecLib.h"

#include "fsl_os_abstraction.h"
#include "app_init.h"
#include "app_config.h"
#include "app_thread_config.h"
#include "thread_network.h"
#include "thread_cfg.h"
#include "debug_log.h"

#include "board.h"

#if THREAD_USE_THCI
#include "thci.h"
#include "FsciInterface.h"
#endif

#if THREAD_USE_SHELL
#include "shell_ip.h"
#endif

extern void APP_Init(void);
extern void APP_Handler(void);

#if gLpmIncluded_d
extern void App_SedWakeUpFromKeyBoard(void);
#endif

#if THR_SOFTWARE_RESET_ENABLE
extern void THCI_ResetCpuEvent(resetCpuStatus_t resetStatus, uint32_t timeoutUs);
#endif

#if gHybridApp_d
/* Bluetooth Low Energy */
#include "ble_init.h"
extern void BleApp_Init(void);
extern void BleApp_Thread (uint32_t param);
#endif
/*==================================================================================================
 Private macros
 ==================================================================================================*/

/*==================================================================================================
 Private type definitions
 ==================================================================================================*/

/*==================================================================================================
 Private prototypes
 ==================================================================================================*/

static void KBD_Callback(uint8_t events);
static void APP_HandleMcuResetOnIdle(void);
#if gLpmIncluded_d
static void APP_HandleLowPowerOnIdle(void);
#endif
#if WDOG_ENABLE
static void APP_WDOG_Init(void);
static void APP_WDOG_Refresh(void);
#endif
/*==================================================================================================
 Private global variables declarations
 ==================================================================================================*/

void (*pfAppKeyboardHandler)(uint8_t*) = NULL;

#if THREAD_USE_THCI
/* FSCI Interface Configuration structure */
static const gFsciSerialConfig_t mFsciSerials[] = 
{
    /* Baudrate,           interface type,   channel number */

#if THREAD_USE_THCI
      
#if THCI_USB_ENABLE
    {
        .baudrate = gUARTBaudRate115200_c,
        .interfaceType = gSerialMgrUSB_c,
        .interfaceChannel = 0,
        .virtualInterface = 0
    },
#endif
    
#if THCI_SPI_ENABLE
    {
        .baudrate = gSPI_BaudRate_1000000_c,
    #ifdef R41Z_DK07
        .interfaceType = gSerialMgrSPISlave_c,
#else
        .interfaceType = gSerialMgrSPIMaster_c,
#endif
#if defined(FRDM_KW41Z) || defined(USB_KW41)
        .interfaceChannel = 1,
#else
        /* including R41Z_DK07 */
        .interfaceChannel = 0,
#endif
        .virtualInterface = 0
    },
#else
    {
        .baudrate = gUARTBaudRate115200_c,
        .interfaceType = APP_SERIAL_INTERFACE_TYPE,
        .interfaceChannel = APP_SERIAL_INTERFACE_INSTANCE,
        .virtualInterface = 0
    },
#if gHybridApp_d
    {
        .baudrate = gUARTBaudRate115200_c,
        .interfaceType = APP_SERIAL_INTERFACE_TYPE,
        .interfaceChannel = APP_SERIAL_INTERFACE_INSTANCE,
        .virtualInterface = 1
    },
#endif
#endif
#endif
};
#endif

#if WDOG_ENABLE
/* Configure watchdog. */
const wdog_config_t wdogConfig =
{
    .enableWdog = TRUE, /* Watchdog mode */
    .timeoutValue = 0x4096U, /* Watchdog overflow time is about 4s*/
    .enableWindowMode = FALSE, /* Disable window function */
    .windowValue = 0, /* Watchdog window value */
    .prescaler = kWDOG_ClockPrescalerDivide1, /* Watchdog clock prescaler */
    .enableUpdate = TRUE, /* Update register enabled */
    .clockSource = kWDOG_LpoClockSource, /* Watchdog clock source is LPO 1KHz */
#if defined(FSL_FEATURE_WDOG_HAS_WAITEN) && FSL_FEATURE_WDOG_HAS_WAITEN
    .workMode.enableWait = TRUE, /* Enable watchdog in wait mode */
#endif
    .workMode.enableStop = FALSE, /* Enable watchdog in stop mode */
    .workMode.enableDebug = FALSE, /* Disable watchdog in debug mode */
};

static WDOG_Type *wdog_base = WDOG;
#endif

/*!< reset MCU timestamp <microseconds> */
static uint64_t gSwResetTimestamp = 0;
/*!< boolean -  nvm format */
static bool_t gResetToFactory = FALSE;
/*==================================================================================================
 Public global variables declarations
 ==================================================================================================*/

taskMsgQueue_t appThreadMsgQueue;
/*==================================================================================================
 Public functions
 ==================================================================================================*/
/*==================================================================================================
 ==================================================================================================*/
void main_task(uint32_t param)
{
    static uint8_t mainInitialized = FALSE;

    if (!mainInitialized)
    {
        mainInitialized = TRUE;

#if gDCDC_Enabled_d
        /* Init DCDC module */
        BOARD_DCDCInit();
#endif

#if WDOG_ENABLE
        /* Init watchdog module */
        APP_WDOG_Init();
#endif
        /* Init memory blocks manager */
        MEM_Init();

        SecLib_Init();
        /* Init  timers module */
        TMR_Init();
        TMR_TimeStampInit();
        /* Init Led module */
        LED_Init();
#if NVM_NG_ENABLED
        /* Non volatile memory module init */
        if (gNVM_OK_c != NvModuleInit())
        {
            panic(0, (uint32_t) NvModuleInit, 0, 0);
        }
#endif /* NVM_NG_ENABLED */
        /* Init phy module */
        Phy_Init();
        /* RNG must be initialized after the PHY is Initialized */
        if (gRngSuccess_d != RNG_Init())
        {
            panic(0, (uint32_t) RNG_Init, 0, 0);
        }
        /* Init mac module */
        MAC_Init();
        /* Initialize Thread Module */
        THR_Init();
#if gLpmIncluded_d
        PWR_Init();
        PWR_DisallowDeviceToSleep();
#endif
        /* Initialize Keyboard (Switches) Module */
        KBD_Init(KBD_Callback);
        SerialManager_Init();

#if THREAD_USE_SHELL
        SHELLComm_Init(&appThreadMsgQueue);
#endif

#if THREAD_USE_THCI
        FSCI_Init((void*)&mFsciSerials);
#endif

#if THREAD_USE_THCI
        APP_FsciInterface(&appThreadMsgQueue);
#endif        
        
#if gHybridApp_d
        /* Initialize ble and peripheral drivers specific to the ble application */
        BleApp_Init();
#endif

        /* Init demo application */
        APP_Init();
    }

    /* Main Application Loop (idle state) */
    while (1)
    {
#if WDOG_ENABLE
        /* Restart the watchdog so it doesn't reset */
        APP_WDOG_Refresh();
#endif
#if NVM_NG_ENABLED
        /* Process NV Storage save-on-idle, save-on-count and save-on-interval requests */
        NvIdle();
#endif
        /* Debug Checks, Leader LED restore check */
        DBG_Check();
#if gHybridApp_d
        /* Ble app */
        BleApp_Thread(0);
#endif
        /* Application handler */
        APP_Handler();
        /* Treat low power */
#if gLpmIncluded_d
        APP_HandleLowPowerOnIdle();
#endif
        /* Reset MCU */
        APP_HandleMcuResetOnIdle();
        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }
}

/*!*************************************************************************************************
 \fn     APP_ResetMcuOnTimeout
 \brief  Reset the MCU on timeout
 \param  [in]    timeoutMs  timeout in milliseconds
 \param  [in]    resetToFactory
 \return         None
 ***************************************************************************************************/
void APP_ResetMcuOnTimeout(uint32_t timeoutMs, bool_t resetToFactory)
{
    gResetToFactory = resetToFactory;
    gSwResetTimestamp = TMR_GetTimestamp();
    gSwResetTimestamp += (timeoutMs * 1000); /* microseconds*/
}

/*!*************************************************************************************************
 \fn     APP_GetResetMcuTimeout
 \brief  Return the interval time until a MCU reset occurs
 \return  the time interval; 0 means that no Mcu reset was programmed
 ***************************************************************************************************/
uint32_t APP_GetResetMcuTimeout(void)
{
    uint32_t timeInterval = 0;

    if (gSwResetTimestamp > TMR_GetTimestamp())
    {
        timeInterval = (uint32_t) ((gSwResetTimestamp - TMR_GetTimestamp())
                / 1000);
    }

    return timeInterval;
}
/*!*************************************************************************************************
\fn     void APP_CriticalExitCb(uint32_t location, uint32_t param)
\brief  If the stack is in a deadlock situation, it calls APP_CriticalExitCb.

\param  [in]  location  Address where the Panic occurred
\param  [in]  param     Parameter with extra debug information
***************************************************************************************************/
void APP_CriticalExitCb
(
    uint32_t location,
    uint32_t param
)
{
   panic(0, location, param, 0);
   ResetMCU();
}

/*==================================================================================================
 Private functions
 ==================================================================================================*/
/*!*************************************************************************************************
 \fn  static void KBD_Callback(uint8_t events)
 \brief  This is a callback function called from the KBD module.

 \param  [in]    events  value of the events

 \return         void
 ***************************************************************************************************/
static void KBD_Callback(uint8_t events)
{
    if ( NULL != pfAppKeyboardHandler)
    {
        /* memory optimisation - app keyboard handler handles the pointer as an events mask*/
        uint32_t msg = (uint32_t) events;
        (void) NWKU_SendMsg(pfAppKeyboardHandler, (void*) msg,
                &appThreadMsgQueue);
    }
}

/*!*************************************************************************************************
 \fn     APP_HandleMcuResetOnIdle
 \brief  Reset the MCU on idle
 \param  [in]
 \return         None
 ***************************************************************************************************/
static void APP_HandleMcuResetOnIdle(void)
{
    if ((gSwResetTimestamp) && (gSwResetTimestamp < TMR_GetTimestamp()))
    {
        gSwResetTimestamp = 0;

        /* disable interrupts */
        //OSA_InterruptDisable();

#if THR_SOFTWARE_RESET_ENABLE
        THR_SoftwareReset(0, gResetToFactory);
        /* inform application */
        THCI_ResetCpuEvent(gResetCpuSuccess_c, 0);
#else
        if (gResetToFactory)
        {
#if NVM_NG_ENABLED
            /* Erase NVM Datasets */
            NvFormat();
#endif
        }
        ResetMCU();
#endif
        /* Enable interrupts */
        OSA_InterruptEnable();
    }
}

/*!*************************************************************************************************
 \fn     APP_HandleLowPowerOnIdle
 \brief  Handle low power on idle
 \param  [in]
 \return         None
 ***************************************************************************************************/
#if gLpmIncluded_d
static void APP_HandleLowPowerOnIdle(void)
{
    if( PWR_CheckIfDeviceCanGoToSleep() )
    {
        PWRLib_WakeupReason_t wakeupReason;
        wakeupReason = PWR_EnterLowPower();
        if( wakeupReason.Bits.FromKeyBoard )
        {
            /* Protection to the LLWD pin enabled on both edges */
            static bool_t wakeUpFlag = FALSE;
            if(TRUE == wakeUpFlag)
            {
                wakeUpFlag = FALSE;
                App_SedWakeUpFromKeyBoard();
            }
            else
            {
                wakeUpFlag = TRUE;
            }
            PWR_AllowDeviceToSleep();
        }
    }
}
#endif

/*!*************************************************************************************************
 \fn     static void APP_WDOG_Init(void)
 \brief  Init watch dog if enabled
 ***************************************************************************************************/
#if WDOG_ENABLE
static void APP_WDOG_Init(void)
{

    uint32_t i=0;

    WDOG_Init(wdog_base, &wdogConfig);
    /* Accessing register by bus clock */
    for (i = 0; i < 256; i++)
    {
        (void)WDOG->RSTCNT;
    }
}

/*!*************************************************************************************************
 \fn     static void APP_WDOG_Refresh(void)
 \brief  Refresh watch dog if enabled
 ***************************************************************************************************/

static void APP_WDOG_Refresh(void)
{
    uint32_t wdogTimer = (uint32_t)((((uint32_t)wdog_base->TMROUTH) << 16U) | (wdog_base->TMROUTL));
    /* Restart the watchdog so it doesn't reset */
    if(wdogTimer > (wdogConfig.timeoutValue >> 3U))
    {
        WDOG_Refresh(wdog_base);
    }
}
#endif
/*==================================================================================================
 Private debug functions
 ==================================================================================================*/
