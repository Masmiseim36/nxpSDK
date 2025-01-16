/*
 * Copyright  2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_elcdif.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_csi.h"

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_CSI_DMA_BUFFER_LINE 4

#define DEMO_WINDOW_ULX    (DEMO_CAMERA_WIDTH / 4)
#define DEMO_WINDOW_ULY    (DEMO_CAMERA_HEIGHT / 4)
#define DEMO_WINDOW_LRX    (DEMO_CAMERA_WIDTH * 3 / 4 - 1)
#define DEMO_WINDOW_LRY    (DEMO_CAMERA_HEIGHT * 3 / 4 - 1)
#define DEMO_WINDOW_HEIGHT (DEMO_WINDOW_LRY - DEMO_WINDOW_ULY + 1)
#define DEMO_WINDOW_WIDTH  (DEMO_WINDOW_LRX - DEMO_WINDOW_ULX + 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[DEMO_LCD_HEIGHT][DEMO_LCD_WIDTH], FRAME_BUFFER_ALIGN);

SDK_ALIGN(static uint16_t s_dmaBuffer[2][DEMO_CSI_DMA_BUFFER_LINE][DEMO_CAMERA_WIDTH], FRAME_BUFFER_ALIGN);

SDK_ALIGN(static uint8_t s_windowBuffer[DEMO_WINDOW_HEIGHT][DEMO_WINDOW_WIDTH], FRAME_BUFFER_ALIGN);

static volatile bool s_frameDone = false;

static csi_frag_handle_t s_csiHandle;

uint32_t lutData[ELCDIF_LUT_ENTRY_NUM];

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t DEMO_MakeLutData(uint8_t r, uint8_t g, uint8_t b)
{
#if (DEMO_LCDIF_DATA_BUS_WIDTH == 16)
    /* 16 bit data bus. */
    return (((r >> 3)) << 11) | (((g >> 2)) << 5) | (((b >> 3)) << 0);
#elif (DEMO_LCDIF_DATA_BUS_WIDTH == 24)
    /* 24-bit data bus. */
    return (r << 16) | (g << 8) | (b << 0);
#else
#error Data bus not supported
#endif
}

void DEMO_FillLutData(void)
{
    uint32_t i;

    for (i = 0; i < ELCDIF_LUT_ENTRY_NUM; i++)
    {
        lutData[i] = DEMO_MakeLutData(i, i, i);
    }
}

void CSI_TransferCallBack(CSI_Type *base, csi_frag_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_CSI_FrameDone)
    {
        s_frameDone = true;
    }
}

void DEMO_InitLCD(void)
{
    elcdif_rgb_mode_config_t lcdConfig = {
        .panelWidth    = DEMO_LCD_WIDTH,
        .panelHeight   = DEMO_LCD_HEIGHT,
        .hsw           = DEMO_HSW,
        .hfp           = DEMO_HFP,
        .hbp           = DEMO_HBP,
        .vsw           = DEMO_VSW,
        .vfp           = DEMO_VFP,
        .vbp           = DEMO_VBP,
        .polarityFlags = DEMO_LCD_POL_FLAGS,
        .pixelFormat   = kELCDIF_PixelFormatRAW8,
        .dataBus       = kELCDIF_DataBus8Bit,
        .bufferAddr    = (uint32_t)s_frameBuffer,
    };

    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

    ELCDIF_RgbModeInit(DEMO_ELCDIF, &lcdConfig);

    DEMO_FillLutData();

    /* Load the LUT data. */
    ELCDIF_UpdateLut(DEMO_ELCDIF, kELCDIF_Lut0, 0, lutData, ELCDIF_LUT_ENTRY_NUM);

    ELCDIF_EnableLut(DEMO_ELCDIF, true);

    ELCDIF_RgbModeStart(DEMO_ELCDIF);
}

void DEMO_InitCamera(void)
{
    const camera_config_t cameraConfig = {
        .pixelFormat                = kVIDEO_PixelFormatYUYV,
        .bytesPerPixel              = 2,
        .resolution                 = FSL_VIDEO_RESOLUTION(DEMO_CAMERA_WIDTH, DEMO_CAMERA_HEIGHT),
        .frameBufferLinePitch_Bytes = DEMO_CAMERA_WIDTH * 2,
        .interface                  = kCAMERA_InterfaceGatedClock,
        .controlFlags               = kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge | kCAMERA_VsyncActiveLow,
        .framePerSec                = 30,
    };

    csi_frag_config_t csiFragConfig = {
        .width               = DEMO_CAMERA_WIDTH,
        .height              = DEMO_CAMERA_HEIGHT,
        .polarityFlags       = kCSI_HsyncActiveHigh | kCSI_DataLatchOnRisingEdge | kCSI_VsyncActiveLow,
        .inputFormat         = kCSI_FragInputYUYV,
        .workMode            = kCSI_GatedClockMode,
        .dataBus             = kCSI_DataBus8Bit,
        .useExtVsync         = true,
        .dmaBufferAddr0      = (uint32_t)s_dmaBuffer[0],
        .dmaBufferAddr1      = (uint32_t)s_dmaBuffer[1],
        .dmaBufferLine       = DEMO_CSI_DMA_BUFFER_LINE,
        .isDmaBufferCachable = true,
    };

    CSI_FragModeInit(DEMO_CSI);

    CSI_FragModeCreateHandle(DEMO_CSI, &s_csiHandle, &csiFragConfig, CSI_TransferCallBack, NULL);

    if (kStatus_Success != CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig))
    {
        PRINTF("Camera init failed\r\n");
        while (1)
            ;
    }

    CAMERA_DEVICE_Start(&cameraDevice);
}

/*!
 * @brief Main function
 */
int main(void)
{
    csi_frag_window_t window = {
        .windowULX = DEMO_WINDOW_ULX,
        .windowULY = DEMO_WINDOW_ULY,
        .windowLRX = DEMO_WINDOW_LRX,
        .windowLRY = DEMO_WINDOW_LRY,
    };

    csi_frag_capture_config_t csiFragCaptureConfig = {
        .outputGrayScale = true,
        .buffer          = (uint32_t)s_windowBuffer,
        .window          = &window,
    };

    BOARD_InitHardware();

    PRINTF("CSI fragment mode example start...\r\n");

    DEMO_InitLCD();

    DEMO_InitCamera();

    while (1)
    {
        PRINTF("Press any key to capture image.\r\n");

        GETCHAR();

        s_frameDone = false;

        CSI_FragModeTransferCaptureImage(DEMO_CSI, &s_csiHandle, &csiFragCaptureConfig);

        while (!s_frameDone)
        {
        }

        /* Copy the captured image to LCD buffer. */
        for (uint32_t y = 0; y < DEMO_WINDOW_HEIGHT; y++)
        {
            memcpy(&(s_frameBuffer[y + DEMO_WINDOW_ULY][DEMO_WINDOW_ULX]), &s_windowBuffer[y][0], DEMO_WINDOW_WIDTH);
        }
    }
}
