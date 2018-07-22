/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
- !!package 'MK80FN256VDC15'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN14_IDX 14u                      /*!< Pin number for pin 14 in a port */
#define PIN15_IDX 15u                      /*!< Pin number for pin 15 in a port */
#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!< LPUART0 receive data source select: LPUART0_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!< LPUART0 transmit data source select: LPUART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART0_InitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: K9, peripheral: LPUART0, signal: TX, pin_signal:
PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: L9, peripheral: LPUART0, signal: RX, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART0_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortA); /* Port A Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt3); /* PORTA14 (pin K9) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt3); /* PORTA15 (pin L9) is configured as LPUART0_RX */
    SIM->SOPT5 =
        ((SIM->SOPT5 &
          (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX) /* LPUART0 transmit data source select: LPUART0_TX pin */
         | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 receive data source select: LPUART0_RX pin */
         );
}

#define PIN14_IDX 14u /*!< Pin number for pin 14 in a port */
#define PIN15_IDX 15u /*!< Pin number for pin 15 in a port */
                      /*
                       * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                      LPUART0_DeinitPins:
                      - options: {coreID: singlecore, enableClock: 'false'}
                      - pin_list:
                        - {pin_num: L9, peripheral: n/a, signal: disabled, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
                        - {pin_num: K9, peripheral: n/a, signal: disabled, pin_signal:
                      PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
                       * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                       */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART0_DeinitPins(void)
{
    PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_PinDisabledOrAnalog); /* PORTA14 (pin K9) is disabled */
    PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_PinDisabledOrAnalog); /* PORTA15 (pin L9) is disabled */
}

#define PIN3_IDX 3u                        /*!< Pin number for pin 3 in a port */
#define PIN4_IDX 4u                        /*!< Pin number for pin 4 in a port */
#define SOPT5_LPUART1RXSRC_LPUART_RX 0x00u /*!< LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX 0x00u /*!< LPUART1 transmit data source select: LPUART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal:
CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal:
PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART1_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortC); /* Port C Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3); /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3); /* PORTC4 (pin A8) is configured as LPUART1_TX */
    SIM->SOPT5 =
        ((SIM->SOPT5 &
          (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
         | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
         );
}

#define PIN3_IDX 3u /*!< Pin number for pin 3 in a port */
#define PIN4_IDX 4u /*!< Pin number for pin 4 in a port */
                    /*
                     * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                    LPUART1_DeinitPins:
                    - options: {coreID: singlecore, enableClock: 'true'}
                    - pin_list:
                      - {pin_num: A8, peripheral: n/a, signal: disabled, pin_signal:
                    PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
                      - {pin_num: B8, peripheral: CMP1, signal: 'IN, 1', pin_signal:
                    CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
                     * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                     */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART1_DeinitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortC); /* Port C Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTC3 (pin B8) is configured as CMP1_IN1 */
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTC4 (pin A8) is disabled */
}

#define PIN2_IDX 2u /*!< Pin number for pin 2 in a port */
#define PIN3_IDX 3u /*!< Pin number for pin 3 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: C3, peripheral: LPUART2, signal: RX, pin_signal:
PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
  - {pin_num: B3, peripheral: LPUART2, signal: TX, pin_signal:
PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART2_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortD); /* Port D Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt3); /* PORTD2 (pin C3) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt3); /* PORTD3 (pin B3) is configured as LPUART2_TX */
}

#define PIN2_IDX 2u /*!< Pin number for pin 2 in a port */
#define PIN3_IDX 3u /*!< Pin number for pin 3 in a port */
                    /*
                     * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                    LPUART2_DeinitPins:
                    - options: {coreID: singlecore, enableClock: 'false'}
                    - pin_list:
                      - {pin_num: C3, peripheral: n/a, signal: disabled, pin_signal:
                    PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
                      - {pin_num: B3, peripheral: n/a, signal: disabled, pin_signal:
                    PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
                     * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                     */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART2_DeinitPins(void)
{
    PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTD2 (pin C3) is disabled */
    PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTD3 (pin B3) is disabled */
}

#define PIN10_IDX 10u /*!< Pin number for pin 10 in a port */
#define PIN11_IDX 11u /*!< Pin number for pin 11 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART3_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: D10, peripheral: LPUART3, signal: RX, pin_signal:
PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: C10, peripheral: LPUART3, signal: TX, pin_signal:
PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART3_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART3_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortB); /* Port B Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAlt3); /* PORTB10 (pin D10) is configured as LPUART3_RX */
    PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAlt3); /* PORTB11 (pin C10) is configured as LPUART3_TX */
}

#define PIN10_IDX 10u /*!< Pin number for pin 10 in a port */
#define PIN11_IDX 11u /*!< Pin number for pin 11 in a port */
                      /*
                       * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                      LPUART3_DeinitPins:
                      - options: {coreID: singlecore, enableClock: 'false'}
                      - pin_list:
                        - {pin_num: D10, peripheral: n/a, signal: disabled, pin_signal:
                      PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
                        - {pin_num: C10, peripheral: n/a, signal: disabled, pin_signal:
                      PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
                       * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                       */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART3_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART3_DeinitPins(void)
{
    PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_PinDisabledOrAnalog); /* PORTB10 (pin D10) is disabled */
    PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTB11 (pin C10) is disabled */
}

#define PIN20_IDX 20u /*!< Pin number for pin 20 in a port */
#define PIN21_IDX 21u /*!< Pin number for pin 21 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART4_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: J5, peripheral: LPUART4, signal: RX, pin_signal: PTA21/LLWU_P21/I2C0_SDA/LPUART4_RX/FXIO0_D9/EWM_IN}
  - {pin_num: H5, peripheral: LPUART4, signal: TX, pin_signal:
PTA20/I2C0_SCL/LPUART4_TX/FTM_CLKIN1/FXIO0_D8/EWM_OUT_b/TPM_CLKIN1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART4_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART4_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortA); /* Port A Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTA, PIN20_IDX, kPORT_MuxAlt3); /* PORTA20 (pin H5) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTA, PIN21_IDX, kPORT_MuxAlt3); /* PORTA21 (pin J5) is configured as LPUART4_RX */
}

#define PIN20_IDX 20u /*!< Pin number for pin 20 in a port */
#define PIN21_IDX 21u /*!< Pin number for pin 21 in a port */
                      /*
                       * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                      LPUART4_DeinitPins:
                      - options: {coreID: singlecore, enableClock: 'false'}
                      - pin_list:
                        - {pin_num: J5, peripheral: n/a, signal: disabled, pin_signal: PTA21/LLWU_P21/I2C0_SDA/LPUART4_RX/FXIO0_D9/EWM_IN}
                        - {pin_num: H5, peripheral: n/a, signal: disabled, pin_signal:
                      PTA20/I2C0_SCL/LPUART4_TX/FTM_CLKIN1/FXIO0_D8/EWM_OUT_b/TPM_CLKIN1}
                       * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                       */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART4_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART4_DeinitPins(void)
{
    PORT_SetPinMux(PORTA, PIN20_IDX, kPORT_PinDisabledOrAnalog); /* PORTA20 (pin H5) is disabled */
    PORT_SetPinMux(PORTA, PIN21_IDX, kPORT_PinDisabledOrAnalog); /* PORTA21 (pin J5) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
