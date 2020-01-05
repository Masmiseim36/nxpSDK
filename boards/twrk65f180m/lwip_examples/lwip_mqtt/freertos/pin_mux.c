/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MK65FN2M0xxx18
package_id: MK65FN2M0VMI18
mcu_data: ksdk2_0
processor_version: 0.0.10
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
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: G11, peripheral: ENET, signal: 'TMR_1588, 0', pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/UART0_RTS_b/ENET0_1588_TMR0/SDRAM_WE/FTM0_FLT3}
  - {pin_num: G10, peripheral: ENET, signal: 'TMR_1588, 1', pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/SDRAM_CS0_b/FTM0_FLT0}
  - {pin_num: H9, peripheral: ENET, signal: 'TMR_1588, 2', pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/SDRAM_CS1_b/FTM1_FLT0, slew_rate: slow, open_drain: enable,
    pull_select: down, pull_enable: disable}
  - {pin_num: F13, peripheral: ENET, signal: 'TMR_1588, 3', pin_signal: ADC1_SE11/PTB5/ENET0_1588_TMR3/FTM2_FLT0, slew_rate: slow, open_drain: enable, pull_select: down,
    pull_enable: disable}
  - {pin_num: L10, peripheral: ENET, signal: MII_MDIO, pin_signal: ADC0_SE10/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3}
  - {pin_num: K9, peripheral: ENET, signal: RMII_MDC, pin_signal: ADC0_SE11/PTA8/FTM1_CH0/RMII0_MDC/MII0_MDC/FTM1_QD_PHA/TPM1_CH0/TRACE_D2}
  - {pin_num: L12, peripheral: ENET, signal: MII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: L11, peripheral: ENET, signal: MII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: K13, peripheral: ENET, signal: RMII_CRS_DV, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: K12, peripheral: ENET, signal: RMII_TXEN, pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0}
  - {pin_num: J13, peripheral: ENET, signal: MII_TXD0, pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: J12, peripheral: ENET, signal: RMII_TXD1, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK}
  - {pin_num: N11, peripheral: GPIOA, signal: 'GPIO, 10', pin_signal: PTA10/LLWU_P22/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TPM2_CH0/TRACE_D0, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: H12, peripheral: GPIOA, signal: 'GPIO, 28', pin_signal: PTA28/MII0_TXER/FB_A25}
  - {pin_num: N10, peripheral: ENET, signal: RMII_RXER, pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: K8, peripheral: ENET, signal: CLKIN_1588, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porta10_pinN11_config = {/* Internal pull-up resistor is enabled */
                                                     kPORT_PullUp,
                                                     /* Fast slew rate is configured */
                                                     kPORT_FastSlewRate,
                                                     /* Passive filter is disabled */
                                                     kPORT_PassiveFilterDisable,
                                                     /* Open drain is disabled */
                                                     kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     kPORT_LowDriveStrength,
                                                     /* Pin is configured as PTA10 */
                                                     kPORT_MuxAsGpio,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};
    /* PORTA10 (pin N11) is configured as PTA10 */
    PORT_SetPinConfig(PORTA, 10U, &porta10_pinN11_config);

    /* PORTA12 (pin L12) is configured as MII0_RXD1 */
    PORT_SetPinMux(PORTA, 12U, kPORT_MuxAlt4);

    /* PORTA13 (pin L11) is configured as MII0_RXD0 */
    PORT_SetPinMux(PORTA, 13U, kPORT_MuxAlt4);

    /* PORTA14 (pin K13) is configured as RMII0_CRS_DV */
    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt4);

    /* PORTA15 (pin K12) is configured as RMII0_TXEN */
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt4);

    /* PORTA16 (pin J13) is configured as MII0_TXD0 */
    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt4);

    /* PORTA17 (pin J12) is configured as RMII0_TXD1 */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt4);

    /* PORTA28 (pin H12) is configured as PTA28 */
    PORT_SetPinMux(PORTA, 28U, kPORT_MuxAsGpio);

    /* PORTA5 (pin N10) is configured as RMII0_RXER */
    PORT_SetPinMux(PORTA, 5U, kPORT_MuxAlt4);

    /* PORTA7 (pin L10) is configured as MII0_MDIO */
    PORT_SetPinMux(PORTA, 7U, kPORT_MuxAlt5);

    /* PORTA8 (pin K9) is configured as RMII0_MDC */
    PORT_SetPinMux(PORTA, 8U, kPORT_MuxAlt5);

    /* PORTB2 (pin G11) is configured as ENET0_1588_TMR0 */
    PORT_SetPinMux(PORTB, 2U, kPORT_MuxAlt4);

    /* PORTB3 (pin G10) is configured as ENET0_1588_TMR1 */
    PORT_SetPinMux(PORTB, 3U, kPORT_MuxAlt4);

    const port_pin_config_t portb4_pinH9_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as ENET0_1588_TMR2 */
                                                   kPORT_MuxAlt4,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTB4 (pin H9) is configured as ENET0_1588_TMR2 */
    PORT_SetPinConfig(PORTB, 4U, &portb4_pinH9_config);

    const port_pin_config_t portb5_pinF13_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as ENET0_1588_TMR3 */
                                                    kPORT_MuxAlt4,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTB5 (pin F13) is configured as ENET0_1588_TMR3 */
    PORT_SetPinConfig(PORTB, 5U, &portb5_pinF13_config);

    /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt3);

    /* PORTE17 (pin F5) is configured as UART2_RX */
    PORT_SetPinMux(PORTE, 17U, kPORT_MuxAlt3);

    /* PORTE26 (pin K8) is configured as ENET_1588_CLKIN */
    PORT_SetPinMux(PORTE, 26U, kPORT_MuxAlt2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
