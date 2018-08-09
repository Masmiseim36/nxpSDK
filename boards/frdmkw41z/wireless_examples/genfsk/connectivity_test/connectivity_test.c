/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This is a source file for the main application.
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
/* Drv */
#include "LED.h"
#include "Keyboard.h"

/* Fwk */
#include "MemManager.h"
#include "TimersManager.h"
#include "RNG_Interface.h"
#include "Messaging.h"
#include "SecLib.h"
#include "Panic.h"
#include "fsl_xcvr.h"
#include "fsl_os_abstraction.h"

/* KSDK */
#include "board.h"

#ifdef cPWR_UsePowerDownMode
#if (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#endif
#endif

#ifdef FSL_RTOS_FREE_RTOS
#include "FreeRTOSConfig.h"
#endif

#include "genfsk_interface.h"
#include "gen_fsk_tests.h"

#include "menus.h"
#include "connectivity_test.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gAppNumberOfTests_d (3)
#define App_NotifySelf() OSA_EventSet(mAppThreadEvt, gCtEvtSelfEvent_c)

#ifdef FSL_RTOS_FREE_RTOS
#if (configUSE_IDLE_HOOK)
#define mAppIdleHook_c 1
#endif
#endif

#ifndef mAppIdleHook_c
#define mAppIdleHook_c 0
#endif

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/
typedef bool_t ( * pCtTestFunction)(ct_event_t evt, void* pAssocData);
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
/*Application main*/
static void App_Thread (uint32_t param); 
/*Application event handler*/
static void App_HandleEvents(osaEventFlags_t flags);
/*Function that reads latest byte from Serial Manager*/
static void App_UpdateUartData(uint8_t* pData);
/*Application Init*/
static void App_InitApp();
/*Print shortcut menu values*/
static void App_PrintTestParameters(bool_t bEraseLine);
/*Generic FSK RX callback*/
static void App_GenFskReceiveCallback(uint8_t *pBuffer, 
                                      uint16_t bufferLength, 
                                      uint64_t timestamp, 
                                      uint8_t rssi,
                                      uint8_t crcValid);
/*Generic FSK Notification callback*/
static void App_GenFskEventNotificationCallback(genfskEvent_t event, 
                                                genfskEventStatus_t eventStatus);
/*Serial Manager UART RX callback*/
static void App_SerialCallback(void* param);
/*Application Thread notification function (sends event to application task)*/
static void App_NotifyAppThread(void);
/*Timer callback*/
static void App_TimerCallback(void* param);

/*Handles all keyboard events*/
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)   
static void App_KeyboardCallBack(uint8_t events);
#endif

#if (cPWR_UsePowerDownMode)
#if (mAppIdleHook_c)
#define AppIdle_TaskInit()
#define App_Idle_Task()
#else
static osaStatus_t AppIdle_TaskInit(void);
static void App_Idle_Task(osaTaskParam_t argument);
#endif /* (mAppIdleHook_c) */
#endif /* (cPWR_UsePowerDownMode) */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t platformInitialized = 0;
/*event used by the application thread*/
static osaEventId_t mAppThreadEvt;
/*variable to store key pressed by user*/
static uint8_t mAppUartData = 0;
/*variable to store button pressed by user*/
static uint8_t mAppPbData = PB_INVALID;
/*application state*/
static app_states_t mAppState = gAppStateInit_c;
/*set TRUE when user presses [ENTER] on logo screen*/
static bool_t mAppStartApp = FALSE;
/*pointer to test currently running*/
static pCtTestFunction pfCtCurrentTest = NULL;
/*pointer to data associated to each event*/
static void* pEvtAssociatedData = NULL;
/*array of existing tests*/
static pCtTestFunction ppfCtAvailableTests[gAppNumberOfTests_d];
/*structure to store information regarding latest received packet*/
static ct_rx_indication_t mAppRxLatestPacket;
/*latest generic fsk event status*/
static genfskEventStatus_t mAppGenfskStatus;

/*extern GENFSK instance id*/
extern uint8_t mAppGenfskId;
/*extern MCU reset api*/
extern void ResetMCU(void);

