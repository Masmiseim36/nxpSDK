/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Configure DMAMUX. */
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX);
    /* Enable DMA request */
#if USE_HS_SPI
    /* Use 48 MHz clock for the FLEXCOMM14 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM14);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm14RxToDmac0Ch26RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm14TxToDmac0Ch27RequestEna, true);
#else
    /* Use 48 MHz clock for the FLEXCOMM5 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM5);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm5RxToDmac0Ch10RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm5TxToDmac0Ch11RequestEna, true);
#endif
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
