/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ipc_shm_mem.h"

int ipc_shm_memmap(uint32_t base, size_t size, struct ipc_shm_mem *mem)
{
	if (mem->num == 0) {
		mem->base = (volatile void *)base;
		mem->size = size;
	}
	mem->num++;
    return 0;
}

int ipc_shm_unmap(struct ipc_shm_mem *mem)
{
	if (mem->num == 1 && mem->base != NULL) {
		mem->base = NULL;
	}
	mem->num--;
    return 0;
}
