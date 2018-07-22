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
- !!processor 'MK65FN2M0xxx18'
- !!package 'MK65FN2M0VMI18'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
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

  PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt3);           /* PORTE16 (pin H3) is configured as UART2_TX */
  PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt3);           /* PORTE17 (pin F5) is configured as UART2_RX */
}


#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: F7, peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE7a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: F6, peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE6a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA, open_drain: enable, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C0_InitPins(void) {
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


#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: F6, peripheral: ADC0, signal: 'SE, 6a', pin_signal: ADC0_SE6a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA}
  - {pin_num: F7, peripheral: ADC0, signal: 'SE, 7a', pin_signal: ADC0_SE7a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C0_DeinitPins(void) {
  PORT_SetPinMux(PORTE, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTE18 (pin F6) is configured as ADC0_SE6a */
  PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_PinDisabledOrAnalog); /* PORTE19 (pin F7) is configured as ADC0_SE7a */
}


#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: A1, peripheral: I2C1, signal: SDA, pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: B1, peripheral: I2C1, signal: SCL, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, open_drain: enable, pull_select: up,
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
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt6);            /* PORTE0 (pin A1) is configured as I2C1_SDA */
  PORTE->PCR[0] = ((PORTE->PCR[0] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt6);            /* PORTE1 (pin B1) is configured as I2C1_SCL */
  PORTE->PCR[1] = ((PORTE->PCR[1] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: A1, peripheral: ADC1, signal: 'SE, 4a', pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: B1, peripheral: ADC1, signal: 'SE, 5a', pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C1_DeinitPins(void) {
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTE0 (pin A1) is configured as ADC1_SE4a */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTE1 (pin B1) is configured as ADC1_SE5a */
}


#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: L12, peripheral: I2C2, signal: SCL, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0, open_drain: enable,
    pull_select: up, pull_enable: enable}
  - {pin_num: L11, peripheral: I2C2, signal: SDA, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1,
    open_drain: enable, pull_select: up, pull_enable: enable}
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

  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAlt5);           /* PORTA12 (pin L12) is configured as I2C2_SCL */
  PORTA->PCR[12] = ((PORTA->PCR[12] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAlt5);           /* PORTA13 (pin L11) is configured as I2C2_SDA */
  PORTA->PCR[13] = ((PORTA->PCR[13] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
}


#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C2_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: L12, peripheral: CMP2, signal: 'IN, 0', pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: L11, peripheral: CMP2, signal: 'IN, 1', pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C2_DeinitPins(void) {
  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_PinDisabledOrAnalog); /* PORTA12 (pin L12) is configured as CMP2_IN0 */
  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_PinDisabledOrAnalog); /* PORTA13 (pin L11) is configured as CMP2_IN1 */
}


#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN28_IDX                       28u   /*!< Pin number for pin 28 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C3_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B6, peripheral: I2C3, signal: SCL, pin_signal: PTC29/I2C3_SCL/ENET0_1588_TMR3/FB_A0/SDRAM_D0, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: C6, peripheral: I2C3, signal: SDA, pin_signal: PTC28/I2C3_SDA/ENET0_1588_TMR2/FB_A1/SDRAM_D1, open_drain: enable, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C3_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C3_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN28_IDX, kPORT_MuxAlt2);           /* PORTC28 (pin C6) is configured as I2C3_SDA */
  PORTC->PCR[28] = ((PORTC->PCR[28] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTC, PIN29_IDX, kPORT_MuxAlt2);           /* PORTC29 (pin B6) is configured as I2C3_SCL */
  PORTC->PCR[29] = ((PORTC->PCR[29] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
}


#define PIN28_IDX                       28u   /*!< Pin number for pin 28 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
I2C3_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: C6, peripheral: n/a, signal: disabled, pin_signal: PTC28/I2C3_SDA/ENET0_1588_TMR2/FB_A1/SDRAM_D1}
  - {pin_num: B6, peripheral: n/a, signal: disabled, pin_signal: PTC29/I2C3_SCL/ENET0_1588_TMR3/FB_A0/SDRAM_D0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C3_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C3_DeinitPins(void) {
  PORT_SetPinMux(PORTC, PIN28_IDX, kPORT_PinDisabledOrAnalog); /* PORTC28 (pin C6) is disabled */
  PORT_SetPinMux(PORTC, PIN29_IDX, kPORT_PinDisabledOrAnalog); /* PORTC29 (pin B6) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
