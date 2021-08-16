/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common.h for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    if (option == kClockOption_EnterBootloader)
    {
        BOARD_InitBootClocks();
    }
    else if (option == kClockOption_ExitBootloader)
    {
         CLOCK_SetSimSafeDivs(); /* Set the SIM clock dividers to save value for the MCG mode transition */
         CLOCK_SetMcgConfig(&mcgConfig_BOARD_BootClockRUN_FEI); /* Setup the specified MCG FEI mode */
         CLOCK_SetSimConfig(&simConfig_BOARD_BootClockRUN_FEI); /* Setup the SIM clock dividers to default value */
         /* The initialization of oscillators can be provided but it can just disable all oscillators. The external oscillators can be also leaved as enabled. */
         CLOCK_InitOsc0(&oscConfig_BOARD_BootClockRUN_FEI);
         SystemCoreClockUpdate(); /* or just direct assign the value: SystemCoreClock = BOARD_BOOTCLOCKRUN_FEI_CORE_CLOCK; */
         MCG->C6 = 0x00u;
    }
}


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
