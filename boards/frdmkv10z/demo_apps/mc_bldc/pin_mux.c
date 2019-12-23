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
- !!board 'FRDM-KV10Z'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */
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
  - {pin_num: '21', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: PTA4/LLWU_P3/FTM0_CH1/FTM0_FLT3/NMI_b, pull_select: up, pull_enable: enable, drive_strength: low,
    slew_rate: fast, passive_filter: enable}
  - {pin_num: '13', peripheral: GPIOE, signal: 'GPIO, 29', pin_signal: CMP1_IN5/CMP0_IN5/PTE29/FTM0_CH2/FTM_CLKIN0}
  - {pin_num: '47', peripheral: GPIOD, signal: 'GPIO, 6', pin_signal: ADC1_SE6/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH0/FTM1_CH0/FTM0_FLT0}
  - {pin_num: '34', peripheral: FTM0, signal: 'CH, 0', pin_signal: ADC1_SE3/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FTM2_CH0, direction: OUTPUT}
  - {pin_num: '35', peripheral: FTM0, signal: 'CH, 1', pin_signal: ADC0_SE11/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FTM2_CH1, direction: OUTPUT}
  - {pin_num: '36', peripheral: FTM0, signal: 'CH, 2', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT, direction: OUTPUT}
  - {pin_num: '37', peripheral: FTM0, signal: 'CH, 3', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/SS_b/UART1_TX/FTM0_CH3/CMP1_OUT, direction: OUTPUT}
  - {pin_num: '45', peripheral: FTM0, signal: 'CH, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FTM2_CH0/EWM_IN, direction: OUTPUT}
  - {pin_num: '46', peripheral: FTM0, signal: 'CH, 5', pin_signal: ADC0_SE3/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FTM2_CH1/EWM_OUT_b, direction: OUTPUT}
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
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  const port_pin_config_t porta4_pin21_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterEnable,                               /* Passive filter is enabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
  };
  PORT_SetPinConfig(PORTA, PIN4_IDX, &porta4_pin21_config);  /* PORTA4 (pin 21) is configured as PTA4 */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt4);            /* PORTC1 (pin 34) is configured as FTM0_CH0 */
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt4);            /* PORTC2 (pin 35) is configured as FTM0_CH1 */
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt4);            /* PORTC3 (pin 36) is configured as FTM0_CH2 */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt4);            /* PORTC4 (pin 37) is configured as FTM0_CH3 */
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt4);            /* PORTD4 (pin 45) is configured as FTM0_CH4 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt4);            /* PORTD5 (pin 46) is configured as FTM0_CH5 */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAsGpio);          /* PORTD6 (pin 47) is configured as PTD6 */
  PORT_SetPinMux(PORTE, PIN29_IDX, kPORT_MuxAsGpio);         /* PORTE29 (pin 13) is configured as PTE29 */
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
