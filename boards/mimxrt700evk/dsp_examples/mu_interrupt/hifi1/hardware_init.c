/*
 * Copyright 2024 NXP
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
#include "board.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

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
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */
    INPUTMUX_Init(INPUTMUX1);
    CLOCK_EnableClock(kCLOCK_InputMux);
    INPUTMUX_AttachSignal(INPUTMUX1, 1U, kINPUTMUX_Mu3BToDspInterrupt);
    BOARD_InitBootPins();
    XOS_INIT();

    RESET_ClearPeripheralReset(kGPIO8_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio8);
}
void LED_INIT()
{
    LED_RED_INIT(LOGIC_LED_OFF);
}
void LED_TOGGLE()
{
    LED_RED_TOGGLE();
}
/*${function:end}*/
