/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "mcmgr.h"

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
    uint32_t core1_image_size, core1_image_source_addr, core1_image_dest_addr;
    uint32_t core1_boot_addr;

    /* Init board hardware. */
    BOARD_InitHardware();
    (void)MCMGR_Init();

    (void)PRINTF("\r\nMulticore trigger demo!\r\n");

    if (kStatus_Success != BOARD_GetCore1ImageAddrSize(&core1_image_source_addr, &core1_image_dest_addr,
                                                       &core1_image_size, &core1_boot_addr))
    {
        (void)PRINTF("Core0 failed to get core1 image info!\r\n");
    }
    else
    {
        (void)PRINTF("Core0 get core1 image info sucessfully!\r\n");
        (void)PRINTF("  Core1 image reside addr = 0x%x\r\n", core1_image_source_addr);
        (void)PRINTF("  Core1 image dest addr   = 0x%x\r\n", core1_image_dest_addr);
        (void)PRINTF("  Core1 image size        = %u(~%uK) bytes\r\n", core1_image_size,
                     (core1_image_size + 1023UL) / 1024UL);
        (void)PRINTF("  Core1 image boot addr   = 0x%x\r\n", core1_boot_addr);

        BOARD_PrepareCore1(core1_image_source_addr, core1_image_dest_addr, core1_image_size, core1_boot_addr);

        /* Boot Secondary core application */
        (void)PRINTF("Core0 is starting core1...\r\n\r\n");
        (void)MCMGR_StartCore(kMCMGR_Core1, (void *)core1_boot_addr, 2, kMCMGR_Start_Synchronous);
        (void)PRINTF("Core1 application has been started.\r\n\r\n");
    }

    while (1)
    {
    }
}
