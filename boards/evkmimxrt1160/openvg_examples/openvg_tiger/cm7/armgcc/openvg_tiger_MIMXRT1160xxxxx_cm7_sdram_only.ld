/*
 *     Abstract:
 *         Linker file for the GNU C Compiler
 *
 *     Copyright 2016-2021 NXP
 *     All rights reserved.
 *
 *     SPDX-License-Identifier: BSD-3-Clause
 *
 *     http:                 www.nxp.com
 *     mail:                 support@nxp.com
 *
 *
 */

ENTRY(Reset_Handler)

/*
 * The memory map looks like this:
 * +--------------------+ <- low memory
 * | .text              |
 * |        _stext      |
 * |        _etext      |
 * |        ctor list   | the ctor and dtor lists are
 * |        dtor list   | for C++ support
 * |        _end_text   |
 * +--------------------+
 * | .data              | initialized data goes here
 * |        _sdata      |
 * |        _edata      |
 * +--------------------+
 * | .bss               |
 * |        __bss_start | start of bss, cleared by crt0
 * |        _end        | start of heap, used by sbrk()
 * +--------------------+
 * |    heap space      |
 * |        _ENDHEAP    |
 * |    stack space     |.
 * |        __stack     | top of stack
 * +--------------------+ <- high memory
 */

/*
 * All the symbols that might be accessed from C code need to be
 * listed twice, once with an additional underscore;
 * aout format needs an extra underscore, whereas coff & elf do not. */

HEAP_SIZE = 0x00900000;
__heap_size = HEAP_SIZE;
_HeapSize = HEAP_SIZE;
__heap_size__ = HEAP_SIZE;
STACK_SIZE = 0xFA00;

MEMORY
{
    m_interrupts          (RX)  : ORIGIN = 0x80000000, LENGTH = 0x00000400

    m_rom1                (RX)  : ORIGIN = 0x80000400, LENGTH = 0x001FFC00

    m_ram1                (RW)  : ORIGIN = 0x202B0000, LENGTH = 0x00020000 - STACK_SIZE

    m_stack               (RW)  : ORIGIN = ORIGIN(m_ram1) + LENGTH(m_ram1), LENGTH = STACK_SIZE

    m_ram3                (RW)  : ORIGIN = 0x80200000, LENGTH = 0x02E00000 - 8 - HEAP_SIZE

    m_heap                (RW)  : ORIGIN = ORIGIN(m_ram3) + LENGTH(m_ram3), LENGTH = HEAP_SIZE

    m_ncache              (RW)  : ORIGIN = 0x83000000, LENGTH = 0x01000000
}

SECTIONS
{

    /* The startup code goes first into internal RAM */
    .interrupts ORIGIN(m_interrupts) :
    {
        _sfixed = .;
        __VECTOR_TABLE = .;
        __Vectors = .;
        . = ALIGN (4);
        KEEP(*(.isr_vector))      /* Startup code */
        . = ALIGN (4);
    } > m_interrupts

    /* .text
     * This section contains executable code.
     * This section has the x (executable),
     * r (read-only) and i (initialized) flags. */
    .text :
    {
        . = ALIGN (4);
        PROVIDE (stext = ABSOLUTE(.));
        PROVIDE (_stext = ABSOLUTE(.));
        PROVIDE (__stext = ABSOLUTE(.));

        *(.text)
        *(.text*)

        /* .rodata
         * This section contains read-only data.
         * This section has the r (read-only) and the i (initialized) flags.
         * Can also be mapped to an initialized, read-only memory
         * that is addressable from the processor data-bus. */
        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.r.*)

        /*
         * NOTE: .glue_7 and .glue_7t sections are not needed because Cortex-M
         * only supports Thumb instructions, no ARM/Thumb interworking.
         */
        *(.glue_7)                /* glue arm to thumb code */
        *(.glue_7t)               /* glue thumb to arm code */

        /* .eh_frame
         * This section contains frame unwind information
         * for exception handling. It contains the same flags
         * as .rodata, and can be mapped to initialized ROM. */
        *(.eh_frame)
    } > m_rom1

    .init :
    {
        /* .init
         * This section contains language initialization code
         * and has the same flags as .text.
         * It must be mapped to initialized ROM. */
        KEEP (*(.init))
    } > m_rom1

    .fini :
    {
        KEEP (*(.fini))
    } > m_rom1

