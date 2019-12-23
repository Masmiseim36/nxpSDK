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
processor: MKV11Z128xxx7
package_id: MKV11Z128VLH7
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
  - {pin_num: '39', peripheral: UART0, signal: RX, pin_signal: PTB16/UART0_RX/FTM_CLKIN2/CAN0_TX/EWM_IN}
  - {pin_num: '40', peripheral: UART0, signal: TX, pin_signal: PTB17/UART0_TX/FTM_CLKIN1/CAN0_RX/EWM_OUT_b}
  - {pin_num: '46', peripheral: FTM0, signal: 'CH, 2', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0, direction: OUTPUT}
  - {pin_num: '49', peripheral: FTM0, signal: 'CH, 3', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT, direction: OUTPUT}
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

    /* PORTB16 (pin 39) is configured as UART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 40) is configured as UART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    /* PORTC3 (pin 46) is configured as FTM0_CH2 */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt4);

    /* PORTC4 (pin 49) is configured as FTM0_CH3 */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt4);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 Transmit Data Source Select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 Receive Data Source Select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));

    SIM->SOPT8 = ((SIM->SOPT8 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT8_FTM0OCH2SRC_MASK | SIM_SOPT8_FTM0OCH3SRC_MASK)))

                  /* FTM0 channel 2 output PWM/OCMP mode source select: FTM0CH2 pin is the output of FTM0 channel
                   * 2 PWM/OCMP. */
                  | SIM_SOPT8_FTM0OCH2SRC(SOPT8_FTM0OCH2SRC_NO_MODULATION)

                  /* FTM0 channel 3 output PWM/OCMP mode source select: FTM0CH3 pin is the output of FTM0 channel
                   * 3 PWM/OCMP. */
                  | SIM_SOPT8_FTM0OCH3SRC(SOPT8_FTM0OCH3SRC_NO_MODULATION));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
