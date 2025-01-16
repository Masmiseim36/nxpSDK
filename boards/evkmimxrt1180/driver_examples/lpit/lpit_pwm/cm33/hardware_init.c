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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClockMux(kCLOCK_Root_Lpit3, kCLOCK_LPIT3_ClockRoot_MuxOscRc24M);
}

void BOARD_ConfigTriggerSource()
{
    /* Init xbara module. */
    XBAR_Init(kXBAR_DSC1);

    /* Configure the XBARA signal connections. */
    XBAR_SetSignalsConnection(kXBAR1_InputPit3Trigger0, kXBAR1_OutputIomuxXbarInout20);
}
/*${function:end}*/
