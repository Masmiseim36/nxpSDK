/*
 * Copyright 2019 NXP.
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
    /* Set the IVT address in SCB */
    extern uint32_t __VECTOR_TABLE[];
    write_vtor((uint32_t)__VECTOR_TABLE);

#if (__FPU_PRESENT)
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access */
#endif

    /* Declare a counter we'll use in all of the copy loops */
    uint32_t  SectionLen;
    uint32_t *data_ram;
    uint32_t *data_rom;
    uint32_t *data_rom_end;
    
    /* Get the addresses for the .data section (initialized data section) */
    data_ram = __section_begin(".data");
    data_rom = __section_begin(".data_init");
    data_rom_end = __section_end(".data_init");

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
    data_ram = __section_begin(".safety_ram");
    data_rom = __section_begin(".safety_ram_init");
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
    bss_end = __section_end(".bss");

    SectionLen  = bss_end - bss_start;
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
    *pVTOR = vtor;
}
