/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "demo_config.h"
#include "model.h"

#include "output_postproc.h"

void MODEL_ProcessOutput(const uint8_t *dataOut,
                         const uint8_t *dataIn,
                         const tensor_dims_t *dims,
                         tensor_type_t type,
                         int inferenceTime,
                         bool *skip)
{
    if (*skip)
        return;

    /* Compute error */
    float err = 0.0;
    float f1;
    float f2;

    for (int i = 0; i < PATCH_SIZE * NUM_CHANELS; i++)
    {
        if (type == kTensorType_UINT8)
        {
            f1 = static_cast<float>(dataIn[i]);
            f2 = static_cast<float>(dataOut[i]);
        }
        else
        {
            f1 = reinterpret_cast<const float *>(dataIn)[i];
            f2 = reinterpret_cast<const float *>(dataOut)[i];
        }
        err += pow(f1 - f2, 2);
    }
    err /= (PATCH_SIZE * NUM_CHANELS);

    if (err > DETECTION_TRESHOLD)
        std::cout << " anomaly detected!!!" << EOL;

    std::cout << "(" << static_cast<int>(inferenceTime) << " us) " << static_cast<int>(err) << "."
              << (static_cast<int>(err * 100)) % 100 << EOL;
}
