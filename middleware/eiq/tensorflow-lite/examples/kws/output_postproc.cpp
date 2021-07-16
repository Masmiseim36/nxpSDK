/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "output_postproc.h"
#include "get_top_n.h"
#include "demo_config.h"
#include "labels.h"
#include "timer.h"

status_t MODEL_ProcessOutput(const uint8_t* data, const tensor_dims_t* dims,
                             tensor_type_t type, int inferenceTime)
{
    const float kThreshold = (float)DETECTION_TRESHOLD / 100;
    result_t topResults[NUM_RESULTS];
    static result_t lastTopResult = {.score = 0.0, .index = -1};
    bool print = false;
    static int lastPrintTime = 0;
    const int kUsInSecond = 1000000;

    /* Find the best label candidates. */
    MODEL_GetTopN(data, dims->data[dims->size - 1], type, NUM_RESULTS, kThreshold, topResults);

    auto result = topResults[0];
    if (result.index >= 0 && result.score > kThreshold)
    {
        print = lastTopResult.index != result.index;
        lastTopResult = result;
    }

    int time = TIMER_GetTimeInUS();
    if (print)
    {
        int index = result.index;
        const char* label = index >= 0 ? labels[index] : "No word detected";
        int score = (int)(result.score * 100);

        printf("----------------------------------------" EOL);
        printf("     Inference time: %d ms" EOL, inferenceTime / 1000);
        printf("     Detected: %-10s (%d%%)\r\n", label, score);
        printf("----------------------------------------" EOL EOL);

        lastPrintTime = time;
    }

    if ((time - lastPrintTime) > kUsInSecond)
    {
    	lastTopResult = {.score = 0.0, .index = -1};
    }

    return kStatus_Success;
}
