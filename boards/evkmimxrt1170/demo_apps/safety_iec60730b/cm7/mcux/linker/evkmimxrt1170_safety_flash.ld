/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*     
 *     Device:              MIMXRT1170
 */

/******************************************************************************/
/******************     LIBRARY     *******************************************/
/******************************************************************************/
GROUP (
  "libcr_c.a"
  "libcr_eabihelpers.a"
)

/******************************************************************************/
/******************     USER CONFIGURATION PART     ***************************/
/******************************************************************************/
stack_test_block_size = 0x10;
ram_test_backup_size  = 0x20;
__vector_table_size__ = 0x400;
__size_cstack__       = 0x0400;

SRAM_DTC_start       = 0x20000000; /* Part of 512KB on-chip RAM (Data-Tightly-Coupled) */
SRAM_DTC_end         = 0x2000FFFF + 1;

m_flash_config_start = 0x30000000;

m_intvec_table_start = 0x30002000; /* IVT in ROM */

__PC_test_start__    = 0x30002410; /* PC object address */
__PC_test_end__      = 0x3000242F; /* PC object reserved size: 0x20 */ 

ROM_start            = 0x30002430; /* ROM memory (external ROM) */
ROM_end              = 0x303FFFFF + 1;

__NCACHE_REGION_START      = 0x202C0000;
__NCACHE_REGION_END        = 0x2033FFFF;
__NCACHE_REGION_SIZE       = 0x00000000;

/******************************************************************************/
/******************     SYMBOLS     *******************************************/
/******************************************************************************/   
m_ram_test_backup    = SRAM_DTC_end - ram_test_backup_size;
m_wd_test_backup     = m_ram_test_backup - 0x20;
m_pc_test_flag       = m_wd_test_backup - 0x4;
m_safety_error_code  = m_pc_test_flag - 0x4;
m_stack_test_p_4     = m_safety_error_code - 0x4;
m_stack_test_p_3     = m_stack_test_p_4 - stack_test_block_size +0x4;
__BOOT_STACK_ADDRESS = m_stack_test_p_3 - 0x4;
m_stack_test_p_2     = __BOOT_STACK_ADDRESS - __size_cstack__;
m_stack_test_p_1     = m_stack_test_p_2 - stack_test_block_size + 0x4;
SRAM_DTC_end_edited  = m_stack_test_p_1 - 0x1;

MEMORY
{
  /* Define each memory region */
  ROM_config (rx) : ORIGIN = m_flash_config_start, LENGTH = m_intvec_table_start - m_flash_config_start /* XIP boot header */
  BOARD_FLASH (rx) : ORIGIN = m_intvec_table_start, LENGTH = ROM_end - ROM_start /* Flash */  
  SRAM_DTC (rwx) : ORIGIN = SRAM_DTC_start, LENGTH = SRAM_DTC_end - SRAM_DTC_start /* RAM */  
}

/******************************************************************************/
/******************     PLACING     *******************************************/
/******************************************************************************/ 
__VECTOR_TABLE = m_intvec_table_start;
__Vectors = __VECTOR_TABLE; /* Not used - must be here because of CMSIS header files upgrade */

ENTRY(ResetISR)

SECTIONS
{
/************************************************************************/
    /* Image Vector Table and Boot Data for booting from external flash */
    .boot_hdr : ALIGN(4)
    {
        FILL(0xff)

        __boot_hdr_start__ = ABSOLUTE(.) ;
        KEEP(*(.boot_hdr.conf))
        . = 0x1000 ;
        KEEP(*(.boot_hdr.ivt))
        . = 0x1020 ;
        KEEP(*(.boot_hdr.boot_data))
        . = 0x1030 ;
        KEEP(*(.boot_hdr.dcd_data))
        __boot_hdr_end__ = ABSOLUTE(.) ;
        . = 0x2000 ;
    } >ROM_config
    
/************************************************************************/
    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        /**** IVT ****/
        FILL(0xff)
        KEEP(*(.intvec))
        /**** IVT ****/

        /**** Reserved space for PC test object ****/
        . = ALIGN(4);
        KEEP(*iec60730b_cm4_cm7_pc_object.o)
        . = ALIGN(4);
        /**** Reserved space for PC test object ****/    

    } >BOARD_FLASH
    
/************************************************************************/
    .text : ALIGN(4)
    {
        *EXCLUDE_FILE(*iec60730b_cm4_cm7_pc_object.o)
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > BOARD_FLASH
    
/************************************************************************/
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4) 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > BOARD_FLASH
    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > BOARD_FLASH
    __exidx_end = .;

    _etext = .;
       
/************************************************************************/
    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM_DTC
    
/************************************************************************/    
    /* Main DATA section (SRAM_DTC) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       m_sec_rwram_load_start = LOADADDR(.data);
       m_sec_rwram_start = .;
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       m_sec_rwram_load_end = LOADADDR(.data) + SIZEOF(.data);
       m_sec_rwram_end = .;
    } > SRAM_DTC AT>BOARD_FLASH
    
/************************************************************************/    
    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        m_sec_bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        m_sec_bss_end = .;
        PROVIDE(end = .);
    } > SRAM_DTC
    
/************************************************************************/    
   
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > SRAM_DTC

/************************************************************************/    
   .safety_ram : ALIGN(4)    
   {
   m_sec_fs_ram_load_start = LOADADDR(.safety_ram);
   m_sec_fs_ram_start = .;
   *(.safety_ram)
   . = ALIGN(4);
   m_sec_fs_ram_load_end = LOADADDR (.safety_ram) + SIZEOF(.safety_ram);
   m_sec_fs_ram_end = .;
   m_safety_flash_end = LOADADDR (.safety_ram) + SIZEOF(.safety_ram);
   } > SRAM_DTC AT>BOARD_FLASH
   
/************************************************************************/    
   .safety_ram_crc : ALIGN(4)    
   {
   m_sec_fs_ram_crc_start = .;
   *(.safety_ram_crc)
   . = ALIGN(4);
   m_sec_fs_ram_crc_end = .;
   } > SRAM_DTC AT>BOARD_FLASH     
 
/************************************************************************/
    .stack __BOOT_STACK_ADDRESS - __size_cstack__ :  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);       
        _vStackTop = . + __size_cstack__;
    } > SRAM_DTC

/************************************************************************/
    /* The safety FLASH CRC. */
    .SEC_CRC : ALIGN(4)
    {
        FILL(0xff)
        KEEP(*(.flshcrc*))
    } >BOARD_FLASH
}
