/*
* The Clear BSD License
* Copyright (c) 2014-2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* By default, the Bootloader is not used. */
/*
gUseBootloaderLink_d = DEFINED(gUseBootloaderLink_d) ? gUseBootloaderLink_d : 0;
*/

/* By default, the NVM is not used. */
gUseNVMLink_d  = DEFINED(gUseNVMLink_d) ? gUseNVMLink_d : 1;


gNVMSectorCountLink_d = DEFINED(gNVMSectorCountLink_d) ? gNVMSectorCountLink_d : 4;

/*
/* By default, the internal storage is not used. */
/*
gUseInternalStorageLink_d = DEFINED(gUseInternalStorageLink_d) ? gUseInternalStorageLink_d : 0;
*/

__ram_vector_table__ = DEFINED(__ram_vector_table__) ? __ram_vector_table__ : 1;

/*-Memory Limits-*/
__region_ROM_start__   =  (0x00000000);
__region_ROM_end__     =  (0x0007FFFF);
/* Workaround for  SRAM boundary GCC placement issue,
 not to place variables that are accessed by the code 
 overlapping this boundary (forbidden by the Kinetis architecture) */
__region_RAM1_start__  =  (0x1FFF8000);
__region_RAM1_end__    =  (0x1FFFFFFF);
__region_RAM2_start__  =  (0x20000000);
__region_RAM2_end__    =  (0x20017FFF);


__vector_table_size__ = (48*4);
__ram_vector_table_size__ = DEFINED(__ram_vector_table__) ? __vector_table_size__ : 0;
_RAM_START_  = __region_RAM1_start__;
_RAM_END_    = __region_RAM2_end__;


m_interrupts_start   = (__region_ROM_start__);
m_interrupts_end     = (m_interrupts_start + __vector_table_size__ - 1);
m_flash_config_start = (0x400);
m_flash_config_end   = (0x410);
/*-Sizes-*/
/* 2k flash sector size */
m_sector_size =  ( 2 * 1024 );

__size_cstack__ = DEFINED(__stack_size__) ? __stack_size__ : 512;


__size_heap__ = DEFINED(__heap_size__) ? __heap_size__ : 0;

/*** FSL Product Info region ***/
m_fsl_prodInfo_start = (__region_ROM_end__ - m_sector_size + 1);
m_fsl_prodInfo_end   = (__region_ROM_end__);
FREESCALE_PROD_DATA_BASE_ADDR = m_fsl_prodInfo_start;


/*** NVM Configuration ***/
NV_STORAGE_SECTOR_SIZE     = m_sector_size;
NV_STORAGE_MAX_SECTORS     = gNVMSectorCountLink_d;     /* NV_STORAGE_MAX_SECTORS * NV_STORAGE_SECTOR_SIZE = total NV Storage space */
NV_STORAGE_START_ADDRESS   = m_fsl_prodInfo_start - 1;
NV_STORAGE_END_ADDRESS     = NV_STORAGE_START_ADDRESS - (NV_STORAGE_MAX_SECTORS * NV_STORAGE_SECTOR_SIZE) + 1;
NV_STORAGE_SECTOR_SIZE_C   = (m_sector_size);
NV_STORAGE_MAX_SECTORS_C   = (gNVMSectorCountLink_d);
NV_STORAGE_START_ADDRESS_C = (m_fsl_prodInfo_start - 1);
NV_STORAGE_END_ADDRESS_C   = (NV_STORAGE_START_ADDRESS_C - (NV_STORAGE_MAX_SECTORS_C * NV_STORAGE_SECTOR_SIZE_C) + 1);
/* Define the limits of the memory regions*/
m_text_start = (m_interrupts_start);
m_text_end   = (NV_STORAGE_END_ADDRESS_C - 1);
m_interrupts_ram_start = (__region_RAM2_start__);
m_interrupts_ram_end   = (m_interrupts_ram_start + __ram_vector_table_size__ - 1);

__CSTACK_end__         = (__region_RAM2_end__);
__CSTACK_start__       = (__CSTACK_end__ - __size_cstack__);
__HEAP_end__           = (__CSTACK_start__ - 1);
__HEAP_start__         = (__HEAP_end__ - __size_heap__ - 0x4);


