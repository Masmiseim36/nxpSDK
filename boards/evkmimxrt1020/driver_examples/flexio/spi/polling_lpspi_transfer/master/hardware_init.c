/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
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

    /* Clock setting for Flexio */
    CLOCK_SetMux(kCLOCK_Flexio1Mux, MASTER_FLEXIO_SPI_CLOCK_SELECT);
    CLOCK_SetDiv(kCLOCK_Flexio1PreDiv, MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Flexio1Div, MASTER_FLEXIO_SPI_CLOCK_DIVIDER);

    /* Clock setting for Lpspi */
    CLOCK_SetMux(kCLOCK_LpspiMux, SLAVE_LPSPI_CLOCK_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, SLAVE_LPSPI_CLOCK_DIVIDER);
}
/*${function:end}*/
