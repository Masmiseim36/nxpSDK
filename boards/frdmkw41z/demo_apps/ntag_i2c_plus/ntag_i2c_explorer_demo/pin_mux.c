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
- !!processor 'MKW41Z512xxx4'
- !!package 'MKW41Z512VHT4'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.0.0'
- !!board 'FRDM-KW41Z'
- pin_labels:
  - {pin_num: '40', pin_signal: TSI0_CH0/PTC4/LLWU_P12/ANT_A/EXTRG_IN/UART0_CTS_b/TPM1_CH0/BSM_DATA, label: SW3, identifier: SW3}
  - {pin_num: '41', pin_signal: TSI0_CH1/PTC5/LLWU_P13/RF_NOT_ALLOWED/LPTMR0_ALT2/UART0_RTS_b/TPM1_CH1/BSM_CLK, label: SW4, identifier: SW4}
  - {pin_num: '37', pin_signal: PTC1/ANT_B/I2C0_SDA/UART0_RTS_b/TPM0_CH2/BLE_RF_ACTIVE, label: LEDRGB_RED, identifier: LED_RED}
  - {pin_num: '6', pin_signal: TSI0_CH12/PTA18/LLWU_P6/SPI1_SCK/TPM2_CH0, label: LEDRGB_BLUE, identifier: LED_BLUE}
  - {pin_num: '7', pin_signal: TSI0_CH13/ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/TPM2_CH1, label: LEDRGB_GREEN, identifier: LED_GREEN}
  - {pin_num: '42', pin_signal: TSI0_CH2/PTC6/LLWU_P14/XTAL_OUT_EN/I2C1_SCL/UART0_RX/TPM2_CH0/BSM_FRAME, label: UART0_RX, identifier: DEBUG_UART_RX}
  - {pin_num: '43', pin_signal: TSI0_CH3/PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/UART0_TX/TPM2_CH1/BSM_DATA, label: UART0_TX, identifier: DEBUG_UART_TX}
  - {pin_num: '38', pin_signal: TSI0_CH14/PTC2/LLWU_P10/TX_SWITCH/I2C1_SCL/UART0_RX/CMT_IRO/DTM_RX, label: I2C1_SCL}
  - {pin_num: '39', pin_signal: TSI0_CH15/PTC3/LLWU_P11/RX_SWITCH/I2C1_SDA/UART0_TX/TPM0_CH1/DTM_TX, label: I2C1_SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '46', peripheral: GPIOC, signal: 'GPIO, 17', pin_signal: TSI0_CH5/PTC17/LLWU_P1/SPI0_SOUT/I2C1_SCL/UART0_RX/BSM_FRAME/DTM_RX, direction: INPUT, slew_rate: fast,
    drive_strength: low, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t portc17_pin46_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC17 */
  };
  PORT_SetPinConfig(PORTC, PIN17_IDX, &portc17_pin46_config); /* PORTC17 (pin 46) is configured as PTC17 */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitButtons:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '40', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: TSI0_CH0/PTC4/LLWU_P12/ANT_A/EXTRG_IN/UART0_CTS_b/TPM1_CH0/BSM_DATA, direction: INPUT, slew_rate: fast,
    pull_select: up, pull_enable: enable}
  - {pin_num: '41', peripheral: GPIOC, signal: 'GPIO, 5', pin_signal: TSI0_CH1/PTC5/LLWU_P13/RF_NOT_ALLOWED/LPTMR0_ALT2/UART0_RTS_b/TPM1_CH1/BSM_CLK, direction: INPUT,
    slew_rate: fast, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitButtons
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitButtons(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t portc4_pin40_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC4 */
  };
  PORT_SetPinConfig(PORTC, PIN4_IDX, &portc4_pin40_config);  /* PORTC4 (pin 40) is configured as PTC4 */
  const port_pin_config_t portc5_pin41_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC5 */
  };
  PORT_SetPinConfig(PORTC, PIN5_IDX, &portc5_pin41_config);  /* PORTC5 (pin 41) is configured as PTC5 */
}


