/*
 * Copyright  2017-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_pxp.h"
#include "display_support.h"

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_PXP PXP

/* Use RGB565 or XRGB8888 */
#define USE_RGB565 0

#define APP_IMG_WIDTH DEMO_PANEL_WIDTH
#define APP_IMG_HEIGHT DEMO_PANEL_HEIGHT

/* PS input buffer is square. */
#if APP_IMG_WIDTH > APP_IMG_HEIGHT
#define APP_PS_SIZE APP_IMG_HEIGHT
#else
#define APP_PS_SIZE APP_IMG_WIDTH
#endif

#define APP_PS_ULC_X ((APP_IMG_WIDTH / 2) - (APP_PS_SIZE / 2))
#define APP_PS_ULC_Y ((APP_IMG_HEIGHT / 2) - (APP_PS_SIZE / 2))
#define APP_PS_LRC_X ((APP_IMG_WIDTH / 2) + (APP_PS_SIZE / 2) - 1U)
#define APP_PS_LRC_Y ((APP_IMG_HEIGHT / 2) + (APP_PS_SIZE / 2) - 1U)

#if USE_RGB565

typedef uint16_t pixel_t;
#define APP_BPP 2U /* Use 16-bit RGB565 format. */
#define APP_RED 0xF100U
#define APP_GREEN 0x07E0U
#define APP_BLUE 0x001FU
#define APP_PXP_PS_FORMAT kPXP_PsPixelFormatRGB565
#define APP_PXP_AS_FORMAT kPXP_AsPixelFormatRGB565
#define APP_PXP_OUT_FORMAT kPXP_OutputPixelFormatRGB565
#define APP_DC_FORMAT kVIDEO_PixelFormatRGB565

#else

typedef uint32_t pixel_t;
#define APP_BPP 4U /* Use 32-bit XRGB888 format. */
#define APP_RED 0x00FF0000U
#define APP_GREEN 0x0000FF00U
#define APP_BLUE 0x000000FFU
#define APP_PXP_PS_FORMAT kPXP_PsPixelFormatRGB888
#define APP_PXP_AS_FORMAT kPXP_AsPixelFormatRGB888
#define APP_PXP_OUT_FORMAT kPXP_OutputPixelFormatRGB888
#define APP_DC_FORMAT kVIDEO_PixelFormatXRGB8888

#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitInputBuffer(void);
static void APP_InitLcdif(void);
static void APP_InitPxp(void);
static void APP_Flip(void);
static void APP_BufferSwitchOffCallback(void *param, void *switchOffBuffer);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_ALIGN(static pixel_t s_BufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static pixel_t s_psBufferPxp[APP_PS_SIZE][APP_PS_SIZE], FRAME_BUFFER_ALIGN);

/* PXP Output buffer config. */
static pxp_output_buffer_config_t outputBufferConfig;

static uint8_t curLcdBufferIdx = 0;

/*
 * When new frame buffer sent to display, it might not be shown immediately.
 * Application could use callback to get new frame shown notification, at the
 * same time, when this flag is set, application could write to the older
 * frame buffer.
 */
static volatile bool s_newFrameShown = false;
static dc_fb_info_t fbInfo;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nPXP FLIP example start...\r\n");

    APP_InitInputBuffer();
    APP_InitPxp();
    APP_InitLcdif();

    APP_Flip();

    while (1)
    {
    }
}

static void APP_InitLcdif(void)
{
    status_t status;

    BOARD_PrepareDisplayController();

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        PRINTF("Display initialization failed\r\n");
        assert(0);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    fbInfo.pixelFormat = APP_DC_FORMAT;
    fbInfo.width       = APP_IMG_WIDTH;
    fbInfo.height      = APP_IMG_HEIGHT;
    fbInfo.strideBytes = APP_IMG_WIDTH * APP_BPP;
    g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

    g_dc.ops->setCallback(&g_dc, 0, APP_BufferSwitchOffCallback, NULL);

    s_newFrameShown = false;
    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_BufferLcd[curLcdBufferIdx]);

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

