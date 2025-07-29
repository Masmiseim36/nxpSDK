/*
 * Copyright 2021, 2024, 2025 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __EDGEFAST_BT_BLE_PORTING_H__
#define __EDGEFAST_BT_BLE_PORTING_H__

#include "fsl_common.h"
#include "fsl_os_abstraction.h"

#include "fsl_debug_console.h"

#include "system_c.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/addr.h>
#include <bluetooth/uuid.h>
#include <bluetooth/conn.h>
#include <bluetooth/hci.h>

#include "bt_ble_settings.h"

#ifdef __cplusplus
extern "C" {
#endif

#if ((defined(FSL_FEATURE_L2CACHE_LINESIZE_BYTE)) && (defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)))
#define EDGEFAST_BT_CACHE_LINESIZE MAX(FSL_FEATURE_L2CACHE_LINESIZE_BYTE, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#elif (defined(FSL_FEATURE_L2CACHE_LINESIZE_BYTE))
#define EDGEFAST_BT_CACHE_LINESIZE MAX(FSL_FEATURE_L2CACHE_LINESIZE_BYTE, 0)
#elif (defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE))
#define EDGEFAST_BT_CACHE_LINESIZE MAX(0, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#else
#define EDGEFAST_BT_CACHE_LINESIZE 0
#endif

#define LOG_HEXDUMP_DBG(data, length, msg)                           \
do                                                                  \
{                                                                   \
    const uint8_t rowCount = 4;                                     \
    uint32_t dataLen = (length);                                    \
    uint32_t index = 0;                                             \
                                                                    \
    LOG_DBG("%s", msg);                                              \
    LOG_DBG("Index: 0  1  2  3");                                    \
    LOG_DBG("=================");                                    \
                                                                    \
    while ((dataLen) >= rowCount)                                   \
    {                                                               \
        LOG_DBG("%04d :%02X %02X %02X %02X", index / rowCount,       \
              ((uint8_t *)(data))[index + 0],                       \
              ((uint8_t *)(data))[index + 1],                       \
              ((uint8_t *)(data))[index + 2],                       \
              ((uint8_t *)(data))[index + 3]);                      \
        (dataLen) -= rowCount;                                      \
        index += rowCount;                                          \
    }                                                               \
                                                                    \
    switch ((dataLen))                                              \
    {                                                               \
    case 3:                                                         \
      LOG_DBG("%04d :%02X %02X %02X", index / rowCount,              \
            ((uint8_t *)(data))[index + 0],                         \
            ((uint8_t *)(data))[index + 1],                         \
            ((uint8_t *)(data))[index + 2]);                        \
      break;                                                        \
    case 2:                                                         \
      LOG_DBG("%04d :%02X %02X", index / rowCount,                   \
            ((uint8_t *)(data))[index + 0],                         \
            ((uint8_t *)(data))[index + 1]);                        \
      break;                                                        \
    case 1:                                                         \
      LOG_DBG("%04d :%02X", index / rowCount,                        \
        ((uint8_t *)(data))[index + 0]);                            \
      break;                                                        \
    default:                                                        \
      /* Fix MISRA C-2012 Rule 16.4 */                              \
      break;                                                        \
    }                                                               \
} while (0);

int char2hex(char c, uint8_t *x);
int hex2char(uint8_t x, char *c);
size_t bin2hex(const uint8_t *buf, size_t buflen, char *hex, size_t hexlen);
size_t hex2bin(const char *hex, size_t hexlen, uint8_t *buf, size_t buflen);

void bt_ble_porting_init(void);
void bt_ble_porting_deinit(void);

const char *bt_addr_le_str(const bt_addr_le_t *addr);
const char *bt_hex(const void *buf, size_t len);
const char *bt_addr_str(const bt_addr_t *addr);
const char *bt_uuid_str(const struct bt_uuid *uuid);

size_t strnlen(const char *s, size_t maxlen);

uint32_t crc32_ieee(const uint8_t *data, size_t len);
static inline atomic_ptr_val_t atomic_ptr_clear(atomic_ptr_t *target)
{
	return atomic_ptr_set(target, NULL);
}

#ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_BT_BLE_PORTING_H__ */