/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "nx_driver_imxrt.h"

extern VOID nx_driver_imx_ethernet_isr(VOID);

/* the IRQ handler for the ENET network driver */
VOID ENET_IRQHandler(VOID)
{
    nx_driver_imx_ethernet_isr();

    SDK_ISR_EXIT_BARRIER;
}
