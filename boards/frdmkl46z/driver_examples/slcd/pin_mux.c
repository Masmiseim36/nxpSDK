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
- !!processor 'MKL46Z256xxx4'
- !!package 'MKL46Z256VLL4'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.1.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART0 Receive Data Source Select: UART_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART0 Transmit Data Source Select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: UART0, signal: RX, pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: UART0, signal: TX, pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
  - {pin_num: '93', peripheral: LCD, signal: 'P, 40', pin_signal: LCD_P40/PTD0/SPI0_PCS0/TPM0_CH0/LCD_P40_Fault}
  - {pin_num: '5', peripheral: LCD, signal: 'P, 52', pin_signal: LCD_P52/PTE4/SPI1_PCS0/LCD_P52_Fault}
  - {pin_num: '6', peripheral: LCD, signal: 'P, 53', pin_signal: LCD_P53/PTE5/LCD_P53_Fault}
  - {pin_num: '57', peripheral: LCD, signal: 'P, 7', pin_signal: LCD_P7/PTB7/LCD_P7_Fault}
  - {pin_num: '58', peripheral: LCD, signal: 'P, 8', pin_signal: LCD_P8/PTB8/SPI1_PCS0/EXTRG_IN/LCD_P8_Fault}
  - {pin_num: '60', peripheral: LCD, signal: 'P, 10', pin_signal: LCD_P10/PTB10/SPI1_PCS0/LCD_P10_Fault}
  - {pin_num: '61', peripheral: LCD, signal: 'P, 11', pin_signal: LCD_P11/PTB11/SPI1_SCK/LCD_P11_Fault}
  - {pin_num: '67', peripheral: LCD, signal: 'P, 17', pin_signal: LCD_P17/PTB21/LCD_P17_Fault}
  - {pin_num: '68', peripheral: LCD, signal: 'P, 18', pin_signal: LCD_P18/PTB22/LCD_P18_Fault}
  - {pin_num: '69', peripheral: LCD, signal: 'P, 19', pin_signal: LCD_P19/PTB23/LCD_P19_Fault}
  - {pin_num: '91', peripheral: LCD, signal: 'P, 37', pin_signal: LCD_P37/PTC17/LCD_P37_Fault}
  - {pin_num: '92', peripheral: LCD, signal: 'P, 38', pin_signal: LCD_P38/PTC18/LCD_P38_Fault}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);            /* PORTA1 (pin 35) is configured as UART0_RX */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt2);            /* PORTA2 (pin 36) is configured as UART0_TX */
  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_PinDisabledOrAnalog); /* PORTB10 (pin 60) is configured as LCD_P10 */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTB11 (pin 61) is configured as LCD_P11 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_PinDisabledOrAnalog); /* PORTB21 (pin 67) is configured as LCD_P17 */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_PinDisabledOrAnalog); /* PORTB22 (pin 68) is configured as LCD_P18 */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_PinDisabledOrAnalog); /* PORTB23 (pin 69) is configured as LCD_P19 */
  PORT_SetPinMux(PORTB, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTB7 (pin 57) is configured as LCD_P7 */
  PORT_SetPinMux(PORTB, PIN8_IDX, kPORT_PinDisabledOrAnalog); /* PORTB8 (pin 58) is configured as LCD_P8 */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTC17 (pin 91) is configured as LCD_P37 */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTC18 (pin 92) is configured as LCD_P38 */
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTD0 (pin 93) is configured as LCD_P40 */
  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTE4 (pin 5) is configured as LCD_P52 */
  PORT_SetPinMux(PORTE, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTE5 (pin 6) is configured as LCD_P53 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART0 Transmit Data Source Select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART0 Receive Data Source Select: UART_RX pin */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
