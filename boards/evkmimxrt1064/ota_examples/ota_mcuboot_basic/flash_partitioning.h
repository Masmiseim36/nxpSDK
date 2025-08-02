/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#include "sblconfig.h"

/*
The memory is allocated as follows:
    - BOOTLOADER:  0x040000 bytes @ 0x70000000
    - APP_ACT:     0x100000 bytes @ 0x70040000
    - APP_CAND:    0x100000 bytes @ 0x70140000
*/

#define BOOT_FLASH_BASE                 0x70000000

#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#if CONFIG_ENCRYPT_XIP_ENABLE
#define BOOT_FLASH_EXEC_APP             CONFIG_BOOT_FLASH_EXEC_APP_ADDRESS
#define BOOT_FLASH_ENC_META             CONFIG_BOOT_FLASH_ENC_META_ADDRESS
#endif
  
/* Default layout setup */

#elif !defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE)
/* Overwrite-only, swap or direct-xip mode with flash remapping */

#define BOOT_FLASH_ACT_APP              0x70040000
#define BOOT_FLASH_CAND_APP             0x70140000
   
#elif defined(CONFIG_ENCRYPT_XIP_EXT_OVERWRITE_ONLY)
/* Encrypted XIP extension: modified overwrite-only mode */

#define BOOT_FLASH_ACT_APP              0x70040000
#define BOOT_FLASH_CAND_APP             0x70140000
#define BOOT_FLASH_ENC_META             0x70240000
#define BOOT_FLASH_EXEC_APP             BOOT_FLASH_ACT_APP

#else
/* Encrypted XIP extension: Three slot mode */

#define BOOT_FLASH_EXEC_APP             0x70040000
#define BOOT_FLASH_ACT_APP              0x70140000
#define BOOT_FLASH_CAND_APP             0x70240000
#define BOOT_FLASH_ENC_META             0x70340000

#endif /* !defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE) */

#endif
