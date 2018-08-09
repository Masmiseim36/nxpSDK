/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
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

/*!=============================================================================
\file       app_main.c
\brief      app main file
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <stdint.h>
#include "jendefs.h"

//#include "app_cfg.h"
#include "board.h"
#include "pin_mux.h"

/* Fwk */
#include "PWR_Interface.h"
#include "NVM_Interface.h"
#include "LED.h"
#include "Keyboard.h"
#include "SecLib.h"
#include "RNG_Interface.h"
#include "SerialManager.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "fsl_os_abstraction.h"

/* 802.15.4 */
#include "PhyInterface.h"
#include "MacInterface.h"

/* KSDK */
#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "fsl_gpio.h"

/* ZigBee */
#include "ZQueue.h"
#include "ZTimer.h"
#include "portmacro.h"
#include "zps_apl_af.h"
#include "mac_vs_sap.h"
#include "dbg.h"
#include "app_main.h"
#include "app_buttons.h"
#include "app_events.h"
#include "app_zcl_switch_task.h"
#include "app_zlo_switch_node.h"
#include "PDM.h"
#include "app_switch_state_machine.h"

#ifdef DimmerSwitch
#include "App_DimmerSwitch.h"
#endif


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef DEBUG_APP
#define TRACE_APP_MAIN   FALSE
#else
#define TRACE_APP_MAIN   TRUE
#endif
#define TIMER_QUEUE_SIZE             8
#define MLME_QUEQUE_SIZE            10
#define MCPS_QUEUE_SIZE             24
#define APP_QUEUE_SIZE               8
#define BDB_QUEUE_SIZE               3
#define APP_ZTIMER_STORAGE           5

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
instanceId_t mMacInstance;

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

PUBLIC uint8 u8TimerPoll;
PUBLIC uint8 u8TimerButtonScan;
PUBLIC uint8 u8TimerButtonDelay;
PUBLIC uint8 u8TimerTick;
PUBLIC uint8 u8TimerChangeMode;
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
PUBLIC uint8 u8TimerNtag;
#endif

PUBLIC tszQueue APP_msgAppEvents;
PUBLIC tszQueue APP_msgBdbEvents;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/* Character that triggers sending of a Device Info Message on the serial terminal interface */
#define gMessageMarkSendDevInfo    0x01

#ifndef mDeviceInfo
#define mDeviceInfo "Kinetis KWxx Dimmer Switch"
#endif


PRIVATE ZTIMER_tsTimer asTimers[APP_ZTIMER_STORAGE + BDB_ZTIMER_STORAGE];

PRIVATE zps_tsTimeEvent asTimeEvent[TIMER_QUEUE_SIZE];
PRIVATE MAC_tsMcpsVsDcfmInd asMacMcpsDcfmInd[MCPS_QUEUE_SIZE];
PRIVATE MAC_tsMlmeVsDcfmInd asMacMlmeVsDcfmInd[MLME_QUEQUE_SIZE];

PRIVATE BDB_tsZpsAfEvent asBdbEvent[BDB_QUEUE_SIZE];
PRIVATE APP_tsEvent asAppEvent[APP_QUEUE_SIZE];

PRIVATE void KBD_Callback( uint8_t events);
void BOARD_InitHardware(void);
void App_init(void);
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

extern const uint8_t gUseRtos_c;

extern void zps_taskZPS(void);
extern void PWRM_vManagePower(void);
extern void vTimerServerInit(void);
extern void vAppMain(void);

static void App_CommRxCallBack( void *pData ) 
{
    uint8_t pressedKey;    
    uint16_t count;        
    (void)pData;    
    (void)Serial_GetByteFromRxBuffer( APP_SerialId, &pressedKey, &count ); 
    
    if( count && gMessageMarkSendDevInfo == pressedKey )    
    {       
        /* Print device info */        
        Serial_Print( APP_SerialId, mDeviceInfo, gAllowToBlock_d );    
    }
}

