/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_config.h"
#include "bootloader/bl_peripheral_interface.h"
#include "fsl_device_registers.h"
#include "fsl_lpi2c.h"
#include "bootloader_common.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static const IRQn_Type lpi2c_irq_ids[FSL_FEATURE_SOC_LPI2C_COUNT] = {
    LPI2C0_IRQn,
#if (FSL_FEATURE_SOC_LPI2C_COUNT > 1)
    LPI2C1_IRQn,
#endif // #if (FSL_FEATURE_SOC_LPI2C_COUNT > 1)
#if (FSL_FEATURE_SOC_LPI2C_COUNT > 2)
    LPI2C2_IRQn,
#endif // #if (FSL_FEATURE_SOC_LPI2C_COUNT > 1)
};

void LPI2C_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        case 0:
#if (FSL_FEATURE_SOC_LPI2C_COUNT > 1)
        case 1:
#endif // #if (LPI2C_INSTANCE_COUNT > 1)
#if (FSL_FEATURE_SOC_LPI2C_COUNT5 > 2)
        case 2:
#endif // #if (LPI2C_INSTANCE_COUNT > 1)
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(lpi2c_irq_ids[instance]);
            }
            else
            {
                NVIC_DisableIRQ(lpi2c_irq_ids[instance]);
            }
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
