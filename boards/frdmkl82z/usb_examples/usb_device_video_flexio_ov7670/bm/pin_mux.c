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
  - {pin_num: '58', peripheral: SIM, signal: CLKOUT, pin_signal: PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/TPM0_CH2/CLKOUT}
  - {pin_num: '40', peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/TPM_CLKIN0}
  - {pin_num: '41', peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/TPM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1}
  - {pin_num: '75', peripheral: I2C0, signal: SCL, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/I2C0_SCL, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '76', peripheral: I2C0, signal: SDA, pin_signal: PTD3/SPI0_SIN/LPUART2_TX/I2C0_SDA, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: '37', peripheral: FLEXIO0, signal: 'D, 23', pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23}
  - {pin_num: '50', peripheral: FLEXIO0, signal: 'D, 5', pin_signal: PTB11/SPI1_SCK/FXIO0_D5}
  - {pin_num: '63', peripheral: FLEXIO0, signal: 'D, 14', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/EXTRG_IN/FXIO0_D14}
  - {pin_num: '64', peripheral: FLEXIO0, signal: 'D, 15', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/FXIO0_D15}
  - {pin_num: '65', peripheral: FLEXIO0, signal: 'D, 16', pin_signal: CMP0_IN2/PTC8/FXIO0_D16}
  - {pin_num: '66', peripheral: FLEXIO0, signal: 'D, 17', pin_signal: CMP0_IN3/PTC9/FXIO0_D17}
  - {pin_num: '67', peripheral: FLEXIO0, signal: 'D, 18', pin_signal: PTC10/I2C1_SCL/FXIO0_D18}
  - {pin_num: '68', peripheral: FLEXIO0, signal: 'D, 19', pin_signal: PTC11/LLWU_P11/I2C1_SDA/FXIO0_D19}
  - {pin_num: '34', peripheral: FLEXIO0, signal: 'D, 20', pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/FXIO0_D20}
  - {pin_num: '35', peripheral: FLEXIO0, signal: 'D, 21', pin_signal: PTA15/SPI0_SCK/LPUART0_RX/FXIO0_D21}
  - {pin_num: '61', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/TPM0_CH3, slew_rate: slow, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '62', peripheral: GPIOC, signal: 'GPIO, 5', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/LPTMR1_ALT2/CMP0_OUT/TPM0_CH2, slew_rate: slow, open_drain: enable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '36', peripheral: GPIOA, signal: 'GPIO, 16', pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/FXIO0_D22, slew_rate: fast, open_drain: enable, pull_select: up,
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
    /* PTA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* PTB Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* PTC Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* PTD Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTA14 (pin 34) is configured as FXIO0_D20 */
    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt5);

    /* PORTA15 (pin 35) is configured as FXIO0_D21 */
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt5);

    const port_pin_config_t porta16_pin36_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Pin is configured as PTA16 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA16 (pin 36) is configured as PTA16 */
    PORT_SetPinConfig(PORTA, 16U, &porta16_pin36_config);

    /* PORTA17 (pin 37) is configured as FXIO0_D23 */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt5);

    /* PORTA18 (pin 40) is configured as EXTAL0 */
    PORT_SetPinMux(PORTA, 18U, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin 41) is configured as XTAL0 */
    PORT_SetPinMux(PORTA, 19U, kPORT_PinDisabledOrAnalog);

    /* PORTB11 (pin 50) is configured as FXIO0_D5 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt7);

    /* PORTB16 (pin 51) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 52) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    /* PORTC10 (pin 67) is configured as FXIO0_D18 */
    PORT_SetPinMux(PORTC, 10U, kPORT_MuxAlt7);

    /* PORTC11 (pin 68) is configured as FXIO0_D19 */
    PORT_SetPinMux(PORTC, 11U, kPORT_MuxAlt7);

    /* PORTC3 (pin 58) is configured as CLKOUT */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt5);

    const port_pin_config_t portc4_pin61_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Pin is configured as PTC4 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC4 (pin 61) is configured as PTC4 */
    PORT_SetPinConfig(PORTC, 4U, &portc4_pin61_config);

    const port_pin_config_t portc5_pin62_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Pin is configured as PTC5 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC5 (pin 62) is configured as PTC5 */
    PORT_SetPinConfig(PORTC, 5U, &portc5_pin62_config);

    /* PORTC6 (pin 63) is configured as FXIO0_D14 */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt7);

    /* PORTC7 (pin 64) is configured as FXIO0_D15 */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt7);

    /* PORTC8 (pin 65) is configured as FXIO0_D16 */
    PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt7);

    /* PORTC9 (pin 66) is configured as FXIO0_D17 */
    PORT_SetPinMux(PORTC, 9U, kPORT_MuxAlt7);

    const port_pin_config_t portd2_pin75_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
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

    const port_pin_config_t portd3_pin76_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
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
