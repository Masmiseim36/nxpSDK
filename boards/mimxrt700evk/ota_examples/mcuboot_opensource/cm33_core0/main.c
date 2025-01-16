/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "boot.h"

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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitNorFlashPins();

    CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
    CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 1u);     /*400MHz*/

    PRINTF("hello sbl.\r\n");

    (void)sbl_boot_main();

    return 0;
}

void SBL_DisablePeripherals(void)
{
    DbgConsole_Deinit();
}

status_t CRYPTO_InitHardware(void)
{
    return kStatus_Success;
}
