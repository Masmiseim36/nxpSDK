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
#include "fsl_lpi2c.h"
#include "bootloader_common.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static const IRQn_Type lpi2c_irq_ids[FSL_FEATURE_SOC_LPI2C_COUNT] = {
    LPI2C0_IRQn, LPI2C1_IRQn, LPI2C2_IRQn,
};

void LPI2C_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        case 2:
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(INTMUX0_1_IRQn);
                INTMUX0->CHANNEL[1].CHn_IER_31_0 |= 1 << lpi2c_irq_ids[instance];
            }
            else
            {
                NVIC_DisableIRQ(INTMUX0_1_IRQn);
                INTMUX0->CHANNEL[1].CHn_IER_31_0 &= ~(1 << lpi2c_irq_ids[instance]);
            }
            break;
        case 0:
        case 1:
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
