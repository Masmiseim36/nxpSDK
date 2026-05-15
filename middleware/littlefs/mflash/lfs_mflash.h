/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LFS_MFLASH_H_
#define _LFS_MFLASH_H_

#include "lfs.h"
#include "mflash_drv.h"

struct lfs_mflash_ctx
{
    uint32_t start_addr;
};

int lfs_get_default_config(struct lfs_config *lfsc);
int lfs_storage_init(const struct lfs_config *lfsc);


/* LittleFS read a block region callback*/
int lfs_mflash_read(const struct lfs_config*, lfs_block_t, lfs_off_t, void*, lfs_size_t);
/* LittleFS program a block region callback*/
int lfs_mflash_prog(const struct lfs_config*, lfs_block_t, lfs_off_t, const void*, lfs_size_t);
/* LittleFS erase a block callback*/
int lfs_mflash_erase(const struct lfs_config*, lfs_block_t);
/* LittleFS state sync callback*/
int lfs_mflash_sync(const struct lfs_config*);

#endif
