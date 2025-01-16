/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "app.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "display_support.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_jpegdec.h"
#include "board.h"
#include "sdmmc_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FB_BPP    2
#define APP_FB_FORMAT kVIDEO_PixelFormatRGB565

#define APP_FB_WIDTH  0x1e0
#define APP_FB_HEIGHT 0x110

/* Total number of pictures to decode */
#define APP_PIC_NUM 121

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t sdcardWaitCardInsert(void);
static void DEMO_FrameCompleteCallback(void *param, void *switchOffBuffer);

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
/* File system object */
AT_NONCACHEABLE_SECTION(static FATFS g_fileSystem);
/* File operation handle. */
AT_NONCACHEABLE_SECTION(static FIL jpgFil);
/* Frame buffer for decoded data. */
static uint32_t s_decodedImgAddr[4] = {DEMO_BUFFER0_ADDR, DEMO_BUFFER1_ADDR, DEMO_BUFFER2_ADDR, DEMO_BUFFER3_ADDR};
/* Frame buffer for converted data. */
static uint32_t s_frameAddr[2] = {DEMO_FB0_ADDR, DEMO_FB1_ADDR};
/* Flag for frame done. */
static volatile bool s_newFrameDone = false;
/* Total displayed frame count. */
static uint32_t g_totalFrameCount = 0U;
/* Buffer for JPEG file content for ping-pong descriptor. */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_fileContent[2][0x3000], 16U); /* The max size for each picture is 12k. */
/* File size. */
AT_NONCACHEABLE_SECTION(static uint32_t g_fileSize[2]);
/* 2 descriptors for 2 slots, each descriptor linked to itself. */
AT_NONCACHEABLE_SECTION_INIT(static jpegdec_descpt_t g_decoderDespt[2]) = {
    {.nextDescptAddr        = (uint32_t)&g_decoderDespt[0], /* Link the descriptor to itself. */
     .config.outBufAddr0    = 0U,
     .config.outBufAddr1    = 0U,
     .config.jpegBufAddr    = (uint32_t)&g_fileContent[0],
     .config.clearStreamBuf = true,
     .config.autoStart      = true},
    {.nextDescptAddr        = (uint32_t)&g_decoderDespt[1], /* Link the descriptor to itself. */
     .config.outBufAddr0    = 0U,
     .config.outBufAddr1    = 0U,
     .config.jpegBufAddr    = (uint32_t)&g_fileContent[1],
     .config.clearStreamBuf = true,
     .config.autoStart      = true}};

/*******************************************************************************
 * Code
 ******************************************************************************/
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

void Convert_yuv420_to_rgb565(uint16_t width, uint16_t height, uint32_t yAddr, uint32_t uvAddr, uint32_t rgbAddr)
{
    uint16_t *rgb = (void *)rgbAddr;
    int16_t r, g, b;
    uint8_t R, G, B, y, u, v;

    uint16_t i, j;

    for (i = 0U; i < height; i++)
    {
        for (j = 0U; j < width; j++)
        {
            y = *(((uint8_t *)yAddr) + width * i + j);
            u = *(((uint8_t *)uvAddr) + width * (i / 2) + j - (j % 2));
            v = *(((uint8_t *)uvAddr) + width * (i / 2) + j + 1 - (j % 2));
            r = y + 1402 * (v - 128) / 1000;
            g = y - (344 * (u - 128) + 714 * (v - 128)) / 1000;
            b = y + 1772 * (u - 128) / 1000;
            R = BYTECLIP(r);
            G = BYTECLIP(g);
            B = BYTECLIP(b);
            *rgb++ =
                (uint16_t)((((uint16_t)R & 0xF8) << 8U) | (((uint16_t)G & 0xFC) << 3U) | (((uint16_t)B & 0xF8) >> 3U));
        }
    }
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
    error = f_open(&jpgFil, _T("/000.jpg"), FA_OPEN_EXISTING);
    if (error != FR_OK)
    {
        PRINTF("No demo jpeg file!\r\n");
        return -4;
    }

    f_close(&jpgFil);

    return 0;
}

void APP_InitDisplay(void)
{
    status_t status;

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
    fbInfo.startX      = (DEMO_PANEL_WIDTH - APP_FB_WIDTH) / 2U;
    fbInfo.startY      = (DEMO_PANEL_HEIGHT - APP_FB_HEIGHT) / 2U;
    fbInfo.strideBytes = APP_FB_WIDTH * APP_FB_BPP;
    if (kStatus_Success != g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo))
    {
        PRINTF("Error: Could not configure the display controller\r\n");
        assert(false);
    }

    g_dc.ops->setCallback(&g_dc, 0, DEMO_FrameCompleteCallback, NULL);

    (void)memset((void *)s_frameAddr[0], 0, APP_FB_WIDTH * APP_FB_HEIGHT * APP_FB_BPP);

    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_frameAddr[0]);

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

void APP_JPEGDEC_IRQHandler(void)
{
    /* When current frame finished, set s_newFrameDone to true and enables the next descriptor. */
    if ((JPEGDEC_GetStatusFlags(APP_JPEGDEC, 0) & kJPEGDEC_DecodeCompleteFlag) != 0U)
    {
        JPEGDEC_ClearStatusFlags(APP_JPEGDEC, 0, kJPEGDEC_DecodeCompleteFlag);
    }

    /* When current frame finished, set s_newFrameDone to true and enables the next descriptor. */
    if ((JPEGDEC_GetStatusFlags(APP_JPEGDEC, 1) & kJPEGDEC_DecodeCompleteFlag) != 0U)
    {
        JPEGDEC_ClearStatusFlags(APP_JPEGDEC, 1, kJPEGDEC_DecodeCompleteFlag);
    }
    s_newFrameDone = true;
}

