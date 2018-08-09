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
#include "ZQueue.h"
#include "ZTimer.h"
#include "portmacro.h"
#include "zps_apl_af.h"
#include "mac_vs_sap.h"
#ifdef CPU_MKW41Z512VHT4
#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "board.h"
#include "PWR_Interface.h"
#include "NVM_Interface.h"
#include "TimersManager.h"
#include "Keyboard.h"
#include "LED.h"
#include "MemManager.h"
#include "SecLib.h"
#include "SerialManager.h"
#include "RNG_Interface.h"
#else
#include "fsl_wwdt.h"
#include "fsl_iocon.h"
#endif
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "app_cfg.h"
#include "dbg.h"
#include "app_main.h"
#include "app_buttons.h"
#include "app_events.h"
#include "app_zcl_light_task.h"

#include "app_manage_temperature.h"
#include "PDM.h"
#include "app_zlo_light_node.h"
#include "app_power_on_counter.h"

#ifdef Light_ColorLight
#include "App_Light_ColorLight.h"
#endif

#ifdef Light_DimmableLight
#include "App_Light_DimmableLight.h"
#endif

#ifdef Light_DimmablePlug
#include "App_Light_DimmablePlug.h"
#endif

#ifdef Light_ExtendedColorLight
#include "App_Light_ExtendedColorLight.h"
#endif

#ifdef Light_ColorTemperatureLight
#include "APP_Light_ColorTemperatureLight.h"
#endif

#ifdef Light_OnOffLight
#include "APP_Light_OnOffLight.h"
#endif

#ifdef Light_OnOffPlug
#include "APP_Light_OnOffPlug.h"
#endif

#ifdef APP_NTAG
#include "app_ntag.h"
#endif

#include "pwrm.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef DEBUG_APP
#define TRACE_APP   FALSE
#else
#define TRACE_APP   TRUE
#endif

#define TIMER_QUEUE_SIZE             8
#define MLME_QUEQUE_SIZE             8
#define MCPS_QUEUE_SIZE             20
#define ZPS_QUEUE_SIZE                  1
#define BDB_QUEUE_SIZE               2
#define APP_QUEUE_SIZE                  8

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void KBD_Callback( uint8_t events);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


PUBLIC uint8 u8TimerButtonScan;
PUBLIC uint8 u8TimerRadioRecal;
PUBLIC uint8 u8TimerTick;
PUBLIC uint8 u8TimerPowerOn;

#define APP_NUM_STD_TMRS            4

#ifdef CLD_GREENPOWER
    PUBLIC uint8 u8GPTimerTick;
    #define APP_NUM_GP_TMRS             1
    #define GP_TIMER_QUEUE_SIZE         2
#else
    #define APP_NUM_GP_TMRS             0
#endif

#ifdef APP_NTAG
    PUBLIC uint8 u8TimerNtag;
    #define APP_NUM_NTAG_TMRS          1
#else
    #define APP_NUM_NTAG_TMRS          0
#endif

#ifdef OTA_CLD_ATTR_REQUEST_DELAY
    PUBLIC uint8 u8TimerZclMsTick;
    #define APP_NUM_ZCL_MS_TMRS         1
#else
    #define APP_NUM_ZCL_MS_TMRS         0
#endif

#define APP_ZTIMER_STORAGE   (APP_NUM_STD_TMRS + APP_NUM_GP_TMRS + APP_NUM_NTAG_TMRS + APP_NUM_ZCL_MS_TMRS)

PUBLIC tszQueue APP_msgBdbEvents;
PUBLIC tszQueue APP_msgAppEvents;


/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE ZTIMER_tsTimer asTimers[APP_ZTIMER_STORAGE + BDB_ZTIMER_STORAGE];

