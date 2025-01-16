/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "app.h"
#include "pin_mux.h"
#include "board_hifi4.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
static void XOS_INIT(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);
    xos_start_system_timer(-1, 0);
    /* DSP interrupt only can be enable after XOS is started. */
    xos_register_interrupt_handler(APP_MU_IRQn, APP_MU_IRQHandler, NULL);
    xos_interrupt_enable(APP_MU_IRQn);
}
void BOARD_InitHardware(void)
{
    INPUTMUX_Init(INPUTMUX);
    BOARD_InitBootPins();
    XOS_INIT();
}
void LED_INIT()
{
    CLOCK_EnableClock(kCLOCK_HsGpio0);
    RESET_PeripheralReset(kHSGPIO0_RST_SHIFT_RSTn);
    LED_RED_INIT(LOGIC_LED_OFF);
}
void LED_TOGGLE()
{
    LED_RED_TOGGLE();
}
/*${function:end}*/
