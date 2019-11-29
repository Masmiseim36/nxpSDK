/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_light_sensor_buttons.c
\brief      Button Press detection (Implementation)
==================================================================================================*/

#if 0
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "ZTimer.h"
#include "ZQueue.h"
#include "dbg.h"
//TODO
//#include "AppHardwareApi.h"
#include "app_events.h"
#include "app_main.h"
#include "pwrm.h"
#include "app_light_sensor_buttons.h"
#include "app_light_sensor_state_machine.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_APP_BUTTON
    #define TRACE_APP_BUTTON               TRUE
#else
    #define TRACE_APP_BUTTON               FALSE
#endif

#define WAKE_FROM_DEEP_SLEEP     (1<<11)
#define DIO_STATE_NVM_LOCATION     0

/* Defines the number of bits sampled for a button debounce event. One sample taken for each bit set */
#define APP_BUTTON_SAMPLE_MASK          (0x1f)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE uint32 u32DioInterrupts = 0;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: APP_bButtonInitialise
 *
 * DESCRIPTION:
 * Button Initialization
 *
 * PARAMETER: void
 *
 * RETURNS: bool
 *
 ****************************************************************************/
PUBLIC void APP_bButtonInitialise(void)
{
    /* Set DIO lines to inputs with buttons connected */
    vAHI_DioSetDirection(APP_BUTTONS_DIO_MASK, 0);

    /* Turn on pull-ups for DIO lines with buttons connected */
    vAHI_DioSetPullup(APP_BUTTONS_DIO_MASK, 0);

    if (FALSE == (u16AHI_PowerStatus() & WAKE_FROM_DEEP_SLEEP))
    {
        /* Set the edge detection for falling edges */
        vAHI_DioWakeEdge(0, APP_BUTTONS_DIO_MASK);
    }
    else
    {
        u32PreviousDioState = u32AHI_ReadNVData(DIO_STATE_NVM_LOCATION);
        DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Woke from deep sleep, previous Dio State = %08x", u32PreviousDioState);
    }

    /* Enable interrupts to occur on selected edge */
    vAHI_DioWakeEnable(APP_BUTTONS_DIO_MASK, 0);
}