#ifndef CPU_MKW41Z512VHT4
PRIVATE zps_tsTimeEvent asTimeEvent[TIMER_QUEUE_SIZE];
PRIVATE MAC_tsMcpsVsDcfmInd asMacMcpsDcfmInd[MCPS_QUEUE_SIZE];
PRIVATE MAC_tsMlmeVsDcfmInd asMacMlmeVsDcfmInd[MLME_QUEQUE_SIZE];
PRIVATE BDB_tsZpsAfEvent asBdbEvent[BDB_QUEUE_SIZE];
PRIVATE APP_tsLightEvent asAppEvent[APP_QUEUE_SIZE];
#ifdef CLD_GREENPOWER
uint8 au8GPZCLEvent[ GP_TIMER_QUEUE_SIZE];
#endif
#endif

#ifdef CLD_GREENPOWER
PUBLIC tszQueue APP_msgGPZCLTimerEvents;
uint8 u8GPZCLTimerEvent;
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern void vTimerServerInit(void);
extern void zps_taskZPS(void);
extern void PWRM_vManagePower(void);
extern void vAppMain(void);
extern void OSA_TimeInit(void);
extern const uint8_t gUseRtos_c;

static volatile uint8_t wdt_update_count;

gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput, 1,
};
#ifndef CPU_MKW41Z512VHT4
void System_IRQHandler(void)
{
    uint32_t wdtStatus = WWDT_GetStatusFlags(WWDT);

    /* The chip should reset before this happens. For this interrupt to occur,
     * it means that the WD timeout flag has been set but the reset has not occurred  */
    if (wdtStatus & kWWDT_TimeoutFlag)
    {
        /* A watchdog feed didn't occur prior to window timeout */
        /* Stop WDT */
        WWDT_Disable(WWDT);
        WWDT_ClearStatusFlags(WWDT, kWWDT_TimeoutFlag);
        DBG_vPrintf(TRUE, "Timeout Flag\r\n");
    }

    /* Handle warning interrupt */
    if (wdtStatus & kWWDT_WarningFlag)
    {
        /* A watchdog feed didn't occur prior to warning timeout */
        WWDT_ClearStatusFlags(WWDT, kWWDT_WarningFlag);
        if (wdt_update_count < 3)
        {
        	/* Feed only for the first 3 warnings then allow for a WDT reset to occur */
            wdt_update_count++;
            DBG_vPrintf(TRUE,"Watchdog warning flag\r\n");
        }
    }
}
#endif

