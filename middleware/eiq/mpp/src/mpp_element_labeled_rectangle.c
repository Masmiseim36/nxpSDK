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

#include "mpp_api.h"
#include "mpp_api_types_internal.h"
#include "mpp_heap.h"
#include "hal_os.h"
#include "string.h"

#include "mpp_debug.h"
#include "hal.h"

#include <stdbool.h>

static hal_mutex_t mutex;
static mpp_labeled_rect_t *labeled_rectangles = NULL;
static mpp_labeled_rect_t *labeled_rectangles_new = NULL;

/* element processing function */
static int label_rectangle_func (_elem_t *elem)
{
    int ret = MPP_SUCCESS;
    mpp_labeled_rect_t *lr = labeled_rectangles;
    mpp_labeled_rect_t *new_lr = labeled_rectangles_new;

    do {
        /** swap new rectangles */
        ret = hal_mutex_lock (mutex);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("%s: mutex failed %d\n", __func__, ret);
            break;
        }
        /* check if an update occured */
        if (new_lr[0].tag == MPP_LBL_RECT_TAG) {
            /* new rectangles detected */
            memcpy(labeled_rectangles, labeled_rectangles_new,
                   sizeof(mpp_labeled_rect_t) * elem->params.labels.detected_count);
            /* reset the first tag to avoid copy the same data for optimisation */
            new_lr[0].tag = 0;
        }
        ret = hal_mutex_unlock (mutex);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("%s: mutex failed %d\n", __func__, ret);
            break;
        }
        /* start drawing */
        for (int idx = 0; idx < elem->params.labels.detected_count; idx++) {
            if (lr == NULL) {
                MPP_LOGE ("labeled rectangles not initialized: 0x%x,\n",
                          (unsigned int)lr);
                ret = MPP_INVALID_PARAM;
                break;
            }
            if (lr[idx].tag != MPP_LBL_RECT_TAG) {
                MPP_LOGE ("mpp_labeled_rectangle element invalid tag: 0x%x, @%p\n",
                          lr[idx].tag, &lr[idx].tag);
                ret = MPP_INVALID_ELEM;
                break;
            }
            /* if clear is set then rectangle should not be drawn */
            if (lr[idx].clear == 0UL) {
                ret = hal_label_rectangle (
                        elem->io.in_buf[0]->hw->addr,
                        elem->io.in_buf[0]->width,
                        elem->io.in_buf[0]->height,
                        elem->io.in_buf[0]->format,
                        &lr[idx],
                        elem->io.in_buf[0]->stripe_num,
                        MPP_STRIPE_NUM);
                if (ret != MPP_SUCCESS) {
                    MPP_LOGE ("mpp_labeled_rectangle element num %x failed !\n", idx);
                    break;
                }
            }
        }
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("%s: return error %d\n", __func__, ret);
            break;
        }
    } while (false);

    return ret;
}

