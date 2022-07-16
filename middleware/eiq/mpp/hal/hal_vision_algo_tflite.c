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

#include <stdlib.h>
#include <stdio.h>
#include "hal.h"
#include "tflite/model.h"
#include "hal_valgo_dev.h"
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
    mpp_inference_cb_param_t out_param;
} tflite_model_param_t;

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

    // get parameters from user passed to HAL
    tflite_model_param->out_cb = param->evt_callback_f;
    tflite_model_param->userdata = param->cb_userdata;
    // get parameters from model passed to HAL
    tflite_model_param->model_in_mean = param->model_input_mean;
    tflite_model_param->model_in_std = param->model_input_std;
    tflite_model_param->inputData = MODEL_GetInputTensorData(&(tflite_model_param->inputDims), &(tflite_model_param->inputType));
    tflite_model_param->out_param.tensor = MODEL_GetOutputTensorData(&(tflite_model_param->out_param.dims), &(tflite_model_param->out_param.type));

    /* display model input format info */
    HAL_LOGI("Model expects width = %d", tflite_model_param->inputDims.data[2]);
    HAL_LOGI("Model expects height = %d", tflite_model_param->inputDims.data[1]);

    switch(tflite_model_param->inputType) {
    case MPP_TENSOR_TYPE_UINT8:
    case MPP_TENSOR_TYPE_INT8:
        if (tflite_model_param->inputDims.data[3] == 3) {
            HAL_LOGI("Model expects format = MPP_PIXEL_RGB");
        } else if (tflite_model_param->inputDims.data[3] == 1) {
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

int hal_tflite_setup(vision_algo_dev_t *dev)
{
    dev->id = 0;    /* TODO set unique id */
    dev->ops = &s_VisionAlgoDev_TFLiteOps;

    return 0;
}
