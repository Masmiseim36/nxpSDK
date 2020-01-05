/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MK82FN256xxx15
package_id: MK82FN256VLL15
mcu_data: ksdk2_0
processor_version: 0.0.8
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '78', peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK/FXIO0_D14,
    direction: INPUT, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    const port_pin_config_t portc6_pin78_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC6 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC6 (pin 78) is configured as PTC6 */
    PORT_SetPinConfig(PORTC, 6U, &portc6_pin78_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
