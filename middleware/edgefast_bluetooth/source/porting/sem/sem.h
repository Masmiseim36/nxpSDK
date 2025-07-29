/*
 * Copyright 2024-2025 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __EDGEFAST_BT_BLE_SEM_H__
#define __EDGEFAST_BT_BLE_SEM_H__

#include <system_c.h>

#ifdef __cplusplus
extern "C" {
#endif

struct k_sem {
	SemaphoreHandle_t sem;\
	StaticSemaphore_t sem_buffer;
	unsigned int count;
	unsigned int limit;

	_POLL_EVENT;

	SYS_PORT_TRACING_TRACKING_FIELD(k_sem)
};

/**
 * @cond INTERNAL_HIDDEN
 */

#define Z_SEM_INITIALIZER(obj, initial_count, count_limit) \
	{ \
	.sem = (SemaphoreHandle_t)NULL, \
	.sem_buffer = {{0}}, \
	.count = initial_count, \
	.limit = count_limit, \
	}

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @defgroup semaphore_apis Semaphore APIs
 * @ingroup kernel_apis
 * @{
 */

/**
 * @brief Maximum limit value allowed for a semaphore.
 *
 * This is intended for use when a semaphore does not have
 * an explicit maximum limit, and instead is just used for
 * counting purposes.
 *
 */
#define K_SEM_MAX_LIMIT UINT_MAX

/**
 * @brief Initialize a semaphore.
 *
 * This routine initializes a semaphore object, prior to its first use.
 *
 * @param sem Address of the semaphore.
 * @param initial_count Initial semaphore count.
 * @param limit Maximum permitted semaphore count.
 *
 * @see K_SEM_MAX_LIMIT
 *
 * @retval 0 Semaphore created successfully
 * @retval -EINVAL Invalid values
 *
 */
__syscall int k_sem_init(struct k_sem *sem, unsigned int initial_count,
			  unsigned int limit);

/**
 * @brief Take a semaphore.
 *
 * This routine takes @a sem.
 *
 * @note @a timeout must be set to K_NO_WAIT if called from ISR.
 *
 * @funcprops \isr_ok
 *
 * @param sem Address of the semaphore.
 * @param timeout Waiting period to take the semaphore,
 *                or one of the special values K_NO_WAIT and K_FOREVER.
 *
 * @retval 0 Semaphore taken.
 * @retval -EBUSY Returned without waiting.
 * @retval -EAGAIN Waiting period timed out,
 *			or the semaphore was reset during the waiting period.
 */
__syscall int k_sem_take(struct k_sem *sem, k_timeout_t timeout);

/**
 * @brief Give a semaphore.
 *
 * This routine gives @a sem, unless the semaphore is already at its maximum
 * permitted count.
 *
 * @funcprops \isr_ok
 *
 * @param sem Address of the semaphore.
 */
__syscall void k_sem_give(struct k_sem *sem);

/**
 * @brief Resets a semaphore's count to zero.
 *
 * This routine sets the count of @a sem to zero.
 * Any outstanding semaphore takes will be aborted
 * with -EAGAIN.
 *
 * @param sem Address of the semaphore.
 */
__syscall void k_sem_reset(struct k_sem *sem);

/**
 * @brief Get a semaphore's count.
 *
 * This routine returns the current count of @a sem.
 *
 * @param sem Address of the semaphore.
 *
 * @return Current semaphore count.
 */
__syscall unsigned int k_sem_count_get(struct k_sem *sem);

/**
 * @internal
 */

/**
 * @brief Statically define and initialize a semaphore.
 *
 * The semaphore can be accessed outside the module where it is defined using:
 *
 * @code extern struct k_sem <name>; @endcode
 *
 * @param name Name of the semaphore.
 * @param initial_count Initial semaphore count.
 * @param count_limit Maximum permitted semaphore count.
 */
#define K_SEM_DEFINE(name, initial_count, count_limit) \
	struct k_sem name = \
		Z_SEM_INITIALIZER(name, initial_count, count_limit); \
	BUILD_ASSERT(((count_limit) != 0) && \
		     ((initial_count) <= (count_limit)) && \
			 ((count_limit) <= K_SEM_MAX_LIMIT));

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_BT_BLE_SEM_H__ */
