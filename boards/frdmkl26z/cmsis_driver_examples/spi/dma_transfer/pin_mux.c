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
- !!processor 'MKL26Z128xxx4'
- !!package 'MKL26Z128VLH4'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.0'
- !!board 'FRDM-KL26Z'
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
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '23', peripheral: UART0, signal: RX, pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0}
  - {pin_num: '24', peripheral: UART0, signal: TX, pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
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

  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);            /* PORTA1 (pin 23) is configured as UART0_RX */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt2);            /* PORTA2 (pin 24) is configured as UART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART0 Transmit Data Source Select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART0 Receive Data Source Select: UART_RX pin */
    );
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
SPI0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '49', peripheral: SPI0, signal: PCS0, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/TPM0_CH3/I2S0_MCLK}
  - {pin_num: '50', peripheral: SPI0, signal: SCK, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/CMP0_OUT}
  - {pin_num: '51', peripheral: SPI0, signal: MOSI, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_MOSI/EXTRG_IN/I2S0_RX_BCLK/SPI0_MISO/I2S0_MCLK}
  - {pin_num: '52', peripheral: SPI0, signal: MISO, pin_signal: CMP0_IN1/PTC7/SPI0_MISO/USB_SOF_OUT/I2S0_RX_FS/SPI0_MOSI}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt2);            /* PORTC4 (pin 49) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAlt2);            /* PORTC5 (pin 50) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt2);            /* PORTC6 (pin 51) is configured as SPI0_MOSI */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt2);            /* PORTC7 (pin 52) is configured as SPI0_MISO */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
SPI0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '49', peripheral: n/a, signal: disabled, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/TPM0_CH3/I2S0_MCLK}
  - {pin_num: '50', peripheral: n/a, signal: disabled, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/CMP0_OUT}
  - {pin_num: '51', peripheral: CMP0, signal: 'IN, 0', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_MOSI/EXTRG_IN/I2S0_RX_BCLK/SPI0_MISO/I2S0_MCLK}
  - {pin_num: '52', peripheral: CMP0, signal: 'IN, 1', pin_signal: CMP0_IN1/PTC7/SPI0_MISO/USB_SOF_OUT/I2S0_RX_FS/SPI0_MOSI}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI0_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTC4 (pin 49) is disabled */
  PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTC5 (pin 50) is disabled */
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTC6 (pin 51) is configured as CMP0_IN0 */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTC7 (pin 52) is configured as CMP0_IN1 */
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
  - {pin_num: '61', peripheral: SPI1, signal: PCS0, pin_signal: PTD4/LLWU_P14/SPI1_PCS0/UART2_RX/TPM0_CH4}
  - {pin_num: '62', peripheral: SPI1, signal: SCK, pin_signal: ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5}
  - {pin_num: '63', peripheral: SPI1, signal: MOSI, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/UART0_RX/SPI1_MISO}
  - {pin_num: '64', peripheral: SPI1, signal: MISO, pin_signal: PTD7/SPI1_MISO/UART0_TX/SPI1_MOSI}
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

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt2);            /* PORTD4 (pin 61) is configured as SPI1_PCS0 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt2);            /* PORTD5 (pin 62) is configured as SPI1_SCK */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt2);            /* PORTD6 (pin 63) is configured as SPI1_MOSI */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAlt2);            /* PORTD7 (pin 64) is configured as SPI1_MISO */
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
  - {pin_num: '61', peripheral: n/a, signal: disabled, pin_signal: PTD4/LLWU_P14/SPI1_PCS0/UART2_RX/TPM0_CH4}
  - {pin_num: '62', peripheral: ADC0, signal: 'SE, 6b', pin_signal: ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5}
  - {pin_num: '63', peripheral: ADC0, signal: 'SE, 7b', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/UART0_RX/SPI1_MISO}
  - {pin_num: '64', peripheral: n/a, signal: disabled, pin_signal: PTD7/SPI1_MISO/UART0_TX/SPI1_MOSI}
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

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTD4 (pin 61) is disabled */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTD5 (pin 62) is configured as ADC0_SE6b */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTD6 (pin 63) is configured as ADC0_SE7b */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTD7 (pin 64) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
