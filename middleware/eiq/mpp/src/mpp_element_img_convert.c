/*
 * Copyright 2020-2024 NXP.
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
#include "mpp_heap.h"
#include "hal_os.h"
#include "string.h"

#include "mpp_debug.h"
#include "hal_graphics_dev.h"
#include "hal.h"

/* element processing function */
static int convert_func(_elem_t *elem)
{
    int ret;
    gfx_dev_t *gfx = elem->dev.gfx;
    buf_desc_t *obuf = elem->io.out_buf[0];
    buf_desc_t *ibuf = elem->io.in_buf[0];

    /* The value of following parameters are only set after
     * the pipeline graph has been constructed.
     * Hence those parameters can't be set from the setup function.
     */
    /* set source buffer */
    gfx->src.buf = ibuf->hw->addr;
    gfx->src.pitch = ibuf->hw->stride;

    /* in stripe mode: */
    /* update crop source, destination window & address */
    if (ibuf->stripe_num > 0)
    {
        int stripe_in_height = gfx->src.height;   /* input stripe height */
        int stripe_out_height = gfx->dst.height;   /* output stripe height */
        int stripe_in_start = (ibuf->stripe_num - 1) * stripe_in_height;    /* input stripe y-position */
        int stripe_out_start = (ibuf->stripe_num - 1) * stripe_out_height;  /* output stripe y-position */

        /* set input 'top' */
        if ((elem->params.convert.crop.top > stripe_in_start) && (elem->params.convert.crop.top < stripe_in_start + stripe_in_height))
        {   /* set top in stripe coords */
            gfx->src.top = elem->params.convert.crop.top - stripe_in_start;
        }
        else if (elem->params.convert.crop.top > stripe_in_start + stripe_in_height)
        {   /* nothing to do, skip this stripe */
            return MPP_SUCCESS;
        }
        else
        {   /* ignore top for now */
            gfx->src.top = 0;
        }

        /* set input 'bottom' */
        if ((elem->params.convert.crop.bottom > stripe_in_start) && (elem->params.convert.crop.bottom < stripe_in_start + stripe_in_height))
        {   /* set top in stripe coords */
            gfx->src.bottom = elem->params.convert.crop.bottom - stripe_in_start;
        }
        else if (elem->params.convert.crop.bottom < stripe_in_start)
        {   /* nothing to do, skip this stripe */
            return MPP_SUCCESS;
        }
        else
        {   /* ignore top for now */
            gfx->src.bottom = stripe_in_height - 1;
        }

        /* set output 'top' */
        if ((elem->params.convert.out_window.top > stripe_out_start) && (elem->params.convert.out_window.top < stripe_out_start + stripe_out_height))
        {   /* set top in stripe coords */
            gfx->dst.top = elem->params.convert.out_window.top - stripe_out_start;
        }
        else if (elem->params.convert.out_window.top > stripe_out_start + stripe_out_height)
        {   /* nothing to do, skip this stripe */
            return MPP_SUCCESS;
        }
        else
        {   /* ignore top for now */
            gfx->dst.top = 0;
        }

        /* set output 'bottom' */
        int outbot = elem->params.convert.out_window.top + elem->params.convert.scale.height - 1;
        if ((outbot > stripe_out_start) && (outbot < stripe_out_start + stripe_out_height))
        {   /* set top in stripe coords */
            gfx->dst.bottom = outbot - stripe_out_start;
        }
        else if (outbot < stripe_out_start)
        {   /* nothing to do, skip this stripe */
            return MPP_SUCCESS;
        }
        else
        {   /* ignore top for now */
            gfx->dst.bottom = stripe_out_height - 1;
        }

        /* update destination address */
        if (!elem->params.convert.stripe_out)
        {
            /* blit from stripe src to full frame destination */
            /* need to point to stripe in destination buffer */
            int offset = (ibuf->stripe_num - 1) * obuf->hw->stride * stripe_out_height;
            gfx->dst.buf = obuf->hw->addr + offset;
        } else {
            /* blit src stripe to dest stripe */
            gfx->dst.buf = obuf->hw->addr;
            /* transmit stripe number for the destination buffer */
            obuf->stripe_num = ibuf->stripe_num;
        }
    } else {
        /* full frame mode */
        gfx->dst.buf = obuf->hw->addr;
    }
    gfx->dst.pitch = obuf->hw->stride;

    gfx_rotate_config_t rot = { .degree = elem->params.convert.angle, .target = kGFXRotate_DSTSurface};

    ret = gfx->ops->blit(gfx, &gfx->src, &gfx->dst, &rot, elem->params.convert.flip);
    return ret;
}

