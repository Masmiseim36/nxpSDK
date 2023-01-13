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
 * @brief vision algorithm GLOW HAL driver implementation.
 */
#include "mpp_config.h"
#include "hal_valgo_dev.h"
#include "hal_debug.h"
#include "hal.h"

#if (HAL_ENABLE_INFERENCE_GLOW == 1)

#include <stdlib.h>
#include <stdio.h>
#include "mpp_api_types.h"
#include "glow/model.h"

// Memory alignment definition with given alignment size
// for static allocation of memory.
#define MPP_GLOW_MEM_ALIGN(size)  __attribute__((aligned(size)))

// Macro function to get the absolute address of a
// placeholder using the base address of the mutable
// weight buffer and placeholder offset definition.
#define MPP_GLOW_GET_ADDR(mutableBaseAddr, placeholderOff)  (((uint8_t*)(mutableBaseAddr)) + placeholderOff)

MPP_GLOW_MEM_ALIGN(GLOW_MEMORY_ALIGN)
uint8_t constantWeight[GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE];
MPP_GLOW_MEM_ALIGN(GLOW_MEMORY_ALIGN)
uint8_t mutableWeight[GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE];
MPP_GLOW_MEM_ALIGN(GLOW_MEMORY_ALIGN)
uint8_t activations[GLOW_ACTIVATIONS_MAX_MEMSIZE];

/* Glow input parameters */
typedef struct {
    int height;
    int width;
    int num_channels;
    mpp_pixel_format_t format;
    mpp_tensor_type_t type;
} glow_input_params_t;

/*glow model parameters*/
typedef struct {
    int model_size;
    glow_input_params_t inputParams;
    inference_entry_point_t bundlefuntion ; //callback used to perform inference
	uint8_t* constantWeight;
	uint8_t* mutableWeight;
	uint8_t* activations;
	int (*out_cb)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);
	mpp_inference_cb_param_t out_param;
	void *userdata;
    uint8_t* inputAddr;
    int num_outputs;
    uint8_t *outputsAddrs[MPP_INFERENCE_MAX_OUTPUTS];
} glow_model_param_t;

