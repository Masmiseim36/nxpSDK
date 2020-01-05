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
processor: MKL82Z128xxx7
package_id: MKL82Z128VMC7
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
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/TPM0_CH2/CLKOUT}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/TPM0_CH3}
  - {pin_num: B1, peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: C2, peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN}
  - {pin_num: C1, peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/QSPI0A_DATA0/SPI1_SCK}
  - {pin_num: D2, peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/QSPI0A_DATA2/SPI1_SOUT}
  - {pin_num: D1, peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/QSPI0A_DATA1}
  - {pin_num: E2, peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/QSPI0A_SS0_B/USB0_SOF_OUT}
  - {pin_num: E1, peripheral: QuadSPI0, signal: 'DATA3, B', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/QSPI0B_DATA3}
  - {pin_num: F3, peripheral: QuadSPI0, signal: 'SCLK, B', pin_signal: PTE7/QSPI0B_SCLK/QSPI0A_SS1_B}
  - {pin_num: F2, peripheral: QuadSPI0, signal: 'DATA0, B', pin_signal: PTE8/QSPI0B_DATA0}
  - {pin_num: F1, peripheral: QuadSPI0, signal: 'DATA2, B', pin_signal: PTE9/LLWU_P17/QSPI0B_DATA2}
  - {pin_num: G2, peripheral: QuadSPI0, signal: 'DATA1, B', pin_signal: PTE10/LLWU_P18/QSPI0B_DATA1}
  - {pin_num: G1, peripheral: QuadSPI0, signal: 'SS0, B', pin_signal: PTE11/QSPI0B_SS0_B/QSPI0A_DQS}
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
    /* PTC Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* PTE Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin A8) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    /* PORTE0 (pin B1) is configured as QSPI0A_DATA3 */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt5);

    /* PORTE1 (pin C2) is configured as QSPI0A_SCLK */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt5);

    /* PORTE10 (pin G2) is configured as QSPI0B_DATA1 */
    PORT_SetPinMux(PORTE, 10U, kPORT_MuxAlt5);

    /* PORTE11 (pin G1) is configured as QSPI0B_SS0_B */
    PORT_SetPinMux(PORTE, 11U, kPORT_MuxAlt5);

    /* PORTE2 (pin C1) is configured as QSPI0A_DATA0 */
    PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt5);

    /* PORTE3 (pin D2) is configured as QSPI0A_DATA2 */
    PORT_SetPinMux(PORTE, 3U, kPORT_MuxAlt5);

    /* PORTE4 (pin D1) is configured as QSPI0A_DATA1 */
    PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt5);

    /* PORTE5 (pin E2) is configured as QSPI0A_SS0_B */
    PORT_SetPinMux(PORTE, 5U, kPORT_MuxAlt5);

    /* PORTE6 (pin E1) is configured as QSPI0B_DATA3 */
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAlt5);

    /* PORTE7 (pin F3) is configured as QSPI0B_SCLK */
    PORT_SetPinMux(PORTE, 7U, kPORT_MuxAlt5);

    /* PORTE8 (pin F2) is configured as QSPI0B_DATA0 */
    PORT_SetPinMux(PORTE, 8U, kPORT_MuxAlt5);

    /* PORTE9 (pin F1) is configured as QSPI0B_DATA2 */
    PORT_SetPinMux(PORTE, 9U, kPORT_MuxAlt5);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK)))

                  /* LPUART1 transmit data source select: LPUART1_TX pin. */
                  | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX)

                  /* LPUART1 receive data source select: LPUART1_RX pin. */
                  | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
