/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_pxp.h"
#include "fsl_elcdif.h"

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_ELCDIF LCDIF
#define APP_PXP PXP

#define APP_IMG_HEIGHT 272
#define APP_IMG_WIDTH 480
#define APP_HSW 41
#define APP_HFP 4
#define APP_HBP 8
#define APP_VSW 10
#define APP_VFP 4
#define APP_VBP 2
#define APP_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/* Display. */
#define LCD_DISP_GPIO GPIO1
#define LCD_DISP_GPIO_PIN 2
/* Back light. */
#define LCD_BL_GPIO GPIO2
#define LCD_BL_GPIO_PIN 31

#define APP_LCDIF_DATA_BUS kELCDIF_DataBus16Bit

/*
 * Frame buffer data alignment.
 * The PXP input buffer, output buffer, and LCDIF frame buffer address 64B align.
 */
#define FRAME_BUFFER_ALIGN 64

/* PS input buffer is square. */
#if APP_IMG_WIDTH > APP_IMG_HEIGHT
#define APP_PS_SIZE (APP_IMG_HEIGHT / 2U)
#else
#define APP_PS_SIZE (APP_IMG_WIDTH / 2U)
#endif

#define APP_PS_ULC_X 0U
#define APP_PS_ULC_Y 0U

#define APP_BPP 4U /* Use 24-bit RGB888 format. */

#ifndef APP_LCDIF_DATA_BUS
#define APP_LCDIF_DATA_BUS kELCDIF_DataBus24Bit
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitInputBuffer(void);
static void APP_InitLcdif(void);
static void APP_InitPxp(void);
static void APP_Scale(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferPxp[APP_PS_SIZE][APP_PS_SIZE], FRAME_BUFFER_ALIGN);

/* PXP Output buffer config. */
static pxp_output_buffer_config_t outputBufferConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        0,
    };

    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}

void BOARD_InitLcdifPixelClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31,
        .postDivider = 8,
        .numerator   = 0,
        .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}


int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();

    PRINTF("\r\nPXP Scale example start...\r\n");

    APP_InitInputBuffer();
    APP_InitPxp();
    APP_InitLcdif();

    APP_Scale();

    while (1)
    {
    }
}

static void APP_InitLcdif(void)
{
    const elcdif_rgb_mode_config_t config = {
        .panelWidth    = APP_IMG_WIDTH,
        .panelHeight   = APP_IMG_HEIGHT,
        .hsw           = APP_HSW,
        .hfp           = APP_HFP,
        .hbp           = APP_HBP,
        .vsw           = APP_VSW,
        .vfp           = APP_VFP,
        .vbp           = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr    = (uint32_t)s_psBufferLcd[0],
        .pixelFormat   = kELCDIF_PixelFormatXRGB8888,
        .dataBus       = APP_LCDIF_DATA_BUS,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);

    ELCDIF_RgbModeStart(APP_ELCDIF);
}

static void APP_InitPxp(void)
{
    PXP_Init(APP_PXP);

    /* PS configure. */
    const pxp_ps_buffer_config_t psBufferConfig = {
        .pixelFormat = kPXP_PsPixelFormatRGB888,
        .swapByte    = false,
        .bufferAddr  = (uint32_t)s_psBufferPxp,
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes  = APP_PS_SIZE * APP_BPP,
    };

    PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0U);

    PXP_SetProcessSurfaceBufferConfig(APP_PXP, &psBufferConfig);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(APP_PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    /* Output config. */
    outputBufferConfig.pixelFormat    = kPXP_OutputPixelFormatRGB888;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer0Addr    = (uint32_t)s_psBufferLcd[0];
    outputBufferConfig.buffer1Addr    = 0U;
    outputBufferConfig.pitchBytes     = APP_IMG_WIDTH * APP_BPP;
    outputBufferConfig.width          = APP_IMG_WIDTH;
    outputBufferConfig.height         = APP_IMG_HEIGHT;

    PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(APP_PXP, false);
}

static void APP_Scale(void)
{
    uint8_t curLcdBufferIdx = 1U;

    float downScaleStart = 0.5f;
    float downScaleEnd   = 4.0f;
    float downScaleStep  = 1.01f;
    float downScaleFact;
    uint16_t outputWidth, outputHeight;

    for (;;)
    {
        for (downScaleFact = downScaleStart; downScaleFact <= downScaleEnd; downScaleFact *= downScaleStep)
        {
            outputWidth  = (uint16_t)((float)APP_PS_SIZE / downScaleFact);
            outputHeight = (uint16_t)((float)APP_PS_SIZE / downScaleFact);

            /* Prepare next buffer for LCD. */
            PXP_SetProcessSurfaceScaler(APP_PXP, APP_PS_SIZE, APP_PS_SIZE, outputWidth, outputHeight);
            PXP_SetProcessSurfacePosition(APP_PXP, APP_PS_ULC_X, APP_PS_ULC_Y, APP_PS_ULC_X + outputWidth - 1U,
                                          APP_PS_ULC_Y + outputHeight - 1U);

            outputBufferConfig.buffer0Addr = (uint32_t)s_psBufferLcd[curLcdBufferIdx];

            PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

            PXP_Start(APP_PXP);

            /* Wait for process complete. */
            while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP)))
            {
            }

            PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);

            /* Now new frame is ready, pass it to LCDIF. */
            ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_psBufferLcd[curLcdBufferIdx]);

            /*
             * The new frame is not loaded untill current frame display finished. So
             * wait until current frame finished.
             */
            ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
            while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
            {
            }

            /* Switch to the other LCD buffer. */
            curLcdBufferIdx ^= 1U;
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
     *  |   BLUE      |   GREEN     |
     *  |             |             |
     *  |             |             |
     *  |-------------+-------------|
     *  |             |             |
     *  |             |             |
     *  |   WHITE     |    RED      |
     *  |             |             |
     *  |             |             |
     *  -----------------------------
     */
    for (i = 0; i < (APP_PS_SIZE / 2); i++)
    {
        for (j = 0; j < (APP_PS_SIZE / 2); j++)
        {
            s_psBufferPxp[i][j] = 0xFFU;
        }

        for (; j < APP_PS_SIZE; j++)
        {
            s_psBufferPxp[i][j] = 0xFF00U;
        }
    }

    for (; i < APP_PS_SIZE; i++)
    {
        for (j = 0; j < (APP_PS_SIZE / 2); j++)
        {
            s_psBufferPxp[i][j] = 0xFFFFFFU;
        }

        for (; j < APP_PS_SIZE; j++)
        {
            s_psBufferPxp[i][j] = 0xFF0000U;
        }
    }

    memset(s_psBufferLcd, 0, sizeof(s_psBufferLcd));
}
