/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LFS_SUPPORT_H_
#define _LFS_SUPPORT_H_

#include "lfs.h"

#define EXAMPLE_FLEXSPI FLEXSPI
#define EXAMPLE_FLEXSPI_CLOCK kCLOCK_FlexSpi
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE

#define FLASH_SIZE 0x10000 /* in KB */
#define FLASH_PAGE_SIZE 512
#define FLASH_SECTOR_SIZE 0x40000

#define LFS_FIRST_SECTOR 128 /* use second half of the flash */
#define LFS_SECTORS 128

#define HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA 0
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS 2
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE 4
#define HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR 6
#define HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM 10
#define CUSTOM_LUT_LENGTH 48

extern int lfs_get_default_config(struct lfs_config *lfsc);
extern int lfs_storage_init(const struct lfs_config *lfsc);

#endif
