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
processor: MKL82Z128xxx7
package_id: MKL82Z128VLK7
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
  - {pin_num: '51', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/TPM_CLKIN0/EWM_IN}
  - {pin_num: '52', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/TPM_CLKIN1/EWM_OUT_b}
  - {pin_num: '75', peripheral: I2C0, signal: SCL, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/I2C0_SCL, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable}
  - {pin_num: '76', peripheral: I2C0, signal: SDA, pin_signal: PTD3/SPI0_SIN/LPUART2_TX/I2C0_SDA, slew_rate: fast, open_drain: enable, pull_select: down, pull_enable: disable}
  - {pin_num: '43', peripheral: FLEXIO0, signal: 'D, 0', pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/TPM1_CH0/FXIO0_D0}
  - {pin_num: '44', peripheral: FLEXIO0, signal: 'D, 1', pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/TPM1_CH1/FXIO0_D1}
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
    /* PTB Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* PTD Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTB0 (pin 43) is configured as FXIO0_D0 */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt7);

    /* PORTB1 (pin 44) is configured as FXIO0_D1 */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt7);

    /* PORTB16 (pin 51) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 52) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    const port_pin_config_t portd2_pin75_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Pin is configured as I2C0_SCL */
                                                   kPORT_MuxAlt7,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD2 (pin 75) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTD, 2U, &portd2_pin75_config);

    const port_pin_config_t portd3_pin76_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Pin is configured as I2C0_SDA */
                                                   kPORT_MuxAlt7,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD3 (pin 76) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTD, 3U, &portd3_pin76_config);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART 0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
