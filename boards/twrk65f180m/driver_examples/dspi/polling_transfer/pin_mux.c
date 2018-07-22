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
- !!product 'Pins v1.0'
- !!processor 'MK65FN2M0xxx18'
- !!package 'MK65FN2M0VMI18'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: C2, peripheral: SPI2, signal: PCS0_SS, pin_signal: PTD11/LLWU_P25/SPI2_PCS0/SDHC0_CLKIN/LPUART0_CTS_b/FB_A19}
  - {pin_num: B2, peripheral: SPI2, signal: SCK, pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/SDHC0_D4/FB_A20}
  - {pin_num: B3, peripheral: SPI2, signal: SOUT, pin_signal: PTD13/SPI2_SOUT/SDHC0_D5/FB_A21}
  - {pin_num: A2, peripheral: SPI2, signal: SIN, pin_signal: PTD14/SPI2_SIN/SDHC0_D6/FB_A22}
  - {pin_num: A3, peripheral: SPI2, signal: PCS1, pin_signal: PTD15/SPI2_PCS1/SDHC0_D7/FB_A23}
  - {pin_num: B1, peripheral: SPI1, signal: SOUT, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN}
  - {pin_num: C1, peripheral: SPI1, signal: SCK, pin_signal: ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2}
  - {pin_num: D1, peripheral: SPI1, signal: SIN, pin_signal: ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT}
  - {pin_num: E1, peripheral: SPI1, signal: PCS0_SS, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN11_IDX, kPORT_MuxAlt2);           /* PORTD11 (pin C2) is configured as SPI2_PCS0 */
  PORT_SetPinMux(PORTD, PIN12_IDX, kPORT_MuxAlt2);           /* PORTD12 (pin B2) is configured as SPI2_SCK */
  PORT_SetPinMux(PORTD, PIN13_IDX, kPORT_MuxAlt2);           /* PORTD13 (pin B3) is configured as SPI2_SOUT */
  PORT_SetPinMux(PORTD, PIN14_IDX, kPORT_MuxAlt2);           /* PORTD14 (pin A2) is configured as SPI2_SIN */
  PORT_SetPinMux(PORTD, PIN15_IDX, kPORT_MuxAlt2);           /* PORTD15 (pin A3) is configured as SPI2_PCS1 */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt2);            /* PORTE1 (pin B1) is configured as SPI1_SOUT */
  PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt3);           /* PORTE16 (pin H3) is configured as UART2_TX */
  PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt3);           /* PORTE17 (pin F5) is configured as UART2_RX */
  PORT_SetPinMux(PORTE, PIN2_IDX, kPORT_MuxAlt2);            /* PORTE2 (pin C1) is configured as SPI1_SCK */
  PORT_SetPinMux(PORTE, PIN3_IDX, kPORT_MuxAlt2);            /* PORTE3 (pin D1) is configured as SPI1_SIN */
  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_MuxAlt2);            /* PORTE4 (pin E1) is configured as SPI1_PCS0 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
