/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "demo_config.h"
#include "audio.h"
#include "audio_data.h"
#include "eiq_audio_worker.h"
#include "kws_mfcc.hpp"

EIQ_AudioWorker_t* s_worker = NULL;
int s_staticCount = 0;
/* (recording_win x frame_shift) is the actual recording window size. */
KWS_MFCC s_kws(NUM_FRAMES);

status_t AUDIO_GetSpectralSample(uint8_t* dstData, int size)
{
    s_staticCount++;
    /* Switch to microphone audio capture after two static samples. */
    if (s_staticCount == 1)
    {
        printf(EOL "Static data processing:" EOL);
        AUDIO_PreprocessSample(dstData, off_sample_data, size);
    }
    else if (s_staticCount == 2 && !s_worker)
    {
        AUDIO_PreprocessSample(dstData, right_sample_data, size);

        s_worker = EIQ_AudioWorkerInit();
        s_worker->base.start();
    }
    else
    {
        if (s_staticCount == 3)
        {
            printf(EOL "Microphone data processing:" EOL);
        }
        while (!s_worker->base.isReady()) {}
        printf("Data for inference are ready" EOL);

        AUDIO_PreprocessSample(dstData, reinterpret_cast<int16_t*>(s_worker->base.getData()), size);
    }
    return kStatus_Success;
}

void AUDIO_PreprocessSample(uint8_t* dstData, const int16_t* srcData, int size)
{
    s_kws.audio_buffer = srcData;
    s_kws.extract_features();

    /* Experimental min, max range for quantization of the input.*/
    float min = -247.0;
    float max = 30.0;
    for (int i = 0; i < size; i++)
    {
        dstData[i] = (uint8_t)round((255.0 * (s_kws.mfcc_buffer[i] - min)) / (max - min));
    }
}

const char* AUDIO_GetSampleName()
{
    switch (s_staticCount)
    {
        case 1:
            return OFF_SAMPLE_NAME;
        case 2:
            return RIGHT_SAMPLE_NAME;
    }
    return NULL;
}
