/*
 * Copyright 2023 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>

/* SLAB */
STRUCT_RW_SECTION_DEFINE(k_mem_slab);

#if 0
STRUCT_SECTION_START_EXTERN(k_mem_slab);
STRUCT_SECTION_END_EXTERN(k_mem_slab);
#endif

/**
 * @brief Initialize a memory slab.
 *
 * Initializes a memory slab, prior to its first use.
 *
 * The memory slab's buffer contains @a slab_num_blocks memory blocks
 * that are @a slab_block_size bytes long. The buffer must be aligned to an
 * N-byte boundary matching a word boundary, where N is a power of 2
 * (i.e. 4 on 32-bit systems, 8, 16, ...).
 * To ensure that each memory block is similarly aligned to this boundary,
 * @a slab_block_size must also be a multiple of N.
 *
 * @param slab Address of the memory slab.
 * @param buffer Pointer to buffer used for the memory blocks.
 * @param block_size Size of each memory block (in bytes).
 * @param num_blocks Number of memory blocks.
 *
 * @retval 0 on success
 * @retval -EINVAL invalid data supplied
 *
 */
int k_mem_slab_init(struct k_mem_slab *slab, void *buffer,
			   size_t block_size, uint32_t num_blocks)
{
	char *p;
	k_spinlock_key_t key;
	int ret;

	if ((slab == NULL) || (NULL == buffer) || (0 == block_size) || (0 == num_blocks)) {
		return -EINVAL;
	}

	slab->lock = (struct k_spinlock) {0};

	key = k_spin_lock(&slab->lock);

	slab->info.block_size = block_size;
	slab->info.num_blocks = num_blocks;
	slab->info.num_used = 0U;
	slab->buffer = buffer;
	slab->free_list =  NULL;

	#ifdef CONFIG_MEM_SLAB_TRACE_MAX_UTILIZATION
		slab->info.max_used = 0U;
	#endif

	p = (char *)buffer;

	for (int i = 0; i < num_blocks; i++)
	{
		*((char **)p) = slab->free_list;
		slab->free_list = p;
		p = p + block_size;
	}

	ret = k_sem_init(&slab->sem, num_blocks, num_blocks);

	k_spin_unlock(&slab->lock, key);

	SYS_PORT_TRACING_OBJ_INIT(k_mem_slab, slab, ret);

	return ret;
}

/**
 * @brief Allocate memory from a memory slab.
 *
 * This routine allocates a memory block from a memory slab.
 *
 * @note @a timeout must be set to K_NO_WAIT if called from ISR.
 * @note When CONFIG_MULTITHREADING=n any @a timeout is treated as K_NO_WAIT.
 *
 * @funcprops \isr_ok
 *
 * @param slab Address of the memory slab.
 * @param mem Pointer to block address area.
 * @param timeout Non-negative waiting period to wait for operation to complete.
 *        Use K_NO_WAIT to return without waiting,
 *        or K_FOREVER to wait as long as necessary.
 *
 * @retval 0 Memory allocated. The block address area pointed at by @a mem
 *         is set to the starting address of the memory block.
 * @retval -ENOMEM Returned without waiting.
 * @retval -EAGAIN Waiting period timed out.
 * @retval -EINVAL Invalid data supplied
 */
int k_mem_slab_alloc(struct k_mem_slab *slab, void **mem,
			    k_timeout_t timeout)
{
	int ret;
	char *p;
	k_spinlock_key_t key;
	uint32_t start = k_uptime_get_32();
	uint32_t current;

	__ASSERT(!((slab == NULL) || (mem == NULL)), "Invalid memory pointer provided");

	if ((slab == NULL) || (mem == NULL)) {
		return -EINVAL;
	}

	if (NULL == slab->sem.sem)
	{
		ret = k_mem_slab_init(slab, slab->buffer, slab->info.block_size, slab->info.num_blocks);
		if (ret < 0)
		{
			return ret;
		}
	}

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_mem_slab, alloc, slab, timeout);

	key = k_spin_lock(&slab->lock);
	do
	{
		if (NULL != slab->free_list)
		{
			p = slab->free_list;
			slab->free_list = *((char **)p);
			*mem = p;
			slab->info.num_used++;
#ifdef CONFIG_MEM_SLAB_TRACE_MAX_UTILIZATION
			slab->info.max_used = MAX(slab->info.num_used,
				slab->info.max_used);
#endif
			k_spin_unlock(&slab->lock, key);
			(void)k_sem_take(&slab->sem, K_NO_WAIT);
			key = k_spin_lock(&slab->lock);
			ret = 0;
			break;
		}
		else
		{
			if (K_TIMEOUT_EQ(timeout, K_NO_WAIT))
			{
				*mem = NULL;
				ret = -ENOMEM;
				break;
			}
			else
			{
				k_spin_unlock(&slab->lock, key);
				(void)k_sem_take(&slab->sem, timeout);
				key = k_spin_lock(&slab->lock);
				current = k_uptime_get_32();
				if ((current - start) >= timeout)
				{
					*mem = NULL;
					ret = -ENOMEM;
					break;
				}
			}
		}
	} while (true);

	k_spin_unlock(&slab->lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_mem_slab, alloc, slab, timeout, ret);

	return ret;
}

