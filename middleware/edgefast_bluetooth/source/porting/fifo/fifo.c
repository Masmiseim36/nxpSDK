
/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "porting.h"

#include "fifo.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_FIFO)
#define LOG_MODULE_NAME bt_fifo
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#ifndef MSEC_TO_TICK
#define MSEC_TO_TICK(msec) \
    (((uint32_t)(msec) + 500uL / (uint32_t)configTICK_RATE_HZ) * (uint32_t)configTICK_RATE_HZ / 1000uL)
#endif /* MSEC_TO_TICK */

static void bt_fifo_sem_post(QueueHandle_t sem)
{
    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xSemaphoreGiveFromISR(sem, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
        }
        else
        {
        }
    }
    else
    {
        if (((BaseType_t)1) == (BaseType_t)xSemaphoreGive(sem))
        {
        }
        else
        {
        }
    }
}

static void bt_fifo_sem_wait(QueueHandle_t sem, uint32_t millisec)
{
    uint32_t timeoutTicks;

    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }

    (void)xSemaphoreTake(sem, timeoutTicks);
}

static void * bt_fifo_sem_create()
{
    return xSemaphoreCreateCounting(0xFFFFFFFFU, 0U);
}

void bt_fifo_init(bt_fifo_t *fifo)
{
    if (NULL == fifo->sem)
    {
        LOG_DBG("FIFO init", fifo);

        fifo->sem = bt_fifo_sem_create();
        if (NULL != fifo->sem)
        {
            bt_list_init(&fifo->list);
        }
        else
        {
            LOG_ERR("failed");
        }
    }
}

#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
void bt_fifo_append_debug(bt_fifo_t *fifo, void *data, const char *func, int line)
#else
void bt_fifo_append(bt_fifo_t *fifo, void *data)
#endif /* CONFIG_BT_DEBUG_FIFO */
{
    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
        LOG_ERR("%s():%d:node %p has been inserted to fifo %p", func, line, data, fifo);
#else
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
#endif /* CONFIG_BT_DEBUG_FIFO */
        return;
    }
    LOG_INF("node %p, fifo %p", data, fifo);
    bt_list_append(&fifo->list, (bt_list_node_t*)data);
    LOG_INF("next %p", ((bt_list_node_t*)data)->next);
    bt_fifo_sem_post(fifo->sem);
}

#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
void bt_fifo_prepend_debug(bt_fifo_t *fifo, void *data, const char *func, int line)
#else
void bt_fifo_prepend(bt_fifo_t *fifo, void *data)
#endif /* CONFIG_BT_DEBUG_FIFO */
{
    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
        LOG_ERR("%s():%d:node %p has been inserted to fifo %p", func, line, data, fifo);
#else
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
#endif /* CONFIG_BT_DEBUG_FIFO */
        return;
    }
    bt_list_prepend(&fifo->list, (bt_list_node_t*)data);
    bt_fifo_sem_post(fifo->sem);
}

#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
void bt_fifo_insert_debug(bt_fifo_t *fifo, void *prev, void *data, const char *func, int line)
#else
void bt_fifo_insert(bt_fifo_t *fifo, void *prev, void *data)
#endif /* CONFIG_BT_DEBUG_FIFO */
{
    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
        LOG_ERR("%s():%d:node %p has been inserted to fifo %p", func, line, data, fifo);
#else
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
#endif /* CONFIG_BT_DEBUG_FIFO */
        return;
    }
    bt_list_insert(&fifo->list, (bt_list_node_t*)prev, (bt_list_node_t*)data);
    bt_fifo_sem_post(fifo->sem);
}

int bt_fifo_append_list(bt_fifo_t *fifo, void *head, void *tail)
{
    bt_fifo_init(fifo);
    bt_list_append_list(&fifo->list, (bt_list_node_t*)head, (bt_list_node_t*)tail);

    bt_fifo_sem_post(fifo->sem);

    return 0;
}

void *bt_fifo_get(bt_fifo_t *fifo, size_t timeout)
{
    bt_list_node_t *head = NULL;
    uint32_t start = OSA_TimeGetMsec();
    uint32_t current = start;

    bt_fifo_init(fifo);

    while (((current - start) <= timeout) && (NULL == head))
    {
        head = bt_list_get(&fifo->list);
        if (NULL == head)
        {
            if ((current - start) < timeout)
            {
                bt_fifo_sem_wait(fifo->sem, timeout - (current - start));
            }
        }
        else
        {
            LOG_INF("node %p, fifo %p", head, fifo);
        }
        current = OSA_TimeGetMsec();
    }
    return head;
}

bool bt_fifo_remove(bt_fifo_t *fifo, void *data)
{
    bt_fifo_init(fifo);

    return bt_list_find_and_remove(&fifo->list, (bt_list_node_t*)data);
}

bool bt_fifo_unique_append(bt_fifo_t *fifo, void *data)
{
    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
        return false;
    }
    bt_list_append(&fifo->list, (bt_list_node_t*)data);
    bt_fifo_sem_post(fifo->sem);

    return true;
}

int bt_fifo_is_empty(bt_fifo_t *fifo)
{
    bt_fifo_init(fifo);

    return bt_list_is_empty(&fifo->list);
}

void *bt_fifo_peek_head(bt_fifo_t *fifo)
{
    bt_fifo_init(fifo);

    return bt_list_peek_head(&fifo->list);
}

void *bt_fifo_peek_tail(bt_fifo_t *fifo)
{
    bt_fifo_init(fifo);

    return bt_list_peek_tail(&fifo->list);
}

void bt_fifo_scan(bt_fifo_t *fifo, bool (*cb)(void* node, void *p), void *p)
{
    bt_fifo_init(fifo);

    bt_list_scan(&fifo->list, cb, p);
}