#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitLEDs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: GPIOC, signal: 'GPIO, 1', pin_signal: PTC1/ANT_B/I2C0_SDA/UART0_RTS_b/TPM0_CH2/BLE_RF_ACTIVE, direction: OUTPUT, slew_rate: slow,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '6', peripheral: GPIOA, signal: 'GPIO, 18', pin_signal: TSI0_CH12/PTA18/LLWU_P6/SPI1_SCK/TPM2_CH0, direction: OUTPUT, slew_rate: slow, pull_select: down,
    pull_enable: disable}
  - {pin_num: '7', peripheral: GPIOA, signal: 'GPIO, 19', pin_signal: TSI0_CH13/ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/TPM2_CH1, direction: OUTPUT, slew_rate: slow, pull_select: down,
    pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitLEDs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitLEDs(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t porta18_pin6_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA18 */
  };
  PORT_SetPinConfig(PORTA, PIN18_IDX, &porta18_pin6_config); /* PORTA18 (pin 6) is configured as PTA18 */
  const port_pin_config_t porta19_pin7_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA19 */
  };
  PORT_SetPinConfig(PORTA, PIN19_IDX, &porta19_pin7_config); /* PORTA19 (pin 7) is configured as PTA19 */
  const port_pin_config_t portc1_pin37_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAsGpio,                                         /* Pin is configured as PTC1 */
  };
  PORT_SetPinConfig(PORTC, PIN1_IDX, &portc1_pin37_config);  /* PORTC1 (pin 37) is configured as PTC1 */
}


#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define SOPT5_LPUART0ODE_DISABLED     0x00u   /*!< LPUART0 Open Drain Enable: Open drain is disabled on LPUART0. */
#define SOPT5_LPUART0RXSRC_LPUART_RX  0x00u   /*!< LPUART0 Receive Data Source Select: LPUART_RX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitDEBUG_UART:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/UART0_TX/TPM2_CH1/BSM_DATA, direction: OUTPUT, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTC6/LLWU_P14/XTAL_OUT_EN/I2C1_SCL/UART0_RX/TPM2_CH0/BSM_FRAME, direction: INPUT, slew_rate: fast,
    drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitDEBUG_UART(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t portc6_pin42_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt4,                                           /* Pin is configured as UART0_RX */
  };
  PORT_SetPinConfig(PORTC, PIN6_IDX, &portc6_pin42_config);  /* PORTC6 (pin 42) is configured as UART0_RX */
  const port_pin_config_t portc7_pin43_config = {
    kPORT_PullDisable,                                       /* Internal pull-up/down resistor is disabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt4,                                           /* Pin is configured as UART0_TX */
  };
  PORT_SetPinConfig(PORTC, PIN7_IDX, &portc7_pin43_config);  /* PORTC7 (pin 43) is configured as UART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART0RXSRC_MASK | SIM_SOPT5_LPUART0ODE_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 Receive Data Source Select: LPUART_RX pin */
      | SIM_SOPT5_LPUART0ODE(SOPT5_LPUART0ODE_DISABLED)      /* LPUART0 Open Drain Enable: Open drain is disabled on LPUART0. */
    );
}


#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '38', peripheral: I2C1, signal: CLK, pin_signal: TSI0_CH14/PTC2/LLWU_P10/TX_SWITCH/I2C1_SCL/UART0_RX/CMT_IRO/DTM_RX, slew_rate: fast, drive_strength: low,
    pull_select: up, pull_enable: enable}
  - {pin_num: '39', peripheral: I2C1, signal: SDA, pin_signal: TSI0_CH15/PTC3/LLWU_P11/RX_SWITCH/I2C1_SDA/UART0_TX/TPM0_CH1/DTM_TX, slew_rate: fast, drive_strength: low,
    pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t portc2_pin38_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as I2C1_SCL */
  };
  PORT_SetPinConfig(PORTC, PIN2_IDX, &portc2_pin38_config);  /* PORTC2 (pin 38) is configured as I2C1_SCL */
  const port_pin_config_t portc3_pin39_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt3,                                           /* Pin is configured as I2C1_SDA */
  };
  PORT_SetPinConfig(PORTC, PIN3_IDX, &portc3_pin39_config);  /* PORTC3 (pin 39) is configured as I2C1_SDA */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
