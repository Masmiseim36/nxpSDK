/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief  display over lpuart HAL driver implementation.
 */

#include <FreeRTOS.h>
#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"

#include "fwk_log.h"
#include "fwk_display_manager.h"
#include "hal_display_dev.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DISPLAY_NAME                        "diplay lpuart"
#define DISPLAY_3D_IR_FRAME_WIDTH           540
#define DISPLAY_3D_IR_FRAME_HEIGHT          640
#define DISPLAY_3D_IR_FRAME_BYTES_PER_PIXEL 3

#define DISPLAY_3D_DEPTH_FRAME_WIDTH           540
#define DISPLAY_3D_DEPTH_FRAME_HEIGHT          640
#define DISPLAY_3D_DEPTH_FRAME_BYTES_PER_PIXEL 2

#define DISPLAY_2D_RGB_FRAME_WIDTH           480
#define DISPLAY_2D_RGB_FRAME_HEIGHT          640
#define DISPLAY_2D_RGB_FRAME_BYTES_PER_PIXEL 3

#define DISPLAY_2D_IR_FRAME_WIDTH           480
#define DISPLAY_2D_IR_FRAME_HEIGHT          640
#define DISPLAY_2D_IR_FRAME_BYTES_PER_PIXEL 3

#define DISPLAY_MAX_FRAME_WIDTH           540
#define DISPLAY_MAX_FRAME_HEIGHT          640
#define DISPLAY_MAX_FRAME_BYTES_PER_PIXEL 3
#define DISPLAY_FRAME_BUFFER_COUNT        2

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
static hal_display_status_t HAL_DisplayDev_Lpuart_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param);
static hal_display_status_t HAL_DisplayDev_Lpuart_DeInit(const display_dev_t *dev);
static hal_display_status_t HAL_DisplayDev_Lpuart_Start(const display_dev_t *dev);
static hal_display_status_t HAL_DisplayDev_Lpuart_Blit(const display_dev_t *dev, void *frame, int width, int height);
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_FrameBuffer[DISPLAY_FRAME_BUFFER_COUNT][DISPLAY_MAX_FRAME_HEIGHT * DISPLAY_MAX_FRAME_WIDTH *
                                                             DISPLAY_MAX_FRAME_BYTES_PER_PIXEL],
    64);

const static display_dev_operator_t s_DisplayDev_Lpuart3DOps[2] = {{
                                                                       .init        = HAL_DisplayDev_Lpuart_Init,
                                                                       .deinit      = HAL_DisplayDev_Lpuart_DeInit,
                                                                       .start       = HAL_DisplayDev_Lpuart_Start,
                                                                       .blit        = HAL_DisplayDev_Lpuart_Blit,
                                                                       .inputNotify = NULL,
                                                                   },
                                                                   {
                                                                       .init        = HAL_DisplayDev_Lpuart_Init,
                                                                       .deinit      = HAL_DisplayDev_Lpuart_DeInit,
                                                                       .start       = NULL,
                                                                       .blit        = HAL_DisplayDev_Lpuart_Blit,
                                                                       .inputNotify = NULL,
                                                                   }};

const static display_dev_operator_t s_DisplayDev_Lpuart2DOps[2] = {{
                                                                       .init        = HAL_DisplayDev_Lpuart_Init,
                                                                       .deinit      = HAL_DisplayDev_Lpuart_DeInit,
                                                                       .start       = HAL_DisplayDev_Lpuart_Start,
                                                                       .blit        = HAL_DisplayDev_Lpuart_Blit,
                                                                       .inputNotify = NULL,
                                                                   },
                                                                   {
                                                                       .init        = HAL_DisplayDev_Lpuart_Init,
                                                                       .deinit      = HAL_DisplayDev_Lpuart_DeInit,
                                                                       .start       = NULL,
                                                                       .blit        = HAL_DisplayDev_Lpuart_Blit,
                                                                       .inputNotify = NULL,
                                                                   }};

static display_dev_t s_DisplayDev_DevLpuart3D[2] = {
    {.id   = 0,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_Lpuart3DOps[0],
     .cap  = {.width       = DISPLAY_3D_IR_FRAME_WIDTH,
             .height      = DISPLAY_3D_IR_FRAME_HEIGHT,
             .pitch       = DISPLAY_3D_IR_FRAME_WIDTH * DISPLAY_3D_IR_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_3D_IR_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_3D_IR_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_BGR,
             .srcFormat   = kPixelFormat_Gray16,
             .frameBuffer = (void *)&s_FrameBuffer[0],
             .callback    = NULL,
             .param       = NULL}},
    {.id   = 1,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_Lpuart3DOps[1],
     .cap  = {.width       = DISPLAY_3D_DEPTH_FRAME_WIDTH,
             .height      = DISPLAY_3D_DEPTH_FRAME_HEIGHT,
             .pitch       = DISPLAY_3D_DEPTH_FRAME_WIDTH * DISPLAY_3D_DEPTH_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_3D_DEPTH_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_3D_DEPTH_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_Depth16,
             .srcFormat   = kPixelFormat_Depth16,
             .frameBuffer = (void *)&s_FrameBuffer[1],
             .callback    = NULL,
             .param       = NULL}},
};

