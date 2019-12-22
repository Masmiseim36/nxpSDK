/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2019
 * Generated linker script file for MIMXRT1062xxxxA
 * Created from library.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v11.0.0 [Build 2516] [2019-06-05] on 09-Jul-2019 12:54:00
 */

ENTRY(ResetISR)

MEMORY
{
  /* Define each memory region */
  BOARD_FLASH (rx) : ORIGIN = 0x60000000, LENGTH = 0x4000000 /* 64M bytes (alias Flash) in QSPI */
  SRAM_DTC (rwx) : ORIGIN = 0x20000000, LENGTH = 0x20000 /* 128K bytes (alias RAM) */
  SRAM_ITC (rwx) : ORIGIN = 0x0, LENGTH = 0x20000 /* 128K bytes (alias RAM2) */
  SRAM_OC (rwx) : ORIGIN = 0x20200000, LENGTH = 0xc0000 /* 768K bytes (alias RAM3) */
  BOARD_SDRAM_LOWER (rwx) : ORIGIN = 0x80000000, LENGTH = 0x01C00000 /* 30M bytes (alias RAM) */
  BOARD_SDRAM_UPPER (rwx) : ORIGIN = 0x81C00000, LENGTH = 0x00400000 /* 2M bytes (alias RAM) */
  NCACHE_REGION (rwx) : ORIGIN = 0x81e00000, LENGTH = 0x200000 /* 2M bytes (alias RAM5) */
}

  /* Define a symbol for the top of each memory region */
  __base_BOARD_FLASH = 0x60000000  ; /* BOARD_FLASH */
  __top_BOARD_FLASH = 0x60000000 + 0x4000000 ; /* 64M bytes (alias Flash) */
  __base_SRAM_ITC = 0x0  ; /* SRAM_ITC */
  __base_RAM2 = 0x0 ; /* RAM2 */
  __top_SRAM_ITC = 0x0 + 0x20000 ; /* 128K bytes */
  __top_RAM2 = 0x0 + 0x20000 ; /* 128K bytes */
  __base_SRAM_DTC = 0x20000000  ; /* SRAM_DTC */
  __base_RAM3 = 0x20000000 ; /* RAM3 */
  __top_SRAM_DTC = 0x20000000 + 0x20000 ; /* 128K bytes */
  __top_RAM3 = 0x20000000 + 0x20000 ; /* 128K bytes */
  __base_SRAM_OC = 0x20200000  ; /* SRAM_OC */
  __base_RAM4 = 0x20200000 ; /* RAM4 */
  __top_SRAM_OC = 0x20200000 + 0xc0000 ; /* 768K bytes */
  __top_RAM4 = 0x20200000 + 0xc0000 ; /* 768K bytes */
/*  __base_BOARD_SDRAM_LOWER = 0x80000000  ; /* BOARD_SDRAM_LOWER */
/*  __base_RAM = 0x80000000 ; /* RAM */
/*  __top_BOARD_SDRAM_LOWER = 0x80000000 + 0x01E00000 ; /* 30M bytes */
/*  __top_RAM = 0x80000000 + 0x01E00000 ; /* 30M bytes */
/*  __base_BOARD_SDRAM_UPPER = 0x81E00000  ; /* BOARD_SDRAM_LOWER */
/*  __top_BOARD_SDRAM_UPPER = 0x81E00000 + 0x00200000 ; /* 2M bytes */
/*  __base_BOARD_SDRAM_LOWER = 0x80000000  ; /* BOARD_SDRAM_LOWER */
  __base_RAM = 0x80000000 ; /* RAM */
  __top_BOARD_SDRAM_LOWER = 0x80000000 + 0x01C00000 ; /* 30M bytes */
  __top_RAM = 0x80000000 + 0x01C00000 ; /* 30M bytes */
  __base_BOARD_SDRAM_UPPER = 0x81C00000  ; /* BOARD_SDRAM_LOWER */
  __top_BOARD_SDRAM_UPPER = 0x81C00000 + 0x00400000 ; /* 2M bytes */
  __base_NCACHE_REGION = 0x81e00000  ; /* NCACHE_REGION */
  __top_NCACHE_REGION = 0x81e00000 + 0x200000 ; /* 2M bytes */

