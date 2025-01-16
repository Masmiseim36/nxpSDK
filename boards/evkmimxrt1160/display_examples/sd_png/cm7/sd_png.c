/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "fsl_common.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "PNGdec.h"
#include "display_support.h"
#include "board.h"
#include "sdmmc_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FB_HEIGHT  DEMO_BUFFER_HEIGHT
#define APP_FB_WIDTH   DEMO_BUFFER_WIDTH
#define APP_FB_START_X DEMO_BUFFER_START_X
#define APP_FB_START_Y DEMO_BUFFER_START_Y

/*
 * For better performance, by default, three frame buffers are used in this demo.
 * If the memory region is not large enough, user can define to 2 to override
 * this value.
 */
#ifndef APP_FB_NUM
#define APP_FB_NUM 3 /* LCD frame buffer number. */
#endif

/* Use XBGR8888 format by default. */
#define APP_FB_BPP    4
#define APP_FB_FORMAT kVIDEO_PixelFormatXBGR8888

/* Cache line size. */
#ifndef FSL_FEATURE_L2CACHE_LINESIZE_BYTE
#define FSL_FEATURE_L2CACHE_LINESIZE_BYTE 0
#endif
#ifndef FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
#define FSL_FEATURE_L1DCACHE_LINESIZE_BYTE 0
#endif

#if (FSL_FEATURE_L2CACHE_LINESIZE_BYTE > FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define APP_CACHE_LINE_SIZE FSL_FEATURE_L2CACHE_LINESIZE_BYTE
#else
#define APP_CACHE_LINE_SIZE FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
#endif

/*
 * For better performance, the frame buffers are located in cachable region and
 * the cache line maintanance is handled in this demo. The start address of
 * frame buffer, and the size of frame buffer, are aligned to the cache line
 * size.
 */
#ifndef APP_FB_ALIGN
#if (APP_CACHE_LINE_SIZE > 0)
#define APP_FB_ALIGN APP_CACHE_LINE_SIZE
#else
#define APP_FB_ALIGN 1
#endif
#endif /* APP_FB_ALIGN */

#ifndef APP_FB_STRIDE_BYTE
#define APP_FB_STRIDE_BYTE (APP_FB_WIDTH * APP_FB_BPP)
#endif

#define APP_FB_SIZE_BYTE (SDK_SIZEALIGN(APP_FB_STRIDE_BYTE * APP_FB_HEIGHT, APP_FB_ALIGN))

#ifndef APP_FB_USE_FIXED_ADDRESS
#define APP_FB_USE_FIXED_ADDRESS 0
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t sdcardWaitCardInsert(void);
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
static void *volatile s_fbList       = NULL; /* List to the frame buffers. */

/* PNG decompression handle. */
PNGIMAGE s_pngimage;

#if APP_FB_USE_FIXED_ADDRESS
static void *g_frameBuffer[APP_FB_NUM] = {
    (void *)APP_FB0_ADDR,
#if (APP_FB_NUM > 1)
    (void *)APP_FB1_ADDR,
#endif
#if (APP_FB_NUM > 2)
    (void *)APP_FB2_ADDR,
#endif
};
#else
SDK_ALIGN(static uint8_t g_frameBuffer[APP_FB_NUM][APP_FB_SIZE_BYTE], APP_FB_ALIGN);
#endif

AT_NONCACHEABLE_SECTION(static FATFS g_fileSystem); /* File system object */
AT_NONCACHEABLE_SECTION(static FIL pngFil);
/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get the empty frame buffer from the s_fbList. */
static void *APP_GetFrameBuffer(void)
{
    void *fb;

    fb = s_fbList;

    if (NULL != fb)
    {
        s_fbList = *(void **)fb;
    }

    return fb;
}

/* Put the unused frame buffer to the s_fbList. */
static void APP_PutFrameBuffer(void *fb)
{
    *(void **)fb = s_fbList;
    s_fbList     = fb;
}

