/*
 * (c) NXP Semiconductors 2013-2018
 * Linker script file for MIMXRT1062DVL6B
 */

/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2018
 * Generated linker script file for MIMXRT1062xxxxB
 * Created from library.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v10.2.0 [Build 759] [2018-05-15]  on Oct 23, 2018 12:41:58 PM
 */

GROUP (
  "libcr_c.a"
  "libcr_eabihelpers.a"
)

/*
 * (c) NXP Semiconductors 2013-2018
 * Linker script file for MIMXRT1062DVL6B
 */

/******************************************************************************/
/******************     USER CONFIGURATION PART     ***************************/
/******************************************************************************/
stack_test_block_size = 0x10;
ram_test_backup_size  = 0x20;
__vector_table_size__ = 0x400;
__size_cstack__       = 0x0400;

RAM_interrupts_start = 0x00000000; /* IVT in RAM */
RAM_interrupts_end   = RAM_interrupts_start + __vector_table_size__;

SRAM_ITC_start       = 0x00000400; /* On-chip RAM (Instruction-Tightly-Coupled) */
SRAM_ITC_end         = 0x00020000;

SRAM_DTC_start       = 0x20000000; /* On-chip RAM (Data-Tightly-Coupled) */
SRAM_DTC_end         = 0x20020000;

ROM_config_start     = 0x60000000;

ROM_interrupts_start = 0x60002000; /* IVT in ROM */

__FlashCRC_start__   = 0x60002400; /* Place checksum result after IVT (in ROM) */
__FlashCRC_end__     = 0x60002410;

__PC_test_start__    = 0x60002410; /* PC object address (pattern to PC:0x60002411) */
__PC_test_end__      = 0x60002430; /* PC object reserved size: 0x20 */ 

ROM_start            = 0x60002430; /* ROM memory (external ROM) */
ROM_end              = 0x60800000;

SDRAM_start          = 0x80000000; /* ~32MB of SDRAM memory (external RAM) */
SDRAM_end            = 0x82000000;

/***** NOT USED IN THIS EXAMPLE *****/
SRAM_OC_start        = 0x20200000; /* On-chip RAM */
SRAM_OC_end          = 0x202C0000;
/***** NOT USED IN THIS EXAMPLE *****/

/******************************************************************************/
/******************     SYMBOLS     *******************************************/
/******************************************************************************/   
m_ram_test_backup    = SRAM_DTC_end - ram_test_backup_size;
m_wd_test_backup     = m_ram_test_backup - 0x10;
m_pc_test_flag       = m_wd_test_backup - 0x4;
m_safety_error_code  = m_pc_test_flag - 0x4;
m_stack_test_p_4     = m_safety_error_code - 0x4;
m_stack_test_p_3     = m_stack_test_p_4 - stack_test_block_size +0x4;
__BOOT_STACK_ADDRESS = m_stack_test_p_3 - 0x4;
m_stack_test_p_2     = __BOOT_STACK_ADDRESS - __size_cstack__;
m_stack_test_p_1     = m_stack_test_p_2 - stack_test_block_size + 0x4;
SRAM_DTC_end_edited  = m_stack_test_p_1 - 0x1;

MEMORY
{
  /* Define each memory region */
  ROM_config (rx) : ORIGIN = ROM_config_start, LENGTH = ROM_interrupts_start - ROM_config_start /* XIP boot header */
  BOARD_FLASH (rx) : ORIGIN = ROM_interrupts_start, LENGTH = ROM_end - ROM_start /* (alias Flash) */  
  SRAM_ITC (rwx) : ORIGIN = RAM_interrupts_start, LENGTH = SRAM_ITC_end - RAM_interrupts_start /* (alias RAM2) */  
  SRAM_DTC (rwx) : ORIGIN = SRAM_DTC_start, LENGTH = SRAM_DTC_end - SRAM_DTC_start /* (alias RAM) */  
  BOARD_SDRAM (rwx) : ORIGIN = SDRAM_start, LENGTH = SDRAM_end - SDRAM_start /* (alias RAM4) */ 
  /***** NOT USED IN THIS EXAMPLE *****/
  SRAM_OC (rwx) : ORIGIN = SRAM_OC_start, LENGTH = SRAM_OC_end - SRAM_OC_start /* (alias RAM3) */  
  /***** NOT USED IN THIS EXAMPLE *****/
}

