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
- !!product 'Pins v1.0'
- !!processor 'MK65FN2M0xxx18'
- !!package 'MK65FN2M0VMI18'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: F4, peripheral: I2S0, signal: TXD0, pin_signal: PTE10/LLWU_P18/I2C3_SDA/I2S0_TXD0/LPUART0_CTS_b/FTM3_CH5/USB1_ID}
  - {pin_num: G4, peripheral: I2S0, signal: TX_FS, pin_signal: PTE11/I2C3_SCL/I2S0_TX_FS/LPUART0_RTS_b/FTM3_CH6}
  - {pin_num: H4, peripheral: I2S0, signal: TX_BCLK, pin_signal: PTE12/I2S0_TX_BCLK/FTM3_CH7}
  - {pin_num: E2, peripheral: I2S0, signal: MCLK, pin_signal: PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/I2S0_MCLK/FTM3_CH1/USB0_SOF_OUT}
  - {pin_num: E3, peripheral: I2S0, signal: RXD0, pin_signal: PTE7/UART3_RTS_b/I2S0_RXD0/FTM3_CH2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN10_IDX, kPORT_MuxAlt4);           /* PORTE10 (pin F4) is configured as I2S0_TXD0 */
  PORT_SetPinMux(PORTE, PIN11_IDX, kPORT_MuxAlt4);           /* PORTE11 (pin G4) is configured as I2S0_TX_FS */
  PORT_SetPinMux(PORTE, PIN12_IDX, kPORT_MuxAlt4);           /* PORTE12 (pin H4) is configured as I2S0_TX_BCLK */
  PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt3);           /* PORTE16 (pin H3) is configured as UART2_TX */
  PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt3);           /* PORTE17 (pin F5) is configured as UART2_RX */
  PORT_SetPinMux(PORTE, PIN6_IDX, kPORT_MuxAlt4);            /* PORTE6 (pin E2) is configured as I2S0_MCLK */
  PORT_SetPinMux(PORTE, PIN7_IDX, kPORT_MuxAlt4);            /* PORTE7 (pin E3) is configured as I2S0_RXD0 */
}


#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: F6, peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE6a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: F7, peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE7a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT, open_drain: enable, pull_select: up, pull_enable: enable}
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

  PORT_SetPinMux(PORTE, PIN18_IDX, kPORT_MuxAlt4);           /* PORTE18 (pin F6) is configured as I2C0_SDA */
  PORTE->PCR[18] = ((PORTE->PCR[18] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_MuxAlt4);           /* PORTE19 (pin F7) is configured as I2C0_SCL */
  PORTE->PCR[19] = ((PORTE->PCR[19] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
