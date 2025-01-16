/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "board.h"
#include "fsl_soc_src.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${prototype:start}*/
/*${prototype:end}*/

/*${function:start}*/
void DEMO_SwitchToUntrustedDomain(void)
{

}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    NVIC_EnableIRQ(APP_MU_IRQn);
}
/*${function:end}*/
