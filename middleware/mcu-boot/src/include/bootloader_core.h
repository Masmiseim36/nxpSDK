/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __BOOTLOADER_CORE_H__
#define __BOOTLOADER_CORE_H__

#include "fsl_device_registers.h"

// These functions are missing from the latest CMSIS core header files.
#if __CORTEX_M == 0x00u

/** \brief  Clear Enabled IRQs

    The function clears all device IRQs
 */
__STATIC_INLINE void NVIC_ClearEnabledIRQs(void)
{
    NVIC->ICER[0] = 0xFFFFFFFFu;
}

/** \brief  Clear All Pending Interrupts

    The function clears the pending bits of all external interrupts.

 */
__STATIC_INLINE void NVIC_ClearAllPendingIRQs(void)
{
    NVIC->ICPR[0] = 0xFFFFFFFFu;
}

#elif __CORTEX_M == 0x04u || __CORTEX_M == 0x07u

/** \brief  Clear Enabled IRQs

    The function clears all device IRQs
 */
__STATIC_INLINE void NVIC_ClearEnabledIRQs(void)
{
    NVIC->ICER[0] = 0xFFFFFFFFu;
    NVIC->ICER[1] = 0xFFFFFFFFu;
    NVIC->ICER[2] = 0xFFFFFFFFu;
    NVIC->ICER[3] = 0xFFFFFFFFu;
    NVIC->ICER[4] = 0xFFFFFFFFu;
    NVIC->ICER[5] = 0xFFFFFFFFu;
    NVIC->ICER[6] = 0xFFFFFFFFu;
    NVIC->ICER[7] = 0xFFFFFFFFu;
}

/** \brief  Clear All Pending Interrupts

    The function clears the pending bits of all external interrupts.

 */
__STATIC_INLINE void NVIC_ClearAllPendingIRQs(void)
{
    NVIC->ICPR[0] = 0xFFFFFFFFu;
    NVIC->ICPR[1] = 0xFFFFFFFFu;
    NVIC->ICPR[2] = 0xFFFFFFFFu;
    NVIC->ICPR[3] = 0xFFFFFFFFu;
    NVIC->ICPR[4] = 0xFFFFFFFFu;
    NVIC->ICPR[5] = 0xFFFFFFFFu;
    NVIC->ICPR[6] = 0xFFFFFFFFu;
    NVIC->ICPR[7] = 0xFFFFFFFFu;
}
#else
#error CORTEX_M version not defined
#endif

#endif // __BOOTLOADER_CORE_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