static void APP_InitPxp(void)
{
    PXP_Init(APP_PXP);

    /* PS configure. */
    const pxp_ps_buffer_config_t psBufferConfig = {
        .pixelFormat = APP_PXP_PS_FORMAT,
        .swapByte    = false,
        .bufferAddr  = (uint32_t)s_psBufferPxp,
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes  = APP_PS_SIZE * APP_BPP,
    };

    PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0U);

    PXP_SetProcessSurfaceBufferConfig(APP_PXP, &psBufferConfig);
    PXP_SetProcessSurfacePosition(APP_PXP, APP_PS_ULC_X, APP_PS_ULC_Y, APP_PS_LRC_X, APP_PS_LRC_Y);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(APP_PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    /* Output config. */
    outputBufferConfig.pixelFormat    = APP_PXP_OUT_FORMAT;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer0Addr    = (uint32_t)s_BufferLcd[0];
    outputBufferConfig.buffer1Addr    = 0U;
    outputBufferConfig.pitchBytes     = APP_IMG_WIDTH * APP_BPP;
    outputBufferConfig.width          = APP_IMG_WIDTH;
    outputBufferConfig.height         = APP_IMG_HEIGHT;
    PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(APP_PXP, false);
}

static void APP_Flip(void)
{
    uint8_t i;

    static const pxp_flip_mode_t flipModes[] = {
        kPXP_FlipDisable,
        kPXP_FlipHorizontal,
        kPXP_FlipBoth,
        kPXP_FlipVertical,
    };

    for (;;)
    {
        for (i = 0; i < ARRAY_SIZE(flipModes); i++)
        {
            /*
             * Wait for the new set frame buffer active, so that the older frame
             * buffer is inactive, then PXP could output to the older frame buffer.
             */
            while (s_newFrameShown == false)
            {
            }

            /* Switch to the other LCD buffer. */
            curLcdBufferIdx ^= 1U;

            /* Prepare next buffer for LCD. */
            PXP_SetRotateConfig(APP_PXP, kPXP_RotateOutputBuffer, kPXP_Rotate0, flipModes[i]);
            outputBufferConfig.buffer0Addr = (uint32_t)s_BufferLcd[curLcdBufferIdx];
            PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

            PXP_Start(APP_PXP);

            /* Wait for process complete. */
            while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP)))
            {
            }

            PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);

            /* Now new frame is ready, pass it to LCDIF. */
            s_newFrameShown = false;
            g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_BufferLcd[curLcdBufferIdx]);

            /* Show the active frame buffer for a while. */
            SDK_DelayAtLeastUs(1 * 1000 * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        }
    }
}

static void APP_InitInputBuffer(void)
{
    uint32_t i, j;

    /*
     * The PS input buffer is:
     *
     *  -----------------------------
     *  |             |             |
     *  |             |             |
     *  |   BLUE      |     RED     |
     *  |             |             |
     *  |             |             |
     *  |-------------+-------------|
     *  |                           |
     *  |                           |
     *  |           GREEN           |
     *  |                           |
     *  |                           |
     *  -----------------------------
     */
    for (i = 0; i < (APP_PS_SIZE / 2); i++)
    {
        for (j = 0; j < (APP_PS_SIZE / 2); j++)
        {
            s_psBufferPxp[i][j] = APP_BLUE;
        }

        for (; j < APP_PS_SIZE; j++)
        {
            s_psBufferPxp[i][j] = APP_RED;
        }
    }

    for (; i < APP_PS_SIZE; i++)
    {
        for (j = 0; j < APP_PS_SIZE; j++)
        {
            s_psBufferPxp[i][j] = APP_GREEN;
        }
    }

    memset(s_BufferLcd, 0x0U, sizeof(s_BufferLcd));
}

static void APP_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
}
