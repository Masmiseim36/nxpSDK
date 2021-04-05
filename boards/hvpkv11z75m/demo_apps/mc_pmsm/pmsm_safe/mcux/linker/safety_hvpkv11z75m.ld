/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

GROUP (
  libcr_c.a
  libcr_eabihelpers.a
)

/*#####################    RAM memory organization    #########################

                           ____________________ ___ [__RAM_end__]
                         0|                    |   ^
                         0|   RAM Test Backup  |   | [__size_fs_ram_tst_bckp__]
  [m_fs_ram_tst_bckp]___ 0|____________________| __v
         [m_data_end]    0|                    |
                         0|                    |
                         0|                    |
                         0|   Non-safety Data  |
                         0|                    |
                         0|                    |
                         0|____________________|
                         #|                    |
                         #|                    |
                         #|     Safety RAM     |
                         #|   (.safety_ram)    |
                         #|                    |
       [m_data_start]___ #|____________________| __
                         #|                    |   ^
                         #|  Safety Error Code |   | [0x04]
    [m_fs_error_code]___ #|____________________| __v
                         #|                    |   ^
                         #|    PC Test Flag    |   | [0x04]
   [m_fs_pc_tst_flag]___ #|____________________| __v
                         #|                    |   ^
                         #|   WDOG Test Data   |   | [__size_fs_wdog_tst_data__]
 [m_fs_wdog_tst_bckp]___ #|____________________| __v
  [m_fs_stack_tst_p4]    #|                    |   ^
                         #| Stack Test Pattern |   | [__size_fs_stack_tst_p__]
  [m_fs_stack_tst_p3]___ #|____________________| __v
      [m_stack_start]    #|                    |   ^
                         #|                    |   |
                         #|                    |   |
                         #|       STACK        |   | [__size_cstack__]
                         #|                    |   |
                         #|                    |   |
                         #|                    |   |
                     ___ #|____________________| __v
  [m_fs_stack_tst_p2]    #|                    |   ^
                         #| Stack Test Pattern |   | [__size_fs_stack_tst_p__]
  [m_fs_stack_tst_p1]___ #|____________________| __v___[__RAM_start__]
                         ^
                         |
           Checked by safety RAM test (#-Runtime; -No)



 #####################   FLASH memory organization    #########################

   [m_fs_flash_crc_end]___ ____________________ ___ __[__ROM_end__]
                          0|                    |   ^
                          0| Safety region CRC  |   | [__size_flash_crc__]
 [m_fs_flash_crc_start]___0|____________________| __v
          [m_flash_end]   0|                    |
                          0|                    |
                          0|                    |
                          0|                    |
                          0|                    |
                          0|  Non-safety FLASH  |
                          0|                    |
                          0|                    |
                          0|                    |
                          0|                    |
                          0|                    |
                          0|____________________|
                          #|                    |
                          #|  Safety RAM init   |
                          #|____________________|
                          #|                    |
                          #|                    |
                          #|                    |
                          #|    Safety FLASH    |
                          #|                    |
                          #|                    |
                          #|                    |
        [m_flash_start]___#|____________________| __
  [m_fs_pc_tst_obj_end]   #|                    |   ^
                          #|   PC test object   |   | [__size_fs_pc_tst_obj__]
[m_fs_pc_tst_obj_start]___#|____________________| __v
   [m_flash_config_end]   #|                    |   ^
                          #|    FLASH Config    |   | [0x10]
 [m_flash_config_start]___#|____________________| __v
   [m_intvec_table_end]   #|                    |   ^
                          #|                    |   |
                          #|    Vector table    |   | [0x400]
                          #|     (.intvec)      |   |
 [m_intvec_table_start]___#|____________________|___v__[__ROM_start__]
                          ^
                          |
    Checked by safety FLASH test (0-No; #-Runtime)


*/
/******************************************************************************/
/**********************     USER CONFIGURATION PART    ************************/
/******************************************************************************/
/* FLASH memory boundaries. */
__ROM_start__             = 0x00000000;
__ROM_end__               = 0x00017FFF;

/* RAM memory boundaries. */
__RAM_start__             = 0x1FFFF000;
__RAM_end__               = 0x20002FFF;

/* Sizes of objects in RAM. */
__size_cstack__           = 0x300;  /* Stack size. */
__size_fs_ram_tst_bckp__  = 0x20;   /* Safety RAM test backup size. */
__size_fs_wdog_tst_data__ = 0x1C;   /* Safety WDOG test data size. */
__size_fs_stack_tst_p__   = 0x10;   /* Safety stack test pattern. */

