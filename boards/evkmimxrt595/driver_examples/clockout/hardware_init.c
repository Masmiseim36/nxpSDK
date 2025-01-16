/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Configure 32K OSC clock. */
    CLOCK_EnableOsc32K(true);               /* Enable 32KHz Oscillator clock */
    CLOCK_EnableClock(kCLOCK_Rtc);          /* Enable the RTC peripheral clock */
    RTC->CTRL &= ~RTC_CTRL_SWRESET_MASK;    /* Make sure the reset bit is cleared */
    RTC->CTRL &= ~RTC_CTRL_RTC_OSC_PD_MASK; /* The RTC Oscillator is powered up */

    CLOCK_InitSysPfd(kCLOCK_Pfd1, 24);      /* Enable DSP PLL clock */
    CLOCK_InitSysPfd(kCLOCK_Pfd3, 24);      /* Enable AUX1 PLL clock */
}
/*${function:end}*/
