/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the license terms that accompany it. By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

/*
 * @brief vision algorithm TFLite HAL driver implementation for the MCU Media Processing Pipeline.
 */

#include "mpp_config.h"
#include "hal_valgo_dev.h"
#include "hal_debug.h"
#include "hal.h"

#if (HAL_ENABLE_INFERENCE_TFLITE == 1)

#include <stdlib.h>
#include <stdio.h>
#include "tflite/model.h"
#include "mpp_api_types.h"

typedef struct _tflite_model_param
{
    mpp_tensor_dims_t inputDims;
    mpp_tensor_type_t inputType;
    uint8_t* inputData;
    int (*out_cb)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);
    void *userdata;
    float model_in_mean;
    float model_in_std;
    mpp_tensor_order_t tensor_order;
    mpp_inference_cb_param_t out_param;
} tflite_model_param_t;

/* returns true if ok, false in case of issue */
static bool check_model_input_dims(tflite_model_param_t *param)
{
    if (param == NULL)
    {
        HAL_LOGE("Model parameters is NULL pointer\n");
        return false;
    }
    if (param->inputDims.size != 4)
    {
        HAL_LOGE("Input Tensor not supported, expected 4 dimensions\n");
        return false;
    }
    return true;
}

static int get_model_input_width(tflite_model_param_t *param)
{
    if (!check_model_input_dims(param)) return 0;

    if (param->tensor_order == MPP_TENSOR_ORDER_NCHW)
    {
        return param->inputDims.data[3];
    }
    else if (param->tensor_order == MPP_TENSOR_ORDER_NHWC)
    {
        return param->inputDims.data[2];
    }
    else
    {
        HAL_LOGE("Bad Input Tensor order\n");
        return 0;
    }
}

static int get_model_input_height(tflite_model_param_t *param)
{
    if (!check_model_input_dims(param)) return 0;

    if (param->tensor_order == MPP_TENSOR_ORDER_NCHW)
    {
        return param->inputDims.data[2];
    }
    else if (param->tensor_order == MPP_TENSOR_ORDER_NHWC)
    {
        return param->inputDims.data[1];
    }
    else
    {
        HAL_LOGE("Bad Input Tensor order\n");
        return 0;
    }
}

static int get_model_input_channels(tflite_model_param_t *param)
{
    if (!check_model_input_dims(param)) return 0;

    if (param->tensor_order == MPP_TENSOR_ORDER_NCHW)
    {
        return param->inputDims.data[1];
    }
    else if (param->tensor_order == MPP_TENSOR_ORDER_NHWC)
    {
        return param->inputDims.data[3];
    }
    else
    {
        HAL_LOGE("Bad Input Tensor order\n");
        return 0;
    }
}

static hal_valgo_status_t HAL_VisionAlgoDev_TFLite_Init(vision_algo_dev_t *dev,
                                                           valgo_dev_callback_t callback,
                                                           model_param_t *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    tflite_model_param_t *tflite_model_param;

    HAL_LOGD("++HAL_VisionAlgoDev_TFLite_Init\n");
    
    if (kStatus_Success != MODEL_Init(param->model_data, param->model_size)) {
        HAL_LOGE("ERROR: MODEL_Init() failed\n");
        return kStatus_HAL_ValgoInitError;
    }

    // init the device
    memset(&dev->cap, 0, sizeof(dev->cap));
    dev->cap.callback = callback;
    dev->priv_data = MPP_MALLOC(sizeof(tflite_model_param_t));
    tflite_model_param = (tflite_model_param_t *)dev->priv_data;
    if(dev->priv_data == NULL){
        HAL_LOGE("NULL pointer\n");
        return kStatus_HAL_ValgoMallocError ;
    }
    memset(dev->priv_data, 0, sizeof(tflite_model_param));

    // get parameters from user passed to HAL
    tflite_model_param->out_cb = param->evt_callback_f;
    tflite_model_param->userdata = param->cb_userdata;
    // get parameters from model passed to HAL
    tflite_model_param->model_in_mean = param->model_input_mean;
    tflite_model_param->model_in_std = param->model_input_std;
    tflite_model_param->tensor_order = param->tensor_order;
    tflite_model_param->inputData = MODEL_GetInputTensorData(&(tflite_model_param->inputDims), &(tflite_model_param->inputType));
    if (!check_model_input_dims(tflite_model_param)) return kStatus_HAL_ValgoInitError;

    // get output tensors description
    int i;
    for(i = 0; i < MPP_INFERENCE_MAX_OUTPUTS; i++)
    {
        if (i >= param->inference_params.num_outputs)
        {   /* set to null unused output tensors descriptions */
            tflite_model_param->out_param.out_tensors[i] = NULL;
            continue;
        }
        tflite_model_param->out_param.out_tensors[i] = MPP_MALLOC(sizeof(mpp_inference_out_tensor_params_t));
        if(tflite_model_param->out_param.out_tensors[i] == NULL){
            HAL_LOGE("NULL pointer\n");
            return kStatus_HAL_ValgoMallocError ;
        }
        memset(tflite_model_param->out_param.out_tensors[i], 0, sizeof(mpp_inference_out_tensor_params_t));
        tflite_model_param->out_param.out_tensors[i]->data = MODEL_GetOutputTensorData(
                &(tflite_model_param->out_param.out_tensors[i]->dims),
                &(tflite_model_param->out_param.out_tensors[i]->type),
                i);
    }

    /* display model input format info */
    HAL_LOGI("Model expects width = %d", get_model_input_width(tflite_model_param));
    HAL_LOGI("Model expects height = %d", get_model_input_height(tflite_model_param));

    switch(tflite_model_param->inputType) {
    case MPP_TENSOR_TYPE_UINT8:
    case MPP_TENSOR_TYPE_INT8:
        if (get_model_input_channels(tflite_model_param) == 3) {
            HAL_LOGI("Model expects format = MPP_PIXEL_RGB");
        } else if (get_model_input_channels(tflite_model_param) == 1) {
            HAL_LOGI("Model expects format = MPP_PIXEL_GRAY");
        } else {
            HAL_LOGE("--HAL_VisionAlgoDev_TFLite_getInput: invalid number of channels\n");
            ret = kStatus_HAL_ValgoError;
        }
        break;
    case MPP_TENSOR_TYPE_FLOAT32:
    default:
        HAL_LOGE("--HAL_VisionAlgoDev_TFLite_getInput: input tensor format not supported\n");
        ret = kStatus_HAL_ValgoError;
        break;
    }

    HAL_LOGD("--HAL_VisionAlgoDev_TFLite_Init\n");
    return ret;
}

