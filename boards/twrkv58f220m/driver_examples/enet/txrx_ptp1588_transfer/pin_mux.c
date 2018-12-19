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
processor: MKV58F1M0xxx24
package_id: MKV58F1M0VLQ24
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
  - {pin_num: '81', peripheral: UART0, signal: RX, pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO}
  - {pin_num: '82', peripheral: UART0, signal: TX, pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC}
  - {pin_num: '123', peripheral: ENET, signal: 'TMR_1588, 0', pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FLEXPWM1_A2/FB_CS5_b/FB_TSIZ1/FB_BE23_16_b}
  - {pin_num: '124', peripheral: ENET, signal: 'TMR_1588, 1', pin_signal: PTC17/CAN1_TX/UART3_TX/ENET0_1588_TMR1/FLEXPWM1_B2/FB_CS4_b/FB_TSIZ0/FB_BE31_24_b}
  - {pin_num: '125', peripheral: ENET, signal: 'TMR_1588, 2', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FLEXPWM1_A3/FB_TBST_b/FB_CS2_b/FB_BE15_8_b}
  - {pin_num: '126', peripheral: ENET, signal: 'TMR_1588, 3', pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FLEXPWM1_B3/FB_CS3_b/FB_BE7_0_b/FB_TA_b}
  - {pin_num: '55', peripheral: ENET, signal: MII_RXER, pin_signal: PTA5/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/JTAG_TRST_b}
  - {pin_num: '59', peripheral: ENET, signal: MII_MDIO, pin_signal: HSADC1B_CH8/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3, slew_rate: slow, open_drain: enable,
    pull_select: down, pull_enable: disable}
  - {pin_num: '60', peripheral: ENET, signal: MII_MDC, pin_signal: HSADC1B_CH9/PTA8/FTM1_CH0/RMII0_MDC/MII0_MDC/TRACE_D2}
  - {pin_num: '61', peripheral: ENET, signal: MII_RXD3, pin_signal: PTA9/FTM1_CH1/MII0_RXD3/TRACE_D1}
  - {pin_num: '62', peripheral: ENET, signal: MII_RXD2, pin_signal: PTA10/LLWU_P22/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TRACE_D0}
  - {pin_num: '63', peripheral: ENET, signal: MII_RXCLK, pin_signal: PTA11/LLWU_P23/FTM2_CH1/MII0_RXCLK/FTM2_QD_PHB/I2C0_SDA}
  - {pin_num: '64', peripheral: ENET, signal: MII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/FTM1_QD_PHA/I2C0_SCL}
  - {pin_num: '65', peripheral: ENET, signal: MII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/FTM1_QD_PHB/I2C1_SDA}
  - {pin_num: '66', peripheral: ENET, signal: MII_RXDV, pin_signal: CMP3_IN0/PTA14/SPI0_PCS0/UART0_TX/CAN2_TX/RMII0_CRS_DV/MII0_RXDV/I2C1_SCL}
  - {pin_num: '67', peripheral: ENET, signal: MII_TXEN, pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/CAN2_RX/RMII0_TXEN/MII0_TXEN}
  - {pin_num: '68', peripheral: ENET, signal: MII_TXD0, pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0}
  - {pin_num: '69', peripheral: ENET, signal: MII_TXD1, pin_signal: HSADC0A_CH15/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1}
  - {pin_num: '76', peripheral: ENET, signal: MII_TXCLK, pin_signal: PTA25/XB_IN5/MII0_TXCLK/FB_A28}
  - {pin_num: '77', peripheral: ENET, signal: MII_TXD3, pin_signal: PTA26/MII0_TXD3/FB_A27}
  - {pin_num: '78', peripheral: ENET, signal: MII_CRS, pin_signal: PTA27/MII0_CRS/FB_A26}
  - {pin_num: '79', peripheral: ENET, signal: MII_TXER, pin_signal: PTA28/MII0_TXER/FB_A25}
  - {pin_num: '80', peripheral: ENET, signal: MII_COL, pin_signal: PTA29/MII0_COL/FB_A24}
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

    /* PORTA10 (pin 62) is configured as MII0_RXD2 */
    PORT_SetPinMux(PORTA, 10U, kPORT_MuxAlt5);

    /* PORTA11 (pin 63) is configured as MII0_RXCLK */
    PORT_SetPinMux(PORTA, 11U, kPORT_MuxAlt5);

    /* PORTA12 (pin 64) is configured as MII0_RXD1 */
    PORT_SetPinMux(PORTA, 12U, kPORT_MuxAlt5);

    /* PORTA13 (pin 65) is configured as MII0_RXD0 */
    PORT_SetPinMux(PORTA, 13U, kPORT_MuxAlt5);

    /* PORTA14 (pin 66) is configured as MII0_RXDV */
    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt5);

    /* PORTA15 (pin 67) is configured as MII0_TXEN */
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt5);

    /* PORTA16 (pin 68) is configured as MII0_TXD0 */
    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt5);

    /* PORTA17 (pin 69) is configured as MII0_TXD1 */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt5);

    /* PORTA25 (pin 76) is configured as MII0_TXCLK */
    PORT_SetPinMux(PORTA, 25U, kPORT_MuxAlt5);

    /* PORTA26 (pin 77) is configured as MII0_TXD3 */
    PORT_SetPinMux(PORTA, 26U, kPORT_MuxAlt5);

    /* PORTA27 (pin 78) is configured as MII0_CRS */
    PORT_SetPinMux(PORTA, 27U, kPORT_MuxAlt5);

    /* PORTA28 (pin 79) is configured as MII0_TXER */
    PORT_SetPinMux(PORTA, 28U, kPORT_MuxAlt5);

    /* PORTA29 (pin 80) is configured as MII0_COL */
    PORT_SetPinMux(PORTA, 29U, kPORT_MuxAlt5);

    /* PORTA5 (pin 55) is configured as MII0_RXER */
    PORT_SetPinMux(PORTA, 5U, kPORT_MuxAlt4);

    const port_pin_config_t porta7_pin59_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Slow slew rate is configured */
                                                   kPORT_SlowSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Open drain is enabled */
                                                   kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as MII0_MDIO */
                                                   kPORT_MuxAlt5,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA7 (pin 59) is configured as MII0_MDIO */
    PORT_SetPinConfig(PORTA, 7U, &porta7_pin59_config);

    /* PORTA8 (pin 60) is configured as MII0_MDC */
    PORT_SetPinMux(PORTA, 8U, kPORT_MuxAlt5);

    /* PORTA9 (pin 61) is configured as MII0_RXD3 */
    PORT_SetPinMux(PORTA, 9U, kPORT_MuxAlt5);

    /* PORTB0 (pin 81) is configured as UART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt7);

    /* PORTB1 (pin 82) is configured as UART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt7);

    /* PORTC16 (pin 123) is configured as ENET0_1588_TMR0 */
    PORT_SetPinMux(PORTC, 16U, kPORT_MuxAlt4);

    /* PORTC17 (pin 124) is configured as ENET0_1588_TMR1 */
    PORT_SetPinMux(PORTC, 17U, kPORT_MuxAlt4);

    /* PORTC18 (pin 125) is configured as ENET0_1588_TMR2 */
    PORT_SetPinMux(PORTC, 18U, kPORT_MuxAlt4);

    /* PORTC19 (pin 126) is configured as ENET0_1588_TMR3 */
    PORT_SetPinMux(PORTC, 19U, kPORT_MuxAlt4);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 receive data source select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
