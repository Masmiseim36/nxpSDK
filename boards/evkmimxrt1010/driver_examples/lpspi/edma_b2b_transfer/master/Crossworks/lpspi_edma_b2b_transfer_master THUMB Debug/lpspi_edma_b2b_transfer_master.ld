MEMORY
{
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  ITCM (wx) : ORIGIN = 0x00000000, LENGTH = 0x00008000
  DTCM (wx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
  OCRAM (wx) : ORIGIN = 0x20200000, LENGTH = 0x00010000
  FlexSPI (rx) : ORIGIN = 0x60000000, LENGTH = 0x04000000
}

SECTIONS
{
  __ITCM_segment_start__ = 0x00000000;
  __ITCM_segment_end__ = 0x00008000;
  __ITCM_segment_size__ = 0x00008000;
  __DTCM_segment_start__ = 0x20000000;
  __DTCM_segment_end__ = 0x20008000;
  __DTCM_segment_size__ = 0x00008000;
  __OCRAM_segment_start__ = 0x20200000;
  __OCRAM_segment_end__ = 0x20210000;
  __OCRAM_segment_size__ = 0x00010000;
  __FlexSPI_segment_start__ = 0x60000000;
  __FlexSPI_segment_end__ = 0x64000000;
  __FlexSPI_segment_size__ = 0x04000000;

  __HEAPSIZE__ = 1024;
  __STACKSIZE_ABT__ = 0;
  __STACKSIZE_FIQ__ = 0;
  __STACKSIZE_IRQ__ = 0;
  __STACKSIZE_PROCESS__ = 0;
  __STACKSIZE_SVC__ = 0;
  __STACKSIZE_UND__ = 0;
  __STACKSIZE__ = 1024;

  __init_rodata_load_start__ = ALIGN(__DTCM_segment_start__ , 4);
  .init_rodata ALIGN(__DTCM_segment_start__ , 4) : AT(ALIGN(__DTCM_segment_start__ , 4))
  {
    __init_rodata_start__ = .;
    *(.init_rodata .init_rodata.*)
  }
  __init_rodata_end__ = __init_rodata_start__ + SIZEOF(.init_rodata);
  __init_rodata_size__ = SIZEOF(.init_rodata);
  __init_rodata_load_end__ = __init_rodata_end__;

  . = ASSERT(__init_rodata_start__ == __init_rodata_end__ || (__init_rodata_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .init_rodata is too large to fit in DTCM memory segment");

  __dtors_load_start__ = ALIGN(__init_rodata_end__ , 4);
  .dtors ALIGN(__init_rodata_end__ , 4) : AT(ALIGN(__init_rodata_end__ , 4))
  {
    __dtors_start__ = .;
    KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors)) KEEP (*(.fini_array)) KEEP (*(SORT(.fini_array.*)))
  }
  __dtors_end__ = __dtors_start__ + SIZEOF(.dtors);
  __dtors_size__ = SIZEOF(.dtors);
  __dtors_load_end__ = __dtors_end__;

  . = ASSERT(__dtors_start__ == __dtors_end__ || (__dtors_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .dtors is too large to fit in DTCM memory segment");

  __ctors_load_start__ = ALIGN(__dtors_end__ , 4);
  .ctors ALIGN(__dtors_end__ , 4) : AT(ALIGN(__dtors_end__ , 4))
  {
    __ctors_start__ = .;
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors)) KEEP (*(.init_array)) KEEP (*(SORT(.init_array.*)))
  }
  __ctors_end__ = __ctors_start__ + SIZEOF(.ctors);
  __ctors_size__ = SIZEOF(.ctors);
  __ctors_load_end__ = __ctors_end__;

  . = ASSERT(__ctors_start__ == __ctors_end__ || (__ctors_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .ctors is too large to fit in DTCM memory segment");

  __ARM.exidx_load_start__ = ALIGN(__ctors_end__ , 4);
  .ARM.exidx ALIGN(__ctors_end__ , 4) : AT(ALIGN(__ctors_end__ , 4))
  {
    __ARM.exidx_start__ = .;
    __exidx_start =   __ARM.exidx_start__;
    *(.ARM.exidx .ARM.exidx.*)
  }
  __ARM.exidx_end__ = __ARM.exidx_start__ + SIZEOF(.ARM.exidx);
  __ARM.exidx_size__ = SIZEOF(.ARM.exidx);
  __exidx_end =   __ARM.exidx_end__;
  __ARM.exidx_load_end__ = __ARM.exidx_end__;

  . = ASSERT(__ARM.exidx_start__ == __ARM.exidx_end__ || (__ARM.exidx_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .ARM.exidx is too large to fit in DTCM memory segment");

  __data_load_start__ = ALIGN(__ARM.exidx_end__ , 4);
  .data ALIGN(__ARM.exidx_end__ , 4) : AT(ALIGN(__ARM.exidx_end__ , 4))
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
  }
  __data_end__ = __data_start__ + SIZEOF(.data);
  __data_size__ = SIZEOF(.data);
  __data_load_end__ = __data_end__;

  . = ASSERT(__data_start__ == __data_end__ || (__data_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .data is too large to fit in DTCM memory segment");

  __data2_load_start__ = ALIGN(__data_end__ , 4);
  .data2 ALIGN(__data_end__ , 4) : AT(ALIGN(__data_end__ , 4))
  {
    __data2_start__ = .;
    *(.data2 .data2.*)
  }
  __data2_end__ = __data2_start__ + SIZEOF(.data2);
  __data2_size__ = SIZEOF(.data2);
  __data2_load_end__ = __data2_end__;

  . = ASSERT(__data2_start__ == __data2_end__ || (__data2_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .data2 is too large to fit in DTCM memory segment");

  __data_tcm_load_start__ = ALIGN(__data2_end__ , 4);
  .data_tcm ALIGN(__data2_end__ , 4) : AT(ALIGN(__data2_end__ , 4))
  {
    __data_tcm_start__ = .;
    *(.data_tcm .data_tcm.*)
  }
  __data_tcm_end__ = __data_tcm_start__ + SIZEOF(.data_tcm);
  __data_tcm_size__ = SIZEOF(.data_tcm);
  __data_tcm_load_end__ = __data_tcm_end__;

  . = ASSERT(__data_tcm_start__ == __data_tcm_end__ || (__data_tcm_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .data_tcm is too large to fit in DTCM memory segment");

  __rodata_load_start__ = ALIGN(__data_tcm_end__ , 4);
  .rodata ALIGN(__data_tcm_end__ , 4) : AT(ALIGN(__data_tcm_end__ , 4))
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  __rodata_end__ = __rodata_start__ + SIZEOF(.rodata);
  __rodata_size__ = SIZEOF(.rodata);
  __rodata_load_end__ = __rodata_end__;

  . = ASSERT(__rodata_start__ == __rodata_end__ || (__rodata_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .rodata is too large to fit in DTCM memory segment");

  __rodata2_load_start__ = ALIGN(__rodata_end__ , 4);
  .rodata2 ALIGN(__rodata_end__ , 4) : AT(ALIGN(__rodata_end__ , 4))
  {
    __rodata2_start__ = .;
    *(.rodata2 .rodata2.*)
  }
  __rodata2_end__ = __rodata2_start__ + SIZEOF(.rodata2);
  __rodata2_size__ = SIZEOF(.rodata2);
  __rodata2_load_end__ = __rodata2_end__;

  . = ASSERT(__rodata2_start__ == __rodata2_end__ || (__rodata2_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .rodata2 is too large to fit in DTCM memory segment");

  __rodata_tcm_load_start__ = ALIGN(__rodata2_end__ , 4);
  .rodata_tcm ALIGN(__rodata2_end__ , 4) : AT(ALIGN(__rodata2_end__ , 4))
  {
    __rodata_tcm_start__ = .;
    *(.rodata_tcm .rodata_tcm.*)
  }
  __rodata_tcm_end__ = __rodata_tcm_start__ + SIZEOF(.rodata_tcm);
  __rodata_tcm_size__ = SIZEOF(.rodata_tcm);
  __rodata_tcm_load_end__ = __rodata_tcm_end__;

  . = ASSERT(__rodata_tcm_start__ == __rodata_tcm_end__ || (__rodata_tcm_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .rodata_tcm is too large to fit in DTCM memory segment");

  __bss_load_start__ = ALIGN(__rodata_tcm_end__ , 4);
  .bss ALIGN(__rodata_tcm_end__ , 4) (NOLOAD) : AT(ALIGN(__rodata_tcm_end__ , 4))
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);
  __bss_size__ = SIZEOF(.bss);
  __bss_load_end__ = __bss_end__;

  . = ASSERT(__bss_start__ == __bss_end__ || (__bss_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .bss is too large to fit in DTCM memory segment");

  __tbss_load_start__ = ALIGN(__bss_end__ , 4);
  .tbss ALIGN(__bss_end__ , 4) (NOLOAD) : AT(ALIGN(__bss_end__ , 4))
  {
    __tbss_start__ = .;
    *(.tbss .tbss.*)
  }
  __tbss_end__ = __tbss_start__ + SIZEOF(.tbss);
  __tbss_size__ = SIZEOF(.tbss);
  __tbss_load_end__ = __tbss_end__;

  . = ASSERT(__tbss_start__ == __tbss_end__ || (__tbss_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .tbss is too large to fit in DTCM memory segment");

  __tdata_load_start__ = ALIGN(__tbss_end__ , 4);
  .tdata ALIGN(__tbss_end__ , 4) : AT(ALIGN(__tbss_end__ , 4))
  {
    __tdata_start__ = .;
    *(.tdata .tdata.*)
  }
  __tdata_end__ = __tdata_start__ + SIZEOF(.tdata);
  __tdata_size__ = SIZEOF(.tdata);
  __tdata_load_end__ = __tdata_end__;

  . = ASSERT(__tdata_start__ == __tdata_end__ || (__tdata_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .tdata is too large to fit in DTCM memory segment");

  __bss2_load_start__ = ALIGN(__tdata_end__ , 4);
  .bss2 ALIGN(__tdata_end__ , 4) (NOLOAD) : AT(ALIGN(__tdata_end__ , 4))
  {
    __bss2_start__ = .;
    *(.bss2 .bss2.*)
  }
  __bss2_end__ = __bss2_start__ + SIZEOF(.bss2);
  __bss2_size__ = SIZEOF(.bss2);
  __bss2_load_end__ = __bss2_end__;

  . = ASSERT(__bss2_start__ == __bss2_end__ || (__bss2_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .bss2 is too large to fit in DTCM memory segment");

  __bss_tcm_load_start__ = ALIGN(__bss2_end__ , 4);
  .bss_tcm ALIGN(__bss2_end__ , 4) (NOLOAD) : AT(ALIGN(__bss2_end__ , 4))
  {
    __bss_tcm_start__ = .;
    *(.bss_tcm .bss_tcm.*)
  }
  __bss_tcm_end__ = __bss_tcm_start__ + SIZEOF(.bss_tcm);
  __bss_tcm_size__ = SIZEOF(.bss_tcm);
  __bss_tcm_load_end__ = __bss_tcm_end__;

  . = ASSERT(__bss_tcm_start__ == __bss_tcm_end__ || (__bss_tcm_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .bss_tcm is too large to fit in DTCM memory segment");

  __non_init_load_start__ = ALIGN(__bss_tcm_end__ , 4);
  .non_init ALIGN(__bss_tcm_end__ , 4) (NOLOAD) : AT(ALIGN(__bss_tcm_end__ , 4))
  {
    __non_init_start__ = .;
    *(.non_init .non_init.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);
  __non_init_size__ = SIZEOF(.non_init);
  __non_init_load_end__ = __non_init_end__;

  . = ASSERT(__non_init_start__ == __non_init_end__ || (__non_init_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .non_init is too large to fit in DTCM memory segment");

  __heap_load_start__ = ALIGN(__non_init_end__ , 4);
  .heap ALIGN(__non_init_end__ , 4) (NOLOAD) : AT(ALIGN(__non_init_end__ , 4))
  {
    __heap_start__ = .;
    *(.heap .heap.*)
    . = ALIGN(MAX(__heap_start__ + __HEAPSIZE__ , .), 4);
  }
  __heap_end__ = __heap_start__ + SIZEOF(.heap);
  __heap_size__ = SIZEOF(.heap);
  __heap_load_end__ = __heap_end__;

  . = ASSERT(__heap_start__ == __heap_end__ || (__heap_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .heap is too large to fit in DTCM memory segment");

  __stack_load_start__ = ALIGN(__heap_end__ , 8);
  .stack ALIGN(__heap_end__ , 8) (NOLOAD) : AT(ALIGN(__heap_end__ , 8))
  {
    __stack_start__ = .;
    *(.stack .stack.*)
    . = ALIGN(MAX(__stack_start__ + __STACKSIZE__ , .), 8);
  }
  __stack_end__ = __stack_start__ + SIZEOF(.stack);
  __stack_size__ = SIZEOF(.stack);
  __stack_load_end__ = __stack_end__;

  . = ASSERT(__stack_start__ == __stack_end__ || (__stack_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .stack is too large to fit in DTCM memory segment");

  __stack_process_load_start__ = ALIGN(__stack_end__ , 8);
  .stack_process ALIGN(__stack_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_end__ , 8))
  {
    __stack_process_start__ = .;
    *(.stack_process .stack_process.*)
    . = ALIGN(MAX(__stack_process_start__ + __STACKSIZE_PROCESS__ , .), 8);
  }
  __stack_process_end__ = __stack_process_start__ + SIZEOF(.stack_process);
  __stack_process_size__ = SIZEOF(.stack_process);
  __stack_process_load_end__ = __stack_process_end__;

  __DTCM_segment_used_end__ = ALIGN(__stack_end__ , 8) + SIZEOF(.stack_process);
  __DTCM_segment_used_size__ = __DTCM_segment_used_end__ - __DTCM_segment_start__;

  . = ASSERT(__stack_process_start__ == __stack_process_end__ || (__stack_process_end__ - __DTCM_segment_start__) <= __DTCM_segment_size__ , "error: .stack_process is too large to fit in DTCM memory segment");

  __vectors_load_start__ = ALIGN(__ITCM_segment_start__ , 256);
  .vectors ALIGN(__ITCM_segment_start__ , 256) : AT(ALIGN(__ITCM_segment_start__ , 256))
  {
    __vectors_start__ = .;
    *(.vectors .vectors.*)
  }
  __vectors_end__ = __vectors_start__ + SIZEOF(.vectors);
  __vectors_size__ = SIZEOF(.vectors);
  __vectors_load_end__ = __vectors_end__;

  . = ASSERT(__vectors_start__ == __vectors_end__ || (__vectors_end__ - __ITCM_segment_start__) <= __ITCM_segment_size__ , "error: .vectors is too large to fit in ITCM memory segment");

  __init_load_start__ = ALIGN(__vectors_end__ , 4);
  .init ALIGN(__vectors_end__ , 4) : AT(ALIGN(__vectors_end__ , 4))
  {
    __init_start__ = .;
    *(.init .init.*)
  }
  __init_end__ = __init_start__ + SIZEOF(.init);
  __init_size__ = SIZEOF(.init);
  __init_load_end__ = __init_end__;

  . = ASSERT(__init_start__ == __init_end__ || (__init_end__ - __ITCM_segment_start__) <= __ITCM_segment_size__ , "error: .init is too large to fit in ITCM memory segment");

  __fast_load_start__ = ALIGN(__init_end__ , 4);
  .fast ALIGN(__init_end__ , 4) : AT(ALIGN(__init_end__ , 4))
  {
    __fast_start__ = .;
    *(.fast .fast.*)
  }
  __fast_end__ = __fast_start__ + SIZEOF(.fast);
  __fast_size__ = SIZEOF(.fast);
  __fast_load_end__ = __fast_end__;

  . = ASSERT(__fast_start__ == __fast_end__ || (__fast_end__ - __ITCM_segment_start__) <= __ITCM_segment_size__ , "error: .fast is too large to fit in ITCM memory segment");

  __text_load_start__ = ALIGN(__fast_end__ , 4);
  .text ALIGN(__fast_end__ , 4) : AT(ALIGN(__fast_end__ , 4))
  {
    __text_start__ = .;
    *(.text .text.* .glue_7t .glue_7 .gnu.linkonce.t.* .gcc_except_table .ARM.extab* .gnu.linkonce.armextab.*)
  }
  __text_end__ = __text_start__ + SIZEOF(.text);
  __text_size__ = SIZEOF(.text);
  __text_load_end__ = __text_end__;

  . = ASSERT(__text_start__ == __text_end__ || (__text_end__ - __ITCM_segment_start__) <= __ITCM_segment_size__ , "error: .text is too large to fit in ITCM memory segment");

  __text2_load_start__ = ALIGN(__text_end__ , 4);
  .text2 ALIGN(__text_end__ , 4) : AT(ALIGN(__text_end__ , 4))
  {
    __text2_start__ = .;
    *(.text2 .text2.*)
  }
  __text2_end__ = __text2_start__ + SIZEOF(.text2);
  __text2_size__ = SIZEOF(.text2);
  __text2_load_end__ = __text2_end__;

  . = ASSERT(__text2_start__ == __text2_end__ || (__text2_end__ - __ITCM_segment_start__) <= __ITCM_segment_size__ , "error: .text2 is too large to fit in ITCM memory segment");

  __text_tcm_load_start__ = ALIGN(__text2_end__ , 4);
  .text_tcm ALIGN(__text2_end__ , 4) : AT(ALIGN(__text2_end__ , 4))
  {
    __text_tcm_start__ = .;
    *(.text_tcm .text_tcm.*)
  }
  __text_tcm_end__ = __text_tcm_start__ + SIZEOF(.text_tcm);
  __text_tcm_size__ = SIZEOF(.text_tcm);
  __text_tcm_load_end__ = __text_tcm_end__;

  __ITCM_segment_used_end__ = ALIGN(__text2_end__ , 4) + SIZEOF(.text_tcm);
  __ITCM_segment_used_size__ = __ITCM_segment_used_end__ - __ITCM_segment_start__;

  . = ASSERT(__text_tcm_start__ == __text_tcm_end__ || (__text_tcm_end__ - __ITCM_segment_start__) <= __ITCM_segment_size__ , "error: .text_tcm is too large to fit in ITCM memory segment");

}

