/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpt.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define GPT_IRQ_ID GPT2_IRQn
#define EXAMPLE_GPT GPT2
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler

/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ CLOCK_GetFreq(kCLOCK_PerClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool gptIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_GPT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);

    gptIsrFlag = true;
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}

/*!
* @brief Main function
*/
int main(void)
{
    uint32_t gptFreq;
    gpt_config_t gptConfig;

    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    GPT_GetDefaultConfig(&gptConfig);

    /* Initialize GPT module */
    GPT_Init(EXAMPLE_GPT, &gptConfig);

    /* Divide GPT clock source frequency by 3 inside GPT module */
    GPT_SetClockDivider(EXAMPLE_GPT, 3);

    /* Get GPT clock frequency */
    gptFreq = EXAMPLE_GPT_CLK_FREQ;

    /* GPT frequency is divided by 3 inside module */
    gptFreq /= 3;

    /* Set both GPT modules to 1 second duration */
    GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

    /* Enable at the Interrupt */
    EnableIRQ(GPT_IRQ_ID);

    PRINTF("\r\nPress any key to start the example");
    GETCHAR();

    /* Start Timer */
    PRINTF("\r\nStarting GPT timer ...");
    GPT_StartTimer(EXAMPLE_GPT);

    while (true)
    {
        /* Check whether occur interupt and toggle LED */
        if (true == gptIsrFlag)
        {
            PRINTF("\r\n GPT interrupt is occured !");
            gptIsrFlag = false;
        }
        else
        {
            __WFI();
        }
    }
}
