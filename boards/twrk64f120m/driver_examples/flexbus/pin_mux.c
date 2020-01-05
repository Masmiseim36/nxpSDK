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



#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
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
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
#define SOPT5_UART1TXSRC_UART_TX      0x00u   /*!< UART 1 transmit data source select: UART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A11, peripheral: UART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A9, peripheral: UART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT}
  - {pin_num: C4, peripheral: FB, signal: 'AD, 4', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL}
  - {pin_num: B4, peripheral: FB, signal: 'AD, 3', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA}
  - {pin_num: A4, peripheral: FB, signal: 'AD, 2', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/EWM_IN/SPI1_PCS0}
  - {pin_num: A3, peripheral: FB, signal: 'AD, 1', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/EWM_OUT_b/SPI1_SCK}
  - {pin_num: A2, peripheral: FB, signal: 'AD, 0', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: B12, peripheral: FB, signal: 'AD, 14', pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14/I2S0_TXD1}
  - {pin_num: B11, peripheral: FB, signal: 'AD, 13', pin_signal: ADC0_SE15/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0}
  - {pin_num: A12, peripheral: FB, signal: 'AD, 12', pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS}
  - {pin_num: D8, peripheral: FB, signal: 'AD, 10', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/CMP0_OUT/FTM0_CH2}
  - {pin_num: C8, peripheral: FB, signal: 'AD, 9', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK}
  - {pin_num: B8, peripheral: FB, signal: 'AD, 8', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8}
  - {pin_num: A8, peripheral: FB, signal: 'AD, 7', pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7}
  - {pin_num: D7, peripheral: FB, signal: 'AD, 6', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0}
  - {pin_num: C7, peripheral: FB, signal: 'AD, 5', pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5}
  - {pin_num: B7, peripheral: FB, signal: RW, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b}
  - {pin_num: A7, peripheral: FB, signal: 'AD, 27', pin_signal: PTC12/UART4_RTS_b/FB_AD27/FTM3_FLT0}
  - {pin_num: D6, peripheral: FB, signal: 'AD, 26', pin_signal: PTC13/UART4_CTS_b/FB_AD26}
  - {pin_num: C6, peripheral: FB, signal: 'AD, 25', pin_signal: PTC14/UART4_RX/FB_AD25}
  - {pin_num: B6, peripheral: FB, signal: 'AD, 24', pin_signal: PTC15/UART4_TX/FB_AD24}
  - {pin_num: D10, peripheral: FB, signal: 'AD, 31', pin_signal: PTB20/SPI2_PCS0/FB_AD31/CMP0_OUT}
  - {pin_num: D9, peripheral: FB, signal: 'AD, 30', pin_signal: PTB21/SPI2_SCK/FB_AD30/CMP1_OUT}
  - {pin_num: C12, peripheral: FB, signal: 'AD, 29', pin_signal: PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT}
  - {pin_num: C11, peripheral: FB, signal: 'AD, 28', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28}
  - {pin_num: E11, peripheral: FB, signal: 'AD, 18', pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/FTM0_FLT2}
  - {pin_num: E12, peripheral: FB, signal: 'AD, 19', pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/FTM0_FLT1}
  - {pin_num: E10, peripheral: FB, signal: 'AD, 17', pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN}
  - {pin_num: E9, peripheral: FB, signal: 'AD, 16', pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
  - {pin_num: D11, peripheral: FB, signal: OE, pin_signal: PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB}
  - {pin_num: D12, peripheral: FB, signal: 'AD, 15', pin_signal: PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA}
  - {pin_num: A5, peripheral: FB, signal: CS1_ALE_TS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b}
  - {pin_num: D4, peripheral: FB, signal: CS0, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAlt5);           /* PORTB10 (pin E12) is configured as FB_AD19 */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAlt5);           /* PORTB11 (pin E11) is configured as FB_AD18 */
  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt5);           /* PORTB16 (pin E10) is configured as FB_AD17 */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt5);           /* PORTB17 (pin E9) is configured as FB_AD16 */
  PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_MuxAlt5);           /* PORTB18 (pin D12) is configured as FB_AD15 */
  PORT_SetPinMux(PORTB, PIN19_IDX, kPORT_MuxAlt5);           /* PORTB19 (pin D11) is configured as FB_OE_b */
  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAlt5);           /* PORTB20 (pin D10) is configured as FB_AD31 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAlt5);           /* PORTB21 (pin D9) is configured as FB_AD30 */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAlt5);           /* PORTB22 (pin C12) is configured as FB_AD29 */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAlt5);           /* PORTB23 (pin C11) is configured as FB_AD28 */
  PORT_SetPinMux(PORTC, PIN0_IDX, kPORT_MuxAlt5);            /* PORTC0 (pin B12) is configured as FB_AD14 */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt5);            /* PORTC1 (pin B11) is configured as FB_AD13 */
  PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_MuxAlt5);           /* PORTC10 (pin C7) is configured as FB_AD5 */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAlt5);           /* PORTC11 (pin B7) is configured as FB_RW_b */
  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAlt5);           /* PORTC12 (pin A7) is configured as FB_AD27 */
  PORT_SetPinMux(PORTC, PIN13_IDX, kPORT_MuxAlt5);           /* PORTC13 (pin D6) is configured as FB_AD26 */
  PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt5);           /* PORTC14 (pin C6) is configured as FB_AD25 */
  PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt5);           /* PORTC15 (pin B6) is configured as FB_AD24 */
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt5);            /* PORTC2 (pin A12) is configured as FB_AD12 */
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);            /* PORTC3 (pin A11) is configured as UART1_RX */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);            /* PORTC4 (pin A9) is configured as UART1_TX */
  PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAlt5);            /* PORTC5 (pin D8) is configured as FB_AD10 */
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt5);            /* PORTC6 (pin C8) is configured as FB_AD9 */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt5);            /* PORTC7 (pin B8) is configured as FB_AD8 */
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt5);            /* PORTC8 (pin A8) is configured as FB_AD7 */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAlt5);            /* PORTC9 (pin D7) is configured as FB_AD6 */
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt5);            /* PORTD0 (pin A5) is configured as FB_CS1_b */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt5);            /* PORTD1 (pin D4) is configured as FB_CS0_b */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt5);            /* PORTD2 (pin C4) is configured as FB_AD4 */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt5);            /* PORTD3 (pin B4) is configured as FB_AD3 */
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt5);            /* PORTD4 (pin A4) is configured as FB_AD2 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt5);            /* PORTD5 (pin A3) is configured as FB_AD1 */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt5);            /* PORTD6 (pin A2) is configured as FB_AD0 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART1TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)       /* UART 1 transmit data source select: UART1_TX pin */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
