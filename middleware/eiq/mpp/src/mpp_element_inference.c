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

#include <string.h>
#include "mpp_api.h"
#include "mpp_api_types_internal.h"

#include "mpp_debug.h"
#include "hal.h"
#include "hal_os.h"

/* element processing function */
static int inference_func(_elem_t *elem)
{
    int ret;

    /* data seems not to be used */
    ret = elem->dev.valgo->ops->run(elem->dev.valgo, NULL);
    return ret;
}

/* inference setup function */
unsigned int elem_inference_setup(_elem_t *elem)
{
    unsigned int ret = MPP_SUCCESS;
    vision_algo_dev_t *valgo = NULL;

    do {
        /* sanity checks */
        if (elem == NULL) {
            MPP_LOGE("invalid input buffer - elem (0x%x)\n", ret);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if ((elem->proc_typ != MPP_ELEMENT_INFERENCE) || (elem->type != MPP_TYPE_PROC))
        {
            MPP_LOGE("invalid element %s (expected element INFERENCE)\n", elem_name(elem->proc_typ));
            ret = MPP_INVALID_PARAM;
            break;
        }
        /* Check the number of inputs and outputs */
        if (elem->params.ml_inference.inference_params.num_outputs > MPP_INFERENCE_MAX_OUTPUTS )
        {
            MPP_LOGE("Inference does not support more than %d outputs\n", MPP_INFERENCE_MAX_OUTPUTS);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if (elem->params.ml_inference.inference_params.num_inputs > MPP_INFERENCE_MAX_INPUTS )
        {
            MPP_LOGE("Inference does not support more than %d inputs\n", MPP_INFERENCE_MAX_INPUTS);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if ((elem->params.ml_inference.inference_params.num_inputs == 0)
                || (elem->params.ml_inference.inference_params.num_outputs == 0))
        {
            MPP_LOGE("Inference requires at least one input and one output tensor\n");
            ret = MPP_INVALID_PARAM;
            break;
        }
        if (elem->params.ml_inference.tensor_order == MPP_TENSOR_ORDER_UNKNOWN)
        {
            MPP_LOGE("Input Tensor dimensions order not set\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        _mpp_t *mpp = elem->mpp;
        if (!mpp) {
            MPP_LOGE("mpp is null\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* get input parameters from previous element */
        buf_desc_t *prev_buf = elem->prev->io.out_buf[0];

        /* assign element entry/function */
        elem->entry = inference_func;

        /* setup the vision device */
        valgo = hal_malloc(sizeof(vision_algo_dev_t));
        if (!valgo) {
            MPP_LOGE ("malloc failed for vision device\n");
            ret = MPP_MALLOC_ERROR;
            break;
        }
        memset(valgo, 0, sizeof(vision_algo_dev_t));
        elem->dev.valgo = valgo;

#ifndef EMULATOR
        mpp_inference_type_t inference_type = elem->params.ml_inference.type;
        switch (inference_type)
        {
        case MPP_INFERENCE_TYPE_TFLITE:
            ret = hal_inference_tflite_setup(valgo);
            break;
        default:
            MPP_LOGE ("ML inference type %d is not supported\n", inference_type);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if (ret != MPP_SUCCESS) {
            MPP_LOGE ("Setup HAL inference fails with ret=%d\n", ret);
            break;
        }
#endif

        model_param_t params;
        memset(&params, 0, sizeof(model_param_t));
        params.model_data = elem->params.ml_inference.model_data;
        params.model_size = elem->params.ml_inference.model_size;
        params.model_input_mean = elem->params.ml_inference.model_input_mean;
        params.model_input_std = elem->params.ml_inference.model_input_std;
        params.evt_callback_f = mpp->params.evt_callback_f;
        params.cb_userdata = mpp->params.cb_userdata;
        params.tensor_order = elem->params.ml_inference.tensor_order;
        memcpy(&params.inference_params,
                        &elem->params.ml_inference.inference_params,sizeof(mpp_inference_params_t));

		/*Get resolution parameters from previous element*/
        params.format = prev_buf->format;
        params.height = prev_buf->height;
        params.width = prev_buf->width;

        /* init HAL function */
        hal_valgo_status_t algoret = kStatus_HAL_ValgoSuccess;
        algoret = valgo->ops->init(valgo, &params);
        if (algoret != kStatus_HAL_ValgoSuccess)
        {
            MPP_LOGE ("HAL inference init() fails with ret=%d\n", algoret);
            ret = MPP_ERROR;
        }

        /* set operating mode */
        elem->io.inplace = true;
        /* get input buffer from previous element */
        elem->io.nb_in_buf = 1;
        elem->io.in_buf[0] = prev_buf;
        /* create output buffer parameters to be passed to next element */
        elem->io.nb_out_buf = 1;
        elem->io.out_buf[0] = hal_malloc(sizeof(buf_desc_t));
        if (elem->io.out_buf[0] == NULL)
        {
            MPP_LOGE("Allocation failed\n");
            ret = MPP_MALLOC_ERROR;
            break;
        }
        /* set buffer descriptor */
        memset(elem->io.out_buf[0], 0, sizeof(buf_desc_t));
        elem->io.out_buf[0]->format = elem->io.in_buf[0]->format;
        elem->io.out_buf[0]->width = elem->io.in_buf[0]->width;
        elem->io.out_buf[0]->height = elem->io.in_buf[0]->height;

        /* retrieve buffer requirements from HAL */
        valgo->ops->get_buf_desc(valgo, &elem->io.in_buf[0]->hw_req_cons,
                                 &elem->io.mem_policy);
        /* copy consumer requirement into producer requirement */
        memcpy(&elem->io.out_buf[0]->hw_req_prod, &elem->io.in_buf[0]->hw_req_cons,
               sizeof(hw_buf_desc_t));

    } while (false);

    if (ret != MPP_SUCCESS) {
        if ( (elem != NULL) && (elem->io.out_buf[0] != NULL) )
            hal_free(elem->io.out_buf[0]);
        if (valgo != NULL)
            hal_free(valgo);
    }

    return ret;
}

uint32_t mpp_inference_update(_elem_t *elem, mpp_element_params_t *params)
{
    unsigned int ret = MPP_SUCCESS;
    vision_algo_dev_t *valgo = NULL;

    do {
        /* sanity checks */
        if (elem == NULL) {
            MPP_LOGE("invalid input buffer - elem (0x%x)\n", ret);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if ((elem->proc_typ != MPP_ELEMENT_INFERENCE) || (elem->type != MPP_TYPE_PROC))
        {
            MPP_LOGE("invalid element %s (expected element INFERENCE)\n", elem_name(elem->proc_typ));
            ret = MPP_INVALID_PARAM;
            break;
        }
        /* check mpp state */
        if (elem->mpp->oper_status != MPP_STOPPED)
        {
            MPP_LOGE("MPP branch must be stopped to update element INFERENCE\r\n");
            ret = MPP_INVALID_PARAM;
            break;
        }
        /* Check the number of inputs and outputs */
        if (params->ml_inference.inference_params.num_outputs > MPP_INFERENCE_MAX_OUTPUTS )
        {
            MPP_LOGE("Inference does not support more than %d outputs\n", MPP_INFERENCE_MAX_OUTPUTS);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if (params->ml_inference.inference_params.num_inputs > MPP_INFERENCE_MAX_INPUTS )
        {
            MPP_LOGE("Inference does not support more than %d inputs\n", MPP_INFERENCE_MAX_INPUTS);
            ret = MPP_INVALID_PARAM;
            break;
        }
        if ((params->ml_inference.inference_params.num_inputs == 0)
                || (params->ml_inference.inference_params.num_outputs == 0))
        {
            MPP_LOGE("Inference requires at least one input and one output tensor\n");
            ret = MPP_INVALID_PARAM;
            break;
        }
        if (params->ml_inference.tensor_order == MPP_TENSOR_ORDER_UNKNOWN)
        {
            MPP_LOGE("Input Tensor dimensions order not set\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        _mpp_t *mpp = elem->mpp;
        if (!mpp) {
            MPP_LOGE("mpp is null\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* get input parameters from previous element */
        buf_desc_t *prev_buf = elem->prev->io.out_buf[0];

        /* check element entry/function */
        if (elem->entry == NULL)
        {
            MPP_LOGE("Element entry function not set\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        /* check the vision algo device */
        if (elem->dev.valgo == NULL)
        {
            MPP_LOGE("Element's algo device not set\n");
            ret = MPP_INVALID_PARAM;
            break;
        }

        memcpy(&elem->params, params, sizeof(mpp_element_params_t));

        model_param_t hal_params;
        memset(&hal_params, 0, sizeof(model_param_t));
        hal_params.model_data = elem->params.ml_inference.model_data;
        hal_params.model_size = elem->params.ml_inference.model_size;
        hal_params.model_input_mean = elem->params.ml_inference.model_input_mean;
        hal_params.model_input_std = elem->params.ml_inference.model_input_std;
        hal_params.evt_callback_f = mpp->params.evt_callback_f;
        hal_params.cb_userdata = mpp->params.cb_userdata;
        hal_params.tensor_order = elem->params.ml_inference.tensor_order;
        memcpy(&hal_params.inference_params,
                &elem->params.ml_inference.inference_params,
                sizeof(mpp_inference_params_t));

        /*Get resolution parameters from previous element*/
        hal_params.format = prev_buf->format;
        hal_params.height = prev_buf->height;
        hal_params.width = prev_buf->width;

        /* init HAL function */
        valgo = elem->dev.valgo;
        hal_valgo_status_t algoret = kStatus_HAL_ValgoSuccess;
        algoret = valgo->ops->deinit(valgo);
        if (algoret != kStatus_HAL_ValgoSuccess)
        {
            MPP_LOGE ("HAL inference deinit() fails with ret=%d\n", algoret);
            ret = MPP_ERROR;
            break;
        }
        algoret = valgo->ops->init(valgo, &hal_params);
        if (algoret != kStatus_HAL_ValgoSuccess)
        {
            MPP_LOGE ("HAL inference init() fails with ret=%d\n", algoret);
            ret = MPP_ERROR;
            break;
        }

        /* update buffer requirements from HAL */
        valgo->ops->get_buf_desc(valgo, &elem->io.in_buf[0]->hw_req_cons,
                                 &elem->io.mem_policy);

        /* check if new res < prev res */
        if (elem->io.in_buf[0]->hw_req_cons.nb_lines != prev_buf->hw->nb_lines)
        {
            MPP_LOGE ("Input buffer height (%d) does not match model height (%d)\r\n",
                    prev_buf->hw->nb_lines,
                    elem->io.in_buf[0]->hw_req_cons.nb_lines);
            ret = MPP_ERROR;
            break;
        }
        if (elem->io.in_buf[0]->hw_req_cons.stride != prev_buf->hw->stride)
        {
            MPP_LOGE ("Input buffer stride (%d) does not match model stride (%d)\r\n",
                    prev_buf->hw->stride,
                    elem->io.in_buf[0]->hw_req_cons.stride);
            ret = MPP_ERROR;
            break;
        }

    } while (false);

    if (ret != MPP_SUCCESS) {
        if ( (elem != NULL) && (elem->io.out_buf[0] != NULL) )
            hal_free(elem->io.out_buf[0]);
        if (valgo != NULL)
            hal_free(valgo);
    }

    return ret;
}