static display_dev_t s_DisplayDev_DevLpuart2D[2] = {
    {.id   = 0,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_Lpuart2DOps[0],
     .cap  = {.width       = DISPLAY_2D_IR_FRAME_WIDTH,
             .height      = DISPLAY_2D_IR_FRAME_HEIGHT,
             .pitch       = DISPLAY_2D_IR_FRAME_WIDTH * DISPLAY_2D_IR_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_2D_IR_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_2D_IR_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_BGR,
             .srcFormat   = kPixelFormat_UYVY1P422_Gray,
             .frameBuffer = (void *)&s_FrameBuffer[0],
             .callback    = NULL,
             .param       = NULL}},
    {.id   = 1,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_Lpuart2DOps[1],
     .cap  = {.width       = DISPLAY_2D_RGB_FRAME_WIDTH,
             .height      = DISPLAY_2D_RGB_FRAME_HEIGHT,
             .pitch       = DISPLAY_2D_RGB_FRAME_WIDTH * DISPLAY_2D_RGB_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_2D_RGB_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_2D_RGB_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_BGR,
             .srcFormat   = kPixelFormat_UYVY1P422_RGB,
             .frameBuffer = (void *)&s_FrameBuffer[1],
             .callback    = NULL,
             .param       = NULL}},
};

static lpuart_rtos_handle_t s_LpuartRTOSHandle;
static lpuart_handle_t s_LpuartHandle;
static uint8_t s_LpuartRingBuffer[512];

/*******************************************************************************
 * Code
 ******************************************************************************/
static hal_display_status_t HAL_DisplayDev_Lpuart_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("HAL_DisplayDev_Lpuart_Init");

    dev->cap.width    = width;
    dev->cap.height   = height;
    dev->cap.callback = callback;
    return ret;
}

static hal_display_status_t HAL_DisplayDev_Lpuart_DeInit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("HAL_DisplayDev_Lpuart_DeInit");
    return ret;
}

static hal_display_status_t HAL_DisplayDev_Lpuart_Start(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("HAL_DisplayDev_Lpuart_Start");

    lpuart_rtos_config_t config = {
        .base        = LPUART12,
        .baudrate    = 3000000,
        .parity      = kLPUART_ParityDisabled,
        .stopbits    = kLPUART_OneStopBit,
        .buffer      = s_LpuartRingBuffer,
        .buffer_size = sizeof(s_LpuartRingBuffer),
#if defined(FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT) && FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
        .enableRxRTS = 0,
        .enableTxCTS = 0,
        .txCtsSource = 0,
        .txCtsConfig = 0,
#endif
    };
    config.srcclk = CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart12);

    NVIC_SetPriority(LPUART12_IRQn, 5);
    if (kStatus_Success != LPUART_RTOS_Init(&s_LpuartRTOSHandle, &s_LpuartHandle, &config))
    {
        LOGE("LPUART_RTOS_Init: Error!");
        ret = kStatus_HAL_DisplayError;
    }
    return ret;
}

static hal_display_status_t HAL_DisplayDev_Lpuart_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret        = kStatus_HAL_DisplaySuccess;
    static const uint8_t TU_MAGIC[] = {0x53, 0x79, 0x4c};

    LOGD("HAL_DisplayDev_Lpuart_Blit");
    uint8_t htUnit[14] = {0};
    memcpy(htUnit, TU_MAGIC, sizeof(TU_MAGIC));
    htUnit[3] = dev->cap.format;
    htUnit[4] = dev->cap.srcFormat;
    htUnit[5] = 0;
    memcpy(htUnit + 6, &dev->cap.width, 4);
    memcpy(htUnit + 10, &dev->cap.height, 4);
    LPUART_RTOS_Send(&s_LpuartRTOSHandle, htUnit, 14);
    LPUART_RTOS_Send(&s_LpuartRTOSHandle, frame, dev->cap.height * dev->cap.pitch);
    return ret;
}

int HAL_DisplayDev_Lpuart3D_Register()
{
    int error = 0;
    LOGD("HAL_DisplayDev_Lpuart3D_Register");
    error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_DevLpuart3D[0]);
    if (error == 0)
    {
        error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_DevLpuart3D[1]);
    }
    return error;
}

int HAL_DisplayDev_Lpuart2D_Register()
{
    int error = 0;
    LOGD("HAL_DisplayDev_Lpuart2D_Register");
    error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_DevLpuart2D[0]);
    if (error == 0)
    {
        error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_DevLpuart2D[1]);
    }
    return error;
}