#if (cPWR_UsePowerDownMode)
#if (!mAppIdleHook_c)
OSA_TASK_DEFINE( App_Idle_Task, gAppIdleTaskPriority_c, 1, gAppIdleTaskStackSize_c, FALSE );
osaTaskId_t gAppIdleTaskId = 0;
#endif
#endif  /* cPWR_UsePowerDownMode */

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
        uint8_t pseudoRNGSeed[20] = {0};
        
        platformInitialized = 1;
        
        hardware_init();
        
        /* Framework init */
        MEM_Init();
        TMR_Init();
        //initialize Serial Manager
        SerialManager_Init();
        LED_Init();
        SecLib_Init();
        
        RNG_Init();   
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[0])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[4])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[8])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[12])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[16])));
        RNG_SetPseudoRandomNoSeed(pseudoRNGSeed);
        
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)        
        KBD_Init(App_KeyboardCallBack);
#endif
        
        GENFSK_Init();
        
        /* GENFSK LL Init with default register config */
        GENFSK_AllocInstance(&mAppGenfskId, NULL, NULL, NULL);   
        
        /*create app thread event*/
        mAppThreadEvt = OSA_EventCreate(TRUE);
        
#if (cPWR_UsePowerDownMode)
#if (!mAppIdleHook_c)
        AppIdle_TaskInit();
#endif
        PWR_Init();
        PWR_DisallowDeviceToSleep();
#else    
        /*start serial flashing using all LEDs*/
        LED_StartSerialFlash(LED1);
#endif           
        
        /*initialize the application interface id*/
        Serial_InitInterface(&mAppSerId, 
                             APP_SERIAL_INTERFACE_TYPE, 
                             APP_SERIAL_INTERFACE_INSTANCE);
        /*set baudrate to 115200*/
        Serial_SetBaudRate(mAppSerId, 
                           APP_SERIAL_INTERFACE_SPEED);
        /*set Serial Manager receive callback*/
        Serial_SetRxCallBack(mAppSerId, App_SerialCallback, NULL);
        
        /*allocate a timer*/
        mAppTmrId = TMR_AllocateTimer();
        /*Prints the Welcome screens in the terminal*/  
        PrintMenu(cu8Logo, mAppSerId);
    }
    
    /* Call application task */
    App_Thread( param );
}

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
    osaEventFlags_t mAppThreadEvtFlags = 0;
    
    while(1)
    {
        (void)OSA_EventWait(mAppThreadEvt, gCtEvtEventsAll_c, FALSE, osaWaitForever_c ,&mAppThreadEvtFlags);
        if(mAppThreadEvtFlags)
        {
            if(mAppStartApp)
            {
                App_HandleEvents(mAppThreadEvtFlags);/*handle app events*/
            }
            else
            {
                if(mAppThreadEvtFlags & gCtEvtUart_c) /*if uart event*/
                {
                    App_UpdateUartData(&mAppUartData); /*read new byte*/
                    if(mAppUartData == '\r')
                    {
                        mAppStartApp = TRUE;
                        /*notify task again to start running*/
                        App_NotifySelf();
                    }
                    else
                    {
                        /*if other key is pressed show screen again*/
                        PrintMenu(cu8Logo, mAppSerId);
                    }
                }
                
                if(mAppThreadEvtFlags & gCtEvtKBD_c) /*if KBD event*/
                {
                    mAppStartApp = TRUE;
                    /*notify task again to start running*/
                    App_NotifySelf();
                }
            }
        }
        if(gUseRtos_c == 0) /*if bare-metal break while*/
        {
            break;
        } 
    }
}

#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)   
static void App_KeyboardCallBack(uint8_t events)
{
    switch (events)
    {
    case gKBD_EventPB1_c:        
        mAppPbData = PB1_PRESSED;
        break;       
    case gKBD_EventPB2_c:        
        mAppPbData = PB2_PRESSED;
        break;        
    case gKBD_EventLongPB1_c:
        mAppPbData = PB1_LONG_PRESS;
        break;
    case gKBD_EventLongPB2_c:        
        mAppPbData = PB2_LONG_PRESS;
        break;
    default:
        break;
    }
    
    OSA_EventSet(mAppThreadEvt, gCtEvtKBD_c);   
}
#endif

