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
processor: MK82FN256xxx15
package_id: MK82FN256VLL15
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
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '3', peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/SPI1_SCK}
  - {pin_num: '7', peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1}
  - {pin_num: '4', peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/SPI1_SOUT}
  - {pin_num: '1', peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: '2', peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN}
  - {pin_num: '14', peripheral: QuadSPI0, signal: 'SS0, B', pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS}
  - {pin_num: '11', peripheral: QuadSPI0, signal: 'DATA0, B', pin_signal: PTE8/I2S0_RXD1/SPI2_SOUT/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5}
  - {pin_num: '13', peripheral: QuadSPI0, signal: 'DATA1, B', pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7}
  - {pin_num: '12', peripheral: QuadSPI0, signal: 'DATA2, B', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/SPI2_PCS1/I2S0_RX_BCLK/QSPI0B_DATA2/FTM3_CH4/SDHC0_D6}
  - {pin_num: '9', peripheral: QuadSPI0, signal: 'DATA3, B', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4}
  - {pin_num: '10', peripheral: QuadSPI0, signal: 'SCLK, B', pin_signal: PTE7/SPI2_SCK/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B}
  - {pin_num: '8', peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    /* PORTE0 (pin 1) is configured as QSPI0A_DATA3 */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt5);

    /* PORTE1 (pin 2) is configured as QSPI0A_SCLK */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt5);

    /* PORTE10 (pin 13) is configured as QSPI0B_DATA1 */
    PORT_SetPinMux(PORTE, 10U, kPORT_MuxAlt5);

    /* PORTE11 (pin 14) is configured as QSPI0B_SS0_B */
    PORT_SetPinMux(PORTE, 11U, kPORT_MuxAlt5);

    /* PORTE2 (pin 3) is configured as QSPI0A_DATA0 */
    PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt5);

    /* PORTE3 (pin 4) is configured as QSPI0A_DATA2 */
    PORT_SetPinMux(PORTE, 3U, kPORT_MuxAlt5);

    /* PORTE4 (pin 7) is configured as QSPI0A_DATA1 */
    PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt5);

    /* PORTE5 (pin 8) is configured as QSPI0A_SS0_B */
    PORT_SetPinMux(PORTE, 5U, kPORT_MuxAlt5);

    /* PORTE6 (pin 9) is configured as QSPI0B_DATA3 */
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAlt5);

    /* PORTE7 (pin 10) is configured as QSPI0B_SCLK */
    PORT_SetPinMux(PORTE, 7U, kPORT_MuxAlt5);

    /* PORTE8 (pin 11) is configured as QSPI0B_DATA0 */
    PORT_SetPinMux(PORTE, 8U, kPORT_MuxAlt5);

    /* PORTE9 (pin 12) is configured as QSPI0B_DATA2 */
    PORT_SetPinMux(PORTE, 9U, kPORT_MuxAlt5);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
