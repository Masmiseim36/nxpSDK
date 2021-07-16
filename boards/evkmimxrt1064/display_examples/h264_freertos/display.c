/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "display_support.h"
#include "fsl_debug_console.h"
#include "fsl_pxp.h"
#include "fsl_fbdev.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ENABLE_DISPLAY       1
#define ENABLE_PXP_INTERRUPT 1

/*
 * In this project, the framebuffer pixel format is set to RGB565. The input YUV420
 * data is converted to RGB565, then sent to display controller.
 */
#ifdef DEMO_BUFFER_BYTE_PER_PIXEL
#undef DEMO_BUFFER_BYTE_PER_PIXEL
#endif
#define DEMO_BUFFER_BYTE_PER_PIXEL 2

#ifdef DEMO_BUFFER_PIXEL_FORMAT
#undef DEMO_BUFFER_PIXEL_FORMAT
#endif
#define DEMO_BUFFER_PIXEL_FORMAT kVIDEO_PixelFormatRGB565

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if ENABLE_DISPLAY
static int DEMO_InitPXP(void);
static int DEMO_InitFBDEV(void);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if ENABLE_DISPLAY

#if !DEMO_BUFFER_FIXED_ADDRESS
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_frameBuffer[DEMO_BUFFER_COUNT][DEMO_BUFFER_HEIGHT][DEMO_BUFFER_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL],
    FRAME_BUFFER_ALIGN);

#define DEMO_BUFFER0_ADDR (uint32_t) s_frameBuffer[0]

#if DEMO_BUFFER_COUNT > 1
#define DEMO_BUFFER1_ADDR (uint32_t) s_frameBuffer[1]
#endif

#if DEMO_BUFFER_COUNT > 2
#define DEMO_BUFFER2_ADDR (uint32_t) s_frameBuffer[2]
#endif

#endif

static const uint32_t s_frameBufferAddress[DEMO_BUFFER_COUNT] = {DEMO_BUFFER0_ADDR,
#if DEMO_BUFFER_COUNT > 1
                                                                 DEMO_BUFFER1_ADDR,
#endif
#if DEMO_BUFFER_COUNT > 2
                                                                 DEMO_BUFFER2_ADDR
#endif
};

/* FBDEV */
static fbdev_t s_fbdev;
static fbdev_fb_info_t s_fbInfo;

/* PXP */
static pxp_output_buffer_config_t s_pxpOutputBufferConfig;
static pxp_ps_buffer_config_t s_pxpPsBufferConfig;
#if ENABLE_PXP_INTERRUPT
static SemaphoreHandle_t s_pxpCompleteSema;
#endif

#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if ENABLE_DISPLAY

#if ENABLE_PXP_INTERRUPT
void PXP_IRQHandler(void)
{
    BaseType_t wake = pdFALSE;

    if (0U != (kPXP_CompleteFlag & PXP_GetStatusFlags(PXP)))
    {
        PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);

        (void)xSemaphoreGiveFromISR(s_pxpCompleteSema, &wake);

        portYIELD_FROM_ISR(wake);
    }
}
#endif