/****************************************************************************
 *
 * NAME: main_task
 *
 * DESCRIPTION:
 * Main application loop
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
 void main_task (uint32_t parameter)
{
    static uint8_t initialized = FALSE;

    if(!initialized)
    {
        vTimerServerInit();
        #if 0
        {
            cop_config_t configCop;
            COP_GetDefaultConfig(&configCop);
            /* timeout cycle - kCOP_2Power5CyclesOr2Power13Cycles gives approx 8 sec timeout and
             * kCOP_2Power8CyclesOr2Power16Cycles gives approx 64 sec timeout */
            /* Set watch dog feed time constant to approximately 8s */
            configCop.timeoutCycles = kCOP_2Power5CyclesOr2Power13Cycles;
            COP_Init(SIM, &configCop);
        }
        #endif
        initialized = TRUE;
        //hardware_init();
    
        /* Init memory blocks manager */
        MEM_Init();
        /* Init  timers module */
        TMR_Init();
        LED_Init();
        SecLib_Init();
        SerialManager_Init();
        
        /* Bind to MAC layer */
        mMacInstance = BindToMAC( (instanceId_t)0 );
        //Mac_RegisterSapHandlers( MCPS_NWK_SapHandler, MLME_NWK_SapHandler, mMacInstance );
        App_init();

        vAppMain();
    }

    while(1)
    {
        zps_taskZPS();
        bdb_taskBDB();
        ZTIMER_vTask();
        APP_taskSwitch();

        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed with in the watchdog period
         * otherwise the system will be reset.
         */
#ifdef CPU_MKW41Z512VHT4
        COP_Refresh(SIM);
#else
        WWDT_Refresh(WWDT);
#endif

#if gNvStorageIncluded_d
        NvIdle();
#endif
        /*
         * suspends CPU operation when the system is idle or puts the device to
         * sleep if there are no activities in progress
         */
#if cPWR_UsePowerDownMode
        PWRM_vManagePower();
#endif

        if(!gUseRtos_c)
        {
            break;
        }
    }
}


/*****************************************************************************
* Initialization function for the App Task. This is called during
* initialization and should contain any application specific initialization
* (ie. hardware initialization/setup, table initialization, power up
* notificaiton.
*
* Interface assumptions: None
*
* Return value: None
*
*****************************************************************************/
void App_init(void)
{
    /* Register keyboard callback function */
    KBD_Init( KBD_Callback );
    
    /* Initialize the serial terminal interface so that we can print out status messages */
    Serial_InitInterface( &APP_SerialId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE );
    Serial_SetBaudRate  ( APP_SerialId, gUARTBaudRate115200_c );
    Serial_SetRxCallBack( APP_SerialId, App_CommRxCallBack, NULL );

    /*signal app ready*/  
    //LED_StartSerialFlash(LED1);
}

/****************************************************************************
 *
 * NAME: APP_vSetUpHardware
 *
 * DESCRIPTION:
 * Set up interrupts
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vSetUpHardware(void)
{
    /* hardware initialization of KW41 device */
    BOARD_InitHardware();
    BOARD_InitPins();

    vTimerServerInit();

    if (RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog)
    {
        //DBG_vPrintf(TRACE_APP, "APP: Watchdog timer has reset device!\r\n");
        COP_Disable(SIM);
        while(1);
    }
    else
    {
        cop_config_t configCop;
        COP_GetDefaultConfig(&configCop);
        /* timeout cycle - kCOP_2Power5CyclesOr2Power13Cycles gives approx 8 sec timeout and
        * kCOP_2Power8CyclesOr2Power16Cycles gives approx 64 sec timeout */
        /* Set watch dog feed time constant to approximately 8s */
        configCop.timeoutCycles = kCOP_2Power5CyclesOr2Power13Cycles;
        COP_Init(SIM, &configCop);
    }
    //vAppRegisterPWRMCallbacks();
}

