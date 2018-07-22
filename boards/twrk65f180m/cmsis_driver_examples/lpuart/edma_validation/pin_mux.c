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
- !!processor 'MK65FN2M0xxx18'
- !!package 'MK65FN2M0VMI18'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN16_IDX 16u /*!< Pin number for pin 16 in a port */
#define PIN17_IDX 17u /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal:
ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal:
ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortE); /* Port E Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt3); /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt3); /* PORTE17 (pin F5) is configured as UART2_RX */
}

#define PIN8_IDX 8u                        /*!< Pin number for pin 8 in a port */
#define PIN9_IDX 9u                        /*!< Pin number for pin 9 in a port */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!< LPUART0 transmit data source select: LPUART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: E4, peripheral: LPUART0, signal: TX, pin_signal: PTE8/I2S0_RXD1/I2S0_RX_FS/LPUART0_TX/FTM3_CH3}
  - {pin_num: F3, peripheral: LPUART0, signal: RX, pin_signal: PTE9/LLWU_P17/I2S0_TXD1/I2S0_RX_BCLK/LPUART0_RX/FTM3_CH4}
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
    CLOCK_EnableClock(kCLOCK_PortE); /* Port E Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTE, PIN8_IDX, kPORT_MuxAlt5); /* PORTE8 (pin E4) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTE, PIN9_IDX, kPORT_MuxAlt5); /* PORTE9 (pin F3) is configured as LPUART0_RX */
    SIM->SOPT5 =
        ((SIM->SOPT5 & (~(SIM_SOPT5_LPUART0TXSRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX) /* LPUART0 transmit data source select: LPUART0_TX pin */
         );
}

#define PIN8_IDX 8u /*!< Pin number for pin 8 in a port */
#define PIN9_IDX 9u /*!< Pin number for pin 9 in a port */
                    /*
                     * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                    LPUART0_DeinitPins:
                    - options: {coreID: singlecore, enableClock: 'false'}
                    - pin_list:
                      - {pin_num: F3, peripheral: n/a, signal: disabled, pin_signal:
                    PTE9/LLWU_P17/I2S0_TXD1/I2S0_RX_BCLK/LPUART0_RX/FTM3_CH4}
                      - {pin_num: E4, peripheral: n/a, signal: disabled, pin_signal: PTE8/I2S0_RXD1/I2S0_RX_FS/LPUART0_TX/FTM3_CH3}
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
    PORT_SetPinMux(PORTE, PIN8_IDX, kPORT_PinDisabledOrAnalog); /* PORTE8 (pin E4) is disabled */
    PORT_SetPinMux(PORTE, PIN9_IDX, kPORT_PinDisabledOrAnalog); /* PORTE9 (pin F3) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
