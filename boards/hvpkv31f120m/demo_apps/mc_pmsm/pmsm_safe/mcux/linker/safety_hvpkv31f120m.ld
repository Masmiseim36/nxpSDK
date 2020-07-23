/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

INCLUDE "safety_hvpkv31f120m_library.ld"
INCLUDE "safety_hvpkv31f120m_memory.ld"

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
        KEEP(*IEC60730_B_CM4_CM7_pc_object*(.text*))

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
        *IEC60730_B_CM4_CM7_clock.o(.text*)
        *IEC60730_B_CM4_CM7_clock.o(.rodata*)
        *IEC60730_B_CM4_CM7_pc.o(.text*)
        *IEC60730_B_CM4_CM7_pc.o(.rodata*)
        *IEC60730_B_CM4_CM7_ram.o(.text*)
        *IEC60730_B_CM4_CM7_ram.o(.rodata*)
        *IEC60730_B_CM4_CM7_reg.o(.text*)
        *IEC60730_B_CM4_CM7_reg.o(.rodata*)
        *IEC60730_B_CM4_CM7_stack.o(.text*)
        *IEC60730_B_CM4_CM7_stack.o(.rodata*)
        *IEC60730_B_CM4_CM7_wdg.o(.text*)
        *IEC60730_B_CM4_CM7_wdg.o(.rodata*)
        *IEC60730_B_CM4_CM7_flash.o(.text*)
        *IEC60730_B_CM4_CM7_flash.o(.rodata*)
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