/****************************************************************************
 *
 * NAME: APP_ButtonsScanTask
 *
 * DESCRIPTION:
 * An Sensor specific Debounce task. In the real world this maybe
 * removed as the digital input will be driven from a sensor.
 *
 * PARAMETER:
 *
 * RETURNS:
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerButtonScan(void *pvParam)
{
    uint8 u8Button;
    uint32 u32DioInput = 0;
    bool_t bButtonDebounceComplete = TRUE;

    /* Clear any existing interrupt pending flags */
    (void)u32AHI_DioInterruptStatus();

    if(u32DioInterrupts != 0)
    {
        u32DioInput = (u32PreviousDioState ^ u32DioInterrupts) & APP_BUTTONS_DIO_MASK;
    }
    else
    {
        u32DioInput = u32AHI_DioReadInput();
    }

    DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: APP_ButtonsScanTask, Buttons = %08x, Interrupts = %08x, Previous = %08x", APP_BUTTONS_DIO_MASK & u32DioInput, (u32DioInterrupts & APP_BUTTONS_DIO_MASK), u32PreviousDioState);

    u32DioInterrupts = 0;

    /* Loop over all buttons to check their Dio states*/
    for (u8Button = 0; u8Button < APP_BUTTONS_NUM; u8Button++)
    {

        /* Shift the previous debounce checks and add the new debounce reading*/
        s_u8ButtonDebounce[u8Button] <<= 1;
        s_u8ButtonDebounce[u8Button] |= (u32DioInput & s_u32ButtonDIOLine[u8Button]) ? TRUE : FALSE;
        s_u8ButtonDebounce[u8Button] &= APP_BUTTON_SAMPLE_MASK;

        DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Button %d, Debounce = %02x, Dio State = %08x", u8Button, s_u8ButtonDebounce[u8Button], u32PreviousDioState);

        /* If previously the button was down but now it is up, post an event to the queue */
        if (((u32PreviousDioState & s_u32ButtonDIOLine[u8Button]) == 0) && (s_u8ButtonDebounce[u8Button] == APP_BUTTON_SAMPLE_MASK))
        {
            APP_tsEvent sButtonEvent;
            sButtonEvent.eType = APP_E_EVENT_BUTTON_UP;
            sButtonEvent.uEvent.sButton.u8Button = u8Button;
            sButtonEvent.uEvent.sButton.u32DIOState = u32DioInput;

            /* Save the new state */
            u32PreviousDioState |= s_u32ButtonDIOLine[u8Button];
            DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Button UP=%d, Dio State = %08x", u8Button, u32PreviousDioState);

            /* Post a message to the stack*/
            ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent);

        }
        /* If previously the button was up but now it is down, post an event to the queue */
        else if (((u32PreviousDioState & s_u32ButtonDIOLine[u8Button]) != 0) && (s_u8ButtonDebounce[u8Button] == 0x0))
        {
            APP_tsEvent sButtonEvent;
            sButtonEvent.eType = APP_E_EVENT_BUTTON_DOWN;
            sButtonEvent.uEvent.sButton.u8Button = u8Button;
            sButtonEvent.uEvent.sButton.u32DIOState = u32DioInput;

            /* Save the new state */
            u32PreviousDioState &= ~s_u32ButtonDIOLine[u8Button];

            DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Button DN=%d, Dio State = %08x", u8Button, u32PreviousDioState);

            /* Post a message to the stack*/
            ZQ_bQueueSend(&APP_msgAppEvents, &sButtonEvent);

        }

        /* Still debouncing this button, clear flag to indicate more samples are required */
        else if(((s_u8ButtonDebounce[u8Button] != 0) && (s_u8ButtonDebounce[u8Button] != APP_BUTTON_SAMPLE_MASK)))
        {
            bButtonDebounceComplete &= FALSE;
        }

    }


    /* If all buttons are in a stable state, stop the scan timer and set the new interrupt edge requirements */
    if(bButtonDebounceComplete == TRUE)
    {
        /* Stop the scan timer as we have finished */
        ZTIMER_eStop(u8TimerButtonScan);

        /* Set the new interrupt edge requirements */
        vAHI_DioWakeEdge((APP_BUTTONS_DIO_MASK & ~u32PreviousDioState), (APP_BUTTONS_DIO_MASK & u32PreviousDioState));

        /* Re enable DIO wake interrupts on all buttons */
        vAHI_DioInterruptEnable(APP_BUTTONS_DIO_MASK, 0);

        DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Debounce complete, timer stopped, interrupts re-enabled, previous state %08x", u32PreviousDioState);
        DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Wake edges: Rising=%08x Falling=%08x", (APP_BUTTONS_DIO_MASK & ~u32PreviousDioState), (APP_BUTTONS_DIO_MASK & u32PreviousDioState));

    }
    else
    {
        DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Debounce in progress, timer continued");
        ZTIMER_eStart(u8TimerButtonScan, ZTIMER_TIME_MSEC(3));
    }

}


/****************************************************************************
 *
 * NAME: vActionOnButtonActivationAfterDeepSleep
 *
 * DESCRIPTION:
 * When we wake up, we have restarted so we need to manually check to see
 * what Dio woke us. Start the ButtonScanTask and disable wake interrupts
 *
 ****************************************************************************/
PUBLIC void vActionOnButtonActivationAfterDeepSleep(void)
{
    DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Interrupt Status = %08x", u32AHI_DioInterruptStatus());
    u32DioInterrupts |= APP_BUTTONS_DIO_MASK;
    vAHI_DioWakeEnable(0, APP_BUTTONS_DIO_MASK);
    /* Begin debouncing the buttons */
    APP_cbTimerButtonScan(NULL);
}

/****************************************************************************
 *
 * NAME: vSaveDioStateBeforeDeepSleep
 *
 * DESCRIPTION:
 * Due to us going to sleep on a falling edge as well as a rising edge, we need
 * to save the Dio state into NVM so when we wake back up we know what edge we
 * had configured to wake us up.
 *
 ****************************************************************************/
PUBLIC void vSaveDioStateBeforeDeepSleep(void)
{
    DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Writing %08x to NVM", u32PreviousDioState);
    vAHI_WriteNVData(DIO_STATE_NVM_LOCATION, u32PreviousDioState);
    DBG_vPrintf(TRACE_APP_BUTTON, "\nAPP Button: Written %08x to NVM", u32AHI_ReadNVData(DIO_STATE_NVM_LOCATION));
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif