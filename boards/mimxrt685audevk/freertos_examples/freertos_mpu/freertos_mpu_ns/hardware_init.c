/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"

/* Non-Secure callable functions. */
#include "nsc_functions.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Get the updated SystemCoreClock from the secure side */
    SystemCoreClock = getSystemCoreClock();
}

void SystemInit(void)
{
}

/*${function:end}*/