void APP_SetLayerConfig(uint32_t width, uint32_t height)
{
    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    fbInfo.pixelFormat = APP_FB_FORMAT;
    fbInfo.width       = width;
    fbInfo.height      = height;
    fbInfo.startX      = (APP_FB_WIDTH - width) / 2U;
    fbInfo.startY      = (APP_FB_HEIGHT - height) / 2U;
    fbInfo.strideBytes = width * APP_FB_BPP;
    if (kStatus_Success != g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo))
    {
        PRINTF("Error: Could not configure the display controller\r\n");
        assert(false);
    }

    g_dc.ops->setCallback(&g_dc, 0, APP_BufferSwitchOffCallback, NULL);
}

/*!
 * @brief  PNG Decode
 * @param  file:    pointer to the png file
 * @param  buff:     pointer to the image buffer
 */
void png_decode(FIL *file, uint8_t *buffer)
{
    int err, width, height;
    uint8_t *png_buffer;
    uint8_t *png_buffer_aligned;
    uint8_t *read_pos;
    UINT png_size;
    UINT bytesRead;
    UINT bytesRemain;

    /* Step 1: Read the PNG file into ram. */
    png_size   = f_size(file);
    png_buffer = (unsigned char *)malloc(png_size + 2 * APP_CACHE_LINE_SIZE);

    if (png_buffer == NULL)
    {
        PRINTF("Error: memory allocation error\r\n");
        assert(false);
    }
#if APP_CACHE_LINE_SIZE
    png_buffer_aligned = (void *)(SDK_SIZEALIGN((uint32_t)png_buffer, APP_CACHE_LINE_SIZE));
#else
    png_buffer_aligned = png_buffer;
#endif

    bytesRemain = png_size;
    read_pos    = png_buffer_aligned;

    DCACHE_CleanInvalidateByRange((uint32_t)png_buffer_aligned, png_size);

    while (bytesRemain > 0)
    {
        f_read(file, read_pos, bytesRemain, &bytesRead);
        bytesRemain -= bytesRead;
        read_pos += bytesRead;
    }

    /* Step 2: Set source buffer, buffer size and pixel draw callback function for PNG decoder handler. */
    err = PNG_openRAM(&s_pngimage, png_buffer_aligned, png_size, NULL); // PNGDraw);//TODO

    if (err != 0)
    {
        PRINTF("Error: PNG open file error\r\n");
        assert(false);
    }

    /* Step 3: Set buffer of generated image for PNG decoder handler, this is required when the pixel draw callback
     * function is null. */
    /* Place the output image to the center of the screen. */
    width  = PNG_getWidth(&s_pngimage);
    height = PNG_getHeight(&s_pngimage);
    APP_SetLayerConfig(width, height);

    PNG_setBuffer(&s_pngimage, buffer);

    /* Step 4: Start PNG decode. */
    err = PNG_decode(&s_pngimage, NULL, 0U);

    if (err != 0)
    {
        PRINTF("Error: PNG decode failed\r\n");
        assert(false);
    }

    /* Step 5: Close PNG decoder handler. */
    PNG_close(&s_pngimage);
    free(png_buffer);
}

static int MOUNT_SDCard(void)
{
    FRESULT error;
    const TCHAR driverName[3U] = {SDDISK + '0', ':', '/'};

    // clear FATFS manually
    memset((void *)&g_fileSystem, 0, sizeof(g_fileSystem));

    /* Wait for the card insert. */
    if (sdcardWaitCardInsert() != kStatus_Success)
    {
        PRINTF("Card not inserted.\r\n");
        return -1;
    }

    // Mount the driver
    if (f_mount(&g_fileSystem, driverName, 0))
    {
        PRINTF("Mount volume failed.\r\n");
        return -2;
    }

#if (FF_FS_RPATH >= 2U)
    if (f_chdrive((char const *)&driverName[0U]))
    {
        PRINTF("Change drive failed.\r\n");
        return -3;
    }
#endif

    // Open file to check
    error = f_open(&pngFil, _T("/000.png"), FA_OPEN_EXISTING);
    if (error != FR_OK)
    {
        PRINTF("No demo png file!\r\n");
        return -4;
    }

    f_close(&pngFil);

    return 0;
}