/****************************************************************************
 *
 * NAME: APP_vInitResources
 *
 * DESCRIPTION:
 * Initialise resources (timers, queue's etc)
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitResources(void)
{

    /* Initialise the Z timer module */
    ZTIMER_eInit(asTimers, sizeof(asTimers) / sizeof(ZTIMER_tsTimer));

    /* Create Z timers */
    ZTIMER_eOpen(&u8TimerPoll,          APP_cbTimerPoll,        NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerButtonDelay,   APP_cbTimerButtonDelay, NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerTick,          APP_cbTimerZclTick,     NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerChangeMode,    APP_cbTimerChangeMode,  NULL, ZTIMER_FLAG_PREVENT_SLEEP);
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    ZTIMER_eOpen(&u8TimerNtag,          APP_cbNtagTimer,        NULL, ZTIMER_FLAG_PREVENT_SLEEP);
#endif
    /* Create all the queues */
    ZQ_vQueueCreate(&APP_msgBdbEvents,          BDB_QUEUE_SIZE,         sizeof(BDB_tsZpsAfEvent),   (uint8*)asBdbEvent);
    ZQ_vQueueCreate(&APP_msgAppEvents,          APP_QUEUE_SIZE,         sizeof(APP_tsEvent),        (uint8*)asAppEvent);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,        MLME_QUEQUE_SIZE,       sizeof(MAC_tsMlmeVsDcfmInd),(uint8*)asMacMlmeVsDcfmInd);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,        MCPS_QUEUE_SIZE,        sizeof(MAC_tsMcpsVsDcfmInd),(uint8*)asMacMcpsDcfmInd);
    ZQ_vQueueCreate(&zps_TimeEvents,            TIMER_QUEUE_SIZE,       sizeof(zps_tsTimeEvent),    (uint8*)asTimeEvent);
}

#ifdef CPU_MKW41Z512VHT4
bool_t APP_GetByteFromSerial(uint8 * byte)
{
    uint16_t len = 0;
    
    Serial_GetByteFromRxBuffer(APP_SerialId, byte, &len);
    
    return (bool_t)len;
}
#endif

/****************************************************************************
 *
 * NAME: APP_vInitLeds
 *
 * DESCRIPTION:
 * Initialises LED's
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitLeds(void)
{
}

/****************************************************************************
 *
 * NAME: APP_vSetLED
 *
 * DESCRIPTION:
 * Initialises LED's
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vSetLED(uint8 u8Led, bool_t bOn)
{
    if (bOn)
    {
        Led_TurnOn(u8Led);
    }
    else
    {
        Led_TurnOff(u8Led);
    }
}


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

PRIVATE void KBD_Callback( uint8_t events)
{
    APP_tsEvent sButtonEvent;
    sButtonEvent.uEvent.sButton.u32DIOState = APP_BUTTONS_DIO_MASK;
    sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;
    
    //DBG_vPrintf(TRACE_APP_MAIN, "KBD_Callback events=%d\r\n", events);   
     
    switch(events)
    {
        case gKBD_EventPB1_c:
            /* SW1 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW1;
            sButtonEvent.uEvent.sButton.u32DIOState &= ~ON; // Down: 021103 , up: 021903
            break;
        case gKBD_EventPB2_c:
            /* SW2 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW2;
            sButtonEvent.uEvent.sButton.u32DIOState &= ~OFF; // Down: 020903 , up: 021903
            break;
        case gKBD_EventPB3_c:
            /* SW3 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW3;
            sButtonEvent.uEvent.sButton.u32DIOState &= ~UP; // Down: 021903 , up: 020903
            break;
        case gKBD_EventPB4_c:
            /* SW4 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW4;
            sButtonEvent.uEvent.sButton.u32DIOState &= ~DOWN; // Down: 021903 , up: 021901
            break;
        case gKBD_EventLongPB1_c:            
            return;
        case gKBD_EventLongPB2_c:
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_1;
            sButtonEvent.uEvent.sButton.u32DIOState &= ~COMM; // Down: 021903 , up: 020903
            break;          
        case gKBD_EventLongPB3_c:
        case gKBD_EventLongPB4_c:            
            return;
        case gKBD_EventVeryLongPB1_c: /* factory reset */            
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_1;
            sButtonEvent.eType = APP_E_EVENT_POR_FACTORY_RESET;
            break;
        case gKBD_EventVeryLongPB2_c:            
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_1;
            sButtonEvent.uEvent.sButton.u32DIOState &= ~COMM; // Down: 021903 , up: 020903
            break;
        case gKBD_EventVeryLongPB3_c:
        case gKBD_EventVeryLongPB4_c:
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_1;
            break;
        default:
            return;
    }

    DBG_vPrintf(TRACE_APP_MAIN, "Button DN=%d\r\n", sButtonEvent.uEvent.sButton.u8Button);   
     
    if(ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent) == FALSE)
    {
        DBG_vPrintf(TRACE_APP_MAIN, "Button: Failed to post Event %d \r\n", sButtonEvent.eType);
    }
}
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
