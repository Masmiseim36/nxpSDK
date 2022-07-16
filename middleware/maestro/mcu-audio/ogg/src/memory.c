/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ogg/memory.h>

/*******************************************************************************
 * Code
 ******************************************************************************/
void *ogg_OSA_malloc(uint32_t size)
{
    uint32_t *memory;

    memory = (uint32_t *)OSA_MemoryAllocate(size + (uint32_t)sizeof(uint32_t));
    if (memory != NULL)
    {
        *memory = size;
        /* Caller-usable buffer is immediately after the header. */
        memory++;
    }

    return (void *)memory;
}

void ogg_OSA_free(void *ptr)
{
    uint32_t *memory;

    if (ptr != NULL)
    {
        memory = (uint32_t *)ptr;
        memory--;
        OSA_MemoryFree((void *)memory);
    }
}

void *ogg_OSA_realloc(void *ptr, uint32_t size)
{
    uint32_t *data;
    uint32_t *new_buf;
    uint32_t old_size;

    if (ptr == NULL)
    {
        return ogg_OSA_malloc(size);
    }

    if (size == 0U)
    {
        ogg_OSA_free(ptr);
        return NULL;
    }

    data = (uint32_t *)ptr;
    data--;
    old_size = data[0];

    new_buf = ogg_OSA_malloc(size);
    if (new_buf != NULL)
    {
        (void *)memcpy(new_buf, ptr, (size < old_size) ? size : old_size);
        ogg_OSA_free(ptr);
    }

    return new_buf;
}
