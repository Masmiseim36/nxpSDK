/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 *     Device:              MIMXRT1170
 */

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
#define SAFETY_RAM_CRC_SIZE   0x190
#define __size_flash_crc__    0x10 /* size of region for placing a checksum */

#define SRAM_DTC_start 0x20000000 /* Part of on-chip RAM (Data-Tightly-Coupled) */
#define SRAM_DTC_end   0x2003FFFF

#define ROM_interrupts_start 0x30002000 /* IVT in ROM */
#define ROM_interrupts_end   ROM_interrupts_start + __vector_table_size__

#define m_flash_config_start 0x30000000
#define m_flash_config_size  0x00001000

#define CRC_config_start ROM_interrupts_end
#define CRC_config_end   CRC_config_start + __size_flash_crc__

#define __PC_test_start__ CRC_config_end           /* PC object address */
#define __PC_test_end__   __PC_test_start__ + 0x20 /* PC object reserved size: 0x20 */

#define ROM_start __PC_test_end__ /* ROM memory (external ROM) */
#define ROM_end   0x300e0000      /* Real ROM_end is 0x303FFFFF, it is shortened due to generated image size. */

/* Compatibility with placing IVT in vectors_mdk.c and setting VTOR in startup_mdk.c */
#undef __VECTOR_TABLE
#define __VECTOR_TABLE ROM_interrupts_start
#define __ROM_start__  ROM_interrupts_start

/******************************************************************************/
/******************     SYMBOLS     *******************************************/
/******************************************************************************/
#define m_ram_test_backup    (SRAM_DTC_end - ram_test_backup_size + 0x1)
#define m_wd_test_backup     (m_ram_test_backup - 0x20)
#define m_pc_test_flag       (m_wd_test_backup - 0x4)
#define m_safety_error_code  (m_pc_test_flag - 0x4)
#define m_stack_test_p_4     (m_safety_error_code - 0x4)
#define m_stack_test_p_3     (m_stack_test_p_4 - stack_test_block_size + 0x4)
#define __BOOT_STACK_ADDRESS (m_stack_test_p_3 - 0x4)
#define m_stack_test_p_2     (__BOOT_STACK_ADDRESS - __size_cstack__)
#define m_stack_test_p_1     (m_stack_test_p_2 - stack_test_block_size + 0x4)

#define m_safety_ram_crc_end   (m_stack_test_p_1 - 0x1)
#define m_safety_ram_crc_start (m_safety_ram_crc_end - SAFETY_RAM_CRC_SIZE + 0x01)

#define m_safety_ram_end   (m_safety_ram_crc_start - 0x1)
#define m_safety_ram_start (m_safety_ram_end - SAFETY_RAM_SIZE + 0x01)

#define m_data_end  (m_safety_ram_start - 0x01)
#define m_data_size (m_data_end - SRAM_DTC_start)

#endif
