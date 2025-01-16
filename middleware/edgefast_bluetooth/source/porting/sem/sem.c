/*
 * Copyright 2023 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>

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
int k_sem_init(struct k_sem *sem, unsigned int initial_count,
			  unsigned int limit)
{
	__ASSERT((sem != NULL) || (limit > initial_count) || (limit > 0U), "Invalid parameter!");

	if (!((sem != NULL) || (limit > initial_count) || (limit > 0U)))
	{
		SYS_PORT_TRACING_OBJ_FUNC(k_sem, init, sem, -EINVAL);
		return -EINVAL;
	}

	sem->count = initial_count;
	sem->limit = limit;

	SYS_PORT_TRACING_OBJ_FUNC(k_sem, init, sem, 0);

	if (limit == 1)
	{
		sem->sem = xSemaphoreCreateBinaryStatic(&sem->sem_buffer);
	}
	else
	{
		sem->sem = xSemaphoreCreateCountingStatic(limit, initial_count, &sem->sem_buffer);
	}

	if (sem->sem != (QueueHandle_t)&sem->sem_buffer)
	{
		return -EINVAL;
	}

	if ((limit == 1) && (initial_count > 0))
	{
		k_sem_give(sem);
	}

	return 0;
}

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
int k_sem_take(struct k_sem *sem, k_timeout_t timeout)
{
	int err = -EINVAL;

	if (NULL == sem)
	{
		return err;
	}

	if (sem->sem != (QueueHandle_t)&sem->sem_buffer)
	{
		if (sem->limit == 0)
		{
			return -EINVAL;
		}

		err = k_sem_init(sem, sem->count, sem->limit);
		if (err < 0)
		{
			return err;
		}
	}


	if (k_is_in_isr())
	{
		portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

		if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreTakeFromISR(sem->sem, &taskToWake))
		{
			portYIELD_FROM_ISR(((bool)(taskToWake)));
			err = 0;
		}
		else
		{
			err = -EBUSY;
		}
	}
	else
	{
		if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreTake(sem->sem, timeout))
		{
			err = 0;
		}
		else
		{
			err = -EAGAIN;
		}
	}
	return err;
}

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
void k_sem_give(struct k_sem *sem)
{
	if (NULL == sem)
	{
		return;
	}

	if (sem->sem != (QueueHandle_t)&sem->sem_buffer)
	{
		if (sem->limit == 0)
		{
			return;
		}

		int err = k_sem_init(sem, sem->count, sem->limit);
		if (err < 0)
		{
			return;
		}
	}

	if (k_is_in_isr())
	{
		portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

		if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreGiveFromISR(sem->sem, &taskToWake))
		{
		portYIELD_FROM_ISR(((bool)(taskToWake)));
		}
		else
		{
		}
	}
	else
	{
		if (((BaseType_t)pdTRUE) == (BaseType_t)xSemaphoreGive(sem->sem))
		{
		}
		else
		{
		}
	}
}

/**
 * @brief Resets a semaphore's count to zero.
 *
 * This routine sets the count of @a sem to zero.
 * Any outstanding semaphore takes will be aborted
 * with -EAGAIN.
 *
 * @param sem Address of the semaphore.
 */
void k_sem_reset(struct k_sem *sem)
{
	if (NULL == sem)
	{
		return;
	}

	if (sem->sem != (QueueHandle_t)&sem->sem_buffer)
	{
		if (sem->limit == 0)
		{
			return;
		}

		int err = k_sem_init(sem, sem->count, sem->limit);
		if (err < 0)
		{
			return;
		}
	}
	else
	{
		vSemaphoreDelete(sem->sem);
		sem->sem = NULL;

		(void)k_sem_init(sem, sem->count, sem->limit);
	}
}

/**
 * @brief Get a semaphore's count.
 *
 * This routine returns the current count of @a sem.
 *
 * @param sem Address of the semaphore.
 *
 * @return Current semaphore count.
 */
unsigned int k_sem_count_get(struct k_sem *sem)
{
	int err = -EINVAL;

	if (NULL == sem)
	{
		return err;
	}

	if (sem->sem != (QueueHandle_t)&sem->sem_buffer)
	{
		if (sem->limit == 0)
		{
			return (unsigned int)-EINVAL;
		}

		err = k_sem_init(sem, sem->count, sem->limit);
		if (err < 0)
		{
			return err;
		}
	}

	return  (unsigned int)uxSemaphoreGetCount(sem->sem);
}
