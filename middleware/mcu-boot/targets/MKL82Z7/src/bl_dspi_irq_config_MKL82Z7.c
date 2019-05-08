/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h" // needed for NULL
#include "bootloader_config.h"
#include "target_config.h"
#include "bootloader/bl_irq_common.h"
#include "fsl_device_registers.h"
#include "fsl_dspi.h"
#include "fsl_intmux.h"
#include "utilities/fsl_assert.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static const IRQn_Type dspi_irq_ids[FSL_FEATURE_SOC_DSPI_COUNT] = {
    SPI0_IRQn, SPI1_IRQn,
};

////////////////////////////////////////////////////////////////////////////////
// Codes
////////////////////////////////////////////////////////////////////////////////
void DSPI_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        // Note: DSPI0 is not supported in KL80 ROM.
        case 1:
        {
            int32_t intmux_offset = (uint32_t)dspi_irq_ids[1] - FSL_FEATURE_INTMUX_IRQ_START_INDEX;
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(INTMUX0_2_IRQn);
                INTMUX0->CHANNEL[2].CHn_IER_31_0 |= 1 << (intmux_offset);
            }
            else
            {
                NVIC_DisableIRQ(INTMUX0_2_IRQn);
                INTMUX0->CHANNEL[2].CHn_IER_31_0 &= ~(1 << (intmux_offset));
            }
        }
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