/* Sizes of objects in FLASH. */
__size_fs_pc_tst_obj__    = 0x20;
__size_intvec_table__     = 0x400;
__size_flash_cfg_table__  = 0x10;
__size_flash_crc__        = 0x10;

/******************************************************************************/
/*******************************     SYMBOLS     ******************************/
/******************************************************************************/
/* Assemble RAM addresses. */
m_fs_stack_tst_p1      = __RAM_start__;
m_fs_stack_tst_p2      = m_fs_stack_tst_p1  + __size_fs_stack_tst_p__ - 0x4;
m_stack_start          = m_fs_stack_tst_p2  + __size_cstack__ + 0x4;
m_fs_stack_tst_p3      = m_stack_start      + 0x4;
m_fs_stack_tst_p4      = m_fs_stack_tst_p3  + __size_fs_stack_tst_p__ - 0x4;
m_fs_wdog_tst_bckp     = m_fs_stack_tst_p4  + 0x4;
m_fs_pc_tst_flag       = m_fs_wdog_tst_bckp + __size_fs_wdog_tst_data__;
m_fs_error_code        = m_fs_pc_tst_flag   + 0x4;
m_data_start           = m_fs_error_code    + 0x4;

m_fs_ram_tst_bckp      = __RAM_end__ - __size_fs_ram_tst_bckp__ + 0x1;
m_data_end             = m_fs_ram_tst_bckp  - 0x1;

/* Assemble FLASH addresses. */
m_intvec_table_start   = __ROM_start__;
m_intvec_table_end     = m_intvec_table_start  + __size_intvec_table__ - 0x1;
m_flash_config_start   = m_intvec_table_end    + 0x1;
m_flash_config_end     = m_flash_config_start  + __size_flash_cfg_table__ - 0x1;
m_fs_pc_tst_obj_start  = m_flash_config_end    + 0x1;
m_fs_pc_tst_obj_end    = m_fs_pc_tst_obj_start + __size_fs_pc_tst_obj__ - 0x1;
m_flash_start          = m_fs_pc_tst_obj_end   + 0x1;

m_fs_flash_crc_end     = __ROM_end__;
m_fs_flash_crc_start   = m_fs_flash_crc_end    - __size_flash_crc__ + 0x1;
m_flash_end            = m_fs_flash_crc_start - 0x1;


MEMORY
{
  /* Define each memory region */
  MEM_FLASH (rx) : ORIGIN = __ROM_start__, LENGTH = (__ROM_end__ - __ROM_start__ + 1)
  MEM_RAM  (rwx) : ORIGIN = __RAM_start__, LENGTH = (__RAM_end__ - __RAM_start__ + 1)
}

ENTRY(IRQHNDLR_fsStart)