void main_task (uint32_t parameter)
{
    /* e.g. osaEventFlags_t ev; */
    static uint8_t initialized = FALSE;
    if(!initialized)
    {
        vTimerServerInit();
        {
            cop_config_t configCop;
            COP_GetDefaultConfig(&configCop);
            /* timeout cycle - kCOP_2Power5CyclesOr2Power13Cycles gives approx 8 sec timeout and
             * kCOP_2Power8CyclesOr2Power16Cycles gives approx 64 sec timeout */
            /* Set watch dog feed time constant to approximately 8s */
            configCop.timeoutCycles = kCOP_2Power5CyclesOr2Power13Cycles;
            COP_Init(SIM, &configCop);
        }
        /* place initialization code here... */
        /*myEventId = OSA_EventCreate(TRUE);*/
        initialized = TRUE;
#ifdef CPU_MKW41Z512VHT4
        MEM_Init();
        RNG_Init();
        SecLib_Init();
        TMR_Init();
        /* Initialize Keyboard (Switches) Module */
        KBD_Init(KBD_Callback);
        LED_Init();
#if defined(DBG_ENABLE)
        SerialManager_Init();
        Serial_InitInterface(&APP_SerialId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
        Serial_SetBaudRate  (APP_SerialId,  APP_SERIAL_INTERFACE_SPEED);
#endif
#endif
        vAppMain();
    }

    while(1)
    {
        DBG_vPrintf(FALSE, "APP: Entering zps_taskZPS\n");
        zps_taskZPS();
        DBG_vPrintf(FALSE, "APP: Entering bdb_taskBDB\n");
        bdb_taskBDB();
        DBG_vPrintf(FALSE, "APP: Entering ZTIMER_vTask\n");
        ZTIMER_vTask();

        DBG_vPrintf(FALSE, "APP: Entering APP_taskLight\n");
        APP_taskLight();
        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed within the watchdog period
         * otherwise the system will be reset.
         */
        /* Kick the watchdog */
#ifdef CPU_MKW41Z512VHT4
        COP_Refresh(SIM);
#else
        WWDT_Refresh(WWDT);
#endif

        /*
         * suspends CPU operation when the system is idle or puts the device to
         * sleep if there are no activities in progress
         */
#ifdef CPU_MKW41Z512VHT4
        NvIdle();
#endif
        PWRM_vManagePower();

        if(!gUseRtos_c)
        {
            break;
        }
    }
}

#if 0
void hardware_init(void)
{
	APP_vSetUpHardware();
}
#endif
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
#ifndef CPU_MKW41Z512VHT4
    wwdt_config_t config;
    /* Enable DMA access to RAM (assuming default configuration and MAC
     * buffers not in first block of RAM) */
    *(volatile uint32 *)0x40001000 = 0xE000F733;
    *(volatile uint32 *)0x40001004 = 0x109;
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Enable ASYNC bus access */
    CLOCK_EnableAPBBridge();

    /* Start crystal */
    CLOCK_EnableClock(kCLOCK_Xtal32M);
    CLOCK_EnableClock(kCLOCK_Xtal32k);

    WWDT_GetDefaultConfig(&config);
    /* Replace default config values where required */

    /* Set watchdog feed time constant to approximately 16s */
    config.clock = kOSC32K_to_WDT_CLK;
    config.divisor = 1 ;
    config.timeoutValue = 480000;
    /* Set watchdog warning time to 512 ticks after feed time constant */
    config.warningValue = 512;
    /*Ignore the window*/
    config.windowValue = 480000 ;
    /* Configure WWDT to reset on timeout */
    config.enableWatchdogReset = true;

    WWDT_Init(WWDT, &config);
    WWDT_ClearStatusFlags(WWDT, 0xfc);
    /* First feed starts the watchdog */
    WWDT_Refresh(WWDT);
    NVIC_EnableIRQ(WDT_BOD_IRQn);
#else
    /* hardware initialization of KW41 device */
    BOARD_InitHardware();
    BOARD_InitPins();
    //BOARD_InitDebugConsole();
    vTimerServerInit();

    if (RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog)
    {
    	DBG_vPrintf(TRACE_APP, "APP: Watchdog timer has reset device!\r\n");
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
#endif
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

       DBG_vPrintf(TRACE_APP, "APP: Initialising resources...\n");

    /* Initialise the Z timer module */
    ZTIMER_eInit(asTimers, sizeof(asTimers) / sizeof(ZTIMER_tsTimer));


    /* Create Z timers */
#ifndef CPU_MKW41Z512VHT4
    ZTIMER_eOpen(&u8TimerButtonScan,    APP_cbTimerButtonScan,      NULL, ZTIMER_FLAG_PREVENT_SLEEP);
#endif
    ZTIMER_eOpen(&u8TimerRadioRecal,    APP_cbTimerRadioRecal,      NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerTick,          APP_cbTimerZclTick,         NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerPowerOn,       APP_cbTimerPowerCount,      NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #ifdef CLD_GREENPOWER
        ZTIMER_eOpen(&u8GPTimerTick,    APP_cbTimerGPZclTick,       NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #endif
    #ifdef APP_NTAG
        ZTIMER_eOpen(&u8TimerNtag,      APP_cbNtagTimer,            NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #endif
    #ifdef OTA_CLD_ATTR_REQUEST_DELAY
        ZTIMER_eOpen(&u8TimerZclMsTick, APP_cbTimerZclMsTick,       NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #endif


    /* Create all the queues */
#ifdef CPU_MKW41Z512VHT4
    ZQ_vQueueCreate(&APP_msgBdbEvents,           BDB_QUEUE_SIZE,         sizeof(BDB_tsZpsAfEvent),   NULL);
    ZQ_vQueueCreate(&APP_msgAppEvents,           APP_QUEUE_SIZE,         sizeof(APP_tsLightEvent),   NULL);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,         MLME_QUEQUE_SIZE,       sizeof(MAC_tsMlmeVsDcfmInd),NULL);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,         MCPS_QUEUE_SIZE,        sizeof(MAC_tsMcpsVsDcfmInd),NULL);
    ZQ_vQueueCreate(&zps_TimeEvents,             TIMER_QUEUE_SIZE,       sizeof(zps_tsTimeEvent),    NULL);
#ifdef CLD_GREENPOWER
    ZQ_vQueueCreate(&APP_msgGPZCLTimerEvents,    GP_TIMER_QUEUE_SIZE,    sizeof(uint8),              NULL);
#endif
    
#else        
    ZQ_vQueueCreate(&APP_msgBdbEvents,           BDB_QUEUE_SIZE,         sizeof(BDB_tsZpsAfEvent),   (uint8*)asBdbEvent);
    ZQ_vQueueCreate(&APP_msgAppEvents,           APP_QUEUE_SIZE,         sizeof(APP_tsLightEvent),   (uint8*)asAppEvent);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,         MLME_QUEQUE_SIZE,       sizeof(MAC_tsMlmeVsDcfmInd),(uint8*)asMacMlmeVsDcfmInd);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,         MCPS_QUEUE_SIZE,        sizeof(MAC_tsMcpsVsDcfmInd),(uint8*)asMacMcpsDcfmInd);
    ZQ_vQueueCreate(&zps_TimeEvents,             TIMER_QUEUE_SIZE,       sizeof(zps_tsTimeEvent),    (uint8*)asTimeEvent);
#ifdef CLD_GREENPOWER
    ZQ_vQueueCreate(&APP_msgGPZCLTimerEvents,    GP_TIMER_QUEUE_SIZE,    sizeof(uint8),              (uint8*)au8GPZCLEvent);
#endif
#endif    
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: main
 *
 * DESCRIPTION:
 * Entry point into the application.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void KBD_Callback( uint8_t events)
{
	APP_tsEvent sButtonEvent;

    switch(events)
    {
#ifdef BUILD_OTA
            case gKBD_EventPB1_c:
            /* SW4 on FRDM-KW41Z*/
              sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW2;
            break;
            case gKBD_EventPB2_c:
            /* SW3 on FRDM-KW41Z*/
              sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW3;
            break;
#else
        case gKBD_EventPB1_c:
          /* do nothing*/
           return;
        case gKBD_EventPB2_c:
            /* SW4 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW2;
            break;
        case gKBD_EventPB3_c:
            /* SW3 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW3;
            break;
        case gKBD_EventPB4_c:
            /* SW2 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW4;
        	break;
#endif
        case gKBD_EventLongPB1_c:
        case gKBD_EventLongPB2_c:
        case gKBD_EventLongPB3_c:
        case gKBD_EventLongPB4_c:
        	return;
        case gKBD_EventVeryLongPB1_c: /* factory reset */
        case gKBD_EventVeryLongPB2_c:
        case gKBD_EventVeryLongPB3_c:
        case gKBD_EventVeryLongPB4_c:
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_1;
        	break;
        default:
            return;
    }
    
    sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;

    DBG_vPrintf(TRACE_APP, "Button DN=%d\n", sButtonEvent.uEvent.sButton.u8Button);

    if(ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent) == FALSE)
    {
        DBG_vPrintf(TRACE_APP, "Button: Failed to post Event %d \n", sButtonEvent.eType);
    }
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
