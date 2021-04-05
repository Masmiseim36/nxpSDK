/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//     ____________
//    |            | --> m_data_start                 ....ADR
//    |            |                                  ....ADR + 0x4
//    |  data      |                                  ....ADR + 0x8
//    |            |                                  ....ADR + 0xC
//    |            |
//    |            | 
//    |            | 
//    |            |
//    |            |                                                
//    |            |                   
//    |            |
//    |  data      |
//    |            |
//    |            |
//    |____________| --> m_data_end
//    |            | --> STACK_TEST_P_1
//    |            |
//    |____________| --> STACK_TEST_P_2
//    |            |
//    |            |
//    |            |
//    |   STACK    |
//    |            |
//    |            |
//    |            |
//    |____________| --> __BOOT_STACK_ADDRESS
//    |            | --> STACK_TEST_P_3
//    |            |
//    |____________| --> STACK_TEST_P_4
//    |____________| --> SAFETY_ERROR_CODE
//    |____________| --> PC_test_flag
//    |            | --> WD_TEST_BACKUP
//    |____________|
//    |            |
//    |            | --> RAM_TEST_BACKUP
//    |____________|

/******************************************************************************/
/******************     USER CONFIGURATION PART     ***************************/
/******************************************************************************/
#ifndef __LINKER_CONFIG__
#define __LINKER_CONFIG__

	/* FLASH memory boundaries. */
	#define __ROM_start__        0x00000000
	#define __ROM_end__          0x0001DFFF

	/* RAM memory boundaries. */
	#define __RAM_start__        0x1FFFF410
	#define __RAM_end__          0x20002EE7

	/* Sizes of objects in RAM. */
	#define __size_cstack__        0x400
	#define stack_test_block_size  0x10
	#define ram_test_backup_size   0x20
	#define wd_test_backup_size    0x20

	/* Sizes of objects in FLASH. */
	#define __vector_table_size__  0x400
	#undef  __VECTOR_TABLE
	#define __VECTOR_TABLE   __ROM_start__
	#define __PC_test_size       0x20
	#define __size_flash_crc__   0x10 /*size of region for placing a checksum */
	#define __flash_cfg_size     0x10


	/******************************************************************************/
	/******************     SYMBOLS     *******************************************/
	/******************************************************************************/
	/* Assemble RAM addresses. */
	#define m_ram_test_backup     (__RAM_end__ - ram_test_backup_size + 0x1)
	#define m_wd_test_backup      (m_ram_test_backup - wd_test_backup_size)
	#define m_pc_test_flag        (m_wd_test_backup - 0x4)
	#define m_safety_error_code   (m_pc_test_flag - 0x4)
	#define m_stack_test_p_4      (m_safety_error_code - 0x4)
	#define m_stack_test_p_3      (m_stack_test_p_4 - stack_test_block_size +0x4)
	#define __BOOT_STACK_ADDRESS  (m_stack_test_p_3 - 0x4)
	#define m_stack_test_p_2      (__BOOT_STACK_ADDRESS - __size_cstack__)
	#define m_stack_test_p_1      (m_stack_test_p_2 - stack_test_block_size + 0x4)

	#define m_safety_ram_start    __RAM_start__
	// NON SAFETY RELATED RAM continue exactly after safety related ram. 

	
	/* Assemble FLASH addresses. */
	#define m_intvec_table_start (__ROM_start__)
	#define m_intvec_table_end   (m_intvec_table_start  + __vector_table_size__ - 0x1)
	#define m_flash_config_start (m_intvec_table_end    + 0x1)
	#define m_flash_config_end   (m_flash_config_start  + __flash_cfg_size - 0x1)
	#define __PC_test_start__    (m_flash_config_end    + 0x1) 
	#define __PC_test_end__ 		 (__PC_test_start__ + __PC_test_size - 0x1)
	#define m_flash_start        (__PC_test_end__   + 0x1)

	#define m_fs_flash_crc_end   (__ROM_end__)
	#define m_fs_flash_crc_start (m_fs_flash_crc_end - __size_flash_crc__ + 0x1)
	#define m_flash_end          (m_fs_flash_crc_start - 0x1)

#endif

