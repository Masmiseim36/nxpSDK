/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_ctimer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    ctimer_config_t config;
    ctimer_match_config_t matchConfig;

    /* Init hardware*/
    BOARD_InitHardware();

#if defined(BOARD_HAS_NO_CTIMER_OUTPUT_PIN_CONNECTED_TO_LED)
    LED_RED1_INIT(LOGIC_LED_OFF);
#endif

    PRINTF("CTimer match example to toggle the output on a match\r\n");

    CTIMER_GetDefaultConfig(&config);

    CTIMER_Init(CTIMER, &config);

    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop  = false;
    matchConfig.matchValue         = CTIMER_CLK_FREQ / 2;
    matchConfig.outControl         = kCTIMER_Output_Toggle;
    matchConfig.outPinInitState    = true;
    matchConfig.enableInterrupt    = false;
    CTIMER_SetupMatch(CTIMER, CTIMER_MAT_OUT, &matchConfig);
    CTIMER_StartTimer(CTIMER);

    while (1)
    {
#if defined(BOARD_HAS_NO_CTIMER_OUTPUT_PIN_CONNECTED_TO_LED)
        /* No timer match output pin connected to a LED
         * toggle LED manually according to match status
         */
        if (CTIMER_GetOutputMatchStatus(CTIMER, CTIMER_EMT_OUT))
        {
            LED_RED1_ON();
        }
        else
        {
            LED_RED1_OFF();
        }
#endif
    }
}
