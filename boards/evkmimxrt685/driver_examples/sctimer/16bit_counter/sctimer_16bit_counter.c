/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_sctimer.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SCTIMER_NEED_CHANGE_CLOCK_SOURCE 1
#define DEMO_CLOCK_MODE kSCTIMER_Input_ClockMode
#define DEMO_CLOCK_SEL kSCTIMER_Clock_On_Rise_Input_7
#define SCTIMER_CLK_FREQ CLOCK_GetSctClkFreq()
#define DEMO_FIRST_SCTIMER_OUT kSCTIMER_Out_0
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* attach FFRO clock to SCTimer input7 (48MHz / 1 = 48MHz). */
    CLOCK_AttachClk(kFFRO_to_SCT_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 1);

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

    /* Switch to 16-bit mode */
    sctimerInfo.enableCounterUnify = false;

    /* Calculate prescaler and match value for Counter L for 100ms interval */
    matchValueL            = MSEC_TO_COUNT(100U, sctimerClock);
    sctimerInfo.prescale_l = matchValueL / 65536;
    matchValueL            = matchValueL / (sctimerInfo.prescale_l + 1) - 1;

    /* Calculate prescaler and match value for Counter H for 200ms interval */
    matchValueH            = MSEC_TO_COUNT(200U, sctimerClock);
    sctimerInfo.prescale_h = matchValueH / 65536;
    matchValueH            = matchValueH / (sctimerInfo.prescale_h + 1) - 1;

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* Schedule a match event for Counter L every 0.1 seconds */
    if (SCTIMER_CreateAndScheduleEvent(SCT0, kSCTIMER_MatchEventOnly, matchValueL, 0, kSCTIMER_Counter_L,
                                       &eventCounterL) == kStatus_Fail)
    {
        return -1;
    }

    /* Toggle first output when Counter L event occurs */
    SCTIMER_SetupOutputToggleAction(SCT0, DEMO_FIRST_SCTIMER_OUT, eventCounterL);

    /* Reset Counter L when Counter L event occurs */
    SCTIMER_SetupCounterLimitAction(SCT0, kSCTIMER_Counter_L, eventCounterL);

    /* Schedule a match event for Counter H every 0.2 seconds */
    if (SCTIMER_CreateAndScheduleEvent(SCT0, kSCTIMER_MatchEventOnly, matchValueH, 0, kSCTIMER_Counter_H,
                                       &eventCounterH) == kStatus_Fail)
    {
        return -1;
    }

    /* Toggle second output when Counter H event occurs */
    SCTIMER_SetupOutputToggleAction(SCT0, DEMO_SECOND_SCTIMER_OUT, eventCounterH);

    /* Reset Counter H when Counter H event occurs */
    SCTIMER_SetupCounterLimitAction(SCT0, kSCTIMER_Counter_H, eventCounterH);

    /* Start the L counter */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);

    /* Start the H counter */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_H);

    while (1)
    {
    }
}
