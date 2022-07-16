/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * HAL public types header
 */

#ifndef _HAL_TYPES_H
#define _HAL_TYPES_H

#include "mpp_api_types.h"

/* maximum number of element inputs/outputs */
#define MAX_INPUT_PORTS    2
#define MAX_OUTPUT_PORTS   2

/* The memory allocation policy of an element's hal.
 * During the pipeline construction, the HAL uses this enum
 * to tell the pipeline if it already owns input/ouput buffers.
 * Before the pipeline starts, the memory manager will map the existing
 * buffers to elements and allocate missing buffers from the heap.
 **/
typedef enum mpp_memory_policy_e {
    HAL_MEM_ALLOC_NONE,     /* element requires buffers to be provided by other elements, or by the pipeline */
    HAL_MEM_ALLOC_INPUT,    /* element allocates its input buffer, it may require output buffers to be provided by other elements, or by the pipeline */
    HAL_MEM_ALLOC_OUTPUT,   /* element allocates its output buffer, it may require input buffers to be provided by other elements, or by the pipeline */
    HAL_MEM_ALLOC_BOTH      /* element allocates both its input and output buffers */
} mpp_memory_policy_t;

/* the hardware specific buffer requirements */
typedef struct {
    int stride;             /* the number of bytes between 2 lines of image */
    int alignment;          /* alignment requirement in bytes */
    bool cacheable;         /* if true, HW will require cache maintenance */
    unsigned char *addr;    /* the buffer address */
} hw_buf_desc_t;

#endif /* _HAL_TYPES_H */