/*
 * Stack unwinding and exception handling sections.
 *
 * ARM compilers emit object files with .ARM.extab and .ARM.exidx sections
 * when using C++ exceptions.
 * Also, at least GCC emits those sections when
 * dividing large numbers (64-bit) in C.
 *
 * ARM uses .ARM.extab and .ARM.exidx instead of the .eh_frame section,
 * which is used on x86.
 */
/* exception unwinding information */
    .ARM.extab :
    {
        *(.ARM.extab*)
        *(.gnu.linkonce.armextab.*)
    } > m_rom1

/* index entries for section unwinding */
    .ARM.exidx :
    {
        __exidx_start = ABSOLUTE(.);
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
        __exidx_end = ABSOLUTE(.);
    } > m_rom1

    /* .ctors
     * This section contains a list of functions that must
     * be invoked at program startup.
     * Has the same flags as .data.
     * It must be mapped to initialized RAM. */
    .ctors :
    {
        __CTOR_LIST__ = ABSOLUTE(.);
        /* gcc uses crtbegin.o to find the start of
         * the constructors, so we make sure it is
         * first.  Because this is a wildcard, it
         * doesn't matter if the user does not
         * actually link against crtbegin.o; the
         * linker won't look for a file to match a
         * wildcard.  The wildcard also means that it
         * doesn't matter which directory crtbegin.o is in.  */
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        /* We don't want to include the .ctor section from
         * the crtend.o file until after the sorted ctors.
         * The .ctor section from the crtend file contains the
         * end of ctors marker and it must be last */
        KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        __CTOR_END__ = ABSOLUTE(.);
    } > m_rom1

    /* .dtors
     * This section contains a list of functions that must
     * be invoked at program end, the same flags as .data.
     * It must be mapped to initialized RAM. */
    .dtors :
    {
        __DTOR_LIST__ = ABSOLUTE(.);
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        __DTOR_END__ = ABSOLUTE(.);
    } > m_rom1

    /* Newlib and Eglibc (at least) need these for C++ support.
     *
     * (Copied from Sourcery CodeBench Lite: arm-none-eabi-gcc -V)
     */
    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } > m_rom1

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } > m_rom1

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } > m_rom1

/*****************************************************************************/
/* etext: the first location after the last
 * read-only loadable segment. */
    PROVIDE (etext = .);
    _etext = .;

/* From startup_MIMXRT1176_cm7.S:311
 * __etext: End of code section, i.e., begin of data sections to copy from. */
    __etext = .;
/*****************************************************************************/

/* _sidata will be the location .data section will be stored
 * in the flash, and _sdata will be the location in the RAM
 * We use these two addresses to copy data from _sidata to _sdata*/
    _sidata = .;
    __sidata = .;
    /************************
     *    DATA
     ************************/
/* .data
 * Initialized data section.
 * This section contains read-write data and has the
 * w (read-write) and the i (initialized) flags.
 * It must be mapped to initialized random access memory (RAM).
 * It cannot be mapped to a ROM. */
