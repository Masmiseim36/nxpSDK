/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MK65FN2M0xxx18'
- !!package 'MK65FN2M0VMI18'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: C5, peripheral: UART0, signal: RX, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: K13, peripheral: SIM, signal: UART0_MOD_OUT, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt3);           /* PORTA14 (pin K13) is configured as UART0_TX */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt3);            /* PORTD6 (pin C5) is configured as UART0_RX */
}


#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: K13, peripheral: n/a, signal: disabled, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: C5, peripheral: ADC0, signal: 'SE, 7b', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART0_DeinitPins(void) {
  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_PinDisabledOrAnalog); /* PORTA14 (pin K13) is disabled */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTD6 (pin C5) is configured as ADC0_SE7b */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B1, peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN}
  - {pin_num: A1, peripheral: SIM, signal: UART1_MOD_OUT, pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT}
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

  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt3);            /* PORTE0 (pin A1) is configured as UART1_TX */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt3);            /* PORTE1 (pin B1) is configured as UART1_RX */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: A1, peripheral: ADC1, signal: 'SE, 4a', pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: B1, peripheral: ADC1, signal: 'SE, 5a', pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART1_DeinitPins(void) {
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTE0 (pin A1) is configured as ADC1_SE4a */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTE1 (pin B1) is configured as ADC1_SE5a */
}


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART2_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt3);           /* PORTE16 (pin H3) is configured as UART2_TX */
  PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt3);           /* PORTE17 (pin F5) is configured as UART2_RX */
}


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART2_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: H3, peripheral: ADC0, signal: 'SE, 4a', pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: ADC0, signal: 'SE, 5a', pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART2_DeinitPins(void) {
  PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_PinDisabledOrAnalog); /* PORTE16 (pin H3) is configured as ADC0_SE4a */
  PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTE17 (pin F5) is configured as ADC0_SE5a */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART3_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: D2, peripheral: UART3, signal: RX, pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0}
  - {pin_num: E1, peripheral: UART3, signal: TX, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART3_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART3_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_MuxAlt3);            /* PORTE4 (pin E1) is configured as UART3_TX */
  PORT_SetPinMux(PORTE, PIN5_IDX, kPORT_MuxAlt3);            /* PORTE5 (pin D2) is configured as UART3_RX */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART3_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: D2, peripheral: n/a, signal: disabled, pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0}
  - {pin_num: E1, peripheral: n/a, signal: disabled, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART3_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART3_DeinitPins(void) {
  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTE4 (pin E1) is disabled */
  PORT_SetPinMux(PORTE, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTE5 (pin D2) is disabled */
}


#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART4_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: K7, peripheral: UART4, signal: RX, pin_signal: ADC0_SE18/PTE25/LLWU_P21/CAN1_RX/UART4_RX/I2C0_SDA/EWM_IN}
  - {pin_num: L7, peripheral: UART4, signal: TX, pin_signal: ADC0_SE17/PTE24/CAN1_TX/UART4_TX/I2C0_SCL/EWM_OUT_b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART4_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART4_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN24_IDX, kPORT_MuxAlt3);           /* PORTE24 (pin L7) is configured as UART4_TX */
  PORT_SetPinMux(PORTE, PIN25_IDX, kPORT_MuxAlt3);           /* PORTE25 (pin K7) is configured as UART4_RX */
}


#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART4_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: L7, peripheral: ADC0, signal: 'SE, 17', pin_signal: ADC0_SE17/PTE24/CAN1_TX/UART4_TX/I2C0_SCL/EWM_OUT_b}
  - {pin_num: K7, peripheral: ADC0, signal: 'SE, 18', pin_signal: ADC0_SE18/PTE25/LLWU_P21/CAN1_RX/UART4_RX/I2C0_SDA/EWM_IN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART4_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART4_DeinitPins(void) {
  PORT_SetPinMux(PORTE, PIN24_IDX, kPORT_PinDisabledOrAnalog); /* PORTE24 (pin L7) is configured as ADC0_SE17 */
  PORT_SetPinMux(PORTE, PIN25_IDX, kPORT_PinDisabledOrAnalog); /* PORTE25 (pin K7) is configured as ADC0_SE18 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
