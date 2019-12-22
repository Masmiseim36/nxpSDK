/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*     
 *     Processors:          MIMXRT1021CAF4A
 *                          MIMXRT1021CAG4A
 *                          MIMXRT1021DAF5A
 *                          MIMXRT1021DAG5A
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

SRAM_ITC_start       = 0x00000400; /* Part of on-chip RAM (Instruction-Tightly-Coupled) */
SRAM_ITC_end         = 0x00010000;

SRAM_DTC_start       = 0x20000000; /* Part of on-chip RAM (Data-Tightly-Coupled) */
SRAM_DTC_end         = 0x20010000;

ROM_config_start     = 0x60000000;

ROM_interrupts_start = 0x60002000; /* IVT in ROM */

__FlashCRC_start__   = 0x60002400; /* Place checksum result after IVT (in ROM) */
__FlashCRC_end__     = 0x60002410;

__PC_test_start__    = 0x60002410; /* PC object address (pattern to PC:0x60002411) */
__PC_test_end__      = 0x60002530; /* PC object reserved size: 0x20 */ 

ROM_start            = 0x60002530; /* ROM memory (external ROM) */
ROM_end              = 0x60800000;

SDRAM_start          = 0x80000000; /* ~32MB of SDRAM memory (external RAM) */
SDRAM_end            = 0x82000000;

/***** NOT USED IN THIS EXAMPLE *****/
SRAM_OC_start        = 0x20200000; /* Part of on-chip RAM */
SRAM_OC_end          = 0x20220000;
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
  BOARD_FLASH (rx) : ORIGIN = ROM_interrupts_start, LENGTH = ROM_end - ROM_start /* 64M bytes (alias Flash) */  
  SRAM_ITC (rwx) : ORIGIN = RAM_interrupts_start, LENGTH = SRAM_ITC_end - RAM_interrupts_start /* 128K bytes (alias RAM2) */  
  SRAM_DTC (rwx) : ORIGIN = SRAM_DTC_start, LENGTH = SRAM_DTC_end - SRAM_DTC_start /* 128K bytes (alias RAM3) */  
  BOARD_SDRAM (rwx) : ORIGIN = SDRAM_start, LENGTH = SDRAM_end - SDRAM_start /* 32M bytes (alias RAM4) */ 
  /***** NOT USED IN THIS EXAMPLE *****/
  SRAM_OC (rwx) : ORIGIN = SRAM_OC_start, LENGTH = SRAM_OC_end - SRAM_OC_start /* 256K bytes (alias RAM) */  
  /***** NOT USED IN THIS EXAMPLE *****/
}
