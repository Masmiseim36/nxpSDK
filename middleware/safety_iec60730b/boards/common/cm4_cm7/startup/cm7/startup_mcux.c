/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The initialized part of non-safety RW data section SEC_RWRAM. */
extern uint32_t m_sec_rwram_load_start;
extern uint32_t m_sec_rwram_load_end;
extern uint32_t m_sec_rwram_start;

/* The zero-initialized part of non-safety RW data section SEC_RWRAM. */
extern uint32_t m_sec_bss_start;
extern uint32_t m_sec_bss_end;

/* The safety-related RW data section SEC_BSS. */
extern uint32_t m_sec_fs_ram_load_start;
extern uint32_t m_sec_fs_ram_load_end;
extern uint32_t m_sec_fs_ram_start;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void write_vtor(int32_t);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Initialization of RAM sections.
 *
 * @param void
 *
 * @return None
 */
void common_startup(void)
{
#ifndef _MKV58F24_H_
    /* Set the IVT address in SCB */
    extern uint32_t __VECTOR_TABLE[];
    write_vtor((uint32_t)__VECTOR_TABLE);
#endif

#if (__FPU_PRESENT)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access */
#endif

    uint32_t ui32SrcAddr;  /* Address of the source memory. */
    uint32_t ui32EndAddr;  /* End of copied memory. */
    uint32_t ui32DestAddr; /* Address of the destination memory. */

    /* Get the addresses for the SEC_RWRAM section (initialized data section). */
    ui32SrcAddr  = (uint32_t)&m_sec_rwram_load_start;
    ui32EndAddr  = (uint32_t)&m_sec_rwram_load_end;
    ui32DestAddr = (uint32_t)&m_sec_rwram_start;

    /* Copy initialized data from ROM to RAM. */
    while (ui32SrcAddr < ui32EndAddr)
    {
        /* Copy one byte. */
        *((uint8_t *)ui32DestAddr) = *((uint8_t *)ui32SrcAddr);

        /* Increment the destination and source pointers. */
        ui32DestAddr++;
        ui32SrcAddr++;

#if WATCHDOG_ENABLED
        Watchdog_refresh;
#endif
    }

    /* Get the addresses for the SEC_FS_RAM section (initialized .safety_ram
         section). */
    ui32SrcAddr  = (uint32_t)&m_sec_fs_ram_load_start;
    ui32EndAddr  = (uint32_t)&m_sec_fs_ram_load_end;
    ui32DestAddr = (uint32_t)&m_sec_fs_ram_start;

    /* Copy initialized data from ROM to RAM. */
    while (ui32SrcAddr < ui32EndAddr)
    {
        /* Copy one byte. */
        *((uint8_t *)ui32DestAddr) = *((uint8_t *)ui32SrcAddr);

        /* Increment the destination and source pointers. */
        ui32DestAddr++;
        ui32SrcAddr++;

#if WATCHDOG_ENABLED
        Watchdog_refresh;
#endif
    }

    /* Get the addresses for the SEC_BSS section (zero-initialized data). */
    ui32DestAddr = (uint32_t)&m_sec_bss_start;
    ui32EndAddr  = (uint32_t)&m_sec_bss_end;

    /* Reset the .bss section RAM. */
    while (ui32DestAddr < ui32EndAddr)
    {
        /* Clear one byte. */
        *((uint8_t *)ui32DestAddr) = 0U;

        /* Increment the destination pointer. */
        ui32DestAddr++;
#if WATCHDOG_ENABLED
        Watchdog_refresh;
#endif
    }

#if WATCHDOG_ENABLED
    Watchdog_refresh;
#endif
}

/*!
 * @brief write to VTOR register
 *
 * @param void
 *
 * @return None
 */
void write_vtor(int32_t vtor)
{
    uint32_t *pVTOR = (uint32_t *)0xE000ED08;
    *pVTOR          = vtor;
}
