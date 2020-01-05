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
- !!board 'FRDM-KL46Z'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART0 Receive Data Source Select: UART_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART0 Transmit Data Source Select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {prefix: BOARD_, coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: UART0, signal: RX, pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: UART0, signal: TX, pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
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

  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);            /* PORTA1 (pin 35) is configured as UART0_RX */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt2);            /* PORTA2 (pin 36) is configured as UART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART0 Transmit Data Source Select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART0 Receive Data Source Select: UART_RX pin */
    );
}


#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
SPI0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '44', peripheral: SPI0, signal: PCS0, pin_signal: PTA14/SPI0_PCS0/UART0_TX/I2S0_RX_BCLK/I2S0_TXD0}
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: PTA15/SPI0_SCK/UART0_RX/I2S0_RXD0}
  - {pin_num: '46', peripheral: SPI0, signal: MOSI, pin_signal: PTA16/SPI0_MOSI/SPI0_MISO/I2S0_RX_FS/I2S0_RXD0}
  - {pin_num: '47', peripheral: SPI0, signal: MISO, pin_signal: PTA17/SPI0_MISO/SPI0_MOSI/I2S0_MCLK}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt2);           /* PORTA14 (pin 44) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt2);           /* PORTA15 (pin 45) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt2);           /* PORTA16 (pin 46) is configured as SPI0_MOSI */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt2);           /* PORTA17 (pin 47) is configured as SPI0_MISO */
}


#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
SPI0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '44', peripheral: n/a, signal: disabled, pin_signal: PTA14/SPI0_PCS0/UART0_TX/I2S0_RX_BCLK/I2S0_TXD0}
  - {pin_num: '45', peripheral: n/a, signal: disabled, pin_signal: PTA15/SPI0_SCK/UART0_RX/I2S0_RXD0}
  - {pin_num: '46', peripheral: n/a, signal: disabled, pin_signal: PTA16/SPI0_MOSI/SPI0_MISO/I2S0_RX_FS/I2S0_RXD0}
  - {pin_num: '47', peripheral: n/a, signal: disabled, pin_signal: PTA17/SPI0_MISO/SPI0_MOSI/I2S0_MCLK}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI0_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_PinDisabledOrAnalog); /* PORTA14 (pin 44) is disabled */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_PinDisabledOrAnalog); /* PORTA15 (pin 45) is disabled */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_PinDisabledOrAnalog); /* PORTA16 (pin 46) is disabled */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTA17 (pin 47) is disabled */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
SPI1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '97', peripheral: SPI1, signal: PCS0, pin_signal: LCD_P44/PTD4/LLWU_P14/SPI1_PCS0/UART2_RX/TPM0_CH4/LCD_P44_Fault}
  - {pin_num: '98', peripheral: SPI1, signal: SCK, pin_signal: LCD_P45/ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5/LCD_P45_Fault}
  - {pin_num: '99', peripheral: SPI1, signal: MOSI, pin_signal: LCD_P46/ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/UART0_RX/SPI1_MISO/LCD_P46_Fault}
  - {pin_num: '100', peripheral: SPI1, signal: MISO, pin_signal: LCD_P47/PTD7/SPI1_MISO/UART0_TX/SPI1_MOSI/LCD_P47_Fault}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt2);            /* PORTD4 (pin 97) is configured as SPI1_PCS0 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt2);            /* PORTD5 (pin 98) is configured as SPI1_SCK */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt2);            /* PORTD6 (pin 99) is configured as SPI1_MOSI */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAlt2);            /* PORTD7 (pin 100) is configured as SPI1_MISO */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
SPI1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '97', peripheral: LCD, signal: 'P, 44', pin_signal: LCD_P44/PTD4/LLWU_P14/SPI1_PCS0/UART2_RX/TPM0_CH4/LCD_P44_Fault}
  - {pin_num: '98', peripheral: ADC0, signal: 'SE, 6b', pin_signal: LCD_P45/ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5/LCD_P45_Fault}
  - {pin_num: '99', peripheral: ADC0, signal: 'SE, 7b', pin_signal: LCD_P46/ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/UART0_RX/SPI1_MISO/LCD_P46_Fault}
  - {pin_num: '100', peripheral: LCD, signal: 'P, 47', pin_signal: LCD_P47/PTD7/SPI1_MISO/UART0_TX/SPI1_MOSI/LCD_P47_Fault}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI1_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTD4 (pin 97) is configured as LCD_P44 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTD5 (pin 98) is configured as ADC0_SE6b */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTD6 (pin 99) is configured as ADC0_SE7b */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTD7 (pin 100) is configured as LCD_P47 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
