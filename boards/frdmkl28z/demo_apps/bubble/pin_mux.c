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
board: FRDM-KL28Z
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
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '38', peripheral: TSI0, signal: 'CH, 5', pin_signal: TSI0_CH5/PTA4/LLWU_P3/LPI2C1_SDA/TPM0_CH1/NMI0_b}
  - {pin_num: '76', peripheral: TPM0, signal: 'CH, 3', pin_signal: PTC4/LLWU_P8/LPSPI0_PCS0/LPUART1_TX/TPM0_CH3/I2S0_MCLK/CMP1_OUT}
  - {pin_num: '26', peripheral: TPM0, signal: 'CH, 2', pin_signal: CMP1_IN5/CMP0_IN5/ADC0_SE4b/PTE29/EMVSIM0_CLK/TPM0_CH2/TPM0_CLKIN}
  - {pin_num: '62', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16}
  - {pin_num: '63', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17}
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
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA4 (pin 38) is configured as TSI0_CH5 */
    PORT_SetPinMux(BOARD_SW2_PORT, BOARD_SW2_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB16 (pin 62) is configured as LPUART0_RX */
    PORT_SetPinMux(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, kPORT_MuxAlt3);

    /* PORTB17 (pin 63) is configured as LPUART0_TX */
    PORT_SetPinMux(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, kPORT_MuxAlt3);

    /* PORTC4 (pin 76) is configured as TPM0_CH3 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAlt4);

    /* PORTE29 (pin 26) is configured as TPM0_CH2 */
    PORT_SetPinMux(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '31', peripheral: LPI2C0, signal: SCL, pin_signal: ADC0_SE20/PTE24/EMVSIM0_IO/TPM0_CH0/LPI2C0_SCL, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '32', peripheral: LPI2C0, signal: SDA, pin_signal: ADC0_SE21/PTE25/LLWU_P21/EMVSIM0_PD/TPM0_CH1/LPI2C0_SDA, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 *
 * END ****************************************************************************************************************/
void BOARD_I2C_ConfigurePins(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte24_pin31_config = {/* Internal pull-up resistor is enabled */
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
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE24 (pin 31) is configured as LPI2C0_SCL */
    PORT_SetPinConfig(PORTE, 24U, &porte24_pin31_config);

    const port_pin_config_t porte25_pin32_config = {/* Internal pull-up resistor is enabled */
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
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE25 (pin 32) is configured as LPI2C0_SDA */
    PORT_SetPinConfig(PORTE, 25U, &porte25_pin32_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