/* check parameters and complete missing fields */
static int check_convert_params(_elem_t *elem)
{
    int ret = MPP_SUCCESS;
    unsigned int input_width = 0, input_height = 0;

    /* get input parameters from previous element */
    buf_desc_t *prev_buf = elem->prev->io.out_buf[0];

    /* check output buffer dimensions */
    if ((elem->params.convert.out_buf.width == 0) ||
            (elem->params.convert.out_buf.height == 0))
    {
        MPP_LOGE("Invalid output buffer parameter\n");
        return MPP_INVALID_PARAM;
    }

    if(!(elem->params.convert.ops & MPP_CONVERT_CROP))
    {
        /* keep full image area */
        elem->params.convert.crop.top = 0;
        elem->params.convert.crop.left = 0;
        elem->params.convert.crop.bottom = prev_buf->height - 1;
        elem->params.convert.crop.right = prev_buf->width - 1;
    }

    /* check crop params */
    if ( (elem->params.convert.crop.top > elem->params.convert.crop.bottom)
            || (elem->params.convert.crop.left > elem->params.convert.crop.right)
            || (elem->params.convert.crop.right > prev_buf->width)
            || (elem->params.convert.crop.bottom > prev_buf->height) )
    {
        MPP_LOGE("Invalid crop parameter\n\r");
        MPP_LOGE("top = %d, left = %d, right = %d, bott = %d\n\r",
                elem->params.convert.crop.top, elem->params.convert.crop.left,
                elem->params.convert.crop.right, elem->params.convert.crop.bottom);
        return MPP_INVALID_PARAM;
    }

    /* compute the input area dimensions */
    input_width = elem->params.convert.crop.right - elem->params.convert.crop.left + 1;
    input_height = elem->params.convert.crop.bottom - elem->params.convert.crop.top + 1;

    /* complete convert params for hal */
    if(!(elem->params.convert.ops & MPP_CONVERT_COLOR))
    {   /* keep source color format */
        elem->params.convert.pixel_format = prev_buf->format;
    }

    /* In case of output window, top and left coordinates from struct out_window
     * should be defined by user.
     * Otherwise, it will be set to:
     * - top/left coordinates of the output buffer
     */
    if(!(elem->params.convert.ops & MPP_CONVERT_OUT_WINDOW))
    {   /* keep source position */
        elem->params.convert.out_window.top = 0;
        elem->params.convert.out_window.left = 0;
    }

    /* In case of scaling, dimensions from struct scale
     * should be defined by user.
     * Otherwise, it will be set to:
     * - dims of input window
     */
    if(!(elem->params.convert.ops & MPP_CONVERT_SCALE))
    {   /* keep source dimensions */
        if(elem->params.convert.ops & MPP_CONVERT_ROTATE)
        {   /* rotate swaps dimensions */
            switch(elem->params.convert.angle)
            {
            case ROTATE_90:
            case ROTATE_270:
                elem->params.convert.scale.width = input_height;
                elem->params.convert.scale.height = input_width;
                break;
            case ROTATE_0:
            case ROTATE_180:
            default:
                elem->params.convert.scale.width = input_width;
                elem->params.convert.scale.height = input_height;
                break;
            }
        } else {
            elem->params.convert.scale.width = input_width;
            elem->params.convert.scale.height = input_height;
        }
    }

    if(!(elem->params.convert.ops & MPP_CONVERT_ROTATE))
    {   /* keep source orientation */
        elem->params.convert.angle = ROTATE_0;
        elem->params.convert.flip = FLIP_NONE;
    }

    /* check output window params */
    if ((elem->params.convert.out_window.top + elem->params.convert.scale.height > elem->params.convert.out_buf.height)
            || (elem->params.convert.out_window.left + elem->params.convert.scale.width > elem->params.convert.out_buf.width))
    {
        MPP_LOGE("Invalid output window parameter\n");
        return MPP_INVALID_PARAM;
    }
    return ret;
}

