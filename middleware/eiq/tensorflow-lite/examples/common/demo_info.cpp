/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "demo_info.h"
#include "model.h"

void DEMO_PrintInfo(void)
{
    printf("%s example using a %s model." EOL, EXAMPLE_NAME, FRAMEWORK_NAME);
    printf("Detection threshold: %d%%" EOL, DETECTION_TRESHOLD);
    printf("Model: %s" EOL, MODEL_GetModelName());
}
