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
#include "fsl_intmux.h"
#include "fsl_lpuart.h"
#include "utilities/fsl_assert.h"

static const IRQn_Type lpuart_irq_ids[FSL_FEATURE_SOC_LPUART_COUNT] = {
    LPUART0_IRQn, LPUART1_IRQn, LPUART2_IRQn,
};

void LPUART_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        case 2:
        {
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(INTMUX0_0_IRQn);
                INTMUX0->CHANNEL[0].CHn_IER_31_0 |= 1 << (lpuart_irq_ids[instance]);
            }
            else
            {
                NVIC_DisableIRQ(INTMUX0_0_IRQn);
                INTMUX0->CHANNEL[0].CHn_IER_31_0 &= ~(1 << (lpuart_irq_ids[instance]));
            }
        }
        break;
        case 0:
        case 1:
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(lpuart_irq_ids[instance]);
            }
            else
            {
                NVIC_DisableIRQ(lpuart_irq_ids[instance]);
            }
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
