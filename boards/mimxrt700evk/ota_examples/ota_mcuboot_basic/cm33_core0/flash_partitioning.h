/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

/*
The memory is allocated as follows:
    Default:
    - BOOTLOADER:  0x040000 bytes @ 0x28000000
    - APP_ACT:     0x200000 bytes @ 0x28040000
    - APP_CAND:    0x200000 bytes @ 0x28240000
*/

#define BOOT_FLASH_BASE     0x28000000

#if defined(CONFIG_BOOT_CUSTOM_DEVICE_SETUP)
/* Layout setup from Kconfig */

#define BOOT_FLASH_ACT_APP              CONFIG_BOOT_FLASH_ACT_APP_ADDRESS
#define BOOT_FLASH_CAND_APP             CONFIG_BOOT_FLASH_CAND_APP_ADDRESS

#else
/* Default layout setup */

#define BOOT_FLASH_ACT_APP  0x28040000
#define BOOT_FLASH_CAND_APP 0x28240000

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
