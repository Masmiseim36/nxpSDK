/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MK64FN1M0xxx12
package_id: MK64FN1M0VMD12
mcu_data: ksdk2_0
processor_version: 2.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define SOPT5_UART1TXSRC_UART_TX      0x00u   /*!< UART 1 transmit data source select: UART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A11, peripheral: UART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A9, peripheral: UART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);            /* PORTC3 (pin A11) is configured as UART1_RX */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);            /* PORTC4 (pin A9) is configured as UART1_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART1TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)       /* UART 1 transmit data source select: UART1_TX pin */
    );
}


#define PCR_DSE_LOW                   0x00u   /*!< Drive Strength Enable: Low drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_DISABLED               0x00u   /*!< Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
#define PCR_PFE_DISABLED              0x00u   /*!< Passive Filter Enable: Passive input filter is disabled on the corresponding pin. */
#define PCR_SRE_FAST                  0x00u   /*!< Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */
#define PIN27_IDX                       27u   /*!< Pin number for pin 27 in a port */
#define PIN28_IDX                       28u   /*!< Pin number for pin 28 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
ENET0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: M9, peripheral: ENET, signal: MII_RXD2, pin_signal: PTA10/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TRACE_D0}
  - {pin_num: L9, peripheral: ENET, signal: MII_RXCLK, pin_signal: PTA11/FTM2_CH1/MII0_RXCLK/I2C2_SDA/FTM2_QD_PHB}
  - {pin_num: K9, peripheral: ENET, signal: RMII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA}
  - {pin_num: J9, peripheral: ENET, signal: RMII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB}
  - {pin_num: L10, peripheral: ENET, signal: RMII_CRS_DV, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: L11, peripheral: ENET, signal: RMII_TXEN, pin_signal: PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0}
  - {pin_num: K10, peripheral: ENET, signal: RMII_TXD0, pin_signal: PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: K11, peripheral: ENET, signal: RMII_TXD1, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK}
  - {pin_num: K12, peripheral: ENET, signal: MII_TXD2, pin_signal: PTA24/MII0_TXD2/FB_A29}
  - {pin_num: J12, peripheral: ENET, signal: MII_TXCLK, pin_signal: PTA25/MII0_TXCLK/FB_A28}
  - {pin_num: J11, peripheral: ENET, signal: MII_TXD3, pin_signal: PTA26/MII0_TXD3/FB_A27}
  - {pin_num: J10, peripheral: ENET, signal: MII_CRS, pin_signal: PTA27/MII0_CRS/FB_A26}
  - {pin_num: H12, peripheral: ENET, signal: MII_TXER, pin_signal: PTA28/MII0_TXER/FB_A25}
  - {pin_num: M8, peripheral: ENET, signal: MII_RXER, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: L8, peripheral: ENET, signal: MII_RXD3, pin_signal: PTA9/FTM1_CH1/MII0_RXD3/FTM1_QD_PHB/TRACE_D1}
  - {pin_num: H10, peripheral: ENET, signal: RMII_MDIO, pin_signal: ADC0_SE8/ADC1_SE8/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/FTM1_QD_PHA, slew_rate: fast,
    open_drain: enable, drive_strength: low, pull_select: no_init, pull_enable: disable, passive_filter: disable}
  - {pin_num: H9, peripheral: ENET, signal: RMII_MDC, pin_signal: ADC0_SE9/ADC1_SE9/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/FTM1_QD_PHB}
  - {pin_num: A6, peripheral: ENET, signal: 'TMR_1588, 0', pin_signal: PTC16/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b}
  - {pin_num: D5, peripheral: ENET, signal: 'TMR_1588, 1', pin_signal: PTC17/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b}
  - {pin_num: C5, peripheral: ENET, signal: 'TMR_1588, 2', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b}
  - {pin_num: B5, peripheral: ENET, signal: 'TMR_1588, 3', pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/FB_TA_b}
  - {pin_num: H11, peripheral: ENET, signal: MII_COL, pin_signal: PTA29/MII0_COL/FB_A24}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void ENET0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN10_IDX, kPORT_MuxAlt4);           /* PORTA10 (pin M9) is configured as MII0_RXD2 */
  PORT_SetPinMux(PORTA, PIN11_IDX, kPORT_MuxAlt4);           /* PORTA11 (pin L9) is configured as MII0_RXCLK */
  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAlt4);           /* PORTA12 (pin K9) is configured as RMII0_RXD1 */
  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAlt4);           /* PORTA13 (pin J9) is configured as RMII0_RXD0 */
  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt4);           /* PORTA14 (pin L10) is configured as RMII0_CRS_DV */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt4);           /* PORTA15 (pin L11) is configured as RMII0_TXEN */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt4);           /* PORTA16 (pin K10) is configured as RMII0_TXD0 */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt4);           /* PORTA17 (pin K11) is configured as RMII0_TXD1 */
  PORT_SetPinMux(PORTA, PIN24_IDX, kPORT_MuxAlt4);           /* PORTA24 (pin K12) is configured as MII0_TXD2 */
  PORT_SetPinMux(PORTA, PIN25_IDX, kPORT_MuxAlt4);           /* PORTA25 (pin J12) is configured as MII0_TXCLK */
  PORT_SetPinMux(PORTA, PIN26_IDX, kPORT_MuxAlt4);           /* PORTA26 (pin J11) is configured as MII0_TXD3 */
  PORT_SetPinMux(PORTA, PIN27_IDX, kPORT_MuxAlt4);           /* PORTA27 (pin J10) is configured as MII0_CRS */
  PORT_SetPinMux(PORTA, PIN28_IDX, kPORT_MuxAlt4);           /* PORTA28 (pin H12) is configured as MII0_TXER */
  PORT_SetPinMux(PORTA, PIN29_IDX, kPORT_MuxAlt4);           /* PORTA29 (pin H11) is configured as MII0_COL */
  PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAlt4);            /* PORTA5 (pin M8) is configured as MII0_RXER */
  PORT_SetPinMux(PORTA, PIN9_IDX, kPORT_MuxAlt4);            /* PORTA9 (pin L8) is configured as MII0_RXD3 */
  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAlt4);            /* PORTB0 (pin H10) is configured as RMII0_MDIO */
  PORTB->PCR[0] = ((PORTB->PCR[0] &
    (~(PORT_PCR_PE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PE(PCR_PE_DISABLED)                         /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
      | PORT_PCR_SRE(PCR_SRE_FAST)                           /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
      | PORT_PCR_PFE(PCR_PFE_DISABLED)                       /* Passive Filter Enable: Passive input filter is disabled on the corresponding pin. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
      | PORT_PCR_DSE(PCR_DSE_LOW)                            /* Drive Strength Enable: Low drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAlt4);            /* PORTB1 (pin H9) is configured as RMII0_MDC */
  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_MuxAlt4);           /* PORTC16 (pin A6) is configured as ENET0_1588_TMR0 */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_MuxAlt4);           /* PORTC17 (pin D5) is configured as ENET0_1588_TMR1 */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAlt4);           /* PORTC18 (pin C5) is configured as ENET0_1588_TMR2 */
  PORT_SetPinMux(PORTC, PIN19_IDX, kPORT_MuxAlt4);           /* PORTC19 (pin B5) is configured as ENET0_1588_TMR3 */
}



