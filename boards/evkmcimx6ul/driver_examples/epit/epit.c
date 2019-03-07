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
#include "fsl_epit.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EPIT_IRQ_ID    EPIT1_IRQn
#define EXAMPLE_EPIT   EPIT1
/* Get source clock for EPIT driver (EPIT prescaler = 0) */
#define EXAMPLE_EPIT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / \
                              (CLOCK_GetDiv(kCLOCK_PerclkDiv) + 1U))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool epitIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_EPIT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    EPIT_ClearStatusFlags(EXAMPLE_EPIT, kEPIT_OutputCompareFlag);
    epitIsrFlag = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize EPIT */
    epit_config_t epitConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(EPIT1_IRQn, (system_irq_handler_t)(uint32_t)EXAMPLE_EPIT_IRQHandler, NULL);

    EPIT_GetDefaultConfig(&epitConfig);

    /* Init EPIT module */
    EPIT_Init(EXAMPLE_EPIT, &epitConfig);

    /* Set timer period */
    EPIT_SetTimerPeriod(EXAMPLE_EPIT, USEC_TO_COUNT(1000000U, EXAMPLE_EPIT_CLK_FREQ) - 1);
    EPIT_SetOutputCompareValue(EXAMPLE_EPIT, 0);

    /* Enable output compare interrupts */
    EPIT_EnableInterrupts(EXAMPLE_EPIT, kEPIT_OutputCompareInterruptEnable);

    /* Enable at the Interrupt */
    EnableIRQ(EPIT_IRQ_ID);

    /* Start Timer */
    PRINTF("\r\nStarting EPIT timer ...");
    EPIT_StartTimer(EXAMPLE_EPIT);

    while (true)
    {
        /* Check whether occur interupt and toggle LED */
        if (true == epitIsrFlag)
        {
            PRINTF("\r\n EPIT interrupt is occured !");
            epitIsrFlag = false;
        }
        else
        {
            __WFI();
        }
    }
}
