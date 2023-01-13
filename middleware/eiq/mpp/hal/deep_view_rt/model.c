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

#include "mpp_config.h"
#include "board_config.h"

#if (HAL_ENABLE_INFERENCE_DVRT == 1)
#include <stdio.h>
#include "hal_valgo_dev.h"
#include "model.h"

#define DVRT_SIGN_UINT 128

/* The cache is used by deep view RT to optimize certain internal loops.
 * Its size should be adjusted based on the model needs.
 * If DVRT_USE_CACHE is set to 0 this memory will be allocated in the heap.*/
#ifndef HAL_DVRT_CACHE_SIZE_KB
#define HAL_DVRT_CACHE_SIZE_KB 128 /* default cache size. */
#endif
#define DVRT_CACHE_SIZE HAL_DVRT_CACHE_SIZE_KB * 1024

#if (HAL_DVRT_USE_CACHE == 1)
uint8_t *dvrt_cache = (uint8_t*)(HAL_DVRT_CACHE_MEMORY);
#else
uint8_t *dvrt_cache = NULL;
#endif

#ifndef HAL_DVRT_MEMPOOL_SIZE_MB
#define HAL_DVRT_MEMPOOL_SIZE_MB 5 /* default mempool size */
#endif

/* The mempool holds DeepViewRT intermediate buffers and can be adjusted based on the model needs. */
#define DVRT_MEMPOOL_SIZE HAL_DVRT_MEMPOOL_SIZE_MB * 1024 * 1024

/* DVRT Memory Pool buffer. */
SDK_ALIGN(uint8_t dvrt_mempool[DVRT_MEMPOOL_SIZE], 32);

NNContext* dvrt_model_init(const void *model_data, size_t model_size)
{
    NNContext *model_nn_context = NULL;

    /* initialize the model nn context */
    model_nn_context = nn_context_init(NULL,
                                       DVRT_MEMPOOL_SIZE, dvrt_mempool,
                                       DVRT_CACHE_SIZE, dvrt_cache);

    if (model_nn_context == NULL) {
            HAL_LOGE("ERROR: DVRT: INSUFFICIENT MEMORY TO CREATE NN CONTEXT\r\n");
            return model_nn_context;
    }

    /* load the provided model into the nn_context */
    NNError err = nn_context_model_load(model_nn_context, model_size, model_data);
    if (err) {
            HAL_LOGE("ERROR: DVRT: FAILED TO LOAD MODEL: %s\r\n", nn_strerror(err));
            // release the memory used by the model_nn_context
            nn_context_release(model_nn_context);
            return NULL;
    }

    return model_nn_context;
}

/* This function is used to get input/output tensors:
 *    -type
 *    -number of dimensions
 *    -shape of each dimension
 */
status_t dvrt_get_tensor_data(NNTensor* placeholder_tensor, mpp_tensor_type_t* type, mpp_tensor_dims_t* dims)
{
    status_t ret = kStatus_Success;

    /* Get tensor type */
    NNTensorType tensor_type;
    tensor_type = nn_tensor_type(placeholder_tensor);
    switch (tensor_type) {
        case NNTensorType_I8: {
            *type = MPP_TENSOR_TYPE_INT8;
            break;
        }
        case NNTensorType_U8: {
            *type = MPP_TENSOR_TYPE_UINT8;
            break;
        }
        case NNTensorType_F32: {
            *type = MPP_TENSOR_TYPE_FLOAT32;
            break;
        }
        default: {
            HAL_LOGE("ERROR: DVRT: UNKNOWN TENSOR DATA TYPE");
            ret = kStatus_Fail ;
            break;
        }
    }

    /* Get tensor number of dimensions */
    dims->size = (uint32_t)nn_tensor_dims(placeholder_tensor);
    if(dims->size > MAX_TENSOR_DIMS) {
            HAL_LOGE("ERROR: DVRT: TENSOR DIMS EXCEEDS MAX_TENSOR_DIMS: %d!\r\n", MAX_TENSOR_DIMS);
            ret = kStatus_Fail;
            return ret;
    }

    /* Get shape of each dimension : for NHWC layout*/
    const int32_t* input_shape = nn_tensor_shape(placeholder_tensor);
    for (int i=0; i < (int)(dims->size); i++) {
            dims->data[i] = (uint32_t)input_shape[i];
    }

    return ret;
}