#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */

#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */

#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */

#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */

#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */

#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */

#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */

#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */

#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */

#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */

#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */

#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */

#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */

#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */

#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */

#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */

#define PIN27_IDX                       27u   /*!< Pin number for pin 27 in a port */

#define PIN28_IDX                       28u   /*!< Pin number for pin 28 in a port */

#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
ENET0_DeinitPins:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: B5, peripheral: n/a, signal: disabled, pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/FB_TA_b}
  - {pin_num: C5, peripheral: n/a, signal: disabled, pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b}
  - {pin_num: D5, peripheral: n/a, signal: disabled, pin_signal: PTC17/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b}
  - {pin_num: A6, peripheral: n/a, signal: disabled, pin_signal: PTC16/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b}
  - {pin_num: L8, peripheral: n/a, signal: disabled, pin_signal: PTA9/FTM1_CH1/MII0_RXD3/FTM1_QD_PHB/TRACE_D1}
  - {pin_num: M8, peripheral: n/a, signal: disabled, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: H9, peripheral: ADC0, signal: 'SE, 9', pin_signal: ADC0_SE9/ADC1_SE9/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/FTM1_QD_PHB}
  - {pin_num: J9, peripheral: CMP2, signal: 'IN, 1', pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB}
  - {pin_num: K9, peripheral: CMP2, signal: 'IN, 0', pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA}
  - {pin_num: L9, peripheral: n/a, signal: disabled, pin_signal: PTA11/FTM2_CH1/MII0_RXCLK/I2C2_SDA/FTM2_QD_PHB}
  - {pin_num: M9, peripheral: n/a, signal: disabled, pin_signal: PTA10/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TRACE_D0}
  - {pin_num: H10, peripheral: ADC0, signal: 'SE, 8', pin_signal: ADC0_SE8/ADC1_SE8/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/FTM1_QD_PHA}
  - {pin_num: J10, peripheral: n/a, signal: disabled, pin_signal: PTA27/MII0_CRS/FB_A26}
  - {pin_num: K10, peripheral: n/a, signal: disabled, pin_signal: PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: L10, peripheral: n/a, signal: disabled, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: H11, peripheral: n/a, signal: disabled, pin_signal: PTA29/MII0_COL/FB_A24}
  - {pin_num: J11, peripheral: n/a, signal: disabled, pin_signal: PTA26/MII0_TXD3/FB_A27}
  - {pin_num: K11, peripheral: ADC1, signal: 'SE, 17', pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK}
  - {pin_num: L11, peripheral: n/a, signal: disabled, pin_signal: PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0}
  - {pin_num: H12, peripheral: n/a, signal: disabled, pin_signal: PTA28/MII0_TXER/FB_A25}
  - {pin_num: J12, peripheral: n/a, signal: disabled, pin_signal: PTA25/MII0_TXCLK/FB_A28}
  - {pin_num: K12, peripheral: n/a, signal: disabled, pin_signal: PTA24/MII0_TXD2/FB_A29}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void ENET0_DeinitPins(void) {
  PORT_SetPinMux(PORTA, PIN10_IDX, kPORT_PinDisabledOrAnalog); /* PORTA10 (pin M9) is disabled */
  PORT_SetPinMux(PORTA, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTA11 (pin L9) is disabled */
  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_PinDisabledOrAnalog); /* PORTA12 (pin K9) is configured as CMP2_IN0 */
  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_PinDisabledOrAnalog); /* PORTA13 (pin J9) is configured as CMP2_IN1 */
  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_PinDisabledOrAnalog); /* PORTA14 (pin L10) is disabled */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_PinDisabledOrAnalog); /* PORTA15 (pin L11) is disabled */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_PinDisabledOrAnalog); /* PORTA16 (pin K10) is disabled */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTA17 (pin K11) is configured as ADC1_SE17 */
  PORT_SetPinMux(PORTA, PIN24_IDX, kPORT_PinDisabledOrAnalog); /* PORTA24 (pin K12) is disabled */
  PORT_SetPinMux(PORTA, PIN25_IDX, kPORT_PinDisabledOrAnalog); /* PORTA25 (pin J12) is disabled */
  PORT_SetPinMux(PORTA, PIN26_IDX, kPORT_PinDisabledOrAnalog); /* PORTA26 (pin J11) is disabled */
  PORT_SetPinMux(PORTA, PIN27_IDX, kPORT_PinDisabledOrAnalog); /* PORTA27 (pin J10) is disabled */
  PORT_SetPinMux(PORTA, PIN28_IDX, kPORT_PinDisabledOrAnalog); /* PORTA28 (pin H12) is disabled */
  PORT_SetPinMux(PORTA, PIN29_IDX, kPORT_PinDisabledOrAnalog); /* PORTA29 (pin H11) is disabled */
  PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTA5 (pin M8) is disabled */
  PORT_SetPinMux(PORTA, PIN9_IDX, kPORT_PinDisabledOrAnalog); /* PORTA9 (pin L8) is disabled */
  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTB0 (pin H10) is configured as ADC0_SE8 */
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTB1 (pin H9) is configured as ADC0_SE9 */
  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_PinDisabledOrAnalog); /* PORTC16 (pin A6) is disabled */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTC17 (pin D5) is disabled */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTC18 (pin C5) is disabled */
  PORT_SetPinMux(PORTC, PIN19_IDX, kPORT_PinDisabledOrAnalog); /* PORTC19 (pin B5) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
