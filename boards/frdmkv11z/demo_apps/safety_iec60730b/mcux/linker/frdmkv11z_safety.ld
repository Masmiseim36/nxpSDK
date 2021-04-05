/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

GROUP (
  "libcr_semihost_nf.a"
  "libcr_c.a"
  "libcr_eabihelpers.a"
  "libgcc.a"
)

/**************************************************************/
/*    |            | --> data region                          */
/*    |            |                                          */
/*    |____________|                                          */
/*    |____________| --> STACK_TEST_P_1     ....ADR           */
/*    |____________|                        ....ADR + 0x4     */
/*    |____________|                        ....ADR + 0x8     */
/*    |____________| --> STACK_TEST_P_2     ....ADR + 0xC     */
/*    |            |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |   STACK    |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |____________| --> __BOOT_STACK_ADDRESS                 */
/*    |____________| --> STACK_TEST_P_3                       */
/*    |____________|                                          */
/*    |____________|                                          */
/*    |____________| --> STACK_TEST_P_4                       */
/*    |____________| --> SAFETY_ERROR_CODE                    */
/*    |____________| --> PC_test_flag                         */
/*    |            | --> WD_TEST_BACKUP                       */
/*    |____________|                                          */
/*    |            |                                          */
/*    |            | --> RAM_TEST_BACKUP                      */
/*    |____________|                                          */
/**************************************************************/

/* FLASH memory boundaries. */
__ROM_start__     = 0x00000000;
__ROM_end__       = 0x0001DFFF;

/* RAM memory boundaries. */
__RAM_start__     = 0x1FFFF410;
__RAM_end__       = 0x20002EE7;

/* Sizes of objects in RAM. */
__size_cstack__       = 0x300;  /* Stack size. */
stack_test_block_size = 0x10;   /* Safety stack test pattern. */
ram_test_backup_size  = 0x20;   /* Safety RAM test backup size. */
wd_test_backup_size   = 0x20;   /* Safety WDOG test data size. */


/* Sizes of objects in FLASH. */
__vector_table_size__ = 0x400;
__PC_test_size        = 0x20;
__size_flash_crc__    = 0x10;
__flash_cfg_size      = 0x10;

__VECTOR_TABLE   = __ROM_start__;
__size_heap__    = 0x40;     /* 2x  heap and heap2stackfill */

/******************************************************************************/
/******************     SYMBOLS     *******************************************/
/******************************************************************************/
/* Assemble RAM addresses. */
m_ram_test_backup     = (__RAM_end__ - ram_test_backup_size + 0x1);
m_wd_test_backup      = (m_ram_test_backup - wd_test_backup_size);
m_pc_test_flag        = (m_wd_test_backup - 0x4);
m_safety_error_code   = (m_pc_test_flag - 0x4);
m_stack_test_p_4      = (m_safety_error_code - 0x4);
m_stack_test_p_3      = (m_stack_test_p_4 - stack_test_block_size +0x4);
__BOOT_STACK_ADDRESS  = (m_stack_test_p_3 - 0x4);
m_stack_test_p_2      = (__BOOT_STACK_ADDRESS - __size_cstack__);
m_stack_test_p_1      = (m_stack_test_p_2 - stack_test_block_size + 0x4);

m_safety_ram_start   =  __RAM_start__;

/* Assemble FLASH addresses. */
m_intvec_table_start = (__ROM_start__);
m_intvec_table_end   = (m_intvec_table_start  + __vector_table_size__ - 0x1);
m_flash_config_start = (m_intvec_table_end    + 0x1);
m_flash_config_end   = (m_flash_config_start  + __flash_cfg_size - 0x1);
__PC_test_start__    = (m_flash_config_end    + 0x1); 
__PC_test_end__      = (__PC_test_start__ + __PC_test_size - 0x1);
m_flash_start        = (__PC_test_end__   + 0x1);

