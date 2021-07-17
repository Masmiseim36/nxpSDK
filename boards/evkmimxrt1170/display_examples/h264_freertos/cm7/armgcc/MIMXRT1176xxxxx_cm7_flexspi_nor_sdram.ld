/*
** ###################################################################
**     Processors:          MIMXRT1176AVM8A_cm7
**                          MIMXRT1176CVM8A_cm7
**                          MIMXRT1176DVMAA_cm7
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT1170RM, Rev 0, 12/2020
**     Version:             rev. 1.0, 2020-12-29
**     Build:               b210202
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

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x1800000;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x1000;
NCACHE_HEAP_START = DEFINED(__heap_noncacheable__) ? 0x84000000 - HEAP_SIZE : 0x83000000 - HEAP_SIZE;
NCACHE_HEAP_SIZE  = DEFINED(__heap_noncacheable__) ? HEAP_SIZE : 0x0000;

/* Specify the memory areas */
MEMORY
{
  m_flash_config        (RX)  : ORIGIN = 0x30000400, LENGTH = 0x00000C00
  m_ivt                 (RX)  : ORIGIN = 0x30001000, LENGTH = 0x00001000
  m_interrupts          (RX)  : ORIGIN = 0x30002000, LENGTH = 0x00000400
  m_text                (RX)  : ORIGIN = 0x30002400, LENGTH = 0x00FFDC00
  m_itcm                (RW)  : ORIGIN = 0x00000000, LENGTH = 0x00040000
  m_dtcm                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00040000
  m_ocram               (RW)  : ORIGIN = 0x202C0000, LENGTH = 0x00080000
  m_data                (RW)  : ORIGIN = 0x80000000, LENGTH = DEFINED(__heap_noncacheable__) ? 0x03000000 : 0x03000000 - HEAP_SIZE
  m_ncache              (RW)  : ORIGIN = 0x83000000, LENGTH = DEFINED(__heap_noncacheable__) ? 0x01000000 - HEAP_SIZE : 0x01000000
  m_heap                (RW)  : ORIGIN = NCACHE_HEAP_START, LENGTH = HEAP_SIZE
}

/* Define output sections */
SECTIONS
{
  __NCACHE_REGION_START = ORIGIN(m_ncache);
  __NCACHE_REGION_SIZE  = LENGTH(m_ncache) + NCACHE_HEAP_SIZE;

  .flash_config :
  {
    . = ALIGN(4);
    __FLASH_BASE = .;
    KEEP(* (.boot_hdr.conf))     /* flash config section */
    . = ALIGN(4);
  } > m_flash_config

  ivt_begin= ORIGIN(m_flash_config) + LENGTH(m_flash_config);

  .ivt : AT(ivt_begin)
  {
    . = ALIGN(4);
	KEEP(* (.boot_hdr.ivt))           /* ivt section */
	KEEP(* (.boot_hdr.boot_data))     /* boot section */
	KEEP(* (.boot_hdr.dcd_data))      /* dcd section */
    . = ALIGN(4);
  } > m_ivt

  /* The startup code goes first into internal RAM */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __Vectors = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into internal RAM */
  .text :
  {
    . = ALIGN(4);

    *(EXCLUDE_FILE(
      */decoder.cpp.obj
      */decoder_core.cpp.obj
      */deblocking.cpp.obj
      */deblocking_common.cpp.obj
      */memory_align.cpp.obj
      */welsDecoderExt.cpp.obj
      */cabac_decoder.cpp.obj
      */mc.cpp.obj
      */mv_pred.cpp.obj
      */bit_stream.cpp.obj
      */copy_mb.cpp.obj
      */utils.cpp.obj
      */decode_slice.cpp.obj
      */get_intra_predictor.cpp.obj
      */cpu.cpp.obj
      */decode_mb_aux.cpp.obj
      */decoder_data_tables.cpp.obj
      */common_tables.cpp.obj
    ) .text )

    *(EXCLUDE_FILE(
      */decoder.cpp.obj
      */decoder_core.cpp.obj
      */deblocking.cpp.obj
      */deblocking_common.cpp.obj
      */memory_align.cpp.obj
      */welsDecoderExt.cpp.obj
      */cabac_decoder.cpp.obj
      */mc.cpp.obj
      */mv_pred.cpp.obj
      */bit_stream.cpp.obj
      */copy_mb.cpp.obj
      */utils.cpp.obj
      */decode_slice.cpp.obj
      */get_intra_predictor.cpp.obj
      */cpu.cpp.obj
      */decode_mb_aux.cpp.obj
      */decoder_data_tables.cpp.obj
      */common_tables.cpp.obj
    ) .text* )

    *(.rodata)
    *(.rodata*)
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
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

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(m_usb_dma_init_data)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __ram_function_flash_start = __DATA_ROM + (__data_end__ - __data_start__); /* Symbol is used by startup for TCM data initialization */

  .ram_function : AT(__ram_function_flash_start)
  {
    . = ALIGN(32);
    __ram_function_start__ = .;
    *(CodeQuickAccess)
    */decoder.cpp.obj(.text .text*)
    */decoder_core.cpp.obj(.text .text*)
    */deblocking.cpp.obj(.text .text*)
    */deblocking_common.cpp.obj(.text .text*)
    */memory_align.cpp.obj(.text .text*)
    */welsDecoderExt.cpp.obj(.text .text*)
    */cabac_decoder.cpp.obj(.text .text*)
    */mc.cpp.obj(.text .text*)
    */mv_pred.cpp.obj(.text .text*)
    */bit_stream.cpp.obj(.text .text*)
    */copy_mb.cpp.obj(.text .text*)
    */utils.cpp.obj(.text .text*)
    */decode_slice.cpp.obj(.text .text*)
    */get_intra_predictor.cpp.obj(.text .text*)
    */cpu.cpp.obj(.text .text*)
    */decode_mb_aux.cpp.obj(.text .text*)
    */decoder_data_tables.cpp.obj(.text .text*)
    */common_tables.cpp.obj(.text .text*)
    . = ALIGN(128);
    __ram_function_end__ = .;
  } > m_itcm

  __NDATA_ROM = __ram_function_flash_start + (__ram_function_end__ - __ram_function_start__);

  .ncache.init : AT(__NDATA_ROM)
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(4);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_ncache
  . = __noncachedata_init_end__;
  .ncache :
  {
    *(NonCacheable)
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_ncache

  __DATA_END = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(m_usb_dma_noninit_data)
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .decode_buf :
  {
    . = ALIGN(4);
    *(decode_buf)
  } > m_dtcm

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_heap

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