/*Initialize the dev for glow*/
static hal_valgo_status_t HAL_VisionAlgoDev_Glow_Init(vision_algo_dev_t *dev, valgo_dev_callback_t callback,
		model_param_t *param)
{
	hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
	glow_model_param_t *glow_model_param;
	int img_bytes, comp_bytes;
	HAL_LOGD("++HAL_VisionAlgoDev_Glow_Init\n");

	if (param->inference_params.constant_weight_MemSize > GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE)
	{
	    HAL_LOGE("--HAL_VisionAlgoDev_Glow_Init: "
	            "ERROR: constant weights memory size (%d) exceeds max limit (%d)\n",
	            (int)(param->inference_params.constant_weight_MemSize), GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE);
	    ret = kStatus_HAL_ValgoInitError ;
	    return ret;
	}
	if (param->inference_params.mutable_weight_MemSize > GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE)
	{
	    HAL_LOGE("--HAL_VisionAlgoDev_Glow_Init: "
	            "ERROR: mutable weights memory size (%d) exceeds max limit (%d)\n",
	            (int)(param->inference_params.mutable_weight_MemSize), GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE);
	    ret = kStatus_HAL_ValgoInitError ;
	    return ret;
	}
	if (param->inference_params.activations_MemSize > GLOW_ACTIVATIONS_MAX_MEMSIZE)
	{
	    HAL_LOGE("--HAL_VisionAlgoDev_Glow_Init: "
	            "ERROR: activations memory size (%d) exceeds max limit (%d)\n",
	            (int)(param->inference_params.activations_MemSize), GLOW_ACTIVATIONS_MAX_MEMSIZE);
	    ret = kStatus_HAL_ValgoInitError ;
	    return ret;
	}
	/* Init the device */
	memset(&dev->cap, 0, sizeof(dev->cap));
	dev->cap.callback = callback;
	dev->priv_data = MPP_MALLOC(sizeof(glow_model_param_t));
	if(dev->priv_data == NULL){
	    HAL_LOGE("NULL pointer\n");
	    return kStatus_HAL_ValgoMallocError ;
	}
	memset(dev->priv_data, 0, sizeof(glow_model_param_t));
	glow_model_param = (glow_model_param_t *)dev->priv_data;
    /*get parameters from previous element*/
    glow_model_param->inputParams.height = param->height;
    glow_model_param->inputParams.width = param->width;
    glow_model_param->inputParams.format = param->format;
    switch(param->format)
    {
       case MPP_PIXEL_RGB:
       case MPP_PIXEL_BGR:
       case MPP_PIXEL_RGB565:
           glow_model_param->inputParams.num_channels = 3;
           break;
       case MPP_PIXEL_GRAY:
           glow_model_param->inputParams.num_channels = 1;
           break;
       default:
       {
           HAL_LOGE("PIXEL FORMAT IS NOT SUPPORTED\n");
           MPP_FREE(dev->priv_data);
           return kStatus_HAL_ValgoInitError ;
       }
       break;
    }
    /* check input image resolution fits in input tensor (mutable) buffer */
    switch(param->inference_params.model_input_tensors_type)
    {
    case MPP_TENSOR_TYPE_FLOAT32:
        comp_bytes = 4;
        break;
    case MPP_TENSOR_TYPE_INT8:
    case MPP_TENSOR_TYPE_UINT8:
        comp_bytes = 1;
        break;
    default:
        comp_bytes = 0;
        HAL_LOGE("TENSOR TYPE IS NOT SUPPORTED\n");
        MPP_FREE(dev->priv_data);
        return kStatus_HAL_ValgoInitError ;
    }
    img_bytes = param->width * param->height * glow_model_param->inputParams.num_channels * comp_bytes;
    if (img_bytes > param->inference_params.mutable_weight_MemSize)
    {
            HAL_LOGE("IMAGE IS LARGER THAN GLOW MUTABLE BUFFER\n");
            MPP_FREE(dev->priv_data);
            return kStatus_HAL_ValgoInitError ;
    }
    /* get parameters from user passed to HAL */
    glow_model_param->inputParams.type = param->inference_params.model_input_tensors_type;
	glow_model_param->bundlefuntion = param->inference_params.model_entry_point ;
	glow_model_param->out_cb = param->evt_callback_f;
	glow_model_param->userdata = param->cb_userdata;
	glow_model_param->model_size = param->model_size ;
    glow_model_param->num_outputs = param->inference_params.num_outputs;
    /* get model data */
	glow_model_param->constantWeight = constantWeight ;
    memcpy(glow_model_param->constantWeight, param->model_data, param->inference_params.constant_weight_MemSize);
	glow_model_param->mutableWeight = mutableWeight;
	glow_model_param->activations = activations;
	/* get inputs and outputs addresses */
	glow_model_param->inputAddr = MPP_GLOW_GET_ADDR(glow_model_param->mutableWeight,
	        param->inference_params.inputs_offsets[0]);
	/* Get ouput */
	for(int i = 0; i < param->inference_params.num_outputs; i++){
	    glow_model_param->outputsAddrs[i] = MPP_GLOW_GET_ADDR(glow_model_param->mutableWeight,
	            param->inference_params.outputs_offsets[i]);
	    glow_model_param->out_param.out_tensors[i] = MPP_MALLOC(sizeof(mpp_inference_out_tensor_params_t));
	    if(glow_model_param->out_param.out_tensors[i] == NULL){
	        HAL_LOGE("NULL pointer\n");
            MPP_FREE(dev->priv_data);
	        return kStatus_HAL_ValgoMallocError ;
	    }
	    memset(glow_model_param->out_param.out_tensors[i], 0, sizeof(mpp_inference_out_tensor_params_t));
	}

	HAL_LOGD("--HAL_VisionAlgoDev_Glow_Init\n");

	return ret;
}

/* deinitialize the dev for glow */
static hal_valgo_status_t HAL_VisionAlgoDev_Glow_Deinit(vision_algo_dev_t *dev)
{
	hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
	HAL_LOGD("++HAL_VisionAlgoDev_Glow_Deinit\n");

	if (dev->priv_data != NULL) {
		MPP_FREE(dev->priv_data);
	}

	HAL_LOGD("--HAL_VisionAlgoDev_Glow_Deinit\n");
	return ret;
}

