/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ringbuffer.h"
#include "fsl_common_dsp.h"

#define AUDIO_BUFFER_SIZE (32 * 1024)

#ifdef HAVE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#else
#include <xtensa/xos.h>
#endif

#if (XCHAL_DCACHE_SIZE > 0)
AT_NONCACHEABLE_SECTION_ALIGN(
#else
SDK_ALIGN(
#endif
    static ringbuf_t ringbuf, 16
);

#if (XCHAL_DCACHE_SIZE > 0)
AT_NONCACHEABLE_SECTION_ALIGN(
#else
SDK_ALIGN(
#endif
    static uint8_t buffer[AUDIO_BUFFER_SIZE], 16
);

/*******************************************************************************
 * Code
 ******************************************************************************/
ringbuf_t *ringbuf_create(void)
{
    ringbuf_t *rb;

    rb = &ringbuf;
    if (!rb)
        return NULL;

    rb->head = 0;
    rb->tail = 0;
    rb->occ  = 0;
    rb->size = AUDIO_BUFFER_SIZE;
    rb->buf  = (uint8_t *)&buffer;

    return rb;
}

void ringbuf_clear(ringbuf_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->occ  = 0;
}

uint32_t ringbuf_get_occupancy(ringbuf_t *rb)
{
    return rb->occ;
}

uint32_t ringbuf_write(ringbuf_t *rb, uint8_t *data, uint32_t size)
{
    uint32_t remaining = rb->size - rb->tail;

    /* MUTEX LOCK */
    // printf("write (%d)\n", size);

    if (size > (rb->size - rb->occ))
    {
        /* Overflow - cap write size to available space. */
        size = rb->size - rb->occ;
    }

    /* Split write into two if it will overflow. */
    if (size > remaining)
    {
        /* First copy to end of buffer. */
        memcpy(rb->buf + rb->tail, data, remaining);
        /* Second copy from start of buffer, remaining size. */
        memcpy(rb->buf, data + remaining, size - remaining);
        /* Set write point to 0 + <second copy size>. */
        rb->tail = size - remaining;
    }
    else
    {
        memcpy(rb->buf + rb->tail, data, size);
        rb->tail += size;
    }

    rb->occ += size;

    // printf("head: %d, tail: %d, occ: %d\n", rb->head, rb->tail, rb->occ);

    return size;
}

static uint32_t _ringbuf_read_internal(ringbuf_t *rb, uint8_t *data, uint32_t size, bool peek)
{
    uint32_t remaining = rb->size - rb->head;

    /* MUTEX LOCK */
    // printf("read (%d)\n", size);

    if (size > rb->occ)
    {
        /* Underrun - cap read at max available. */
        size = rb->occ;
    }

    if (size > remaining)
    {
        if (data)
        {
            /* First copy to end of buffer. */
            memcpy(data, rb->buf + rb->head, remaining);
            /* Second copy from start of buffer, remaining size. */
            memcpy(data + remaining, rb->buf, size - remaining);
        }

        if (!peek)
        {
            rb->head = size - remaining;
        }
    }
    else
    {
        if (data)
        {
            memcpy(data, rb->buf + rb->head, size);
        }

        if (!peek)
        {
            rb->head += size;
        }
    }

    if (!peek)
    {
        rb->occ -= size;
    }

    // printf("head: %d, tail: %d, occ: %d\n", rb->head, rb->tail, rb->occ);

    return size;
}

uint32_t ringbuf_read(ringbuf_t *rb, uint8_t *data, uint32_t size)
{
    return _ringbuf_read_internal(rb, data, size, false);
}

uint32_t ringbuf_peek(ringbuf_t *rb, uint8_t *data, uint32_t size)
{
    return _ringbuf_read_internal(rb, data, size, true);
}
