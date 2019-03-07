/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
/*******************************************************************************
 * Code
 ******************************************************************************/

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    /* Initialize LPUART0 pins below */
    IOMUXC_SetPinMux(IOMUXC_PTA18_LPUART0_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_PTA19_LPUART0_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_PTA18_LPUART0_TX, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK  |
                                                 IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTA19_LPUART0_RX, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK  |
                                                 IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
}
