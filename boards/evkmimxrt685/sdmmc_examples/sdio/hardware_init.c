/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /*Make sure USDHC ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_APD_USDHC0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USDHC0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC);
    POWER_ApplyPD();

    /* SDIO0 */
    /* usdhc depend on 32K clock also */
    CLOCK_AttachClk(kLPOSC_DIV32_to_32KHZWAKE_CLK);
    CLOCK_AttachClk(kAUX0_PLL_to_SDIO0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSdio0Clk, 1);
}
/*${function:end}*/