/**
 * @brief Free memory allocated from a memory slab.
 *
 * This routine releases a previously allocated memory block back to its
 * associated memory slab.
 *
 * @param slab Address of the memory slab.
 * @param mem Pointer to the memory block (as returned by k_mem_slab_alloc()).
 */
void k_mem_slab_free(struct k_mem_slab *slab, void *mem)
{
	int ret;
	k_spinlock_key_t key;

	if (slab == NULL) {
		return;
	}

	if (NULL == slab->sem.sem)
	{
		ret = k_mem_slab_init(slab, slab->buffer, slab->info.block_size, slab->info.num_blocks);
		if (ret < 0)
		{
			return;
		}
	}

	__ASSERT(((char *)mem >= slab->buffer) &&
		 ((((char *)mem - slab->buffer) % slab->info.block_size) == 0) &&
		 ((char *)mem <= (slab->buffer + (slab->info.block_size *
						  (slab->info.num_blocks - 1)))),
		 "Invalid memory pointer provided");

	if (!(((char *)mem >= slab->buffer) &&
		((((char *)mem - slab->buffer) % slab->info.block_size) == 0) &&
		((char *)mem <= (slab->buffer + (slab->info.block_size *
		(slab->info.num_blocks - 1))))))
	{
		/* Invalid memory pointer */
		return;
	}
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_mem_slab, free, slab);

	key = k_spin_lock(&slab->lock);


	*((char **)mem) = slab->free_list;
	slab->free_list = (char *)mem;
	slab->info.num_used --;

	k_spin_unlock(&slab->lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_mem_slab, free, slab);
}

/**
 * @brief Get the memory stats for a memory slab
 *
 * This routine gets the runtime memory usage stats for the slab @a slab.
 *
 * @param slab Address of the memory slab
 * @param stats Pointer to memory into which to copy memory usage statistics
 *
 * @retval 0 Success
 * @retval -EINVAL Any parameter points to NULL
 */

int k_mem_slab_runtime_stats_get(struct k_mem_slab *slab, struct sys_memory_stats *stats)
{
	k_spinlock_key_t key;

	if ((slab == NULL) || (stats == NULL)) {
		return -EINVAL;
	}

	key = k_spin_lock(&slab->lock);

	stats->allocated_bytes = slab->info.num_used * slab->info.block_size;
	stats->free_bytes = (slab->info.num_blocks - slab->info.num_used) *
			    slab->info.block_size;
#ifdef CONFIG_MEM_SLAB_TRACE_MAX_UTILIZATION
	stats->max_allocated_bytes = slab->info.max_used *
				     slab->info.block_size;
#else
	stats->max_allocated_bytes = 0;
#endif

	k_spin_unlock(&slab->lock, key);

	return 0;
}

/**
 * @brief Reset the maximum memory usage for a slab
 *
 * This routine resets the maximum memory usage for the slab @a slab to its
 * current usage.
 *
 * @param slab Address of the memory slab
 *
 * @retval 0 Success
 * @retval -EINVAL Memory slab is NULL
 */
#ifdef CONFIG_MEM_SLAB_TRACE_MAX_UTILIZATION
int k_mem_slab_runtime_stats_reset_max(struct k_mem_slab *slab)
{
	if (slab == NULL) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&slab->lock);

	slab->info.max_used = slab->info.num_used;

	k_spin_unlock(&slab->lock, key);

	return 0;
}
#endif
