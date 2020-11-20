/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_sctimer.h"

#include "pin_mux.h"
#include <stdbool.h>
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SCTIMER_CLK_FREQ        CLOCK_GetFreq(kCLOCK_BusClk)
#define DEMO_FIRST_SCTIMER_OUT  kSCTIMER_Out_2
#define DEMO_SECOND_SCTIMER_OUT kSCTIMER_Out_6

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    sctimer_config_t sctimerInfo;
    uint32_t eventCounterL, eventCounterH;
    uint32_t sctimerClock;
    uint32_t matchValueL, matchValueH;

    /* Board pin, clock, debug console init */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    sctimerClock = SCTIMER_CLK_FREQ;

    /* Print a note to terminal */
    PRINTF("\r\nSCTimer example to use it in 16-bit mode\r\n");
    PRINTF("\r\nThe example shows both 16-bit counters running and toggling an output periodically  ");

    SCTIMER_GetDefaultConfig(&sctimerInfo);
/* Add judgement for change colck source*/
#if defined(SCTIMER_NEED_CHANGE_CLOCK_SOURCE)
    sctimerInfo.clockMode   = DEMO_CLOCK_MODE;
    sctimerInfo.clockSelect = DEMO_CLOCK_SEL;
#endif

    /* Switch to 16-bit mode, if we want to use any 16-bit counter, no matter the Low 16-bit one or the
     * High 16-bit one, we need to disable the Unify 32-bit Counter by hardware limit.
     */
    sctimerInfo.enableCounterUnify = false;

    /* Calculate prescaler and match value for the 16-bit low counter for 100ms interval */
    matchValueL            = MSEC_TO_COUNT(100U, sctimerClock);
    sctimerInfo.prescale_l = matchValueL / 65536;
    matchValueL            = matchValueL / (sctimerInfo.prescale_l + 1) - 1;

    /* Calculate prescaler and match value for the 16-bit high counter for 200ms interval */
    matchValueH            = MSEC_TO_COUNT(200U, sctimerClock);
    sctimerInfo.prescale_h = matchValueH / 65536;
    matchValueH            = matchValueH / (sctimerInfo.prescale_h + 1) - 1;

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* Schedule a match event for the 16-bit low counter every 0.1 seconds */
    if (SCTIMER_CreateAndScheduleEvent(SCT0, kSCTIMER_MatchEventOnly, matchValueL, 0, kSCTIMER_Counter_L,
                                       &eventCounterL) == kStatus_Fail)
    {
        return -1;
    }

    /* Toggle first output when the 16-bit low counter event occurs */
    SCTIMER_SetupOutputToggleAction(SCT0, DEMO_FIRST_SCTIMER_OUT, eventCounterL);

    /* Reset Counter L when the 16-bit low counter event occurs */
    SCTIMER_SetupCounterLimitAction(SCT0, kSCTIMER_Counter_L, eventCounterL);

    /* Schedule a match event for the 16-bit high counter every 0.2 seconds */
    if (SCTIMER_CreateAndScheduleEvent(SCT0, kSCTIMER_MatchEventOnly, matchValueH, 0, kSCTIMER_Counter_H,
                                       &eventCounterH) == kStatus_Fail)
    {
        return -1;
    }

    /* Toggle second output when the 16-bit high counter event occurs */
    SCTIMER_SetupOutputToggleAction(SCT0, DEMO_SECOND_SCTIMER_OUT, eventCounterH);

    /* Reset Counter H when the 16-bit high counter event occurs */
    SCTIMER_SetupCounterLimitAction(SCT0, kSCTIMER_Counter_H, eventCounterH);

    /* Start the 16-bit low counter */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);

    /* Start the 16-bit high counter */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_H);

    while (1)
    {
    }
}
