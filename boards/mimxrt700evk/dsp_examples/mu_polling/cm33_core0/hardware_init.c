/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "core1_support.h"
/*${header:end}*/

/*${function:start}*/
void APP_BootCore1(void)
{
    BOARD_ReleaseCore1Power();
    BOARD_BootCore1(CORE1_BOOT_ADDRESS, CORE1_BOOT_ADDRESS);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();
    /* Clear MUA reset */
    RESET_PeripheralReset(kMU4_RST_SHIFT_RSTn);

    BOARD_InitAHBSC();

    /* Enable GPIO access from DSP side */
    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio0);

    GPIO_EnablePinControlNonSecure(BOARD_LED_BLUE_GPIO, 1 << BOARD_LED_BLUE_GPIO_PIN);
    GPIO_EnablePinControlNonPrivilege(BOARD_LED_BLUE_GPIO, 1 << BOARD_LED_BLUE_GPIO_PIN);

#ifdef CORE1_IMAGE_COPY_TO_RAM
    BOARD_CopyCore1Image(CORE1_BOOT_ADDRESS);
#endif
    APP_BootCore1();
}
/*${function:end}*/