m_data1_start           = (__region_RAM1_start__);
m_data1_end             = (__region_RAM1_end__);
m_data2_start           =  (m_interrupts_ram_start);
m_data2_end             =  (__HEAP_start__ - 1);

__RAM_VECTOR_TABLE_SIZE = __ram_vector_table_size__;
__BOOT_STACK_ADDRESS = m_data2_end - 0x0F;

/* Specify the memory areas */
MEMORY
{
        
        TEXT_region1              (RX) : ORIGIN = m_text_start, LENGTH = (m_flash_config_start - m_text_start)
        m_flash_config_region     (RX) : ORIGIN = m_flash_config_start,           LENGTH = (m_flash_config_end - m_flash_config_start)
        TEXT_region2              (RX) : ORIGIN = m_flash_config_end + 1,           LENGTH = (m_text_end - m_flash_config_end - 1)
        
        NVM_region           (RW) : ORIGIN = NV_STORAGE_END_ADDRESS_C,       LENGTH = (NV_STORAGE_START_ADDRESS_C - NV_STORAGE_END_ADDRESS_C)
        DATA1_region         (RW) : ORIGIN = m_data1_start, LENGTH = (m_data1_end - m_data1_start + 1)
        DATA2_region         (RW) : ORIGIN = m_data2_start, LENGTH = (__region_RAM2_end__ - __region_RAM2_start__ + 1)
        PRODUCT_INFO_region  (RX) : ORIGIN = m_fsl_prodInfo_start, LENGTH = (m_fsl_prodInfo_end - m_fsl_prodInfo_start)
}

/* Define output sections */
SECTIONS
{
   /* Place in the first memory bank the memHeap */
  /*
  .data1 :
  {
    ./framework/MemManager/Source/MemManager.o(.bss*)
  } > DATA1_region
  */
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > TEXT_region1
  .freescale_prod_data :
  {
    . = ALIGN(4);
    KEEP(*(.FREESCALE_PROD_DATA))
    . = ALIGN(4);
  } > PRODUCT_INFO_region
  .flash_config :
  {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
    . = ALIGN(4);
  } > m_flash_config_region
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
  } > TEXT_region2
  .NVM_TABLE :
  {
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
  } > TEXT_region2
  .VERSION_TAGS :
  {
    . = ALIGN(4);
    PROVIDE(__start_VERSION_TAGS = .);
    KEEP(*(.VERSION_TAGS));
    PROVIDE(__stop_VERSION_TAGS = .);
    . = ALIGN(4);
  } > TEXT_region2

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > TEXT_region2

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > TEXT_region2

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
  } > TEXT_region2

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > TEXT_region2

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > TEXT_region2

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > TEXT_region2

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > TEXT_region2

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  /* reserve MTB memory at the beginning of m_data */
  .mtb : /* MTB buffer address as defined by the hardware */
  {
    . = ALIGN(8);
    _mtb_start = .;
    KEEP(*(.mtb_buf)) /* need to KEEP Micro Trace Buffer as not referenced by application */
    . = ALIGN(8);
    _mtb_end = .;
  } > DATA2_region

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += __ram_vector_table_size__;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > DATA2_region
  __VECTOR_RAM = __VECTOR_RAM__;
  __RAM_VECTOR_TABLE_SIZE_BYTES = (__interrupts_ram_end__ - __interrupts_ram_start__);
  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > DATA2_region

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(TEXT_region2) + LENGTH(TEXT_region2);
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
    /*
    *(EXCLUDE_FILE (./framework/MemManager/Source/MemManager.o) .bss*)
    */
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > DATA2_region

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += __size_heap__;
    __HeapLimit = .;
  } > DATA2_region

  .stack :
  {
    . = ALIGN(8);
    . += __size_cstack__;
  } > DATA2_region

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(DATA2_region) + LENGTH(DATA2_region);
  __StackLimit = __StackTop - __size_cstack__;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /* Remove this section to keep the nvm section on writting the device */
  .NVM :
  {
    FILL(0xFFFFFFFF);
    . = ORIGIN(NVM_region) + LENGTH(NVM_region) - 1;
    BYTE(0xFF)
  } > NVM_region
  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}
