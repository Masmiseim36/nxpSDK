/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>

#include "demo_info.h"
#include "demo_config.h"
#include "audio.h"
#include "model.h"

void DEMO_PrintInfo()
{
    std::cout << "Keyword spotting example using a TensorFlow Lite model." << EOL
        << "Detection threshold: " << DETECTION_TRESHOLD << EOL;
}
