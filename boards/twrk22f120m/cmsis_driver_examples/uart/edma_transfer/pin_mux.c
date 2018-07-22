/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
- !!processor 'MK22FN512xxx12'
- !!package 'MK22FN512VDC12'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.1'
- !!board 'TWR-K22F120M'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
}


/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART0_InitPins:
- options: {prefix: BOARD_, coreID: singlecore, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART0_InitPins(void) {
}


/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART0_DeinitPins(void) {
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define SOPT5_UART1TXSRC_UART_TX      0x00u   /*!< UART 1 transmit data source select: UART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: E4, peripheral: UART1, signal: TX, pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: E3, peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt3);            /* PORTE0 (pin E4) is configured as UART1_TX */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt3);            /* PORTE1 (pin E3) is configured as UART1_RX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART1TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)       /* UART 1 transmit data source select: UART1_TX pin */
    );
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: E4, peripheral: ADC1, signal: 'SE, 4a', pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: E3, peripheral: ADC1, signal: 'SE, 5a', pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART1_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTE0 (pin E4) is configured as ADC1_SE4a */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTE1 (pin E3) is configured as ADC1_SE5a */
}


#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: C3, peripheral: UART2, signal: RX, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/LPUART0_RX/I2C0_SCL}
  - {pin_num: B3, peripheral: UART2, signal: TX, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/LPUART0_TX/I2C0_SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART2_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt3);            /* PORTD2 (pin C3) is configured as UART2_RX */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt3);            /* PORTD3 (pin B3) is configured as UART2_TX */
}


#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART2_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: C3, peripheral: n/a, signal: disabled, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/LPUART0_RX/I2C0_SCL}
  - {pin_num: B3, peripheral: n/a, signal: disabled, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/LPUART0_TX/I2C0_SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART2_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTD2 (pin C3) is disabled */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTD3 (pin B3) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
