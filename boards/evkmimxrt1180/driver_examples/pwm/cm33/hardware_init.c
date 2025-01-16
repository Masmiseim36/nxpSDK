/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set the PWM Fault inputs to a low value */
    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputLogicHigh, kXBAR1_OutputFlexpwm1Fault0);
    XBAR_SetSignalsConnection(kXBAR1_InputLogicHigh, kXBAR1_OutputFlexpwm1Fault1);
    XBAR_SetSignalsConnection(kXBAR1_InputLogicHigh, kXBAR1_OutputFlexpwm1234Fault2);
    XBAR_SetSignalsConnection(kXBAR1_InputLogicHigh, kXBAR1_OutputFlexpwm1234Fault3);
}
/*${function:end}*/
