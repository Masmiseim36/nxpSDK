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
- !!processor 'MKV10Z32xxx7'
- !!package 'MKV10Z32VLF7'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.1.0'
- !!board 'FRDM-KV10Z'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART 0 Receive Data Source Select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 Transmit Data Source Select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '31', peripheral: UART0, signal: RX, pin_signal: PTB16/UART0_RX/FTM_CLKIN2/EWM_IN}
  - {pin_num: '32', peripheral: UART0, signal: TX, pin_signal: PTB17/UART0_TX/FTM_CLKIN1/EWM_OUT_b}
  - {pin_num: '39', peripheral: SPI0, signal: SOUT, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/UART0_RX/I2C0_SCL}
  - {pin_num: '40', peripheral: SPI0, signal: SIN, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/UART0_TX/I2C0_SDA}
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

  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 31) is configured as UART0_RX */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 32) is configured as UART0_TX */
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt2);            /* PORTC6 (pin 39) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt2);            /* PORTC7 (pin 40) is configured as SPI0_SIN */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 Transmit Data Source Select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART 0 Receive Data Source Select: UART0_RX pin */
    );
}


#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '39', peripheral: I2C0, signal: SCL, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/UART0_RX/I2C0_SCL, pull_select: up, pull_enable: enable,
    drive_strength: low, slew_rate: fast, passive_filter: disable}
  - {pin_num: '40', peripheral: I2C0, signal: SDA, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/UART0_TX/I2C0_SDA, pull_select: up, pull_enable: enable, drive_strength: low,
    slew_rate: fast, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 *
 *END**************************************************************************/
void BOARD_I2C_ConfigurePins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  const port_pin_config_t portc6_pin39_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt7,                                           /* Pin is configured as I2C0_SCL */
  };
  PORT_SetPinConfig(PORTC, PIN6_IDX, &portc6_pin39_config);  /* PORTC6 (pin 39) is configured as I2C0_SCL */
  const port_pin_config_t portc7_pin40_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt7,                                           /* Pin is configured as I2C0_SDA */
  };
  PORT_SetPinConfig(PORTC, PIN7_IDX, &portc7_pin40_config);  /* PORTC7 (pin 40) is configured as I2C0_SDA */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
