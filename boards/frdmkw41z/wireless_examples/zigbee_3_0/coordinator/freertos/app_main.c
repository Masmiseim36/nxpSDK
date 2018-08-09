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
#include "pwrm.h"
#include "portmacro.h"
#include "zps_apl_af.h"
#include "mac_vs_sap.h"
#include "dbg.h"
#include "app_coordinator.h"
#include "app_zb_shell.h"
#include "app_leds.h"
#include "app_buttons.h"
#include "app_events.h"
#include "app_main.h"
#include "app_zcl_task.h"
#include "bdb_api.h"
#ifdef APP_NCI_ICODE
#include "app_nci_icode.h"
#endif
#ifdef APP_NCI_AES
#include "app_nci_aes.h"
#endif
#ifdef APP_NTAG_I2C_PLUS
/* NTAG middleware module */
#include "HAL_I2C_driver.h"
#include "ntag_driver.h"
#include "ntag_driver_intern.h"
#include "ntag_bridge.h"
#include "nfc_device.h"
#include "ntag_defines.h"
#include "ndef_message.h"
/* NTAG APP */
#include "app_ntag_i2c_plus.h"
#endif


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define BDB_QUEUE_SIZE               3
#if (defined APP_NCI_ICODE) || (defined APP_NCI_AES)
#define APP_ZTIMER_STORAGE           4 /* NCI: Added timer */
#else
#define APP_ZTIMER_STORAGE           3
#endif
#define TIMER_QUEUE_SIZE             8
#define MLME_QUEQUE_SIZE             9
#define MCPS_QUEUE_SIZE             24
#define APP_QUEUE_SIZE               1
#define TX_QUEUE_SIZE               10
#define RX_QUEUE_SIZE               90


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PRIVATE void KBD_Callback( uint8_t events);
#ifdef APP_NTAG_I2C_PLUS
static void vAPP_NTAG_Callback(APP_NTAG_Events_t event);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

PUBLIC uint8 u8TimerPoll;
PUBLIC uint8 u8TimerId;
PUBLIC uint8 u8TimerZCL;
#if (defined APP_NCI_ICODE) || (defined APP_NCI_AES)
PUBLIC uint8 u8TimerNci;
#endif

PUBLIC tszQueue APP_msgBdbEvents;
PUBLIC tszQueue APP_msgAppEvents;
extern uint8     APP_SerialId;

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
extern void vAppMain(void);
PRIVATE void APP_vSetUpWdog(void);

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

void main_task (uint32_t parameter)
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

        app_zb_shell_init();
        vAppMain();
#ifdef APP_NTAG_I2C_PLUS
    APP_NTAG_RegisterCallback(vAPP_NTAG_Callback);
#endif
    }

    while(1)
    {

         /* place event handler code here... */
        zps_taskZPS();
        bdb_taskBDB();
        ZTIMER_vTask();

        APP_taskCoordinator();

        app_zb_shell_task();

        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed with in the watchdog period
         * otherwise the system will be reset.
         */

         /* Kick the watchdog */
        COP_Refresh(SIM);

#if gNvStorageIncluded_d
        NvIdle();
#endif

        if(!gUseRtos_c)
        {
            break;
        }
    }
}

/****************************************************************************
 *
 * NAME: vAPP_NTAG_Callback
 *
 * DESCRIPTION:
 * NTAG application callback. Called from app_ntag module when an NTAG event
 * has occured
 *
 * PARAM(S):
 * [IN] event - the NTAG event
 * RETURNS:
 * void
 *
 ****************************************************************************/
#ifdef APP_NTAG_I2C_PLUS
static void vAPP_NTAG_Callback(APP_NTAG_Events_t event)
{
  uint8_t* pu8InstallCode = NULL;
  uint64_t u64Addr;
  uint8_t u8Status;

  switch(event)
  {
  case NTAG_READY_c:
  case NTAG_COMM_ERROR_c:
  case NTAG_ICODE_INVALID_c:
    break;

  case NTAG_ICODE_VALID_c:
    /* get the Install Code */
    pu8InstallCode = APP_NTAG_GetInstallCode();

    if(NULL == pu8InstallCode)
    {
      break;
    }

    /* get device Extended Address */
    u64Addr = APP_NTAG_GetDeviceExtAddress();

    /* Install the new code */
    u8Status = ZPS_eAplZdoAddReplaceInstallCodes( u64Addr, pu8InstallCode, 16, ZPS_APS_UNIQUE_LINK_KEY);
    DBG_vPrintf(TRUE, "Key Added for %016llx Status %02x\r\n", u64Addr, u8Status);
    break;

  default:
    break;
  }
}
#endif /* #ifdef APP_NTAG_I2C_PLUS */
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
    ZTIMER_eOpen(&u8TimerZCL,           APP_cbTimerZclTick ,    NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerId,            APP_cbTimerId,          NULL, ZTIMER_FLAG_PREVENT_SLEEP);
#if (defined APP_NCI_ICODE) || (defined APP_NCI_AES)
    ZTIMER_eOpen(&u8TimerNci,           APP_cbNciTimer,         NULL, ZTIMER_FLAG_PREVENT_SLEEP);
#endif

    /* Create all the queues */
#ifdef CPU_MKW41Z512VHT4
    ZQ_vQueueCreate(&APP_msgAppEvents,     APP_QUEUE_SIZE,     sizeof(APP_tsEvent),        NULL );
    ZQ_vQueueCreate(&APP_msgBdbEvents,     BDB_QUEUE_SIZE,     sizeof(BDB_tsZpsAfEvent),   NULL );
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,   MLME_QUEQUE_SIZE,   sizeof(MAC_tsMlmeVsDcfmInd),NULL );
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,   MCPS_QUEUE_SIZE,    sizeof(MAC_tsMcpsVsDcfmInd),NULL );
    ZQ_vQueueCreate(&zps_TimeEvents,       TIMER_QUEUE_SIZE,   sizeof(zps_tsTimeEvent),    NULL );
#else
    ZQ_vQueueCreate(&APP_msgAppEvents,     APP_QUEUE_SIZE,     sizeof(APP_tsEvent),        (uint8*)asAppEvent);
    ZQ_vQueueCreate(&APP_msgBdbEvents,     BDB_QUEUE_SIZE,     sizeof(BDB_tsZpsAfEvent),   (uint8*)asBdbEvent);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,   MLME_QUEQUE_SIZE,   sizeof(MAC_tsMlmeVsDcfmInd),(uint8*)asMacMlmeVsDcfmInd);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,   MCPS_QUEUE_SIZE,    sizeof(MAC_tsMcpsVsDcfmInd),(uint8*)asMacMcpsDcfmInd);
    ZQ_vQueueCreate(&zps_TimeEvents,       TIMER_QUEUE_SIZE,   sizeof(zps_tsTimeEvent),    (uint8*)asTimeEvent);
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
            /* SW5 on FRDM-KW41Z*/
            sButtonEvent.uEvent.sButton.u8Button = APP_E_BUTTONS_BUTTON_SW1;
            break;
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

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
