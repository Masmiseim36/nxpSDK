/*
** ###################################################################
**     Processors:          MK64FN1M0CAJ12
**                          MK64FN1M0VDC12
**                          MK64FN1M0VLL12
**                          MK64FN1M0VLQ12
**                          MK64FN1M0VMD12
**
**     Compiler:            GNU C Compiler
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
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

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x1000;

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = 0x1FFF0000, LENGTH = 0x00000400
  m_data                (RW)  : ORIGIN = 0x1FFF0400, LENGTH = 0x0003FC00
}

GROUP (
  libnosys.a
  libgcc.a
  libc_nano.a
  libm.a
  libcr_newlib_none.a
)

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal RAM */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  __VECTOR_RAM = __VECTOR_TABLE;
  __RAM_VECTOR_TABLE_SIZE_BYTES = 0x0;

  /* The program code and other data goes into internal RAM */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } > m_data

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_data

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_data

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);

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
  
  m_usb_bdt :
  {
    . = ALIGN(512);
    __START_USBGLOBAL = .;
    *(m_usb_bdt)
    KEEP(*(m_usb_bdt))
  } > m_data
  
  m_usb_global :
  {
    *(m_usb_global)
    KEEP(*(m_usb_global))
    __END_USBGLOBAL = .;
  } > m_data

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    _pvHeapStart = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . += STACK_SIZE;
    __StackTop = .;
    __STACK_TOP = .;
    PROVIDE(__stack = __StackTop);
  } > m_data

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

