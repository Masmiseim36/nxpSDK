/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "board.h"
#include "fsl_pit.h"
#include "fsl_xbar.h"
#include "fsl_debug_console.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUS_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
/* Channel of PIT module. */
#define PIT_CHANNEL kPIT_Chnl_0
/* Timer period for channel PIT module. */
#define PIT_PERIOD 1000000U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool xbarIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void XBAR_IRQHandler(void)
{
    if (XBAR_GetStatusFlags(XBAR) & kXBAR_EdgeDetectionOut0)
    {
        /* Clear interrupt flag. */
        XBAR_ClearStatusFlags(XBAR, kXBAR_EdgeDetectionOut0);
        xbarIsrFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize XBAR. */
    xbar_control_config_t xbaraConfig;

    /* Structure of initialize PIT. */
    pit_config_t pitConfig;
    pitConfig.enableRunInDebug = false;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nXBAR Peripheral Driver Example.");

    /* Init pit module. */
    PIT_Init(PIT0, &pitConfig);

    /* Set timer period for channels using. */
    PIT_SetTimerPeriod(PIT0, PIT_CHANNEL, USEC_TO_COUNT(PIT_PERIOD, BUS_CLK_FREQ));

    /* Start channel using. */
    PIT_StartTimer(PIT0, PIT_CHANNEL);

    /* Init xbar module. */
    XBAR_Init(XBAR);

    /* Configure the XBAR signal connections. */
    XBAR_SetSignalsConnection(XBAR, kXBAR_InputPit0Tif0, kXBAR_OutputXbDmaInt0);

    /* Configure the XBAR interrupt. */
    xbaraConfig.activeEdge = kXBAR_EdgeRising;
    xbaraConfig.requestType = kXBAR_RequestInterruptEnalbe;
    XBAR_SetOutputSignalConfig(XBAR, kXBAR_OutputXbDmaInt0, &xbaraConfig);

    /* Enable at the NVIC. */
    EnableIRQ(XBAR_IRQn);

    while (true)
    {
        if (true == xbarIsrFlag)
        {
            PRINTF("\r\nXBAR interrupt is occured !");
            xbarIsrFlag = false;
        }
    }
}
