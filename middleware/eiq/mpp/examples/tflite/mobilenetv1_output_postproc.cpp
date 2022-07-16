/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * The function that processes the tensor output of model MobileNet_v1
 */

#include <stdio.h>

#include "output_postproc.h"
#include "get_top_n.h"
#include "mobilenetv1_labels.h"
#include "fsl_debug_console.h"

extern "C" {
#include "mpp_api.h"
}

#define DETECTION_TRESHOLD 23
#define NUM_RESULTS        1
#define EOL "\r\n"

int32_t MODEL_ProcessOutput(const mpp_inference_cb_param_t *inf_out, void *mpp,
        mpp_elem_handle_t elem, mpp_labeled_rect_t *rects)
{
    const float threshold = (float)DETECTION_TRESHOLD / 100;
    result_t topResults[NUM_RESULTS];
    const char* label = "No label detected";
    int inferenceTime = inf_out->inference_time_ms;

    /* Find best label candidates. */
    MODEL_GetTopN(inf_out->tensor, inf_out->dims.data[inf_out->dims.size - 1], inf_out->type, NUM_RESULTS, threshold, topResults);

    float confidence = 0;
    if (topResults[0].index >= 0)
    {
        auto result = topResults[0];
        confidence = result.score;
        int index = result.index;
        if (confidence * 100 > DETECTION_TRESHOLD)
        {
            label = labels[index];
        }
    }

    int score = (int)(confidence * 100);
    PRINTF("----------------------------------------" EOL);
    PRINTF("     Inference time: %d ms" EOL, inferenceTime);
    PRINTF("     Detected: %s (%d%%)\r\n", label, score);
    PRINTF("----------------------------------------" EOL);

    if ( (mpp != NULL) && (elem != 0) && (rects != NULL) )
    {
        mpp_element_params_t params;
        /* update the label in first rectangle */
        params.labels.detected_count = 1;
        params.labels.max_count = 1;
        params.labels.rectangles = rects;
        strncpy((char *) params.labels.rectangles[0].label, label, sizeof(params.labels.rectangles[0].label));
        mpp_element_update(mpp, elem, &params);
    }

    return 0;
}
