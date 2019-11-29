/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <stdint.h>
#include "jendefs.h"
#include "app_cfg.h"
#include "pin_mux.h"
#include "ZQueue.h"
#include "ZTimer.h"
#include "portmacro.h"
#include "zps_apl_af.h"
#include "mac_vs_sap.h"

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

#include "dbg.h"
#include "app_main.h"
#include "app_events.h"
#include "app_buttons.h"
#include "app_zcl_controller_task.h"
#include "app_zcl_controller_task.h"
#include "PDM.h"
#include "zlo_controller_node.h"
#include "app_led_control.h"
#include "bdb_api.h"

#ifdef DR1199
#include "app_buttons.h"
#include "app_serial_interface.h"
#include "app_menu_handler.h"
#endif


#ifdef Controller_ColorController
#include "App_Controller_ColorController.h"
#endif

#ifdef Controller_ColorSceneController
#include "App_Controller_ColorSceneController.h"
#endif

#ifdef Controller_NonColorController
#include "App_Controller_NonColorController.h"
#endif

#ifdef Controller_NonColorSceneController
#include "App_Controller_NonColorSceneController.h"
#endif

#ifdef Controller_OnOffSensor
#include "App_Controller_OnOffSensor.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef DEBUG_APP
#define TRACE_APP   TRUE
#else
#define TRACE_APP   FALSE
#endif

#define TIMER_QUEUE_SIZE             8
#define MLME_QUEQUE_SIZE            10
#define MCPS_QUEUE_SIZE             24
#define APP_QUEUE_SIZE               8
#define BDB_QUEUE_SIZE               3
#ifdef DR1199
#define TX_QUEUE_SIZE               10
#define RX_QUEUE_SIZE               30
#endif

#ifndef DR1199
#define APP_ZTIMER_STORAGE           5
#else
#define APP_ZTIMER_STORAGE           6
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

PUBLIC uint8 u8TimerLedBlink;
PUBLIC uint8 u8TimerId;
PUBLIC uint8 u8TimerAddGroup;
PUBLIC uint8 u8TimerPoll;
PUBLIC uint8 u8TimerZCL;

PUBLIC tszQueue APP_msgAppEvents;
PUBLIC tszQueue APP_msgBdbEvents;
#ifdef DR1199
PUBLIC tszQueue APP_msgSerialTx;
PUBLIC tszQueue APP_msgSerialRx;
#endif

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
extern void zps_taskZPS(void);
extern void PWRM_vManagePower(void);
extern void vAppMain(void);
extern void OSA_TimeInit(void);
extern const uint8_t gUseRtos_c;

/****************************************************************************
 *
 * NAME: APP_vMainLoop
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

        initialized = TRUE;
    
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

#ifdef CPU_MKW41Z512VHT4
        Serial_InitInterface(&APP_SerialId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
        Serial_SetBaudRate  (APP_SerialId,  APP_SERIAL_INTERFACE_SPEED);
#endif
        vAppMain();
    }

    while (TRUE)
    {
        zps_taskZPS();

        bdb_taskBDB();

        ZTIMER_vTask();

#ifdef DR1199
        APP_taskAtSerial();
#endif

        APP_taskController();

        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed within the watchdog period
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
        //PWRM_vManagePower();
#endif

        if(!gUseRtos_c)
        {
            break;
        }
    }
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
    ZTIMER_eOpen(&u8TimerLedBlink,      APP_cbTimerLedBlink,    NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerId,            APP_cbTimerId,          NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerAddGroup,      APP_cbTimerAddGroup,    NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerPoll,          APP_cbTimerPoll,        NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerZCL,           APP_cbTimerZclTick,     NULL, ZTIMER_FLAG_PREVENT_SLEEP);

    /* Create all the queues */
#ifdef CPU_MKW41Z512VHT4
    ZQ_vQueueCreate(&APP_msgBdbEvents,        BDB_QUEUE_SIZE,         sizeof(BDB_tsZpsAfEvent),    NULL);
    ZQ_vQueueCreate(&APP_msgAppEvents,        APP_QUEUE_SIZE,         sizeof(APP_tsEvent),         NULL);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,      MLME_QUEQUE_SIZE,       sizeof(MAC_tsMlmeVsDcfmInd), NULL);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,      MCPS_QUEUE_SIZE,        sizeof(MAC_tsMcpsVsDcfmInd), NULL);
    ZQ_vQueueCreate(&zps_TimeEvents,          TIMER_QUEUE_SIZE,        sizeof(zps_tsTimeEvent),    NULL);
#else
    ZQ_vQueueCreate(&APP_msgBdbEvents,           BDB_QUEUE_SIZE,         sizeof(BDB_tsZpsAfEvent),   (uint8*)asBdbEvent);
    ZQ_vQueueCreate(&APP_msgAppEvents,           APP_QUEUE_SIZE,         sizeof(APP_tsEvent),        (uint8*)asAppEvent);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,         MLME_QUEQUE_SIZE,       sizeof(MAC_tsMlmeVsDcfmInd),(uint8*)asMacMlmeVsDcfmInd);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,         MCPS_QUEUE_SIZE,        sizeof(MAC_tsMcpsVsDcfmInd),(uint8*)asMacMcpsDcfmInd);
    ZQ_vQueueCreate(&zps_TimeEvents,             TIMER_QUEUE_SIZE,       sizeof(zps_tsTimeEvent),    (uint8*)asTimeEvent);
#endif
    
#ifdef DR1199
    //ZQ_vQueueCreate(&APP_msgSerialTx,            TX_QUEUE_SIZE,   sizeof(uint8),    (uint8*)au8TxBuffer);
    //ZQ_vQueueCreate(&APP_msgSerialRx,            RX_QUEUE_SIZE,   sizeof(uint8),    (uint8*)au8RxBuffer);
#endif
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
PRIVATE void KBD_Callback( uint8_t events)
{
    APP_tsEvent sButtonEvent;

    switch(events)
    {
        case gKBD_EventPB1_c:
            /* SW5 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = BUTTON_SW5;
            break;
        case gKBD_EventPB2_c:
            /* SW4 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = BUTTON_SW4;
            break;
        case gKBD_EventPB3_c:
            /* SW3 on FRDM-KW41Z */
            sButtonEvent.uEvent.sButton.u8Button = BUTTON_SW3;
            break;
        case gKBD_EventPB4_c:
            /* SW2 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = BUTTON_SW2;
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
    }

    sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;

    DBG_vPrintf(TRUE, "Button DN=%d\n", sButtonEvent.uEvent.sButton.u8Button);

    if(ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent) == FALSE)
    {
        DBG_vPrintf(TRUE, "Button: Failed to post Event %d \n", sButtonEvent.eType);
    }
}

/****************************************************************************
 *
 * NAME: APP_GetByteFromSerial
 *
 * DESCRIPTION:
 * Return bytes receive in serial buffer
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
bool_t APP_GetByteFromSerial(uint8 * byte)
{
    uint16_t len = 0;
    
    Serial_GetByteFromRxBuffer(APP_SerialId, byte, &len);
    
    return (bool_t)len;
}

PUBLIC void App_SedWakeUpFromKeyBoard(void)
{
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
