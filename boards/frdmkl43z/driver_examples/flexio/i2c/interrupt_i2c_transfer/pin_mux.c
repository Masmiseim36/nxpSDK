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
processor: MKL43Z256xxx4
package_id: MKL43Z256VLH4
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
  - {pin_num: '23', peripheral: LPUART0, signal: RX, pin_signal: PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '24', peripheral: LPUART0, signal: TX, pin_signal: PTA2/LPUART0_TX/TPM2_CH1}
  - {pin_num: '63', peripheral: FLEXIO, signal: 'D, 6', pin_signal: LCD_P46/ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/LPUART0_RX/SPI1_MISO/FXIO0_D6, slew_rate: fast, drive_strength: low,
    pull_select: up, pull_enable: enable}
  - {pin_num: '64', peripheral: FLEXIO, signal: 'D, 7', pin_signal: LCD_P47/PTD7/SPI1_MISO/LPUART0_TX/SPI1_MOSI/FXIO0_D7, slew_rate: fast, drive_strength: low, pull_select: up,
    pull_enable: enable}
  - {pin_num: '1', peripheral: I2C1, signal: SDA, pin_signal: LCD_P48/PTE0/CLKOUT32K/SPI1_MISO/LPUART1_TX/RTC_CLKOUT/CMP0_OUT/I2C1_SDA, slew_rate: slow, pull_select: up,
    pull_enable: enable}
  - {pin_num: '2', peripheral: I2C1, signal: SCL, pin_signal: LCD_P49/PTE1/SPI1_MOSI/LPUART1_RX/SPI1_MISO/I2C1_SCL, slew_rate: slow, pull_select: up, pull_enable: enable}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA1 (pin 23) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 24) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    const port_pin_config_t portd6_pin63_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as FXIO0_D6 */
                                                   kPORT_MuxAlt6};
    /* PORTD6 (pin 63) is configured as FXIO0_D6 */
    PORT_SetPinConfig(PORTD, 6U, &portd6_pin63_config);

    const port_pin_config_t portd7_pin64_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as FXIO0_D7 */
                                                   kPORT_MuxAlt6};
    /* PORTD7 (pin 64) is configured as FXIO0_D7 */
    PORT_SetPinConfig(PORTD, 7U, &portd7_pin64_config);

    const port_pin_config_t porte0_pin1_config = {/* Internal pull-up resistor is enabled */
                                                  kPORT_PullUp,
                                                  /* Slow slew rate is configured */
                                                  kPORT_SlowSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as I2C1_SDA */
                                                  kPORT_MuxAlt6};
    /* PORTE0 (pin 1) is configured as I2C1_SDA */
    PORT_SetPinConfig(PORTE, 0U, &porte0_pin1_config);

    const port_pin_config_t porte1_pin2_config = {/* Internal pull-up resistor is enabled */
                                                  kPORT_PullUp,
                                                  /* Slow slew rate is configured */
                                                  kPORT_SlowSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as I2C1_SCL */
                                                  kPORT_MuxAlt6};
    /* PORTE1 (pin 2) is configured as I2C1_SCL */
    PORT_SetPinConfig(PORTE, 1U, &porte1_pin2_config);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 Transmit Data Source Select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 Receive Data Source Select: LPUART_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
