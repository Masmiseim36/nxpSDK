/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MKW41Z512xxx4'
- !!package 'MKW41Z512VHT4'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.0.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define SOPT5_LPUART0RXSRC_LPUART_RX  0x00u   /*!< LPUART0 Receive Data Source Select: LPUART_RX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTC6/LLWU_P14/XTAL_OUT_EN/I2C1_SCL/UART0_RX/TPM2_CH0/BSM_FRAME}
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/UART0_TX/TPM2_CH1/BSM_DATA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt4);            /* PORTC6 (pin 42) is configured as UART0_RX */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt4);            /* PORTC7 (pin 43) is configured as UART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART0RXSRC_MASK)))                        /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 Receive Data Source Select: LPUART_RX pin */
    );
}


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: TSI0_CH4/PTC16/LLWU_P0/SPI0_SCK/I2C0_SDA/UART0_RTS_b/TPM0_CH3}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal: TSI0_CH5/PTC17/LLWU_P1/SPI0_SOUT/I2C1_SCL/UART0_RX/BSM_FRAME/DTM_RX}
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: TSI0_CH6/PTC18/LLWU_P2/SPI0_SIN/I2C1_SDA/UART0_TX/BSM_DATA/DTM_TX}
  - {pin_num: '48', peripheral: SPI0, signal: PCS0_SS, pin_signal: TSI0_CH7/PTC19/LLWU_P3/SPI0_PCS0/I2C0_SCL/UART0_CTS_b/BSM_CLK/BLE_RF_ACTIVE}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_MuxAlt2);           /* PORTC16 (pin 45) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_MuxAlt2);           /* PORTC17 (pin 46) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAlt2);           /* PORTC18 (pin 47) is configured as SPI0_SIN */
  PORT_SetPinMux(PORTC, PIN19_IDX, kPORT_MuxAlt2);           /* PORTC19 (pin 48) is configured as SPI0_PCS0 */
}


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '46', peripheral: TSI0, signal: 'CH, 5', pin_signal: TSI0_CH5/PTC17/LLWU_P1/SPI0_SOUT/I2C1_SCL/UART0_RX/BSM_FRAME/DTM_RX}
  - {pin_num: '47', peripheral: TSI0, signal: 'CH, 6', pin_signal: TSI0_CH6/PTC18/LLWU_P2/SPI0_SIN/I2C1_SDA/UART0_TX/BSM_DATA/DTM_TX}
  - {pin_num: '48', peripheral: TSI0, signal: 'CH, 7', pin_signal: TSI0_CH7/PTC19/LLWU_P3/SPI0_PCS0/I2C0_SCL/UART0_CTS_b/BSM_CLK/BLE_RF_ACTIVE}
  - {pin_num: '45', peripheral: TSI0, signal: 'CH, 4', pin_signal: TSI0_CH4/PTC16/LLWU_P0/SPI0_SCK/I2C0_SDA/UART0_RTS_b/TPM0_CH3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI0_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_PinDisabledOrAnalog); /* PORTC16 (pin 45) is configured as TSI0_CH4 */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTC17 (pin 46) is configured as TSI0_CH5 */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTC18 (pin 47) is configured as TSI0_CH6 */
  PORT_SetPinMux(PORTC, PIN19_IDX, kPORT_PinDisabledOrAnalog); /* PORTC19 (pin 48) is configured as TSI0_CH7 */
}


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '4', peripheral: SPI1, signal: SOUT, pin_signal: TSI0_CH10/PTA16/LLWU_P4/SPI1_SOUT/TPM0_CH0}
  - {pin_num: '5', peripheral: SPI1, signal: SIN, pin_signal: TSI0_CH11/PTA17/LLWU_P5/RF_RESET/SPI1_SIN/TPM_CLKIN1}
  - {pin_num: '6', peripheral: SPI1, signal: SCK, pin_signal: TSI0_CH12/PTA18/LLWU_P6/SPI1_SCK/TPM2_CH0}
  - {pin_num: '7', peripheral: SPI1, signal: PCS0_SS, pin_signal: TSI0_CH13/ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/TPM2_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt2);           /* PORTA16 (pin 4) is configured as SPI1_SOUT */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt2);           /* PORTA17 (pin 5) is configured as SPI1_SIN */
  PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_MuxAlt2);           /* PORTA18 (pin 6) is configured as SPI1_SCK */
  PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_MuxAlt2);           /* PORTA19 (pin 7) is configured as SPI1_PCS0 */
}


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '4', peripheral: TSI0, signal: 'CH, 10', pin_signal: TSI0_CH10/PTA16/LLWU_P4/SPI1_SOUT/TPM0_CH0}
  - {pin_num: '5', peripheral: TSI0, signal: 'CH, 11', pin_signal: TSI0_CH11/PTA17/LLWU_P5/RF_RESET/SPI1_SIN/TPM_CLKIN1}
  - {pin_num: '6', peripheral: TSI0, signal: 'CH, 12', pin_signal: TSI0_CH12/PTA18/LLWU_P6/SPI1_SCK/TPM2_CH0}
  - {pin_num: '7', peripheral: TSI0, signal: 'CH, 13', pin_signal: TSI0_CH13/ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/TPM2_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI1_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_PinDisabledOrAnalog); /* PORTA16 (pin 4) is configured as TSI0_CH10 */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTA17 (pin 5) is configured as TSI0_CH11 */
  PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTA18 (pin 6) is configured as TSI0_CH12 */
  PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_PinDisabledOrAnalog); /* PORTA19 (pin 7) is configured as TSI0_CH13 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