/* This section is stored (loaded) into FLASH (LMA address)
 * and copied-to/run-from RAM (VMA address) by the startup code. */
    .data : AT(__etext) /* The AT defines the load address of the .data section. This is where the section will be loaded */
    {
        . = ORIGIN(m_ram3);
        __data_start__ = ABSOLUTE(.);
        PROVIDE (sdata = ABSOLUTE(.));
        _sdata = ABSOLUTE(.);
        __sdata = ABSOLUTE(.);

        *(m_usb_dma_init_data)
        *(.data)
        *(.data*)

        *(.shdata)
        *(vtable)
        *(.gnu.linkonce.d*)

     /* .jcr
      * This section contains information necessary for
      * registering compiled Java classes. The contents are
      * compiler-specific and used by compiler initialization
      * functions. This section must be mapped to initialized RAM. */
        KEEP(*(.jcr*))

        . = ALIGN (4);
        __data_end__ = ABSOLUTE(.);
    } > m_ram3

    /* .bss
     * This section contains un-initialized data.
     * This section has the w (read-write) flag.
     * It is zero-initialized at runtime.
     * It must be mapped to RAM. */
    .bss (NOLOAD) : /*AT (LOADADDR(.data) + SIZEOF(.data)) */
    {
        . = ALIGN (4);

     /* edata: the first location after the last
        read-write loadable segment. */
        PROVIDE (edata = ABSOLUTE(.));
        _edata = ABSOLUTE(.);
        __edata = ABSOLUTE(.);

        PROVIDE (sbss = ABSOLUTE(.));
        _sbss = ABSOLUTE(.);
        __sbss = ABSOLUTE(.);
        __bss_start = ABSOLUTE(.);
        __bss_start__ = ABSOLUTE(.);
        __START_BSS = ABSOLUTE(.);

        *(m_usb_dma_noninit_data)

        /* Statically-allocated variables represented solely
         * by zero-valued bits initially */
        *(.shbss)
        *(.bss)
        *(.bss*)

        /* Support C++ vague linkage(inline functions, vtables */
        *(.gnu.linkonce.b*)

        *(COMMON)

        _bss_end__ = ABSOLUTE(.);
        __bss_end__ = ABSOLUTE(.);
        __END_BSS = ABSOLUTE(.);
        PROVIDE (ebss = ABSOLUTE(.));
        _ebss = ABSOLUTE(.);
        __ebss = ABSOLUTE(.);
    } > m_ram3

    /* .heap
     * This section contains uninitialized data that is used as
     * the global program heap. Dynamic memory allocation routines
     * allocate memory from this section.
     * This section must be mapped to RAM. */
    .heap ALIGN(ORIGIN(m_heap),8) (NOLOAD):
    {
     /* If the heap base value [r0, #0] is 0 then the heap base is actually
        at the end of program data (i.e. end/__end__). See:
        http://infocenter.arm.com/help/topic/com.arm.doc.dui0471-/Bacbefaa.html */

       /* end: the first address past the end of the uninitialized
        * data segment (also known as the BSS segment). */
        PROVIDE (end = ABSOLUTE(.));
        _end = ABSOLUTE(.);
        __end = ABSOLUTE(.);
        __end__ = ABSOLUTE(.);
        __sheap = ABSOLUTE(.);
        __HeapBase = ABSOLUTE(.);

        . += HEAP_SIZE;

        . = ALIGN (8);
        __HeapLimit = ABSOLUTE(.);
        __heap_limit = ABSOLUTE(.);
    } > m_heap

    /* .stack
     * This section contains uninitialized data that is used as
     * the program stack. This section must be mapped to RAM.
     * This section is typically laid out right after the .heap
     * section.
     * In some versions of the linker, the .stack and .heap
     * sections might appear merged together into
     * a section named .bss_stack. */
    .stack ALIGN(ORIGIN(m_stack),8) (NOLOAD):
    {
        __StackLimit = ABSOLUTE(.);

        . += STACK_SIZE;

        . = ALIGN (8);
        _stack = ABSOLUTE(.);
        _estack = ABSOLUTE(.);
        __stack = ABSOLUTE(.);
        __StackTop = ABSOLUTE(.);
        __stack_base__ = ABSOLUTE(.);
    } > m_stack


    /******************************************************************/
    .ncache.init : AT (_sidata + SIZEOF(.data))
    {
        /*. = ORIGIN(m_ncache); */
        __noncachedata_start__ = ABSOLUTE(.);
        __base_NCACHE_REGION = ABSOLUTE(.);
        Image$$RW_m_ncache$$Base = ABSOLUTE(.);

        *(NonCacheable.init)

        . = ALIGN (4);
        __noncachedata_init_end__ = ABSOLUTE(.);
    } > m_ncache

    .ncache :
    {
        /*. = ABSOLUTE(__noncachedata_init_end__);*/
        *(NonCacheable)

        /*. = ORIGIN(m_ncache) + LENGTH(m_ncache);*/
        __noncachedata_end__ = ABSOLUTE(.);
        __top_NCACHE_REGION = ABSOLUTE(.);
        Image$$RW_m_ncache_unused$$ZI$$Limit = ABSOLUTE(.);
    } > m_ncache

    __NCACHE_REGION_START = ORIGIN(m_ncache);
    __NCACHE_REGION_SIZE  = LENGTH(m_ncache);

    .ARM.attributes 0 : { *(.ARM.attributes) }
}
