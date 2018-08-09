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
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define SOPT4_TPM1CH0SRC_TPM          0x00u   /*!< TPM1 Channel 0 Input Capture Source Select: TPM1_CH0 signal */
#define SOPT5_LPUART0RXSRC_LPUART_RX  0x00u   /*!< LPUART0 Receive Data Source Select: LPUART_RX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTC6/LLWU_P14/XTAL_OUT_EN/I2C1_SCL/UART0_RX/TPM2_CH0/BSM_FRAME}
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/UART0_TX/TPM2_CH1/BSM_DATA}
  - {pin_num: '40', peripheral: TPM1, signal: 'CH, 0', pin_signal: TSI0_CH0/PTC4/LLWU_P12/ANT_A/EXTRG_IN/UART0_CTS_b/TPM1_CH0/BSM_DATA, slew_rate: slow, pull_select: up,
    pull_enable: enable}
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

  const port_pin_config_t portc4_pin40_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt5,                                           /* Pin is configured as TPM1_CH0 */
  };
  PORT_SetPinConfig(PORTC, PIN4_IDX, &portc4_pin40_config);  /* PORTC4 (pin 40) is configured as TPM1_CH0 */
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt4);            /* PORTC6 (pin 42) is configured as UART0_RX */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt4);            /* PORTC7 (pin 43) is configured as UART0_TX */
  SIM->SOPT4 = ((SIM->SOPT4 &
    (~(SIM_SOPT4_TPM1CH0SRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT4_TPM1CH0SRC(SOPT4_TPM1CH0SRC_TPM)           /* TPM1 Channel 0 Input Capture Source Select: TPM1_CH0 signal */
    );
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART0RXSRC_MASK)))                        /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 Receive Data Source Select: LPUART_RX pin */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
