/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_gpt.h"

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
    uint32_t gptFreq;
    gpt_config_t gptConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    GPT_GetDefaultConfig(&gptConfig);

    /* Initialize GPT module */
    GPT_Init(EXAMPLE_GPT, &gptConfig);

    /* Set the response on the output pin when a compare event occurs */
    GPT_SetOutputOperationMode(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, kGPT_OutputOperation_Toggle);

    /* Divide GPT clock source frequency by 3 inside GPT module */
    GPT_SetClockDivider(EXAMPLE_GPT, 3);

    /* Get GPT clock frequency */
    gptFreq = EXAMPLE_GPT_CLK_FREQ;

    /* GPT frequency is divided by 3 inside module */
    gptFreq /= 3;

    /* Set both GPT modules to 1 second duration */
    GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

    GPT_StartTimer(EXAMPLE_GPT);

    PRINTF("\r\nCompare output example\r\n");
    while (true)
    {
    }
}
