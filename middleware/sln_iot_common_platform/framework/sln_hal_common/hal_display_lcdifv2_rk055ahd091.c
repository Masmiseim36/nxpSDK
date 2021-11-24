/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief display dev HAL driver implementation for lcdifv2 and rk055ahd091.
 */

#include <FreeRTOS.h>
#include <queue.h>

#include "display_support.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_display_manager.h"
#include "hal_display_dev.h"

#if 0
#include "welcomehome_240x131.h"

//
// tempory debug code to show welcome screen
//
#define RGB565_RED   0xf800
#define RGB565_GREEN 0x07e0
#define RGB565_BLUE  0x001f

#define RGB565_NXPGREEN 0xBEA6
#define RGB565_NXPRED   0xFD83
#define RGB565_NXPBLUE  0x6D5B

static void draw_fillrect(int x, int y, int w, int h, int rect_color, unsigned short* pCanvasBuffer)
{
    for (int i = x; i < (x + w); i++)
        for (int j = y; j < (y + h); j++) {
            *(pCanvasBuffer + j * DEMO_BUFFER_WIDTH + i) = rect_color;
        }
}

static void draw_welcome(unsigned short* pRGB565)
{
    static int i = 0;
    int color = RGB565_BLUE;

    if (i % 2 == 0) {
        color = RGB565_NXPBLUE;
    } else if (i % 3 == 0) {
        color = RGB565_NXPRED;
    } else {
        color = RGB565_NXPGREEN;
    }

    i++;
    draw_fillrect(0, 0, 480, 640, color, pRGB565);

    for (int i = 0; i < welcomehome_H; i++) {
        for (int j = 0; j < welcomehome_W; j++) {
            pRGB565[ (i + 254) * DEMO_BUFFER_WIDTH + j + 120] = welcomehome_240x131[i * welcomehome_W + j];
        }
    }
}
#endif

#if defined(__cplusplus)
extern "C" {
#endif

int s_DisplayDev_Lcdif_rk055ahd091_register();

#if defined(__cplusplus)
}
#endif

#define DISPLAY_NAME "rk055ahd091"

/* LCD input frame buffer is RGB565, converted by PXP. */
#define DISPLAY_BUFFER_BPP   2
#define DISPLAY_BUFFER_COUNT 1

AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_LcdBuffer[DISPLAY_BUFFER_COUNT][DEMO_BUFFER_HEIGHT][DEMO_BUFFER_WIDTH * DISPLAY_BUFFER_BPP],
    FRAME_BUFFER_ALIGN);

static volatile bool s_newFrameShown = false;
static dc_fb_info_t s_fbInfo;
static volatile uint8_t s_lcdActiveFbIdx;

#include "fsl_soc_src.h"

static void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);

    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}

static void DISPLAY_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
    s_lcdActiveFbIdx ^= 1;
}

static void DISPLAY_InitDisplay(void)
{
    status_t status;

    BOARD_PrepareDisplayController();

    status = g_dc.ops->init(&g_dc);

    if (kStatus_Success != status)
    {
        LOGE("Display initialization failed");
        assert(0);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &s_fbInfo);
    s_fbInfo.pixelFormat = kVIDEO_PixelFormatRGB565;
    s_fbInfo.width       = DEMO_BUFFER_WIDTH;
    s_fbInfo.height      = DEMO_BUFFER_HEIGHT;
    s_fbInfo.startX      = DEMO_BUFFER_START_X;
    s_fbInfo.startY      = DEMO_BUFFER_START_Y;
    s_fbInfo.strideBytes = DEMO_BUFFER_WIDTH * DISPLAY_BUFFER_BPP;
    g_dc.ops->setLayerConfig(&g_dc, 0, &s_fbInfo);

    g_dc.ops->setCallback(&g_dc, 0, DISPLAY_BufferSwitchOffCallback, NULL);

    s_lcdActiveFbIdx = 0;
    s_newFrameShown  = false;
    g_dc.ops->setFrameBuffer(&g_dc, 0, s_LcdBuffer[s_lcdActiveFbIdx]);

    /* For the DBI interface display, application must wait for the first
     * frame buffer sent to the panel.
     */
    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        while (s_newFrameShown == false)
        {
        }
    }

    s_newFrameShown = true;

    g_dc.ops->enableLayer(&g_dc, 0);
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++HAL_DisplayDev_Lcdifv2Rk055ah_Init");

    memset(s_LcdBuffer, 0x0, sizeof(s_LcdBuffer));

    dev->cap.width       = width;
    dev->cap.height      = height;
    dev->cap.frameBuffer = (void *)s_LcdBuffer[0];

    BOARD_ResetDisplayMix();
    LOGD("--HAL_DisplayDev_Lcdifv2Rk055ah_Init");
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Start(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++HAL_DisplayDev_Lcdifv2Rk055ah_Start");
    DISPLAY_InitDisplay();
    LOGD("--HAL_DisplayDev_Lcdifv2Rk055ah_Start");
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGI("++HAL_DisplayDev_Lcdifv2Rk055ah_Blit");
    // draw_welcome(frame);
    g_dc.ops->setFrameBuffer(&g_dc, 0, frame);
    LOGI("--HAL_DisplayDev_Lcdifv2Rk055ah_Blit");
    return ret;
}

const static display_dev_operator_t s_DisplayDev_LcdifOps = {
    .init        = HAL_DisplayDev_Lcdifv2Rk055ah_Init,
    .deinit      = HAL_DisplayDev_Lcdifv2Rk055ah_Deinit,
    .start       = HAL_DisplayDev_Lcdifv2Rk055ah_Start,
    .blit        = HAL_DisplayDev_Lcdifv2Rk055ah_Blit,
    .inputNotify = NULL,
};

static display_dev_t s_DisplayDev_Lcdif = {.id   = 0,
                                           .name = DISPLAY_NAME,
                                           .ops  = &s_DisplayDev_LcdifOps,
                                           .cap  = {.width       = DEMO_BUFFER_WIDTH,
                                                   .height      = DEMO_BUFFER_HEIGHT,
                                                   .pitch       = DEMO_BUFFER_WIDTH * DISPLAY_BUFFER_BPP,
                                                   .left        = 0,
                                                   .top         = 0,
                                                   .right       = 479,
                                                   .bottom      = 639,
                                                   .rotate      = kCWRotateDegree_0,
                                                   .format      = kPixelFormat_RGB565,
                                                   .srcFormat   = kPixelFormat_UYVY1P422_RGB,
                                                   .frameBuffer = NULL,
                                                   .callback    = NULL,
                                                   .param       = NULL}};

int HAL_DisplayDev_Lcdifv2Rk055ah_Register()
{
    int ret = 0;
    LOGD("s_DisplayDev_Lcdif_rk055ahd091_register");
    ret = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_Lcdif);
    return ret;
}
