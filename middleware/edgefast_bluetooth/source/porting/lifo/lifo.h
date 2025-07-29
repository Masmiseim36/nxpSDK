/*
 * Copyright 2023,2025 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __EDGEFAST_BT_BLE_LIFO_H__
#define __EDGEFAST_BT_BLE_LIFO_H__

#include <system_c.h>

#ifdef __cplusplus
extern "C" {
#endif

struct k_lifo {
	struct k_queue _queue;
#ifdef CONFIG_OBJ_CORE_LIFO
	struct k_obj_core  obj_core;
#endif
};

/**
 * @cond INTERNAL_HIDDEN
 */

#define Z_LIFO_INITIALIZER(obj) \
	{ \
	._queue = {{{NULL}}} \
	}

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @defgroup lifo_apis LIFO APIs
 * @ingroup kernel_apis
 * @{
 */

/**
 * @brief Initialize a LIFO queue.
 *
 * This routine initializes a LIFO queue object, prior to its first use.
 *
 * @param lifo Address of the LIFO queue.
 */
#define k_lifo_init(lifo)                                \
	({                                                   \
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_lifo, init, lifo); \
	k_queue_init(&(lifo)->_queue);                       \
	K_OBJ_CORE_INIT(K_OBJ_CORE(lifo), _obj_type_lifo);   \
	K_OBJ_CORE_LINK(K_OBJ_CORE(lifo));                   \
	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_lifo, init, lifo);  \
	})

/**
 * @brief Add an element to a LIFO queue.
 *
 * This routine adds a data item to @a lifo. A LIFO queue data item must be
 * aligned on a word boundary, and the first word of the item is
 * reserved for the kernel's use.
 *
 * @funcprops \isr_ok
 *
 * @param lifo Address of the LIFO queue.
 * @param data Address of the data item.
 */
#define k_lifo_put(lifo, data) \
	({ \
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_lifo, put, lifo, data); \
	k_queue_prepend(&(lifo)->_queue, data); \
	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_lifo, put, lifo, data); \
	})

/**
 * @brief Add an element to a LIFO queue.
 *
 * This routine adds a data item to @a lifo. There is an implicit memory
 * allocation to create an additional temporary bookkeeping data structure from
 * the calling thread's resource pool, which is automatically freed when the
 * item is removed. The data itself is not copied.
 *
 * @funcprops \isr_ok
 *
 * @param lifo Address of the LIFO.
 * @param data Address of the data item.
 *
 * @retval 0 on success
 * @retval -ENOMEM if there isn't sufficient RAM in the caller's resource pool
 */
#define k_lifo_alloc_put(lifo, data) \
	({ \
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_lifo, alloc_put, lifo, data); \
	int lap_ret = k_queue_alloc_prepend(&(lifo)->_queue, data); \
	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_lifo, alloc_put, lifo, data, lap_ret); \
	lap_ret; \
	})

/**
 * @brief Get an element from a LIFO queue.
 *
 * This routine removes a data item from @a LIFO in a "last in, first out"
 * manner. The first word of the data item is reserved for the kernel's use.
 *
 * @note @a timeout must be set to K_NO_WAIT if called from ISR.
 *
 * @funcprops \isr_ok
 *
 * @param lifo Address of the LIFO queue.
 * @param timeout Waiting period to obtain a data item,
 *                or one of the special values K_NO_WAIT and K_FOREVER.
 *
 * @return Address of the data item if successful; NULL if returned
 * without waiting, or waiting period timed out.
 */
#define k_lifo_get(lifo, timeout) \
	({ \
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_lifo, get, lifo, timeout); \
	void *lg_ret = k_queue_get(&(lifo)->_queue, timeout); \
	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_lifo, get, lifo, timeout, lg_ret); \
	lg_ret; \
	})

/**
 * @brief Statically define and initialize a LIFO queue.
 *
 * The LIFO queue can be accessed outside the module where it is defined using:
 *
 * @code extern struct k_lifo <name>; @endcode
 *
 * @param name Name of the fifo.
 */
#define K_LIFO_DEFINE(name) \
	struct k_lifo name = \
		Z_LIFO_INITIALIZER(name)

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_BT_BLE_LIFO_H__ */