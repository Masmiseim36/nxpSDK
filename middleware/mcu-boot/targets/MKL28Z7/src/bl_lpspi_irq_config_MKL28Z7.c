/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_config.h"
#include "bootloader/bl_peripheral_interface.h"
#include "fsl_device_registers.h"
#include "fsl_intmux.h"
#include "fsl_lpspi.h"
#include "bootloader_common.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static const IRQn_Type lpspi_irq_ids[FSL_FEATURE_SOC_LPSPI_COUNT] = {
    LPSPI0_IRQn, LPSPI1_IRQn, LPSPI2_IRQn,
};

void LPSPI_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        case 2:
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(INTMUX0_2_IRQn);
                // LPSPI2_IRQ is assigned to INTMUX, LPTMR1_IRQ is the first irq assigned to INTMUX.
                // So LPSPI2_IRQ id - LPTMR1_IRQ id is the index of LPSPI2_IRQ in INTMUX.
                INTMUX0->CHANNEL[2].CHn_IER_31_0 |= 1 << (lpspi_irq_ids[instance] - LPTMR1_IRQn);
            }
            else
            {
                NVIC_DisableIRQ(INTMUX0_2_IRQn);
                INTMUX0->CHANNEL[2].CHn_IER_31_0 &= ~(1 << (lpspi_irq_ids[instance] - LPTMR1_IRQn));
            }
            break;
        case 0:
        case 1:
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(lpspi_irq_ids[instance]);
            }
            else
            {
                NVIC_DisableIRQ(lpspi_irq_ids[instance]);
            }
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
