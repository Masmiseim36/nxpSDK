/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    POWER_DisablePD(kPDRUNCFG_APD_DMA2_3);
    POWER_DisablePD(kPDRUNCFG_PPD_DMA2_3);
    POWER_ApplyPD();
    RESET_ClearPeripheralReset(kDMA3_RST_SHIFT_RSTn);

}
/*${function:end}*/