/* start the dev */
static hal_valgo_status_t HAL_VisionAlgoDev_Glow_Run(const vision_algo_dev_t *dev, void *data)
{

	hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
	glow_model_param_t *glow_model_param ;
	int start_time, stop_time, imgnumval;

	HAL_LOGD("++HAL_VisionAlgoDev_GLOW_Run\n");

	glow_model_param = (glow_model_param_t *)dev->priv_data;
	imgnumval = glow_model_param->inputParams.width * glow_model_param->inputParams.height * glow_model_param->inputParams.num_channels;

	ret = GLOW_ConvertInput(glow_model_param->inputAddr, glow_model_param->inputParams.type, imgnumval);
	if(ret){
		HAL_LOGE("ERROR: Failed to convert input buffer\n");
	}
	/* run the inference and compute the duration */
	start_time = TICK_TO_MS(GET_TICK());
	if (kStatus_Success != GLOW_MODEL_RunInference(glow_model_param->bundlefuntion,
	        glow_model_param->constantWeight, glow_model_param->mutableWeight, glow_model_param->activations)) {
		HAL_LOGE("ERROR: MODEL_RunInference() failed\n");
		return kStatus_HAL_ValgoError;
	}
	stop_time = TICK_TO_MS(GET_TICK());
	/* get inference outputs */
    for(int i=0; i < glow_model_param->num_outputs; i++){
        glow_model_param->out_param.out_tensors[i]->data = glow_model_param->outputsAddrs[i] ;
    }
	glow_model_param->out_param.inference_time_ms = stop_time - start_time;
	glow_model_param->out_param.inference_type = MPP_INFERENCE_TYPE_GLOW;

	glow_model_param->out_cb(NULL, MPP_EVENT_INFERENCE_OUTPUT_READY,
			(void *)&glow_model_param->out_param , glow_model_param->userdata);

	HAL_LOGD("--HAL_VisionAlgoDev_Glow_Run\n");
	return ret;
}

/* Get buffer description */
static hal_valgo_status_t HAL_VisionAlgoDev_Glow_getBufDesc(const vision_algo_dev_t *dev,
        hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy)
{
	hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
	glow_model_param_t *glow_model_param;
	HAL_LOGD("++HAL_VisionAlgoDev_Glow_getInputBuffer\n");

    if ((in_buf == NULL) || (policy == NULL))
    {
        HAL_LOGE("NULL pointer\n");
        return kStatus_HAL_ValgoError;
    }

    *policy = HAL_MEM_ALLOC_BOTH;

	glow_model_param = (glow_model_param_t *)dev->priv_data;

	/* set the pointer in device data that will be passed via Fwk Msg */
    in_buf->alignment = 0;
    in_buf->cacheable = true;
    in_buf->stride = glow_model_param->inputParams.width * glow_model_param->inputParams.num_channels; /* width * channels */
    in_buf->addr = glow_model_param->inputAddr;

	HAL_LOGD("--HAL_VisionAlgoDev_GLOW_getInput\n");
	return ret;
}

const static vision_algo_dev_operator_t s_VisionAlgoDev_GLOWOps = {
		.init        = HAL_VisionAlgoDev_Glow_Init,
		.deinit      = HAL_VisionAlgoDev_Glow_Deinit,
		.run         = HAL_VisionAlgoDev_Glow_Run,
		.get_buf_desc    = HAL_VisionAlgoDev_Glow_getBufDesc,
};

int hal_inference_glow_setup(vision_algo_dev_t *dev)
{
	dev ->id = 1;
	dev->ops = (vision_algo_dev_operator_t *)&s_VisionAlgoDev_GLOWOps ;
	return 0;
};
#else  /* (HAL_ENABLE_INFERENCE_GLOW != 1) */
int hal_inference_glow_setup(vision_algo_dev_t *dev)
{
    HAL_LOGE("Inference Glow not enabled\n");
    return -1;
}
#endif /* (HAL_ENABLE_INFERENCE_GLOW == 1) */
