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
- options: {callFromInitBoot: 'true', coreID: ca7}
- pin_list:
  - {pin_num: K16, peripheral: UART1, signal: uart_rx, pin_signal: UART1_RX_DATA, PUE: KEEP, PKE: ENABLED, DSE: OHM_120}
  - {pin_num: K14, peripheral: UART1, signal: uart_tx, pin_signal: UART1_TX_DATA, PUE: KEEP, PKE: ENABLED, DSE: OHM_120}
  - {pin_num: G14, peripheral: FLEXCAN1, signal: can_rx, pin_signal: UART3_RTS_B, HYS: ENABLED, PUS: OHM_100K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_40}
  - {pin_num: H15, peripheral: FLEXCAN1, signal: can_tx, pin_signal: UART3_CTS_B, HYS: ENABLED, PUS: OHM_100K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_40}
  - {pin_num: N10, peripheral: GPIO5, signal: 'gpio_io, 7', pin_signal: SNVS_TAMPER7, HYS: ENABLED, PUS: OHM_100K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_40}
  - {pin_num: N9, peripheral: GPIO5, signal: 'gpio_io, 8', pin_signal: SNVS_TAMPER8, HYS: ENABLED, PUS: OHM_100K_PU, PUE: PULL, PKE: ENABLED, DSE: OHM_40}
  - {pin_num: T10, peripheral: GPIO5, signal: 'gpio_io, 10', pin_signal: BOOT_MODE0, PUE: PULL}
  - {pin_num: U10, peripheral: GPIO5, signal: 'gpio_io, 11', pin_signal: BOOT_MODE1, PUE: PULL}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_BOOT_MODE0_GPIO5_IO10, 0U);
    IOMUXC_SetPinConfig(IOMUXC_BOOT_MODE0_GPIO5_IO10, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(4U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_BOOT_MODE1_GPIO5_IO11, 0U);
    IOMUXC_SetPinConfig(IOMUXC_BOOT_MODE1_GPIO5_IO11, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(4U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SNVS_TAMPER7_GPIO5_IO07, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_TAMPER7_GPIO5_IO07, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_SNVS_TAMPER8_GPIO5_IO08, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_TAMPER8_GPIO5_IO08, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
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
    IOMUXC_SetPinMux(IOMUXC_UART3_CTS_B_FLEXCAN1_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART3_CTS_B_FLEXCAN1_TX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
    IOMUXC_SetPinMux(IOMUXC_UART3_RTS_B_FLEXCAN1_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART3_RTS_B_FLEXCAN1_RX, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
