/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(__RING_BUFFER_H__)
#define __RING_BUFFER_H__

#include <stdint.h>

#if !defined(WIN32)
#include <stdbool.h>
#endif

//! @addtogroup ring_buffer
//! @{

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

//! @brief Ring buffer object.
//!
//! The read and write heads will point to the same byte either when the ring
//! buffer is empty, or when it is full. You can use the filled member to tell
//! which of those two cases applies.
typedef struct _ring_buffer
{
    uint8_t *buffer;    //!< Pointer to the buffer.
    uint32_t length;    //!< Length in bytes of the buffer.
    uint32_t filled;    //!< Number of bytes currently in the buffer.
    uint8_t *readHead;  //!< Pointer to next byte to read.
    uint8_t *writeHead; //!< Pointer to next byte to write.
} ring_buffer_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if __cplusplus
extern "C" {
#endif

//! @name Ring buffer
//@{
//! @brief Initialize a ring buffer.
extern void ring_buffer_init(ring_buffer_t *ring, uint8_t *buffer, uint32_t length);

//! @brief Read data from a ring buffer.
extern uint32_t ring_buffer_read(ring_buffer_t *ring, uint8_t *buffer, uint32_t count);

//! @brief Put data into a ring buffer.
extern uint32_t ring_buffer_write(ring_buffer_t *ring, const uint8_t *buffer, uint32_t count);

//! @brief Returns the number of bytes in a ring buffer.
extern uint32_t ring_buffer_get_count(ring_buffer_t *ring);

//! @brief Returns whether a ring buffer is empty.
extern bool ring_buffer_is_empty(ring_buffer_t *ring);
//@}

#if __cplusplus
}
#endif

//! @}

#endif // __RING_BUFFER_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
