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

#include "fsl_wdog.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_WDOG_BASE WDOG1
#define WDOG_WCT_INSTRUCITON_COUNT (256U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/
static WDOG_Type *wdog_base = DEMO_WDOG_BASE;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void WaitWctRefresh()
{
    for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
    {
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    wdog_config_t config;
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* force warm reset sources to generate cold reset for a more reliable restart */
    SRC->SCR &= ~SRC_SCR_WARM_RESET_ENABLE_MASK;
    PRINTF("--- Start Wdog test ---\r\n");
    /*
    * wdogConfig->enableWdog = true;
    * wdogConfig->workMode.enableWait = true;
    * wdogConfig->workMode.enableStop = false;
    * wdogConfig->workMode.enableDebug = false;
    * wdogConfig->enableInterrupt = false;
    * wdogConfig->enablePowerdown = false;
    * wdogConfig->resetExtension = flase;
    * wdogConfig->timeoutValue = 0xFFU;
    * wdogConfig->interruptTimeValue = 0x04u;
    */
    WDOG_GetDefaultConfig(&config);
    config.timeoutValue = 0x0fu;
    config.interruptTimeValue = 0x08u;
    WDOG_Init(wdog_base, &config);
    PRINTF("--- wdog Init done---\r\n");

    for (uint32_t i = 0; i < 10; i++)
    {
        WDOG_Refresh(wdog_base);
        PRINTF("--- Refresh wdog %d time ---\r\n", i + 1);
        WaitWctRefresh();
    }

    PRINTF("\r\nEnd of Wdog example!\r\n");
    while (1)
    {
    }
}
