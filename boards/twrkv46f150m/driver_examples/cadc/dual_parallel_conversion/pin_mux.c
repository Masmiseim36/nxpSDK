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
  - {pin_num: '26', peripheral: ADC, signal: 'ADCA, 4', pin_signal: ADCA_CH4/CMP1_IN5/CMP0_IN5/PTE29/FTM0_CH2/FTM_CLKIN0}
  - {pin_num: '27', peripheral: ADC, signal: 'ADCA, 5', pin_signal: DAC0_OUT/CMP1_IN3/ADCA_CH5/PTE30/FTM0_CH3/FTM_CLKIN1}
  - {pin_num: '31', peripheral: ADC, signal: 'ADCB, 4', pin_signal: ADCB_CH4/PTE24/CAN1_TX/FTM0_CH0/XBARIN2/I2C0_SCL/EWM_OUT_b/XBAROUT4}
  - {pin_num: '32', peripheral: ADC, signal: 'ADCB, 5', pin_signal: ADCB_CH5/PTE25/LLWU_P21/CAN1_RX/FTM0_CH1/XBARIN3/I2C0_SDA/EWM_IN/XBAROUT5}
  - {pin_num: '58', peripheral: ADC, signal: 'ADCB, 6', pin_signal: ADCB_CH6a/PTB10/FTM0_FLT1}
  - {pin_num: '56', peripheral: ADC, signal: 'ADCB, 7', pin_signal: ADCB_CH7e/CMP3_IN5/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/FTM0_FLT0}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB10 (pin 58) is configured as ADCB_CH6a */
    PORT_SetPinMux(PORTB, 10U, kPORT_PinDisabledOrAnalog);

    /* PORTB3 (pin 56) is configured as ADCB_CH7e */
    PORT_SetPinMux(PORTB, 3U, kPORT_PinDisabledOrAnalog);

    /* PORTE0 (pin 1) is configured as UART1_TX */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);

    /* PORTE1 (pin 2) is configured as UART1_RX */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    /* PORTE24 (pin 31) is configured as ADCB_CH4 */
    PORT_SetPinMux(PORTE, 24U, kPORT_PinDisabledOrAnalog);

    /* PORTE25 (pin 32) is configured as ADCB_CH5 */
    PORT_SetPinMux(PORTE, 25U, kPORT_PinDisabledOrAnalog);

    /* PORTE29 (pin 26) is configured as ADCA_CH4 */
    PORT_SetPinMux(PORTE, 29U, kPORT_PinDisabledOrAnalog);

    /* PORTE30 (pin 27) is configured as ADCA_CH5 */
    PORT_SetPinMux(PORTE, 30U, kPORT_PinDisabledOrAnalog);

    SIM->ADCOPT = ((SIM->ADCOPT &
                    /* Mask bits to zero which are setting */
                    (~(SIM_ADCOPT_ADCBCH6SEL_MASK | SIM_ADCOPT_ADCBCH7SEL_MASK)))

                   /* ADCB MUX1 selection for ADCB channel 6: ADCB MUX0's channel a. */
                   | SIM_ADCOPT_ADCBCH6SEL(ADCOPT_ADCBCH6SEL_CH_A)

                   /* ADCB MUX1 selection for ADCB channel 7: ADCB MUX1's channel e. */
                   | SIM_ADCOPT_ADCBCH7SEL(ADCOPT_ADCBCH7SEL_CH_E));

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
