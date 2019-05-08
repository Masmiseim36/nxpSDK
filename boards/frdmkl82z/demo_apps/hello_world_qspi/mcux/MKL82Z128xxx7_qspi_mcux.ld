/*
** ###################################################################
**     Processors:          MKL82Z128VLH7
**                          MKL82Z128VLK7
**                          MKL82Z128VLL7
**                          MKL82Z128VMC7
**                          MKL82Z128VMP7
**
**     Compiler:            GNU C Compiler
**     Reference manual:    KL82P121M72SF0RM, Rev.0.3 June 2015
**     Version:             rev. 1.4, 2015-08-28
**     Build:               b150910
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(ResetISR)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0400;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0140 : 0x0;

/* Specify the memory areas */
MEMORY
{
  m_config              (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000410
  m_text                (RX)  : ORIGIN = 0x68001000, LENGTH = 0x01000000
  m_data                (RW)  : ORIGIN = 0x1FFFA000, LENGTH = 0x00005C00
  m_data_2              (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00011FF0
  m_ramfunc             (RX)  : ORIGIN = 0x1FFFFC00, LENGTH = 0x00000400
}

GROUP (
  libcr_semihost_nf.a
  libcr_c.a
  libcr_eabihelpers.a
)

SECTIONS
{
    /* MAIN TEXT SECTION */
    .m_config : ALIGN(4)
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
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        __bss_section_table_end = .;
        __section_table_end = . ;
      /* End of Global Section Table */
      
        /* Bootloader config */
        . = 0x3c0 ;
        KEEP(*(.BootloaderConfig)) ;

        /* Kinetis Flash Configuration data */
        . = 0x400 ;
        PROVIDE(__FLASH_CONFIG_START__ = .) ;
        KEEP(*(.FlashConfig))
        PROVIDE(__FLASH_CONFIG_END__ = .) ;
        ASSERT(!(__FLASH_CONFIG_START__ == __FLASH_CONFIG_END__), "Linker Flash Config Support Enabled, but no .FlashConfig section provided within application");
        /* End of Kinetis Flash Configuration data */
        

    } >m_config

    .text : ALIGN(4)
    {
        *(.after_vectors*)
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > m_text
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4) 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > m_text
    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > m_text
    __exidx_end = .;

    _etext = .;
        
 
    /* USB_RAM */
    .m_usb_data (NOLOAD) :
    {
        *(m_usb_bdt)
    } > m_data_2
    /* DATA section for m_data */
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$m_data)
        *(.data.$RAM2*)
        *(.data.$m_data*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
     } > m_data AT>m_text

    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > m_data_2
    /* Main DATA section (m_data_2) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > m_data_2 AT>m_text
    /* BSS section for m_data */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       *(.bss.$RAM2*)
       *(.bss.$m_data*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
    } > m_data 
    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > m_data_2
    /* NOINIT section for m_data */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM2*)
       *(.noinit.$m_data*)
       . = ALIGN(4) ;
    } > m_data
     
    /* DEFAULT NOINIT SECTION */
    ramfunc_section : ALIGN(4)
    {
       *(ramfunc_section)  
    } > m_ramfunc AT>m_text

    /* ram function section parameters*/
    ramfunc_load_address = LOADADDR(ramfunc_section);
    ramfunc_length = SIZEOF(ramfunc_section);
    ramfunc_execution_address = ADDR(ramfunc_section);
    
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > m_data_2
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += 0x1000;
    . = ALIGN(4);
    _pvHeapLimit = .;
    } > m_data_2
       .heap2stackfill  : 
  {
    . += 0x1000;
  } > m_data_2
  .stack ORIGIN(m_data_2) + LENGTH(m_data_2) - 0x1000 - 0:  ALIGN(4)
  {
    _vStackBase = .;
    . = ALIGN(4);
    _vStackTop = . + 0x1000;
  } > m_data_2
}
