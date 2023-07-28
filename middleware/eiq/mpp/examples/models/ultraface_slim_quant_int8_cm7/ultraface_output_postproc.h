/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ULTRAFACE_M_OUTPUT_POSTPROCESS_H_
#define _ULTRAFACE_M_OUTPUT_POSTPROCESS_H_

#include "mpp_api_types.h"
#include "../utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_POINTS          4420 //maximum number of center priors

/**
 * Process the Ultraface output tensors
 *
 * @param [in] inf_out: inference output (tensor data and description)
 * @param [out] final_boxes: array of object bounding boxes
 * @param [in] nb_box_max: nb of elements in array final_boxes
 *
 * @return: 0 if succeeded, else failed.
 */
int32_t ULTRAFACE_ProcessOutput(const mpp_inference_cb_param_t *inf_out, box_data* final_boxes, int nb_box_max);

#endif /* _ULTRAFACE_M_OUTPUT_POSTPROCESS_H_ */
