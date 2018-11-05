/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
#include "fsl_xbara.h"
#include "fsl_debug_console.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_XBARA_USER_CHANNEL_INPUT kXBARA_InputPitTrigger0
#define DEMO_XBARA_USER_CHANNEL_OUTPUT kXBARA_OutputDmamux18
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

volatile bool xbaraIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void XBARA_IRQHandler(void)
{
    if (XBARA_GetStatusFlags(XBARA) & kXBARA_EdgeDetectionOut0)
    {
        /* Clear interrupt flag. */
        XBARA_ClearStatusFlags(XBARA, kXBARA_EdgeDetectionOut0);
        xbaraIsrFlag = true;
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize XBARA. */
    xbara_control_config_t xbaraConfig;

    /* Structure of initialize PIT. */
    pit_config_t pitConfig;
    pitConfig.enableRunInDebug = false;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nXBARA Peripheral Driver Example.");

    /* Init pit module. */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channels using. */
    PIT_SetTimerPeriod(PIT, PIT_CHANNEL, USEC_TO_COUNT(PIT_PERIOD, BUS_CLK_FREQ));

    /* Start channel using. */
    PIT_StartTimer(PIT, PIT_CHANNEL);

    /* Init xbara module. */
    XBARA_Init(XBARA);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(XBARA, DEMO_XBARA_USER_CHANNEL_INPUT, DEMO_XBARA_USER_CHANNEL_OUTPUT);

    /* Configure the XBARA interrupt. */
    xbaraConfig.activeEdge = kXBARA_EdgeRising;
    xbaraConfig.requestType = kXBARA_RequestInterruptEnalbe;
    XBARA_SetOutputSignalConfig(XBARA, DEMO_XBARA_USER_CHANNEL_OUTPUT, &xbaraConfig);

    /* Enable at the NVIC. */
    EnableIRQ(XBARA_IRQn);

    while (true)
    {
        if (true == xbaraIsrFlag)
        {
            PRINTF("\r\nXBARA interrupt is occured !");
            xbaraIsrFlag = false;
        }
    }
}
