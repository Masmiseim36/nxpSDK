/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
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
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA424_RST_SHIFT_RSTn);
    /* Clear MUA reset */
    RESET_PeripheralReset(kMU4_RST_SHIFT_RSTn);
    BOARD_InitAHBSC();

    /* Set Hifi4 and Hifi1 as Secure privileged master */
    GlikeyWriteEnable(GLIKEY0, 6U);
    AHBSC0->MASTER_SEC_LEVEL        = 0x3;
    AHBSC0->MASTER_SEC_ANTI_POL_REG = 0xFFC;
    GlikeyWriteEnable(GLIKEY1, 6U);
    AHBSC3->MASTER_SEC_LEVEL        = 0x3;
    AHBSC3->MASTER_SEC_ANTI_POL_REG = 0xFFC;

#ifdef CORE1_IMAGE_COPY_TO_RAM
    BOARD_CopyCore1Image(CORE1_BOOT_ADDRESS);
#endif
    APP_BootCore1();
}

void LED_INIT()
{
    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    LED_BLUE_INIT(LOGIC_LED_ON);
    /* Enable GPIO access from DSP side */
    GPIO_EnablePinControlNonSecure(BOARD_LED_BLUE_GPIO, 1 << BOARD_LED_BLUE_GPIO_PIN);
    GPIO_EnablePinControlNonPrivilege(BOARD_LED_BLUE_GPIO, 1 << BOARD_LED_BLUE_GPIO_PIN);
}

/*${function:end}*/
