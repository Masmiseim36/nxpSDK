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
- !!processor 'MKL25Z128xxx4'
- !!package 'MKL25Z128VLK4'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.1.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART0 transmit data source select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '27', peripheral: UART0, signal: RX, pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0}
  - {pin_num: '28', peripheral: UART0, signal: TX, pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
  - {pin_num: '53', peripheral: GPIOB, signal: 'GPIO, 18', pin_signal: TSI0_CH11/PTB18/TPM2_CH0, direction: OUTPUT}
  - {pin_num: '54', peripheral: GPIOB, signal: 'GPIO, 19', pin_signal: TSI0_CH12/PTB19/TPM2_CH1, direction: OUTPUT}
  - {pin_num: '74', peripheral: GPIOD, signal: 'GPIO, 1', pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/TPM0_CH1, direction: OUTPUT}
  - {pin_num: '37', peripheral: GPIOA, signal: 'GPIO, 17', pin_signal: PTA17/SPI0_MISO/SPI0_MOSI, direction: INPUT}
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
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);            /* PORTA1 (pin 27) is configured as UART0_RX */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAsGpio);         /* PORTA17 (pin 37) is configured as PTA17 */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt2);            /* PORTA2 (pin 28) is configured as UART0_TX */
  PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_MuxAsGpio);         /* PORTB18 (pin 53) is configured as PTB18 */
  PORT_SetPinMux(PORTB, PIN19_IDX, kPORT_MuxAsGpio);         /* PORTB19 (pin 54) is configured as PTB19 */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAsGpio);          /* PORTD1 (pin 74) is configured as PTD1 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART0 transmit data source select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART0 receive data source select: UART0_RX pin */
    );
}


#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '24', peripheral: I2C0, signal: SCL, pin_signal: PTE24/TPM0_CH0/I2C0_SCL, pull_enable: enable}
  - {pin_num: '25', peripheral: I2C0, signal: SDA, pin_signal: PTE25/TPM0_CH1/I2C0_SDA, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_I2C_ConfigurePins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  const port_pin_config_t porte24_pin24_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SCL */
  };
  PORT_SetPinConfig(PORTE, PIN24_IDX, &porte24_pin24_config); /* PORTE24 (pin 24) is configured as I2C0_SCL */
  const port_pin_config_t porte25_pin25_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SDA */
  };
  PORT_SetPinConfig(PORTE, PIN25_IDX, &porte25_pin25_config); /* PORTE25 (pin 25) is configured as I2C0_SDA */
}


#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART0 transmit data source select: UART0_TX pin */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '27', peripheral: UART0, signal: RX, pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0}
  - {pin_num: '28', peripheral: UART0, signal: TX, pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
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

  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);            /* PORTA1 (pin 27) is configured as UART0_RX */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt2);            /* PORTA2 (pin 28) is configured as UART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART0 transmit data source select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART0 receive data source select: UART0_RX pin */
    );
}


#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
UART0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '27', peripheral: TSI0, signal: 'CH, 2', pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0}
  - {pin_num: '28', peripheral: TSI0, signal: 'CH, 3', pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void UART0_DeinitPins(void) {
  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTA1 (pin 27) is configured as TSI0_CH2 */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTA2 (pin 28) is configured as TSI0_CH3 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
