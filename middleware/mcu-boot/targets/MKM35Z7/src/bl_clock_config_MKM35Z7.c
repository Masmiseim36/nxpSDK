/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bl_context.h"
#include "property.h"
#include "fsl_device_registers.h"
#include <assert.h>
#include "clock_config.h"

// See bootloader_common.h for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{   
    if (option == kClockOption_EnterBootloader)
    {
        BOARD_BootClockFEI();
    }
    else if (option == kClockOption_ExitBootloader)
    {
        BOARD_BootClockFBI();
        BOARD_BootClockBLPI();
        CLOCK_SetBlpiMode();
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