GROUP (
  "libgcc.a"
  "libc.a"
  "libm.a"
  "libcr_newlib_semihost.a"
)

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
    } >BOARD_FLASH

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
        LONG(LOADADDR(.data_non_cache_init));
        LONG(    ADDR(.data_non_cache_init));
        LONG(  SIZEOF(.data_non_cache_init));
        LONG(LOADADDR(.data_internal_non_cache_init));
        LONG(    ADDR(.data_internal_non_cache_init));
        LONG(  SIZEOF(.data_internal_non_cache_init));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        LONG(LOADADDR(.data_RAM4));
        LONG(    ADDR(.data_RAM4));
        LONG(  SIZEOF(.data_RAM4));
        LONG(LOADADDR(.data_BOARD_SDRAM_LOWER));
        LONG(    ADDR(.data_BOARD_SDRAM_LOWER));
        LONG(  SIZEOF(.data_BOARD_SDRAM_LOWER));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_non_cache));
        LONG(  SIZEOF(.bss_non_cache));
        LONG(    ADDR(.bss_internal_non_cache));
        LONG(  SIZEOF(.bss_internal_non_cache));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        LONG(    ADDR(.bss_RAM4));
        LONG(  SIZEOF(.bss_RAM4));
        LONG(    ADDR(.bss_BOARD_SDRAM_LOWER));
        LONG(  SIZEOF(.bss_BOARD_SDRAM_LOWER));
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

    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > BOARD_FLASH
    __exidx_end = .;

    _etext = .;

    /* DATA section for SRAM_ITC */
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_ITC)
        *(.data.$RAM2*)
        *(.data.$SRAM_ITC*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
     } > SRAM_ITC AT>BOARD_FLASH

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

    /* DATA section for SRAM_OC */
    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$SRAM_OC)
        *(.data.$RAM4*)
        *(.data.$SRAM_OC*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
     } > SRAM_OC AT>BOARD_FLASH

    /* DATA section for SDRAM_LOWER */
    .data_BOARD_SDRAM_LOWER : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_BOARD_SDRAM_LOWER = .) ;
        *(.ramfunc.$BOARD_SDRAM_LOWER)
        *(.data.$BOARD_SDRAM_LOWER)
        *(.data.$BOARD_SDRAM_LOWER.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_BOARD_SDRAM_LOWER = .) ;
     } > BOARD_SDRAM_LOWER AT>BOARD_FLASH

    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM_DTC

    /* Main DATA section (SRAM_DTC) */
    .data : ALIGN(4)
    {
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > SRAM_DTC AT>BOARD_FLASH

    /* Non-Cache default section mapped to internal SRAM (for performance use SRAM_OC or DTCM) */
    .data_internal_non_cache_init : ALIGN(4)
    {
       FILL(0xff)
       PROVIDE(__start_data_internal_non_cache = .) ;
        *(NonCacheableInternal.init)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_internal_non_cache = .) ;
    } > SRAM_OC AT>BOARD_FLASH
/*    } > SRAM_DTC AT>BOARD_FLASH */

    /* Non-Cache external DATA section (BOARD_SDRAM_UPPER) */
    .data_non_cache_init : ALIGN(4)
    {
       FILL(0xff)
       PROVIDE(__start_data_non_cache = .) ;
        *(NonCacheable.init)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_non_cache = .) ;
    } > BOARD_SDRAM_UPPER AT>BOARD_FLASH

    /* BSS section for SRAM_ITC */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       *(.bss.$RAM2*)
       *(.bss.$SRAM_ITC*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
    } > SRAM_ITC

    /* BSS section for SRAM_DTC */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       *(.bss.$RAM3*)
       *(.bss.$SRAM_DTC*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
    } > SRAM_DTC

    /* BSS section for SRAM_OC */
    .bss_RAM4 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       *(.bss.$RAM4*)
       *(.bss.$SRAM_OC*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
    } > SRAM_OC

    /* BSS section for BOARD_SDRAM */
    .bss_BOARD_SDRAM_LOWER :
    {
       . = ALIGN(4) ;
       PROVIDE(__start_bss_BOARD_SDRAM_LOWER = .) ;
       *(.bss.$BOARD_SDRAM_LOWER)
       *(.bss.$BOARD_SDRAM_LOWER.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_BOARD_SDRAM_LOWER = .) ;
    } > BOARD_SDRAM_LOWER

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(__end = .);
    } > SRAM_DTC

    /* Non-Cache BSS SECTION  - use DTCM or OCRAM for best performance*/
    .bss_internal_non_cache : ALIGN(4)
    {
        PROVIDE(__start_bss_internal_non_cache = .) ;
        *(NonCacheableInternal)
        . = ALIGN(4) ;
        PROVIDE(__end_bss_internal_non_cache = .) ;
    } > SRAM_OC
/*    } > SRAM_DTC

    /* Non-Cache BSS SECTION (default) in external memory (SDRAM)*/
    .bss_non_cache : ALIGN(4)
    {
        PROVIDE(__start_bss_non_cache = .) ;
        *(NonCacheable)
        . = ALIGN(4) ;
        PROVIDE(__end_bss_non_cache = .) ;
    } > BOARD_SDRAM_UPPER

    /* NOINIT section for SRAM_ITC */
    .noinit_RAM2 (NOLOAD) :
    {
       . = ALIGN(4) ;
       *(.noinit.$RAM2*)
       *(.noinit.$SRAM_ITC*)
       . = ALIGN(4) ;
    } > SRAM_ITC

    /* NOINIT section for SRAM_DTC */
    .noinit_RAM3 (NOLOAD) :
    {
       . = ALIGN(4) ;
       *(.noinit.$RAM3*)
       *(.noinit.$SRAM_DTC*)
       . = ALIGN(4) ;
    } > SRAM_DTC

    /* NOINIT section for SRAM_OC */
    .noinit_RAM4 (NOLOAD) :
    {
       . = ALIGN(4) ;
       *(.noinit.$RAM4*)
       *(.noinit.$SRAM_OC*)
       . = ALIGN(4) ;
    } > SRAM_OC

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD):
    {
         . = ALIGN(4) ;
        _noinit = .;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
    } > SRAM_DTC

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x1400000;
    .heap :  ALIGN(0x10000)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > BOARD_SDRAM_LOWER

     _StackSize = 0x1000;
     /* Reserve space in memory for Stack */
    .heap2stackfill  :
    {
        . += _StackSize;
    } > SRAM_DTC

    /* Locate actual Stack in internal SRAM_DTC memory map */
    .stack ORIGIN(SRAM_DTC) + LENGTH(SRAM_DTC) - _StackSize - 0:  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
    } > SRAM_DTC

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
}
