/*
 * Copyright 2021,2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_BT_BLE_PRINTK_H__
#define __EDGEFAST_BT_BLE_PRINTK_H__

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include <fsl_debug_console.h>

#ifdef __cplusplus
extern "C" {
#endif

#define snprintk snprintf
#define printk PRINTF

#ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_BT_BLE_TYPES_H__ */