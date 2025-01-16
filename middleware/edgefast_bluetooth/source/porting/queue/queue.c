
/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "porting.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_FIFO)
#define LOG_MODULE_NAME bt_fifo
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

static void bt_fifo_sem_post(QueueHandle_t sem)
{
	if (k_is_in_isr())
	{
		portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

		if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreGiveFromISR(sem, &taskToWake))
		{
			portYIELD_FROM_ISR(((bool)(taskToWake)));
		}
		else
		{
		}
	}
	else
	{
		if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreGive(sem))
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

	if (millisec == K_FOREVER)
	{
		timeoutTicks = portMAX_DELAY;
	}
	else
	{
		timeoutTicks = MSEC_TO_TICK(millisec);
	}

	(void)xSemaphoreTake(sem, timeoutTicks);
}

static void * bt_fifo_sem_create(bt_fifo_t *fifo)
{
	return xSemaphoreCreateCountingStatic(0xFFFFFFFFU, 0U, &fifo->sem_buffer);
}

void bt_fifo_init(bt_fifo_t *fifo)
{
	if (fifo->sem != (QueueHandle_t)&fifo->sem_buffer)
	{
		LOG_DBG("FIFO init", fifo);

		/* Clear object */
		(void)memset((void *)fifo, 0, sizeof(*fifo));

		fifo->sem = bt_fifo_sem_create(fifo);
		if (fifo->sem == (QueueHandle_t)&fifo->sem_buffer)
		{
			bt_list_init(&fifo->list);
		}
		else
		{
			__ASSERT(0, "fifo init failed");
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
	if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data, NULL))
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
	if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data, NULL))
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
	if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data, NULL))
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

int bt_fifo_merge_slist(bt_fifo_t *fifo, sys_slist_t *head)
{
	bt_fifo_init(fifo);
	bt_list_append_list(&fifo->list, (bt_list_node_t*)head->head, (bt_list_node_t*)NULL);
	head->head = NULL;
	bt_fifo_sem_post(fifo->sem);

	return 0;
}

void *bt_fifo_get(bt_fifo_t *fifo, size_t timeout)
{
	bt_list_node_t *head = NULL;
	uint32_t start;
	uint32_t current;

	bt_fifo_init(fifo);

	if (k_is_in_isr())
	{
		head = bt_list_get(&fifo->list);
	}
	else
	{
		start = k_uptime_get_32();
		current = start;
		while (((current - start) <= timeout) && (NULL == head))
		{
			head = bt_list_get(&fifo->list);
			if (NULL == head)
			{
				if ((current - start) < timeout)
				{
					bt_fifo_sem_wait(fifo->sem, timeout - (current - start));
				}
				else
				{
					break;
				}
			}
			else
			{
				LOG_INF("node %p, fifo %p", head, fifo);
			}
			current = k_uptime_get_32();
		}
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
	if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data, NULL))
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

int bt_fifo_alloc_prepend(bt_fifo_t *fifo, void *data)
{
	return -ENOMEM;
}

int bt_fifo_alloc_append(bt_fifo_t *fifo, void *data)
{
	return -ENOMEM;
}

void bt_fifo_cancel_wait(bt_fifo_t *fifo)
{
}

/* IMPLEMENTATION for k_queue */
static bool slist_scan_callback(void* node, void *param);
static bool slist_scan_callback(void* node, void *param)
{
	struct k_poll_event *poll_event = (struct k_poll_event *)CONTAINER_OF(node, struct k_poll_event, _node);

	if (NULL == poll_event)
	{
		return false;
	}

	if (NULL == poll_event->poller)
	{
		return false;
	}

	if (NULL == poll_event->poller->event_update)
	{
		return false;
	}

	poll_event->poller->event_update(poll_event);

	return false;
}

static void k_queue_poll_event_update(struct k_queue *queue);
static void k_queue_poll_event_update(struct k_queue *queue)
{
	bt_list_scan(&queue->poll_events, slist_scan_callback, queue);
}

void k_queue_init(struct k_queue *queue)
{
	bt_fifo_init(&queue->fifo);
}

void k_queue_cancel_wait(struct k_queue *queue)
{
	bt_fifo_cancel_wait(&queue->fifo);
}

void k_queue_append(struct k_queue *queue, void *data)
{
	bt_fifo_append(&queue->fifo, data);

	k_queue_poll_event_update(queue);
}

int32_t k_queue_alloc_append(struct k_queue *queue, void *data)
{
	int ret = bt_fifo_alloc_append(&queue->fifo, data);

	if (0 == ret)
	{
		k_queue_poll_event_update(queue);
	}
	return (int32_t)ret;
}

void k_queue_prepend(struct k_queue *queue, void *data)
{
	bt_fifo_prepend(&queue->fifo, data);
	k_queue_poll_event_update(queue);
}

int32_t k_queue_alloc_prepend(struct k_queue *queue, void *data)
{
	int ret = bt_fifo_alloc_prepend(&queue->fifo, data);

	if (0 == ret)
	{
		k_queue_poll_event_update(queue);
	}
	return (int32_t)ret;
}

void k_queue_insert(struct k_queue *queue, void *prev, void *data)
{
	bt_fifo_insert(&queue->fifo, prev, data);
	k_queue_poll_event_update(queue);
}

int k_queue_append_list(struct k_queue *queue, void *head, void *tail)
{
	int ret;

	ret = bt_fifo_append_list(&queue->fifo, head, tail);
	k_queue_poll_event_update(queue);

	return ret;
}

int k_queue_merge_slist(struct k_queue *queue, sys_slist_t *list)
{
	int ret;

	ret = bt_fifo_merge_slist(&queue->fifo, list);
	k_queue_poll_event_update(queue);

	return ret;
}

void *k_queue_get(struct k_queue *queue, k_timeout_t timeout)
{
	return bt_fifo_get(&queue->fifo, timeout);
}

bool k_queue_remove(struct k_queue *queue, void *data)
{
	return bt_fifo_remove(&queue->fifo, data);
}

bool k_queue_unique_append(struct k_queue *queue, void *data)
{
	bool ret = bt_fifo_unique_append(&queue->fifo, data);

	k_queue_poll_event_update(queue);

	return ret;
}

int k_queue_is_empty(struct k_queue *queue)
{
	return bt_fifo_is_empty(&queue->fifo);
}

void *k_queue_peek_head(struct k_queue *queue)
{
	return bt_fifo_peek_head(&queue->fifo);
}

void *k_queue_peek_tail(struct k_queue *queue)
{
	return bt_fifo_peek_tail(&queue->fifo);
}