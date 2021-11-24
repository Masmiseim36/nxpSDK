/*
** ###################################################################
**     Processors:          MIMXRT117HAVM8A_cm7
**                          MIMXRT117HCVM8A_cm7
**                          MIMXRT117HDVMAA_cm7
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT1170RM, Rev 1, 02/2021
**     Version:             rev. 1.0, 2020-12-29
**     Build:               b210414
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2021 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x1000;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x1000;

/* Specify the memory areas */
MEMORY
{
  /* Define each memory region */
  BOARD_FLASH (rx) : ORIGIN = 0x30200000, LENGTH = 0x600000 /* 6M bytes (alias Flash) */  
  BOARD_SDRAM (rwx) : ORIGIN = 0x80000000, LENGTH = 0xA00000 /* 10M bytes (alias RAM) */  
  SRAM_DTC_cm7 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x40000 /* 256K bytes (alias RAM2) */  
  SRAM_ITC_cm7 (rwx) : ORIGIN = 0x0, LENGTH = 0x40000 /* 256K bytes (alias RAM3) */  
  NCACHE_REGION (rwx) : ORIGIN = 0x80A00000, LENGTH = 0x600000 /* 6M bytes (alias RAM4) */  
  SRAM_OCRAM_CACHED (rwx) : ORIGIN = 0x20240000, LENGTH = 0x100000 /* 1M bytes (alias RAM5) */  
  SRAM_OCRAM_NCACHED (rwx) : ORIGIN = 0x20340000, LENGTH = 0x40000 /* 256K bytes (alias RAM6) */  
}

  /* Define a symbol for the top of each memory region */
  __base_BOARD_FLASH = 0x30200000  ; /* BOARD_FLASH */  
  __base_Flash = 0x30200000 ; /* Flash */  
  __top_BOARD_FLASH = 0x30200000 + 0x600000 ; /* 6M bytes */  
  __top_Flash = 0x30200000 + 0x600000 ; /* 6M bytes */  
  __base_BOARD_SDRAM = 0x80000000  ; /* BOARD_SDRAM */  
  __base_RAM = 0x80000000 ; /* RAM */  
  __top_BOARD_SDRAM = 0x80000000 + 0xA00000 ; /* 10M bytes */  
  __top_RAM = 0x80000000 + 0xA00000 ; /* 10M bytes */  
  __base_SRAM_DTC_cm7 = 0x20000000  ; /* SRAM_DTC_cm7 */  
  __base_RAM2 = 0x20000000 ; /* RAM2 */  
  __top_SRAM_DTC_cm7 = 0x20000000 + 0x40000 ; /* 256K bytes */  
  __top_RAM2 = 0x20000000 + 0x40000 ; /* 256K bytes */  
  __base_SRAM_ITC_cm7 = 0x0  ; /* SRAM_ITC_cm7 */  
  __base_RAM3 = 0x0 ; /* RAM3 */  
  __top_SRAM_ITC_cm7 = 0x0 + 0x40000 ; /* 256K bytes */  
  __top_RAM3 = 0x0 + 0x40000 ; /* 256K bytes */  
  __base_NCACHE_REGION = 0x80A00000  ; /* NCACHE_REGION */  
  __base_RAM4 = 0x80A00000 ; /* RAM4 */  
  __top_NCACHE_REGION = 0x80A00000 + 0x600000 ; /* 6M bytes */  
  __top_RAM4 = 0x80A00000 + 0x600000 ; /* 6M bytes */  
  __base_SRAM_OCRAM_CACHED = 0x20240000  ; /* SRAM_OCRAM_CACHED */  
  __base_RAM5 = 0x20240000 ; /* RAM5 */  
  __top_SRAM_OCRAM_CACHED = 0x20240000 + 0x100000 ; /* 1M bytes */  
  __top_RAM5 = 0x20240000 + 0x100000 ; /* 1M bytes */  
  __base_SRAM_OCRAM_NCACHED = 0x20340000  ; /* SRAM_OCRAM_NCACHED */  
  __base_RAM6 = 0x20340000 ; /* RAM6 */  
  __top_SRAM_OCRAM_NCACHED = 0x20340000 + 0x40000 ; /* 256K bytes */  
  __top_RAM6 = 0x20340000 + 0x40000 ; /* 256K bytes */  

