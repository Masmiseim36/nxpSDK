/*
 * Copyright 2022-2024 NXP.
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

#include "mpp_api.h"
#include "mpp_api_types_internal.h"
#include "hal_types.h"
#include "mpp_debug.h"
#include "hal_utils.h"
#include "hal_os.h"

/* allocate input buffer
 * Note: address alignment requirement not considered here
 **/
static int mpp_alloc_input_buf(_elem_t *elem)
{
    int i, ret = MPP_SUCCESS;
    int height;
   /* allocate input buffers */
    for(i = 0; i < elem->io.nb_in_buf; i++)
    {
        /* pointer sanity check */
        if ((elem->io.in_buf[i] == NULL) || (elem->io.in_buf[i]->hw == NULL))
        {
            ret = MPP_ERROR;
            break;
        }
        /* buffer already allocated? */
        if (elem->io.in_buf[i]->hw->heap_p != NULL)
            continue;   /* yes: move to next input */

        /*** buffer allocation ***/
        /* time to set stride */
        if (elem->io.in_buf[i]->hw->stride == 0)
            elem->io.in_buf[i]->hw->stride = elem->io.in_buf[i]->width * get_bitpp(elem->io.in_buf[i]->format) / 8;

        if (elem->io.in_buf[i]->stripe_num > 0)
            height = elem->io.in_buf[i]->height / MPP_STRIPE_NUM;
        else
            height = elem->io.in_buf[i]->height;

        elem->io.in_buf[i]->hw->heap_p = hal_malloc(height * elem->io.in_buf[i]->hw->stride + elem->io.in_buf[i]->hw->alignment);

        if (elem->io.in_buf[i]->hw->heap_p == NULL)
        {
            MPP_LOGE("Allocation failed\n");
            ret = MPP_MALLOC_ERROR;
            break;
        }

        /* get buffer aligned address */
        unsigned char *heap_p = elem->io.in_buf[i]->hw->heap_p;
        unsigned int alignment = (unsigned int)elem->io.in_buf[i]->hw->alignment;

        if (alignment)
            elem->io.in_buf[i]->hw->addr = (unsigned char *)(heap_p + alignment - ((unsigned int)heap_p % alignment));
        else    /* avoid modulo with 0 */
            elem->io.in_buf[i]->hw->addr = heap_p;

        /* buffer is cacheable */
        elem->io.in_buf[i]->hw->cacheable = true;
    }
    return ret;
}

/* browse the mpp backward to find a non-inplace element
 * return the memory policy of this element
 **/
mpp_memory_policy_t get_prev_policy(_elem_t *elem)
{
    _elem_t *prevelem = elem->prev;

    do {
        if (prevelem->io.inplace && (prevelem->io.mem_policy == HAL_MEM_ALLOC_NONE))
        {
            prevelem = prevelem->prev;
            continue;
        }
        else
            return prevelem->io.mem_policy;
    } while (prevelem != NULL);
    MPP_LOGE("\n\r Ill-formed pipeline: cannot get policy of previous element!");
    return HAL_MEM_ALLOC_NONE;
}

/* solve conflict between consumer and producer buffer requirement
 * note: those buffers are already allocated
 * in case of equal constraints, take producer's requirements
 **/
static int solve_buf_req(buf_desc_t *buf)
{
    bool is_prod = true;    /* by default take producer requirement */

    /* check stride compatibility */
    if ( (buf->hw_req_prod.stride > 0) && (buf->hw_req_cons.stride > 0)
            && (buf->hw_req_prod.stride != buf->hw_req_cons.stride))
    {
        MPP_LOGE("Impossible to solve conflict with different Buffer Stride.\n");
        return MPP_ERROR;
    }

    /* decision based on memory alignment */
    if (buf->hw_req_prod.alignment > 0)
    {
        if (buf->hw_req_cons.alignment <= buf->hw_req_prod.alignment)
            is_prod = true;
        else
            is_prod = false;
    }
    else
    {   /* no alignment for producer */
        if (buf->hw_req_cons.alignment > 0)
            is_prod = false;
        else
            is_prod = true; /* same: by default take producer reqt */
    }

    /* assign final requirement */
    if (is_prod)
        buf->hw = &buf->hw_req_prod;
    else
        buf->hw = &buf->hw_req_cons;
    return MPP_SUCCESS;
}

/* check buffer address and set buffer status */
int mpp_memory_check(_mpp_t *mpp)
{
    int i;
    int ret = MPP_SUCCESS;
    _elem_t *elem = mpp->first_elem;

    while ((elem != NULL) && (elem->mpp == mpp))
    {
        for(i = 0; i < elem->io.nb_in_buf; i++)
        {
            /* set input buffers status */
            elem->io.in_buf[i]->status = MPP_BUFFER_EMPTY;
            /* check address for input buffers */
            MPP_LOGI("Element %s: input buffer#%d address 0x%x \n", elem_name(elem->proc_typ), i, (unsigned int)elem->io.in_buf[i]->hw->addr);
        }

        for(i = 0; i < elem->io.nb_out_buf; i++)
        {
            /* set output buffers status */
            elem->io.out_buf[i]->status = MPP_BUFFER_EMPTY;
            /* check address for output buffers */
            MPP_LOGI("Element %s: output buffer#%d address 0x%x \n", elem_name(elem->proc_typ), i, (unsigned int)elem->io.out_buf[i]->hw->addr);
        }
        elem = elem->next[0];
    }
    return ret;
}

