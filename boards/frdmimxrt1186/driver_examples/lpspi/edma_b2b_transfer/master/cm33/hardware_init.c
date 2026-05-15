/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_trdc.h"
#include "app.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitSPIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_EnableClock(kCLOCK_Trdc);
}
/*${function:end}*/
