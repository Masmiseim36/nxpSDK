/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKV46F256xxx16
package_id: MKV46F256VLL16
mcu_data: ksdk2_0
processor_version: 0.0.8
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: UART1, signal: TX, pin_signal: ADCB_CH6f/PTE0/UART1_TX/XBAROUT10/XBARIN11}
  - {pin_num: '2', peripheral: UART1, signal: RX, pin_signal: ADCB_CH7f/PTE1/LLWU_P0/UART1_RX/XBAROUT11/XBARIN7}
  - {pin_num: '93', peripheral: GPIOD, signal: 'GPIO, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/FTM3_CH0/FTM0_CH0/FlexPWM_A0}
  - {pin_num: '94', peripheral: GPIOD, signal: 'GPIO, 1', pin_signal: ADCA_CH7f/PTD1/SPI0_SCK/FTM3_CH1/FTM0_CH1/FlexPWM_B0}
  - {pin_num: '95', peripheral: GPIOD, signal: 'GPIO, 2', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/FTM3_CH2/FTM0_CH2/FlexPWM_A1/I2C0_SCL}
  - {pin_num: '96', peripheral: GPIOD, signal: 'GPIO, 3', pin_signal: PTD3/SPI0_SIN/FTM3_CH3/FTM0_CH3/FlexPWM_B1/I2C0_SDA}
  - {pin_num: '97', peripheral: GPIOD, signal: 'GPIO, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FlexPWM_A2/EWM_IN/SPI0_PCS0}
  - {pin_num: '98', peripheral: GPIOD, signal: 'GPIO, 5', pin_signal: ADCA_CH6g/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FlexPWM_B2/EWM_OUT_b/SPI0_SCK}
  - {pin_num: '71', peripheral: GPIOC, signal: 'GPIO, 1', pin_signal: ADCB_CH7b/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FlexPWM_A3/XBARIN11}
  - {pin_num: '72', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: ADCB_CH6c/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FlexPWM_B3/XBARIN6}
  - {pin_num: '69', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI0_PCS5/FlexPWM_X3/CMP3_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB23 (pin 69) is configured as PTB23 */
    PORT_SetPinMux(PORTB, 23U, kPORT_MuxAsGpio);

    /* PORTC1 (pin 71) is configured as PTC1 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAsGpio);

    /* PORTC2 (pin 72) is configured as PTC2 */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAsGpio);

    /* PORTD0 (pin 93) is configured as PTD0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAsGpio);

    /* PORTD1 (pin 94) is configured as PTD1 */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAsGpio);

    /* PORTD2 (pin 95) is configured as PTD2 */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAsGpio);

    /* PORTD3 (pin 96) is configured as PTD3 */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAsGpio);

    /* PORTD4 (pin 97) is configured as PTD4 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAsGpio);

    /* PORTD5 (pin 98) is configured as PTD5 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAsGpio);

    /* PORTE0 (pin 1) is configured as UART1_TX */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);

    /* PORTE1 (pin 2) is configured as UART1_RX */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK | SIM_SOPT5_UART1RXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)

                  /* UART 1 receive data source select: UART1_RX pin. */
                  | SIM_SOPT5_UART1RXSRC(SOPT5_UART1RXSRC_UART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