SECTIONS
{
    /* Safety-related code and read-only data section. */
    .SEC_FS_ROM : ALIGN(4)
    {
        FILL(0xff)

        /* The interrupt vector table. */
        . = m_intvec_table_start;
        KEEP(*(.intvec*))

        /* Flash configuration table. */
        . = m_flash_config_start;
        KEEP(*(.flshcfg*))

        /* PC test object. */
        . = m_fs_pc_tst_obj_start;
        KEEP(*iec60730b_cm0_pc_object.o(.text*))

        /* Safety-related FLASH code and RO data. */
        . = m_flash_start;
        *vectors_mcux.o(.text*)
        *vectors_mcux.o(.rodata*)
        *startup_mcux.o(.text*)
        *startup_mcux.o(.rodata*)
        *safety_periph_init.o(.text*)
        *safety_periph_init.o(.rodata*)
        *mcdrv_periph_init.o(.text*)
        *mcdrv_periph_init.o(.rodata*)
        *main.o(.text*)
        *main.o(.rodata*)
        *safety_routines.o(.text*)
        *safety_routines.o(.rodata*)
        *iec60730b_clock.o(.text*)
        *iec60730b_clock.o(.rodata*)
        *iec60730b_cm0_pc.o(.text*)
        *iec60730b_cm0_pc.o(.rodata*)
        *iec60730b_cm0_ram.o(.text*)
        *iec60730b_cm0_ram.o(.rodata*)
        *iec60730b_cm0_reg.o(.text*)
        *iec60730b_cm0_reg.o(.rodata*)
        *iec60730b_cm0_stack.o(.text*)
        *iec60730b_cm0_stack.o(.rodata*)
        *iec60730b_wdog.o(.text*)
        *iec60730b_wdog.o(.rodata*)
        *iec60730b_cm0_flash.o(.text*)
        *iec60730b_cm0_flash.o(.rodata*)
        *m1_state_machine.o(.text*)
        *m1_state_machine.o(.rodata*)
        *mcdrv_adc_adc16.o(.text*)
        *mcdrv_adc_adc16.o(.rodata*)
        *mcdrv_pwm3ph_ftm.o(.text*)
        *mcdrv_pwm3ph_ftm.o(.rodata*)
        *mcdrv_gpio.o(.text*)
        *mcdrv_gpio.o(.rodata*)
        *AMCLIB_PMSMBemfObsrvDQ_A32_Asm.o(.text*)
        *AMCLIB_PMSMBemfObsrvDQ_A32_Asm.o(.rodata*)
        *AMCLIB_TrackObsrv_F32_Asm.o(.text*)
        *AMCLIB_TrackObsrv_F32_Asm.o(.rodata*)
        *GDFLIB_FilterIIR1_F32_Asm.o(.text*)
        *GDFLIB_FilterIIR1_F32_Asm.o(.rodata*)
        *GMCLIB_ElimDcBusRip_F16_Asm.o(.text*)
        *GMCLIB_ElimDcBusRip_F16_Asm.o(.rodata*)
        *GMCLIB_Svm_F16_Asm.o(.text*)
        *GMCLIB_Svm_F16_Asm.o(.rodata*)
        *GFLIB_AtanYX_F32_Asm.o(.text*)
        *GFLIB_AtanYX_F32_Asm.o(.rodata*)
        *GFLIB_Atan_F32_Asm.o(.text*)
        *GFLIB_Atan_F32_Asm.o(.rodata*)
        *GFLIB_SinCos_A32_Asm.o(.text*)
        *GFLIB_SinCos_A32_Asm.o(.rodata*)
        . = . + 1;
        . = ALIGN(4);
    } >MEM_FLASH


    /* The safety-related RAM. */
    .SEC_FS_RAM m_data_start : AT (ADDR(.SEC_FS_ROM) + SIZEOF(.SEC_FS_ROM))
    {
    	m_sec_fs_ram_load_start = LOADADDR(.SEC_FS_RAM);
    	m_sec_fs_ram_start = .;
    	*(.safety_ram*)
        *gflib_sincos_a32.o(.data*)
        *gflib_sqrt_f16.o(.data*)
        *gflib_atan_f32.o(.data*)
        . = . + 1;
        . = ALIGN(4);
        m_sec_fs_ram_load_end = LOADADDR (.SEC_FS_RAM) + SIZEOF(.SEC_FS_RAM);
        m_sec_fs_ram_end = .;

        /* The end of safety-related FLASH memory. */
        m_safety_flash_end = LOADADDR (.SEC_FS_RAM) + SIZEOF(.SEC_FS_RAM);
    } >MEM_RAM


    /* The non-safety RW data. */
    .SEC_RWRAM m_sec_fs_ram_end : AT (m_safety_flash_end)
    {
       m_sec_rwram_load_start = LOADADDR(.SEC_RWRAM);
       m_sec_rwram_start = .;
       *(.data*)
       . = . + 1;
       . = ALIGN(4);
       m_sec_rwram_load_end = LOADADDR(.SEC_RWRAM) + SIZEOF(.SEC_RWRAM);
       m_sec_rwram_end = .;
    } >MEM_RAM

    /* The non-safety code and RO data. */
    .SEC_ROM m_sec_rwram_load_end : ALIGN(4)
    {
        FILL(0xff)
        *(.text*)
        KEEP(*(.rodata .rodata.* .constdata .constdata.*))
        . = . + 1;
        . = ALIGN(4);
        *(.tsacode*)
        *(.tsadata*)
        *(.tsaentry*)
        . = . + 1;
        . = ALIGN(4);
    } >MEM_FLASH

    /* The safety FLASH CRC. */
    .SEC_CRC m_fs_flash_crc_start : ALIGN(4)
    {
        FILL(0xff)
        KEEP(*(.flshcrc*))
    } >MEM_FLASH

    /* Stack memory. */
    stack (m_stack_start - __size_cstack__) : ALIGN(4)
    {
        . = ALIGN(4);
    } > MEM_RAM

    /* The zero-initialized RW data. */
    .SEC_BSS m_sec_rwram_end : ALIGN(4)
    {
       m_sec_bss_start = .;
       *(.bss*)
       *(COMMON)
       . = . + 1;
       . = ALIGN(4);
       m_sec_bss_end = .;
    } >MEM_RAM
}

