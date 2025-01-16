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
#include "fsl_xbar.h"
#include "app.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarInout22, kXBAR1_OutputEqdc1Phasea);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarInout20, kXBAR1_OutputEqdc1Phaseb);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarInout18, kXBAR1_OutputEqdc1Index);
}
/*${function:end}*/
