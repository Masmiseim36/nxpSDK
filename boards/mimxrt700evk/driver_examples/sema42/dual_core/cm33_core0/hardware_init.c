/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size()
{
    return CORE1_IMAGE_SIZE;
}
#endif

void APP_BootCore1(void)
{
    BOARD_InitAHBSC();
    BOARD_ReleaseCore1Power();
    BOARD_BootCore1(CORE1_BOOT_ADDRESS, CORE1_BOOT_ADDRESS);
}

void APP_InitCore0Domain(void)
{
    /* BOARD_CopyCore1Image(CORE1_BOOT_ADDRESS); */
    APP_BootCore1();
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    RESET_PeripheralReset(kMU1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kSEMA420_RST_SHIFT_RSTn);
}
/*${function:end}*/
