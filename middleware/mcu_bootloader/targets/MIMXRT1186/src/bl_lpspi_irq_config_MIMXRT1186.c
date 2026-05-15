/*
 * Copyright 2022 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_irq_common.h"
#include "bootloader_common.h"

void LPSPI_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    if (set == kPeripheralEnableIRQ)
    {
        NVIC_EnableIRQ(LPSPI1_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(LPSPI1_IRQn);
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
