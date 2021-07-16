/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".safety_ram"
#pragma section = ".safety_ram_init"
#pragma section = ".bss"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void common_startup(void);
void write_vtor(uint32_t);

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
    /* Set the IVT address in SCB */
    extern uint32_t __VECTOR_TABLE[];
    write_vtor((uint32_t)__VECTOR_TABLE);

#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Secure mode */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    SCB_NS->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Non-secure mode */
#endif                                                    /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
#endif                                                    /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    /* Declare a counter we'll use in all of the copy loops */
    uint32_t SectionLen;
    uint32_t *data_ram;
    uint32_t *data_rom;
    uint32_t *data_rom_end;

    /* Get the addresses for the .data section (initialized data section) */
    data_ram     = __section_begin(".data");
    data_rom     = __section_begin(".data_init");
    data_rom_end = __section_end(".data_init");

#if WATCHDOG_ENABLED
    Watchdog_refresh;
#endif

    /* Copy initialized data from ROM to RAM */
    SectionLen = data_rom_end - data_rom; /* This case in number of adress*/
    while (SectionLen--)
    {
        *data_ram++ = *data_rom++;
#if WATCHDOG_ENABLED
        Watchdog_refresh;
#endif
    }

    /* Get the addresses for the .safety_ram section (initialized safety_ram section) */
    data_ram     = __section_begin(".safety_ram");
    data_rom     = __section_begin(".safety_ram_init");
    data_rom_end = __section_end(".safety_ram_init");
    /* Copy initialized data from ROM to RAM */
    SectionLen = data_rom_end - data_rom; /* This case in number of adress*/
    while (SectionLen--)
    {
        *data_ram++ = *data_rom++;
#if WATCHDOG_ENABLED
        Watchdog_refresh;
#endif
    }

    /* Get the addresses for the .bss section (zero-initialized data) */
    uint32_t *bss_start, *bss_end;

    bss_start = __section_begin(".bss");
    bss_end   = __section_end(".bss");

    SectionLen = bss_end - bss_start;
    while (SectionLen--)
    {
        *bss_start++ = 0;
#if WATCHDOG_ENABLED
        Watchdog_refresh;
#endif
    }

#if WATCHDOG_ENABLED
    Watchdog_refresh;
#endif

#if defined(MECC1) || defined(MECC2)
    /* When ECC is enabled, SRC->SRSR need to be cleared since only correct SRSR value can trigger ROM ECC preload
       procedure. Save SRSR to SRC->GPR[10] so that application can still check SRSR value from SRC->GPR[10]. */
    SRC->GPR[10] = SRC->SRSR;
    /* clear SRSR */
    SRC->SRSR = 0xFFFFFFFF;

    /* Enable entry to thread mode when divide by zero */
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
    __DSB();
    __ISB();
#endif
}

/*!
 * @brief write to VTOR register
 *
 * @param void
 *
 * @return None
 */
void write_vtor(uint32_t vtor)
{
    uint32_t *pVTOR = (uint32_t *)0xE000ED08;
    *pVTOR          = vtor;
}
