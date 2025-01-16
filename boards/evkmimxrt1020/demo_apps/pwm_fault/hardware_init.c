/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbara.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set the PWM Fault inputs to CMP1 output pin */
    XBARA_Init(XBARA);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputAcmp1Out, kXBARA1_OutputFlexpwm1Fault0);
}
/*${function:end}*/
