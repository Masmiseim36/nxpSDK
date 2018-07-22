/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MK80FN256xxx15'
- !!package 'MK80FN256VLQ15'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define SOPT5_LPUART1RXSRC_LPUART_RX  0x00u   /*!< LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX  0x00u   /*!< LPUART1 transmit data source select: LPUART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '106', peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: '109', peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
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

  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);            /* PORTC3 (pin 106) is configured as LPUART1_RX */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);            /* PORTC4 (pin 109) is configured as LPUART1_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
      | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
    );
}


#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '137', peripheral: I2C0, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: '138', peripheral: I2C0, signal: SDA, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25, open_drain: enable, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN8_IDX, kPORT_MuxAlt2);            /* PORTD8 (pin 137) is configured as I2C0_SCL */
  PORTD->PCR[8] = ((PORTD->PCR[8] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTD, PIN9_IDX, kPORT_MuxAlt2);            /* PORTD9 (pin 138) is configured as I2C0_SDA */
  PORTD->PCR[9] = ((PORTD->PCR[9] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
}


#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '137', peripheral: n/a, signal: disabled, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24}
  - {pin_num: '138', peripheral: n/a, signal: disabled, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C0_DeinitPins(void) {
  PORT_SetPinMux(PORTD, PIN8_IDX, kPORT_PinDisabledOrAnalog); /* PORTD8 (pin 137) is disabled */
  PORT_SetPinMux(PORTD, PIN9_IDX, kPORT_PinDisabledOrAnalog); /* PORTD9 (pin 138) is disabled */
}


#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '116', peripheral: I2C1, signal: SDA, pin_signal: PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b/FXIO0_D19, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '115', peripheral: I2C1, signal: SCL, pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, open_drain: enable, pull_select: up,
    pull_enable: enable}
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

  PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_MuxAlt2);           /* PORTC10 (pin 115) is configured as I2C1_SCL */
  PORTC->PCR[10] = ((PORTC->PCR[10] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAlt2);           /* PORTC11 (pin 116) is configured as I2C1_SDA */
  PORTC->PCR[11] = ((PORTC->PCR[11] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
}


#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '115', peripheral: n/a, signal: disabled, pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18}
  - {pin_num: '116', peripheral: n/a, signal: disabled, pin_signal: PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b/FXIO0_D19}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C1_DeinitPins(void) {
  PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_PinDisabledOrAnalog); /* PORTC10 (pin 115) is disabled */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTC11 (pin 116) is disabled */
}


#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '63', peripheral: I2C2, signal: SCL, pin_signal: PTA11/LLWU_P23/I2C2_SCL/FTM2_CH1/FXIO0_D17/FTM2_QD_PHB/TPM2_CH1}
  - {pin_num: '59', peripheral: I2C2, signal: SDA, pin_signal: ADC0_SE10/PTA7/I2C2_SDA/FTM0_CH4/EMVSIM1_IO/TRACE_D3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C2_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN11_IDX, kPORT_MuxAlt2);           /* PORTA11 (pin 63) is configured as I2C2_SCL */
  PORT_SetPinMux(PORTA, PIN7_IDX, kPORT_MuxAlt2);            /* PORTA7 (pin 59) is configured as I2C2_SDA */
}


#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C2_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '63', peripheral: n/a, signal: disabled, pin_signal: PTA11/LLWU_P23/I2C2_SCL/FTM2_CH1/FXIO0_D17/FTM2_QD_PHB/TPM2_CH1}
  - {pin_num: '59', peripheral: ADC0, signal: 'SE, 10', pin_signal: ADC0_SE10/PTA7/I2C2_SDA/FTM0_CH4/EMVSIM1_IO/TRACE_D3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C2_DeinitPins(void) {
  PORT_SetPinMux(PORTA, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTA11 (pin 63) is disabled */
  PORT_SetPinMux(PORTA, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTA7 (pin 59) is configured as ADC0_SE10 */
}


#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C3_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '14', peripheral: I2C3, signal: SCL, pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS}
  - {pin_num: '13', peripheral: I2C3, signal: SDA, pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C3_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C3_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN10_IDX, kPORT_MuxAlt2);           /* PORTE10 (pin 13) is configured as I2C3_SDA */
  PORT_SetPinMux(PORTE, PIN11_IDX, kPORT_MuxAlt2);           /* PORTE11 (pin 14) is configured as I2C3_SCL */
}


#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C3_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '13', peripheral: n/a, signal: disabled, pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7}
  - {pin_num: '14', peripheral: n/a, signal: disabled, pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C3_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C3_DeinitPins(void) {
  PORT_SetPinMux(PORTE, PIN10_IDX, kPORT_PinDisabledOrAnalog); /* PORTE10 (pin 13) is disabled */
  PORT_SetPinMux(PORTE, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTE11 (pin 14) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
