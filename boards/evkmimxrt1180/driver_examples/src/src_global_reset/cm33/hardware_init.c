/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_rtwdog.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

void APP_GlobalSystemResetInit(void)
{
    rtwdog_config_t config;

    /*
     * config.enableWdog32 = true;
     * config.clockSource = kWDOG32_ClockSource1;
     * config.prescaler = kWDOG32_ClockPrescalerDivide1;
     * config.testMode = kWDOG32_TestModeDisabled;
     * config.enableUpdate = true;
     * config.enableInterrupt = false;
     * config.enableWindowMode = false;
     * config.windowValue = 0U;
     * config.timeoutValue = 0xFFFFU;
     */
    RTWDOG_GetDefaultConfig(&config);
    config.timeoutValue = 0xFFFFU; /* Timeout value is (0xF + 1)/2 = 8 sec. */
    RTWDOG_Init(DEMO_WDOG_BASE, &config);
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

    /* Misc config to enable the watchdog */
    SRC_GENERAL_REG->SRMASK         = 0;
    BLK_CTRL_S_AONMIX->LP_HANDSHAKE = 0xFFFFFFFF;
}
/*${function:end}*/
