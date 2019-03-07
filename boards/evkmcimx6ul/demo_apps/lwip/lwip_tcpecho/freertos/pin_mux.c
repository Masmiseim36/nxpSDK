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
  - {pin_num: K13, peripheral: ENET1, signal: enet_1588_event0_in, pin_signal: GPIO1_IO00}
  - {pin_num: L15, peripheral: ENET1, signal: enet_1588_event0_out, pin_signal: GPIO1_IO01, SPEED: MHZ_200}
  - {pin_num: L16, peripheral: ENET1, signal: enet_mdc, pin_signal: GPIO1_IO07, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: MHZ_200, DSE: OHM_48, SRE: FAST}
  - {pin_num: K17, peripheral: ENET1, signal: enet_mdio, pin_signal: GPIO1_IO06, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: RESERVED0, DSE: OHM_48, SRE: FAST}
  - {pin_num: F16, peripheral: ENET1, signal: 'enet_rdata, 0', pin_signal: ENET1_RX_DATA0, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: MHZ_200, DSE: OHM_48,
    SRE: FAST}
  - {pin_num: E17, peripheral: ENET1, signal: 'enet_rdata, 1', pin_signal: ENET1_RX_DATA1, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: MHZ_200, DSE: OHM_48,
    SRE: FAST}
  - {pin_num: E16, peripheral: ENET1, signal: enet_rx_en, pin_signal: ENET1_RX_EN, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: MHZ_200, DSE: OHM_48, SRE: FAST}
  - {pin_num: D15, peripheral: ENET1, signal: enet_rx_er, pin_signal: ENET1_RX_ER, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: MHZ_200, DSE: OHM_48, SRE: FAST}
  - {pin_num: E15, peripheral: ENET1, signal: 'enet_tdata, 0', pin_signal: ENET1_TX_DATA0, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: MHZ_200, DSE: OHM_48,
    SRE: FAST}
  - {pin_num: E14, peripheral: ENET1, signal: 'enet_tdata, 1', pin_signal: ENET1_TX_DATA1, PUS: OHM_100K_PU, PUE: PULL, PKE: DISABLED, SPEED: MHZ_200, DSE: OHM_48,
    SRE: FAST}
  - {pin_num: F14, peripheral: ENET1, signal: 'enet_ref_clk, 1', pin_signal: ENET1_TX_CLK, SION: ENABLED, PUS: OHM_100K_PD, PUE: KEEP, DSE: OHM_40, SRE: FAST}
  - {pin_num: F15, peripheral: ENET1, signal: enet_tx_en, pin_signal: ENET1_TX_EN, PUS: OHM_100K_PU, PUE: PULL, PKE: ENABLED, SPEED: MHZ_200, DSE: OHM_48, SRE: FAST}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_DATA0_ENET1_RDATA00, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_DATA0_ENET1_RDATA00, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_DATA1_ENET1_RDATA01, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_DATA1_ENET1_RDATA01, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_EN_ENET1_RX_EN, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_EN_ENET1_RX_EN, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_ER_ENET1_RX_ER, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_ER_ENET1_RX_ER, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_CLK_ENET1_REF_CLK1, 1U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_CLK_ENET1_REF_CLK1, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_DATA0_ENET1_TDATA00, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_DATA0_ENET1_TDATA00, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_DATA1_ENET1_TDATA01, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_DATA1_ENET1_TDATA01, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_EN_ENET1_TX_EN, 0U);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_EN_ENET1_TX_EN, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO00_ENET1_1588_EVENT0_IN, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO01_ENET1_1588_EVENT0_OUT, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO01_ENET1_1588_EVENT0_OUT, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO06_ENET1_MDIO, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO06_ENET1_MDIO, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO07_ENET1_MDC, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO07_ENET1_MDC, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
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

    IOMUXC_GPR->GPR1 |= 0x20000;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
