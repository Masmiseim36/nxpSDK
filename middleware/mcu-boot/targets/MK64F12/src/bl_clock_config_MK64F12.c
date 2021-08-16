/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bl_context.h"
#include "property.h"
#include "fsl_device_registers.h"
#include "fsl_assert.h"
#include "clock_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Clock mode types
typedef enum _target_clock_mode
{
    kClockMode_FEI_48MHz = 0,
    kClockMode_FEI_21MHz = 1,
    kClockMode_Default = kClockMode_FEI_21MHz,
} target_clock_mode_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common for documentation on this function.
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
         MCG->C5 = 0x0U;
         MCG->C6 = 0x0U;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
