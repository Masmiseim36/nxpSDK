/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

#define BOOT_FLASH_BASE     0x04000000

/* Default layout setup */

/*
 * 256 kB for bootloader
 *   2 MB for application
 */

#define BOOT_FLASH_ACT_APP  0x04040000
#define BOOT_FLASH_CAND_APP 0x04240000

#endif