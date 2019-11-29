/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MKW41Z512xxx4'
- !!package 'MKW41Z512VHT4'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.25'
- !!board 'FRDM-KW41Z'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitButtons:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '40', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: TSI0_CH0/PTC4/LLWU_P12/ANT_A/EXTRG_IN/UART0_CTS_b/TPM1_CH0/BSM_DATA}
  - {pin_num: '41', peripheral: GPIOC, signal: 'GPIO, 5', pin_signal: TSI0_CH1/PTC5/LLWU_P13/RF_NOT_ALLOWED/LPTMR0_ALT2/UART0_RTS_b/TPM1_CH1/BSM_CLK}
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

  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTC4 (pin 40) is configured as PTC4 */
  PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTC5 (pin 41) is configured as PTC5 */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitLEDs:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '16', peripheral: GPIOB, signal: 'GPIO, 0', pin_signal: PTB0/LLWU_P8/XTAL_OUT_EN/I2C0_SCL/CMP0_OUT/TPM0_CH1/CLKOUT}
  - {pin_num: '37', peripheral: GPIOC, signal: 'GPIO, 1', pin_signal: PTC1/ANT_B/I2C0_SDA/UART0_RTS_b/TPM0_CH2/BLE_RF_ACTIVE}
  - {pin_num: '7', peripheral: GPIOA, signal: 'GPIO, 19', pin_signal: TSI0_CH13/ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/TPM2_CH1}
  - {pin_num: '6', peripheral: GPIOA, signal: 'GPIO, 18', pin_signal: TSI0_CH12/PTA18/LLWU_P6/SPI1_SCK/TPM2_CH0}
  - {pin_num: '47', peripheral: GPIOC, signal: 'GPIO, 18', pin_signal: TSI0_CH6/PTC18/LLWU_P2/SPI0_SIN/I2C1_SDA/UART0_TX/BSM_DATA/DTM_TX}
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
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_MuxAsGpio);         /* PORTA18 (pin 6) is configured as PTA18 */
  PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_MuxAsGpio);         /* PORTA19 (pin 7) is configured as PTA19 */
  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAsGpio);          /* PORTB0 (pin 16) is configured as PTB0 */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAsGpio);          /* PORTC1 (pin 37) is configured as PTC1 */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAsGpio);         /* PORTC18 (pin 47) is configured as PTC18 */
}


#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define SOPT4_TPM2CH0SRC_TPM          0x00u   /*!< TPM2 Channel 0 Input Capture Source Select: TPM2_CH0 signal */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitRGB:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: TPM0, signal: 'CH, 2', pin_signal: PTC1/ANT_B/I2C0_SDA/UART0_RTS_b/TPM0_CH2/BLE_RF_ACTIVE}
  - {pin_num: '7', peripheral: TPM2, signal: 'CH, 1', pin_signal: TSI0_CH13/ADC0_SE5/PTA19/LLWU_P7/SPI1_PCS0/TPM2_CH1}
  - {pin_num: '6', peripheral: TPM2, signal: 'CH, 0', pin_signal: TSI0_CH12/PTA18/LLWU_P6/SPI1_SCK/TPM2_CH0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitRGB
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitRGB(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_MuxAlt5);           /* PORTA18 (pin 6) is configured as TPM2_CH0 */
  PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_MuxAlt5);           /* PORTA19 (pin 7) is configured as TPM2_CH1 */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt5);            /* PORTC1 (pin 37) is configured as TPM0_CH2 */
  SIM->SOPT4 = ((SIM->SOPT4 &
    (~(SIM_SOPT4_TPM2CH0SRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT4_TPM2CH0SRC(SOPT4_TPM2CH0SRC_TPM)           /* TPM2 Channel 0 Input Capture Source Select: TPM2_CH0 signal */
    );
}


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitSPI:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: TSI0_CH6/PTC18/LLWU_P2/SPI0_SIN/I2C1_SDA/UART0_TX/BSM_DATA/DTM_TX}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal: TSI0_CH5/PTC17/LLWU_P1/SPI0_SOUT/I2C1_SCL/UART0_RX/BSM_FRAME/DTM_RX}
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: TSI0_CH4/PTC16/LLWU_P0/SPI0_SCK/I2C0_SDA/UART0_RTS_b/TPM0_CH3}
  - {pin_num: '48', peripheral: SPI0, signal: PCS0_SS, pin_signal: TSI0_CH7/PTC19/LLWU_P3/SPI0_PCS0/I2C0_SCL/UART0_CTS_b/BSM_CLK/BLE_RF_ACTIVE}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitSPI
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitSPI(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_MuxAlt2);           /* PORTC16 (pin 45) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_MuxAlt2);           /* PORTC17 (pin 46) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAlt2);           /* PORTC18 (pin 47) is configured as SPI0_SIN */
  PORT_SetPinMux(PORTC, PIN19_IDX, kPORT_MuxAlt2);           /* PORTC19 (pin 48) is configured as SPI0_PCS0 */
}


#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define SOPT5_LPUART0RXSRC_LPUART_RX  0x00u   /*!< LPUART0 Receive Data Source Select: LPUART_RX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitLPUART:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTC6/LLWU_P14/XTAL_OUT_EN/I2C1_SCL/UART0_RX/TPM2_CH0/BSM_FRAME}
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/UART0_TX/TPM2_CH1/BSM_DATA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitLPUART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitLPUART(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt4);            /* PORTC6 (pin 42) is configured as UART0_RX */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt4);            /* PORTC7 (pin 43) is configured as UART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART0RXSRC_MASK)))                        /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 Receive Data Source Select: LPUART_RX pin */
    );
}


#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitI2C:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '16', peripheral: I2C0, signal: CLK, pin_signal: PTB0/LLWU_P8/XTAL_OUT_EN/I2C0_SCL/CMP0_OUT/TPM0_CH1/CLKOUT, slew_rate: no_init, pull_select: up, pull_enable: enable}
  - {pin_num: '17', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE1/CMP0_IN5/PTB1/DTM_RX/I2C0_SDA/LPTMR0_ALT1/TPM0_CH2/CMT_IRO, slew_rate: no_init, pull_select: up,
    pull_enable: enable}
  - {pin_num: '38', peripheral: I2C1, signal: CLK, pin_signal: TSI0_CH14/PTC2/LLWU_P10/TX_SWITCH/I2C1_SCL/UART0_RX/CMT_IRO/DTM_RX, slew_rate: no_init, pull_select: up,
    pull_enable: enable}
  - {pin_num: '39', peripheral: I2C1, signal: SDA, pin_signal: TSI0_CH15/PTC3/LLWU_P11/RX_SWITCH/I2C1_SDA/UART0_TX/TPM0_CH1/DTM_TX, slew_rate: no_init, pull_select: up,
    pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitI2C
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitI2C(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAlt3);            /* PORTB0 (pin 16) is configured as I2C0_SCL */
  PORTB->PCR[0] = ((PORTB->PCR[0] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAlt3);            /* PORTB1 (pin 17) is configured as I2C0_SDA */
  PORTB->PCR[1] = ((PORTB->PCR[1] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt3);            /* PORTC2 (pin 38) is configured as I2C1_SCL */
  PORTC->PCR[2] = ((PORTC->PCR[2] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);            /* PORTC3 (pin 39) is configured as I2C1_SDA */
  PORTC->PCR[3] = ((PORTC->PCR[3] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
