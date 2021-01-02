/*
 * Copyright 2019 NXP.
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

//     Processors:          MKV11Z128xxx7
//     Version:             KV11P64M75RM

/******************************************************************************/
/******************     USER CONFIGURATION PART     ***************************/
/******************************************************************************/
#ifndef __LINKER_CONFIG__
    #define __LINKER_CONFIG__

#define __ROM_start__        0x00000000
#define __ROM_end__          0x0001E000

#define __RAM_start__        0x1ffff410
#define __RAM_end__          0x20002ee7 //TODO

#define stack_test_block_size  0x10
#define ram_test_backup_size   0x20
#define __size_cstack__        0x400

    #define __FlashCRC_start__  0x6FF0 /* for placing a checksum */
    #define __FlashCRC_end__    0x6FF0 /* for placing a checksum */

    #define __vector_table_size__  0x400
    #undef  __VECTOR_TABLE
    #define __VECTOR_TABLE   __ROM_start__
    #define SAFETY_RAM_SIZE  0x300

/******************************************************************************/
/******************     SYMBOLS     *******************************************/
/******************************************************************************/
#define m_flash_config_start     0x00000400
#define m_flash_config_end       0x00000410
#define m_flash_config_size      (m_flash_config_end - m_flash_config_start)

#define __PC_test_size 0x20
#define __PC_test_end__ 		__ROM_end__
#define __PC_test_start__ 	(__PC_test_end__ - __PC_test_size)

#define m_flash_end  				(__PC_test_start__ - 0x1)

    #define _region_ROM_start__   (__ROM_start__ + 0x410)
    #define m_data_start          __RAM_start__

#define m_ram_test_backup     (__RAM_end__ - ram_test_backup_size + 0x1)
#define m_wd_test_backup      (m_ram_test_backup - 0x20)
#define m_pc_test_flag        (m_wd_test_backup - 0x4)
#define m_safety_error_code   (m_pc_test_flag - 0x4)
#define m_stack_test_p_4      (m_safety_error_code - 0x4)
#define m_stack_test_p_3      (m_stack_test_p_4 - stack_test_block_size +0x4)
#define __BOOT_STACK_ADDRESS  (m_stack_test_p_3 - 0x4)
#define m_stack_test_p_2      (__BOOT_STACK_ADDRESS - __size_cstack__)
#define m_stack_test_p_1      (m_stack_test_p_2 - stack_test_block_size + 0x4)

    #define m_safety_ram_end      (m_stack_test_p_1 - 0x1)
    #define m_safety_ram_start    (m_safety_ram_end - SAFETY_RAM_SIZE + 0x01)  

    #define m_data_end            (m_safety_ram_start - 0x01)
    #define m_data_size           (m_data_end - m_data_start)
#endif
