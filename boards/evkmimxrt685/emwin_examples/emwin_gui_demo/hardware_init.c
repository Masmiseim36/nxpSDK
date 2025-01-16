/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_dma.h"
#include <stdbool.h>
/*${header:end}*/

/*${macro:start}*/
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA0
/*${macro:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Use 48 MHz clock for the SPI5 */
    CLOCK_AttachClk(kFFRO_to_FLEXCOMM5);
    /* Use 16 MHz clock for the I2C2 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM2);

    DMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASEADDR);
}

uint32_t SPI5_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(5U);
}

uint32_t I2C2_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(2U);
}

/*${function:end}*/
