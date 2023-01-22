/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "board.h"

#include "tx_api.h"

#define SE05X_BYTE_POOL_SIZE    (1024 * 48)

AT_NONCACHEABLE_SECTION_ALIGN(uint8_t se05x_byte_pool[SE05X_BYTE_POOL_SIZE], 64);

static TX_BYTE_POOL se05x_pool;
static UINT memory_pool_initialized = 0;

UINT se_memory_pool_init(VOID)
{
    UINT ret;

    ret = tx_byte_pool_create(&se05x_pool,
                              "memory_pool",
                              se05x_byte_pool,
                              sizeof(se05x_byte_pool));
    if(ret != TX_SUCCESS)
    {
        return ret;
    }

    memory_pool_initialized = 1;

    return TX_SUCCESS;
}

void *threadx_malloc(size_t size)
{
    UINT ret;
    void *mem;

    if (memory_pool_initialized == 1)
    {
        ret = tx_byte_allocate(&se05x_pool, &mem, size, 0);
        if(ret != TX_SUCCESS) {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }

    return mem;
}

void *threadx_calloc(size_t size)
{
    void *mem;

    if (memory_pool_initialized == 1)
    {
        mem = threadx_malloc(size);
        if (mem == NULL)
        {
            return NULL;
        }

        memset(mem, 0, size);
    }
    else
    {
        return NULL;
    }

    return mem;
}

void threadx_free(void *ptr)
{
    if (memory_pool_initialized == 1 && ptr != NULL)
    {
        tx_byte_release(ptr);
    }
}

