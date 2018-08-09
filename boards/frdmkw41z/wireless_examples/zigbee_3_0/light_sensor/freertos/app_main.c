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

/*!=================================================================================================
\file       app_main.c
\brief      ZLO Main handler (Implementation)
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <stdint.h>
#include "jendefs.h"

#include "app_cfg.h"

#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "board.h"
#include "PWR_Interface.h"
#include "NVM_Interface.h"
#include "TimersManager.h"
#include "Keyboard.h"
#include "LED.h"
#include "SecLib.h"
#include "MemManager.h"
#include "SerialManager.h"

#include "fsl_gpio.h"
#include "pin_mux.h"
#include "ZQueue.h"
#include "ZTimer.h"
#include "portmacro.h"
#include "zps_apl_af.h"
#include "mac_vs_sap.h"
#include "dbg.h"
#include "app_main.h"
#include "app_buttons.h"
#include "app_light_sensor_buttons.h"
#include "app_events.h"
#include "app_zcl_sensor_task.h"
#include "app_light_sensor_state_machine.h"
#include "PDM.h"
#include "app_zlo_sensor_node.h"
#include "app_nwk_event_handler.h"
#include "app_power_on_counter.h"
#include "app_blink_led.h"
#include "App_LightSensor.h"
#include "app_sleep_handler.h"
#ifdef APP_NTAG_ICODE
#include "app_ntag_icode.h"
#endif
#ifdef APP_NTAG_AES
#include "app_ntag_aes.h"
#endif

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
#define MCPS_QUEUE_SIZE              24
#define ZPS_QUEUE_SIZE               1
#define ZCL_QUEUE_SIZE               1
#define APP_QUEUE_SIZE               8
#define BDB_QUEUE_SIZE               3
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
#define APP_ZTIMER_STORAGE           7 /* NTAG: Added timer */
#else
#define APP_ZTIMER_STORAGE           6
#endif

#ifndef APP_MIN_WAKE_TIME
#define APP_MIN_WAKE_TIME            3000
#endif

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
PUBLIC uint8 u8TimerMinWake;
PUBLIC uint8 u8TimerPoll;
PUBLIC uint8 u8TimerPowerOnCount;
PUBLIC uint8 u8TimerLightSensorSample;
PUBLIC uint8 u8TimerTick;
PUBLIC uint8 u8TimerBlink;
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
PUBLIC uint8 u8TimerNtag;
#endif
PUBLIC tszQueue APP_msgZpsEvents;
PUBLIC tszQueue APP_msgZclEvents;
PUBLIC tszQueue APP_msgAppEvents;
PUBLIC tszQueue APP_msgBdbEvents;

PUBLIC bool_t APP_bPersistantPolling = FALSE;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE ZTIMER_tsTimer asTimers[APP_ZTIMER_STORAGE + BDB_ZTIMER_STORAGE];

extern const uint8_t gUseRtos_c;
static volatile uint8_t wdt_update_count;
gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput, 0,
};

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern void vTimerServerInit(void);
extern void vAppMain(void);
PRIVATE void APP_vSetUpWdog(void);

extern void zps_taskZPS(void);
extern void PWRM_vManagePower(void);

/*start of file*/
/****************************************************************************
 *
 * NAME: main_task
 *
 * DESCRIPTION:
 * Main  execution loop
 *
 * RETURNS:
 * Never
 *
 ****************************************************************************/
