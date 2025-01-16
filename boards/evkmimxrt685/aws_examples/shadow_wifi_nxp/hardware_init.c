/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Define the init structure for the OSPI reset pin*/
    gpio_pin_config_t reset_config = {
        kGPIO_DigitalOutput,
        1,
    };

    /* Init output OSPI reset pin. */
    GPIO_PortInit(BOARD_FLASH_RESET_GPIO, BOARD_FLASH_RESET_GPIO_PORT);
    GPIO_PinInit(BOARD_FLASH_RESET_GPIO, BOARD_FLASH_RESET_GPIO_PORT, BOARD_FLASH_RESET_GPIO_PIN, &reset_config);

    /* Make sure casper ram buffer has power up */
    POWER_DisablePD(kPDRUNCFG_APD_CASPER_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_CASPER_SRAM);

    /* Enable the FlexSPI reset pin P2_12 by the ROM */
    uint32_t flexspi_rom_reset = 0;
    flexspi_rom_reset |= (1U << 14U);  /* FlexSPI reset pin enable */
    flexspi_rom_reset |= (2U << 15U);  /* FlexSPI reset port */
    flexspi_rom_reset |= (12U << 18U); /* FlexSPI reset pin */
    OCOTP->OTP_SHADOW[0x61] = flexspi_rom_reset;
}
/*${function:end}*/