/*! *********************************************************************************
* \brief  The application event handler 
*         This function is called each time there is an OS event for the AppThread
* \param[in]  flags The OS event flags specific to the Connectivity Test App.
*
********************************************************************************** */
void App_HandleEvents(osaEventFlags_t flags)
{
    if(flags & gCtEvtUart_c)
    {
        App_UpdateUartData(&mAppUartData);
        if(CT_IsShortcutMenuEnabled())
        {
            /*if it is a shortcut key clear the flag so it is not handled twice*/
            if(CT_UpdateShortcutKeyParam(mAppUartData))
            {
                App_PrintTestParameters(TRUE);
            }
        }
    }
    switch(mAppState)
    {
    case gAppStateInit_c:
        /*initialize app and map tests*/
        App_InitApp();
        /*enter app default state*/
        mAppState = gAppStateIdle_c;
        /*notify app task to move to new state*/
        App_NotifySelf();
        break;
    case gAppStateIdle_c:
        /*print main menu*/
        PrintMenu(cu8MainMenu, mAppSerId);
        /*print test params*/
        App_PrintTestParameters(FALSE); 
#if (cPWR_UsePowerDownMode)
        LED_StopFlashingAllLeds();
#endif /* (cPWR_UsePowerDownMode) */
        mAppState = gAppStateSelectTest_c;
        break;
    case gAppStateSelectTest_c:
        if(flags & gCtEvtUart_c) /*if uart event*/
        {
            if(mAppUartData >= '1' && 
               mAppUartData <= ('0' + gAppNumberOfTests_d)) /*select which test to run*/
            {
                pfCtCurrentTest = ppfCtAvailableTests[mAppUartData - '1'];
                mAppState = gAppStateRunning_c;
                App_NotifySelf();
            }
            else if(mAppUartData == '!')
            {
                ResetMCU();
            }
        }
        
        break;
    case gAppStateRunning_c: /*event handling for test currently running*/
        if(flags & gCtEvtRxDone_c)
        {
            pEvtAssociatedData = &mAppRxLatestPacket;
            if(pfCtCurrentTest(gCtEvtRxDone_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtTxDone_c)
        {
            pEvtAssociatedData = &mAppGenfskStatus;
            if(pfCtCurrentTest(gCtEvtTxDone_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtRxFailed_c)
        {
            if(pfCtCurrentTest(gCtEvtRxFailed_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtSeqTimeout_c)
        {
            if(pfCtCurrentTest(gCtEvtSeqTimeout_c, NULL))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            } 
        }
        if(flags & gCtEvtUart_c)
        {
            pEvtAssociatedData = &mAppUartData;
            if(pfCtCurrentTest(gCtEvtUart_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtTimerExpired_c)
        {
            pEvtAssociatedData = NULL;
            if(pfCtCurrentTest(gCtEvtTimerExpired_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
#if (cPWR_UsePowerDownMode)
        if(flags & gCtEvtWakeUp_c)
        {
            if(pfCtCurrentTest(gCtEvtWakeUp_c, NULL))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
#endif /* (cPWR_UsePowerDownMode) */
        if(flags & gCtEvtKBD_c)
        {
            pEvtAssociatedData = &mAppPbData;
            if(pfCtCurrentTest(gCtEvtKBD_c, pEvtAssociatedData))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        if(flags & gCtEvtSelfEvent_c)
        {
            if(pfCtCurrentTest(gCtEvtSelfEvent_c, NULL))
            {
                mAppState = gAppStateIdle_c;
                App_NotifySelf();
            }
        }
        break;
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief  This function is called each time SerialManager notifies the application
*         task that a byte was received.
*         The function checks if there are additional bytes in the SerialMgr  
*         queue and simulates a new SM event if there is more data.
* \param[in]  pData Pointer to where to store byte read.
*
********************************************************************************** */
static void App_UpdateUartData(uint8_t* pData)
{
    uint16_t u16SerBytesCount = 0;
    if(gSerial_Success_c == Serial_GetByteFromRxBuffer(mAppSerId, pData, &u16SerBytesCount))
    {
        Serial_RxBufferByteCount(mAppSerId, &u16SerBytesCount);
        if(u16SerBytesCount)
        {
            (void)OSA_EventSet(mAppThreadEvt, gCtEvtUart_c);
        }
    } 
}

/*! *********************************************************************************
* \brief  Application initialization. It installs the main menu callbacks and
*         calls the Connectivity Test for Generic FSK init.
*
********************************************************************************** */
static void App_InitApp()
{   
   ppfCtAvailableTests[0] = CT_ContinuousTests;
   ppfCtAvailableTests[1] = CT_PacketErrorRate;
   ppfCtAvailableTests[2] = CT_RangeTest;

   /*register callbacks for the generic fsk LL */
   GENFSK_RegisterCallbacks(mAppGenfskId,
                            App_GenFskReceiveCallback, 
                            App_GenFskEventNotificationCallback);
   
   /*init and provide means to notify the app thread from connectivity tests*/
   CT_GenFskInit(App_NotifyAppThread, App_TimerCallback);
}

/*! *********************************************************************************
* \brief  This function represents the Generic FSK receive callback. 
*         This function is called each time the Generic FSK Link Layer receives a 
*         valid packet
* \param[in]  pBuffer Pointer to receive buffer as byte array
* \param[in]  timestamp Generic FSK timestamp for received packet
* \param[in]  rssi The RSSI measured during the reception of the packet
*
********************************************************************************** */
static void App_GenFskReceiveCallback(uint8_t *pBuffer, 
                                      uint16_t bufferLength, 
                                      uint64_t timestamp, 
                                      uint8_t rssi,
                                      uint8_t crcValid)
{
   mAppRxLatestPacket.pBuffer      = pBuffer;
   mAppRxLatestPacket.bufferLength = bufferLength;
   mAppRxLatestPacket.timestamp    = timestamp;
   mAppRxLatestPacket.rssi         = rssi;
   mAppRxLatestPacket.crcValid     = crcValid;
   
   /*send event to app thread*/
   OSA_EventSet(mAppThreadEvt, gCtEvtRxDone_c);
}

/*! *********************************************************************************
* \brief  This function represents the Generic FSK event notification callback. 
*         This function is called each time the Generic FSK Link Layer has 
*         a notification for the upper layer
* \param[in]  event The event that generated the notification
* \param[in]  eventStatus status of the event
*
********************************************************************************** */
static void App_GenFskEventNotificationCallback(genfskEvent_t event, 
                                                genfskEventStatus_t eventStatus)
{
   if(event & gGenfskTxEvent)
   {
       mAppGenfskStatus = eventStatus;
       /*send event done*/
       OSA_EventSet(mAppThreadEvt, gCtEvtTxDone_c);
   }
   if(event & gGenfskRxEvent)
   {
       if(eventStatus == gGenfskTimeout)
       {
           OSA_EventSet(mAppThreadEvt, gCtEvtSeqTimeout_c);
       }
       else
       {
           OSA_EventSet(mAppThreadEvt, gCtEvtRxFailed_c);
       }
   }
#if (cPWR_UsePowerDownMode)
   if(event & gGenfskWakeEvent)
   {     
       PWR_DisallowDeviceToSleep();
       OSA_EventSet(mAppThreadEvt, gCtEvtWakeUp_c);
   }
#endif /* (cPWR_UsePowerDownMode) */
   /*not handling other events in this application*/
}

static void App_SerialCallback(void* param)
{
    OSA_EventSet(mAppThreadEvt, gCtEvtUart_c);
}

static void App_NotifyAppThread(void)
{
    App_NotifySelf();
}
static void App_TimerCallback(void* param)
{
    OSA_EventSet(mAppThreadEvt, gCtEvtTimerExpired_c);
}
/*! *********************************************************************************
* \brief  Prints the test parameters: mode, channel, TX power and payload length.
*         Some or all of the above parameters are used throughout the tests.
********************************************************************************** */
static void App_PrintTestParameters(bool_t bEraseLine)
{
   PrintTestParams(gaConfigParams, bEraseLine, mAppSerId);  
}

#if (cPWR_UsePowerDownMode)
static void App_Idle(void)
{
    PWRLib_WakeupReason_t wakeupReason;
    
    if( PWR_CheckIfDeviceCanGoToSleep() )
    {
        /* Enter Low Power */
        wakeupReason = PWR_EnterLowPower();

#if gKBD_KeysCount_c > 0      
        /* Woke up on Keyboard Press */
        if(wakeupReason.Bits.FromKeyBoard)
        {
            KBD_SwitchPressedOnWakeUp();
            PWR_DisallowDeviceToSleep();
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
#if (cPWR_UsePowerDownMode)
    App_Idle();  
#endif  
}
#else /* mAppIdleHook_c */
#if (cPWR_UsePowerDownMode)
static void App_Idle_Task(osaTaskParam_t argument)
{
    while(1)
    {           
        App_Idle();
 
        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }
}

static osaStatus_t AppIdle_TaskInit(void)
{	     
    if(gAppIdleTaskId)
    {      
        return osaStatus_Error;
    }
   
    /* Task creation */
    gAppIdleTaskId = OSA_TaskCreate(OSA_TASK(App_Idle_Task), NULL);
    
    if( NULL == gAppIdleTaskId )
    {
        panic(0,0,0,0);
        return osaStatus_Error;
    }

    return osaStatus_Success;
}
#endif /* cPWR_UsePowerDownMode */
#endif /* mAppIdleHook_c */

