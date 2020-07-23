/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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

__ROM_start__         = 0x00000000;
__ROM_end__           = 0x00020000; 

__RAM_start__     = 0x1ffff410;
__RAM_end__       = 0x20002eef;

stack_test_block_size = 0x10;
ram_test_backup_size  = 0x20;
__size_cstack__       = 0x400;

__FlashCRC_start__ = 0x6FF0; /* for placing a checksum */
__FlashCRC_end__   = 0x6FFF; /* for placing a checksum */

__ram_vector_table_size__ = 0x400;
__VECTOR_TABLE  = __ROM_start__;
__VECTOR_RAM    = __RAM_start__;
__RAM_VECTOR_TABLE_SIZE = __ram_vector_table_size__;

SAFETY_RAM_SIZE  = 0x300;

__size_heap__    = 0x40;     /* 2x  heap and heap2stackfill */

/******************************************************************************/
/******************     SYMBOLS     *******************************************/
/******************************************************************************/
m_flash_config_start =    0x00000400;
m_flash_config_end   =    0x00000410;
m_flash_config_size  =    m_flash_config_end - m_flash_config_start;

__PC_test_size =     0x1EE;
__PC_test_end__ 	=	__ROM_end__;
__PC_test_start__ =	__PC_test_end__ - __PC_test_size;

m_flash_end  			=	__PC_test_start__ - 0x1;

_region_ROM_start__    =  __ROM_start__ + 0x410;
m_interrupts_ram_start =  __RAM_start__;
m_interrupts_ram_end   =  __RAM_start__ + __ram_vector_table_size__ - 0x1;
m_data_start           =  __RAM_start__ + __ram_vector_table_size__;

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
m_data_size 					= m_data_end - m_data_start;             


MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = __ROM_start__, LENGTH = m_flash_end + 1 /* 512K bytes (alias Flash) */  
  PCTEST_region (rx) : ORIGIN = __PC_test_start__, LENGTH = __PC_test_size   /*For PC test code*/
  SRAM_UPPER   (rwx) : ORIGIN = __RAM_start__, LENGTH = m_data_size /* 64K bytes (alias RAM) */  
  SRAM_LOWER   (rwx) : ORIGIN = m_safety_ram_start, LENGTH = SAFETY_RAM_SIZE /* 64K bytes (alias RAM) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
  __base_Flash = __ROM_start__ ; /* Flash */  
  __top_PROGRAM_FLASH = __ROM_end__ ; /* 512K bytes */  
  __top_Flash = __ROM_end__ ; /* 512K bytes */  
  __base_RAM = __RAM_start__ ; /* RAM */  
  __top_RAM = __RAM_start__ + m_data_size; 
  
