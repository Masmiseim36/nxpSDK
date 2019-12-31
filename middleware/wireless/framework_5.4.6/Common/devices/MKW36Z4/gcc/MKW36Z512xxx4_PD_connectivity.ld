/*
** ###################################################################
**     Compiler:            GNU C Compiler
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2018 Freescale Semiconductor, Inc.
**     Copyright 2018-2019 NXP
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

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0000;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00C0 : 0x0;
WARMBOOT_STACK_SIZE = DEFINED(__warmboot_stack_size__)  ? __warmboot_stack_size__ : 0x00000400;
WARMBOOT_STACK_SIZE = DEFINED(gUseWarmBootLink_d) ? WARMBOOT_STACK_SIZE : 0x00;

_RAM_START_  = 0x1FFFC000;
_RAM_END_    = 0x2000C000;

_ROM_START_  = 0x00000000;
_ROM_END_    = 0x00080000;

FREESCALE_PROD_DATA_BASE_ADDR = 0x0007F800;
__sector_size__ = 2048;
__vector_table_size__ = (48*4);
_APP_START_ = DEFINED(gUseBootloaderLink_d) ? (_ROM_END_ / 64) : _ROM_START_;
__BootFlags_Start__ = _APP_START_ + __vector_table_size__;
__BootFlags_Size__ = DEFINED(gUseBootloaderLink_d) ? 32 : 0x0;

NV_STORAGE_END_ADDRESS = FREESCALE_PROD_DATA_BASE_ADDR;
NV_STORAGE_SECTOR_SIZE = __sector_size__;
NV_STORAGE_MAX_SECTORS = 4;     /* NV_STORAGE_MAX_SECTORS * NV_STORAGE_SECTOR_SIZE = total NV Storage space */
NV_STORAGE_START_ADDRESS = NV_STORAGE_END_ADDRESS - (NV_STORAGE_MAX_SECTORS * NV_STORAGE_SECTOR_SIZE);

INT_STORAGE_SECTOR_SIZE = NV_STORAGE_SECTOR_SIZE;
INT_STORAGE_END = DEFINED(gUseNVMLink_d) ? NV_STORAGE_START_ADDRESS : FREESCALE_PROD_DATA_BASE_ADDR;
INT_STORAGE_SIZE  = DEFINED(gUseInternalStorageLink_d) ? INT_STORAGE_SECTOR_SIZE * (( (INT_STORAGE_END - _APP_START_) / 2 ) / INT_STORAGE_SECTOR_SIZE + 1) : 0x00;
INT_STORAGE_START   = INT_STORAGE_END - INT_STORAGE_SIZE;

_FLASH_CONFIG_START_ = DEFINED(gUseBootloaderLink_d) ? (__BootFlags_Start__ + __BootFlags_Size__) : 0x400;
_FLASH_CONFIG_SIZE_  = 0x10;

_TEXT_START_ = _FLASH_CONFIG_START_ + _FLASH_CONFIG_SIZE_;
_TEXT_END_ = DEFINED(gUseNVMLink_d) ? NV_STORAGE_START_ADDRESS : FREESCALE_PROD_DATA_BASE_ADDR;
_TEXT_END_ = DEFINED(gUseInternalStorageLink_d) ? INT_STORAGE_START : _TEXT_END_;

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = _APP_START_,                                                          LENGTH = __vector_table_size__
  BootFlags_region      (RX)  : ORIGIN = __BootFlags_Start__,                                                  LENGTH = __BootFlags_Size__
  m_flash_config        (RX)  : ORIGIN = _FLASH_CONFIG_START_,                                                 LENGTH = _FLASH_CONFIG_SIZE_
  m_text                (RX)  : ORIGIN = _TEXT_START_,                                                         LENGTH = _TEXT_END_ - _TEXT_START_
  InternalStorage       (RX)  : ORIGIN = _TEXT_END_,                                                           LENGTH = INT_STORAGE_SIZE
  NVM_region            (RW)  : ORIGIN = DEFINED(gUseInternalStorageLink_d) ? INT_STORAGE_END : _TEXT_END_ ,   LENGTH = DEFINED(gUseNVMLink_d) ? NV_STORAGE_END_ADDRESS - NV_STORAGE_START_ADDRESS : 0x01 /* dummy byte for the bellow fill */

  m_data_2              (RW)  : ORIGIN = _RAM_START_,                                                          LENGTH = 0x20000000 - _RAM_START_
  m_data                (RW)  : ORIGIN = 0x20000000,                                                           LENGTH = _RAM_END_ - 0x20000000
}

/* Define output sections */
SECTIONS
{

  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  .flash_config :
  {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
    . = ALIGN(4);
  } > m_flash_config

  .BootloaderFlags :
  {
    . = ALIGN(4);
    KEEP(*(.BootloaderFlags));
    . = ALIGN(4);
  } > BootFlags_region

  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text

  .NVM_TABLE :
  {
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
  } > m_text

  .VERSION_TAGS :
  {
    . = ALIGN(4);
    PROVIDE(__start_VERSION_TAGS = .);
    KEEP(*(.VERSION_TAGS));
    PROVIDE(__stop_VERSION_TAGS = .);
    . = ALIGN(4);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
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
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
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
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .warmboot_config : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __warmboot_config_start__ = .;
    *(WarmbootConfig)
    . = ALIGN(4);
    __warmboot_config_end__ = .;
  } > m_data

  __WARMBOOT_CONFIG_END = __DATA_ROM + (__warmboot_config_end__ - __warmboot_config_start__);

  .data : AT(__WARMBOOT_CONFIG_END)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __WARMBOOT_CONFIG_END + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .noinit :
  {
    /* Data section which will not be initialized */
    . = ALIGN(4);
    __noinit_start__ = .;
    *(NOINIT)
    . = ALIGN(4);
    __noinit_end__ = .;
  } > m_data

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data_2

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data_2

  .warmboot_stack :
  {
    . = ALIGN(8);
    __WarmbootStackBase = .;
    . += WARMBOOT_STACK_SIZE;
    __WarmbootStackLimit = .;
  } > m_data_2

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data_2) + LENGTH(m_data_2);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /* Remove this section to keep the nvm section on writting the device */
  .NVM :
  {
    FILL(0xFFFFFFFF);
    . = ORIGIN(NVM_region) + LENGTH(NVM_region) - 1;
    BYTE(0xFF)
  } > NVM_region

  ASSERT(__StackLimit >= __HeapLimit, "region m_data_2 overflowed with stack and heap")
}
