/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_LITTLEFS_PL_H__
#define __EDGEFAST_LITTLEFS_PL_H__

#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
#include "lfs.h"

#ifdef __cplusplus
extern "C" {
#endif

lfs_t * lfs_pl_init(void);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_BT_SETTINGS */
#endif /* __EDGEFAST_LITTLEFS_PL_H__ */