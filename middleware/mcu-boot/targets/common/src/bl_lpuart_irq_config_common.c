/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "bootloader_config.h"
#include "bootloader/bl_irq_common.h"
#include "autobaud/autobaud.h"
#include "packet/serial_packet.h"
#include "fsl_device_registers.h"
#include "fsl_lpuart.h"
#include "utilities/fsl_assert.h"

static const IRQn_Type lpuart_irq_ids[FSL_FEATURE_SOC_LPUART_COUNT] = {
#ifdef PKE18F15_SERIES
    LPUART0_RxTx_IRQn,
#endif
#if defined(KW36Z4_SERIES) || defined (KW38A4_SERIES)
    LPUART0_LPUART1_IRQn,
#else
    LPUART0_IRQn,
#endif
#if (FSL_FEATURE_SOC_LPUART_COUNT > 1u)
#ifdef PKE18F15_SERIES
    LPUART1_RxTx_IRQn,
#endif
#if defined(KW36Z4_SERIES) || defined (KW38A4_SERIES)
    LPUART0_LPUART1_IRQn,
#else
    LPUART1_IRQn,
#endif
#endif // #if (LPUART_INSTANCE_COUNT > 1)
#if (FSL_FEATURE_SOC_LPUART_COUNT > 2u)
#ifdef PKE18F15_SERIES
    LPUART2_RxTx_IRQn,
#else
    LPUART2_IRQn,
#endif
#endif // #if (LPUART_INSTANCE_COUNT > 2)

#if (FSL_FEATURE_SOC_LPUART_COUNT > 3u)
    LPUART3_IRQn,
#endif // #if (LPUART_INSTANCE_COUNT > 3)
#if (FSL_FEATURE_SOC_LPUART_COUNT > 4u)
    LPUART4_IRQn,
#endif // #if (LPUART_INSTANCE_COUNT > 4)
};

void LPUART_SetSystemIRQ(uint32_t instance, PeripheralSystemIRQSetting set)
{
    switch (instance)
    {
        case 0u:
#if (FSL_FEATURE_SOC_LPUART_COUNT > 1)
        case 1u:
#endif // #if (LPUART_INSTANCE_COUNT > 1)
#if (FSL_FEATURE_SOC_LPUART_COUNT > 2)
        case 2u:
#endif // #if (LPUART_INSTANCE_COUNT > 2)
#if (FSL_FEATURE_SOC_LPUART_COUNT > 3)
        case 3u:
#endif // #if (LPUART_INSTANCE_COUNT > 3)
#if (FSL_FEATURE_SOC_LPUART_COUNT > 4)
        case 4u:
#endif // #if (LPUART_INSTANCE_COUNT > 4)
            if (set == kPeripheralEnableIRQ)
            {
                NVIC_EnableIRQ(lpuart_irq_ids[instance]);
            }
            else
            {
                NVIC_DisableIRQ(lpuart_irq_ids[instance]);
            }
            break;
         default:
            // doing nothing
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
