/*
 * Copyright 2020-2022,2024 NXP.
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
#include "hal.h"
#include "mpp_debug.h"

static inline int display_enqueue(_mpp_t *mpp)
{
    int ret = MPP_SUCCESS;
    _elem_t *elem = mpp->last_elem;
    _display_dev_t *disp = elem->dev.disp;

    /* check buffer status */
    if (elem->io.in_buf[0]->status == MPP_BUFFER_WRITTING)
    {
        MPP_LOGI("Warning: display may show an uncompleted frame \n");
    }
    elem->io.in_buf[0]->status = MPP_BUFFER_READING;

    /* display current buffer */
    ret = disp->dev.ops->blit(&disp->dev, elem->io.in_buf[0]->hw->addr, elem->io.in_buf[0]->stripe_num);

    /* update buffer status */
    elem->io.in_buf[0]->status = MPP_BUFFER_EMPTY;
    return ret;
}

int mpp_display_add(mpp_t mpp, const char *name, mpp_display_params_t *params)
{
    int ret = MPP_SUCCESS;
    if (!mpp)
        return MPP_INVALID_PARAM;
    _mpp_t *_mpp = (_mpp_t *)mpp;
    if (_mpp->status != MPP_OPENED)
        return MPP_ERROR;

    _elem_t *elem;
    ret  = mpp_create_elem(_mpp, &elem);
    if (ret != MPP_SUCCESS)
        return ret;

    elem->type = MPP_TYPE_SINK;
    elem->sink_typ = MPP_SINK_DISPLAY;

    _display_dev_t *disp = hal_malloc(sizeof(*disp));
    if (!disp)
        return MPP_MALLOC_ERROR;

    memset(disp->name, 0, sizeof(disp->name));
    elem->dev.disp = disp;

    strncpy(disp->name, name, MAX_DEV_NAME);

    /* fill in display parameters */
    memcpy(&disp->params, params, sizeof(*params));

    /* register/initialize display with the framework */
    ret = hal_display_setup(name, &disp->dev);
    if (ret != MPP_SUCCESS)
        return ret;

    /* HAL init function */
    ret = disp->dev.ops->init(&disp->dev, &disp->params, _mpp->params.evt_callback_f, _mpp->params.cb_userdata);
    if (ret != MPP_SUCCESS)
        return ret;

    /* set operating mode */
    elem->io.inplace = false;
    elem->io.nb_in_buf = 1;
    elem->io.nb_out_buf = 0;

    /* get input parameters from previous element */
    buf_desc_t *prev_buf = elem->prev->io.out_buf[0];

    /* check previous element buff descriptor parameters compatibility with display parameters */
    if ((prev_buf->height > disp->dev.cap.height) || (prev_buf->width > disp->dev.cap.width)) {
        MPP_LOGE("Previous element resolution parameters:(width =%d, height =%d) exceeds display's resolution:(width =%d, height =%d) \n",
            prev_buf->width, prev_buf->height, disp->dev.cap.width, disp->dev.cap.height);
        return MPP_ERROR;
    } else {
        /* get buffer descriptor from previous element */
        elem->io.in_buf[0] = prev_buf;
    }

    /* get buffer requirements from HAL */
    disp->dev.ops->get_buf_desc(&disp->dev, &elem->io.in_buf[0]->hw_req_cons, &elem->io.mem_policy);

    /* check if the display requires a specific number of lines or if its
     * configurable (can keep and use previous element resolution) */
    if (elem->io.in_buf[0]->hw_req_cons.nb_lines > 0) {
        /* display should use its proper resolution parameters */
        elem->io.in_buf[0]->width = disp->params.width;
        elem->io.in_buf[0]->height = disp->params.height;
    }

    elem->sink_enqueue = display_enqueue;

    /* pipeline has been closed */
    _mpp->status = MPP_CLOSED;

    return ret;
}
