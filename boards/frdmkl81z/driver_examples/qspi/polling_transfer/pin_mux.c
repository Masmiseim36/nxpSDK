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
processor: MKL81Z128xxx7
package_id: MKL81Z128VLK7
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
  - {pin_num: '51', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/TPM_CLKIN0/EWM_IN}
  - {pin_num: '52', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/TPM_CLKIN1/EWM_OUT_b}
  - {pin_num: '1', peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: '2', peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN}
  - {pin_num: '3', peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/QSPI0A_DATA0/SPI1_SCK}
  - {pin_num: '4', peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/QSPI0A_DATA2/SPI1_SOUT}
  - {pin_num: '7', peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/QSPI0A_DATA1}
  - {pin_num: '8', peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/QSPI0A_SS0_B/USB0_SOF_OUT}
  - {pin_num: '9', peripheral: QuadSPI0, signal: 'SCLK, B', pin_signal: PTE7/QSPI0B_SCLK/QSPI0A_SS1_B}
  - {pin_num: '10', peripheral: QuadSPI0, signal: 'DATA0, B', pin_signal: PTE8/QSPI0B_DATA0}
  - {pin_num: '11', peripheral: QuadSPI0, signal: 'SS0, B', pin_signal: PTE11/QSPI0B_SS0_B/QSPI0A_DQS}
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
    /* PTB Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* PTE Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB16 (pin 51) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);

    /* PORTB17 (pin 52) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    /* PORTE0 (pin 1) is configured as QSPI0A_DATA3 */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt5);

    /* PORTE1 (pin 2) is configured as QSPI0A_SCLK */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt5);

    /* PORTE11 (pin 11) is configured as QSPI0B_SS0_B */
    PORT_SetPinMux(PORTE, 11U, kPORT_MuxAlt5);

    /* PORTE2 (pin 3) is configured as QSPI0A_DATA0 */
    PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt5);

    /* PORTE3 (pin 4) is configured as QSPI0A_DATA2 */
    PORT_SetPinMux(PORTE, 3U, kPORT_MuxAlt5);

    /* PORTE4 (pin 7) is configured as QSPI0A_DATA1 */
    PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt5);

    /* PORTE5 (pin 8) is configured as QSPI0A_SS0_B */
    PORT_SetPinMux(PORTE, 5U, kPORT_MuxAlt5);

    /* PORTE7 (pin 9) is configured as QSPI0B_SCLK */
    PORT_SetPinMux(PORTE, 7U, kPORT_MuxAlt5);

    /* PORTE8 (pin 10) is configured as QSPI0B_DATA0 */
    PORT_SetPinMux(PORTE, 8U, kPORT_MuxAlt5);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART 0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
