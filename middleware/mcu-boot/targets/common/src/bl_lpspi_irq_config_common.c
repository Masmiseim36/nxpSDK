/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_config.h"
#include "bootloader/bl_peripheral_interface.h"
#include "fsl_device_registers.h"
#include "fsl_lpspi.h"
#include "bootloader_common.h"
#if FSL_FEATURE_SOC_INTMUX_COUNT
#include "intmux/fsl_intmux.h"
#endif // #if FSL_FEATURE_SOC_INTMUX_COUNT
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static const IRQn_Type lpspi_irq_ids[FSL_FEATURE_SOC_LPSPI_COUNT] = {
    LPSPI0_IRQn,
#if (FSL_FEATURE_SOC_LPSPI_COUNT > 1)
    LPSPI1_IRQn,
#endif // #if (FSL_FEATURE_SOC_LPSPI_COUNT > 1)
#if (FSL_FEATURE_SOC_LPSPI_COUNT > 2)
    LPSPI2_IRQn,
#endif // #if (FSL_FEATURE_SOC_LPSPI_COUNT > 2)
};

void LPSPI_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        case 0:
#if (FSL_FEATURE_SOC_LPSPI_COUNT > 1)
        case 1:
#endif // #if (FSL_FEATURE_SOC_LPSPI_COUNT > 1)
#if (FSL_FEATURE_SOC_LPSPI_COUNT > 2) && !FSL_FEATURE_SOC_INTMUX_COUNT
        case 2:
#endif
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(lpspi_irq_ids[instance]);
            }
            else
            {
                NVIC_DisableIRQ(lpspi_irq_ids[instance]);
            }
            break;
#if (FSL_FEATURE_SOC_LPSPI_COUNT > 2) && FSL_FEATURE_SOC_INTMUX_COUNT
        case 2:
#if !defined(K32W042S1M2_M4_SERIES)
            uint32_t lpspi_intmux_offset = (uint32_t)lpspi_irq_ids[instance] - FSL_FEATURE_INTMUX_IRQ_START_INDEX;
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(INTMUX0_2_IRQn);
                INTMUX_EnableInterrupt(INTMUX0_INSTANCE, kIntmuxChannel2, 1 << lpspi_intmux_offset);
            }
            else
            {
                NVIC_DisableIRQ(INTMUX0_2_IRQn);
                INTMUX_DisableInterrupt(INTMUX0_INSTANCE, kIntmuxChannel2, 1 << lpspi_intmux_offset);
            }
#endif // #if !defined(K32W042S1M2_M4_SERIES)
            break;
#endif // #if (FSL_FEATURE_SOC_LPSPI_COUNT > 2) && FSL_FEATURE_SOC_INTMUX_COUNT
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
