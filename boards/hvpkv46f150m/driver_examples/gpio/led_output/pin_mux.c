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
  - {pin_num: '73', peripheral: UART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0}
  - {pin_num: '76', peripheral: UART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT, direction: OUTPUT}
  - {pin_num: '55', peripheral: GPIOB, signal: 'GPIO, 2', pin_signal: ADCA_CH6e/CMP2_IN2/PTB2/I2C0_SCL/UART0_RTS_b/FTM0_FLT1/FTM0_FLT3, direction: OUTPUT}
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

    /* PORTB2 (pin 55) is configured as PTB2 */
    PORT_SetPinMux(PORTB, 2U, kPORT_MuxAsGpio);

    /* PORTC3 (pin 73) is configured as UART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin 76) is configured as UART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

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
