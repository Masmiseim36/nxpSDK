/*
 * Copyright 2019-2024 NXP.
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
 * @brief mipi_ov5640 camera module HAL camera driver implementation.
 */

#include "mpp_config.h"
#include "hal_camera_dev.h"
#include "hal_debug.h"

#if (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_MipiOv5640 == 1)
#include <stdlib.h>

#include "board.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_ov5640.h"
#include "fsl_mipi_csi2rx.h"
#include "fsl_debug_console.h"
#include "camera_support.h"
#include "display_support.h"
#include "fsl_common.h"
#include "hal.h"
#include "hal_os.h"
#include "hal_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif


hal_camera_status_t HAL_CameraDev_MipiOv5640_Enqueue(const camera_dev_t *dev, void *data);
void HAL_CameraDev_MipiOv5640_ReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData);
void BOARD_Camera_I2C_Init(void);
status_t BOARD_Camera_I2C_Send(uint8_t deviceAddress, uint32_t subAddress,
                                 uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_Receive(uint8_t deviceAddress, uint32_t subAddress,
                                    uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(__cplusplus)
}
#endif

#define CAMERA_NAME "MipiOv5640"

/* TODO get those values from framework/user at 'init' */
#define CAMERA_DEV_MipiOv5640_BUFFER_COUNT 3
#define CAMERA_DEV_MipiOv5640_MAX_HEIGHT 720    /* set camera max resolution equal to display max */
#define CAMERA_DEV_MipiOv5640_MAX_WIDTH 1280    /* set camera max resolution equal to display max */
#define CAMERA_DEV_MipiOv5640_MAX_BPP 4             /* max value for YUYV */
#define CAMERA_DEV_MipiOv5640_ROTATE ROTATE_0
#define CAMERA_DEV_MipiOv5640_FLIP FLIP_NONE
#define CAMERA_DEV_MipiOv5640_SWAPBYTE 0


#define CAMERA_DEV_BUFFER_ALIGN 64

/* TODO support aligned heap memory */
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_cameraBuffer[CAMERA_DEV_MipiOv5640_BUFFER_COUNT][CAMERA_DEV_MipiOv5640_MAX_HEIGHT]
                                 [CAMERA_DEV_MipiOv5640_MAX_WIDTH * CAMERA_DEV_MipiOv5640_MAX_BPP],
    CAMERA_DEV_BUFFER_ALIGN);

#define CAMERA_DEV_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)
#define CAMERA_MIPI_CSI_LANE     2

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CSI_CLK_FREQ          (CLOCK_GetFreqFromObs(CCM_OBS_BUS_CLK_ROOT))
#define DEMO_MIPI_CSI2_UI_CLK_FREQ (CLOCK_GetFreqFromObs(CCM_OBS_CSI2_UI_CLK_ROOT))

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

void MipiOv5640_InitCameraResource(void)
{
    BOARD_Camera_I2C_Init();

    /* CSI MCLK is connect to dedicated 24M OSC, so don't need to configure it. */
}

static status_t MipiOv5640_VerifyCameraClockSource(void)
{
    status_t status;
    uint32_t srcClkFreq;
    /*
     * The MIPI CSI clk_ui, clk_esc, and core_clk are all from
     * System PLL3 (PLL_480M). Verify the clock source to ensure
     * it is ready to use.
     */
    srcClkFreq = CLOCK_GetPllFreq(kCLOCK_PllSys3);

    if (480 != (srcClkFreq / 1000000))
    {
        status = kStatus_Fail;
    }
    else
    {
        status = kStatus_Success;
    }

    return status;
}

