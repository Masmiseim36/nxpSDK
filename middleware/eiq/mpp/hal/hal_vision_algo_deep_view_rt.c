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
 * @brief vision algorithm Deep view RT HAL driver implementation for the MCU Media Processing Pipeline.
 */
#include "mpp_config.h"
#include "hal_valgo_dev.h"
#include "hal_debug.h"
#include "hal.h"

#if (HAL_ENABLE_INFERENCE_DVRT == 1)

#include <stdlib.h>
#include <stdio.h>
#include "deep_view_rt/model.h"
#include "mpp_api_types.h"

/**
 * This symbol is required by DeepViewRT for internal time keeping and MUST
 * return a 64-bit signed integer of continuous nanoseconds.  The epoch is
 * not important but the counter should never reset during model evaluation.
 */
int64_t os_clock_now()
{
    return ((int64_t) TICK_TO_MS(GET_TICK()));
}

typedef struct _deep_view_rt_model_param
{
    NNContext *model_context;
    uint8_t* input_addr;
    mpp_tensor_type_t input_type;
    size_t input_size;
    mpp_tensor_dims_t input_dims;
    int (*out_cb)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);
    void *userdata;
    int n_outputs;    /* number of outputs */
    mpp_inference_cb_param_t out_param;
} deep_view_rt_model_param_t;

/* Initialize the dev for DVRT */
static hal_valgo_status_t hal_vision_algo_dev_dvrt_init(vision_algo_dev_t *dev, valgo_dev_callback_t callback,
                                                              model_param_t *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    deep_view_rt_model_param_t *deep_view_rt_model_param;

    HAL_LOGD("++hal_vision_algo_dev_dvrt_init\n");

    // init the device
    memset(&dev->cap, 0, sizeof(dev->cap));
    dev->cap.callback = callback;
    dev->priv_data = MPP_MALLOC(sizeof(deep_view_rt_model_param_t));
    if(dev->priv_data == NULL) {
            HAL_LOGE("ERROR: DVRT: NULL pointer to vision algo device private data\n");
            return kStatus_HAL_ValgoMallocError ;
    }
    deep_view_rt_model_param = (deep_view_rt_model_param_t *)dev->priv_data;
    memset(dev->priv_data, 0, sizeof(deep_view_rt_model_param));

    deep_view_rt_model_param->model_context = dvrt_model_init(param->model_data, (size_t)param->model_size);
    if (deep_view_rt_model_param->model_context == NULL) {
            HAL_LOGE("ERROR: DVRT: MODEL_Init() failed\n");
            MPP_FREE(dev->priv_data);
            return kStatus_HAL_ValgoInitError;
    }

    // get parameters from user passed to HAL
    deep_view_rt_model_param->out_cb = param->evt_callback_f;
    deep_view_rt_model_param->userdata = param->cb_userdata;

    // get input tensor address
    deep_view_rt_model_param->input_addr = dvrt_get_input_layer_tensor(param->model_data, deep_view_rt_model_param->model_context,
          &(deep_view_rt_model_param->input_type), &(deep_view_rt_model_param->input_dims),
          &(deep_view_rt_model_param->input_size));

    // get number of outputs
    deep_view_rt_model_param->n_outputs = param->inference_params.num_outputs;

    for(int i = 0; i < MPP_INFERENCE_MAX_OUTPUTS; i++) {
            if (i >= param->inference_params.num_outputs) {
                    /* set to null unused output tensors descriptions */
                    deep_view_rt_model_param->out_param.out_tensors[i] = NULL;
                    continue;
            }
            deep_view_rt_model_param->out_param.out_tensors[i] = MPP_MALLOC(sizeof(mpp_inference_out_tensor_params_t));
            if(deep_view_rt_model_param->out_param.out_tensors[i] == NULL) {
                    HAL_LOGE("ERROR: DVRT: Invalid output tensor %d \n", i);
                    MPP_FREE(dev->priv_data);
                    for(int j = 0; j < i; j++) {
                            MPP_FREE(deep_view_rt_model_param->out_param.out_tensors[j]);
                    }
                    return kStatus_HAL_ValgoMallocError ;
            }
            memset(deep_view_rt_model_param->out_param.out_tensors[i], 0, sizeof(mpp_inference_out_tensor_params_t));
            deep_view_rt_model_param->out_param.out_tensors[i]->data = dvrt_get_output_layer_tensor(param->model_data,
                deep_view_rt_model_param->model_context, &(deep_view_rt_model_param->out_param.out_tensors[i]->dims),
                &(deep_view_rt_model_param->out_param.out_tensors[i]->type), i);
    }

    HAL_LOGD("--hal_vision_algo_dev_dvrt_init\n");
    return ret;
}

