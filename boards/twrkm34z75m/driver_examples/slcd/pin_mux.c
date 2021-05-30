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
- !!processor 'MKM34Z256xxx7'
- !!package 'MKM34Z256VLQ7'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.1.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define MISC_CTL_UART2IRSEL_NONE      0x00u   /*!< UART2 IrDA Select: Pad RX input PTI[6] or PTE[6] selected for RX input of UART2 and UART2 TX signal is not used for modulation */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '6', peripheral: UART2, signal: RX, pin_signal: LCD_P46/PTI6/UART2_RX}
  - {pin_num: '7', peripheral: UART2, signal: TX, pin_signal: LCD_P47/PTI7/UART2_TX}
  - {pin_num: '4', peripheral: LCD, signal: 'P, 23', pin_signal: LCD_P23/PTA0/LLWU_P16}
  - {pin_num: '5', peripheral: LCD, signal: 'P, 24', pin_signal: LCD_P24/PTA1}
  - {pin_num: '8', peripheral: LCD, signal: 'P, 25', pin_signal: LCD_P25/PTA2}
  - {pin_num: '9', peripheral: LCD, signal: 'P, 26', pin_signal: LCD_P26/PTA3}
  - {pin_num: '11', peripheral: LCD, signal: 'P, 28', pin_signal: LCD_P28/PTA5/CMP0_OUT}
  - {pin_num: '12', peripheral: LCD, signal: 'P, 29', pin_signal: LCD_P29/PTA6/LLWU_P14/XBAR_IN0}
  - {pin_num: '13', peripheral: LCD, signal: 'P, 30', pin_signal: LCD_P30/PTA7/XBAR_OUT0}
  - {pin_num: '16', peripheral: LCD, signal: 'P, 31', pin_signal: LCD_P31/PTB0}
  - {pin_num: '20', peripheral: LCD, signal: 'P, 32', pin_signal: LCD_P32/PTB1/LLWU_P17}
  - {pin_num: '21', peripheral: LCD, signal: 'P, 33', pin_signal: LCD_P33/PTB2}
  - {pin_num: '22', peripheral: LCD, signal: 'P, 34', pin_signal: LCD_P34/PTB3}
  - {pin_num: '23', peripheral: LCD, signal: 'P, 35', pin_signal: LCD_P35/PTB4}
  - {pin_num: '24', peripheral: LCD, signal: 'P, 36', pin_signal: LCD_P36/PTB5}
  - {pin_num: '25', peripheral: LCD, signal: 'P, 37', pin_signal: LCD_P37/CMP1_IN0/PTB6}
  - {pin_num: '26', peripheral: LCD, signal: 'P, 38', pin_signal: LCD_P38/PTB7/AFE_CLK}
  - {pin_num: '31', peripheral: LCD, signal: 'P, 43', pin_signal: LCD_P43/PTC4}
  - {pin_num: '116', peripheral: LCD, signal: 'P, 13', pin_signal: LCD_P13/PTG6/LLWU_P0/LPTMR0_ALT3}
  - {pin_num: '117', peripheral: LCD, signal: 'P, 14', pin_signal: LCD_P14/PTG7}
  - {pin_num: '122', peripheral: LCD, signal: 'P, 19', pin_signal: LCD_P19/PTH4}
  - {pin_num: '123', peripheral: LCD, signal: 'P, 20', pin_signal: LCD_P20/PTH5}
  - {pin_num: '131', peripheral: LCD, signal: 'P, 22', pin_signal: LCD_P22/PTI3/LPUART0_TX/CMP2_OUT}
  - {pin_num: '143', peripheral: LCD, signal: 'P, 44', pin_signal: LCD_P44/PTI4}
  - {pin_num: '3', peripheral: LCD, signal: 'P, 45', pin_signal: LCD_P45/PTI5}
  - {pin_num: '17', peripheral: LCD, signal: 'P, 50', pin_signal: LCD_P50/PTJ2}
  - {pin_num: '139', peripheral: LCD, signal: 'P, 56', pin_signal: LCD_P56/PTL3/EWM_IN}
  - {pin_num: '140', peripheral: LCD, signal: 'P, 57', pin_signal: LCD_P57/PTL4/EWM_OUT_b}
  - {pin_num: '141', peripheral: LCD, signal: 'P, 58', pin_signal: LCD_P58/PTL5/LLWU_P23}
  - {pin_num: '142', peripheral: LCD, signal: 'P, 59', pin_signal: LCD_P59/PTL6}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* PCTLA Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* PCTLB Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* PCTLC Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortG);                           /* PCTLG Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortH);                           /* PCTLH Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortI);                           /* PCTLI Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortJ);                           /* PCTLJ Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortL);                           /* PCTLL Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTA0 (pin 4) is configured as LCD_P23 */
  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTA1 (pin 5) is configured as LCD_P24 */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTA2 (pin 8) is configured as LCD_P25 */
  PORT_SetPinMux(PORTA, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTA3 (pin 9) is configured as LCD_P26 */
  PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTA5 (pin 11) is configured as LCD_P28 */
  PORT_SetPinMux(PORTA, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTA6 (pin 12) is configured as LCD_P29 */
  PORT_SetPinMux(PORTA, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTA7 (pin 13) is configured as LCD_P30 */
  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTB0 (pin 16) is configured as LCD_P31 */
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTB1 (pin 20) is configured as LCD_P32 */
  PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTB2 (pin 21) is configured as LCD_P33 */
  PORT_SetPinMux(PORTB, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTB3 (pin 22) is configured as LCD_P34 */
  PORT_SetPinMux(PORTB, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTB4 (pin 23) is configured as LCD_P35 */
  PORT_SetPinMux(PORTB, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTB5 (pin 24) is configured as LCD_P36 */
  PORT_SetPinMux(PORTB, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTB6 (pin 25) is configured as LCD_P37 */
  PORT_SetPinMux(PORTB, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTB7 (pin 26) is configured as LCD_P38 */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTC4 (pin 31) is configured as LCD_P43 */
  PORT_SetPinMux(PORTG, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTG6 (pin 116) is configured as LCD_P13 */
  PORT_SetPinMux(PORTG, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTG7 (pin 117) is configured as LCD_P14 */
  PORT_SetPinMux(PORTH, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTH4 (pin 122) is configured as LCD_P19 */
  PORT_SetPinMux(PORTH, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTH5 (pin 123) is configured as LCD_P20 */
  PORT_SetPinMux(PORTI, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTI3 (pin 131) is configured as LCD_P22 */
  PORT_SetPinMux(PORTI, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTI4 (pin 143) is configured as LCD_P44 */
  PORT_SetPinMux(PORTI, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTI5 (pin 3) is configured as LCD_P45 */
  PORT_SetPinMux(PORTI, PIN6_IDX, kPORT_MuxAlt2);            /* PORTI6 (pin 6) is configured as UART2_RX */
  PORT_SetPinMux(PORTI, PIN7_IDX, kPORT_MuxAlt2);            /* PORTI7 (pin 7) is configured as UART2_TX */
  PORT_SetPinMux(PORTJ, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTJ2 (pin 17) is configured as LCD_P50 */
  PORT_SetPinMux(PORTL, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTL3 (pin 139) is configured as LCD_P56 */
  PORT_SetPinMux(PORTL, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTL4 (pin 140) is configured as LCD_P57 */
  PORT_SetPinMux(PORTL, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTL5 (pin 141) is configured as LCD_P58 */
  PORT_SetPinMux(PORTL, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTL6 (pin 142) is configured as LCD_P59 */
  SIM->MISC_CTL = ((SIM->MISC_CTL &
    (~(SIM_MISC_CTL_UART2IRSEL_MASK)))                       /* Mask bits to zero which are setting */
      | SIM_MISC_CTL_UART2IRSEL(MISC_CTL_UART2IRSEL_NONE)    /* UART2 IrDA Select: Pad RX input PTI[6] or PTE[6] selected for RX input of UART2 and UART2 TX signal is not used for modulation */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
