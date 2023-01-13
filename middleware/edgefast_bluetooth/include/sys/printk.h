/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef __EDGEFAST_BT_BLE_PRINTK_H__
#define __EDGEFAST_BT_BLE_PRINTK_H__

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define snprintk snprintf

#define BT_DBG  LOG_DBG
#define BT_ERR  LOG_ERR
#define BT_WARN LOG_WRN
#define BT_INFO LOG_INF

#endif /* __EDGEFAST_BT_BLE_TYPES_H__ */