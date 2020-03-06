/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_ostimer.h"
#include "fsl_power.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(OSTIMER0)
#define EXAMPLE_OSTIMER OSTIMER0
#else
#define EXAMPLE_OSTIMER OSTIMER
#endif
#define EXAMPLE_OSTIMER_FREQ CLOCK_GetLpOscFreq()

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_RUNCFG0 (SYSCTL0_PDRUNCFG0_LPOSC_PD_MASK | SYSCTL0_PDSLEEPCFG0_RBB_PD_MASK)
#define APP_DEEPSLEEP_RAM_APD 0xFFFFF8U
#define APP_DEEPSLEEP_RAM_PPD 0x0U
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP                                                                            \
    (((const uint32_t[]){APP_DEEPSLEEP_RUNCFG0,                                                                   \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_PPD_MASK), \
                         APP_DEEPSLEEP_RAM_APD, APP_DEEPSLEEP_RAM_PPD}))
#define EXAMPLE_OSTIMER_IRQn OS_EVENT_IRQn
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep(void);


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool matchFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_EnterDeepSleep(void)
{
    /* Enable deep sleep IRQ. */
    EnableDeepSleepIRQ(EXAMPLE_OSTIMER_IRQn);
    /* Enter deep sleep mode by using power API. */
    POWER_EnterDeepSleep(EXAMPLE_EXCLUDE_FROM_DEEPSLEEP);
}
/* User Callback. */
void EXAMPLE_OstimerCallback(void)
{
    matchFlag = true;
    /* User code. */
}

/* Set the match value with unit of ms. OStimer will trigger a match interrupt after the a certain time. */
static void EXAMPLE_SetMatchInterruptTime(OSTIMER_Type *base, uint32_t ms, uint32_t freq, ostimer_callback_t cb)
{
    uint64_t timerTicks = OSTIMER_GetCurrentTimerValue(base);

    /* Translate the ms to ostimer count value. */
    timerTicks += MSEC_TO_COUNT(ms, freq);

    /* Set the match value with unit of ticks. */
    OSTIMER_SetMatchValue(base, timerTicks, cb);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kLPOSC_to_OSTIMER_CLK);

    PRINTF("Board will enter power deep sleep mode, and then wakeup by OS timer after about 5 seconds.\r\n");
    PRINTF("After Board wakeup, the OS timer will trigger the match interrupt about every 2 seconds.\r\n");

    /* Intialize the OS timer, setting clock configuration. */
    OSTIMER_Init(EXAMPLE_OSTIMER);

    /* Set the OS timer match value. */
    EXAMPLE_SetMatchInterruptTime(EXAMPLE_OSTIMER, 5000U, EXAMPLE_OSTIMER_FREQ, EXAMPLE_OstimerCallback);

    /* Enter deep sleep mode. */
    EXAMPLE_EnterDeepSleep();

    /* Wait until OS timer interrupt occurred. */
    while (!matchFlag)
    {
    }

    /* Wakeup from deep sleep mode. */
    PRINTF("Board wakeup from deep sleep mode.\r\n\r\n");

    while (1)
    {
        matchFlag = false;

        /* Set the match value to trigger the match interrupt. */
        EXAMPLE_SetMatchInterruptTime(EXAMPLE_OSTIMER, 2000U, EXAMPLE_OSTIMER_FREQ, EXAMPLE_OstimerCallback);

        /* Wait for the match value reached. */
        while (!matchFlag)
        {
        }

        PRINTF("OS timer match value reached\r\n");
    }
}
