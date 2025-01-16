/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("Prepare to wake up core1 booted from flash, console will be transferred to it.\r\n");
    /* Boot core 1. */
#if BOOT_CORE1_BY_MU
    MU_BootCoreB(APP_MU, APP_CORE1_BOOT_MODE);
#else
    APP_BootCore1();
#endif

    while (1)
    {
    }
}