__VECTOR_TABLE = ROM_interrupts_start;

ENTRY(ResetISR)

SECTIONS
{
    /* Image Vector Table and Boot Data for booting from external flash */
    .boot_hdr : ALIGN(4)
    {
        FILL(0xff)
        __boot_hdr_start__ = ABSOLUTE(.) ;
        KEEP(*(.boot_hdr.conf))
        . = 0x1000 ;
        KEEP(*(.boot_hdr.ivt))
        . = 0x1020 ;
        KEEP(*(.boot_hdr.boot_data))
        . = 0x1030 ;
        KEEP(*(.boot_hdr.dcd_data))
        __boot_hdr_end__ = ABSOLUTE(.) ;
        . = 0x2000 ;
    } >ROM_config

    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        /**** IVT ****/
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /**** IVT ****/
        
        /**** Reserved space for CRC calculated by srec_cat ****/
        . = ALIGN(4);
        FILL(0xff)
        . = (__FlashCRC_end__ - ROM_interrupts_start); /* Get the relative offset from 0x60002000 (should be 0x410) */
        /**** Reserved space for CRC calculated by srec_cat ****/

        /**** Reserved space for PC test object ****/
        . = ALIGN(4);
        *iec60730b_cm4_cm7_pc_object.o
        . = ALIGN(4);
        /**** Reserved space for PC test object ****/

        /* Global Section Table */
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM));
        LONG(    ADDR(.data_RAM));
        LONG(  SIZEOF(.data_RAM));
        __data_section_table_end = .;
        __safety_ram_section_table = .;
        LONG(LOADADDR(.safety_ram));
        LONG(    ADDR(.safety_ram));
        LONG(  SIZEOF(.safety_ram));
        __safety_ram_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM));
        LONG(  SIZEOF(.bss_RAM));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)       

    } >BOARD_FLASH

    .text : ALIGN(4)
    {
        *EXCLUDE_FILE(*iec60730b_cm4_cm7_pc_object.o)
        *(.text*)
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
    
/************************************************************************/
/************************************************************************/    
    /* DATA section for SRAM_DTC */
    .data_RAM : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM = .) ;
        *(.ramfunc.$RAM)
        *(.ramfunc.$SRAM_DTC)
        *(.data.$RAM*)
        *(.data.$SRAM_DTC*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM = .) ;
     } > SRAM_DTC AT>BOARD_FLASH

/************************************************************************/
    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM_DTC
    
/************************************************************************/    
    /* Main DATA section (SRAM_DTC) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       *(vtable) 
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > SRAM_DTC AT>BOARD_FLASH
    
/************************************************************************/    
    /* BSS section for SRAM_DTC */
    .bss_RAM : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM = .) ;
       *(.bss.$RAM*)
       *(.bss.$SRAM_DTC*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM = .) ;
    } > SRAM_DTC 
    
/************************************************************************/    
    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > SRAM_DTC
    
/************************************************************************/    
    /* NOINIT section for SRAM_DTC */
    .noinit_RAM (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM*)
       *(.noinit.$SRAM_DTC*)
       . = ALIGN(4) ;
    } > SRAM_DTC 
    
/************************************************************************/    
   
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > SRAM_DTC

/************************************************************************/    
   .safety_ram : ALIGN(4)    
   {
   _safety_ram = .;
   *(.safety_ram)
   . = ALIGN(4);
   _end_safety_ram = .;
   } > SRAM_DTC AT>BOARD_FLASH 
    
/************************************************************************/    
   .safety_ram_crc : ALIGN(4)    
   {
   _safety_ram_crc = .;
   *(.safety_ram_crc)
   . = ALIGN(4);
   _end_safety_ram_crc = .;
   } > SRAM_DTC AT>BOARD_FLASH
    
/************************************************************************/    
 
/************************************************************************/
    .stack __BOOT_STACK_ADDRESS - __size_cstack__ :  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);       
        _vStackTop = . + __size_cstack__;
    } > SRAM_DTC
}
