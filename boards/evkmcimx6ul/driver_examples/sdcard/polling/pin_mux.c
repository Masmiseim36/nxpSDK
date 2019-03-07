/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
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
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MCIMX6G3xxx05
package_id: MCIMX6G3CVM05
mcu_data: i_mx_1_0
processor_version: 1.0.0
board: MCIMX6UL-EVK-REV-B
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {coreID: ca7}
- pin_list:
  - {pin_num: C1, peripheral: uSDHC1, signal: usdhc_clk, pin_signal: SD1_CLK, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, SPEED: MHZ_50, DSE: OHM_240,
    SRE: FAST}
  - {pin_num: C2, peripheral: uSDHC1, signal: usdhc_cmd, pin_signal: SD1_CMD, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: B3, peripheral: uSDHC1, signal: 'usdhc_data, 0', pin_signal: SD1_DATA0, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: B2, peripheral: uSDHC1, signal: 'usdhc_data, 1', pin_signal: SD1_DATA1, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: B1, peripheral: uSDHC1, signal: 'usdhc_data, 2', pin_signal: SD1_DATA2, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: A2, peripheral: uSDHC1, signal: 'usdhc_data, 3', pin_signal: SD1_DATA3, HYS: ENABLED, PUS: OHM_47K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_240, SRE: FAST}
  - {pin_num: M15, peripheral: GPIO1, signal: 'gpio_io, 9', pin_signal: GPIO1_IO09, HYS: ENABLED, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, SPEED: MHZ_50, DSE: OHM_240,
    SRE: FAST}
  - {pin_num: K14, peripheral: UART1, signal: uart_rx, pin_signal: UART1_TX_DATA, DSE: OHM_120}
  - {pin_num: K16, peripheral: UART1, signal: uart_tx, pin_signal: UART1_RX_DATA, DSE: OHM_120}
  - {pin_num: J14, peripheral: GPIO1, signal: 'gpio_io, 19', pin_signal: UART1_RTS_B, HYS: ENABLED, PUS: OHM_100K_PU, PUE: KEEP, PKE: DISABLED, DSE: HIZ}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_CLK_USDHC1_CLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_CLK_USDHC1_CLK, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_CMD_USDHC1_CMD, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_CMD_USDHC1_CMD, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA0_USDHC1_DATA0, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA0_USDHC1_DATA0, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA1_USDHC1_DATA1, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA1_USDHC1_DATA1, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA2_USDHC1_DATA2, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA2_USDHC1_DATA2, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA3_USDHC1_DATA3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA3_USDHC1_DATA3, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART1_RTS_B_GPIO1_IO19, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RTS_B_GPIO1_IO19, 
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
