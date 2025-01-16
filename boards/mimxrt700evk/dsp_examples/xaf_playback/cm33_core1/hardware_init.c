/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA423_RST_SHIFT_RSTn);
    /* Clear MUA reset */
    RESET_PeripheralReset(kMU3_RST_SHIFT_RSTn);

    GlikeyWriteEnable(GLIKEY1, 6U);
    AHBSC3->MASTER_SEC_LEVEL        = 0x3;
    AHBSC3->MASTER_SEC_ANTI_POL_REG = 0xFFC;

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);

    POWER_DisablePD(kPDRUNCFG_APD_DMA2_3);
    POWER_DisablePD(kPDRUNCFG_PPD_DMA2_3);

    POWER_DisablePD(kPDRUNCFG_PD_FRO2); /* Sense uses FRO2. */
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    /* SAI clock = 24MHz */
    CLOCK_AttachClk(kFRO2_DIV1_to_SAI3);
    CLOCK_SetClkDiv(kCLOCK_DivSai3Clk, 8U);

    SYSCON1->SAI3_MCLK_CTRL |= SYSCON1_SAI3_MCLK_CTRL_SAIMCLKDIR_MASK;

}

/* Dummy codec init */
int BOARD_CODEC_Init(void)
{
    return 0;
}
/*${function:end}*/