void MipiOv5640_InitMipiCsi(mpp_camera_params_t *config)
{
    csi2rx_config_t csi2rxConfig = {0};

    /* This clock should be equal or faster than the receive byte clock,
     * D0_HS_BYTE_CLKD, from the RX DPHY. For this board, there are two
     * data lanes, the MIPI CSI pixel format is 16-bit per pixel, the
     * max resolution supported is 720*1280@30Hz, so the MIPI CSI2 clock
     * should be faster than 720*1280*30 = 27.6MHz, choose 60MHz here.
     */
    const clock_root_config_t csi2ClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 7,
    };

    /* ESC clock should be in the range of 60~80 MHz */
    const clock_root_config_t csi2EscClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 7,
    };

    /* UI clock should be equal or faster than the input pixel clock.
     * The camera max resolution supported is 720*1280@30Hz, so this clock
     * should be faster than 720*1280*30 = 27.6MHz, choose 60MHz here.
     */
    const clock_root_config_t csi2UiClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 7,
    };

    if (kStatus_Success != MipiOv5640_VerifyCameraClockSource())
    {
        HAL_LOGE("MIPI CSI clock source not valid\r\n");

        while (1)
        {
        }
    }

    /* MIPI CSI2 connect to CSI. */
    CLOCK_EnableClock(kCLOCK_Video_Mux);
    VIDEO_MUX->VID_MUX_CTRL.SET = (VIDEO_MUX_VID_MUX_CTRL_CSI_SEL_MASK);

    CLOCK_SetRootClock(kCLOCK_Root_Csi2, &csi2ClockConfig);
    CLOCK_SetRootClock(kCLOCK_Root_Csi2_Esc, &csi2EscClockConfig);
    CLOCK_SetRootClock(kCLOCK_Root_Csi2_Ui, &csi2UiClockConfig);

    /* The CSI clock should be faster than MIPI CSI2 clk_ui. The CSI clock
     * is bus clock.
     */
    if (DEMO_CSI_CLK_FREQ < DEMO_MIPI_CSI2_UI_CLK_FREQ)
    {
        HAL_LOGE("CSI clock should be faster than MIPI CSI2 ui clock.\r\n");

        while (1)
        {
        }
    }

    /*
     * Initialize the MIPI CSI2
     *
     * From D-PHY specification, the T-HSSETTLE should in the range of 85ns+6*UI to 145ns+10*UI
     * UI is Unit Interval, equal to the duration of any HS state on the Clock Lane
     *
     * T-HSSETTLE = csi2rxConfig.tHsSettle_EscClk * (Tperiod of RxClkInEsc)
     *
     * csi2rxConfig.tHsSettle_EscClk setting for camera:
     *
     *    Resolution  |  frame rate  |  T_HS_SETTLE
     *  =============================================
     *     720P       |     30       |     0x12
     *  ---------------------------------------------
     *     720P       |     15       |     0x17
     *  ---------------------------------------------
     *      VGA       |     30       |     0x1F
     *  ---------------------------------------------
     *      VGA       |     15       |     0x24
     *  ---------------------------------------------
     *     QVGA       |     30       |     0x1F
     *  ---------------------------------------------
     *     QVGA       |     15       |     0x24
     *  ---------------------------------------------
     */
    static const uint32_t csi2rxHsSettle[][3] = {
        {
            kVIDEO_Resolution720P,
            30,
            0x12,
        },
        {
            kVIDEO_Resolution720P,
            15,
            0x17,
        },
        {
            kVIDEO_ResolutionVGA,
            30,
            0x1F,
        },
        {
            kVIDEO_ResolutionVGA,
            15,
            0x24,
        },
        {
            kVIDEO_ResolutionQVGA,
            30,
            0x1F,
        },
        {
            kVIDEO_ResolutionQVGA,
            15,
            0x24,
        },
    };

    csi2rxConfig.laneNum          = CAMERA_MIPI_CSI_LANE;
    csi2rxConfig.tHsSettle_EscClk = 0x12;

    for (uint8_t i = 0; i < ARRAY_SIZE(csi2rxHsSettle); i++)
    {
        if ((FSL_VIDEO_RESOLUTION(config->width, config->height) == csi2rxHsSettle[i][0]) &&
            (csi2rxHsSettle[i][1] >= config->fps))
        {
            csi2rxConfig.tHsSettle_EscClk = csi2rxHsSettle[i][2];
            break;
        }
    }

    CSI2RX_Init(MIPI_CSI2RX, &csi2rxConfig);
}

