/*
 * Copyright 2023 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @brief Ezh+Ov7670 camera module HAL camera driver implementation.
 */

#include "mpp_config.h"
#include "hal_camera_dev.h"
#include "hal_debug.h"

#if (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_EzhOv7670 == 1)
#include <stdlib.h>

#include "board.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_gpio.h"
#include "fsl_ov7670.h"
#include "fsl_smartdma.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_inputmux_connections.h"
#include "fsl_inputmux.h"
#include "hal.h"
#include "hal_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAMERA_NAME "EzhOv7670"

#define CAMERA_DEV_EzhOv7670_FPS 30   /* camera frame capture rate */
#define CAMERA_DEV_EzhOv7670_BPP 2   /* camera bytes per pixels for RGB565 */
#define EZH_STACK_SIZE 64   /* stack size for EZH, see smart_dma driver recommendation */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

void BOARD_Camera_I2C_Init(void);
status_t BOARD_Camera_I2C_Send(uint8_t deviceAddress, uint32_t subAddress,
                                 uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_Receive(uint8_t deviceAddress, uint32_t subAddress,
                                    uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint8_t g_smartdma_stack[EZH_STACK_SIZE];
static volatile int g_data_ready;   /* non-zero when new data signaled by SmartDMA IRQ */
static uint8_t *g_camera_buffer;   /* destination pixel buffer for camera */
static camera_device_handle_t g_camdev = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void ezh_camera_callback(void *param){
   g_data_ready=1;
}

hal_camera_status_t HAL_CameraDev_EzhOv7670_Init(
    camera_dev_t *dev, mpp_camera_params_t *config, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_EzhOv7670_Init( param[%p]\n", param);

    ov7670_resource_t camres = {0};
    camera_config_t camconfig = {0};
    smartdma_param_t smartdmaParam; /* SMARTDMA parameters. */

    /* init camera and pinmux */
    g_camdev.ops = &ov7670_ops;
    camres.xclock = kOV7670_InputClock12MHZ;
    camres.i2cReceiveFunc = BOARD_Camera_I2C_ReceiveSCCB;
    camres.i2cSendFunc = BOARD_Camera_I2C_SendSCCB;
    g_camdev.resource = &camres;
    camconfig.bytesPerPixel = CAMERA_DEV_EzhOv7670_BPP;
    camconfig.framePerSec = CAMERA_DEV_EzhOv7670_FPS;
    camconfig.pixelFormat = kVIDEO_PixelFormatRGB565;
    camconfig.interface = kCAMERA_InterfaceGatedClock;

    if ((config->width == 320) && (config->height == 240))
    {
        camconfig.resolution = kVIDEO_ResolutionQVGA;
    }
    else if ((config->width == 160) && (config->height == 120))
    {
        camconfig.resolution = kVIDEO_ResolutionQQVGA;
    }
    else
    {
        HAL_LOGE("Camera resolution unsupported\n");
        return kStatus_HAL_CameraError;
    }

    if (config->format != MPP_PIXEL_RGB565)
    {
        HAL_LOGE("Camera format unsupported\n");
        return kStatus_HAL_CameraError;
    }

    /* init ov7670 module */
    CAMERA_DEVICE_Init(&g_camdev, &camconfig);

    /* init SmartDMA */
    g_data_ready=0;
    SMARTDMA_InitWithoutFirmware();
    SMARTDMA_InstallFirmware(SMARTDMA_CAMERA_MEM_ADDR,s_smartdmaCameraFirmware,
                             SMARTDMA_CAMERA_FIRMWARE_SIZE);
    SMARTDMA_InstallCallback(ezh_camera_callback, NULL);
    NVIC_EnableIRQ(SMARTDMA_IRQn);
    NVIC_SetPriority(SMARTDMA_IRQn, 3);

    smartdmaParam.cameraParam.smartdma_stack = (uint32_t*)g_smartdma_stack;
    /* allocate Pixels buffer */
    g_camera_buffer = (uint8_t *) MPP_MALLOC(config->width * camconfig.bytesPerPixel * config->height);
    if (g_camera_buffer == NULL)
    {
        HAL_LOGE("Malloc camera buffer failed \n");
        return kStatus_HAL_CameraError;
    }

    smartdmaParam.cameraParam.p_buffer = (uint32_t*)g_camera_buffer;
    SMARTDMA_Boot1(kSMARTDMA_FlexIO_CameraWholeFrame, &smartdmaParam, 0x2);

    /* save config */
    dev->config.width = config->width;
    dev->config.height = config->height;
    dev->config.framerate = config->fps;
    dev->config.format = config->format;
    dev->cap.callback = callback;
    dev->cap.param    = param;
    dev->config.pitch = config->width * camconfig.bytesPerPixel;
    strncpy(dev->name, CAMERA_NAME, HAL_DEVICE_NAME_MAX_LENGTH);

    HAL_LOGD("--HAL_CameraDev_EzhOv7670_Init\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhOv7670_Getbufdesc(const camera_dev_t *dev, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_EzhOv7670_Getbufdesc(out_buf=[%p])\n", out_buf);

    if ((out_buf == NULL) || (policy == NULL))
    {
        HAL_LOGE("\nNULL pointer to buffer descriptor\n");
        return kStatus_HAL_CameraError;
    }
    /* set memory policy */
    *policy = HAL_MEM_ALLOC_OUTPUT;
    out_buf->alignment = 0;
    out_buf->cacheable = true;
    out_buf->stride = dev->config.pitch;
    out_buf->nb_lines = dev->config.height;
    out_buf->addr = g_camera_buffer;

    HAL_LOGD("--HAL_CameraDev_EzhOv7670_Getbufdesc\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhOv7670_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;

    SMARTDMA_Deinit();
    MPP_FREE(g_camera_buffer);
    status = CAMERA_DEVICE_Deinit(&g_camdev);
    if (status != kStatus_Success) return kStatus_HAL_CameraError;

    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhOv7670_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++HAL_CameraDev_EzhOv7670_Start\n");

    status = CAMERA_DEVICE_Start(&g_camdev);
    if (status != kStatus_Success) return kStatus_HAL_CameraError;

    HAL_LOGD("--HAL_CameraDev_EzhOv7670_Start\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhOv7670_Stop(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++\nHAL_CameraDev_EzhOv7670_Stop");

    status = CAMERA_DEVICE_Stop(&g_camdev);
    if (status != kStatus_Success) return kStatus_HAL_CameraError;

    HAL_LOGD("--\nHAL_CameraDev_EzhOv7670_Stop");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhOv7670_Dequeue(const camera_dev_t *dev, void **data, mpp_pixel_format_t *format)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_EzhOv7670_Dequeue\n");

    // wait for new frame buffer
    while (g_data_ready == 0);
    g_data_ready = 0;

    *data   = (void *)g_camera_buffer;
    *format = dev->config.format;
    HAL_LOGD("--HAL_CameraDev_EzhOv7670_Dequeue\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhOv7670_Enqueue(const camera_dev_t *dev, void *data)
{
    int error = 0;
    HAL_LOGD("++HAL_CameraDev_EzhOv7670_Enqueue\n");
    /* nothing to do, see HAL_CameraDev_EzhOv7670_Dequeue() */
    HAL_LOGD("--HAL_CameraDev_EzhOv7670_Enqueue\n");
    return error;
}

const static camera_dev_operator_t camera_dev_ezh_ov7670_ops = {
    .init        = HAL_CameraDev_EzhOv7670_Init,
    .deinit      = HAL_CameraDev_EzhOv7670_Deinit,
    .start       = HAL_CameraDev_EzhOv7670_Start,
    .stop        = HAL_CameraDev_EzhOv7670_Stop,
    .enqueue     = HAL_CameraDev_EzhOv7670_Enqueue,
    .dequeue     = HAL_CameraDev_EzhOv7670_Dequeue,
    .get_buf_desc = HAL_CameraDev_EzhOv7670_Getbufdesc,
};

int HAL_CameraDev_EzhOv7670_setup(const char *name, camera_dev_t *dev)
{
    dev->ops = &camera_dev_ezh_ov7670_ops;

    return 0;
}
#else /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_EzhOv7670 == 1) */
int HAL_CameraDev_EzhOv7670_setup(const char *name, camera_dev_t *dev, _Bool defconfig)
{
    HAL_LOGE("Camera EzhOv7670 not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_EzhOv7670 == 1) */
