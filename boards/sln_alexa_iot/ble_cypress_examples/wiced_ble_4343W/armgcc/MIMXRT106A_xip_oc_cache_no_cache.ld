/*
 * Linker Script File for MIMXRT106A builds 
 * with half (256 KB) OCRAM cacheable and half (256 KB) non-cacheable; 32 KB ITC an 480 KB DTC
 * (c) NXP Semiconductors 2013-2019
 */

/* Edit the symbols below to change memory sizes */

/* Define board flash base address and max available bytes */
BOARD_FLASH_BASE_ADDR = 0x60300000;
BOARD_FLASH_MAX_BYTES = 0xa00000;

/* Define SRAM DTC base address and max available bytes */
SRAM_DTC_BASE_ADDR = 0x20000000;
SRAM_DTC_MAX_BYTES = 0x78000;

/* Define SRAM ITC base address and max available bytes */
SRAM_ITC_BASE_ADDR = 0x0;
SRAM_ITC_MAX_BYTES = 0x8000;

/* Define SRAM OC non-cacheable base address and max available bytes */
SRAM_OC_NON_CACHEABLE_BASE_ADDR = 0x20200000;
SRAM_OC_NON_CACHEABLE_MAX_BYTES = 0x40000;

/* Define SRAM OC cacheable base address and max available bytes */
SRAM_OC_CACHEABLE_BASE_ADDR = 0x20240000;
SRAM_OC_CACHEABLE_MAX_BYTES = 0x40000;

ENTRY(Reset_Handler)

GROUP (
  "libgcc.a"
  "libc.a"
  "libm.a"
)

