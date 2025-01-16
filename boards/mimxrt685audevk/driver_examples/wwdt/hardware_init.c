/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#if !defined(FSL_FEATURE_WWDT_HAS_NO_PDCFG) || (!FSL_FEATURE_WWDT_HAS_NO_PDCFG)
#include "fsl_power.h"
#endif
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_HsGpio0);
    RESET_PeripheralReset(kHSGPIO0_RST_SHIFT_RSTn);

    CLOCK_AttachClk(kLPOSC_to_WDT0_CLK);

    /* Config OTP SHADOW register to enable ROM code QSPI reset function. (Reset PIN is P2_12)*/
    OCOTP->OTP_SHADOW[97] = (uint32_t)((1 << 14) | (2 << 15) | (12 << 18));

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
#if !defined(FSL_FEATURE_WWDT_HAS_NO_PDCFG) || (!FSL_FEATURE_WWDT_HAS_NO_PDCFG)
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC);
#endif
}
/*${function:end}*/
