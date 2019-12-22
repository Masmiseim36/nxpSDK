/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

extern uint32_t __data_section_table;
extern uint32_t __data_section_table_end;   //before BSS
extern uint32_t __safety_ram_section_table;
extern uint32_t __safety_ram_section_table_end;
extern uint32_t __bss_section_table;
extern uint32_t __bss_section_table_end;

__attribute__ ((used,section(".FlashConfig"))) const struct {
    uint32_t word1;
    uint32_t word2;
    uint32_t word3;
    uint32_t word4;
} Flash_Config = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE};

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

    uint32_t *SectionTableAddr;
	/* Load base address of Global Section Table*/
	SectionTableAddr = &__data_section_table; /*This variables point to   Global Section Table defined in Safety_common_XX_Debug.ld, due to this, we will increment this pointer alter*/
	/* This different approach is due to linker. We know only start, load  and end address of each section in linker. See "Safety_common_XX_Debug.ld  On lines 26 - 40.
    * This is different from IAR linker, where we know start and end address of whole DATA section. in GCC linker we have for each region own section DATA and BSS, as you can see in files **_Debug.ld*/

    /* Copy the data sections from flash to SRAM.*/
	while (SectionTableAddr < &__data_section_table_end)
	{
		data_rom = (uint32_t *)*SectionTableAddr++; /*Load address*/
		data_ram = (uint32_t *)*SectionTableAddr++; /*Address*/
		SectionLen = *SectionTableAddr++; /* Length - this case in BYTES*/
		/*This wihle cycle ensures to read all region of data section, all data sections are described in **_Debug.ld file*/
		for (uint32_t loop = 0; loop < SectionLen; loop = loop + 4)
        {
            *data_ram++ = *data_rom++;
#if WATCHDOG_ENABLED
            Watchdog_refresh;
#endif
        }
    }

    /* At this point, SectionTableAddr = &__safety_ram_section_table;*/
    /* Copy the safety_ram section from flash to SRAM.*/
    while (SectionTableAddr < &__safety_ram_section_table_end)
    {
        data_rom = (uint32_t *)*SectionTableAddr++; /*Load address*/
        data_ram = (uint32_t *)*SectionTableAddr++; /*Address*/
        SectionLen = *SectionTableAddr++; /* Length - this case in BYTES*/
        /*This wihle cycle ensures to read all region of data section, all data sections are described in **_Debug.ld file*/
        for (uint32_t loop = 0; loop < SectionLen; loop = loop + 4)
        {
            *data_ram++ = *data_rom++;
#if WATCHDOG_ENABLED
            Watchdog_refresh;
#endif
        }
    }

    /* Get the addresses for the .bss section (zero-initialized data) */
	uint32_t *bss_start;

	/* At this point, SectionTableAddr = &__bss_section_table;*/
	/* This different approach due to linker. We know only start and end adress of each section in linker. See "Safety_common_XX_Debug.ld */
	/* Zero fill the bss segment*/
	while (SectionTableAddr < &__bss_section_table_end) {
		bss_start = (uint32_t *)*SectionTableAddr++;
		SectionLen = *SectionTableAddr++;
		for (uint32_t loop = 0; loop < SectionLen; loop = loop + 4)
		{	*bss_start++ = 0;
#if WATCHDOG_ENABLED
        Watchdog_refresh;
#endif
        }
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
    uint32_t *pVTOR = (uint32_t* )0xE000ED08;
    *pVTOR = vtor;
}
