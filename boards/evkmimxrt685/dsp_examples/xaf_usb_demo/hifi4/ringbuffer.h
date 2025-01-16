/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
struct ringbuf
{
    uint8_t *buf;
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint32_t occ;
};

typedef struct ringbuf ringbuf_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief    Create and initialize a ring buffer structure
 *
 * @returns  Initialized and allocated pointer to ring buffer object
 */
ringbuf_t *ringbuf_create(void);

/*!
 * @brief    Destroy and free ring buffer structure
 *
 * @param    rb Pointer to ring buffer object created with ringbuf_create
 */
void ringbuf_destroy(ringbuf_t *rb);

/*!
 * @brief    Empty ring buffer structure
 *
 * @param    rb Pointer to ring buffer object created with ringbuf_create
 */
void ringbuf_clear(ringbuf_t *rb);

/*!
 * @brief    Return the number of bytes stored in the ringbuffer
 *
 * @param    rb Pointer to ring buffer object created with ringbuf_create
 * @returns  Number of bytes of data currently occupying space in the buffer
 */
uint32_t ringbuf_get_occupancy(ringbuf_t *rb);

/*!
 * @brief    Write data to ring buffer
 *
 * @param    rb Pointer to ring buffer object created with ringbuf_create
 * @param    data Input buffer of data to write to ring buffer
 * @param    size Number of bytes to write to ring buffer, from 'data'
 * @returns  Number of bytes successfully written to buffer
 */
uint32_t ringbuf_write(ringbuf_t *rb, uint8_t *data, uint32_t size);

/*!
 * @brief    Read data from ring buffer
 *
 * @param    rb Pointer to ring buffer object created with ringbuf_create
 * @param    data Output buffer of data to write from ring buffer.
 *                Can be NULL, in which case data is discarded.
 * @param    size Number of bytes to read from ring buffer, to store in 'data'
 * @returns  Number of bytes successfully read to buffer or discarded
 */
uint32_t ringbuf_read(ringbuf_t *rb, uint8_t *data, uint32_t size);

/*!
 * @brief    Peak data from ring buffer without removing it
 *
 * @param    rb Pointer to ring buffer object created with ringbuf_create
 * @param    data Output buffer of data to write from ring buffer.
 * @param    size Number of bytes to read from ring buffer, to store in 'data'
 * @returns  Number of bytes successfully read to buffer
 */
uint32_t ringbuf_peek(ringbuf_t *rb, uint8_t *data, uint32_t size);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _RINGBUFFER_H_ */
