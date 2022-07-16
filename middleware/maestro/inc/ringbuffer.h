/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

/*!
 * @file    ringbuffer.h
 * @brief   Implements a data ringbuffer.
 */

#include <stdint.h>

/**
 * @brief Ringbuffer structure
 *
 */
struct ringbuf
{
    uint8_t *buf;  /*!< @brief buffer pointer */
    uint32_t head; /*!< @brief buffer head */
    uint32_t tail; /*!< @brief buffer tail */
    uint32_t size; /*!< @brief buffer size */
    uint32_t occ;  /*!< @brief buffer occupancy*/
};

typedef struct ringbuf ringbuf_t;

/*!
 * @brief    Create and initialize a ring buffer structure
 *
 * @param    size Buffer size to be malloc'd
 * @returns  Initialized and allocated pointer to ring buffer object
 */
ringbuf_t *ringbuf_create(uint32_t size);

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

#endif
