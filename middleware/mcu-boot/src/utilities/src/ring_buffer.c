/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "utilities/fsl_assert.h"
#include "utilities/ring_buffer.h"

#if !defined(BOOTLOADER_HOST)
#include "fsl_device_registers.h"
#include "utilities/fsl_rtos_abstraction.h"

#endif

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
// The ring buffer could protect itself by disabling/enabling IRQs

void ring_buffer_init(ring_buffer_t *ring, uint8_t *buffer, uint32_t length)
{
    ring->buffer = buffer;
    ring->length = length;
    ring->filled = 0;
    ring->readHead = buffer;
    ring->writeHead = buffer;
}

uint32_t ring_buffer_read(ring_buffer_t *ring, uint8_t *buffer, uint32_t count)
{
    assert(ring);
    assert(ring->buffer);

#if !defined(BOOTLOADER_HOST)
    lock_acquire();
#endif

    uint32_t actual = 0;
    while (actual < count && ring->filled > 0)
    {
        *buffer++ = *(ring->readHead)++;
        --ring->filled;
        ++actual;

        if (ring->readHead >= ring->buffer + ring->length)
        {
            ring->readHead = ring->buffer;
        }
    }

#if !defined(BOOTLOADER_HOST)
    lock_release();
#endif
    return actual;
}

uint32_t ring_buffer_write(ring_buffer_t *ring, const uint8_t *buffer, uint32_t count)
{
    assert(ring);
    assert(ring->buffer);

#if !defined(BOOTLOADER_HOST)
    lock_acquire();
#endif

    uint32_t actual = 0;
    while (actual < count && (ring->filled == 0 || ring->writeHead != ring->readHead))
    {
        *(ring->writeHead)++ = *buffer++;
        ++ring->filled;
        ++actual;

        if (ring->writeHead >= ring->buffer + ring->length)
        {
            ring->writeHead = ring->buffer;
        }
    }

#if !defined(BOOTLOADER_HOST)
    lock_release();
#endif
    return actual;
}

uint32_t ring_buffer_get_count(ring_buffer_t *ring)
{
    assert(ring);

#if !defined(BOOTLOADER_HOST)
    lock_acquire();
    uint32_t temp = ring->filled;
    lock_release();

    return temp;
#else
    return ring->filled;
#endif
}

bool ring_buffer_is_empty(ring_buffer_t *ring)
{
    assert(ring);

#if !defined(BOOTLOADER_HOST)
    lock_acquire();
    bool temp = (ring->filled == 0);
    lock_release();

    return temp;
#else
    return (ring->filled == 0);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
