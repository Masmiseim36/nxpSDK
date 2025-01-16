/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_BT_BLE_QUEUE_H__
#define __EDGEFAST_BT_BLE_QUEUE_H__

#include <system_c.h>

typedef struct bt_fifo
{
    bt_list_t list;
    QueueHandle_t sem;
    StaticSemaphore_t  sem_buffer;
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

int bt_fifo_alloc_prepend(bt_fifo_t *fifo, void *data);
int bt_fifo_alloc_append(bt_fifo_t *fifo, void *data);
void bt_fifo_cancel_wait(bt_fifo_t *fifo);
int bt_fifo_append_list(bt_fifo_t *fifo, void *head, void *tail);
int bt_fifo_merge_slist(bt_fifo_t *fifo, sys_slist_t *head);
void *bt_fifo_get(bt_fifo_t *fifo, size_t timeout);
bool bt_fifo_remove(bt_fifo_t *fifo, void *data);
bool bt_fifo_unique_append(bt_fifo_t *fifo, void *data);
int bt_fifo_is_empty(bt_fifo_t *fifo);
void *bt_fifo_peek_head(bt_fifo_t *fifo);
void *bt_fifo_peek_tail(bt_fifo_t *fifo);

void bt_fifo_scan(bt_fifo_t *fifo, bool (*cb)(void* node, void *p), void *p);

#define bt_fifo_put bt_fifo_append

struct k_queue {
	struct bt_fifo fifo;

	_POLL_EVENT;

	SYS_PORT_TRACING_TRACKING_FIELD(k_queue)
};

/**
 * @cond INTERNAL_HIDDEN
 */

#define Z_QUEUE_INITIALIZER(obj) \
	{ \
	.fifo = {{NULL}}, \
	_POLL_EVENT_OBJ_INIT(obj)		\
	}


/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @defgroup queue_apis Queue APIs
 * @ingroup kernel_apis
 * @{
 */

/**
 * @brief Initialize a queue.
 *
 * This routine initializes a queue object, prior to its first use.
 *
 * @param queue Address of the queue.
 */
__syscall void k_queue_init(struct k_queue *queue);

/**
 * @brief Cancel waiting on a queue.
 *
 * This routine causes first thread pending on @a queue, if any, to
 * return from k_queue_get() call with NULL value (as if timeout expired).
 * If the queue is being waited on by k_poll(), it will return with
 * -EINTR and K_POLL_STATE_CANCELLED state (and per above, subsequent
 * k_queue_get() will return NULL).
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 */
__syscall void k_queue_cancel_wait(struct k_queue *queue);

/**
 * @brief Append an element to the end of a queue.
 *
 * This routine appends a data item to @a queue. A queue data item must be
 * aligned on a word boundary, and the first word of the item is reserved
 * for the kernel's use.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param data Address of the data item.
 */
extern void k_queue_append(struct k_queue *queue, void *data);

/**
 * @brief Append an element to a queue.
 *
 * This routine appends a data item to @a queue. There is an implicit memory
 * allocation to create an additional temporary bookkeeping data structure from
 * the calling thread's resource pool, which is automatically freed when the
 * item is removed. The data itself is not copied.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param data Address of the data item.
 *
 * @retval 0 on success
 * @retval -ENOMEM if there isn't sufficient RAM in the caller's resource pool
 */
__syscall int32_t k_queue_alloc_append(struct k_queue *queue, void *data);

/**
 * @brief Prepend an element to a queue.
 *
 * This routine prepends a data item to @a queue. A queue data item must be
 * aligned on a word boundary, and the first word of the item is reserved
 * for the kernel's use.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param data Address of the data item.
 */
extern void k_queue_prepend(struct k_queue *queue, void *data);

/**
 * @brief Prepend an element to a queue.
 *
 * This routine prepends a data item to @a queue. There is an implicit memory
 * allocation to create an additional temporary bookkeeping data structure from
 * the calling thread's resource pool, which is automatically freed when the
 * item is removed. The data itself is not copied.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param data Address of the data item.
 *
 * @retval 0 on success
 * @retval -ENOMEM if there isn't sufficient RAM in the caller's resource pool
 */
__syscall int32_t k_queue_alloc_prepend(struct k_queue *queue, void *data);

/**
 * @brief Inserts an element to a queue.
 *
 * This routine inserts a data item to @a queue after previous item. A queue
 * data item must be aligned on a word boundary, and the first word of
 * the item is reserved for the kernel's use.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param prev Address of the previous data item.
 * @param data Address of the data item.
 */
extern void k_queue_insert(struct k_queue *queue, void *prev, void *data);

/**
 * @brief Atomically append a list of elements to a queue.
 *
 * This routine adds a list of data items to @a queue in one operation.
 * The data items must be in a singly-linked list, with the first word
 * in each data item pointing to the next data item; the list must be
 * NULL-terminated.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param head Pointer to first node in singly-linked list.
 * @param tail Pointer to last node in singly-linked list.
 *
 * @retval 0 on success
 * @retval -EINVAL on invalid supplied data
 *
 */
extern int k_queue_append_list(struct k_queue *queue, void *head, void *tail);

/**
 * @brief Atomically add a list of elements to a queue.
 *
 * This routine adds a list of data items to @a queue in one operation.
 * The data items must be in a singly-linked list implemented using a
 * sys_slist_t object. Upon completion, the original list is empty.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param list Pointer to sys_slist_t object.
 *
 * @retval 0 on success
 * @retval -EINVAL on invalid data
 */
extern int k_queue_merge_slist(struct k_queue *queue, sys_slist_t *list);

/**
 * @brief Get an element from a queue.
 *
 * This routine removes first data item from @a queue. The first word of the
 * data item is reserved for the kernel's use.
 *
 * @note @a timeout must be set to K_NO_WAIT if called from ISR.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param timeout Non-negative waiting period to obtain a data item
 *                or one of the special values K_NO_WAIT and
 *                K_FOREVER.
 *
 * @return Address of the data item if successful; NULL if returned
 * without waiting, or waiting period timed out.
 */
__syscall void *k_queue_get(struct k_queue *queue, k_timeout_t timeout);

/**
 * @brief Remove an element from a queue.
 *
 * This routine removes data item from @a queue. The first word of the
 * data item is reserved for the kernel's use. Removing elements from k_queue
 * rely on sys_slist_find_and_remove which is not a constant time operation.
 *
 * @note @a timeout must be set to K_NO_WAIT if called from ISR.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param data Address of the data item.
 *
 * @return true if data item was removed
 */
bool k_queue_remove(struct k_queue *queue, void *data);

/**
 * @brief Append an element to a queue only if it's not present already.
 *
 * This routine appends data item to @a queue. The first word of the data
 * item is reserved for the kernel's use. Appending elements to k_queue
 * relies on sys_slist_is_node_in_list which is not a constant time operation.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 * @param data Address of the data item.
 *
 * @return true if data item was added, false if not
 */
bool k_queue_unique_append(struct k_queue *queue, void *data);

/**
 * @brief Query a queue to see if it has data available.
 *
 * Note that the data might be already gone by the time this function returns
 * if other threads are also trying to read from the queue.
 *
 * @funcprops \isr_ok
 *
 * @param queue Address of the queue.
 *
 * @return Non-zero if the queue is empty.
 * @return 0 if data is available.
 */
__syscall int k_queue_is_empty(struct k_queue *queue);

/**
 * @brief Peek element at the head of queue.
 *
 * Return element from the head of queue without removing it.
 *
 * @param queue Address of the queue.
 *
 * @return Head element, or NULL if queue is empty.
 */
__syscall void *k_queue_peek_head(struct k_queue *queue);

/**
 * @brief Peek element at the tail of queue.
 *
 * Return element from the tail of queue without removing it.
 *
 * @param queue Address of the queue.
 *
 * @return Tail element, or NULL if queue is empty.
 */
__syscall void *k_queue_peek_tail(struct k_queue *queue);

/**
 * @brief Statically define and initialize a queue.
 *
 * The queue can be accessed outside the module where it is defined using:
 *
 * @code extern struct k_queue <name>; @endcode
 *
 * @param name Name of the queue.
 */
#define K_QUEUE_DEFINE(name) \
	struct k_queue name = \
		Z_QUEUE_INITIALIZER(name)

/** @} */


#endif /* __EDGEFAST_BT_BLE_QUEUE_H__ */
