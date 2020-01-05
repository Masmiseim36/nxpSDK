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
  - {pin_num: '53', peripheral: FLEXIO0, signal: 'D, 0', pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0/FXIO0_D0}
  - {pin_num: '56', peripheral: FLEXIO0, signal: 'D, 3', pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/LPUART0_CTS_b/SDRAM_CS0_b/FTM0_FLT0/FXIO0_D3}
  - {pin_num: '58', peripheral: FLEXIO0, signal: 'D, 4', pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: '59', peripheral: FLEXIO0, signal: 'D, 5', pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
  - {pin_num: '64', peripheral: FLEXIO0, signal: 'D, 6', pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6}
  - {pin_num: '65', peripheral: FLEXIO0, signal: 'D, 7', pin_signal: TSI0_CH12/PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1/FXIO0_D7}
  - {pin_num: '66', peripheral: FLEXIO0, signal: 'D, 8', pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT/FXIO0_D8}
  - {pin_num: '67', peripheral: FLEXIO0, signal: 'D, 9', pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT/FXIO0_D9}
  - {pin_num: '68', peripheral: FLEXIO0, signal: 'D, 10', pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/FXIO0_D10}
  - {pin_num: '69', peripheral: FLEXIO0, signal: 'D, 11', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/FXIO0_D11}
  - {pin_num: '37', peripheral: I2C3, signal: SDA, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI, slew_rate: fast, open_drain: enable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '38', peripheral: I2C3, signal: SCL, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, slew_rate: fast,
    open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: '73', peripheral: SIM, signal: CLKOUT, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: '50', peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0}
  - {pin_num: '51', peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1}
  - {pin_num: '80', peripheral: GPIOC, signal: 'GPIO, 8', pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16, slew_rate: slow, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '81', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, slew_rate: slow, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '55', peripheral: GPIOB, signal: 'GPIO, 2', pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/LPUART0_RTS_b/SDRAM_WE/FTM0_FLT3/FXIO0_D2, slew_rate: fast,
    open_drain: disable, pull_select: up, pull_enable: enable}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t porta1_pin37_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C3_SDA */
                                                   kPORT_MuxAlt4,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA1 (pin 37) is configured as I2C3_SDA */
    PORT_SetPinConfig(PORTA, 1U, &porta1_pin37_config);

    /* PORTA18 (pin 50) is configured as EXTAL0 */
    PORT_SetPinMux(PORTA, 18U, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin 51) is configured as XTAL0 */
    PORT_SetPinMux(PORTA, 19U, kPORT_PinDisabledOrAnalog);

    const port_pin_config_t porta2_pin38_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C3_SCL */
                                                   kPORT_MuxAlt4,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA2 (pin 38) is configured as I2C3_SCL */
    PORT_SetPinConfig(PORTA, 2U, &porta2_pin38_config);

    /* PORTB0 (pin 53) is configured as FXIO0_D0 */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt7);

    /* PORTB10 (pin 58) is configured as FXIO0_D4 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt7);

    /* PORTB11 (pin 59) is configured as FXIO0_D5 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt7);

    /* PORTB18 (pin 64) is configured as FXIO0_D6 */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAlt7);

    /* PORTB19 (pin 65) is configured as FXIO0_D7 */
    PORT_SetPinMux(PORTB, 19U, kPORT_MuxAlt7);

    const port_pin_config_t portb2_pin55_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTB2 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTB2 (pin 55) is configured as PTB2 */
    PORT_SetPinConfig(PORTB, 2U, &portb2_pin55_config);

    /* PORTB20 (pin 66) is configured as FXIO0_D8 */
    PORT_SetPinMux(PORTB, 20U, kPORT_MuxAlt7);

    /* PORTB21 (pin 67) is configured as FXIO0_D9 */
    PORT_SetPinMux(PORTB, 21U, kPORT_MuxAlt7);

    /* PORTB22 (pin 68) is configured as FXIO0_D10 */
    PORT_SetPinMux(PORTB, 22U, kPORT_MuxAlt7);

    /* PORTB23 (pin 69) is configured as FXIO0_D11 */
    PORT_SetPinMux(PORTB, 23U, kPORT_MuxAlt7);

    /* PORTB3 (pin 56) is configured as FXIO0_D3 */
    PORT_SetPinMux(PORTB, 3U, kPORT_MuxAlt7);

    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    /* PORTC3 (pin 73) is configured as CLKOUT */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt5);

    const port_pin_config_t portc8_pin80_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC8 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC8 (pin 80) is configured as PTC8 */
    PORT_SetPinConfig(PORTC, 8U, &portc8_pin80_config);

    const port_pin_config_t portc9_pin81_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is disabled */
                                                   kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC9 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTC9 (pin 81) is configured as PTC9 */
    PORT_SetPinConfig(PORTC, 9U, &portc9_pin81_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