void APP_ReadNewJpegFile(uint32_t frameNb)
{
    char jpgFileName[10];

    /* Format the filename */
    sprintf(jpgFileName, "/%03d.jpg", (int)(frameNb % APP_PIC_NUM));

    /* open the file. */
    f_open(&jpgFil, jpgFileName, FA_READ);

    /* Read the file into buffer. */
    g_fileSize[frameNb % 2U] = f_size(&jpgFil);

    f_read(&jpgFil, g_fileContent[frameNb % 2U], g_fileSize[frameNb % 2U], NULL);

    /* Close the file. */
    f_close(&jpgFil);
}

/*!
 * @brief Main function
 */
int main(void)
{
    jpegdec_config_t config;

    BOARD_InitHardware();

    BOARD_InitSDCardPins();

    PRINTF("SD JPEG decoder demo start:\r\n");
    PRINTF("2 slots take turns to decode a series of pictures.\r\n");

    APP_InitDisplay();

    /* Init the SD card */
    if (0 != MOUNT_SDCard())
    {
        PRINTF("SD card mount error. Demo stopped!");
        return -1;
    }

    /* Init JPEG decoder module. */
    JPEGDEC_GetDefaultConfig(&config);
    config.slots = kJPEGDEC_Slot0 | kJPEGDEC_Slot1; /* Enable 2 slots for switch. */
    JPEGDEC_Init(APP_JPEGDEC, &config);

    /* Get the first frame. */
    APP_ReadNewJpegFile(0U);

    /* Configure the descriptors for 2 slots. */
    /* Set source buffer, buffer size. */
    JPEGDEC_SetJpegBuffer(&g_decoderDespt[0].config, g_fileContent[0], g_fileSize[0]);

    /* Set the buffer for decoded image buffer for JPEG decoder. */
    JPEGDEC_SetOutputBuffer(&g_decoderDespt[0].config, (uint8_t *)s_decodedImgAddr[0], (uint8_t *)s_decodedImgAddr[1]);
    JPEGDEC_SetOutputBuffer(&g_decoderDespt[1].config, (uint8_t *)s_decodedImgAddr[2], (uint8_t *)s_decodedImgAddr[3]);

    /* Parse header to get the frame width, height, pixel format and pixel depth. */
    JPEGDEC_ParseHeader(&g_decoderDespt[0].config);

    /* Each frame has the same info, copy the info from the first descriptor to the second descriptor. */
    g_decoderDespt[1].config.height      = g_decoderDespt[0].config.height;
    g_decoderDespt[1].config.width       = g_decoderDespt[0].config.width;
    g_decoderDespt[1].config.pixelDepth  = g_decoderDespt[0].config.pixelDepth;
    g_decoderDespt[1].config.pixelFormat = g_decoderDespt[0].config.pixelFormat;

    /* Set output pitch, clear stream buffer and auto start decode when descriptor is loaded. */
    JPEGDEC_SetDecodeOption(&g_decoderDespt[0].config, g_decoderDespt[0].config.width, true, true);
    JPEGDEC_SetDecodeOption(&g_decoderDespt[1].config, g_decoderDespt[1].config.width, true, true);

    /* Enable 2 slots' frame done interrupt. */
    NVIC_EnableIRQ(APP_JPEGDEC_IRQn);
    JPEGDEC_EnableInterrupts(APP_JPEGDEC, 0, kJPEGDEC_DecodeCompleteFlag);
    JPEGDEC_EnableInterrupts(APP_JPEGDEC, 1, kJPEGDEC_DecodeCompleteFlag);

    /* Set the descriptor addresses. */
    JPEGDEC_SetSlotNextDescpt(APP_JPEGDEC, 0, &g_decoderDespt[0]);
    JPEGDEC_SetSlotNextDescpt(APP_JPEGDEC, 1, &g_decoderDespt[1]);

    /* Enable the descriptor to start the decoding. */
    JPEGDEC_EnableSlotNextDescpt(APP_JPEGDEC, 0);

    while (1)
    {
        /* Wait for current decoding done  */
        while (s_newFrameDone == false)
        {
        }
        s_newFrameDone = false;

        /* Convert the decoded YUV format into RGB565 to display. */
        Convert_yuv420_to_rgb565(g_decoderDespt[g_totalFrameCount % 2U].config.width,
                                 g_decoderDespt[g_totalFrameCount % 2U].config.height,
                                 s_decodedImgAddr[g_totalFrameCount % 2U * 2],
                                 s_decodedImgAddr[g_totalFrameCount % 2U * 2 + 1], s_frameAddr[g_totalFrameCount % 2U]);

        /* Wait in case the last frame is not done, normally it shall already been shown. */
        while (s_newFrameShown == false)
        {
        }
        s_newFrameShown = false;

        /* Now new frame is ready, pass it to controller to display. */
        g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_frameAddr[g_totalFrameCount % 2U]);

        /* Increase the frame count and read next frame of JPEG picture. */
        g_totalFrameCount++;
        APP_ReadNewJpegFile(g_totalFrameCount);

        /* Update the next descriptor, mainly the file size. */
        JPEGDEC_SetJpegBuffer(&g_decoderDespt[g_totalFrameCount % 2U].config, g_fileContent[g_totalFrameCount % 2U],
                              g_fileSize[g_totalFrameCount % 2U]);

        /* Need to read the JPEG from card and set the size first, so here manually enable the next descriptor,
           In parctice if the picture is already ready, can use JPEGDEC_EnableLinkedDescpt to configure the descriptor
           to automatically switch. */
        JPEGDEC_EnableSlotNextDescpt(APP_JPEGDEC, g_totalFrameCount % 2U);
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

static void DEMO_FrameCompleteCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
}
