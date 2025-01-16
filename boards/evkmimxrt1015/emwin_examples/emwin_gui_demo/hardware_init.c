/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set LPSPI_PODF. */
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);
    /* Set Lpspi clock source. */
    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLOCK_SOURCE_SELECT);

    /* Set LPI2C_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
    /* Set Lpi2c clock source. */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
}

/* Get LPSPI1_CLK_Freq (8MHz).  */
uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_OscClk) / (LPSPI_CLOCK_SOURCE_DIVIDER + 1);
}
/* Get LPI2C1_CLK_Freq (12MHz).  */
uint32_t LPI2C1_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_OscClk) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1);
}
/*${function:end}*/
