/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_soc_src.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
static void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_ResetDisplayMix();
    BOARD_InitDebugConsole();

    /* ERR050396
     * Errata description:
     * AXI to AHB conversion for CM7 AHBS port (port to access CM7 to TCM) is by a NIC301 block, instead of XHB400
     * block. NIC301 doesn't support sparse write conversion. Any AXI to AHB conversion need XHB400, not by NIC. This
     * will result in data corruption in case of AXI sparse write reaches the NIC301 ahead of AHBS. Errata workaround:
     * For uSDHC, don't set the bit#1 of IOMUXC_GPR28 (AXI transaction is cacheable), if write data to TCM aligned in 4
     * bytes; No such write access limitation for OCRAM or external RAM
     */
    IOMUXC_GPR->GPR28 &= (~IOMUXC_GPR_GPR28_AWCACHE_USDHC_MASK);
}
/*${function:end}*/