static int DEMO_InitPXP(void)
{
#if ENABLE_PXP_INTERRUPT
    /* Semaphore */
    s_pxpCompleteSema = xSemaphoreCreateBinary();

    if (NULL == s_pxpCompleteSema)
    {
        return -1;
    }
#endif

    /* Initialize variables. */
    memset(&s_pxpPsBufferConfig, 0, sizeof(s_pxpPsBufferConfig));
    memset(&s_pxpOutputBufferConfig, 0, sizeof(s_pxpOutputBufferConfig));

    s_pxpPsBufferConfig.pixelFormat = kPXP_PsPixelFormatYVU420;
    s_pxpPsBufferConfig.swapByte    = false,

    s_pxpOutputBufferConfig.pixelFormat    = kPXP_OutputPixelFormatRGB565;
    s_pxpOutputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    s_pxpOutputBufferConfig.pitchBytes     = (DEMO_BUFFER_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL);

    /* Initialize hardware. */
    PXP_Init(PXP);

    PXP_SetProcessSurfaceBackGroundColor(PXP, 0U);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    PXP_SetCsc1Mode(PXP, kPXP_Csc1YCbCr2RGB);
    PXP_EnableCsc1(PXP, true);

#if ENABLE_PXP_INTERRUPT
    NVIC_SetPriority(PXP_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    EnableIRQ(PXP_IRQn);

    PXP_EnableInterrupts(PXP, kPXP_CompleteInterruptEnable);
#endif

    return 0;
}

static int DEMO_InitFBDEV(void)
{
    void *buffer;
    BOARD_PrepareDisplayController();

    FBDEV_Open(&s_fbdev, &g_dc, 0);

    FBDEV_GetFrameBufferInfo(&s_fbdev, &s_fbInfo);

    s_fbInfo.bufInfo.pixelFormat = DEMO_BUFFER_PIXEL_FORMAT;
    s_fbInfo.bufInfo.startX      = DEMO_BUFFER_START_X;
    s_fbInfo.bufInfo.startY      = DEMO_BUFFER_START_Y;
    s_fbInfo.bufInfo.width       = DEMO_BUFFER_WIDTH;
    s_fbInfo.bufInfo.height      = DEMO_BUFFER_HEIGHT;
    s_fbInfo.bufInfo.strideBytes = (DEMO_BUFFER_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL);

    s_fbInfo.bufferCount = DEMO_BUFFER_COUNT;
    for (uint8_t i = 0; i < DEMO_BUFFER_COUNT; i++)
    {
        s_fbInfo.buffers[i] = (void *)s_frameBufferAddress[i];
    }

    if (kStatus_Success != FBDEV_SetFrameBufferInfo(&s_fbdev, &s_fbInfo))
    {
        PRINTF("Set frame buffer info error\r\n");

        return -1;
    }

    /* Set the first frame, it is shown when enabled. */
    buffer = FBDEV_GetFrameBuffer(&s_fbdev, 0);

    assert(buffer != NULL);

    memset(buffer, 0, DEMO_BUFFER_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL * DEMO_BUFFER_HEIGHT);

    FBDEV_SetFrameBuffer(&s_fbdev, buffer, 0);

    FBDEV_Enable(&s_fbdev);

    return 0;
}
#endif

int DEMO_InitDisplay(void)
{
#if ENABLE_DISPLAY
    int ret;

    ret = DEMO_InitFBDEV();

    if (0 == ret)
    {
        ret = DEMO_InitPXP();
    }

    return ret;
#else
    return 0;
#endif
}

void DEMO_DisplayFrame(uint16_t width,
                       uint16_t height,
                       const uint8_t *Y,
                       const uint8_t *U,
                       const uint8_t *V,
                       uint32_t Y_Stride,
                       uint32_t UV_Stride)
{
#if ENABLE_DISPLAY
    void *buffer;
    bool rotate                    = false;
    static uint16_t oldInputWidth  = 0U;
    static uint16_t oldInputHeight = 0U;

    uint16_t lcdWidth  = s_fbInfo.bufInfo.width;
    uint16_t lcdHeight = s_fbInfo.bufInfo.height;

    DCACHE_CleanInvalidateByRange((uint32_t)Y, height * Y_Stride);
    DCACHE_CleanInvalidateByRange((uint32_t)U, height * UV_Stride / 2);
    DCACHE_CleanInvalidateByRange((uint32_t)V, height * UV_Stride / 2);

    /* PS configure. */
    s_pxpPsBufferConfig.bufferAddr  = (uint32_t)Y;
    s_pxpPsBufferConfig.bufferAddrU = (uint32_t)U;
    s_pxpPsBufferConfig.bufferAddrV = (uint32_t)V;
    s_pxpPsBufferConfig.pitchBytes  = Y_Stride;

    PXP_SetProcessSurfaceBufferConfig(PXP, &s_pxpPsBufferConfig);

    /* Input frame size changed. */
    if ((oldInputHeight != height) || (oldInputWidth != width))
    {
        PRINTF("Input Frame Size: %d x %d\r\n", width, height);

#if DEMO_BUFFER_HEIGHT > DEMO_BUFFER_WIDTH
        rotate = (height < width);
#else
        rotate = (height > width);
#endif

        if (rotate)
        {
            s_pxpOutputBufferConfig.width  = lcdHeight;
            s_pxpOutputBufferConfig.height = lcdWidth;

            PXP_SetRotateConfig(PXP, kPXP_RotateOutputBuffer, kPXP_Rotate90, kPXP_FlipDisable);
            PXP_SetProcessSurfaceScaler(PXP, width, height, lcdHeight, lcdWidth);
            PXP_SetProcessSurfacePosition(PXP, 0, 0, lcdHeight - 1, lcdWidth - 1);
        }
        else
        {
            s_pxpOutputBufferConfig.width  = lcdWidth;
            s_pxpOutputBufferConfig.height = lcdHeight;

            PXP_SetProcessSurfaceScaler(PXP, width, height, lcdWidth, lcdHeight);
            PXP_SetProcessSurfacePosition(PXP, 0, 0, lcdWidth - 1, lcdHeight - 1);
        }

        oldInputHeight = height;
        oldInputWidth  = width;
    }

    buffer                              = FBDEV_GetFrameBuffer(&s_fbdev, 0);
    s_pxpOutputBufferConfig.buffer0Addr = (uint32_t)buffer;

    PXP_SetOutputBufferConfig(PXP, &s_pxpOutputBufferConfig);

    PXP_Start(PXP);

#if ENABLE_PXP_INTERRUPT
    (void)xSemaphoreTake(s_pxpCompleteSema, portMAX_DELAY);
#else
    while (0U == (kPXP_CompleteFlag & PXP_GetStatusFlags(PXP)))
    {
    }
    PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);
#endif

    FBDEV_SetFrameBuffer(&s_fbdev, buffer, 0);
#endif
}
