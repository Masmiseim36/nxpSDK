/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    ringbuffer.c
 * @brief   Implements a data ringbuffer.
 */

#include <string.h>
#include "ringbuffer.h"
#include "fsl_os_abstraction.h"

ringbuf_t *ringbuf_create(uint32_t size)
{
    ringbuf_t *rb;

    rb = OSA_MemoryAllocate(sizeof(struct ringbuf));
    if (!rb)
        return NULL;

    rb->head = 0;
    rb->tail = 0;
    rb->occ  = 0;
    rb->size = size;
    rb->buf  = OSA_MemoryAllocate(size);
    if (!rb->buf)
    {
        OSA_MemoryFree(rb);
        rb = NULL;
    }

    return rb;
}

void ringbuf_destroy(ringbuf_t *rb)
{
    OSA_MemoryFree(rb->buf);
    rb->buf = NULL;
    OSA_MemoryFree(rb);
    rb = NULL;
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

/**
 * @brief Read data from a ring buffer
 *
 * @param rb   Pointer to ringbuffer structure
 * @param data Data pointer
 * @param size Size to be read
 * @param peek Flag for moving ringbuffer head
 * @return uint32_t
 */
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

/* UNIT TEST */
#if 0
void dump(int8_t buffer[], int size)
{
    for (int i = 0; i < size; i++)
        printf("%c", buffer[i]);
    printf("\n\n");
}

int main(void)
{
    uint32_t ret;
    int8_t data_read[1000];
    int8_t data_write[255];

    memset(data_read, 0, 1000);
    memset(data_write, 66, 255);

    for (int x = 0; x < 4; x++) {
        for (int i = 65; i < 91; i++)
            data_write[(26 * x) + (i - 65)] = i;
    }

    printf("WRITE buffer:\n");
    dump(data_write, 100);

    ringbuf_t *rb = ringbuf_create(100);

    ringbuf_write(rb, data_write, 10);
    dump(rb->buf, 100);
    ringbuf_write(rb, data_write, 20);
    dump(rb->buf, 100);
    ringbuf_read(rb, data_read, 15);
    dump(data_read, 15);
    ringbuf_write(rb, data_write, 60);
    dump(rb->buf, 100);
    ringbuf_read(rb, data_read, 15);
    dump(data_read, 15);
    ringbuf_write(rb, data_write, 30);
    dump(rb->buf, 100);
    ringbuf_read(rb, data_read, 85);
    dump(data_read, 85);

    ringbuf_write(rb, data_write, 55);
    dump(rb->buf, 100);
    ringbuf_read(rb, data_read, 50);
    dump(data_read, 50);
    ringbuf_write(rb, data_write, 30);
    dump(rb->buf, 100);
    ringbuf_read(rb, data_read, 36);
    dump(data_read, 36);

    /* OVERFLOW */
    /*
    ret = ringbuf_read(rb, data_read, 5);
    if (ret != 5)
        printf("main UNDERRUN: %d\n", ret);

    ret = ringbuf_read(rb, data_read, 2);
    if (ret != 2)
        printf("main UNDERRUN: %d\n", ret);
    */

    /* OVERFLOW */
    /*
    ret = ringbuf_write(rb, data_write, 98);
    if (ret != 98)
        printf("main OVERFLOW: %d\n\n", ret);

    ret = ringbuf_write(rb, data_write, 2);
    if (ret != 2)
        printf("main OVERFLOW: %d\n\n", ret);

    ret = ringbuf_read(rb, data_read, 5);
    if (ret != 5)
        printf("main UNDERRUN: %d\n\n", ret);

    ret = ringbuf_write(rb, data_write, 2);
    if (ret != 2)
        printf("main OVERFLOW: %d\n\n", ret);
    */

    ringbuf_destroy(rb);

    return 0;
}
#endif
