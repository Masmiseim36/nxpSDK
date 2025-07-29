/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "fsl_common.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "input.h"
#include "display_support.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer);
static void DEMO_InitDisplay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(static dc_fb_info_t fbInfo);
static volatile bool s_newFrameShown = false;
static uint32_t s_frameBufferAddr = DEMO_BUFFER0_ADDR;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void DEMO_InitDisplay(void)
{
    status_t status;

    BOARD_PrepareDisplayController();

    memcpy((void *)s_frameBufferAddr, (void *)inputBuf, inputBufLen);

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        PRINTF("Display initialization failed\r\n");
        assert(false);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);

    /* Configure display layer configuration. */
    fbInfo.width       = DEMO_PIC_WIDTH;
    fbInfo.height      = DEMO_PIC_HEIGHT;
    fbInfo.startX      = 0U;
    fbInfo.startY      = 0U;

    switch (DEMO_PIC_FORMAT)
    {
        case DEMO_PIC_FORMAT_RGB8None:
            fbInfo.pixelFormat = kVIDEO_PixelFormatRGB888Nonsample;
            fbInfo.strideBytes = DEMO_PIC_WIDTH * 3U;
            break;
        case DEMO_PIC_FORMAT_RGB8H:
            fbInfo.pixelFormat = kVIDEO_PixelFormatRGB888Hsample;
            fbInfo.strideBytes = DEMO_PIC_WIDTH * 3U;
            break;
        case DEMO_PIC_FORMAT_RGB8HV:
            fbInfo.pixelFormat = kVIDEO_PixelFormatRGB888HVsample;
            fbInfo.strideBytes = DEMO_PIC_WIDTH * 3U;
            break;
        case DEMO_PIC_FORMAT_ARGB8None:
            fbInfo.pixelFormat = kVIDEO_PixelFormatARGB8888Nonsample;
            fbInfo.strideBytes = DEMO_PIC_WIDTH * 4U;
            break;
        case DEMO_PIC_FORMAT_ARGB8H:
            fbInfo.pixelFormat = kVIDEO_PixelFormatARGB8888Hsample;
            fbInfo.strideBytes = DEMO_PIC_WIDTH * 4U;
            break;
        case DEMO_PIC_FORMAT_ARGB8HV:
            fbInfo.pixelFormat = kVIDEO_PixelFormatARGB8888HVsample;
            fbInfo.strideBytes = DEMO_PIC_WIDTH * 4U;
            break;
        default:
            PRINTF("Error: Do not support this compressed format\r\n");
            assert(false);
            break;
    }

    if (kStatus_Success != g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo))
    {
        PRINTF("Error: Could not configure the display controller\r\n");
        assert(false);
    }

    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, NULL);

    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_frameBufferAddr);

    /* Enable layer. */
    g_dc.ops->enableLayer(&g_dc, 0);

    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        while (s_newFrameShown == false)
        {
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("DECNano decompress demo start:\r\n");
    PRINTF("The decompressed picture will be shown on top-left corner.\r\n");

    DEMO_InitDisplay();

    PRINTF("done!\r\n");

    while (1)
    {
    }
}

static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
}