/* de-initialize the dev for DVRT */
static hal_valgo_status_t hal_vision_algo_dev_dvrt_deinit(vision_algo_dev_t *dev)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    deep_view_rt_model_param_t *deep_view_rt_model_param = NULL;

    HAL_LOGD("++hal_vision_algo_dev_dvrt_deinit\n");

    if (dev->priv_data != NULL) {
            deep_view_rt_model_param = (deep_view_rt_model_param_t *)dev->priv_data;
            /* release nn_context */
            nn_context_release(deep_view_rt_model_param->model_context);
            /* free allocated output tensors */
            for(int i = 0; i < deep_view_rt_model_param->n_outputs; i++) {
                    if (deep_view_rt_model_param->out_param.out_tensors[i] != NULL) {
                            MPP_FREE(deep_view_rt_model_param->out_param.out_tensors[i]);
                            deep_view_rt_model_param->out_param.out_tensors[i] = NULL;
                    }
            }
            /* free private data */
            MPP_FREE(dev->priv_data);
            dev->priv_data = NULL;
    }
    HAL_LOGD("--hal_vision_algo_dev_dvrt_deinit\n");
    return ret;
}

/* start the dev */
static hal_valgo_status_t hal_vision_algo_dvrt_run(const vision_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    deep_view_rt_model_param_t *deep_view_rt_model_param ;

    HAL_LOGD("++hal_vision_algo_dvrt_run\n");

    deep_view_rt_model_param = (deep_view_rt_model_param_t *)dev->priv_data;

    ret = dvrt_convert_input(deep_view_rt_model_param->input_addr, deep_view_rt_model_param->input_type,
                            deep_view_rt_model_param->input_size);

    if(ret) {
            HAL_LOGE("ERROR: DVRT: Failed to convert input buffer\n");
            return kStatus_HAL_ValgoError;
    }

    int startTime = TICK_TO_MS(GET_TICK());

    // perform the inference
    NNError nn_run_err = nn_context_run(deep_view_rt_model_param->model_context);
    if (nn_run_err != NN_SUCCESS) {
            HAL_LOGE("ERROR: dvrt_model_run_inference() failed %s\n", nn_strerror(nn_run_err));
            nn_context_release(deep_view_rt_model_param->model_context);
            deep_view_rt_model_param->model_context = NULL;
            return kStatus_HAL_ValgoError;
    }

    deep_view_rt_model_param->out_param.inference_time_ms = TICK_TO_MS(GET_TICK()) - startTime;

    deep_view_rt_model_param->out_cb(
            NULL, /* TODO pass mpp_t object here? */
            MPP_EVENT_INFERENCE_OUTPUT_READY,
            (void *)&deep_view_rt_model_param->out_param,
            deep_view_rt_model_param->userdata);

    HAL_LOGD("--hal_vision_algo_dvrt_run\n");
    return ret;
}

/* Get buffer description */
static hal_valgo_status_t hal_vision_algo_dev_dvrt_get_buf_desc(const vision_algo_dev_t *dev,
                                                                    hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    deep_view_rt_model_param_t *deep_view_rt_model_param;
    HAL_LOGD("++HAL_VisionAlgoDev_Deep_View_Rt_getInput\n");

    if ((in_buf == NULL) || (policy == NULL)) {
            HAL_LOGE("\n INVALID INPUT BUFFER PARAMETERS\n");
            ret = kStatus_HAL_ValgoError;
    }

    *policy = HAL_MEM_ALLOC_BOTH;

    deep_view_rt_model_param = (deep_view_rt_model_param_t *)dev->priv_data;

    in_buf->alignment = 0;
    in_buf->cacheable = true;
    in_buf->stride = deep_view_rt_model_param->input_dims.data[2] * deep_view_rt_model_param->input_dims.data[3]; /* width * channels */

    in_buf->addr = deep_view_rt_model_param->input_addr;

    return ret;
}

const static vision_algo_dev_operator_t s_VisionAlgoDev_DVRtOps = {
        .init        = hal_vision_algo_dev_dvrt_init,
        .deinit      = hal_vision_algo_dev_dvrt_deinit,
        .run         = hal_vision_algo_dvrt_run,
        .get_buf_desc    = hal_vision_algo_dev_dvrt_get_buf_desc,
};

int hal_inference_dvrt_setup(vision_algo_dev_t *dev)
{
    dev ->id = 2;
    dev->ops = (vision_algo_dev_operator_t *)&s_VisionAlgoDev_DVRtOps ;
    return 0;
};

#else  /* (HAL_ENABLE_INFERENCE_DVRT != 1) */
int hal_inference_dvrt_setup(vision_algo_dev_t *dev)
{
    HAL_LOGE("ERROR: DeepView RT inference support not enabled\n");
    return -1;
}
#endif /* (HAL_ENABLE_INFERENCE_DVRT == 1) */
