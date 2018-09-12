/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
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
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
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
    config.timeoutValue = 0xffu;
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
