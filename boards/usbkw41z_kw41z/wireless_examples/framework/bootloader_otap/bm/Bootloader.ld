/*
/*
** ###################################################################
**     Copyright (c) 2016-2018 NXP Semiconductors, Inc.
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
ENTRY(__thumb_startup)

gRomSize_c = 524288;
gSectorSize_c = 2048;

__ROM_SIZE__ = gRomSize_c;
__region_BOOT_ROM_start__	= 0x00000000;
__region_BOOT_ROM_end__   	= (gRomSize_c / 32) - 1;
__region_RAM_start__  	  	= 0x20000000;
__region_RAM_end__			= 0x20003fff; /* limit to  16K */
__code_end__ = __region_BOOT_ROM_end__ - gSectorSize_c;
__BOOT_Flags_start__ = __code_end__ + 1;

/* Generate a link error if heap and stack don't fit into RAM */
__heap_size = 0; /* required amount of heap  */
__stack_size = 0x1000; /* required amount of stack */

/* Highest address of the user mode stack */
__SP_INIT = __region_RAM_end__ - 7;

/* Specify the memory areas */
MEMORY
{
  m_interrupts	(rx) : ORIGIN = 0x00000000, LENGTH = 0xc0
  m_cfmprotrom 	(rx) : ORIGIN = 0x00000400, LENGTH = 0x10
  m_bootinfo    (rx) : ORIGIN = 0x00000410, LENGTH = 0x10
  m_text        (rx) : ORIGIN = 0x00000420, LENGTH = (gRomSize_c / 32) - 1 - 0x420
  m_data	   (rwx) : ORIGIN = 0x20000000, LENGTH = 16K
  BootFlags_region (rx) : ORIGIN = ((gRomSize_c / 32) - gSectorSize_c), LENGTH = gSectorSize_c
}


/* Define output sections */
SECTIONS
{
  /* The startup code goes first into Flash */
  .interrupts :
  {
	__vector_table = .;
    . = ALIGN(4);
    KEEP(*(.vectortable)) /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  .cfmconfig :
  {
    . = ALIGN(4);
	KEEP(*(.cfmconfig))	/* Flash Configuration Field (FCF) */
	. = ALIGN(4);
  } > m_cfmprotrom

  .BOOT_MODULE_INFO :
  {
    . = ALIGN(4);
    KEEP(*(.BOOT_MODULE_INFO))
    . = ALIGN(4);
  } > m_bootinfo  

  /* The program code and other data goes into Flash */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } > m_text

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > m_text
  .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
  } > m_text

  .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);

	___ROM_AT = .;
  } > m_text

  .ramvectortable (NOLOAD) :
  {
    . = ALIGN(4);
    *(.ramvectortable)
  } > m_data
  
  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : AT(___ROM_AT)
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } > m_data  

  ___data_size = _edata - _sdata;
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    __START_BSS = .;
	PROVIDE ( __bss_start__ = __START_BSS );
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    __END_BSS = .;
	PROVIDE ( __bss_end__ = __END_BSS );
  } > m_data

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(4);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
	__heap_addr = .;
    . = . + __heap_size;
    . = . + __stack_size;
    . = ALIGN(4);
  } > m_data
  
  .BootFlags :
  {
      *(.BootFlags)
  } > BootFlags_region

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
