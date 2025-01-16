/*
 * Copyright 2022-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*! \addtogroup HAL_TYPES
*  @{
*/

/**
 * HAL public types header
 */

#ifndef _HAL_TYPES_H
#define _HAL_TYPES_H

#include "mpp_api_types.h"

/** maximum number of element inputs/outputs */
#define MAX_INPUT_PORTS    2
#define MAX_OUTPUT_PORTS   2

/** The memory allocation policy of an element's hal.
 * During the pipeline construction, the HAL uses this enum
 * to tell the pipeline if it already owns input/ouput buffers.
 * Before the pipeline starts, the memory manager will map the existing
 * buffers to elements and allocate missing buffers from the heap.
 **/
typedef enum mpp_memory_policy_e {
    HAL_MEM_ALLOC_NONE,     /*!< element requires buffers to be provided by other elements, or by the pipeline */
    HAL_MEM_ALLOC_INPUT,    /*!< element allocates its input buffer, it may require output buffers to be provided by other elements, or by the pipeline */
    HAL_MEM_ALLOC_OUTPUT,   /*!< element allocates its output buffer, it may require input buffers to be provided by other elements, or by the pipeline */
    HAL_MEM_ALLOC_BOTH      /*!< element allocates both its input and output buffers */
} mpp_memory_policy_t;

/** the hardware specific buffer requirements */
typedef struct {
    int stride;             /*!< the number of bytes between 2 lines of image */
    int nb_lines;           /*!< the number of lines required (set to 0 if the element doesn't require a specific number of lines) */
    int alignment;          /*!< alignment requirement in bytes */
    bool cacheable;         /*!< if true, HW will require cache maintenance */
    unsigned char *addr;    /*!< the aligned buffer address */
    unsigned char *heap_p;  /*!< pointer to the heap that should be freed */
} hw_buf_desc_t;

/** maximum length of device name */
#define HAL_DEVICE_NAME_MAX_LENGTH  (64)

/** @} */

#endif /* _HAL_TYPES_H */
