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
processor: MK28FN2M0Axxx15
package_id: MK28FN2M0AVMI15
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
  - {pin_num: A7, peripheral: LPUART0, signal: RX, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: B7, peripheral: LPUART0, signal: TX, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
  - {pin_num: K9, peripheral: I2C1, signal: SCL, pin_signal: ADC0_SE11/PTA8/I2C1_SCL/FTM1_CH0/FTM1_QD_PHA/TPM1_CH0/TRACE_D2, slew_rate: fast, open_drain: enable,
    pull_select: down, pull_enable: disable}
  - {pin_num: K10, peripheral: I2C1, signal: SDA, pin_signal: PTA9/I2C1_SDA/FTM1_CH1/FTM1_QD_PHB/TPM1_CH1/TRACE_D1, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable}
  - {pin_num: L8, peripheral: FLEXIO0, signal: 'D, 7', pin_signal: PTA23/LPUART4_RTS_b/FXIO0_D7, slew_rate: slow, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: L7, peripheral: FLEXIO0, signal: 'D, 8', pin_signal: PTA20/I2C0_SCL/LPUART4_TX/FTM_CLKIN1/FXIO0_D8/EWM_OUT_b/TPM_CLKIN1, slew_rate: slow, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: N11, peripheral: FLEXIO0, signal: 'D, 16', pin_signal: PTA10/LLWU_P22/I2C2_SDA/FTM2_CH0/FXIO0_D16/FTM2_QD_PHA/TPM2_CH0/TRACE_D0}
  - {pin_num: M11, peripheral: FLEXIO0, signal: 'D, 17', pin_signal: PTA11/LLWU_P23/I2C2_SCL/FTM2_CH1/FXIO0_D17/FTM2_QD_PHB/TPM2_CH1/USB1_ID}
  - {pin_num: L12, peripheral: FLEXIO0, signal: 'D, 18', pin_signal: PTA12/FTM1_CH0/TRACE_CLKOUT/FXIO0_D18/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: L11, peripheral: FLEXIO0, signal: 'D, 19', pin_signal: PTA13/LLWU_P4/FTM1_CH1/TRACE_D3/FXIO0_D19/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: K13, peripheral: FLEXIO0, signal: 'D, 20', pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: K12, peripheral: FLEXIO0, signal: 'D, 21', pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: J13, peripheral: FLEXIO0, signal: 'D, 22', pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: J12, peripheral: FLEXIO0, signal: 'D, 23', pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK/I2S1_MCLK}
  - {pin_num: D5, peripheral: FLEXIO0, signal: 'D, 24', pin_signal: PTD8/LLWU_P24/I2C0_SCL/LPUART1_RX/FB_A16/FXIO0_D24}
  - {pin_num: D4, peripheral: FLEXIO0, signal: 'D, 25', pin_signal: PTD9/I2C0_SDA/LPUART1_TX/FB_A17/FXIO0_D25}
  - {pin_num: D3, peripheral: FLEXIO0, signal: 'D, 26', pin_signal: PTD10/LPUART1_RTS_b/FB_A18/FXIO0_D26}
  - {pin_num: C2, peripheral: FLEXIO0, signal: 'D, 27', pin_signal: PTD11/LLWU_P25/SPI2_PCS0/LPUART1_CTS_b/FB_A19/FXIO0_D27}
  - {pin_num: B2, peripheral: FLEXIO0, signal: 'D, 28', pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/FB_A20/FXIO0_D28}
  - {pin_num: B3, peripheral: FLEXIO0, signal: 'D, 29', pin_signal: PTD13/SPI2_SOUT/FB_A21/FXIO0_D29}
  - {pin_num: A2, peripheral: FLEXIO0, signal: 'D, 30', pin_signal: PTD14/SPI2_SIN/FB_A22/FXIO0_D30}
  - {pin_num: A3, peripheral: FLEXIO0, signal: 'D, 31', pin_signal: PTD15/SPI2_PCS1/FB_A23/FXIO0_D31}
  - {pin_num: K8, peripheral: GPIOA, signal: 'GPIO, 22', pin_signal: PTA22/LPUART4_CTS_b/FXIO0_D6/RTC_CLKOUT/USB0_CLKIN, direction: OUTPUT, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: M10, peripheral: GPIOA, signal: 'GPIO, 6', pin_signal: PTA6/I2C2_SCL/FTM0_CH3/CLKOUT/TRACE_CLKOUT, direction: OUTPUT, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: K7, peripheral: GPIOA, signal: 'GPIO, 21', pin_signal: PTA21/LLWU_P21/I2C0_SDA/LPUART4_RX/FXIO0_D9/EWM_IN, direction: INPUT}
  - {pin_num: N10, peripheral: GPIOA, signal: 'GPIO, 5', pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/FXIO0_D15/I2S0_TX_BCLK/JTAG_TRST_b, direction: OUTPUT}
  - {pin_num: M9, peripheral: GPIOA, signal: 'GPIO, 2', pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/JTAG_TDO/TRACE_SWO, direction: INPUT}
  - {pin_num: C1, peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/FXIO0_D0/SPI1_SCK}
  - {pin_num: E1, peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1}
  - {pin_num: D1, peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/FXIO0_D1/SPI1_SOUT}
  - {pin_num: A1, peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: B1, peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN}
  - {pin_num: D2, peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA10 (pin N11) is configured as FXIO0_D16 */
    PORT_SetPinMux(PORTA, 10U, kPORT_MuxAlt5);

    /* PORTA11 (pin M11) is configured as FXIO0_D17 */
    PORT_SetPinMux(PORTA, 11U, kPORT_MuxAlt5);

    /* PORTA12 (pin L12) is configured as FXIO0_D18 */
    PORT_SetPinMux(PORTA, 12U, kPORT_MuxAlt5);

    /* PORTA13 (pin L11) is configured as FXIO0_D19 */
    PORT_SetPinMux(PORTA, 13U, kPORT_MuxAlt5);

    /* PORTA14 (pin K13) is configured as FXIO0_D20 */
    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt5);

    /* PORTA15 (pin K12) is configured as FXIO0_D21 */
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt5);

    /* PORTA16 (pin J13) is configured as FXIO0_D22 */
    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt5);

    /* PORTA17 (pin J12) is configured as FXIO0_D23 */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt5);

    /* PORTA2 (pin M9) is configured as PTA2 */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAsGpio);

    const port_pin_config_t porta20_pinL7_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as FXIO0_D8 */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA20 (pin L7) is configured as FXIO0_D8 */
    PORT_SetPinConfig(PORTA, 20U, &porta20_pinL7_config);

    /* PORTA21 (pin K7) is configured as PTA21 */
    PORT_SetPinMux(PORTA, 21U, kPORT_MuxAsGpio);

    const port_pin_config_t porta22_pinK8_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTA22 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA22 (pin K8) is configured as PTA22 */
    PORT_SetPinConfig(PORTA, 22U, &porta22_pinK8_config);

    const port_pin_config_t porta23_pinL8_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as FXIO0_D7 */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA23 (pin L8) is configured as FXIO0_D7 */
    PORT_SetPinConfig(PORTA, 23U, &porta23_pinL8_config);

    /* PORTA5 (pin N10) is configured as PTA5 */
    PORT_SetPinMux(PORTA, 5U, kPORT_MuxAsGpio);

    const port_pin_config_t porta6_pinM10_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTA6 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA6 (pin M10) is configured as PTA6 */
    PORT_SetPinConfig(PORTA, 6U, &porta6_pinM10_config);

    const port_pin_config_t porta8_pinK9_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C1_SCL */
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA8 (pin K9) is configured as I2C1_SCL */
    PORT_SetPinConfig(PORTA, 8U, &porta8_pinK9_config);

    const port_pin_config_t porta9_pinK10_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C1_SDA */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA9 (pin K10) is configured as I2C1_SDA */
    PORT_SetPinConfig(PORTA, 9U, &porta9_pinK10_config);

    /* PORTC24 (pin B7) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTC, 24U, kPORT_MuxAlt3);

    /* PORTC25 (pin A7) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTC, 25U, kPORT_MuxAlt3);

    /* PORTD10 (pin D3) is configured as FXIO0_D26 */
    PORT_SetPinMux(PORTD, 10U, kPORT_MuxAlt7);

    /* PORTD11 (pin C2) is configured as FXIO0_D27 */
    PORT_SetPinMux(PORTD, 11U, kPORT_MuxAlt7);

    /* PORTD12 (pin B2) is configured as FXIO0_D28 */
    PORT_SetPinMux(PORTD, 12U, kPORT_MuxAlt7);

    /* PORTD13 (pin B3) is configured as FXIO0_D29 */
    PORT_SetPinMux(PORTD, 13U, kPORT_MuxAlt7);

    /* PORTD14 (pin A2) is configured as FXIO0_D30 */
    PORT_SetPinMux(PORTD, 14U, kPORT_MuxAlt7);

    /* PORTD15 (pin A3) is configured as FXIO0_D31 */
    PORT_SetPinMux(PORTD, 15U, kPORT_MuxAlt7);

    /* PORTD8 (pin D5) is configured as FXIO0_D24 */
    PORT_SetPinMux(PORTD, 8U, kPORT_MuxAlt7);

    /* PORTD9 (pin D4) is configured as FXIO0_D25 */
    PORT_SetPinMux(PORTD, 9U, kPORT_MuxAlt7);

    /* PORTE0 (pin A1) is configured as QSPI0A_DATA3 */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt5);

    /* PORTE1 (pin B1) is configured as QSPI0A_SCLK */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt5);

    /* PORTE2 (pin C1) is configured as QSPI0A_DATA0 */
    PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt5);

    /* PORTE3 (pin D1) is configured as QSPI0A_DATA2 */
    PORT_SetPinMux(PORTE, 3U, kPORT_MuxAlt5);

    /* PORTE4 (pin E1) is configured as QSPI0A_DATA1 */
    PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt5);

    /* PORTE5 (pin D2) is configured as QSPI0A_SS0_B */
    PORT_SetPinMux(PORTE, 5U, kPORT_MuxAlt5);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