/* allocates missing buffers, assign buffers */
int mpp_memory_alloc(_mpp_t *mpp)
{
    int i;
    int ret = MPP_SUCCESS;
    _elem_t *elem = mpp->first_elem;

    /*** all elements: assign input buffers according policy of previous element ***/
    while ((elem != NULL) && (elem->mpp == mpp))
    {
        if (elem->io.nb_in_buf == 0)
        {   /* no input buffer */
            elem = elem->next[0];
            continue;
        }

        /* handle special case of in-place operations */
        if (elem->io.inplace && (elem->io.mem_policy == HAL_MEM_ALLOC_NONE))
        {
            /* element is transparent regarding memory management */
            elem = elem->next[0];
            continue;
        }

        /* get policy of previous element (without in-place processing) */
        mpp_memory_policy_t prev_policy = get_prev_policy(elem);

        /* assign buffers final reqts and allocate missing buffers */
        switch (elem->io.mem_policy)
        {
        case HAL_MEM_ALLOC_NONE:
        case HAL_MEM_ALLOC_OUTPUT:
            /* element does NOT provide the input buffer */
            /* check previous element policy */
            if ( (prev_policy == HAL_MEM_ALLOC_NONE)
                    ||  (prev_policy == HAL_MEM_ALLOC_INPUT) )
            {
                /* take producer reqt by default. */
                for(i = 0; i < elem->io.nb_in_buf; i++)
                {
                    if (elem->io.in_buf[i]->hw_req_prod.alignment >= elem->io.in_buf[i]->hw_req_cons.alignment) {
                        elem->io.in_buf[i]->hw = &elem->io.in_buf[i]->hw_req_prod;
                    } else {
                        elem->io.in_buf[i]->hw = &elem->io.in_buf[i]->hw_req_cons;
                    }
                }
                MPP_LOGD("Element %s: Allocating missing input buffer\n", elem_name(elem->proc_typ));
                /* input buffer missing: allocate it */
                ret = mpp_alloc_input_buf(elem);
                if (ret != MPP_SUCCESS)
                    return ret;
            }
            else if ( (prev_policy == HAL_MEM_ALLOC_BOTH)
                    || (prev_policy == HAL_MEM_ALLOC_OUTPUT) )
            {
                /* input buffer existing: reuse it */
                for(i = 0; i < elem->io.nb_in_buf; i++)
                {
                    elem->io.in_buf[i]->hw = &elem->io.in_buf[i]->hw_req_prod;
                }
                MPP_LOGD("Element %s: Reusing buffer from previous element\n", elem_name(elem->proc_typ));
            }
            break;
        case HAL_MEM_ALLOC_INPUT:
        case HAL_MEM_ALLOC_BOTH:
            /* element does provide the input buffer */
            /* check previous element policy */
            if ( (prev_policy == HAL_MEM_ALLOC_NONE)
                    ||  (prev_policy == HAL_MEM_ALLOC_INPUT) )
            {
                /* input buffer owned by element: use it */
                for(i = 0; i < elem->io.nb_in_buf; i++)
                {
                    elem->io.in_buf[i]->hw = &elem->io.in_buf[i]->hw_req_cons;
                }
                MPP_LOGD("Element %s: Using its own input buffer\n", elem_name(elem->proc_typ));

            } else if ( (prev_policy == HAL_MEM_ALLOC_BOTH)
                    || (prev_policy == HAL_MEM_ALLOC_OUTPUT) )
            {
                /* input buffer conflict: need to resolve it! */
                MPP_LOGD("Element %s: Buffer conflict between two elements!\n", elem_name(elem->proc_typ));
                for(i = 0; i < elem->io.nb_in_buf; i++)
                {
                    ret = solve_buf_req(elem->io.in_buf[i]);
                    if (ret != MPP_SUCCESS)
                        return ret;
                }
            }
            break;
        default:
            MPP_LOGE("Element %s: Unexpected memory policy\n", elem_name(elem->proc_typ));
            return MPP_ERROR;
            break;
        }

        elem = elem->next[0];
    }

    return ret;
}

/* free buffers allocated by pipeline */
void mpp_memory_free(_mpp_t *mpp)
{
    int i;
    _elem_t *elem = mpp->first_elem;

    while ((elem != NULL) && (elem->mpp == mpp))
    {
        if (elem->io.nb_in_buf == 0)
        {   /* no input buffer */
            elem = elem->next[0];
            continue;
        }

        /* handle special case of in-place operations */
        if (elem->io.inplace)
        {
            /* element is transparent regarding memory management */
            elem = elem->next[0];
            continue;
        }

        /* get policy of previous element (without in-place processing) */
        mpp_memory_policy_t prev_policy = get_prev_policy(elem);

        /* free only the input buffers allocated by us */
        if ( ( (elem->io.mem_policy == HAL_MEM_ALLOC_NONE) || (elem->io.mem_policy == HAL_MEM_ALLOC_OUTPUT) )
                && ((prev_policy == HAL_MEM_ALLOC_NONE) || (prev_policy == HAL_MEM_ALLOC_INPUT) ) )
        {
            for(i = 0; i < elem->io.nb_in_buf; i++)
            {
                if (elem->io.in_buf[i]->hw != NULL)
                {
                    hal_free(elem->io.in_buf[i]->hw->heap_p);
                    hal_free(elem->io.in_buf[i]->hw);
                }
            }
        }
    }

    return;
}
