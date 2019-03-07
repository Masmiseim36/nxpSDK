/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
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
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    /* Initialize UART1 pins below */
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, IOMUXC_SW_PAD_CTL_PAD_SPEED(2) | IOMUXC_SW_PAD_CTL_PAD_DSE(2));
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, IOMUXC_SW_PAD_CTL_PAD_SPEED(2));

    IOMUXC_SetPinMux(IOMUXC_SD1_CMD_USDHC1_CMD, 0);
    IOMUXC_SetPinMux(IOMUXC_SD1_CLK_USDHC1_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_USDHC1_CD_B, 0);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA0_USDHC1_DATA0, 0);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA1_USDHC1_DATA1, 0);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA2_USDHC1_DATA2, 0);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA3_USDHC1_DATA3, 0);
    /* Power reset PIN */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0);

    IOMUXC_SetPinConfig(IOMUXC_SD1_CMD_USDHC1_CMD, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(2) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(1));
    IOMUXC_SetPinConfig(IOMUXC_SD1_CLK_USDHC1_CLK, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(1));
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA0_USDHC1_DATA0, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(1));

    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA1_USDHC1_DATA1, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(1));

    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA2_USDHC1_DATA2, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(1));

    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA3_USDHC1_DATA3, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(1));
    /*power reset pin*/
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09,
                        IOMUXC_SW_PAD_CTL_PAD_DSE(1) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);

    /* card detect pin */
    IOMUXC_SetPinMux(IOMUXC_UART1_RTS_B_GPIO1_IO19, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RTS_B_GPIO1_IO19, IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
}