/* fill-in the gfx source and destination structures */
static void set_gfx_dev(gfx_dev_t *gfx, _elem_t * elem)
{
    /* set source buffer */
    gfx->src.format = elem->io.in_buf[0]->format;
    gfx->src.width = elem->io.in_buf[0]->width;
    if (elem->io.in_buf[0]->stripe_num > 0)
        gfx->src.height = elem->io.in_buf[0]->height / MPP_STRIPE_NUM;
    else
        gfx->src.height = elem->io.in_buf[0]->height;
    gfx->src.swapByte = 0;

    /* set destination buffer */
    gfx->dst.format = elem->params.convert.pixel_format;
    gfx->dst.width = elem->params.convert.out_buf.width;
    if (elem->io.in_buf[0]->stripe_num > 0)
    {
        /* even if output is full frame, deal with stripe */
        float v_scaling = (float) elem->params.convert.scale.height / elem->io.in_buf[0]->height;
        gfx->dst.height = gfx->src.height * v_scaling;  /* output stripe height is input x scale */
    }
    else
        gfx->dst.height = elem->params.convert.out_buf.height;
    gfx->dst.swapByte = 0;

    /* crop source window */
    gfx->src.top = elem->params.convert.crop.top;
    gfx->src.bottom = elem->params.convert.crop.bottom;
    gfx->src.left = elem->params.convert.crop.left;
    gfx->src.right = elem->params.convert.crop.right;

    /* output destination window */
    gfx->dst.left = elem->params.convert.out_window.left;
    gfx->dst.top = elem->params.convert.out_window.top;
    gfx->dst.right = elem->params.convert.out_window.left + elem->params.convert.scale.width - 1;
    gfx->dst.bottom = elem->params.convert.out_window.top + elem->params.convert.scale.height - 1;

    return;
}

/* function to setup/configure input/output memory  */
int setup_convert_mem(uint8_t **io_bufs[], unsigned int io_buf_sz[]);

