/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpt.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_GPT_IRQn GPT2_IRQn
#define DEMO_GPT_BASE GPT2
/* GPT channel used for input capture */
#define BOARD_GPT_INPUT_CAPTURE_CHANNEL kGPT_InputCapture_Channel1
/* Interrupt to enable and flag to read; depends on the GPT channel used */
#define BOARD_GPT_CHANNEL_INTERRUPT_ENABLE kGPT_InputCapture1InterruptEnable
#define BOARD_GPT_CHANNEL_FLAG kGPT_InputCapture1Flag

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
void EXAMPLE_GPT_CAPTURE_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(DEMO_GPT_BASE, BOARD_GPT_CHANNEL_FLAG);

    gptIsrFlag = true;
}

/*!
* @brief Main function
*/
int main(void)
{
    uint32_t captureVal = 0;
    gpt_config_t gptConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(GPT2_IRQn, (system_irq_handler_t)(uint32_t)EXAMPLE_GPT_CAPTURE_IRQHandler, NULL);

    /* Print a note to terminal */
    PRINTF("\r\nGPT input capture example\r\n");
    PRINTF("\r\nOnce the input signal is received the input capture value is printed\r\n");

    GPT_GetDefaultConfig(&gptConfig);

    /* Initialize GPT module */
    GPT_Init(DEMO_GPT_BASE, &gptConfig);

    /* Setup input capture on a gpt channel */
    GPT_SetInputOperationMode(DEMO_GPT_BASE, BOARD_GPT_INPUT_CAPTURE_CHANNEL, kGPT_InputOperation_RiseEdge);

    /* Enable GPT Input Capture1 interrupt */
    GPT_EnableInterrupts(DEMO_GPT_BASE, BOARD_GPT_CHANNEL_INTERRUPT_ENABLE);

    /* Enable at the Interrupt */
    EnableIRQ(DEMO_GPT_IRQn);

    /* Start Timer */
    GPT_StartTimer(DEMO_GPT_BASE);

    while (true)
    {
        /* Check whether occur interupt */
        if (true == gptIsrFlag)
        {
            captureVal = GPT_GetInputCaptureValue(DEMO_GPT_BASE, BOARD_GPT_INPUT_CAPTURE_CHANNEL);
            PRINTF("\r\n Capture value =%x\r\n", captureVal);
            gptIsrFlag = false;
        }
        else
        {
            __WFI();
        }
    }
}