/* This function will return an uint8_t accessible pointer to the input area in nn_context */
uint8_t* dvrt_get_input_layer_tensor(const void *model_data, NNContext* context, mpp_tensor_type_t* type,
                                  mpp_tensor_dims_t* dims, size_t* tensor_size)
{
    NNTensor* input_tensor = NULL;
    size_t input_index = (size_t)(nn_model_inputs(model_data, NULL))[0];
    input_tensor = nn_context_tensor_index(context, input_index);
    if (input_tensor == NULL) {
            HAL_LOGE("ERROR: DVRT: FAILED TO GET INPUT TENSOR\r\n");
            return NULL;
    }

    /* get tensor size */
    *tensor_size = (size_t)nn_tensor_size(input_tensor);

    /* get tensor type and dims */
    status_t status = dvrt_get_tensor_data(input_tensor, type, dims);
    if (status == kStatus_Fail) {
            HAL_LOGE("ERROR: DVRT: INVALID INPUT DIMS\r\n");
            return NULL;
    }

    /* get pointer to input data */
    uint8_t* input_data = (uint8_t *)nn_tensor_maprw(input_tensor);
    nn_tensor_unmap(input_tensor);
    if(input_data == NULL)
        HAL_LOGE("ERROR: DVRT: NO MEMORY FOR INPUT TENSOR!\r\n");

    return input_data;
}

/* This function will return an uint8_t accessible pointer to the output area in nn_context */
const uint8_t* dvrt_get_output_layer_tensor(const void *model_data, NNContext* context,
                                         mpp_tensor_dims_t* dims, mpp_tensor_type_t* type, int output_idx)
{
    NNTensor* output_tensor = NULL;
    size_t output_index = (size_t)(nn_model_outputs(model_data, NULL))[output_idx];
    output_tensor = nn_context_tensor_index(context, output_index);
    if (!output_tensor) {
            HAL_LOGE("ERROR: DVRT: FAILED TO GET OUTPUT TENSOR\r\n");
            return NULL;
    }

    /* get tensor dims and type */
    status_t status = dvrt_get_tensor_data(output_tensor, type, dims);
    if (status == kStatus_Fail) {
            HAL_LOGE("ERROR: DVRT: INVALID OUTPUT '%d' DIMS\r\n", output_idx+1);
            return NULL;
    }

    const uint8_t *output_data = (const uint8_t *)nn_tensor_mapro(output_tensor);
    nn_tensor_unmap(output_tensor);
    if(output_data == NULL)
        HAL_LOGE("ERROR: DVRT: OUTPUT TENSOR MAPPED WRITE-ONLY\r\n");

    return output_data;
}

// Convert image data to model input format in-place.
status_t dvrt_convert_input(uint8_t* input_addr, mpp_tensor_type_t type, int input_size)
{
    int ret = kStatus_Success ;
    switch(type) {
        case MPP_TENSOR_TYPE_UINT8 :
            break;
        case MPP_TENSOR_TYPE_INT8 : {
            int8_t *bundle_input = (int8_t *)input_addr;
            for (int i = 0; i < input_size; i++)
                bundle_input[i] = (int8_t)input_addr[i] - DVRT_SIGN_UINT;
            break ;
        }
        case MPP_TENSOR_TYPE_FLOAT32 : {
            HAL_LOGE("ERROR: DVRT: FLOAT INPUT FORMAT IS NOT SUPPORTED\n");
            ret = kStatus_Fail ;
            break ;
        }
        default: {
            HAL_LOGE("ERROR: DVRT: UNKNOWN INPUT DATA TYPE\n");
            ret = kStatus_Fail ;
            break ;
        }
    }
    return ret;
}
#endif /* (HAL_ENABLE_INFERENCE_DVRT == 1) */
