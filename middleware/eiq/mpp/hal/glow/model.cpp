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

#include <stdio.h>
#include "hal_valgo_dev.h"
#include "model.h"

status_t GLOW_MODEL_RunInference(inference_entry_point_t model_entry_point, uint8_t * constant_weights, uint8_t * mutable_weights, uint8_t * activations)
{
    if ( (model_entry_point)(constant_weights, mutable_weights, activations)!= 0)
    {
        HAL_LOGE("Execution failed!\r\n");
        return kStatus_Fail;
    }
    return kStatus_Success;
}
// Convert image data to model input format in-place.
status_t GLOW_ConvertInput(uint8_t* input_Addr, mpp_tensor_type_t type, int input_size)
{
	int ret = kStatus_Success ;
	switch(type){
	case MPP_TENSOR_TYPE_UINT8 :
		break;
	case MPP_TENSOR_TYPE_INT8 :
	{
		int8_t *bundleInput = (int8_t *)input_Addr;
		for (int i = 0; i < input_size; i++){
		    bundleInput[i] = (int8_t)input_Addr[i] - SIGN_UINT;
		}
	}
	    break ;
	case MPP_TENSOR_TYPE_FLOAT32 :
	    HAL_LOGE("FLOAT INPUT FORMAT IS NOT SUPPORTED\n");
	    ret = kStatus_Fail ;
	    break;
	default:
	{
		printf("UNKNOWN INPUT DATA TYPE\r\n");
		ret = kStatus_Fail ;
	}
	    break ;
	}
	return ret;
}