MEMORY
{
  /* Define each memory region */
  BOARD_FLASH (rx) : ORIGIN = BOARD_FLASH_BASE_ADDR, LENGTH = BOARD_FLASH_MAX_BYTES /* 10M bytes (alias Flash) */  
  SRAM_DTC (rwx) : ORIGIN = SRAM_DTC_BASE_ADDR, LENGTH = SRAM_DTC_MAX_BYTES /* 512K bytes (alias RAM) */  
  SRAM_ITC (rwx) : ORIGIN = SRAM_ITC_BASE_ADDR, LENGTH = SRAM_ITC_MAX_BYTES /* 0M bytes (alias RAM2) */  
  SRAM_OC_NON_CACHEABLE (rwx) : ORIGIN = SRAM_OC_NON_CACHEABLE_BASE_ADDR, LENGTH = SRAM_OC_NON_CACHEABLE_MAX_BYTES /* 256K bytes (alias RAM3) */  
  SRAM_OC_CACHEABLE (rwx) : ORIGIN = SRAM_OC_CACHEABLE_BASE_ADDR, LENGTH = SRAM_OC_CACHEABLE_MAX_BYTES /* 256K bytes (alias RAM4) */  
}

  /* Define a symbol for the top of each memory region */
  __base_BOARD_FLASH = BOARD_FLASH_BASE_ADDR  ; /* BOARD_FLASH */  
  __base_Flash = BOARD_FLASH_BASE_ADDR ; /* Flash */  
  __top_BOARD_FLASH = BOARD_FLASH_BASE_ADDR + BOARD_FLASH_MAX_BYTES ; /* 10M bytes */  
  __top_Flash = BOARD_FLASH_BASE_ADDR + BOARD_FLASH_MAX_BYTES ; /* 10M bytes */  
  __base_SRAM_DTC = SRAM_DTC_BASE_ADDR  ; /* SRAM_DTC */  
  __base_RAM = SRAM_DTC_BASE_ADDR ; /* RAM */  
  __top_SRAM_DTC = SRAM_DTC_BASE_ADDR + SRAM_DTC_MAX_BYTES ; /* 512K bytes */  
  __top_RAM = SRAM_DTC_BASE_ADDR + SRAM_DTC_MAX_BYTES ; /* 512K bytes */  
  __base_SRAM_ITC = SRAM_ITC_BASE_ADDR  ; /* SRAM_ITC */  
  __base_RAM2 = SRAM_ITC_BASE_ADDR ; /* RAM2 */  
  __top_SRAM_ITC = SRAM_ITC_BASE_ADDR + SRAM_ITC_MAX_BYTES ; /* 0M bytes */  
  __top_RAM2 = SRAM_ITC_BASE_ADDR + SRAM_ITC_MAX_BYTES ; /* 0M bytes */  
  __base_SRAM_OC_NON_CACHEABLE = SRAM_OC_NON_CACHEABLE_BASE_ADDR  ; /* SRAM_OC_NON_CACHEABLE */  
  __base_RAM3 = SRAM_OC_NON_CACHEABLE_BASE_ADDR ; /* RAM3 */  
  __top_SRAM_OC_NON_CACHEABLE = SRAM_OC_NON_CACHEABLE_BASE_ADDR + SRAM_OC_NON_CACHEABLE_MAX_BYTES ; /* 256K bytes */  
  __top_RAM3 = SRAM_OC_NON_CACHEABLE_BASE_ADDR + SRAM_OC_NON_CACHEABLE_MAX_BYTES ; /* 256K bytes */  
  __base_SRAM_OC_CACHEABLE = SRAM_OC_CACHEABLE_BASE_ADDR  ; /* SRAM_OC_CACHEABLE */  
  __base_RAM4 = SRAM_OC_CACHEABLE_BASE_ADDR ; /* RAM4 */  
  __top_SRAM_OC_CACHEABLE = SRAM_OC_CACHEABLE_BASE_ADDR + SRAM_OC_CACHEABLE_MAX_BYTES ; /* 256K bytes */  
  __top_RAM4 = SRAM_OC_CACHEABLE_BASE_ADDR + SRAM_OC_CACHEABLE_MAX_BYTES ; /* 256K bytes */  

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
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

    } > BOARD_FLASH

    .text : ALIGN(4)
    {
        *(EXCLUDE_FILE(
            /* Exclude flash and frequently executed functions from XIP */
            */flexspi_hyper_flash_ops.c.obj
            */fsl_flexspi.c.obj
            */sln_flash.c.obj
        ) .text)                 /* .text sections (code) */
        *(EXCLUDE_FILE(
            /* Exclude flash and frequently executed functions from XIP */
            */flexspi_hyper_flash_ops.c.obj
            */fsl_flexspi.c.obj
            */sln_flash.c.obj
        ) .text*)                /* .text* sections (code) */
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
        *(.vectorTableRam)
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_ITC)
        /* Explicit placement of flash and frequently executed functions in RAM */
        */flexspi_hyper_flash_ops.c.obj(.text*)
        */fsl_flexspi.c.obj(.text*)
        */sln_flash.c.obj(.text*)
        *(.data.$RAM2*)
        *(.data.$SRAM_ITC*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
     } > SRAM_ITC AT>BOARD_FLASH
    /* DATA section for SRAM_OC_NON_CACHEABLE */

    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$SRAM_OC_NON_CACHEABLE)
       *(.ocram_non_cacheable_data)
        *(.data.$RAM3*)
        *(.data.$SRAM_OC_NON_CACHEABLE*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
     } > SRAM_OC_NON_CACHEABLE AT>BOARD_FLASH
    /* DATA section for SRAM_OC_CACHEABLE */

    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$SRAM_OC_CACHEABLE)
       *(.ocram_cacheable_data)
        *(.data.$RAM4*)
        *(.data.$SRAM_OC_CACHEABLE*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
     } > SRAM_OC_CACHEABLE AT>BOARD_FLASH
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
       FILL(0xff)
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(NonCacheable.init)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > SRAM_DTC AT>BOARD_FLASH

    /* BSS section for SRAM_ITC */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       *(.bss.$RAM2*)
       *(.bss.$SRAM_ITC*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
    } > SRAM_ITC 

    /* BSS section for SRAM_OC_NON_CACHEABLE */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       *(.bss.$RAM3*)
       *(.bss.$SRAM_OC_NON_CACHEABLE*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
    } > SRAM_OC_NON_CACHEABLE 

    /* BSS section for SRAM_OC_CACHEABLE */
    .bss_RAM4 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       *(.bss.$RAM4*)
       *(.bss.$SRAM_OC_CACHEABLE*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
    } > SRAM_OC_CACHEABLE 

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
       *(NonCacheable)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > SRAM_DTC

    /* NOINIT section for SRAM_ITC */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM2*)
       *(.noinit.$SRAM_ITC*)
       . = ALIGN(4) ;
    } > SRAM_ITC 

    /* NOINIT section for SRAM_OC_NON_CACHEABLE */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM3*)
       *(.noinit.$SRAM_OC_NON_CACHEABLE*)
       . = ALIGN(4) ;
    } > SRAM_OC_NON_CACHEABLE 

    /* NOINIT section for SRAM_OC_CACHEABLE */
    .noinit_RAM4 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM4*)
       *(.noinit.$SRAM_OC_CACHEABLE*)
       . = ALIGN(4) ;
    } > SRAM_OC_CACHEABLE 

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > SRAM_DTC

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x1000;
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
        __HeapLimit = _pvHeapLimit;
    } > SRAM_DTC

     _StackSize = 0x1000;
     /* Reserve space in memory for Stack */
    .heap2stackfill  :
    {
        . += _StackSize;
    } > SRAM_DTC
    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM_DTC) + LENGTH(SRAM_DTC) - _StackSize - 0:  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        __StackTop = . + _StackSize;
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
