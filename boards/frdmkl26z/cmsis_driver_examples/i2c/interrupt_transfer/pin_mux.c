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


#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/TPM2_CH0, slew_rate: slow, pull_select: up, pull_enable: enable}
  - {pin_num: '38', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/TPM2_CH1, slew_rate: slow, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */

  const port_pin_config_t portb2_pin37_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SCL */
  };
  PORT_SetPinConfig(PORTB, PIN2_IDX, &portb2_pin37_config);  /* PORTB2 (pin 37) is configured as I2C0_SCL */
  const port_pin_config_t portb3_pin38_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SDA */
  };
  PORT_SetPinConfig(PORTB, PIN3_IDX, &portb3_pin38_config);  /* PORTB3 (pin 38) is configured as I2C0_SDA */
}


#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: ADC0, signal: 'SE, 12', pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/TPM2_CH0}
  - {pin_num: '38', peripheral: ADC0, signal: 'SE, 13', pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/TPM2_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C0_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTB2 (pin 37) is configured as ADC0_SE12 */
  PORT_SetPinMux(PORTB, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTB3 (pin 38) is configured as ADC0_SE13 */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: I2C1, signal: SDA, pin_signal: PTE0/SPI1_MISO/UART1_TX/RTC_CLKOUT/CMP0_OUT/I2C1_SDA, slew_rate: slow, pull_select: up, pull_enable: enable}
  - {pin_num: '2', peripheral: I2C1, signal: SCL, pin_signal: PTE1/SPI1_MOSI/UART1_RX/SPI1_MISO/I2C1_SCL, slew_rate: slow, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  const port_pin_config_t porte0_pin1_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt6,                                           /* Pin is configured as I2C1_SDA */
  };
  PORT_SetPinConfig(PORTE, PIN0_IDX, &porte0_pin1_config);   /* PORTE0 (pin 1) is configured as I2C1_SDA */
  const port_pin_config_t porte1_pin2_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt6,                                           /* Pin is configured as I2C1_SCL */
  };
  PORT_SetPinConfig(PORTE, PIN1_IDX, &porte1_pin2_config);   /* PORTE1 (pin 2) is configured as I2C1_SCL */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: n/a, signal: disabled, pin_signal: PTE0/SPI1_MISO/UART1_TX/RTC_CLKOUT/CMP0_OUT/I2C1_SDA}
  - {pin_num: '2', peripheral: n/a, signal: disabled, pin_signal: PTE1/SPI1_MOSI/UART1_RX/SPI1_MISO/I2C1_SCL}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C1_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTE0 (pin 1) is disabled */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTE1 (pin 2) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
