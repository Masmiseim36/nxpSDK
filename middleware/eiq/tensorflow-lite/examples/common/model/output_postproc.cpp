/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "output_postproc.h"
#include "get_top_n.h"
#include "demo_config.h"
#include "labels.h"

status_t MODEL_ProcessOutput(const uint8_t* data, const tensor_dims_t* dims,
                             tensor_type_t type, int inferenceTime)
{
    const float threshold = (float)DETECTION_TRESHOLD / 100;
    std::vector<std::pair<float, int>> topResults;
    std::string label = "No label detected";

    /* Find best label candidates. */
    MODEL_GetTopN(data, dims->data[dims->size - 1], type, NUM_RESULTS, threshold, &topResults);

    float confidence = 0;
    if (!topResults.empty())
    {
        auto result = topResults.front();
        confidence = result.first;
        int index = result.second;
        if (confidence * 100 > DETECTION_TRESHOLD)
        {
            label = labels[index];
        }
    }

    std::cout << "----------------------------------------\r\n"
              << "     Inference time: " << inferenceTime / 1000 << " ms\r\n"
              << "     Detected: " << std::setw(10) << label
              << " (" << (int)(confidence * 100) << "%)\r\n"
              << "----------------------------------------\r\n\r\n";

    return kStatus_Success;
}
