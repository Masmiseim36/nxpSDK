/*
 * Copyright 2017 NXP
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
 * Copyright (c) 2016, NXP Semiconductors, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

    /* Initialize the ENET1 pins below. */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO00_ENET1_1588_EVENT0_IN, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO01_ENET1_1588_EVENT0_OUT, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO07_ENET1_MDC, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO06_ENET1_MDIO, 0);
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_CLK_ENET1_REF_CLK1, 1);
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_DATA0_ENET1_RDATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_DATA1_ENET1_RDATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_EN_ENET1_RX_EN, 0);
    IOMUXC_SetPinMux(IOMUXC_ENET1_RX_ER_ENET1_RX_ER, 0);
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_DATA0_ENET1_TDATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_DATA1_ENET1_TDATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_ENET1_TX_EN_ENET1_TX_EN, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO07_ENET1_MDC, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
                                                         IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) |
                                                         IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                         IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO06_ENET1_MDIO,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(0) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_ODE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_CLK_ENET1_REF_CLK1,
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_DATA0_ENET1_RDATA00,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_DATA1_ENET1_RDATA01,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_EN_ENET1_RX_EN,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_ER_ENET1_RX_ER,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_DATA0_ENET1_TDATA00,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_DATA1_ENET1_TDATA01,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_EN_ENET1_TX_EN,
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);

    IOMUXC_GPR->GPR1 |= 0x20000;
}