// deinitialize the dev
static hal_valgo_status_t HAL_VisionAlgoDev_TFLite_Deinit(vision_algo_dev_t *dev)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    HAL_LOGD("++HAL_VisionAlgoDev_TFLite_Deinit\n");

    if (dev->priv_data != NULL) {
        MPP_FREE(dev->priv_data);
    }

    HAL_LOGD("--HAL_VisionAlgoDev_TFLite_Deinit\n");
    return ret;
}

// start the dev
static hal_valgo_status_t HAL_VisionAlgoDev_TFLite_Run(const vision_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    tflite_model_param_t *tflite_model_param;
    HAL_LOGD("++HAL_VisionAlgoDev_TFLite_Run\n");

    tflite_model_param = (tflite_model_param_t *)dev->priv_data;

    // TODO replace by a generic model->ConvertInput() call
    MODEL_ConvertInput((uint8_t *) tflite_model_param->inputData,
            &(tflite_model_param->inputDims),
            tflite_model_param->inputType,
            tflite_model_param->model_in_mean,
            tflite_model_param->model_in_std);

    int startTime = TICK_TO_MS(GET_TICK());
    if (kStatus_Success != MODEL_RunInference()) {
        HAL_LOGE("ERROR: MODEL_RunInference() failed\n");
        return kStatus_HAL_ValgoError;
    }
    tflite_model_param->out_param.inference_time_ms = TICK_TO_MS(GET_TICK()) - startTime;
    tflite_model_param->out_param.inference_type = MPP_INFERENCE_TYPE_TFLITE;

    tflite_model_param->out_cb(
            NULL, /* TODO pass mpp_t object here? */
            MPP_EVENT_INFERENCE_OUTPUT_READY,
            (void *)&tflite_model_param->out_param,
            tflite_model_param->userdata);

    HAL_LOGD("--HAL_VisionAlgoDev_TFLite_Run\n");
    return ret;
}

static hal_valgo_status_t HAL_VisionAlgoDev_TFLite_getBufDesc(const vision_algo_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    tflite_model_param_t *tflite_model_param;
    HAL_LOGD("++HAL_VisionAlgoDev_TFLite_getInput\n");

    if ((in_buf == NULL) || (policy == NULL))
    {
        HAL_LOGE("\nNULL pointer\n");
        return kStatus_HAL_ValgoError;
    }
    /* TFlite allocates tensor arena */
    *policy = HAL_MEM_ALLOC_BOTH;

    tflite_model_param = (tflite_model_param_t *)dev->priv_data;
    /*
     * TODO: add support for other TFlite models with different component order.
     * For now, this is only assuming NHWC order.
     */
    in_buf->alignment = 0;
    in_buf->cacheable = true;
    in_buf->stride = tflite_model_param->inputDims.data[2] * tflite_model_param->inputDims.data[3]; /* width * channels */
    in_buf->addr = tflite_model_param->inputData;

    HAL_LOGD("--HAL_VisionAlgoDev_TFLite_getInput\n");
    return ret;
}

const static vision_algo_dev_operator_t s_VisionAlgoDev_TFLiteOps = {
    .init        = HAL_VisionAlgoDev_TFLite_Init,
    .deinit      = HAL_VisionAlgoDev_TFLite_Deinit,
    .run         = HAL_VisionAlgoDev_TFLite_Run,
    .get_buf_desc   = HAL_VisionAlgoDev_TFLite_getBufDesc,
};

int hal_inference_tflite_setup(vision_algo_dev_t *dev)
{
    dev->id = 0;    /* TODO set unique id */
    dev->ops = &s_VisionAlgoDev_TFLiteOps;

    return 0;
}
#else  /* (HAL_ENABLE_INFERENCE_TFLITE != 1) */
int hal_inference_tflite_setup(vision_algo_dev_t *dev)
{
    HAL_LOGE("Inference TF-lite not enabled\n");
    return -1;
}
#endif /* (HAL_ENABLE_INFERENCE_TFLITE == 1) */
