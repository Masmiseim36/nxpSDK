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
  - {pin_num: P1, peripheral: MMDC, signal: mmdc_sdclk0_p, pin_signal: DRAM_SDCLK0_P, PUS: OHM_100K_PD}
  - {pin_num: J2, peripheral: MMDC, signal: mmdc_cas_b, pin_signal: DRAM_CAS_B, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED}
  - {pin_num: M5, peripheral: MMDC, signal: mmdc_ras_b, pin_signal: DRAM_RAS_B, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40, DDR_INPUT: DIFFERENTIAL}
  - {pin_num: G4, peripheral: MMDC, signal: mmdc_reset, pin_signal: DRAM_RESET, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40}
  - {pin_num: K2, peripheral: MMDC, signal: 'mmdc_sdba, 2', pin_signal: DRAM_SDBA2, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DDR_SEL: RESERVED0}
  - {pin_num: T7, peripheral: MMDC, signal: 'mmdc_dqm, 0', pin_signal: DRAM_DQM0, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40}
  - {pin_num: T3, peripheral: MMDC, signal: 'mmdc_dqm, 1', pin_signal: DRAM_DQM1, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40}
  - {pin_num: N1, peripheral: MMDC, signal: 'mmdc_odt, 0', pin_signal: DRAM_ODT0, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40}
  - {pin_num: F1, peripheral: MMDC, signal: 'mmdc_odt, 1', pin_signal: DRAM_ODT1, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40}
  - {pin_num: P6, peripheral: MMDC, signal: 'mmdc_sdqs_p, 0', pin_signal: DRAM_SDQS0_P, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40}
  - {pin_num: T1, peripheral: MMDC, signal: 'mmdc_sdqs_p, 1', pin_signal: DRAM_SDQS1_P, PUS: OHM_100K_PD, PUE: KEEP, PKE: DISABLED, DSE: OHM_40}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
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

    IOMUXC_SetPinConfig(IOMUXC_DRAM_CAS_B, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_DQM0, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_DQM1, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_ODT0, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_ODT1, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_RAS_B, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_RESET, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_SDBA2, 0U);
    IOMUXC_SetPinConfig(IOMUXC_DRAM_SDCLK0_P, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_SDQS0_P, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));
    IOMUXC_SetPinConfig(IOMUXC_DRAM_SDQS1_P, 
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U));

    IOMUXC_SetPinConfig(IOMUXC_GRP_DDR_TYPE, IOMUXC_SW_PAD_CTL_GRP_DDR_SEL(3));
    IOMUXC_SetPinConfig(IOMUXC_GRP_DDRPKE, 0);
    IOMUXC_SetPinConfig(IOMUXC_GRP_ADDDS, IOMUXC_SW_PAD_CTL_GRP_DSE(6));
    IOMUXC_SetPinConfig(IOMUXC_GRP_CTLDS, IOMUXC_SW_PAD_CTL_GRP_DSE(6));
    IOMUXC_SetPinConfig(IOMUXC_GRP_DDRMODE_CTL, IOMUXC_SW_PAD_CTL_GRP_DDR_INPUT(1));
    IOMUXC_SetPinConfig(IOMUXC_GRP_DDRMODE, IOMUXC_SW_PAD_CTL_GRP_DDR_INPUT(1));
    IOMUXC_SetPinConfig(IOMUXC_GRP_B0DS, IOMUXC_SW_PAD_CTL_GRP_DSE(6));
    IOMUXC_SetPinConfig(IOMUXC_GRP_B1DS, IOMUXC_SW_PAD_CTL_GRP_DSE(6));
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
