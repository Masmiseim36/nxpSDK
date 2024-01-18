/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_BT_BLE_FIFO_H__
#define __EDGEFAST_BT_BLE_FIFO_H__

typedef struct bt_fifo
{
    bt_list_t list;
    QueueHandle_t sem;
} bt_fifo_t;

#define BT_FIFO_DEFINE(name)  \
    struct bt_fifo name

void bt_fifo_init(bt_fifo_t *fifo);

#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
void bt_fifo_append_debug(bt_fifo_t *fifo, void *data, const char *func, int line);
void bt_fifo_prepend_debug(bt_fifo_t *fifo, void *data, const char *func, int line);
void bt_fifo_insert_debug(bt_fifo_t *fifo, void *prev, void *data, const char *func, int line);

#define bt_fifo_append(fifo, data) bt_fifo_append_debug(fifo, data, __func__, __LINE__)
#define bt_fifo_prepend(fifo, data) bt_fifo_prepend_debug(fifo, data, __func__, __LINE__)
#define bt_fifo_insert(fifo, prev, data) bt_fifo_insert_debug(fifo, prev, data, __func__, __LINE__)
#else
void bt_fifo_append(bt_fifo_t *fifo, void *data);
void bt_fifo_prepend(bt_fifo_t *fifo, void *data);
void bt_fifo_insert(bt_fifo_t *fifo, void *prev, void *data);
#endif /* CONFIG_BT_DEBUG_FIFO */

int bt_fifo_append_list(bt_fifo_t *fifo, void *head, void *tail);
void *bt_fifo_get(bt_fifo_t *fifo, size_t timeout);
bool bt_fifo_remove(bt_fifo_t *fifo, void *data);
bool bt_fifo_unique_append(bt_fifo_t *fifo, void *data);
int bt_fifo_is_empty(bt_fifo_t *fifo);
void *bt_fifo_peek_head(bt_fifo_t *fifo);
void *bt_fifo_peek_tail(bt_fifo_t *fifo);

void bt_fifo_scan(bt_fifo_t *fifo, bool (*cb)(void* node, void *p), void *p);

#define bt_fifo_put bt_fifo_append

#endif /* __EDGEFAST_BT_BLE_FIFO_H__ */
