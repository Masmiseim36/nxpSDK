/*
 * Copyright 2022-2024 NXP.
 * All rights reserved.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * @brief csi_mt9m114 camera module HAL camera driver implementation.
 */

#include "mpp_config.h"
#include "hal_camera_dev.h"
#include "hal_debug.h"

#if (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_CsiMt9m114 == 1)
#include <stdlib.h>

#include "board.h"
#include "fsl_debug_console.h"
#include "camera_support.h"
#include "hal.h"
#include "hal_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif


hal_camera_status_t HAL_CameraDev_CsiMt9m114_Enqueue(const camera_dev_t *dev, void *data);
void HAL_CameraDev_CsiMt9m114_ReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData);
void BOARD_Camera_I2C_Init(void);
status_t BOARD_Camera_I2C_Send(uint8_t deviceAddress, uint32_t subAddress,
                                 uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_Receive(uint8_t deviceAddress, uint32_t subAddress,
                                    uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(__cplusplus)
}
#endif

#define CAMERA_NAME "CsiMt9m114"

#define CAMERA_DEV_CsiMt9m114_BUFFER_COUNT 3
#define CAMERA_DEV_CsiMt9m114_MAX_HEIGHT 272    /* set camera max resolution equal to display max */
#define CAMERA_DEV_CsiMt9m114_MAX_WIDTH 480    /* set camera max resolution equal to display max */
#define CAMERA_DEV_CsiMt9m114_MAX_BPP 2             /* max value for YUV422/RGB565 */
#define CAMERA_DEV_CsiMt9m114_ROTATE ROTATE_0
#define CAMERA_DEV_CsiMt9m114_FLIP FLIP_NONE
#define CAMERA_DEV_CsiMt9m114_SWAPBYTE 0


#define CAMERA_DEV_BUFFER_ALIGN 64

/* TODO support aligned heap memory */
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_cameraBuffer[CAMERA_DEV_CsiMt9m114_BUFFER_COUNT][CAMERA_DEV_CsiMt9m114_MAX_HEIGHT]
                                 [CAMERA_DEV_CsiMt9m114_MAX_WIDTH * CAMERA_DEV_CsiMt9m114_MAX_BPP],
    CAMERA_DEV_BUFFER_ALIGN);

#define CAMERA_DEV_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* defined in eiq_camera_conf.c */
extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;

/*******************************************************************************
 * Code
 ******************************************************************************/
hal_camera_status_t HAL_CameraDev_CsiMt9m114_Init(
    camera_dev_t *dev, mpp_camera_params_t *config, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_CsiMt9m114_Init( param[%p]\n", param);

    dev->config.width = config->width;
    dev->config.height = config->height;
    dev->config.framerate = config->fps;
    dev->config.format = config->format;
    dev->cap.callback = callback;
    dev->cap.param    = param;

    camera_config_t cameraConfig;

    memset(&cameraConfig, 0, sizeof(cameraConfig));

    BOARD_InitCameraResource();

    switch(config->format) {
    /* Only support RGB565 and YUV422 */
    case MPP_PIXEL_UYVY1P422:
        /* YUV422 format */
        cameraConfig.pixelFormat   = kVIDEO_PixelFormatYUYV;
        cameraConfig.bytesPerPixel = 2;
        break;
    case MPP_PIXEL_RGB565:
        /* RGB565 format */
        cameraConfig.pixelFormat   = kVIDEO_PixelFormatRGB565;
        cameraConfig.bytesPerPixel = 2;
        break;
    default:
        HAL_LOGE("Camera format unsupported\n");
        return kStatus_HAL_CameraError;
    }

    dev->config.pitch = config->width * cameraConfig.bytesPerPixel;
    cameraConfig.resolution    = FSL_VIDEO_RESOLUTION(config->width, config->height);
    cameraConfig.frameBufferLinePitch_Bytes = dev->config.pitch;
    cameraConfig.interface                  = kCAMERA_InterfaceGatedClock;
    cameraConfig.controlFlags               = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec                = config->fps;

    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig,
			 HAL_CameraDev_CsiMt9m114_ReceiverCallback, NULL);

    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    /* Submit the empty frame buffers to buffer queue. */
    for (uint32_t i = 0; i < CAMERA_DEV_CsiMt9m114_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_cameraBuffer[i]));
    }

    HAL_LOGD("--HAL_CameraDev_CsiMt9m114_Init\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiMt9m114_Getbufdesc(const camera_dev_t *dev, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_CsiMt9m114_Getbufdesc(out_buf=[%p])\n", out_buf);
    do
    {
        if ((out_buf == NULL) || (policy == NULL))
        {
            HAL_LOGE("\nNULL pointer to buffer descriptor\n");
            ret = kStatus_HAL_CameraError;
            break;
        }
        /* set memory policy */
        *policy = HAL_MEM_ALLOC_OUTPUT;
        out_buf->alignment = CAMERA_DEV_BUFFER_ALIGN;
        out_buf->cacheable = false;
        out_buf->stride = dev->config.pitch;
    } while (false);
    HAL_LOGD("--HAL_CameraDev_CsiMt9m114_Getbufdesc\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiMt9m114_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

static uint32_t gCurrentBufferAddr = 0;

hal_camera_status_t HAL_CameraDev_CsiMt9m114_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++HAL_CameraDev_CsiMt9m114_Start\n");

    /*
     * Since 2 frame buffers are expected in CSI's queue before starting, it is
     * necessary to submit at least one frame
     */
    if (gCurrentBufferAddr != 0)
    {
        HAL_LOGD("Submitting empty buffer\n");
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)gCurrentBufferAddr);
        gCurrentBufferAddr = 0;
    }

    status = CAMERA_RECEIVER_Start(&cameraReceiver);
    if (status != kStatus_Success) {
        HAL_LOGE("Error with status=%d\n", (int)status);
        ret = kStatus_HAL_CameraError;
    }
    HAL_LOGD("--HAL_CameraDev_CsiMt9m114_Start\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiMt9m114_Stop(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++\n");
    status = CAMERA_RECEIVER_Stop(&cameraReceiver);
    if (status != kStatus_Success) {
        HAL_LOGE("Error with status=%d\n", (int)status);
        ret = kStatus_HAL_CameraError;
    }
    HAL_LOGD("--\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiMt9m114_Dequeue(const camera_dev_t *dev, void **data, int *stripe)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_CsiMt9m114_Dequeue\n");
    // give back previous frame buffer
    if (gCurrentBufferAddr != 0)
    {
        HAL_LOGD("Submitting empty buffer\n");
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)gCurrentBufferAddr);
        gCurrentBufferAddr = 0;
    }

    // get new frame buffer
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &gCurrentBufferAddr))
    {
    }

    *data   = (void *)gCurrentBufferAddr;
    *stripe = 0;
    HAL_LOGD("--HAL_CameraDev_CsiMt9m114_Dequeue\n");
    return ret;
}

