/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief camera dev 3D simulator HAL driver implementation.
 */

#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <time.h>

#include "fwk_log.h"
#include "fwk_camera_manager.h"
#include "hal_camera_dev.h"
#include "hal_sln_timer.h"

#include "hal_camera_3d_sim_ir_frame.h"
#include "hal_camera_3d_sim_depth_frame.h"

#define CAMERA_NAME               "3d_sim"
#define CAMERA_IR_PIXEL_FORMAT    kPixelFormat_Gray16
#define CAMERA_DEPTH_PIXEL_FORMAT kPixelFormat_Depth16
#define CAMERA_WIDTH              540
#define CAMERA_HEIGHT             640
#define CAMERA_BYTES_PER_PIXEL    2
#define CAMERA_BUFFER_COUNT       4
#define CAMERA_VSYNC_TIME         30
#define SIM_FRAME_COUNT           2

static sln_timer_t *s_pVsyncTimer;

static unsigned char s_Frames[SIM_FRAME_COUNT][CAMERA_WIDTH * CAMERA_HEIGHT * CAMERA_BYTES_PER_PIXEL];

static int s_FrameIndex = 0;

static void HAL_CameraDev_3DSim_ReceiverCallback(void *arg)
{
    camera_dev_t *dev = (camera_dev_t *)arg;

    s_FrameIndex++;
    if (s_FrameIndex == SIM_FRAME_COUNT)
    {
        s_FrameIndex = 0;
    }

    LOGI("3D:%d", s_FrameIndex);

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev, kCameraEvent_SendFrame, dev->cap.param, fromISR);
    }
}

static hal_camera_status_t HAL_CameraDev_3DSim_Init(
    camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    dev->cap.callback = callback;
    dev->cap.param    = param;

    for (int i = 0; i < SIM_FRAME_COUNT; i++)
    {
        if (i % 2)
        {
            /* IR frame 0, 2, 4 */
            memcpy((void *)s_Frames[i], s_RAW16_540_640_IR_FRAME,
                   CAMERA_WIDTH * CAMERA_HEIGHT * CAMERA_BYTES_PER_PIXEL);
        }
        else
        {
            /* depth  frame 1, 3, 5 */
            memcpy((void *)s_Frames[i], s_RAW16_540_640_DEPTH_FRAME,
                   CAMERA_WIDTH * CAMERA_HEIGHT * CAMERA_BYTES_PER_PIXEL);
        }
    }

    return ret;
}

static hal_camera_status_t HAL_CameraDev_3DSim_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

static hal_camera_status_t HAL_CameraDev_3DSim_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    int status;
    LOGI("HAL_CameraDev_3DSim_Start");

    status = sln_timer_start("CameraDev3DSim", CAMERA_VSYNC_TIME, 1, HAL_CameraDev_3DSim_ReceiverCallback, (void *)dev,
                            &s_pVsyncTimer);

    if (status)
    {
        LOGE("Failed to start timer \"CameraDev3DSim\"");
        ret = kStatus_HAL_CameraError;
    }

    LOGI("HAL_CameraDev_3DSim_Start");
    return ret;
}

static hal_camera_status_t HAL_CameraDev_3DSim_Enqueue(const camera_dev_t *dev, void *data)
{
    LOGI("++HAL_CameraDev_3DSim_Start");
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    LOGI("--HAL_CameraDev_3DSim_Enqueue");
    return ret;
}

static hal_camera_status_t HAL_CameraDev_3DSim_Dequeue(const camera_dev_t *dev, void **data, pixel_format_t *format)
{
    LOGI("++HAL_CameraDev_3DSim_Dequeue: %d", s_FrameIndex);

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    *data = (void *)s_Frames[s_FrameIndex];

    if (s_FrameIndex % 2)
    {
        /* IR frame 0, 2, 4 */
        LOGI("3D_IR");
        *format = CAMERA_IR_PIXEL_FORMAT;
    }
    else
    {
        /* depth  frame 1, 3, 5 */
        LOGI("3D_DEPTH");
        *format = CAMERA_DEPTH_PIXEL_FORMAT;
    }

    LOGI("--HAL_CameraDev_3DSim_Dequeue");
    return ret;
}

const static camera_dev_operator_t s_CameraDev_3DSimOps = {
    .init        = HAL_CameraDev_3DSim_Init,
    .deinit      = HAL_CameraDev_3DSim_Deinit,
    .start       = HAL_CameraDev_3DSim_Start,
    .enqueue     = HAL_CameraDev_3DSim_Enqueue,
    .dequeue     = HAL_CameraDev_3DSim_Dequeue,
    .inputNotify = NULL,
};

static camera_dev_t s_CameraDev_3DSim = {
    .id   = 0,
    .name = CAMERA_NAME,
    .ops  = &s_CameraDev_3DSimOps,
    .config =
        {
            .height   = CAMERA_HEIGHT,
            .width    = CAMERA_WIDTH,
            .pitch    = CAMERA_WIDTH * CAMERA_BYTES_PER_PIXEL,
            .left     = 0,
            .top      = 0,
            .right    = CAMERA_WIDTH - 1,
            .bottom   = CAMERA_HEIGHT - 1,
            .rotate   = kCWRotateDegree_0,
            .flip     = kFlipMode_None,
            .swapByte = 0,
        },
    .cap =
        {

            .callback = NULL,
            .param    = NULL,
        },
};

int HAL_CameraDev_3DSim_Register()
{
    int error = 0;
    LOGD("HAL_CameraDev_3DSim_Register");
    error = FWK_CameraManager_DeviceRegister(&s_CameraDev_3DSim);
    return error;
}
