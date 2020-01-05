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
package_id: MKL82Z128VMC7
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
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/TPM0_CH2/CLKOUT}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/TPM0_CH3}
  - {pin_num: B1, peripheral: RTC, signal: CLKOUT, pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: L11, peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/TPM_CLKIN0}
  - {pin_num: K11, peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/TPM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1}
  - {pin_num: B11, peripheral: EMVSIM1, signal: IO, pin_signal: PTB4/EMVSIM1_IO, slew_rate: fast, open_drain: enable, pull_select: down, pull_enable: disable}
  - {pin_num: C11, peripheral: EMVSIM1, signal: CLK, pin_signal: PTB5/EMVSIM1_CLK, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: D11, peripheral: EMVSIM1, signal: RST, pin_signal: PTB8/EMVSIM1_RST, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: D6, peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: CMP0_IN3/PTC9/FXIO0_D17, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: C8, peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: ADC0_SE4b/TSI0_CH15/PTC2/SPI0_PCS2/LPUART1_CTS_b/TPM0_CH1, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: E11, peripheral: EMVSIM1, signal: PD, pin_signal: PTB7/EMVSIM1_PD}
  - {pin_num: L8, peripheral: GPIOA, signal: 'GPIO, 13', pin_signal: PTA13/LLWU_P4/TPM1_CH1/FXIO0_D19, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: F11, peripheral: EMVSIM1, signal: VCCEN, pin_signal: PTB6/EMVSIM1_VCCEN}
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
    /* PTA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* PTB Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* PTC Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* PTE Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porta13_pinL8_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Pin is configured as PTA13 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA13 (pin L8) is configured as PTA13 */
    PORT_SetPinConfig(PORTA, 13U, &porta13_pinL8_config);

    /* PORTA18 (pin L11) is configured as EXTAL0 */
    PORT_SetPinMux(PORTA, 18U, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin K11) is configured as XTAL0 */
    PORT_SetPinMux(PORTA, 19U, kPORT_PinDisabledOrAnalog);

    const port_pin_config_t portb4_pinB11_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Pin is configured as EMVSIM1_IO */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB4 (pin B11) is configured as EMVSIM1_IO */
    PORT_SetPinConfig(PORTB, 4U, &portb4_pinB11_config);

    const port_pin_config_t portb5_pinC11_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Pin is configured as EMVSIM1_CLK */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB5 (pin C11) is configured as EMVSIM1_CLK */
    PORT_SetPinConfig(PORTB, 5U, &portb5_pinC11_config);

    /* PORTB6 (pin F11) is configured as EMVSIM1_VCCEN */
    PORT_SetPinMux(PORTB, 6U, kPORT_MuxAlt2);

    /* PORTB7 (pin E11) is configured as EMVSIM1_PD */
    PORT_SetPinMux(PORTB, 7U, kPORT_MuxAlt2);

    const port_pin_config_t portb8_pinD11_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Pin is configured as EMVSIM1_RST */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB8 (pin D11) is configured as EMVSIM1_RST */
    PORT_SetPinConfig(PORTB, 8U, &portb8_pinD11_config);

    const port_pin_config_t portc2_pinC8_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Pin is configured as PTC2 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC2 (pin C8) is configured as PTC2 */
    PORT_SetPinConfig(PORTC, 2U, &portc2_pinC8_config);

    /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin A8) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    const port_pin_config_t portc9_pinD6_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Pin is configured as PTC9 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC9 (pin D6) is configured as PTC9 */
    PORT_SetPinConfig(PORTC, 9U, &portc9_pinD6_config);

    /* PORTE0 (pin B1) is configured as RTC_CLKOUT */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt7);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK)))

                  /* LPUART1 transmit data source select: LPUART1_TX pin. */
                  | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX)

                  /* LPUART1 receive data source select: LPUART1_RX pin. */
                  | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
