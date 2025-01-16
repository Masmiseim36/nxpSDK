/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_xbara.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    XBARA_Init(XBARA1);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout38, kXBARA1_OutputDec1Phasea);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout39, kXBARA1_OutputDec1Phaseb);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout40, kXBARA1_OutputDec1Index);
}
/*${function:end}*/
