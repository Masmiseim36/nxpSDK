/*
 * Copyright 2018-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FWK_LFS_MFLASH_H_
#define _FWK_LFS_MFLASH_H_

#include "lfs.h"
#include "mflash_drv.h"

/**
 * \brief underlying porting layer to mflash driver for LittleFS
 *
 * Exclusively used by FileSystem module for LittleFS port definition
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * \brief Get default configuration structure from peripheral.[ch] for littleFs Initialization
 *
 * \param[out] *lfsc pointer to the default littleFS configuration structure
 * \return int LFS_ERR_OK
 */
int lfs_get_default_config(const struct lfs_config **lfsc);

/**
 * \brief Initialize Get default configuration structure from peripheral.[ch] for littleFs Initialization
 *
 * \return returned status from mflash_drv_init()
 */
int lfs_storage_init(void);

/**
 * @brief littleFS underlining APi to read in mflash drivers
 *
 * @param lfsc
 * @param block
 * @param off
 * @param buffer
 * @param size
 * @return int
 */

int lfs_mflash_read(const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

/**
 * @brief littleFS underlining APi to program in mflash drivers
 *
 * @param lfsc
 * @param block
 * @param off
 * @param buffer
 * @param size
 * @return int
 */
int lfs_mflash_prog(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

/**
 * @brief littleFS underlining APi to erase in mflash drivers
 *
 * @param lfsc
 * @param block
 * @return int
 */
int lfs_mflash_erase(const struct lfs_config *lfsc, lfs_block_t block);

/**
 * @brief
 *
 * @param lfsc
 * @return int
 */
int lfs_mflash_sync(const struct lfs_config *lfsc);

/**
 * @brief
 *
 * @param lfsc
 * @return int
 */
int lfs_mutex_lock(const struct lfs_config *lfsc);

/**
 * @brief
 *
 */
int lfs_mutex_unlock(const struct lfs_config *lfsc);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif
