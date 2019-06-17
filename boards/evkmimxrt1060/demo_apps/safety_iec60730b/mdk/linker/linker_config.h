/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/
/******************************************************************************/
/******************     USER CONFIGURATION PART     ***************************/
/******************************************************************************/
#ifndef __LINKER_CONFIG__
    #define __LINKER_CONFIG__

    #define stack_test_block_size 0x10
    #define ram_test_backup_size  0x20
    #define __vector_table_size__ 0x400
    #define __size_cstack__       0x0400
    #define SAFETY_RAM_SIZE       0x300
    #define SAFETY_RAM_CRC_SIZE   0x200

    #define m_flash_config_start 0x60000000
    #define m_flash_config_size  0x00001000

    #define m_ivt_start          0x60001000
    #define m_ivt_size           0x00001000

    #define RAM_interrupts_start 0x00000000 /* IVT in RAM */
    #define RAM_interrupts_end   RAM_interrupts_start + __vector_table_size__

    #define SRAM_ITC_start       0x00000000 /* Part of 512KB on-chip RAM (Instruction-Tightly-Coupled) */
    #define SRAM_ITC_end         0x0001FFFF

    #define SRAM_DTC_start       0x20000000 /* Part of 512KB on-chip RAM (Data-Tightly-Coupled) */
    #define SRAM_DTC_end         0x2001FFFF

    #define ROM_interrupts_start 0x60002000 /* IVT in ROM */
    #define ROM_interrupts_size  __vector_table_size__

    #define __FlashCRC_start__   0x60002400 /* Place checksum result after IVT (in ROM) */
    #define __FlashCRC_end__     0x6000240F

    #define __PC_test_start__    0x60002410 /* PC object address (pattern to PC:0x60002411) */
    #define __PC_test_end__      0x6000242F /* PC object reserved size: 0x20 */ 

    #define ROM_start            0x60002430 /* ~64MB of ROM memory (external ROM) */
    #define ROM_end              0x607FFFFF /* 0x64000000 (ROM_end) - 0x60002430 (ROM_start) */

    #define SDRAM_start          0x80000000 /* ~32MB of SDRAM memory (external RAM) */
    #define SDRAM_end            0x81FFFFFF

    /* Compatibility with placing IVT in vectors_mdk.c and setting VTOR in startup_mdk.c */
    #ifdef _DEBUG
        #define __VECTOR_TABLE   RAM_interrupts_start
        #define __ROM_start__    RAM_interrupts_start
    #else
        #define __VECTOR_TABLE   ROM_interrupts_start
        #define __ROM_start__    ROM_interrupts_start
    #endif /* _DEBUG */

    /***** NOT USED IN THIS EXAMPLE *****/
    #define SRAM_OC_start 0x20200000 /* Part of 512KB on-chip RAM */
    #define SRAM_OC_end   0x202BFFFF
    /***** NOT USED IN THIS EXAMPLE *****/

    /******************************************************************************/
    /******************     SYMBOLS     *******************************************/
    /******************************************************************************/
    #define m_ram_test_backup    SRAM_DTC_end - ram_test_backup_size + 0x1
    #define m_wd_test_backup     m_ram_test_backup - 0x10
    #define m_pc_test_flag       m_wd_test_backup - 0x4
    #define m_safety_error_code  m_pc_test_flag - 0x4
    #define m_stack_test_p_4     m_safety_error_code - 0x4
    #define m_stack_test_p_3     m_stack_test_p_4 - stack_test_block_size +0x4
    #define __BOOT_STACK_ADDRESS m_stack_test_p_3 - 0x4
    #define m_stack_test_p_2     __BOOT_STACK_ADDRESS - __size_cstack__
    #define m_stack_test_p_1     m_stack_test_p_2 - stack_test_block_size + 0x4

    #define m_safety_ram_end     m_stack_test_p_1 - 0x1
    #define m_safety_ram_start   m_safety_ram_end - SAFETY_RAM_SIZE + 0x01  
    
    #define m_safety_ram_crc_end     m_safety_ram_start - 0x1
    #define m_safety_ram_crc_start   m_safety_ram_crc_end - SAFETY_RAM_CRC_SIZE + 0x01  

    #define m_data_end           m_safety_ram_start - 0x01
    #define m_data_size          m_data_end - SRAM_DTC_start
    
#endif