void APP_InitDisplay(void)
{
    uint8_t i;
    void *freeFb;

    status_t status;

    for (i = 0; i < APP_FB_NUM; i++)
    {
        memset(g_frameBuffer[i], 0, APP_FB_SIZE_BYTE);
        APP_PutFrameBuffer(g_frameBuffer[i]);
    }

    BOARD_PrepareDisplayController();

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        PRINTF("Display initialization failed\r\n");
        assert(false);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    fbInfo.pixelFormat = APP_FB_FORMAT;
    fbInfo.width       = APP_FB_WIDTH;
    fbInfo.height      = APP_FB_HEIGHT;
    fbInfo.startX      = APP_FB_START_X;
    fbInfo.startY      = APP_FB_START_Y;
    fbInfo.strideBytes = APP_FB_STRIDE_BYTE;
    if (kStatus_Success != g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo))
    {
        PRINTF("Error: Could not configure the display controller\r\n");
        assert(false);
    }

    g_dc.ops->setCallback(&g_dc, 0, APP_BufferSwitchOffCallback, NULL);

    s_newFrameShown = false;

    /* Get the first frame buffer and send to display controller. */
    freeFb = APP_GetFrameBuffer();

    if (freeFb == NULL)
    {
        PRINTF("Error: Could not get frame buffer\r\n");
        assert(false);
    }

    memset(freeFb, 0, APP_FB_SIZE_BYTE);
    g_dc.ops->setFrameBuffer(&g_dc, 0, freeFb);

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

/*!
 * @brief Main function
 */
int main(void)
{
    int i = 0;
    FRESULT error;
    char pngFileName[10];
    void *freeFb;
    uint32_t oldIntStat;

    BOARD_InitHardware();

    PRINTF("SD PNG demo start:\r\n");

    APP_InitDisplay();

    // Init the SD card
    if (0 != MOUNT_SDCard())
    {
        PRINTF("SD card mount error. Demo stopped!");
        return -1;
    }

    while (1)
    {
        // format the filename
        sprintf(pngFileName, "/%03d.png", i++);
        // open it
        error = f_open(&pngFil, pngFileName, FA_READ);
        if (error != FR_OK)
        {
            i = 0;
            continue;
        }

        /* Get free frame buffer and convert the png output to it. */
        do
        {
            oldIntStat = DisableGlobalIRQ();
            freeFb     = APP_GetFrameBuffer();
            EnableGlobalIRQ(oldIntStat);
        } while (NULL == freeFb);

        PRINTF("Decoding %s...", pngFileName);
        png_decode(&pngFil, freeFb);
        PRINTF("done!\r\n", pngFileName);
        f_close(&pngFil);

        DCACHE_CleanInvalidateByRange((uint32_t)freeFb, APP_FB_SIZE_BYTE);

        /*
         * Wait for the previous set frame buffer active.
         */
        while (s_newFrameShown == false)
        {
        }

        /* Now new frame is ready, pass it to LCDIF. */
        s_newFrameShown = false;
        g_dc.ops->setFrameBuffer(&g_dc, 0, freeFb);
    }
}

static status_t sdcardWaitCardInsert(void)
{
    BOARD_SD_Config(&g_sd, NULL, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);

    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_SetCardPower(&g_sd, false);
    /* wait card insert */
    if (SD_PollingCardInsert(&g_sd, kSD_Inserted) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* power on the card */
        SD_SetCardPower(&g_sd, true);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}

static void APP_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
    APP_PutFrameBuffer(switchOffBuffer);
}
