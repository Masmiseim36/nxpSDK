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
- !!processor 'MKV10Z32xxx7'
- !!package 'MKV10Z32VLF7'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART 0 Receive Data Source Select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 Transmit Data Source Select: UART0_TX pin */
#define SOPT8_FTM0OCH0SRC_NO_MODULATION 0x00u /*!< FTM0 channel 0 output PWM/OCMP mode source select: FTM0CH0 pin is the output of FTM0 channel 0 PWM/OCMP */
#define SOPT8_FTM0OCH1SRC_NO_MODULATION 0x00u /*!< FTM0 channel 1 output PWM/OCMP mode source select: FTM0CH1 pin is the output of FTM0 channel 1 PWM/OCMP */
#define SOPT8_FTM0OCH2SRC_NO_MODULATION 0x00u /*!< FTM0 channel 2 output PWM/OCMP mode source select: FTM0CH2 pin is the output of FTM0 channel 2 PWM/OCMP */
#define SOPT8_FTM0OCH3SRC_NO_MODULATION 0x00u /*!< FTM0 channel 3 output PWM/OCMP mode source select: FTM0CH3 pin is the output of FTM0 channel 3 PWM/OCMP */
#define SOPT8_FTM0OCH4SRC_NO_MODULATION 0x00u /*!< FTM0 channel 4 output PWM/OCMP mode source select: FTM0CH4 pin is the output of FTM0 channel 4 PWM/OCMP */
#define SOPT8_FTM0OCH5SRC_NO_MODULATION 0x00u /*!< FTM0 channel 5 output PWM/OCMP mode source select: FTM0CH5 pin is the output of FTM0 channel 5 PWM/OCMP */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '31', peripheral: UART0, signal: RX, pin_signal: PTB16/UART0_RX/FTM_CLKIN2/EWM_IN}
  - {pin_num: '32', peripheral: UART0, signal: TX, pin_signal: PTB17/UART0_TX/FTM_CLKIN1/EWM_OUT_b}
  - {pin_num: '47', peripheral: FTM0, signal: 'CH, 0', pin_signal: ADC1_SE6/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH0/FTM1_CH0/FTM0_FLT0, direction: OUTPUT}
  - {pin_num: '45', peripheral: FTM0, signal: 'CH, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FTM2_CH0/EWM_IN, direction: OUTPUT}
  - {pin_num: '46', peripheral: FTM0, signal: 'CH, 5', pin_signal: ADC0_SE3/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FTM2_CH1/EWM_OUT_b, direction: OUTPUT}
  - {pin_num: '16', peripheral: FTM0, signal: 'CH, 1', pin_signal: PTE25/FTM0_CH1/I2C0_SDA/EWM_IN, direction: OUTPUT}
  - {pin_num: '13', peripheral: FTM0, signal: 'CH, 2', pin_signal: CMP1_IN5/CMP0_IN5/PTE29/FTM0_CH2/FTM_CLKIN0, direction: OUTPUT}
  - {pin_num: '37', peripheral: FTM0, signal: 'CH, 3', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/SS_b/UART1_TX/FTM0_CH3/CMP1_OUT, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
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
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 31) is configured as UART0_RX */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 32) is configured as UART0_TX */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt4);            /* PORTC4 (pin 37) is configured as FTM0_CH3 */
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt4);            /* PORTD4 (pin 45) is configured as FTM0_CH4 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt4);            /* PORTD5 (pin 46) is configured as FTM0_CH5 */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt4);            /* PORTD6 (pin 47) is configured as FTM0_CH0 */
  PORT_SetPinMux(PORTE, PIN25_IDX, kPORT_MuxAlt3);           /* PORTE25 (pin 16) is configured as FTM0_CH1 */
  PORT_SetPinMux(PORTE, PIN29_IDX, kPORT_MuxAlt3);           /* PORTE29 (pin 13) is configured as FTM0_CH2 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 Transmit Data Source Select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART 0 Receive Data Source Select: UART0_RX pin */
    );
  SIM->SOPT8 = ((SIM->SOPT8 &
    (~(SIM_SOPT8_FTM0OCH0SRC_MASK | SIM_SOPT8_FTM0OCH1SRC_MASK | SIM_SOPT8_FTM0OCH2SRC_MASK | SIM_SOPT8_FTM0OCH3SRC_MASK | SIM_SOPT8_FTM0OCH4SRC_MASK | SIM_SOPT8_FTM0OCH5SRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT8_FTM0OCH0SRC(SOPT8_FTM0OCH0SRC_NO_MODULATION) /* FTM0 channel 0 output PWM/OCMP mode source select: FTM0CH0 pin is the output of FTM0 channel 0 PWM/OCMP */
      | SIM_SOPT8_FTM0OCH1SRC(SOPT8_FTM0OCH1SRC_NO_MODULATION) /* FTM0 channel 1 output PWM/OCMP mode source select: FTM0CH1 pin is the output of FTM0 channel 1 PWM/OCMP */
      | SIM_SOPT8_FTM0OCH2SRC(SOPT8_FTM0OCH2SRC_NO_MODULATION) /* FTM0 channel 2 output PWM/OCMP mode source select: FTM0CH2 pin is the output of FTM0 channel 2 PWM/OCMP */
      | SIM_SOPT8_FTM0OCH3SRC(SOPT8_FTM0OCH3SRC_NO_MODULATION) /* FTM0 channel 3 output PWM/OCMP mode source select: FTM0CH3 pin is the output of FTM0 channel 3 PWM/OCMP */
      | SIM_SOPT8_FTM0OCH4SRC(SOPT8_FTM0OCH4SRC_NO_MODULATION) /* FTM0 channel 4 output PWM/OCMP mode source select: FTM0CH4 pin is the output of FTM0 channel 4 PWM/OCMP */
      | SIM_SOPT8_FTM0OCH5SRC(SOPT8_FTM0OCH5SRC_NO_MODULATION) /* FTM0 channel 5 output PWM/OCMP mode source select: FTM0CH5 pin is the output of FTM0 channel 5 PWM/OCMP */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
