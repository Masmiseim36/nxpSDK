/*
 * Copyright 2024-2025 NXP
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __EDGEFAST_BT_BLE_SLAB_H__
#define __EDGEFAST_BT_BLE_SLAB_H__

#include <system_c.h>

/**
 * @cond INTERNAL_HIDDEN
 */

struct k_mem_slab_info {
	uint32_t num_blocks;
	size_t   block_size;
	uint32_t num_used;
#ifdef CONFIG_MEM_SLAB_TRACE_MAX_UTILIZATION
	uint32_t max_used;
#endif
};

struct k_mem_slab {
	struct k_sem sem;
	struct k_spinlock lock;
	char *buffer;
	char *free_list;
	struct k_mem_slab_info info;

	SYS_PORT_TRACING_TRACKING_FIELD(k_mem_slab)

#ifdef CONFIG_OBJ_CORE_MEM_SLAB
	struct k_obj_core  obj_core;
#endif
};

#define Z_MEM_SLAB_INITIALIZER(_slab, _slab_buffer, _slab_block_size, \
			       _slab_num_blocks)                      \
	{                                                             \
	.sem = {0},                                                   \
	.lock = {0},                                                  \
	.buffer = _slab_buffer,                                       \
	.free_list = NULL,                                            \
	.info = {_slab_num_blocks, _slab_block_size, 0}               \
	}

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @defgroup mem_slab_apis Memory Slab APIs
 * @ingroup kernel_apis
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Statically define and initialize a memory slab in a public (non-static) scope.
 *
 * The memory slab's buffer contains @a slab_num_blocks memory blocks
 * that are @a slab_block_size bytes long. The buffer is aligned to a
 * @a slab_align -byte boundary. To ensure that each memory block is similarly
 * aligned to this boundary, @a slab_block_size must also be a multiple of
 * @a slab_align.
 *
 * The memory slab can be accessed outside the module where it is defined
 * using:
 *
 * @code extern struct k_mem_slab <name>; @endcode
 *
 * @note This macro cannot be used together with a static keyword.
 *       If such a use-case is desired, use @ref K_MEM_SLAB_DEFINE_STATIC
 *       instead.
 *
 * @param name Name of the memory slab.
 * @param slab_block_size Size of each memory block (in bytes).
 * @param slab_num_blocks Number memory blocks.
 * @param slab_align Alignment of the memory slab's buffer (power of 2).
 */
#define K_MEM_SLAB_DEFINE(name, slab_block_size, slab_num_blocks, slab_align) \
	char __noinit_named(k_mem_slab_buf_##name) \
	   __aligned(WB_UP(slab_align)) \
	   _k_mem_slab_buf_##name[(slab_num_blocks) * WB_UP(slab_block_size)]; \
	STRUCT_SECTION_ITERABLE(k_mem_slab, name) = \
		Z_MEM_SLAB_INITIALIZER(name, _k_mem_slab_buf_##name, \
					WB_UP(slab_block_size), slab_num_blocks)

/**
 * @brief Statically define and initialize a memory slab in a private (static) scope.
 *
 * The memory slab's buffer contains @a slab_num_blocks memory blocks
 * that are @a slab_block_size bytes long. The buffer is aligned to a
 * @a slab_align -byte boundary. To ensure that each memory block is similarly
 * aligned to this boundary, @a slab_block_size must also be a multiple of
 * @a slab_align.
 *
 * @param name Name of the memory slab.
 * @param slab_block_size Size of each memory block (in bytes).
 * @param slab_num_blocks Number memory blocks.
 * @param slab_align Alignment of the memory slab's buffer (power of 2).
 */
#define K_MEM_SLAB_DEFINE_STATIC(name, slab_block_size, slab_num_blocks, slab_align) \
	static char __noinit_named(k_mem_slab_buf_##name) \
	   __aligned(WB_UP(slab_align)) \
	   _k_mem_slab_buf_##name[(slab_num_blocks) * WB_UP(slab_block_size)]; \
	static STRUCT_SECTION_ITERABLE(k_mem_slab, name) = \
		Z_MEM_SLAB_INITIALIZER(name, _k_mem_slab_buf_##name, \
					WB_UP(slab_block_size), slab_num_blocks)

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
extern int k_mem_slab_init(struct k_mem_slab *slab, void *buffer,
			   size_t block_size, uint32_t num_blocks);

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
extern int k_mem_slab_alloc(struct k_mem_slab *slab, void **mem,
			    k_timeout_t timeout);

/**
 * @brief Free memory allocated from a memory slab.
 *
 * This routine releases a previously allocated memory block back to its
 * associated memory slab.
 *
 * @param slab Address of the memory slab.
 * @param mem Pointer to the memory block (as returned by k_mem_slab_alloc()).
 */
extern void k_mem_slab_free(struct k_mem_slab *slab, void *mem);

/**
 * @brief Get the number of used blocks in a memory slab.
 *
 * This routine gets the number of memory blocks that are currently
 * allocated in @a slab.
 *
 * @param slab Address of the memory slab.
 *
 * @return Number of allocated memory blocks.
 */
static inline uint32_t k_mem_slab_num_used_get(struct k_mem_slab *slab)
{
	return slab->info.num_used;
}

/**
 * @brief Get the number of maximum used blocks so far in a memory slab.
 *
 * This routine gets the maximum number of memory blocks that were
 * allocated in @a slab.
 *
 * @param slab Address of the memory slab.
 *
 * @return Maximum number of allocated memory blocks.
 */
static inline uint32_t k_mem_slab_max_used_get(struct k_mem_slab *slab)
{
#ifdef CONFIG_MEM_SLAB_TRACE_MAX_UTILIZATION
	return slab->info.max_used;
#else
	ARG_UNUSED(slab);
	return 0;
#endif
}

/**
 * @brief Get the number of unused blocks in a memory slab.
 *
 * This routine gets the number of memory blocks that are currently
 * unallocated in @a slab.
 *
 * @param slab Address of the memory slab.
 *
 * @return Number of unallocated memory blocks.
 */
static inline uint32_t k_mem_slab_num_free_get(struct k_mem_slab *slab)
{
	return slab->info.num_blocks - slab->info.num_used;
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

int k_mem_slab_runtime_stats_get(struct k_mem_slab *slab, struct sys_memory_stats *stats);

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
int k_mem_slab_runtime_stats_reset_max(struct k_mem_slab *slab);

#ifdef __cplusplus
}
#endif

/** @} */
/** @} */

#endif /* __EDGEFAST_BT_BLE_SLAB_H__*/
