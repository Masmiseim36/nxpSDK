/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_lcdif.h"
#include "lcdif_support.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_BYTE_PER_PIXEL 4U
#define DEMO_PIXEL_FORMAT   kLCDIF_PixelFormatARGB8888

#define DEMO_IMG_HEIGHT         DEMO_PANEL_HEIGHT
#define DEMO_IMG_WIDTH          DEMO_PANEL_WIDTH
#define DEMO_IMG_BYTES_PER_LINE LCDIF_ALIGN_ADDR((DEMO_IMG_WIDTH * DEMO_BYTE_PER_PIXEL), LCDIF_FB_ALIGN)

#define DEMO_MAKE_COLOR(red, green, blue) (((0xFFUL) << 24U) | ((red) << 16U) | ((green) << 8U) | ((blue) << 0U))

#define DEMO_COLOR_RED   DEMO_MAKE_COLOR(0xFFUL, 0x0UL, 0x0UL)
#define DEMO_COLOR_GREY  DEMO_MAKE_COLOR(0x80UL, 0x80UL, 0x80UL)
#define DEMO_COLOR_WHITE DEMO_MAKE_COLOR(0xFFUL, 0xFFUL, 0xFFUL)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t s_frameBufferAddr = DEMO_FB0_ADDR;
static volatile bool s_frameDone  = false;
static uint32_t loops             = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_LCDIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = LCDIF_GetAndClearInterruptPendingFlags(DEMO_LCDIF);

    if (0 != (intStatus & kLCDIF_Display0FrameDoneInterrupt))
    {
        s_frameDone = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_FillFrameBuffer(void)
{
    uint32_t i, j;
    uint32_t(*fb)[DEMO_IMG_WIDTH] = (void *)s_frameBufferAddr;

    /* The video/graphic layer input buffer is chess board pattern. */
    for (i = 0; i < DEMO_IMG_HEIGHT; i++)
    {
        for (j = 0; j < DEMO_IMG_WIDTH; j++)
        {
            if ((i / (DEMO_IMG_HEIGHT / 4U)) == (j / (DEMO_IMG_WIDTH / 4U)))
            {
                ((uint32_t *)(((uint8_t *)fb) + DEMO_IMG_BYTES_PER_LINE * i))[j] = DEMO_COLOR_GREY;
            }
            else
            {
                ((uint32_t *)(((uint8_t *)fb) + DEMO_IMG_BYTES_PER_LINE * i))[j] = DEMO_COLOR_WHITE;
            }
        }
    }
}

void DEMO_LCDIF_Init(void)
{
    const lcdif_dpi_config_t dpiConfig = {
        .panelWidth    = DEMO_IMG_WIDTH,
        .panelHeight   = DEMO_IMG_HEIGHT,
        .hsw           = DEMO_HSW,
        .hfp           = DEMO_HFP,
        .hbp           = DEMO_HBP,
        .vsw           = DEMO_VSW,
        .vfp           = DEMO_VFP,
        .vbp           = DEMO_VBP,
        .polarityFlags = DEMO_POL_FLAGS,
        .format        = kLCDIF_Output24Bit,
    };

    lcdif_panel_config_t config;

    LCDIF_Init(DEMO_LCDIF);

    LCDIF_DpiModeSetConfig(DEMO_LCDIF, 0, &dpiConfig);

    LCDIF_PanelGetDefaultConfig(&config);

    LCDIF_SetPanelConfig(DEMO_LCDIF, 0, &config);

    if (kStatus_Success != BOARD_InitDisplayInterface())
    {
        PRINTF("Display interface initialize failed\r\n");

        while (1)
        {
        }
    }

    NVIC_EnableIRQ(DEMO_LCDIF_IRQn);

    LCDIF_EnableInterrupts(DEMO_LCDIF, kLCDIF_Display0FrameDoneInterrupt);
}

void DEMO_LCDIF_Colorkey(void)
{
    static lcdif_fb_config_t fbConfig;

    (void)memset(&fbConfig, 0, sizeof(fbConfig));

    /* Set the background layer as red, it performs as the destination layer. */
    LCDIF_SetFrameBufferBackground(DEMO_LCDIF, 0U, DEMO_COLOR_RED);

    DEMO_FillFrameBuffer();

    /* The stride shall be set to the exact byte count per line. */
    LCDIF_SetFrameBufferStride(DEMO_LCDIF, 0, DEMO_IMG_BYTES_PER_LINE);
    LCDIF_SetFrameBufferAddr(DEMO_LCDIF, 0, (uint32_t)s_frameBufferAddr);

    /* Set the video layer config, it performs as the source layer. */
    fbConfig.enable             = true;
    fbConfig.inOrder            = kLCDIF_PixelInputOrderARGB;
    fbConfig.format             = DEMO_PIXEL_FORMAT;
    fbConfig.alpha.enable       = false;
    fbConfig.colorkey.enable    = true;
    fbConfig.colorkey.lowValue  = DEMO_MAKE_COLOR(0x79, 0x79, 0x79);
    fbConfig.colorkey.highValue = DEMO_MAKE_COLOR(0x81, 0x81, 0x81);
    fbConfig.topLeftX           = 0U;
    fbConfig.topLeftY           = 0U;
    fbConfig.width              = DEMO_IMG_WIDTH;
    fbConfig.height             = DEMO_IMG_HEIGHT;
    fbConfig.rotateFlipMode     = kLCDIF_Rotate0;

    LCDIF_SetFrameBufferConfig(DEMO_LCDIF, 0, &fbConfig);

    LCDIF_Start(DEMO_LCDIF);

    while (1)
    {
        s_frameDone = false;

        if (fbConfig.colorkey.enable)
        {
            PRINTF("The color key mode is on.\r\n");
        }
        else
        {
            PRINTF("The color key mode is off.\r\n");
        }

        /* Wait for previous frame complete. */
        while (!s_frameDone)
        {
        }
        loops++;

        fbConfig.colorkey.enable = !fbConfig.colorkey.enable;
        LCDIF_SetFrameBufferConfig(DEMO_LCDIF, 0, &fbConfig);

        /* After updating any double buffered register, call this
           to let the change take effect in next v-blank. */
        LCDIF_SetUpdateReady(DEMO_LCDIF);

        /* Let the picture be displayed for 1s then change. */
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("LCDIF color key example start...\r\n");

    DEMO_LCDIF_Init();

    DEMO_LCDIF_Colorkey();

    while (1)
    {
    }
}
