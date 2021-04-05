/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
    Checked by safety FLASH test (0-No; #-Runtime;)


*/
/******************************************************************************/
/******************     USER CONFIGURATION PART     ***************************/
/******************************************************************************/
#ifndef __LINKER_CONFIG__
#define __LINKER_CONFIG__

/******************************************************************************/
/**********************     USER CONFIGURATION PART    ************************/
/******************************************************************************/
/* FLASH memory boundaries. */
#define __ROM_start__             (0x00000000)
#define __ROM_end__               (0x00017FFF)

/* RAM memory boundaries. */
#define __RAM_start__             (0x1FFFF000)
#define __RAM_end__               (0x20002FFF)

/* Sizes of objects in RAM. */
#define __size_cstack__           (0x300)  /* Stack size. */
#define __size_fs_ram_tst_bckp__  (0x20)   /* Safety RAM test backup size. */
#define __size_fs_wdog_tst_data__ (0x1C)   /* Safety WDOG test data size. */
#define __size_fs_stack_tst_p__   (0x10)   /* Safety stack test pattern. */

/* Sizes of objects in FLASH. */
#define __size_fs_pc_tst_obj__    (0x20)
#define __size_intvec_table__     (0x400)
#define __size_flash_cfg_table__  (0x10)
#define __size_flash_crc__        (0x10)

/******************************************************************************/
/*******************************     SYMBOLS     ******************************/
/******************************************************************************/
/* Assemble RAM addresses. */
#define m_fs_stack_tst_p1         (__RAM_start__)
#define m_fs_stack_tst_p2         (m_fs_stack_tst_p1  + __size_fs_stack_tst_p__ - 0x4)
#define m_stack_start             (m_fs_stack_tst_p2  + __size_cstack__ + 0x4)
#define m_fs_stack_tst_p3         (m_stack_start      + 0x4)
#define m_fs_stack_tst_p4         (m_fs_stack_tst_p3  + __size_fs_stack_tst_p__ - 0x4)
#define m_fs_wdog_tst_bckp        (m_fs_stack_tst_p4  + 0x4)
#define m_fs_pc_tst_flag          (m_fs_wdog_tst_bckp + __size_fs_wdog_tst_data__)
#define m_fs_error_code           (m_fs_pc_tst_flag   + 0x4)
#define m_data_start              (m_fs_error_code    + 0x4)

#define m_fs_ram_tst_bckp         (__RAM_end__ - __size_fs_ram_tst_bckp__ + 0x1)
#define m_data_end                (m_fs_ram_tst_bckp  - 0x1)

/* Assemble FLASH addresses. */
#define m_intvec_table_start      (__ROM_start__)
#define m_intvec_table_end        (m_intvec_table_start  + __size_intvec_table__ - 0x1)
#define m_flash_config_start      (m_intvec_table_end    + 0x1)
#define m_flash_config_end        (m_flash_config_start  + __size_flash_cfg_table__ - 0x1)
#define m_fs_pc_tst_obj_start     (m_flash_config_end    + 0x1)
#define m_fs_pc_tst_obj_end       (m_fs_pc_tst_obj_start + __size_fs_pc_tst_obj__ - 0x1)
#define m_flash_start             (m_fs_pc_tst_obj_end   + 0x1)

#define m_fs_flash_crc_end        (__ROM_end__)
#define m_fs_flash_crc_start      (m_fs_flash_crc_end - __size_flash_crc__ + 0x1)
#define m_flash_end               (m_fs_flash_crc_start - 0x1)

#endif /* __LINKER_CONFIG__ */