/* labeled rectangle setup function */
unsigned int elem_lbl_rct_setup (_elem_t *elem)
{
    volatile uint32_t ret = MPP_ERROR;

    do {
        /* sanity checks */
        if (elem == NULL) {
            ret = MPP_INVALID_PARAM;
            MPP_LOGE ("invalid input buffer - elem (0x%x)\n",
                      (unsigned int)ret);
            break;
        }
        if ((elem->type != MPP_TYPE_PROC) || (elem->proc_typ != MPP_ELEMENT_LABELED_RECTANGLE))
        {
            ret = MPP_INVALID_ELEM;
            MPP_LOGE ("invalid element %s (expected element LABELED_RECTANGLE)\n", elem_name(elem->proc_typ));
            break;
        }
        /* get input parameters */
        _mpp_t *mpp = elem->mpp;

        if (!mpp) {
            ret = MPP_INVALID_PARAM;
            MPP_LOGE ("mpp is null\n");
            break;
        }

        /* set operating mode */
        elem->io.inplace = true;
        /* input buffer points to previous element buffer */
        elem->io.nb_in_buf = 1;
        elem->io.in_buf[0] = elem->prev->io.out_buf[0];
        /* create output buffer parameters to be passed to next element */
        elem->io.nb_out_buf = 1;
        /* element process in-place: means input & output point to same buffer */
        elem->io.out_buf[0] = elem->io.in_buf[0];

        if (elem->params.labels.detected_count > elem->params.labels.max_count) {
            ret = MPP_INVALID_PARAM;
            MPP_LOGE ("invalid elem params - detected count is bigger than max count %d\n",
                      (int)elem->params.labels.detected_count);
            break;
        }
        /* allocate global buffer */
        labeled_rectangles = hal_malloc(sizeof(mpp_labeled_rect_t) * elem->params.labels.max_count);
        labeled_rectangles_new = hal_malloc(sizeof(mpp_labeled_rect_t) * elem->params.labels.max_count);
        if (labeled_rectangles == NULL || labeled_rectangles_new == NULL) {
            ret = MPP_MALLOC_ERROR;
            MPP_LOGE ("ERR: malloc failed for labeled_rectangles\n");
            break;
        }
        /* copy added rectangles */
        memcpy(labeled_rectangles, elem->params.labels.rectangles,
                sizeof(mpp_labeled_rect_t) * elem->params.labels.detected_count);
        elem->params.labels.rectangles = labeled_rectangles;
        /* update tags */
        for (int idx=0; idx < elem->params.labels.detected_count; idx++) {
            labeled_rectangles[idx].tag = MPP_LBL_RECT_TAG;
        }

        /* assign element entry/function */
        elem->entry = label_rectangle_func;

        /* create mutex to protect param buffer during update and draw */
        ret = hal_mutex_create(&mutex);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("%s: mutex failed %d\n", __func__, (int)ret);
        }

    } while (false);

    if (ret != MPP_SUCCESS) {
        if (labeled_rectangles != NULL) {
            hal_free(labeled_rectangles);
        }
        if (labeled_rectangles_new != NULL) {
            hal_free(labeled_rectangles_new);
        }
    }

    return ret;
}

/* labeled rectangle update function */
uint32_t mpp_lbl_rectangle_update (_elem_t *elem, mpp_element_params_t *params)
{
    volatile uint32_t ret = MPP_ERROR;

    do {
        /* sanity checks */
        if (elem == NULL) {
            ret = MPP_INVALID_PARAM;
            MPP_LOGE("ERR: invalid input buffer - elem (0x%x)\n",
                      (unsigned int)ret);
            break;
        }
        if (elem->proc_typ != MPP_ELEMENT_LABELED_RECTANGLE)
        {
            ret = MPP_INVALID_ELEM;
            MPP_LOGE("ERR: invalid element id (0x%x)\n", elem->proc_typ);
            break;
        }
        if (elem->params.labels.max_count < params->labels.max_count)
        {
            ret = MPP_INVALID_PARAM;
            MPP_LOGE ("ERR: invalid elem params - max count is bigger than setup %d < %d\n",
                      (int)elem->params.labels.max_count,
                      (int)params->labels.max_count);
            break;
        }
        if (params->labels.detected_count > params->labels.max_count)
        {
            ret = MPP_INVALID_PARAM;
            MPP_LOGE ("ERR: invalid elem params - detected count is bigger than max count %d < %d\n",
                      (int)params->labels.detected_count,
                      (int)params->labels.max_count);
            break;
        }
        /* update rectangles count */
        elem->params.labels.detected_count = params->labels.detected_count;

        /* copy added rectangles */
        ret = hal_mutex_lock (mutex);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("%s: mutex failed %d\n", __func__, (int)ret);
            break;
        }
        memcpy(labeled_rectangles_new, params->labels.rectangles,
                sizeof(mpp_labeled_rect_t) * params->labels.detected_count);
        /* update tags */
        for (int idx=0; idx < elem->params.labels.detected_count; idx++) {
            labeled_rectangles_new[idx].tag = MPP_LBL_RECT_TAG;
        }
        ret = hal_mutex_unlock (mutex);
        if (ret != MPP_SUCCESS) {
            MPP_LOGE("%s: mutex failed %d\n", __func__, (int)ret);
            break;
        }

    } while (false);

    return ret;
}
