/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: IJG
 */

#include <stdio.h>
#include <string.h>
#include "fsl_common.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "jpeg.h"
#include "jpeglib.h"
#include "display_support.h"
#include "board.h"

#if LIB_JPEG_USE_HW_ACCEL
#include "fsl_jpegdec.h"

JPEG_DECODER_Type g_jpegdec = {
    .core    = JPEGDEC,
    .wrapper = JPGDECWRP,
};

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FB_BPP    3 /* LCD frame buffer byte per pixel, RGB888 format, 24-bit. */
#define APP_FB_FORMAT kVIDEO_PixelFormatRGB888
#define APP_RGB_ADDR  0x20239000U
#define APP_YUV_ADDR  0x204DC000U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_BufferSwitchOffCallback(void *param, void *switchOffBuffer);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static dc_fb_info_t fbInfo;
/*
 * When new frame buffer sent to display, it might not be shown immediately.
 * Application could use callback to get new frame shown notification, at the
 * same time, when this flag is set, application could write to the older
 * frame buffer.
 */
static volatile bool s_newFrameShown = false;

/* This struct contains the JPEG decompression parameters */
AT_NONCACHEABLE_SECTION(static struct jpeg_decompress_struct cinfo);
/* This struct represents a JPEG error handler */
static struct jpeg_error_mgr jerr;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if LIB_JPEG_USE_HW_ACCEL
static uint8_t BYTECLIP(int val)
{
    if (val < 0)
    {
        return 0U;
    }
    else if (val > 255)
    {
        return 255U;
    }
    else
    {
        return (uint8_t)val;
    }
}

void Convert_yuv420_to_rgb565(uint16_t width, uint16_t height, uint32_t yuvAddr, uint32_t rgbAddr)
{
    uint8_t *rgb = (void *)rgbAddr;
    int16_t r, g, b;
    uint8_t R, G, B, y, u, v;

    uint16_t i, j;

    for (i = 0U; i < height; i++)
    {
        for (j = 0U; j < width; j++)
        {
            y      = *(((uint8_t *)yuvAddr) + 3 * (width * i + j));
            u      = *(((uint8_t *)yuvAddr) + 3 * (width * i + j) + 1U);
            v      = *(((uint8_t *)yuvAddr) + 3 * (width * i + j) + 2U);
            r      = y + 1402 * (v - 128) / 1000;
            g      = y - (344 * (u - 128) + 714 * (v - 128)) / 1000;
            b      = y + 1772 * (u - 128) / 1000;
            R      = BYTECLIP(r);
            G      = BYTECLIP(g);
            B      = BYTECLIP(b);
            *rgb++ = B;
            *rgb++ = G;
            *rgb++ = R;
        }
    }
}
#endif

/*!
 * @brief  Jpeg Decode
 * @param  file:    pointer to the jpg file
 * @param  buff:     pointer to the image line
 */
void APP_Decode_JPEG()
{
    uint8_t *buffer         = (uint8_t *)APP_RGB_ADDR;
    JSAMPROW row_pointer[1] = {0}; /* Output row buffer */

    // Step 1: allocate and initialize JPEG decompression object
    cinfo.err = jpeg_std_error(&jerr);

    // Step 2: Initialize the JPEG decompression object
    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, jpegImg, jpegImgLen);

    // Step 3: read image parameters with jpeg_read_header()
    jpeg_read_header(&cinfo, true);

    // Step 4: set parameters for decompression
    cinfo.dct_method = JDCT_FLOAT;

#if LIB_JPEG_USE_HW_ACCEL
    /* When using JPEG decoder IP, it only support when the input and output
       color space are the same, and the resize function is off. */
    cinfo.out_color_space = cinfo.jpeg_color_space;

    cinfo.jpegdec = (void *)&g_jpegdec;
    jpeg_set_output(&cinfo, (uint8_t *)APP_YUV_ADDR, APP_FB_BPP * cinfo.image_width);
#else
    /*
     * Resize to fit the screen, the actual resize rate is:
     * cinfo.scale_num / 8, the cinfo.scale_num must be in the range of 1 ~ 16
     */
    if ((cinfo.image_width * DEMO_PANEL_HEIGHT) > (cinfo.image_height * DEMO_PANEL_WIDTH))
    {
        cinfo.scale_num = DEMO_PANEL_WIDTH * 8 / cinfo.image_width;
    }
    else
    {
        cinfo.scale_num = DEMO_PANEL_HEIGHT * 8 / cinfo.image_height;
    }

    if (cinfo.scale_num < 1)
    {
        cinfo.scale_num = 1;
    }
    else if (cinfo.scale_num > 16)
    {
        cinfo.scale_num = 16;
    }
#endif

    // Step 5: start decompressor
    jpeg_start_decompress(&cinfo);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        row_pointer[0] = &buffer[cinfo.output_scanline * cinfo.output_width * APP_FB_BPP];
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
    }

    // Step 6: Finish decompression
    jpeg_finish_decompress(&cinfo);

    // Step 7: Release JPEG decompression object
    jpeg_destroy_decompress(&cinfo);

#if LIB_JPEG_USE_HW_ACCEL
    Convert_yuv420_to_rgb565(cinfo.image_width, cinfo.image_height, APP_YUV_ADDR, APP_RGB_ADDR);
#endif
}

void APP_DisplayDecodedImage(void)
{
    /* Step 10: Configure dispaly layer configuration. */
    fbInfo.pixelFormat = APP_FB_FORMAT;
    fbInfo.width       = cinfo.output_width;
    fbInfo.height      = cinfo.output_height;
    fbInfo.startX      = (DEMO_PANEL_WIDTH - cinfo.output_width) / 2U;
    fbInfo.startY      = (DEMO_PANEL_HEIGHT - cinfo.output_height) / 2U;
    fbInfo.strideBytes = cinfo.output_width * APP_FB_BPP;
    if (kStatus_Success != g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo))
    {
        PRINTF("Error: Could not configure the display controller\r\n");
        assert(false);
    }

    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)APP_RGB_ADDR);

    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        while (s_newFrameShown == false)
        {
        }
    }

    s_newFrameShown = true;

    /* Step 11: Enable layer and display the decoded image. */
    g_dc.ops->enableLayer(&g_dc, 0);
}

void APP_InitDisplay(void)
{
    status_t status;

    BOARD_PrepareDisplayController();

    memset((void *)APP_RGB_ADDR, 0, DEMO_PANEL_HEIGHT * DEMO_PANEL_WIDTH * APP_FB_BPP);

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        PRINTF("Display initialization failed\r\n");
        assert(false);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    g_dc.ops->setCallback(&g_dc, 0, APP_BufferSwitchOffCallback, NULL);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("Lib JPEG demo with hardware acceleration start:\r\n");

    APP_InitDisplay();

    PRINTF("Decoding the image...\r\n");
    APP_Decode_JPEG();
    PRINTF("done!\r\n");

    APP_DisplayDecodedImage();

    while (1)
    {
    }
}

static void APP_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
}
