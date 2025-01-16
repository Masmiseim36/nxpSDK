/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "ezhv_support.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    /* AHBSC setting */
    BOARD_InitAHBSC();

    /*
     * Set EZHV master to secure privileged mode, this is used when EZHV wants to
     * configure some registers which must be accessed in secure privileged mode,
     * such as GPIO registers.
     * Another way is, the primary core configures all registers need secure
     * privileged accessing, and EZHV only run in non-secure and unprivileged mode.
     *
     * When debugging EZHV project independently, this is done by debugger, for example,
     * in Jlink Script.
     */
    BOARD_EZHV_SetSecurePriv();
}
/*${function:end}*/
