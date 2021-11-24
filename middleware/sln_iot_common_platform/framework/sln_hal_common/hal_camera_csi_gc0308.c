/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief camera dev CSI GC0308 HAL driver implementation.
 */
#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <time.h>

#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"

#include "sln_gc0308.h"

#include "board.h"

#include "fwk_log.h"
#include "fwk_camera_manager.h"
#include "hal_camera_dev.h"

#define CAMERA_NAME "CSI_GC0308"
/* the default pixel format will be set to GRAY which is IR camera sensor */
#define CAMERA_PIXEL_FORMAT     kPixelFormat_UYVY1P422_Gray
#define CAMERA_WIDTH            640
#define CAMERA_HEIGHT           480
#define CAMERA_BYTES_PER_PIXEL  2
#define CAMERA_DEV_BUFFER_COUNT 4

#define CAMERA_RGB_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_FrameBuffers[CAMERA_DEV_BUFFER_COUNT][CAMERA_HEIGHT][CAMERA_WIDTH * CAMERA_BYTES_PER_PIXEL], 32);

static uint8_t *s_pCurrentFrameBuffer = NULL;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
void BOARD_PullCSICameraResetPin(bool pullUp);
void BOARD_PullCSICameraPowerDownPin(bool pullUp);
status_t BOARD_CSICameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_CSICameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

static csi_resource_t s_CsiResource = {
    .csiBase = CSI,
};

static csi_private_data_t s_CsiPrivateData;

static camera_receiver_handle_t s_CameraReceiver = {
    .resource    = &s_CsiResource,
    .ops         = &csi_ops,
    .privateData = &s_CsiPrivateData,
};

static gc0308_resource_t gc0308Resource = {
    .i2cSendFunc       = BOARD_CSICameraI2CSend,
    .i2cReceiveFunc    = BOARD_CSICameraI2CReceive,
    .pullResetPin      = BOARD_PullCSICameraResetPin,
    .pullPowerDownPin  = BOARD_PullCSICameraPowerDownPin,
    .inputClockFreq_Hz = 24000000,
};

static camera_device_handle_t cameraDevice = {
    .resource = &gc0308Resource,
    .ops      = &gc0308_ops,
};

static pixel_format_t csi_gc0308_format = CAMERA_PIXEL_FORMAT;

static void _camera_init_interface(void)
{
    CLOCK_EnableClock(kCLOCK_Video_Mux);

    /* CSI Sensor data is from Parallel CSI . */
    VIDEO_MUX->VID_MUX_CTRL.CLR = VIDEO_MUX_VID_MUX_CTRL_CSI_SEL_MASK;
}

static void HAL_CameraDev_CsiGc0308_ReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData)
{
    camera_dev_t *dev = (camera_dev_t *)userData;

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev, kCameraEvent_SendFrame, dev->cap.param, fromISR);
    }
}

hal_camera_status_t HAL_CameraDev_CsiGc0308_InputNotify(const camera_dev_t *dev, void *data)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiGc0308_Init(camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    camera_config_t cameraConfig;
    LOGD("s_CameraDev_CsiGC0308_init");

    dev->config.width  = width;
    dev->config.height = height;
    dev->cap.callback  = callback;
    dev->cap.param     = param;

    // init csi receiver
    memset(&cameraConfig, 0, sizeof(cameraConfig));
    cameraConfig.pixelFormat                = kVIDEO_PixelFormatYUYV;
    cameraConfig.bytesPerPixel              = CAMERA_BYTES_PER_PIXEL;
    cameraConfig.resolution                 = FSL_VIDEO_RESOLUTION(width, height);
    cameraConfig.frameBufferLinePitch_Bytes = width * CAMERA_BYTES_PER_PIXEL;
    cameraConfig.interface                  = kCAMERA_InterfaceGatedClock;
    cameraConfig.controlFlags               = CAMERA_RGB_CONTROL_FLAGS;
    cameraConfig.framePerSec                = 15;

    BOARD_InitCSICameraResource();

    _camera_init_interface();

    NVIC_SetPriority(CSI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    CAMERA_RECEIVER_Init(&s_CameraReceiver, &cameraConfig, HAL_CameraDev_CsiGc0308_ReceiverCallback, dev);

    // init camera dev
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);
    if (csi_gc0308_format == kPixelFormat_UYVY1P422_Gray)
    {
        CAMERA_DEVICE_Control(&cameraDevice, kCAMERA_DeviceMonoMode, CAMERA_MONO_MODE_ENABLED);
    }
    else
    {
        CAMERA_DEVICE_Control(&cameraDevice, kCAMERA_DeviceMonoMode, CAMERA_MONO_MODE_DISABLED);
    }

    for (int i = 0; i < CAMERA_DEV_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_FrameBuffers[i]);
    }

    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiGc0308_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiGc0308_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGD("camera_dev_csi_gc0308_start");
    CAMERA_DEVICE_Start(&cameraDevice);
    CAMERA_RECEIVER_Start(&s_CameraReceiver);

    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiGc0308_Enqueue(const camera_dev_t *dev, void *data)
{
    LOGI("++s_CameraDev_CsiGC0308_enqueue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    if (s_pCurrentFrameBuffer != NULL)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_pCurrentFrameBuffer);
        s_pCurrentFrameBuffer = NULL;
    }

    LOGI("--s_CameraDev_CsiGC0308_enqueue");
    return ret;
}

hal_camera_status_t HAL_CameraDev_CsiGc0308_Dequeue(const camera_dev_t *dev, void **data, pixel_format_t *format)
{
    LOGI("++s_CameraDev_CsiGC0308_dequeue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&s_CameraReceiver, (uint32_t *)&s_pCurrentFrameBuffer))
    {
    }

    *data   = s_pCurrentFrameBuffer;
    *format = csi_gc0308_format;
    LOGI("--s_CameraDev_CsiGC0308_dequeue");
    return ret;
}

const static camera_dev_operator_t s_CameraDev_CsiGC0308Ops = {.init        = HAL_CameraDev_CsiGc0308_Init,
                                                               .deinit      = HAL_CameraDev_CsiGc0308_Deinit,
                                                               .start       = HAL_CameraDev_CsiGc0308_Start,
                                                               .enqueue     = HAL_CameraDev_CsiGc0308_Enqueue,
                                                               .dequeue     = HAL_CameraDev_CsiGc0308_Dequeue,
                                                               .inputNotify = HAL_CameraDev_CsiGc0308_InputNotify};

static camera_dev_t s_CameraDev_CsiGC0308 = {
    .id   = 1,
    .ops  = &s_CameraDev_CsiGC0308Ops,
    .name = CAMERA_NAME,
    .config =
        {
            .height = CAMERA_HEIGHT,
            .width  = CAMERA_WIDTH,
            .pitch  = CAMERA_WIDTH * CAMERA_BYTES_PER_PIXEL,
            .left   = 0,
            .top    = 0,
            .right  = CAMERA_WIDTH - 1,
            .bottom = CAMERA_HEIGHT - 1,
            .rotate = kCWRotateDegree_270,

            .flip     = kFlipMode_None,
            .swapByte = 1,
        },
    .cap =
        {
            .callback = NULL,
            .param    = NULL,
        },
};

int HAL_CameraDev_CsiGc0308_Register(camera_dev_static_config_t *config)
{
    int error = 0;
    LOGD("HAL_CameraDev_CsiGc0308_register");

    memcpy(&(s_CameraDev_CsiGC0308.config), config, sizeof(camera_dev_static_config_t));

    error = FWK_CameraManager_DeviceRegister(&s_CameraDev_CsiGC0308);
    return error;
}
