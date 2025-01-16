/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
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
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Gpio0);
    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kLPOSC_to_WWDT0);

    /* ROM may change ITRC OUT3_SEL0 and OUT4_SEL0, which will select WWDT0-2 as input for ITRC. When a WWDT timeout
      happened the event will first assert and trigger a ITRC reset, This ITRC rest is faster than WWDT reset and reset
      the system before a WWDT reset. Change back the OUT3_SEL0 and OUT4_SEL0 to allow RSTCTL capturing WWDT reset
      event. */
    ITRC->OUT_SEL[3][0] = 0xAAAAAA0A;
    ITRC->OUT_SEL[4][0] = 0xAAAAAA0A;
}
/*${function:end}*/
