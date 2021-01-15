/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "demo_config.h"
#include "image.h"
#include "image_utils.h"
#include "image_data.h"
#include "eiq_video_worker.h"

const char* s_imageName = STATIC_IMAGE_NAME;
EIQ_VideoWorker_t* s_worker = NULL;
uint8_t* s_captureBuffer = NULL;
Dims_t s_captureBufferDims;
bool s_static = true;

status_t IMAGE_GetImage(uint8_t* dstData, int32_t dstWidth, int32_t dstHeight, int32_t dstChannels)
{
    /* Switch to camera image capture after one static image. */
    if (s_static && !s_worker)
    {
        s_worker = EIQ_VideoWorkerInit();
        s_captureBuffer = s_worker->base.getData();
        s_captureBufferDims = s_worker->base.getResolution();
        s_worker->base.start();

        printf(EOL "Static data processing:" EOL);
        return IMAGE_Decode(image_data, dstData, dstWidth, dstHeight, dstChannels);
    }
    else
    {
        if (s_static)
        {
            printf(EOL "Camera data processing:" EOL);
            s_static = false;
        }
        while (!s_worker->base.isReady()) {}
        printf("Data for inference are ready" EOL);

        IMAGE_Resize(s_captureBuffer, s_captureBufferDims.width, s_captureBufferDims.height,
                     dstData, dstWidth, dstHeight, dstChannels);
    }
    return kStatus_Success;
}

const char* IMAGE_GetImageName()
{
    return s_imageName;
}
