/*! *********************************************************************************
* \addtogroup Beacon
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Beacon application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "LED.h"
#include "Keyboard.h"
#include "MemManager.h"
#include "SerialManager.h"
#include "FunctionLib.h"
#include "TimersManager.h"
#include "fsl_os_abstraction.h"
#include "RNG_Interface.h"
#include "SecLib.h"
#include "Messaging.h"
#include "Panic.h"

/* Bluetooth Low Energy */
#include "ble_init.h"

/* Application */
#include "hci_black_box.h"

#ifdef CPU_QN908X
#include "controller_interface.h"
#include "fsl_wdt.h"
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
#if !MULTICORE_APPLICATION_CORE
#define STATIC  static
#else
#define STATIC  
#endif

/* Application Events */
#define gAppEvtMsgFromHostStack_c       (1 << 0)
#define gAppEvtAppCallback_c            (1 << 1)

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t platformInitialized = 0;
static osaEventId_t  mAppEvent;

/* Application input queues */
static anchor_t mHostAppInputQueue;
static anchor_t mAppCbInputQueue;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void App_Thread (uint32_t param);
STATIC void App_GenericCallback (gapGenericEvent_t* pGenericEvent);

#ifndef CPU_QN908X
#if !gUseHciTransportDownward_d
static void BLE_SignalFromISRCallback(void);
#endif /* gUseHciTransportDownward_d */
#endif /* CPU_QN908X */

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/

#ifndef CPU_QN908X
#if !gUseHciTransportDownward_d
extern void (*pfBLE_SignalFromISR)(void);
#endif /* gUseHciTransportDownward_d */
#endif /* CPU_QN908X */

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
    TMR_TimeStampInit();
}

/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize 
*         the system
*
* \param[in]  param
*
********************************************************************************** */
void main_task(uint32_t param)
{
    if (!platformInitialized)
    {
        platformInitialized = 1;
        
#ifdef CPU_QN908X
        /* Initialize QN9080 Controller */
        BLE_Init(gAppMaxConnections_c);
#endif /* CPU_QN908X */
        
        /* Framework init */
        MEM_Init();
        TMR_Init();
        LED_Init();
        
#if !MULTICORE_APPLICATION_CORE
        /* Cryptogrpahic and RNG hardware initialization */
        SecLib_Init();    
        /* RNG software initialization and PRNG initial seeding (from hardware) */
        RNG_Init();
        RNG_SetPseudoRandomNoSeed(NULL);
#endif /* MULTICORE_APPLICATION_CORE */
        
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
        KBD_Init(App_KeyboardCallBack);
#endif

#if gAppUseNvm_d        
        /* Initialize NV module */
        NvModuleInit();
#endif

#ifndef CPU_QN908X   
#if !gUseHciTransportDownward_d
        pfBLE_SignalFromISR = BLE_SignalFromISRCallback;        
#endif /* !gUseHciTransportDownward_d */
#endif /* CPU_QN908X */
        
#if (cPWR_UsePowerDownMode || gAppUseNvm_d)
#if (!mAppIdleHook_c)
        AppIdle_TaskInit();
#endif
#endif
#if (cPWR_UsePowerDownMode)
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
        
#if !(MULTICORE_CONNECTIVITY_CORE)
        /* BLE Host Stack Init */
        if (Ble_Initialize(App_GenericCallback) != gBleSuccess_c)
        {
            panic(0,0,0,0);
            return;
        }
#endif /* MULTICORE_CONNECTIVITY_CORE */
    }
    
    /* Call application task */
    App_Thread( param );
}

#ifndef CPU_QN908X 
#if !gUseHciTransportDownward_d
/* Called by BLE when a connect is received */
static void BLE_SignalFromISRCallback(void)
{
#if (cPWR_UsePowerDownMode)  
    PWR_DisallowDeviceToSleep();  
#endif /* cPWR_UsePowerDownMode */
}
#endif /* !gUseHciTransportDownward_d */
#endif /* CPU_QN908X */

/*! *********************************************************************************
* \brief  This function represents the Application task. 
*         This task reuses the stack alocated for the MainThread.
*         This function is called to process all events for the task. Events 
*         include timers, messages and any other user defined events.
* \param[in]  argument
*
********************************************************************************** */
void App_Thread (uint32_t param)
{
    while(1)
    {
        /* For BareMetal break the while(1) after 1 run */
        if( gUseRtos_c == 0 )
        {
            break;
        }
    }
}

STATIC void App_GenericCallback (gapGenericEvent_t* pGenericEvent)
{

}
/*! *********************************************************************************
* @}
********************************************************************************** */
