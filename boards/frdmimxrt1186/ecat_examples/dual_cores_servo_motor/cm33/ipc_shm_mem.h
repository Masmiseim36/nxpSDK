/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __IPC__SHM_MEM__H__
#define __IPC__SHM_MEM__H__
#include "ipc_shm.h"

struct ipc_shm_mem
{
    volatile  void *base;
    int num;
    size_t size;
};

int ipc_shm_memmap(uint32_t base, size_t size, struct ipc_shm_mem *mem);
int ipc_shm_unmap(struct ipc_shm_mem *mem);

#endif
