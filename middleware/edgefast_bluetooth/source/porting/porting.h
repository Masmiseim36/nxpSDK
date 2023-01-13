/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_BT_BLE_PORTING_H__
#define __EDGEFAST_BT_BLE_PORTING_H__

#include "fsl_common.h"
#include "fsl_os_abstraction.h"

#include <zephyr/types.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/printk.h>
#include <sys/slist.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/addr.h>
#include <bluetooth/uuid.h>

#include "bt_ble_settings.h"

#include <bluetooth/buf.h>

#define k_sleep OSA_TimeDelay

#define K_MSEC BT_MSEC
#define K_NO_WAIT osaWaitNone_c

/* TODO: K_KERNEL_STACK_MEMBER */
#define K_KERNEL_STACK_MEMBER(s, size) size_t (s)[size]
/* TODO: K_KERNEL_STACK_MEMBER */
#define K_KERNEL_STACK_DEFINE(s, size) size_t (s)[size]

#ifndef BT_DBG
#define BT_DBG(fmt, ...) LOG_DBG("%s " fmt, __func__, ##__VA_ARGS__)
#endif /* BT_DBG */
#ifndef BT_ERR
#define BT_ERR(fmt, ...) LOG_ERR("%s " fmt, __func__, ##__VA_ARGS__)
#endif /* BT_ERR */
#ifndef BT_WARN
#define BT_WARN(fmt, ...) LOG_WRN("%s " fmt, __func__, ##__VA_ARGS__)
#endif /* BT_WARN */
#ifndef BT_INFO
#define BT_INFO(fmt, ...) LOG_INF("%s " fmt, __func__, ##__VA_ARGS__)
#endif /* BT_INFO */

static inline uint32_t k_ticks_to_ms_floor32(uint32_t t)
{
	return t;
}

#define BT_HEXDUMP_DBG(data, length, msg)                           \
do                                                                  \
{                                                                   \
    const uint8_t rowCount = 4;                                     \
    uint32_t dataLen = (length);                                    \
    uint32_t index = 0;                                             \
                                                                    \
    BT_DBG("%s", msg);                                              \
    BT_DBG("Index: 0  1  2  3");                                    \
    BT_DBG("=================");                                    \
                                                                    \
    while ((dataLen) >= rowCount)                                   \
    {                                                               \
        BT_DBG("%04d :%02X %02X %02X %02X", index / rowCount,       \
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
      BT_DBG("%04d :%02X %02X %02X", index / rowCount,              \
            ((uint8_t *)(data))[index + 0],                         \
            ((uint8_t *)(data))[index + 1],                         \
            ((uint8_t *)(data))[index + 2]);                        \
      break;                                                        \
    case 2:                                                         \
      BT_DBG("%04d :%02X %02X", index / rowCount,                   \
            ((uint8_t *)(data))[index + 0],                         \
            ((uint8_t *)(data))[index + 1]);                        \
      break;                                                        \
    case 1:                                                         \
      BT_DBG("%04d :%02X", index / rowCount,                        \
        ((uint8_t *)(data))[index + 0]);                            \
      break;                                                        \
    default:                                                        \
      /* Fix MISRA C-2012 Rule 16.4 */                              \
      break;                                                        \
    }                                                               \
} while (0);

typedef struct k_thread
{
    /* TODO: struct k_thread */
    void* taskHandle;
} k_thread_t;

typedef struct k_fifo
{
	osa_msgq_handle_t queue;
    OSA_MSGQ_HANDLE_DEFINE(queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void *));
} k_fifo_t;

void k_fifo_init(k_fifo_t *fifo);
void k_fifo_deinit(k_fifo_t *fifo);
int k_fifo_is_empty(k_fifo_t *fifo);
void k_fifo_put_head(k_fifo_t *fifo, void *data);

typedef struct k_sem
{
	osa_semaphore_handle_t               sem;
	OSA_SEMAPHORE_HANDLE_DEFINE(sem_handle);
} k_sem_t;

void k_sem_init(struct k_sem *sem, unsigned int initial_count, unsigned int limit);
void k_sem_deinit(struct k_sem *sem);


int char2hex(char c, uint8_t *x);
int hex2char(uint8_t x, char *c);
size_t bin2hex(const uint8_t *buf, size_t buflen, char *hex, size_t hexlen);
size_t hex2bin(const char *hex, size_t hexlen, uint8_t *buf, size_t buflen);

void bt_ble_porting_init(void);

const char *bt_addr_le_str(const bt_addr_le_t *addr);
const char *bt_hex(const void *buf, size_t len);
const char *bt_addr_str(const bt_addr_t *addr);
const char *bt_uuid_str(const struct bt_uuid *uuid);

size_t strnlen(const char *s, size_t maxlen);

#endif /* __EDGEFAST_BT_BLE_PORTING_H__ */