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
- !!processor 'MK22FN512xxx12'
- !!package 'MK22FN512VDC12'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.1'
- !!board 'TWR-K22F120M'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define SOPT5_UART1TXSRC_UART_TX      0x00u   /*!< UART 1 transmit data source select: UART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {prefix: BOARD_, coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: E4, peripheral: UART1, signal: TX, pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: E3, peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN}
  - {pin_num: A7, peripheral: I2S0, signal: MCLK, pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7}
  - {pin_num: D8, peripheral: I2S0, signal: TXD0, pin_signal: ADC0_SE15/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0/LPUART0_RTS_b}
  - {pin_num: D9, peripheral: I2S0, signal: TX_BCLK, pin_signal: PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA}
  - {pin_num: C9, peripheral: I2S0, signal: TX_FS, pin_signal: PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB}
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
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_MuxAlt4);           /* PORTB18 (pin D9) is configured as I2S0_TX_BCLK */
  PORT_SetPinMux(PORTB, PIN19_IDX, kPORT_MuxAlt4);           /* PORTB19 (pin C9) is configured as I2S0_TX_FS */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt6);            /* PORTC1 (pin D8) is configured as I2S0_TXD0 */
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt4);            /* PORTC8 (pin A7) is configured as I2S0_MCLK */
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt3);            /* PORTE0 (pin E4) is configured as UART1_TX */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt3);            /* PORTE1 (pin E3) is configured as UART1_RX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART1TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)       /* UART 1 transmit data source select: UART1_TX pin */
    );
}


#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: H5, peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE17/PTE24/I2C0_SCL/EWM_OUT_b, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: J5, peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE18/PTE25/I2C0_SDA/EWM_IN, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
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

  const port_pin_config_t porte24_pinH5_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SCL */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTE, PIN24_IDX, &porte24_pinH5_config); /* PORTE24 (pin H5) is configured as I2C0_SCL */
  const port_pin_config_t porte25_pinJ5_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SDA */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTE, PIN25_IDX, &porte25_pinJ5_config); /* PORTE25 (pin J5) is configured as I2C0_SDA */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