void main_task (void const * parameter)
{
    static uint8_t initialized = FALSE;

    if(!initialized)
    {
        initialized = TRUE;

        vTimerServerInit();

        /* Init memory blocks manager */
        MEM_Init();
        SecLib_Init();
        /* Init  timers module */
        TMR_Init();
        SerialManager_Init();
        /* Init Led module */
        LED_Init();
        /* Initialize Keyboard (Switches) Module */
        KBD_Init(KBD_Callback);
        /* Wdog/Cop Setup */
        APP_vSetUpWdog();

        Serial_InitInterface(&APP_SerialId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
        Serial_SetBaudRate  (APP_SerialId,  APP_SERIAL_INTERFACE_SPEED);

        vAppMain();
    }

    while(1)
    {
        DBG_vPrintf(FALSE, "ZPS\n");
        zps_taskZPS();

        DBG_vPrintf(FALSE, "APP: Entering bdb_taskBDB\n");
        bdb_taskBDB();

        DBG_vPrintf(FALSE, "TMR\n");
        ZTIMER_vTask();

        DBG_vPrintf(FALSE, "ZLO\n");
        APP_taskSensor();

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

        /*
         * suspends CPU operation when the system is idle or puts the device to
         * sleep if there are no activities in progress
         */
#ifdef CPU_MKW41Z512VHT4
        NvIdle();
#endif

#if cPWR_UsePowerDownMode
        /* See if we are able to sleep or not */
        vAttemptToSleep();
#endif

        if(!gUseRtos_c)
        {
            break;
        }
    }
}

/****************************************************************************
 *
 * NAME: APP_vSetUpWdog
 *
 * DESCRIPTION:
 * Set up Wdog 
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void APP_vSetUpWdog(void)
{

    if (RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog)
    {
        DBG_vPrintf(TRUE, "APP: Watchdog timer has reset device!\r\n");
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
    ZTIMER_eOpen(&u8TimerMinWake,           NULL,                           NULL,   ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerLightSensorSample, APP_cbTimerLightSensorSample,   NULL,   ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerBlink,             vAPP_cbBlinkLED,                NULL,   ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerPoll,              APP_cbTimerPoll,                NULL,   ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerTick,              APP_cbTimerZclTick,             NULL,   ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerPowerOnCount,      vAPP_cbTimerPowerOnCount,       NULL,   ZTIMER_FLAG_PREVENT_SLEEP);
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    ZTIMER_eOpen(&u8TimerNtag,              APP_cbNtagTimer,                NULL,   ZTIMER_FLAG_PREVENT_SLEEP);
#endif

    /* Create all the queues */
#ifdef CPU_MKW41Z512VHT4
    ZQ_vQueueCreate(&APP_msgAppEvents,     APP_QUEUE_SIZE,     sizeof(APP_tsEvent),        NULL );
    ZQ_vQueueCreate(&APP_msgBdbEvents,     BDB_QUEUE_SIZE,     sizeof(BDB_tsZpsAfEvent),   NULL );
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,   MLME_QUEQUE_SIZE,   sizeof(MAC_tsMlmeVsDcfmInd),NULL );
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,   MCPS_QUEUE_SIZE,    sizeof(MAC_tsMcpsVsDcfmInd),NULL );
    ZQ_vQueueCreate(&zps_TimeEvents,       TIMER_QUEUE_SIZE,   sizeof(zps_tsTimeEvent),    NULL );
#else
    ZQ_vQueueCreate(&APP_msgBdbEvents,      BDB_QUEUE_SIZE,     sizeof(BDB_tsZpsAfEvent),   (uint8*)asBdbEvent);
    ZQ_vQueueCreate(&APP_msgZpsEvents,      ZPS_QUEUE_SIZE,     sizeof(ZPS_tsAfEvent),      (uint8*)asZpsStackEvent);
    ZQ_vQueueCreate(&APP_msgZclEvents,      ZCL_QUEUE_SIZE,     sizeof(ZPS_tsAfEvent),      (uint8*)asZclStackEvent);
    ZQ_vQueueCreate(&APP_msgAppEvents,      APP_QUEUE_SIZE,     sizeof(APP_tsEvent),        (uint8*)asAppEvent);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,    MLME_QUEQUE_SIZE,   sizeof(MAC_tsMlmeVsDcfmInd),(uint8*)asMacMlmeVsDcfmInd);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,    MCPS_QUEUE_SIZE,    sizeof(MAC_tsMcpsVsDcfmInd),(uint8*)asMacMcpsDcfmInd);
    ZQ_vQueueCreate(&zps_TimeEvents,        TIMER_QUEUE_SIZE,   sizeof(zps_tsTimeEvent),    (uint8*)asTimeEvent);
#endif
}

#ifdef CPU_MKW41Z512VHT4
bool_t APP_GetByteFromSerial(uint8 * byte)
{
    uint16_t len = 0;
    
    Serial_GetByteFromRxBuffer(APP_SerialId, byte, &len);
    
    return (bool_t)len;
}
#endif

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

PRIVATE void KBD_Callback( uint8_t events)
{
    APP_tsEvent sButtonEvent;

    switch(events)
    {
#ifdef BUILD_OTA
        case gKBD_EventPB1_c:
            /* SW5 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW4;
            break;
        case gKBD_EventPB2_c:
            /* SW4 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW2;
            break;
#else
        case gKBD_EventPB1_c:
            /* SW5 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW1;
            break;
        case gKBD_EventPB2_c:
            /* SW4 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW2;
            break;
        case gKBD_EventPB3_c:
            /* SW3 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW3;
            break;
        case gKBD_EventPB4_c:
            /* SW2 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW4;
            break;
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
#endif
    }

    sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;

    DBG_vPrintf(TRUE, "Button DN=%d\n", sButtonEvent.uEvent.sButton.u8Button);

    if(ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent) == FALSE)
    {
        DBG_vPrintf(TRUE, "Button: Failed to post Event %d \n", sButtonEvent.eType);
    }
}

PUBLIC void App_SedWakeUpFromKeyBoard(void)
{
    ZTIMER_eStart(u8TimerMinWake, APP_MIN_WAKE_TIME);
    vWakeCallBack();
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
