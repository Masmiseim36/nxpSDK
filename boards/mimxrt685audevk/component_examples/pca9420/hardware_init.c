/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Use 16 MHz clock for the FLEXCOMM15 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM15);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_ConfigPMICModes(pca9420_modecfg_t *cfg, uint32_t num)
{
    assert(cfg);

    if (num >= 2)
    {
        /* Mode 1: High drive. */
        cfg[1].sw1OutVolt  = kPCA9420_Sw1OutVolt1V100;
        cfg[1].sw2OutVolt  = kPCA9420_Sw2OutVolt1V900;
        cfg[1].ldo1OutVolt = kPCA9420_Ldo1OutVolt1V900;
        cfg[1].ldo2OutVolt = kPCA9420_Ldo2OutVolt3V300;
    }
    if (num >= 3)
    {
        /* Mode 2: Low drive. */
        cfg[2].sw1OutVolt  = kPCA9420_Sw1OutVolt0V975;
        cfg[2].sw2OutVolt  = kPCA9420_Sw2OutVolt1V800;
        cfg[2].ldo1OutVolt = kPCA9420_Ldo1OutVolt1V800;
        cfg[2].ldo2OutVolt = kPCA9420_Ldo2OutVolt3V300;
    }
    if (num >= 4)
    {
        /* Mode 3: VDDIO off, watchdog enabled. */
        cfg[3].enableLdo2Out = false;
        cfg[3].wdogTimerCfg  = kPCA9420_WdTimer16s;
    }
}
/*${function:end}*/
