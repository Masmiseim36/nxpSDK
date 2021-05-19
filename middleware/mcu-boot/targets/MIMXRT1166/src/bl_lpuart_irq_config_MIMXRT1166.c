/*
 * Copyright 2016-2017 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "autobaud.h"
#include "bl_context.h"
#include "bl_irq_common.h"
#include "bootloader_common.h"
#include "bootloader_config.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fsl_lpuart.h"
#include "serial_packet.h"

void LPUART_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    if (set == kPeripheralEnableIRQ)
    {
        NVIC_EnableIRQ(LPUART1_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(LPUART1_IRQn);
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
