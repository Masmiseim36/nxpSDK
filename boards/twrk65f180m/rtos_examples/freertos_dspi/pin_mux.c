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
processor: MK65FN2M0xxx18
package_id: MK65FN2M0VMI18
mcu_data: ksdk2_0
processor_version: 0.0.10
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
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: C2, peripheral: SPI2, signal: PCS0_SS, pin_signal: PTD11/LLWU_P25/SPI2_PCS0/SDHC0_CLKIN/LPUART0_CTS_b/FB_A19}
  - {pin_num: B2, peripheral: SPI2, signal: SCK, pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/SDHC0_D4/FB_A20}
  - {pin_num: B3, peripheral: SPI2, signal: SOUT, pin_signal: PTD13/SPI2_SOUT/SDHC0_D5/FB_A21}
  - {pin_num: A2, peripheral: SPI2, signal: SIN, pin_signal: PTD14/SPI2_SIN/SDHC0_D6/FB_A22}
  - {pin_num: A3, peripheral: SPI2, signal: PCS1, pin_signal: PTD15/SPI2_PCS1/SDHC0_D7/FB_A23}
  - {pin_num: C1, peripheral: SPI1, signal: SCK, pin_signal: ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2}
  - {pin_num: B1, peripheral: SPI1, signal: SOUT, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN}
  - {pin_num: D1, peripheral: SPI1, signal: SIN, pin_signal: ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT}
  - {pin_num: E1, peripheral: SPI1, signal: PCS0_SS, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0}
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
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTD11 (pin C2) is configured as SPI2_PCS0 */
    PORT_SetPinMux(PORTD, 11U, kPORT_MuxAlt2);

    /* PORTD12 (pin B2) is configured as SPI2_SCK */
    PORT_SetPinMux(PORTD, 12U, kPORT_MuxAlt2);

    /* PORTD13 (pin B3) is configured as SPI2_SOUT */
    PORT_SetPinMux(PORTD, 13U, kPORT_MuxAlt2);

    /* PORTD14 (pin A2) is configured as SPI2_SIN */
    PORT_SetPinMux(PORTD, 14U, kPORT_MuxAlt2);

    /* PORTD15 (pin A3) is configured as SPI2_PCS1 */
    PORT_SetPinMux(PORTD, 15U, kPORT_MuxAlt2);

    /* PORTE1 (pin B1) is configured as SPI1_SOUT */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt2);

    /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt3);

    /* PORTE17 (pin F5) is configured as UART2_RX */
    PORT_SetPinMux(PORTE, 17U, kPORT_MuxAlt3);

    /* PORTE2 (pin C1) is configured as SPI1_SCK */
    PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt2);

    /* PORTE3 (pin D1) is configured as SPI1_SIN */
    PORT_SetPinMux(PORTE, 3U, kPORT_MuxAlt2);

    /* PORTE4 (pin E1) is configured as SPI1_PCS0 */
    PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
