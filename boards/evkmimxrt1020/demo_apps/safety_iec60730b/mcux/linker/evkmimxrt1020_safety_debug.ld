/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*     
 *     Processors:          MIMXRT1021CAF4A
 *                          MIMXRT1021CAG4A
 *                          MIMXRT1021DAF5A
 *                          MIMXRT1021DAG5A
 */

INCLUDE "evkmimxrt1020_safety_library.ld"
INCLUDE "evkmimxrt1020_safety_memory.ld"

__VECTOR_TABLE = ROM_interrupts_start;

ENTRY(ResetISR)

SECTIONS
{
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

    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        /**** IVT ****/
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /**** IVT ****/
        
        /**** Reserved space for CRC calculated by srec_cat ****/
        . = ALIGN(4);
        FILL(0xff)
        . = (__FlashCRC_end__ - ROM_interrupts_start); /* Get the relative offset from 0x60002000 (should be 0x410) */
        /**** Reserved space for CRC calculated by srec_cat ****/

        /**** Reserved space for PC test object ****/
        . = ALIGN(4);
        *iec60730b_cm4_cm7_pc_object.o
        . = ALIGN(4);
        /**** Reserved space for PC test object ****/

        /* Global Section Table */
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        __data_section_table_end = .;
        __safety_ram_section_table = .;
        LONG(LOADADDR(.safety_ram));
        LONG(    ADDR(.safety_ram));
        LONG(  SIZEOF(.safety_ram));
        __safety_ram_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)       

    } >BOARD_FLASH

    .text : ALIGN(4)
    {
        *EXCLUDE_FILE(*iec60730b_cm4_cm7_pc_object.o)
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > BOARD_FLASH
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
/************************************************************************/    
    /* DATA section for SRAM_DTC */
    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$SRAM_DTC)
        *(.data.$RAM3*)
        *(.data.$SRAM_DTC*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
     } > SRAM_DTC AT>BOARD_FLASH

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
       _data = . ;
       *(vtable) 
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > SRAM_DTC AT>BOARD_FLASH
    
/************************************************************************/    
    /* BSS section for SRAM_DTC */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       *(.bss.$RAM3*)
       *(.bss.$SRAM_DTC*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
    } > SRAM_DTC 
    
/************************************************************************/    
    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > SRAM_DTC
    
/************************************************************************/    
    /* NOINIT section for SRAM_DTC */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM3*)
       *(.noinit.$SRAM_DTC*)
       . = ALIGN(4) ;
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
   _safety_ram = .;
   *(.safety_ram*)
   . = ALIGN(4);
   _end_safety_ram = .;
   } > SRAM_DTC AT>BOARD_FLASH 
    
/************************************************************************/    
 
/************************************************************************/
    .stack __BOOT_STACK_ADDRESS - __size_cstack__ :  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);       
        _vStackTop = . + __size_cstack__;
    } > SRAM_DTC
}
