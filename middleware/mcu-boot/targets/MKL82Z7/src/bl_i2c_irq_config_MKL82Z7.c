/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "bootloader_config.h"
#include "target_config.h"
#include "bootloader/bl_irq_common.h"
#include "autobaud/autobaud.h"
#include "packet/serial_packet.h"
#include "fsl_device_registers.h"
#include "fsl_i2c.h"
#include "fsl_intmux.h"
#include "utilities/fsl_assert.h"

static const IRQn_Type i2c_irq_ids[FSL_FEATURE_SOC_I2C_COUNT] = {
    I2C0_IRQn, I2C1_IRQn,
};

void I2C_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        case 1:
        {
            uint32_t intmux_offset = (uint32_t)i2c_irq_ids[1] - FSL_FEATURE_INTMUX_IRQ_START_INDEX;
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(INTMUX0_1_IRQn);
                INTMUX0->CHANNEL[1].CHn_IER_31_0 |= 1 << (intmux_offset);
            }
            else
            {
                NVIC_DisableIRQ(INTMUX0_1_IRQn);
                INTMUX0->CHANNEL[1].CHn_IER_31_0 &= ~(1 << (intmux_offset));
            }
        }
        break;
// Note: I2C0 is not supported in KL80 ROM.
#if 0
    case 0:
        if (set == kPeripheralEnableIRQ)
        {
            NVIC_EnableIRQ(i2c_irq_ids[instance]);
        }
        else
        {
            NVIC_DisableIRQ(i2c_irq_ids[instance]);
        }
        break;
#endif //
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
