/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_wdog.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

void APP_GlobalSystemResetInit(void)
{
    wdog_config_t config;

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
    config.timeoutValue = 0xFU; /* Timeout value is (0xF + 1)/2 = 8 sec. */
    WDOG_Init(DEMO_WDOG_BASE, &config);
    while (1)
    {
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
