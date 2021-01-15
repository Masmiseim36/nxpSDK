/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>

#include "demo_info.h"
#include "demo_config.h"
#include "image.h"
#include "model.h"

void DEMO_PrintInfo()
{
    std::cout << "Label image object recognition example using a TensorFlow Lite model." << EOL
        << "Detection threshold: " << DETECTION_TRESHOLD << "%" << EOL
        << "Expected category: " << IMAGE_GetImageName() << EOL
        << "Model: " << MODEL_GetModelName() << EOL;
}
