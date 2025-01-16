/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    clock_osc32k_config_t config = {
        .bypass = false, .monitorEnable = false, .lowPowerMode = false, .cap = kCLOCK_Osc32kCapPf0};
    /* If OSC32K not enabled, enable OSC32K. */
    if (CLOCK_GetOsc32KFreq() == 0U)
    {
        CLOCK_EnableOsc32K(&config);
    }
}
/*${function:end}*/
