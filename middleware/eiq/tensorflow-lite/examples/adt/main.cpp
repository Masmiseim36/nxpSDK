/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>
#include <algorithm>

#include "board_init.h"
#include "demo_config.h"
#include "demo_info.h"
#include "model.h"
#include "sensor.h"
#include "output_postproc.h"
#include "timer.h"

int main(void)
{
    BOARD_Init();
    TIMER_Init();

    bool skipOutput;
    status_t status;
#if DATA_COLLECT == true
    CHECK_STATUS(status, SENSOR_CollectData(), "Failed retrieving sensors data");
#endif

    DEMO_PrintInfo();

    CHECK_STATUS(status, MODEL_Init(), "Inference failed.");

    tensor_dims_t inputDims;
    tensor_type_t inputType;
    uint8_t *inputData = MODEL_GetInputTensorData(&inputDims, &inputType);

    tensor_dims_t outputDims;
    tensor_type_t outputType;
    uint8_t *outputData = MODEL_GetOutputTensorData(&outputDims, &outputType);

#ifdef MICRO_BUFF
    /* Buffer for storing data from input tensor. */
    uint8_t inputDataBuffer[PATCH_SIZE * NUM_CHANELS * 4];
    std::fill_n(inputDataBuffer, PATCH_SIZE * NUM_CHANELS * 4, 0);
#endif

    while (1)
    {
        /* Expected tensor dimensions: [batches, height, width, channels] */
        CHECK_STATUS(status, SENSOR_GetData(inputData, inputType, &skipOutput), "Failed retrieving sensors data");

        auto startTime = TIMER_GetTimeInUS();
#ifdef MICRO_BUFF
        /* Input tensor share memory with output tensor.   */
        /* Data must be copied before inference is called. */
        std::copy_n(inputData, PATCH_SIZE * NUM_CHANELS * 4, inputDataBuffer);
        inputData = inputDataBuffer;
#endif

        MODEL_RunInference();
        auto endTime = TIMER_GetTimeInUS();

        MODEL_ProcessOutput(outputData, inputData, &outputDims, outputType, endTime - startTime, &skipOutput);
    }
}
