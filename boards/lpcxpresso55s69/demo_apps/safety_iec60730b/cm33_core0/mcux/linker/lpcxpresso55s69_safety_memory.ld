/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*     
 *     Processors:          LPC55S69JBD100_cm33_core0
 *                          LPC55S69JBD64_cm33_core0
 *                          LPC55S69JEV98_cm33_core0
 */

/**************************************************************/
/*    |            | --> data region                          */
/*    |            |                                          */
/*    |____________|                                          */
/*    |____________| --> STACK_TEST_P_1     ....ADR           */
/*    |____________|                        ....ADR + 0x4     */
/*    |____________|                        ....ADR + 0x8     */
/*    |____________| --> STACK_TEST_P_2     ....ADR + 0xC     */
/*    |            |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |   STACK    |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |            |                                          */
/*    |____________| --> __BOOT_STACK_ADDRESS                 */
/*    |____________| --> STACK_TEST_P_3                       */
/*    |____________|                                          */
/*    |____________|                                          */
/*    |____________| --> STACK_TEST_P_4                       */
/*    |____________| --> SAFETY_ERROR_CODE                    */
/*    |____________| --> PC_test_flag                         */
/*    |            | --> WD_TEST_BACKUP                       */
/*    |____________|                                          */
/*    |            |                                          */
/*    |            | --> RAM_TEST_BACKUP                      */
/*    |____________|                                          */
/**************************************************************/

__ROM_start__  = 0x00000140;
__ROM_end__    = 0x00071FFF; 

__RAM_start__  = 0x20000000;
__RAM_end__    = 0x200317FF;

stack_test_block_size = 0x10;
ram_test_backup_size  = 0x20;
__size_cstack__       = 0x400;

__FlashCRC_start__ = 0xFFF0; /* for placing a checksum */
__FlashCRC_end__   = 0xFFFF; /* for placing a checksum */

__VECTOR_TABLE  = 0x00000000;

SAFETY_RAM_SIZE = 0x300;

__size_heap__   = 0x40;     /* 2x  heap and heap2stackfill */

/******************************************************************************/
/******************     SYMBOLS     *******************************************/
/******************************************************************************/
__PC_test_size    = 0x270; /* 0x1EE */
__PC_test_end__   = __ROM_end__;
__PC_test_start__ = __PC_test_end__ - __PC_test_size;

m_flash_end           =   __PC_test_start__ - 0x1;

m_data_start          =  __RAM_start__;

m_ram_test_backup     = __RAM_end__ - ram_test_backup_size + 0x1;  
m_wd_test_backup      = m_ram_test_backup - 0x20;                       
m_pc_test_flag        = m_wd_test_backup - 0x4;                         
m_safety_error_code   = m_pc_test_flag - 0x4;                           

m_stack_test_p_4      = m_safety_error_code - 0x4;                      
m_stack_test_p_3      = m_stack_test_p_4 - stack_test_block_size +0x4; 
__BOOT_STACK_ADDRESS  = m_stack_test_p_3 - 0x4;                        
m_stack_test_p_2      = __BOOT_STACK_ADDRESS - __size_cstack__ -0x4;   
m_stack_test_p_1      = m_stack_test_p_2 - stack_test_block_size +0x4; 

m_safety_ram_end      = m_stack_test_p_1 - 0x1;
m_safety_ram_start    = m_safety_ram_end - SAFETY_RAM_SIZE + 0x01;  

m_data_end            = m_safety_ram_start - 0x01;
m_data_size           = m_data_end - m_data_start;             


MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = m_flash_end + 1 /* 608K bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = m_data_size /* 272K bytes (alias RAM) */  
  SRAMX (rwx) : ORIGIN = 0x4000000, LENGTH = 0x8000 /* 32K bytes (alias RAM2) */  
  PCTEST_region (rx) : ORIGIN = __PC_test_start__, LENGTH = __PC_test_size
}

/* Define a symbol for the top of each memory region */
__base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
__base_Flash = 0x0 ; /* Flash */  
__top_PROGRAM_FLASH = 0x0 + 0x98000 ; /* 608K bytes */  
__top_Flash = 0x0 + 0x98000 ; /* 608K bytes */  
__base_SRAM = 0x20000000  ; /* SRAM */  
__base_RAM = 0x20000000 ; /* RAM */  
__top_SRAM = 0x20000000 + m_data_size ; /* 272K bytes */  
__top_RAM = 0x20000000 + m_data_size ; /* 272K bytes */  
__base_SRAMX = 0x4000000  ; /* SRAMX */  
__base_RAM2 = 0x4000000 ; /* RAM2 */  
__top_SRAMX = 0x4000000 + 0x8000 ; /* 32K bytes */  
__top_RAM2 = 0x4000000 + 0x8000 ; /* 32K bytes */  
__base_USB_RAM = 0x40100000  ; /* USB_RAM */  
__base_RAM3 = 0x40100000 ; /* RAM3 */  
__top_USB_RAM = 0x40100000 + 0x4000 ; /* 16K bytes */  
__top_RAM3 = 0x40100000 + 0x4000 ; /* 16K bytes */  
  
