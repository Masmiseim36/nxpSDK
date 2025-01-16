/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "app.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_gpt.h"
#endif
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
#if configUSE_TICKLESS_IDLE == 2
    gpt_config_t gptConfig;
#endif
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Workaround: Disable interrupt which might be enabled by ROM. */
    RGPIO_SetPinInterruptConfig(RGPIO1, 9U, kRGPIO_InterruptOutput0, kRGPIO_InterruptOrDMADisabled);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    NVIC_ClearPendingIRQ(GPIO1_0_IRQn);

#if configUSE_TICKLESS_IDLE == 2
    GPT_GetDefaultConfig(&gptConfig);

    /* Initialize GPT module */
    GPT_Init(GPT1, &gptConfig);

    /* Divide GPT clock source frequency by 1 inside GPT module */
    GPT_SetClockDivider(GPT1, 1);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(GPT1, kGPT_OutputCompare1InterruptEnable);

    /* Enable at the Interrupt */
    EnableIRQ(GPT1_IRQn);
#endif
}

#if configUSE_TICKLESS_IDLE == 2
/*!
 * @brief Interrupt service fuction of GPT timer.
 *
 * This function to call low power timer ISR
 */
void GPT1_IRQHandler(void)
{
    vPortGptIsr();
}

/*!
 * @brief Fuction of GPT timer.
 *
 * This function to return GPT timer base address
 */

GPT_Type *vPortGetGptBase(void)
{
    return GPT1;
}

/*!
 * @brief Fuction of GPT timer.
 *
 * This function to return GPT timer interrupt number
 */

IRQn_Type vPortGetGptIrqn(void)
{
    return GPT1_IRQn;
}
#endif
/*${function:end}*/
