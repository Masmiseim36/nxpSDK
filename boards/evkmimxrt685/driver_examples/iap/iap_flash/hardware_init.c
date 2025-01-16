/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_iap.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kHSGPIO2_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Switch flexspi clock to FFRO*/
    IAP_FlexspiSetClockSource(3);
}
AT_QUICKACCESS_SECTION_CODE(status_t BOARD_FlexspiInit(uint32_t instance,
                                                       flexspi_nor_config_t *config,
                                                       serial_nor_config_option_t *option))
{
    // Reset external flash
    GPIO->CLR[2] = 1 << 12;
    for (uint32_t i = 0; i < 6000; i++)
        __NOP();
    GPIO->SET[2] = 1 << 12;
    // Clear FLEXSPI NOR flash configure context
    SYSCTL0->FLEXSPI_BOOTROM_SCRATCH0 = 0;

    status_t status = IAP_FlexspiNorAutoConfig(instance, config, option);
    if ((CACHE64->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) != 0)
    {
        /* set command to invalidate all ways and write GO bit to initiate command */
        CACHE64->CCR = CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_INVW0_MASK;
        CACHE64->CCR |= CACHE64_CTRL_CCR_GO_MASK;
        /* Wait until the command completes */
        while (CACHE64->CCR & CACHE64_CTRL_CCR_GO_MASK)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        CACHE64->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);
    }
    return status;
}
/*${function:end}*/
