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
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
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
  - {pin_num: '35', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
  - {pin_num: '97', peripheral: FLEXIO0, signal: 'D, 4', pin_signal: PTD4/LLWU_P14/LPSPI1_PCS0/LPUART2_RX/TPM0_CH4/LPUART0_RTS_b/FXIO0_D4}
  - {pin_num: '98', peripheral: FLEXIO0, signal: 'D, 5', pin_signal: ADC0_SE6b/PTD5/LPSPI1_SCK/LPUART2_TX/TPM0_CH5/LPUART0_CTS_b/FXIO0_D5}
  - {pin_num: '94', peripheral: FLEXIO0, signal: 'D, 1', pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1}
  - {pin_num: '93', peripheral: FLEXIO0, signal: 'D, 0', pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0}
  - {pin_num: '80', peripheral: LPI2C0, signal: SCL, pin_signal: CMP0_IN2/PTC8/LPI2C0_SCL/TPM0_CH4/I2S0_MCLK/FXIO0_D21, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '81', peripheral: LPI2C0, signal: SDA, pin_signal: CMP0_IN3/PTC9/LPI2C0_SDA/TPM0_CH5/I2S0_RX_BCLK/FXIO0_D22, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTA1 (pin 35) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 36) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    const port_pin_config_t portc8_pin80_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C0_SCL */
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC8 (pin 80) is configured as LPI2C0_SCL */
    PORT_SetPinConfig(PORTC, 8U, &portc8_pin80_config);

    const port_pin_config_t portc9_pin81_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C0_SDA */
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC9 (pin 81) is configured as LPI2C0_SDA */
    PORT_SetPinConfig(PORTC, 9U, &portc9_pin81_config);

    /* PORTD0 (pin 93) is configured as FXIO0_D0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt6);

    /* PORTD1 (pin 94) is configured as FXIO0_D1 */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt6);

    /* PORTD4 (pin 97) is configured as FXIO0_D4 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt6);

    /* PORTD5 (pin 98) is configured as FXIO0_D5 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt6);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