/* element setup function */
unsigned int elem_convert_setup(_elem_t *elem)
{
    int ret = MPP_SUCCESS;
    gfx_dev_t *gfx = NULL;

    do {
        /* sanity checks */
        if (elem == NULL) {
            MPP_LOGE("invalid input buffer - elem (0x%x)\n", ret);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if ((elem->type != MPP_TYPE_PROC) || (elem->proc_typ != MPP_ELEMENT_CONVERT))
        {
            MPP_LOGE("invalid element %s (expected element CONVERT)\n", elem_name(elem->proc_typ));
            ret = MPP_INVALID_PARAM;
            break;
        }
        /* setup the device */
        gfx = hal_malloc(sizeof(gfx_dev_t));
        if (!gfx)
        {
            MPP_LOGE("\nAllocation failed\n");
            ret = MPP_MALLOC_ERROR;
            break;
        }
        memset(gfx, 0, sizeof(gfx_dev_t));
        elem->dev.gfx = gfx;

        _mpp_t *mpp = elem->mpp;
        if (!mpp)
        {
            ret = MPP_INVALID_PARAM;
            break;
        }

        ret = check_convert_params(elem);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("invalid parameters for Image Convert\r\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* set operating mode */
        elem->io.inplace = false;
        /* input buffer points to previous element buffer */
        elem->io.nb_in_buf = 1;
        elem->io.in_buf[0] = elem->prev->io.out_buf[0];
        /* create output buffer parameters to be passed to next element */
        elem->io.nb_out_buf = 1;
        elem->io.out_buf[0] = hal_malloc(sizeof(buf_desc_t));
        if (elem->io.out_buf[0] == NULL)
        {
            MPP_LOGE("\nAllocation failed\n");
            ret = MPP_MALLOC_ERROR;
            break;
        }
        /* set buffer descriptor */
        memset(elem->io.out_buf[0], 0, sizeof(buf_desc_t));
        elem->io.out_buf[0]->format = elem->params.convert.pixel_format;
        elem->io.out_buf[0]->width = elem->params.convert.out_buf.width;
        elem->io.out_buf[0]->height = elem->params.convert.out_buf.height;

        /* init stripes */
        if (elem->params.convert.stripe_out)
            elem->io.out_buf[0]->stripe_num = 1;
        else
            elem->io.out_buf[0]->stripe_num = 0;

        if (   ((elem->io.in_buf[0]->stripe_num > 0) && !elem->params.convert.stripe_in)
            || ((elem->io.in_buf[0]->stripe_num == 0) && elem->params.convert.stripe_in) )
        {
            MPP_LOGE("\nStripe mode not consistent between Convert element and previous element.\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        if (((elem->io.in_buf[0]->stripe_num > 0) || (elem->io.out_buf[0]->stripe_num > 0))
                && elem->params.convert.angle != ROTATE_0)
        {
            MPP_LOGE("\nStripe mode not supported with rotation.\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        if (elem->params.convert.stripe_in && (elem->io.in_buf[0]->stripe_num == 0))
        {
            MPP_LOGE("\nStripe mode not enabled in previous element.\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        gfx->callback  = mpp->params.evt_callback_f;
        gfx->user_data = mpp->params.cb_userdata;

        /* setup HAL gfx structure */
        ret  = hal_gfx_setup(elem->params.convert.dev_name, gfx);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE ("Setup HAL graphics fails with ret=%d\n", ret);
            break;
        }

        /* init HAL function */
        if (gfx->ops->init != NULL)
            gfx->ops->init(gfx, &elem->params);

        /* set source and destination in gfx */
        set_gfx_dev(gfx, elem);

        /* retrieve buffer requirements from HAL */
        ret = gfx->ops->get_buf_desc(gfx, &elem->io.in_buf[0]->hw_req_cons, &elem->io.out_buf[0]->hw_req_prod, &elem->io.mem_policy);
        if (ret != MPP_SUCCESS) break;

        /* assign element entry/function */
        elem->entry = convert_func;

    } while (false);

    if (ret != MPP_SUCCESS)
    {
        if (elem->io.out_buf[0] != NULL)
            hal_free(elem->io.out_buf[0]);
        if (gfx != NULL)
            hal_free(gfx);
    }

    return ret;
}

unsigned int mpp_convert_update(_elem_t *elem, mpp_element_params_t *params)
{
    int ret = MPP_SUCCESS;
    gfx_dev_t *gfx = NULL;

    do {
        /* sanity checks */
        if (params == NULL) {
            MPP_LOGE("invalid input buffer - params (0x%x)\n", params);
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* check forbidden parameter changes */
        if (params->convert.pixel_format != elem->params.convert.pixel_format)
        {
            MPP_LOGE("Cannot update pixel format\n");
            ret = MPP_INVALID_PARAM;
            break;
        }
        if (params->convert.stripe_in != elem->params.convert.stripe_in)
        {
            MPP_LOGE("Cannot update input stripe format\n");
            ret = MPP_INVALID_PARAM;
            break;
        }
        if (params->convert.stripe_out != elem->params.convert.stripe_out)
        {
            MPP_LOGE("Cannot update output stripe format\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* save new parameters in element context */
        memcpy(&elem->params, params, sizeof(mpp_element_params_t));

        ret = check_convert_params(elem);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("invalid parameters for Image Convert\r\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* do not change resolution in buffer descriptor */

        if (((elem->io.in_buf[0]->stripe_num > 0) || (elem->io.out_buf[0]->stripe_num > 0))
                && elem->params.convert.angle != ROTATE_0)
        {
            MPP_LOGE("\nStripe mode not supported with rotation.\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* update gfx source and destination */
        gfx = elem->dev.gfx;
        set_gfx_dev(gfx, elem);

    } while (false);

    return ret;
}