hal_camera_status_t HAL_CameraDev_MipiOv5640_Init(
    camera_dev_t *dev, mpp_camera_params_t *config, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_MipiOv5640_Init( param[%p]\n", param);

    dev->config.width = config->width;
    dev->config.height = config->height;
    dev->config.framerate = config->fps;
    dev->config.format = config->format;
    dev->cap.callback = callback;
    dev->cap.param    = param;

    camera_config_t cameraConfig;

    memset(&cameraConfig, 0, sizeof(cameraConfig));

    BOARD_PrepareDisplayController();

    MipiOv5640_InitCameraResource();

    switch(config->format) {
    case MPP_PIXEL_YUV1P444:
    /* CSI input data bus is 24-bit, and save as XYUV8888.. */
        cameraConfig.pixelFormat   = kVIDEO_PixelFormatXYUV;
        cameraConfig.bytesPerPixel = 4;
        break;
    case MPP_PIXEL_BGRA:
        cameraConfig.pixelFormat   = kVIDEO_PixelFormatXRGB8888;
        cameraConfig.bytesPerPixel = 4;
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

    NVIC_SetPriority(CSI_IRQn, hal_get_max_syscall_prio() + 1);
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, HAL_CameraDev_MipiOv5640_ReceiverCallback, NULL);

    MipiOv5640_InitMipiCsi(config);

    switch(config->format) {
    case MPP_PIXEL_YUV1P444:
        cameraConfig.pixelFormat   = kVIDEO_PixelFormatYUYV;
        cameraConfig.bytesPerPixel = 2;
        break;
    case MPP_PIXEL_BGRA:
        cameraConfig.pixelFormat   = kVIDEO_PixelFormatRGB565;
        cameraConfig.bytesPerPixel = 2;
        break;
    default:
        HAL_LOGE("Camera format unsupported\n");
        return kStatus_HAL_CameraError;
    }
    cameraConfig.resolution    = FSL_VIDEO_RESOLUTION(config->width, config->height);
    cameraConfig.interface     = kCAMERA_InterfaceMIPI;
    cameraConfig.controlFlags  = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec   = dev->config.framerate;
    cameraConfig.csiLanes      = CAMERA_MIPI_CSI_LANE;
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    /* Submit the empty frame buffers to buffer queue. */
    for (uint32_t i = 0; i < CAMERA_DEV_MipiOv5640_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_cameraBuffer[i]));
    }

    HAL_LOGD("--HAL_CameraDev_MipiOv5640_Init\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_MipiOv5640_Getbufdesc(const camera_dev_t *dev, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_MipiOv5640_Getbufdesc(out_buf=[%p])\n", out_buf);
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
    HAL_LOGD("--HAL_CameraDev_MipiOv5640_Getbufdesc\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_MipiOv5640_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

static uint32_t gCurrentBufferAddr = 0;

hal_camera_status_t HAL_CameraDev_MipiOv5640_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++HAL_CameraDev_MipiOv5640_Start\n");

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
    HAL_LOGD("--HAL_CameraDev_MipiOv5640_Start\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_MipiOv5640_Stop(const camera_dev_t *dev)
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

hal_camera_status_t HAL_CameraDev_MipiOv5640_Dequeue(const camera_dev_t *dev, void **data, int *stripe)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_MipiOv5640_Dequeue\n");
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
    HAL_LOGD("--HAL_CameraDev_MipiOv5640_Dequeue\n");
    return ret;
}

const static camera_dev_operator_t camera_dev_mipi_ov5640_ops = {
    .init        = HAL_CameraDev_MipiOv5640_Init,
    .deinit      = HAL_CameraDev_MipiOv5640_Deinit,
    .start       = HAL_CameraDev_MipiOv5640_Start,
    .stop        = HAL_CameraDev_MipiOv5640_Stop,
    .enqueue     = HAL_CameraDev_MipiOv5640_Enqueue,
    .dequeue     = HAL_CameraDev_MipiOv5640_Dequeue,
    .get_buf_desc = HAL_CameraDev_MipiOv5640_Getbufdesc,
};

static camera_dev_t camera_dev_mipi_ov5640 = {
    .id   = 1,
    .name = CAMERA_NAME,
    .ops  = &camera_dev_mipi_ov5640_ops,
    .config =
        {
            .rotate   = CAMERA_DEV_MipiOv5640_ROTATE,
            .flip     = CAMERA_DEV_MipiOv5640_FLIP,
            .swapByte = CAMERA_DEV_MipiOv5640_SWAPBYTE,
        },
    .cap = {.callback = NULL, .param = NULL},
};

void HAL_CameraDev_MipiOv5640_ReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData)
{
    if (camera_dev_mipi_ov5640.cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        camera_dev_mipi_ov5640.cap.callback(&camera_dev_mipi_ov5640, kCameraEvent_SendFrame,
                                            camera_dev_mipi_ov5640.cap.param, fromISR);
    }
}

hal_camera_status_t HAL_CameraDev_MipiOv5640_Enqueue(const camera_dev_t *dev, void *data)
{
    int error = 0;
    HAL_LOGD("++HAL_CameraDev_MipiOv5640_Enqueue\n");
    /* nothing to do, see HAL_CameraDev_MipiOv5640_Dequeue() */
    HAL_LOGD("--HAL_CameraDev_MipiOv5640_Enqueue\n");
    return error;
}

int HAL_CameraDev_MipiOv5640_setup(const char *name, camera_dev_t *dev, _Bool defconfig)
{
    dev->ops = &camera_dev_mipi_ov5640_ops;

    return 0;
}
#else /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_MipiOv5640 == 1) */
int HAL_CameraDev_MipiOv5640_setup(const char *name, camera_dev_t *dev, _Bool defconfig)
{
    HAL_LOGE("Camera MipiOv5640 not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_MipiOv5640 == 1) */
