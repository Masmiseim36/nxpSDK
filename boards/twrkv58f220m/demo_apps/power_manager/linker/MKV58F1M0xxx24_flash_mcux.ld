/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2018
 * Generated linker script file for MKV58F1M0xxx24
 * Created from linkscript.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v10.2.0_prc1e [Build 729] [2018-03-23]  on Apr 23, 2018 2:36:50 PM
 */

GROUP (
  "libcr_semihost_nf.a"
  "libcr_c.a"
  "libcr_eabihelpers.a"
)

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 0x100000 /* 1M bytes (alias Flash) */  
  SRAM_DTC (rwx) : ORIGIN = 0x20000000, LENGTH = 0x20000 /* 128K bytes (alias RAM) */  
  SRAM_OC (rwx) : ORIGIN = 0x2f000000, LENGTH = 0x10000 /* 64K bytes (alias RAM2) */  
  SRAM_ITC (rwx) : ORIGIN = 0x0, LENGTH = 0x10000 /* 64K bytes (alias RAM3) */  
  FLEX_RAM (rwx) : ORIGIN = 0x18000000, LENGTH = 0x1000 /* 4K bytes (alias RAM4) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x10000000  ; /* PROGRAM_FLASH */  
  __base_Flash = 0x10000000 ; /* Flash */  
  __top_PROGRAM_FLASH = 0x10000000 + 0x100000 ; /* 1M bytes */  
  __top_Flash = 0x10000000 + 0x100000 ; /* 1M bytes */  
  __base_SRAM_DTC = 0x20000000  ; /* SRAM_DTC */  
  __base_RAM = 0x20000000 ; /* RAM */  
  __top_SRAM_DTC = 0x20000000 + 0x20000 ; /* 128K bytes */  
  __top_RAM = 0x20000000 + 0x20000 ; /* 128K bytes */  
  __base_SRAM_OC = 0x2f000000  ; /* SRAM_OC */  
  __base_RAM2 = 0x2f000000 ; /* RAM2 */  
  __top_SRAM_OC = 0x2f000000 + 0x10000 ; /* 64K bytes */  
  __top_RAM2 = 0x2f000000 + 0x10000 ; /* 64K bytes */  
  __base_SRAM_ITC = 0x0  ; /* SRAM_ITC */  
  __base_RAM3 = 0x0 ; /* RAM3 */  
  __top_SRAM_ITC = 0x0 + 0x10000 ; /* 64K bytes */  
  __top_RAM3 = 0x0 + 0x10000 ; /* 64K bytes */  
  __base_FLEX_RAM = 0x18000000  ; /* FLEX_RAM */  
  __base_RAM4 = 0x18000000 ; /* RAM4 */  
  __top_FLEX_RAM = 0x18000000 + 0x1000 ; /* 4K bytes */  
  __top_RAM4 = 0x18000000 + 0x1000 ; /* 4K bytes */  

ENTRY(ResetISR)