SECTIONS
{
     /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        LONG(LOADADDR(.data_RAM4));
        LONG(    ADDR(.data_RAM4));
        LONG(  SIZEOF(.data_RAM4));
        LONG(LOADADDR(.data_RAM5));
        LONG(    ADDR(.data_RAM5));
        LONG(  SIZEOF(.data_RAM5));
        LONG(LOADADDR(.data_RAM6));
        LONG(    ADDR(.data_RAM6));
        LONG(  SIZEOF(.data_RAM6));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        LONG(    ADDR(.bss_RAM4));
        LONG(  SIZEOF(.bss_RAM4));
        LONG(    ADDR(.bss_RAM5));
        LONG(  SIZEOF(.bss_RAM5));
        LONG(    ADDR(.bss_RAM6));
        LONG(  SIZEOF(.bss_RAM6));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

    } > BOARD_FLASH

    .text : ALIGN(4)
    {
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

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > BOARD_FLASH
 
    _etext = .;
        
    /* DATA section for SRAM_DTC_cm7 */

    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        PROVIDE(__start_data_SRAM_DTC_cm7 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_DTC_cm7)
        *(NonCacheable.init)
         . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
        PROVIDE(__end_data_SRAM_DTC_cm7 = .) ;
     } > SRAM_DTC_cm7 AT>BOARD_FLASH

    /* DATA section for SRAM_ITC_cm7 */

    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        PROVIDE(__start_data_SRAM_ITC_cm7 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$SRAM_ITC_cm7)
 
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
        PROVIDE(__end_data_SRAM_ITC_cm7 = .) ;
     } > SRAM_ITC_cm7 AT>BOARD_FLASH

    /* DATA section for NCACHE_REGION */

    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        PROVIDE(__start_data_NCACHE_REGION = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$NCACHE_REGION)
         . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
        PROVIDE(__end_data_NCACHE_REGION = .) ;
     } > NCACHE_REGION AT>BOARD_FLASH

    /* DATA section for SRAM_OCRAM_CACHED */

    .data_RAM5 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM5 = .) ;
        PROVIDE(__start_data_SRAM_OCRAM_CACHED = .) ;
        *(.ramfunc.$RAM5)
        *(.ramfunc.$SRAM_OCRAM_CACHED)
         . = ALIGN(4) ;
        PROVIDE(__end_data_RAM5 = .) ;
        PROVIDE(__end_data_SRAM_OCRAM_CACHED = .) ;
     } > SRAM_OCRAM_CACHED AT>BOARD_FLASH

    /* DATA section for SRAM_OCRAM_NCACHED */

    .data_RAM6 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM6 = .) ;
        PROVIDE(__start_data_SRAM_OCRAM_NCACHED = .) ;
        *(.ramfunc.$RAM6)
        *(.ramfunc.$SRAM_OCRAM_NCACHED)
         . = ALIGN(4) ;
        PROVIDE(__end_data_RAM6 = .) ;
        PROVIDE(__end_data_SRAM_OCRAM_NCACHED = .) ;
     } > SRAM_OCRAM_NCACHED AT>BOARD_FLASH

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
       . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* Main DATA section (BOARD_SDRAM) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       PROVIDE(__start_data_RAM = .) ;
       PROVIDE(__start_data_BOARD_SDRAM = .) ;
       *(vtable)
       *(.ramfunc*)
       KEEP(*(CodeQuickAccess))
       KEEP(*(DataQuickAccess))
       *(RamFunction)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_RAM = .) ;
       PROVIDE(__end_data_BOARD_SDRAM = .) ;
    } > BOARD_SDRAM AT>BOARD_FLASH

    /* BSS section for SRAM_DTC_cm7 */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       PROVIDE(__start_bss_SRAM_DTC_cm7 = .) ;
       *(NonCacheable)
       *(.bss.$RAM2)
       *(.bss.$SRAM_DTC_cm7)
       *(.bss.$RAM2.*)
       *(.bss.$SRAM_DTC_cm7.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
       PROVIDE(__end_bss_SRAM_DTC_cm7 = .) ;
    } > SRAM_DTC_cm7 AT> SRAM_DTC_cm7

    /* BSS section for SRAM_ITC_cm7 */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       PROVIDE(__start_bss_SRAM_ITC_cm7 = .) ;
       *(.bss.$RAM3)
       *(.bss.$SRAM_ITC_cm7)
       *(.bss.$RAM3.*)
       *(.bss.$SRAM_ITC_cm7.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
       PROVIDE(__end_bss_SRAM_ITC_cm7 = .) ;
    } > SRAM_ITC_cm7 AT> SRAM_ITC_cm7

    /* BSS section for NCACHE_REGION */
    .bss_RAM4 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       PROVIDE(__start_bss_NCACHE_REGION = .) ;
       *(.bss.$RAM4)
       *(.bss.$NCACHE_REGION)
       *(.bss.$RAM4.*)
       *(.bss.$NCACHE_REGION.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
       PROVIDE(__end_bss_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* BSS section for SRAM_OCRAM_CACHED */
    .bss_RAM5 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM5 = .) ;
       PROVIDE(__start_bss_SRAM_OCRAM_CACHED = .) ;
       *(.bss.$RAM5)
       *(.bss.$SRAM_OCRAM_CACHED)
       *(.bss.$RAM5.*)
       *(.bss.$SRAM_OCRAM_CACHED.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM5 = .) ;
       PROVIDE(__end_bss_SRAM_OCRAM_CACHED = .) ;
    } > SRAM_OCRAM_CACHED AT> SRAM_OCRAM_CACHED

    /* BSS section for SRAM_OCRAM_NCACHED */
    .bss_RAM6 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM6 = .) ;
       PROVIDE(__start_bss_SRAM_OCRAM_NCACHED = .) ;
       *(.bss.$RAM6)
       *(.bss.$SRAM_OCRAM_NCACHED)
       *(.bss.$RAM6.*)
       *(.bss.$SRAM_OCRAM_NCACHED.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM6 = .) ;
       PROVIDE(__end_bss_SRAM_OCRAM_NCACHED = .) ;
    } > SRAM_OCRAM_NCACHED AT> SRAM_OCRAM_NCACHED

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .) ;
        PROVIDE(__start_bss_BOARD_SDRAM = .) ;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(__end_bss_RAM = .) ;
        PROVIDE(__end_bss_BOARD_SDRAM = .) ;
        PROVIDE(end = .);
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* NOINIT section for SRAM_DTC_cm7 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM2 = .) ;
       PROVIDE(__start_noinit_SRAM_DTC_cm7 = .) ;
       *(.noinit.$RAM2)
       *(.noinit.$SRAM_DTC_cm7)
       *(.noinit.$RAM2.*)
       *(.noinit.$SRAM_DTC_cm7.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM2 = .) ;
       PROVIDE(__end_noinit_SRAM_DTC_cm7 = .) ;
    } > SRAM_DTC_cm7 AT> SRAM_DTC_cm7

    /* NOINIT section for SRAM_ITC_cm7 */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM3 = .) ;
       PROVIDE(__start_noinit_SRAM_ITC_cm7 = .) ;
       *(.noinit.$RAM3)
       *(.noinit.$SRAM_ITC_cm7)
       *(.noinit.$RAM3.*)
       *(.noinit.$SRAM_ITC_cm7.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM3 = .) ;
       PROVIDE(__end_noinit_SRAM_ITC_cm7 = .) ;
    } > SRAM_ITC_cm7 AT> SRAM_ITC_cm7

    /* NOINIT section for NCACHE_REGION */
    .noinit_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM4 = .) ;
       PROVIDE(__start_noinit_NCACHE_REGION = .) ;
       *(.noinit.$RAM4)
       *(.noinit.$NCACHE_REGION)
       *(.noinit.$RAM4.*)
       *(.noinit.$NCACHE_REGION.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM4 = .) ;
       PROVIDE(__end_noinit_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* NOINIT section for SRAM_OCRAM_CACHED */
    .noinit_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM5 = .) ;
       PROVIDE(__start_noinit_SRAM_OCRAM_CACHED = .) ;
       *(.noinit.$RAM5)
       *(.noinit.$SRAM_OCRAM_CACHED)
       *(.noinit.$RAM5.*)
       *(.noinit.$SRAM_OCRAM_CACHED.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM5 = .) ;
       PROVIDE(__end_noinit_SRAM_OCRAM_CACHED = .) ;
    } > SRAM_OCRAM_CACHED AT> SRAM_OCRAM_CACHED

    /* NOINIT section for SRAM_OCRAM_NCACHED */
    .noinit_RAM6 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM6 = .) ;
       PROVIDE(__start_noinit_SRAM_OCRAM_NCACHED = .) ;
       *(.noinit.$RAM6)
       *(.noinit.$SRAM_OCRAM_NCACHED)
       *(.noinit.$RAM6.*)
       *(.noinit.$SRAM_OCRAM_NCACHED.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM6 = .) ;
       PROVIDE(__end_noinit_SRAM_OCRAM_NCACHED = .) ;
    } > SRAM_OCRAM_NCACHED AT> SRAM_OCRAM_NCACHED

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .) ;
        PROVIDE(__start_noinit_BOARD_SDRAM = .) ;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
       PROVIDE(__end_noinit_RAM = .) ;
       PROVIDE(__end_noinit_BOARD_SDRAM = .) ;        
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x1000;
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        __HeapLimit = .;
    } > SRAM_DTC_cm7

     _StackSize = 0x1000;
     /* Reserve space in memory for Stack */
    .heap2stackfill  :
    {
        . += _StackSize;
    } > SRAM_DTC_cm7

    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM_DTC_cm7) + LENGTH(SRAM_DTC_cm7) - _StackSize - 0:  ALIGN(4)
    {
        __StackBase = .;
        . = ALIGN(4);
        __StackTop = . + _StackSize;
    } > SRAM_DTC_cm7

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
    /* Provide symbols for MIMXRT1170 parts for boot header generation code
     * to set image to be plain load image or XIP.
     * Config : Plain load image = false
     */
    _boot_loadaddr = ORIGIN(BOARD_FLASH);
    _boot_size = LENGTH(BOARD_FLASH);
}
