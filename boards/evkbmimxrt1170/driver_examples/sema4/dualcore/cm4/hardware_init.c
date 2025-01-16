/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_soc_src.h"
/*${header:end}*/

/*${function:start}*/
/*
 * For the dual core project, generally primary core starts first, initializes
 * the system, then starts the secondary core to run.
 * In the case that debugging dual-core at the same time (for example, using IAR+DAPLink),
 * the secondary core is started by debugger. Then the secondary core might
 * run when the primary core initialization not finished. The SRC->GPR is used
 * here to indicate whether secondary core could go. When started, the secondary core
 * should check and wait the flag in SRC->GPR, the primary core sets the
 * flag in SRC->GPR when its initialization work finished.
 */
#define BOARD_SECONDARY_CORE_GO_FLAG 0xa5a5a5a5u
#define BOARD_SECONDARY_CORE_SRC_GPR kSRC_GeneralPurposeRegister20

void BOARD_WaitAndClearSecondaryCoreGoFlag(void)
{
    while (BOARD_SECONDARY_CORE_GO_FLAG != SRC_GetGeneralPurposeRegister(SRC, BOARD_SECONDARY_CORE_SRC_GPR))
    {
    }

    SRC_SetGeneralPurposeRegister(SRC, BOARD_SECONDARY_CORE_SRC_GPR, 0x0);
}

void BOARD_InitHardware(void)
{
    BOARD_WaitAndClearSecondaryCoreGoFlag();
    BOARD_InitPins();
}
/*${function:end}*/