SECTIONS
{
    /* MAIN TEXT SECTION */
    .text : ALIGN(8)
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

        /* Kinetis Flash Configuration data */
        . = 0x400 ;
        PROVIDE(__FLASH_CONFIG_START__ = .) ;
        KEEP(*(.FlashConfig))
        PROVIDE(__FLASH_CONFIG_END__ = .) ;
        ASSERT(!(__FLASH_CONFIG_START__ == __FLASH_CONFIG_END__), "Linker Flash Config Support Enabled, but no .FlashConfig section provided within application");
        /* End of Kinetis Flash Configuration data */
    } > PROGRAM_FLASH

    .text : ALIGN(8)
    {
       EXCLUDE_FILE (*fsl_ftfx_cache*) *(.text*)
       EXCLUDE_FILE (*fsl_ftfx_cache*) *(.rodata .rodata.* .constdata .constdata.*)
       . = ALIGN(8);
    } > PROGRAM_FLASH
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(8) 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > PROGRAM_FLASH

    __exidx_start = .;

    .ARM.exidx : ALIGN(8)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > PROGRAM_FLASH
    __exidx_end = .;

    _etext = .;
        
    /* DATA section for SRAM_OC */

    .data_RAM2 : ALIGN(8)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_OC)
        *(.data.$RAM2*)
        *(.data.$SRAM_OC*)
        . = ALIGN(8) ;
        PROVIDE(__end_data_RAM2 = .) ;
     } > SRAM_OC AT>PROGRAM_FLASH
    /* DATA section for SRAM_ITC */

    .data_RAM3 : ALIGN(8)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        *fsl_ftfx_cache.*
        *(.ramfunc.$RAM3)
        *(.ramfunc.$SRAM_ITC)
        *(.data.$RAM3*)
        *(.data.$SRAM_ITC*)
        . = ALIGN(8) ;
        PROVIDE(__end_data_RAM3 = .) ;
     } > SRAM_ITC AT>PROGRAM_FLASH
    /* DATA section for FLEX_RAM */

    .data_RAM4 : ALIGN(8)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$FLEX_RAM)
        *(.data.$RAM4*)
        *(.data.$FLEX_RAM*)
        . = ALIGN(8) ;
        PROVIDE(__end_data_RAM4 = .) ;
     } > FLEX_RAM AT>PROGRAM_FLASH
    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(8)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM_DTC

    /* Main DATA section (SRAM_DTC) */
    .data : ALIGN(8)
    {
       FILL(0xff)
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(NonCacheable.init)
       *(.data*)
       . = ALIGN(8) ;
       _edata = . ;
    } > SRAM_DTC AT>PROGRAM_FLASH

    /* BSS section for SRAM_OC */
    .bss_RAM2 : ALIGN(8)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       *(.bss.$RAM2*)
       *(.bss.$SRAM_OC*)
       . = ALIGN (. != 0 ? 8 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
    } > SRAM_OC 

    /* BSS section for SRAM_ITC */
    .bss_RAM3 : ALIGN(8)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       *(.bss.$RAM3*)
       *(.bss.$SRAM_ITC*)
       . = ALIGN (. != 0 ? 8 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
    } > SRAM_ITC 

    /* BSS section for FLEX_RAM */
    .bss_RAM4 : ALIGN(8)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       *(.bss.$RAM4*)
       *(.bss.$FLEX_RAM*)
       . = ALIGN (. != 0 ? 8 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
    } > FLEX_RAM 

    /* MAIN BSS SECTION */
    .bss : ALIGN(8)
    {
        _bss = .;
       *(NonCacheable)
        *(.bss*)
        *(COMMON)
        . = ALIGN(8) ;
        _ebss = .;
        PROVIDE(end = .);
    } > SRAM_DTC

    /* NOINIT section for SRAM_OC */
    .noinit_RAM2 (NOLOAD) : ALIGN(8)
    {
       *(.noinit.$RAM2*)
       *(.noinit.$SRAM_OC*)
       . = ALIGN(8) ;
    } > SRAM_OC 

    /* NOINIT section for SRAM_ITC */
    .noinit_RAM3 (NOLOAD) : ALIGN(8)
    {
       *(.noinit.$RAM3*)
       *(.noinit.$SRAM_ITC*)
       . = ALIGN(8) ;
    } > SRAM_ITC 

    /* NOINIT section for FLEX_RAM */
    .noinit_RAM4 (NOLOAD) : ALIGN(8)
    {
       *(.noinit.$RAM4*)
       *(.noinit.$FLEX_RAM*)
       . = ALIGN(8) ;
    } > FLEX_RAM 

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(8)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(8) ;
        _end_noinit = .;
    } > SRAM_DTC

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x1000;
    .heap :  ALIGN(8)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(8);
        _pvHeapLimit = .;
    } > SRAM_DTC

     _StackSize = 0x1000;
     /* Reserve space in memory for Stack */
    .heap2stackfill  :
    {
        . += _StackSize;
    } > SRAM_DTC
    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM_DTC) + LENGTH(SRAM_DTC) - _StackSize - 0:  ALIGN(8)
    {
        _vStackBase = .;
        . = ALIGN(8);
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
