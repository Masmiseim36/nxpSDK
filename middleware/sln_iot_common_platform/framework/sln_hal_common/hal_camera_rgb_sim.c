/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief camera dev RGB simulator HAL driver implementation.
 */

#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <time.h>

#include "fwk_log.h"
#include "fwk_camera_manager.h"
#include "hal_camera_dev.h"
#include "hal_sln_timer.h"

#define CAMERA_NAME             "rgb_sim"
#define CAMERA_RGB_PIXEL_FORMAT kPixelFormat_YUV1P444_RGB
#define CAMERA_WIDTH            640
#define CAMERA_HEIGHT           480
#define CAMERA_BYTES_PER_PIXEL  4
#define CAMERA_BUFFER_COUNT     4
#define CAMERA_VSYNC_TIME       30

static sln_timer_t *s_VsyncTimer;

// make sure the address is align with the address where frames bin file had been loaded
// please refer to doc/camera_simulator/HOW_TO_SIMULATE_YUV_CAMERA_FOR_YOUR_RT1170.MD
#define SIMULATOR_FRAMES_FLASH_ADDRESS 0x30800000

#define SIM_FRAME_COUNT 5

static unsigned char s_Frames[SIM_FRAME_COUNT][CAMERA_WIDTH * CAMERA_HEIGHT * CAMERA_BYTES_PER_PIXEL];
#define FRAME_REPEAT_COUNT 5

static int s_FrameIndex = 0;

static void _CameraReceiverCallback(void *arg)
{
    camera_dev_t *dev = (camera_dev_t *)arg;

    s_FrameIndex++;
    if (s_FrameIndex == SIM_FRAME_COUNT * FRAME_REPEAT_COUNT)
    {
        s_FrameIndex = 0;
    }

    LOGI("RGB:%d", s_FrameIndex);

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev, kCameraEvent_SendFrame, dev->cap.param, fromISR);
    }
}

static hal_camera_status_t _CameraDevRGBSimInit(camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    dev->cap.callback = callback;
    dev->cap.param    = param;

    memcpy(s_Frames, SIMULATOR_FRAMES_FLASH_ADDRESS,
           CAMERA_WIDTH * CAMERA_HEIGHT * CAMERA_BYTES_PER_PIXEL * SIM_FRAME_COUNT);

    return ret;
}

static hal_camera_status_t _CameraDevRGBSimDeinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

static hal_camera_status_t _CameraDevRGBSimStart(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGI("_CameraDevRGBSimStart");

    if (sln_timer_start("CameraDevRGBSim", CAMERA_VSYNC_TIME, 1, _CameraReceiverCallback, (void *)dev, &s_VsyncTimer))
    {
        LOGE("Failed to start timer \"CameraDevRGBSim\"");
        ret = kStatus_HAL_CameraError;
    }

    LOGI("_CameraDevRGBSimStart");
    return ret;
}

static hal_camera_status_t _CameraDevRGBSimEnqueue(const camera_dev_t *dev, void *data)
{
    LOGI("++_CameraDevRGBSimStart");
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    LOGI("--_CameraDevRGBSimEnqueue");
    return ret;
}

static hal_camera_status_t _CameraDevRGBSimDequeue(const camera_dev_t *dev, void **data, pixel_format_t *format)
{
    LOGI("++_CameraDevRGBSimDequeue: %d", s_FrameIndex);

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    *data   = (void *)s_Frames[s_FrameIndex / FRAME_REPEAT_COUNT];
    *format = CAMERA_RGB_PIXEL_FORMAT;

    LOGI("--_CameraDevRGBSimDequeue");
    return ret;
}

const static camera_dev_operator_t s_CameraDev_RGBSimOps = {
    .init        = _CameraDevRGBSimInit,
    .deinit      = _CameraDevRGBSimDeinit,
    .start       = _CameraDevRGBSimStart,
    .enqueue     = _CameraDevRGBSimEnqueue,
    .dequeue     = _CameraDevRGBSimDequeue,
    .inputNotify = NULL,
};

static camera_dev_t s_CameraDev_RGBSim = {
    .id   = 0,
    .name = CAMERA_NAME,
    .ops  = &s_CameraDev_RGBSimOps,
    .config =
        {
            .height   = CAMERA_HEIGHT,
            .width    = CAMERA_WIDTH,
            .pitch    = CAMERA_WIDTH * CAMERA_BYTES_PER_PIXEL,
            .left     = 0,
            .top      = 0,
            .right    = CAMERA_WIDTH - 1,
            .bottom   = CAMERA_HEIGHT - 1,
            .rotate   = kCWRotateDegree_90,
            .flip     = kFlipMode_None,
            .swapByte = 0,
        },
    .cap =
        {

            .callback = NULL,
            .param    = NULL,
        },
};

int SLN_CameraDevRGBSimRegister()
{
    int error = 0;
    LOGD("HAL_CameraDev_3DSim_Register");
    error = FWK_CameraManager_DeviceRegister(&s_CameraDev_RGBSim);
    return error;
}
