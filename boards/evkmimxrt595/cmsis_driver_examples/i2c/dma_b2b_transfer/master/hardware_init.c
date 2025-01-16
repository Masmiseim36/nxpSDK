/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_inputmux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Use 48 MHz clock for the FLEXCOMM11 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM11);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Configure INPUTMUX. */
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);
    INPUTMUX_Init(INPUTMUX);
    /* Enable DMA request */
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm11TxToDmac0Ch33RequestEna, true);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);
}

uint32_t I2C11_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(11);
}

/*${function:end}*/
