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
#include "hal_utils.h"
#include "mpp_debug.h"

#include "hal.h"

static inline int camera_dequeue(_mpp_t *mpp)
{
    int ret = MPP_SUCCESS;
    _elem_t *elem = mpp->first_elem;
    _camera_dev_t *cam = elem->dev.cam;

    /* check buffer status */
    if (elem->io.out_buf[0]->status == MPP_BUFFER_READING)
    {
        MPP_LOGI("Warning: camera may overwrite buffer in use.\n");
    }
    elem->io.out_buf[0]->status = MPP_BUFFER_WRITTING;

    ret = cam->dev.ops->dequeue(&cam->dev, (void **)(&elem->io.out_buf[0]->hw->addr), &elem->io.out_buf[0]->stripe_num);

    /* update buffer status */
    elem->io.out_buf[0]->status = MPP_BUFFER_READY;
    elem->io.out_buf[0]->frame_id++;

    return ret;
}

int mpp_camera_add(mpp_t mpp, const char* name, mpp_camera_params_t *params)
{
    int ret = MPP_SUCCESS;
    if (!mpp)
        return MPP_INVALID_PARAM;
    _mpp_t *_mpp = (_mpp_t *)mpp;
    if (_mpp->status != MPP_CREATED)
        return MPP_ERROR;

    _elem_t *elem;
    ret  = mpp_create_elem(_mpp, &elem);
    if (ret != MPP_SUCCESS)
        return ret;

    elem->type = MPP_TYPE_SOURCE;
    elem->sink_typ = MPP_SRC_CAMERA;

    _camera_dev_t *cam = hal_malloc(sizeof(*cam) + CAMERA_MAX_PRIV_SIZE);
    if (!cam)
        return MPP_MALLOC_ERROR;
    memset(cam, 0, sizeof(_camera_dev_t));
    elem->dev.cam = cam;

    /* copy params */
    memcpy(&cam->params, params, sizeof(*params));

    /* setup HAL camera structure */
    ret = hal_camera_setup(name, &cam->dev);
    if (ret != MPP_SUCCESS)
        return ret;

    /* init HAL function */
    ret = cam->dev.ops->init(&cam->dev, &cam->params, NULL, NULL);
    if (ret != MPP_SUCCESS)
        return ret;

    memset(cam->name, 0, sizeof(cam->name));
    strncpy(cam->name, name, MAX_DEV_NAME);

    MPP_LOGD("Adding camera %s\n", cam->name);

    /* set dequeue function */
    elem->src_dequeue = camera_dequeue;

    /* create buffer parameters to be passed to next element */
    elem->io.inplace = false;
    elem->io.nb_in_buf = 0;
    elem->io.out_buf[0] = hal_malloc(sizeof(buf_desc_t));
    if (elem->io.out_buf[0] == NULL)
    {
        MPP_LOGE("\nAllocation failed\n");
        return MPP_MALLOC_ERROR;
    }
    elem->io.nb_out_buf = 1;
    elem->io.out_buf[0]->format = cam->params.format;
    elem->io.out_buf[0]->width = cam->params.width;
    elem->io.out_buf[0]->height = cam->params.height;
    /* init stripes */
    if (cam->params.stripe)
        elem->io.out_buf[0]->stripe_num = 1;
    else
        elem->io.out_buf[0]->stripe_num = 0;

    cam->dev.ops->get_buf_desc(&cam->dev, &elem->io.out_buf[0]->hw_req_prod, &elem->io.mem_policy);

    /* pipeline has been opened */
    _mpp->status = MPP_OPENED;

    return ret;
}
