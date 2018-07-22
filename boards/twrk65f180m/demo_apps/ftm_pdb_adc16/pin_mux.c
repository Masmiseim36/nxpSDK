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
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN1_IDX 1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX 2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX 3u   /*!< Pin number for pin 3 in a port */
#define PIN5_IDX 5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX 6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX 7u   /*!< Pin number for pin 7 in a port */
#define PIN16_IDX 16u /*!< Pin number for pin 16 in a port */
#define PIN17_IDX 17u /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal:
ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal:
ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: B12, peripheral: FTM0, signal: 'CH, 0', pin_signal:
ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0}
  - {pin_num: A13, peripheral: FTM0, signal: 'CH, 1', pin_signal:
ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS}
  - {pin_num: A12, peripheral: FTM0, signal: 'CH, 2', pin_signal:
CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: M10, peripheral: FTM0, signal: 'CH, 3', pin_signal: PTA6/FTM0_CH3/CLKOUT/TRACE_CLKOUT}
  - {pin_num: L10, peripheral: FTM0, signal: 'CH, 4', pin_signal: ADC0_SE10/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3}
  - {pin_num: C4, peripheral: FTM0, signal: 'CH, 5', pin_signal:
ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK}
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
    CLOCK_EnableClock(kCLOCK_PortA); /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC); /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD); /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE); /* Port E Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTA, PIN6_IDX, kPORT_MuxAlt3);  /* PORTA6 (pin M10) is configured as FTM0_CH3 */
    PORT_SetPinMux(PORTA, PIN7_IDX, kPORT_MuxAlt3);  /* PORTA7 (pin L10) is configured as FTM0_CH4 */
    PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt4);  /* PORTC1 (pin B12) is configured as FTM0_CH0 */
    PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt4);  /* PORTC2 (pin A13) is configured as FTM0_CH1 */
    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt4);  /* PORTC3 (pin A12) is configured as FTM0_CH2 */
    PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt4);  /* PORTD5 (pin C4) is configured as FTM0_CH5 */
    PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt3); /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt3); /* PORTE17 (pin F5) is configured as UART2_RX */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