m_fs_flash_crc_end   = (__ROM_end__);
m_fs_flash_crc_start = (m_fs_flash_crc_end - __size_flash_crc__ + 0x1);
m_flash_end          = (m_fs_flash_crc_start - 0x1);

MEMORY
{
  /* Define each memory region */
  MEM_FLASH (rx) : ORIGIN = __ROM_start__, LENGTH = (__ROM_end__ - __ROM_start__ + 1)
  MEM_RAM  (rwx) : ORIGIN = __RAM_start__, LENGTH = (__RAM_end__ - __RAM_start__ + 1)
}
  

ENTRY(ResetISR)

SECTIONS
{
    /* Safety-related code and read-only data section. */
    .SEC_FS_ROM : ALIGN(4)
    {
        FILL(0xff)

        /* The interrupt vector table. */
        . = m_intvec_table_start;
        KEEP(*(.intvec*))

        /* Flash configuration table. */
        . = m_flash_config_start;
        KEEP(*(.flshcfg*))

        /* PC test object. */
        . = __PC_test_start__;
        KEEP(*iec60730b_cm0_pc_object.o(.text*))

        /* Safety-related FLASH code and RO data. */
        . = m_flash_start;
        *(.rodata*)
        . = . + 1;
        . = ALIGN(4);
    } >MEM_FLASH

    /* The safety-related RAM. */
    .SEC_FS_RAM m_safety_ram_start : AT (ADDR(.SEC_FS_ROM) + SIZEOF(.SEC_FS_ROM))
    {
    	m_sec_fs_ram_load_start = LOADADDR(.SEC_FS_RAM);
    	m_sec_fs_ram_start = .;
    	*(.safety_ram*)
        *main.o(.data*)
        *safety_test_items.o(.data*)
        . = . + 1;
        . = ALIGN(4);
        m_sec_fs_ram_load_end = LOADADDR (.SEC_FS_RAM) + SIZEOF(.SEC_FS_RAM);
        m_sec_fs_ram_end = .;

        /* The end of safety-related FLASH memory. */
        m_safety_flash_end = LOADADDR (.SEC_FS_RAM) + SIZEOF(.SEC_FS_RAM);
    } >MEM_RAM

    /* The non-safety RW data. */
    .SEC_RWRAM m_sec_fs_ram_end : AT (m_safety_flash_end)
    {
       m_sec_rwram_load_start = LOADADDR(.SEC_RWRAM);
       m_sec_rwram_start = .;
       *(.data*)
       . = . + 1;
       . = ALIGN(4);
       m_sec_rwram_load_end = LOADADDR(.SEC_RWRAM) + SIZEOF(.SEC_RWRAM);
       m_sec_rwram_end = .;
    } >MEM_RAM

    /* The non-safety code and RO data. */
    .SEC_ROM m_sec_rwram_load_end : ALIGN(4)
    {
        FILL(0xff)
        *(.text*)
        KEEP(*(.rodata .rodata.* .constdata .constdata.*))
        . = . + 1;
        . = ALIGN(4);
    } >MEM_FLASH

    /* The safety FLASH CRC. */
    .SEC_CRC m_fs_flash_crc_start : ALIGN(4)
    {
        FILL(0xff)
        KEEP(*(.flshcrc*))
    } >MEM_FLASH

    /* Stack memory. */
    stack (__BOOT_STACK_ADDRESS - __size_cstack__) : ALIGN(4)
    {
        . = ALIGN(4);
    } > MEM_RAM

    /* The zero-initialized RW data. */
    .SEC_BSS m_sec_rwram_end : ALIGN(4)
    {
       m_sec_bss_start = .;
       *(.bss*)
       *(COMMON)
       . = . + 1;
       . = ALIGN(4);
       m_sec_bss_end = .;
    } >MEM_RAM
    
    /* Reserve and place Heap within memory map */
    _HeapSize = __size_heap__;
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > MEM_RAM
    
	
}