const static camera_dev_operator_t camera_dev_csi_mt9m114_ops = {
    .init        = HAL_CameraDev_CsiMt9m114_Init,
    .deinit      = HAL_CameraDev_CsiMt9m114_Deinit,
    .start       = HAL_CameraDev_CsiMt9m114_Start,
    .stop        = HAL_CameraDev_CsiMt9m114_Stop,
    .enqueue     = HAL_CameraDev_CsiMt9m114_Enqueue,
    .dequeue     = HAL_CameraDev_CsiMt9m114_Dequeue,
    .get_buf_desc = HAL_CameraDev_CsiMt9m114_Getbufdesc,
};

static camera_dev_t camera_dev_csi_mt9m114 = {
    .id   = 1,
    .name = CAMERA_NAME,
    .ops  = &camera_dev_csi_mt9m114_ops,
    .config =
        {
            .rotate   = CAMERA_DEV_CsiMt9m114_ROTATE,
            .flip     = CAMERA_DEV_CsiMt9m114_FLIP,
            .swapByte = CAMERA_DEV_CsiMt9m114_SWAPBYTE,
        },
    .cap = {.callback = NULL, .param = NULL},
};

void HAL_CameraDev_CsiMt9m114_ReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData)
{
    if (camera_dev_csi_mt9m114.cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        camera_dev_csi_mt9m114.cap.callback(&camera_dev_csi_mt9m114, kCameraEvent_SendFrame,
                                            camera_dev_csi_mt9m114.cap.param, fromISR);
    }
}

hal_camera_status_t HAL_CameraDev_CsiMt9m114_Enqueue(const camera_dev_t *dev, void *data)
{
    int error = 0;
    HAL_LOGD("++HAL_CameraDev_CsiMt9m114_Enqueue\n");
    /* nothing to do, see HAL_CameraDev_CsiMt9m114_Dequeue() */
    HAL_LOGD("--HAL_CameraDev_CsiMt9m114_Enqueue\n");
    return error;
}


int HAL_CameraDev_CsiMt9m114_setup(const char *name, camera_dev_t *dev, _Bool defconfig)
{
    dev->ops = &camera_dev_csi_mt9m114_ops;

    return 0;
}
#else /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_CsiMt9m114 == 1) */
int HAL_CameraDev_CsiMt9m114_setup(const char *name, camera_dev_t *dev, _Bool defconfig)
{
    HAL_LOGE("Camera CsiMt9m114 not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_CsiMt9m114 == 1) */
