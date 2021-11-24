/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief SLN performance measurement implementation. Used to capture performance statistics.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"
#include "fwk_perf.h"
#include "fwk_common.h"

#if FWK_PERF

typedef struct _fwk_fps_data
{
    unsigned int startTime;
    unsigned int frames;
    float fps;
} fwk_fps_data_t;

#define FWK_FPS_COUNT (MAXIMUM_CAMERA_DEV + MAXIMUM_DISPLAY_DEV + MAXIMUM_VISION_ALGO_DEV)

static fwk_fps_data_t s_FpsData[FWK_FPS_COUNT];

static int _fwk_fps_id(fwk_fps_type_t type, int id)
{
    int fps_id = FWK_FPS_COUNT;

    switch (type)
    {
        case kFWKFPSType_Camera:
        {
            if (id < MAXIMUM_CAMERA_DEV)
            {
                fps_id = id;
            }
        }
        break;

        case kFWKFPSType_Display:
        {
            if (id < MAXIMUM_DISPLAY_DEV)
            {
                fps_id = MAXIMUM_CAMERA_DEV + id;
            }
        }
        break;

        case kFWKFPSType_VAlgo:
        {
            if (id < MAXIMUM_VISION_ALGO_DEV)
            {
                fps_id = MAXIMUM_CAMERA_DEV + MAXIMUM_DISPLAY_DEV + id;
            }
        }
        break;

        default:
            break;
    }

    return fps_id;
}

float fwk_fps(fwk_fps_type_t type, int id)
{
    float fps = 0.0f;

    unsigned int time_current = 0;

    int fps_id = _fwk_fps_id(type, id);

    if (fps_id >= FWK_FPS_COUNT)
    {
        LOGE("Invalid fps type \"%d\" and id \"%d\"", type, id);
        return fps;
    }

    if (s_FpsData[fps_id].startTime == 0)
    {
        s_FpsData[fps_id].startTime = sln_current_time_us();
    }

    s_FpsData[fps_id].frames++;

    time_current = sln_current_time_us();
    int diff     = abs(time_current - s_FpsData[fps_id].startTime);

    if (diff >= 1000000)
    {
        s_FpsData[fps_id].fps = s_FpsData[fps_id].frames * 1000000.0f / diff;
        LOGI("FPS[%d]:[%d]:%.1f %d", type, id, s_FpsData[fps_id].fps, s_FpsData[fps_id].frames);
        s_FpsData[fps_id].frames    = 0;
        s_FpsData[fps_id].startTime = time_current;
    }

    fps = s_FpsData[fps_id].fps;

    return fps;
}

float fwk_get_fps(fwk_fps_type_t type, int id)
{
    float fps = 0.0f;

    int fps_id = _fwk_fps_id(type, id);

    if (fps_id >= FWK_FPS_COUNT)
    {
        LOGE("Invalid FPS type \"%d\" and id \"%d\"", type, id);
        return fps;
    }

    fps = s_FpsData[fps_id].fps;

    return fps;
}
#endif
