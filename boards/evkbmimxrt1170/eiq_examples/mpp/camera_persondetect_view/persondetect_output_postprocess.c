/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "persondetect_output_postprocess.h"
#include <math.h>
#include <stdio.h>
#include "get_top_n.h"
#include "fsl_debug_console.h"
#include "mpp_config.h"
#define EOL "\r\n"

/* include model infos  */
#include APP_TFLITE_PERSONDETECT_INFO

// Sigmoid function
float _sigmoid(float x)
{
	return (1 / (1 + exp(-x)));
}

/* The function that processes the tensor output of model Persondetect*/
int32_t Persondetect_Output_postprocessing(const mpp_inference_cb_param_t *inf_out,
		box_data *final_boxes, int nb_box_max)
{
	// Check for null pointers in input parameters
	if (inf_out == NULL)
	{
		PRINTF("ERROR: Persondetect_ProcessOutput parameter 'inf_out' is a null pointer" EOL);
		return -1;
	}
	if (final_boxes == NULL)
	{
		PRINTF("ERROR: Persondetect_ProcessOutput parameter 'final_boxes' is a null pointer" EOL);
		return -1;
	}

	/* clear old data */
	memset(final_boxes, 0, nb_box_max*sizeof(box_data));

	// Variables for indexing
	int index1 = 0; /* Index used to access elements in inf_out data */
	int index2 = 0; /* Index used to access elements in final_boxes arrays.*/

	// Loop through the detection grid and process detection results
	for (int i = 0; i < PERSONDETECT_GRID_HEIGHT; i++)
	{
		for (int j = 0; (j < PERSONDETECT_GRID_WIDTH) && (index2 < nb_box_max); j++)
		{
			// Calculate index for each grid cell
			index1 = i * PERSONDETECT_GRID_WIDTH * PERSONDETECT_CANAL + j * PERSONDETECT_CANAL;

			// Rescaled inf_out->out_tensors[0]->data values in intermediates variables
			float score = _sigmoid((float)(((int8_t)(inf_out->out_tensors[0]->data[index1]) - PERSONDETECT_OUTPUT_ZERO_POINT) * PERSONDETECT_OUTPUT_SCALE));
			float x = tanh((float)(((int8_t)(inf_out->out_tensors[0]->data[index1 + 1]) - PERSONDETECT_OUTPUT_ZERO_POINT) * PERSONDETECT_OUTPUT_SCALE));
			float y = tanh((float)(((int8_t)(inf_out->out_tensors[0]->data[index1 + 2]) - PERSONDETECT_OUTPUT_ZERO_POINT) * PERSONDETECT_OUTPUT_SCALE));
			float w = _sigmoid((float)(((int8_t)(inf_out->out_tensors[0]->data[index1 + 3]) - PERSONDETECT_OUTPUT_ZERO_POINT) * PERSONDETECT_OUTPUT_SCALE));
			float h = _sigmoid((float)(((int8_t)(inf_out->out_tensors[0]->data[index1 + 4]) - PERSONDETECT_OUTPUT_ZERO_POINT) * PERSONDETECT_OUTPUT_SCALE));
			float classe = _sigmoid((float)(((int8_t)(inf_out->out_tensors[0]->data[index1 + 5]) - PERSONDETECT_OUTPUT_ZERO_POINT) * PERSONDETECT_OUTPUT_SCALE));

			// Calculate and Adjust detection boxes dimensions to match those of the 'persondetect' model and save them in the final_boxes array.
			final_boxes[index2].left = (int16_t)(((x + j) / PERSONDETECT_GRID_WIDTH) * PERSONDETECT_WIDTH - (w * PERSONDETECT_WIDTH) / 2);
			final_boxes[index2].top = (int16_t)(((y + i) / PERSONDETECT_GRID_HEIGHT) * PERSONDETECT_HEIGHT - (h * PERSONDETECT_HEIGHT) / 2);
			final_boxes[index2].right = (int16_t)(((x + j) / PERSONDETECT_GRID_WIDTH) * PERSONDETECT_WIDTH + (w * PERSONDETECT_WIDTH) / 2);
			final_boxes[index2].bottom = (int16_t)(((y + i) / PERSONDETECT_GRID_HEIGHT) * PERSONDETECT_HEIGHT + (h * PERSONDETECT_HEIGHT) / 2);
			final_boxes[index2].score = score;
			final_boxes[index2].area = (int16_t)(w * PERSONDETECT_WIDTH * h * PERSONDETECT_HEIGHT);
			final_boxes[index2].label = (int16_t)classe;

			// Ensure bounding box coordinates are within model dimensions
			if (final_boxes[index2].left < 0)
			{
				final_boxes[index2].left = 0;
			}
			if (final_boxes[index2].bottom > PERSONDETECT_HEIGHT)
			{
				final_boxes[index2].bottom = PERSONDETECT_HEIGHT;
			}
			if (final_boxes[index2].right > PERSONDETECT_WIDTH)
			{
				final_boxes[index2].right = PERSONDETECT_WIDTH;
			}
			if (final_boxes[index2].top < 0)
			{
				final_boxes[index2].top = 0;
			}

			// Set score to 0 if the area is 0
			if (final_boxes[index2].area == 0)
			{
				final_boxes[index2].score = 0.00;
			}
			index2++;
		}
	}

	// Apply non-maximum suppression
	nms(final_boxes, PERSONDETECT_MAX_BOXES, PERSONDETECT_NMS_THRESHOLD, PERSONDETECT_SCORE_THRESHOLD / 100);

	return 0;
}
